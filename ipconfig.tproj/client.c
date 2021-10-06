/*
 * Copyright (c) 1999, 2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 * client.c
 * - client side program to talk to ipconfigd
 */
/* 
 * Modification History
 *
 * September, 1999 	Dieter Siegmund (dieter@apple.com)
 * - initial revision
 * July 31, 2000	Dieter Siegmund (dieter@apple.com)
 * - changed to add set, and new implementation of waitall
 * - removed waitif
 * February 24, 2003	Dieter Siegmund (dieter@apple.com)
 * - added support to retrieve information about the netboot (bsdp)
 *   packet stored in the device tree
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <servers/bootstrap.h>
#include <ctype.h>
#include <string.h>
#include <sysexits.h>

#include "machcompat.h"
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/bootp.h>
#include <arpa/inet.h>

#include "ipconfig_ext.h"
#include "ipconfig.h"
#include "dhcp_options.h"
#include "dhcplib.h"
#include "bsdp.h"
#include "bsdplib.h"
#include "ioregpath.h"
#include <SystemConfiguration/SystemConfiguration.h>
#include <SystemConfiguration/SCValidation.h>

typedef int func_t(port_t server, int argc, char * argv[]);
typedef func_t * funcptr_t;
char * progname = NULL;

#define STARTUP_KEY	CFSTR("Plugin:IPConfiguration")

#define SHADOW_MOUNT_PATH_COMMAND	"shadow_mount_path"
#define SHADOW_FILE_PATH_COMMAND	"shadow_file_path"
#define MACHINE_NAME_COMMAND		"machine_name"

static void
on_alarm(int sigraised)
{
    exit(0);
}

#define WAIT_ALL_DEFAULT_TIMEOUT	60
#define WAIT_ALL_MAX_TIMEOUT		120

static void
key_appeared(SCDynamicStoreRef session, CFArrayRef changes, void * arg)
{
    exit(0);
}

static int
S_wait_all(port_t server, int argc, char * argv[])
{
    CFMutableArrayRef	keys;
    SCDynamicStoreRef 	session;
    CFRunLoopSourceRef	rls;
    unsigned long	t = WAIT_ALL_DEFAULT_TIMEOUT;
    CFPropertyListRef	value;
    struct itimerval 	v;

    if (argc > 0) {
	t = strtoul(argv[0], 0, 0);
	if (t > WAIT_ALL_MAX_TIMEOUT) {
	    t = WAIT_ALL_MAX_TIMEOUT;
	}
    }

    session = SCDynamicStoreCreate(NULL, CFSTR("ipconfig command"), 
				   key_appeared, NULL);
    if (session == NULL) {
	fprintf(stderr, "SCDynamicStoreCreate failed: %s\n", 
		SCErrorString(SCError()));
	return (0);
    }
    keys = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
    CFArrayAppendValue(keys, STARTUP_KEY);
    SCDynamicStoreSetNotificationKeys(session, keys, NULL);
    CFRelease(keys);

    rls = SCDynamicStoreCreateRunLoopSource(NULL, session, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), rls, kCFRunLoopDefaultMode);
    CFRelease(rls);

    signal(SIGALRM, on_alarm);
    bzero(&v, sizeof(v));
    v.it_value.tv_sec = t;
    if (setitimer(ITIMER_REAL, &v, NULL) < 0) {
	perror("setitimer");
	return (0);
    }
    value = SCDynamicStoreCopyValue(session, STARTUP_KEY);
    if (value == NULL) {
	CFRunLoopRun();
	return (0);
    }
    CFRelease(session);
    return (0);
}

#if 0
static int
S_wait_if(port_t server, int argc, char * argv[])
{
    return (0);
    if_name_t		name;
    kern_return_t	status;

    strcpy(name, argv[0]);
    status = ipconfig_wait_if(server, name);
    if (status == KERN_SUCCESS) {
	return (0);
    }
    fprintf(stderr, "wait if %s failed, %s\n", name,
	    mach_error_string(status));
    return (1);
}
#endif 0

static int
S_bsdp_get_packet(port_t server, int argc, char * argv[])
{
    CFDictionaryRef	chosen = NULL;
    struct dhcp *	dhcp;
    int			length;
    CFDataRef		response = NULL;
    int			ret = 1;

    if (getuid() != 0) {
	return (EX_NOPERM);
    }

    chosen = myIORegistryEntryCopyValue("IODeviceTree:/chosen");
    if (chosen == NULL) {
	goto done;
    }
    response = CFDictionaryGetValue(chosen, CFSTR("bsdp-response"));
    if (isA_CFData(response) == NULL) {
	response = CFDictionaryGetValue(chosen, CFSTR("bootp-response"));
	if (isA_CFData(response) == NULL) {
	    goto done;
	}
    }
    dhcp = (struct dhcp *)CFDataGetBytePtr(response);
    length = CFDataGetLength(response);
    bsdp_print_packet(dhcp, length, 0);
    ret = 0;
 done:
    return (ret);
}

static int
S_bsdp_option(port_t server, int argc, char * argv[])
{
    CFDictionaryRef	chosen = NULL;
    void *		data = NULL;
    int			data_len;
    struct dhcp *	dhcp;
    char		err[256];
    int			length;
    dhcpol_t		options;
    CFDataRef		response = NULL;
    int			ret = 1;
    int			tag = 0;
    dhcpol_t		vendor_options;
    int			vendor_tag = 0;

    if (getuid() != 0) {
	return (EX_NOPERM);
    }

    chosen = myIORegistryEntryCopyValue("IODeviceTree:/chosen");
    if (chosen == NULL) {
	goto done;
    }
    response = CFDictionaryGetValue(chosen, CFSTR("bsdp-response"));
    if (isA_CFData(response) == NULL) {
	response = CFDictionaryGetValue(chosen, CFSTR("bootp-response"));
	if (isA_CFData(response) == NULL) {
	    goto done;
	}
    }
    dhcp = (struct dhcp *)CFDataGetBytePtr(response);
    length = CFDataGetLength(response);
    if (dhcpol_parse_packet(&options, dhcp, length, err) == FALSE) {
	goto done;
    }
    if (strcmp(argv[0], SHADOW_MOUNT_PATH_COMMAND) == 0) {
	tag = dhcptag_vendor_specific_e;
	vendor_tag = bsdptag_shadow_mount_path_e;
    }
    else if (strcmp(argv[0], SHADOW_FILE_PATH_COMMAND) == 0) {
	tag = dhcptag_vendor_specific_e;
	vendor_tag = bsdptag_shadow_file_path_e;
    }
    else if (strcmp(argv[0], MACHINE_NAME_COMMAND) == 0) {
	tag = dhcptag_vendor_specific_e;
	vendor_tag = bsdptag_machine_name_e;
    }
    else {
	tag = atoi(argv[0]);
	if (argc == 2) {
	    vendor_tag = atoi(argv[1]);
	}
    }
    if (tag == dhcptag_vendor_specific_e && vendor_tag != 0) {
	if (dhcpol_parse_vendor(&vendor_options, &options, err) == FALSE) {
	    goto done;
	}
	data = dhcpol_get(&vendor_options, vendor_tag, &data_len);
	if (data != NULL) {
	    dhcptype_print(bsdptag_type(vendor_tag), data, data_len);
	    ret = 0;
	}
	dhcpol_free(&vendor_options);
    }
    else {
	dhcptag_info_t * entry;

	entry = dhcptag_info(tag);
	if (entry == NULL) {
	    goto done;
	}
	data = dhcpol_get(&options, tag, &data_len);
	if (data != NULL) {
	    dhcptype_print(entry->type, data, data_len);
	    ret = 0;
	}
    }
 done:
    if (data != NULL) {
	free(data);
    }
    if (chosen != NULL) {
	CFRelease(chosen);
    }
    return (ret);
}

static int
S_if_addr(port_t server, int argc, char * argv[])
{
    struct in_addr	ip;
    if_name_t		name;
    kern_return_t	status;

    strcpy(name, argv[0]);
    status = ipconfig_if_addr(server, name, (ip_address_t *)&ip);
    if (status == KERN_SUCCESS) {
	printf("%s\n", inet_ntoa(ip));
	return (0);
    }
    fprintf(stderr, "get if addr %s failed, %s\n", name,
	    mach_error_string(status));
    return (1);
}

static int
S_if_count(port_t server, int argc, char * argv[])
{
    int 		count = 0;
    kern_return_t	status;

    status = ipconfig_if_count(server, &count);
    if (status == KERN_SUCCESS) {
	printf("%d\n", count);
	return (0);
    }
    fprintf(stderr, "get if count failed, %s\n", mach_error_string(status));
    return (1);
}

static int
S_get_option(port_t server, int argc, char * argv[])
{
    char		buf[1024];
    inline_data_t 	data;
    unsigned int 	data_len = sizeof(data);
    if_name_t		name;
    char		err[1024];
    int			tag;
    kern_return_t	status;

    strcpy(name, argv[0]);

    tag = dhcptag_with_name(argv[1]);
    if (tag == -1)
	tag = atoi(argv[1]);
    status = ipconfig_get_option(server, name, tag, data, &data_len);
    if (status == KERN_SUCCESS) {
	if (dhcptag_to_str(buf, tag, data, data_len, err)) {
	    printf("%s\n", buf);
	    return (0);
	}
	fprintf(stderr, "couldn't convert the option, %s\n",
		err);
    }
    else {
	fprintf(stderr, "ipconfig_get_option failed, %s\n", 
		mach_error_string(status));
    }
    return (1);
}

static int
S_get_packet(port_t server, int argc, char * argv[])
{
    inline_data_t 	data;
    unsigned int 	data_len = sizeof(data);
    if_name_t		name;
    kern_return_t	status;

    strcpy(name, argv[0]);
    status = ipconfig_get_packet(server, name, data, &data_len);
    if (status == KERN_SUCCESS) {
	dhcp_print_packet((struct dhcp *)data, data_len);
    }
    return (1);
}

static __inline__ boolean_t
ipconfig_method_from_string(const char * m, ipconfig_method_t * method)
{
    if (strcmp(m, "BOOTP") == 0) {
	*method = ipconfig_method_bootp_e;
    }
    else if (strcmp(m, "DHCP") == 0) {
	*method = ipconfig_method_dhcp_e;
    }
    else if (strcmp(m, "MANUAL") == 0) {
	*method = ipconfig_method_manual_e;
    }
    else if (strcmp(m, "INFORM") == 0) {
	*method = ipconfig_method_inform_e;
    }
    else if (strcmp(m, "LINKLOCAL") == 0) {
	*method = ipconfig_method_linklocal_e;
    }
    else if (strcmp(m, "NONE") == 0) {
	*method = ipconfig_method_none_e;
    }
    else {
	return (FALSE);
    }
    return (TRUE);
}

static int
S_set(port_t server, int argc, char * argv[])
{
    char			buf[512];	
    ipconfig_method_data_t *	data = (ipconfig_method_data_t *)buf;
    int				data_len = 0;
    if_name_t			name;
    char *			method_name;
    ipconfig_method_t		method;
    kern_return_t		status;
    ipconfig_status_t		ipstatus = ipconfig_status_success_e;

    strcpy(name, argv[0]);
    method_name = argv[1];
    if (ipconfig_method_from_string(method_name, &method) == FALSE) {
	fprintf(stderr, "ipconfig: set: method '%s' unknown\n",
		argv[1]);
	fprintf(stderr, 
		"method is one of MANUAL, INFORM, BOOTP, DHCP, or NONE\n");
	exit(1);
    }
    argv += 2;
    argc -= 2;
    switch (method) {
      case ipconfig_method_inform_e:
      case ipconfig_method_manual_e: {
	  int i;
	  if (argc < 2 || (argc & 0x1)) {
	      fprintf(stderr, "usage: ipconfig set en0 %s <ip> <mask>\n",
		      method_name);
	      exit(1);
	  }
	  bzero(buf, sizeof(buf));
	  data->n_ip = argc / 2;
	  for (i = 0; i < data->n_ip; i++) {
	      if (inet_aton(argv[i * 2], &data->ip[i].addr) == 0) {
		  fprintf(stderr, "Invalid IP address %s\n", argv[i * 2]);
		  exit(1);
	      }
	      if (inet_aton(argv[i * 2 + 1], &data->ip[i].mask) == 0) {
		  fprintf(stderr, "Invalid IP mask %s\n", argv[i * 2]);
		  exit(1);
	      }
	  }
	  data_len = sizeof(*data) + data->n_ip * sizeof(data->ip[0]);
	  break;
      }
      default:
	  if (argc) {
	      fprintf(stderr, "too many arguments for method\n");
	      exit(1);
	  }
	  break;
    }
    status = ipconfig_set(server, name, method, (void *)data, data_len,
			  &ipstatus);
    if (status != KERN_SUCCESS) {
	mach_error("ipconfig_set failed", status);
	exit(1);
    }
    if (ipstatus != ipconfig_status_success_e) {
	fprintf(stderr, "ipconfig_set %s %s failed: %s\n",
		name, method_name, ipconfig_status_string(ipstatus));
    }
    return (1);
}

static struct command_info {
    char *	command;
    funcptr_t	func;
    int		argc;
    char *	usage;
    int		display;
    int		no_server;
} commands[] = {
    { "waitall", S_wait_all, 0, "[ timeout secs ]", 1, 1 },
    { "getifaddr", S_if_addr, 1, "<interface name>", 1, 0 },
#if 0
    { "waitif", S_wait_if, 1, " <interface name>", 1, 0 },
#endif 0
    { "ifcount", S_if_count, 0, "", 1, 0 },
    { "getoption", S_get_option, 2, 
      " <interface name | \"\" > <option name> | <option code>", 1, 0 },
    { "getpacket", S_get_packet, 1, " <interface name>", 1, 0 },
    { "set", S_set, 2, 
      " <interface name> < BOOTP | MANUAL | DHCP | INFORM | NONE > <method args>", 1, 0 },
    { "netbootoption", S_bsdp_option, 1, "<option> [<vendor option>] | " 
      SHADOW_MOUNT_PATH_COMMAND " | " SHADOW_FILE_PATH_COMMAND
      " | " MACHINE_NAME_COMMAND, 0, 1 },
    { "netbootpacket", S_bsdp_get_packet, 0, "", 0, 1 },
    { NULL, NULL, NULL },
};

void
usage()
{
    int i;
    fprintf(stderr, "usage: %s <command> <args>\n", progname);
    fprintf(stderr, "where <command> is one of ");
    for (i = 0; commands[i].command; i++) {
	if (commands[i].display) {
	    fprintf(stderr, "%s%s",  i == 0 ? "" : ", ",
		    commands[i].command);
	}
    }
    fprintf(stderr, "\n");
    exit(1);
}

static struct command_info *
S_lookup_command(char * cmd, int argc)
{
    int i;

    for (i = 0; commands[i].command; i++) {
	if (strcmp(cmd, commands[i].command) == 0) {
	    if (argc < commands[i].argc) {
		if (commands[i].display) {
		    fprintf(stderr, "usage: %s %s\n", commands[i].command,
			    commands[i].usage ? commands[i].usage : "");
		}
		exit(1);
	    }
	    return commands + i;
	}
    }
    return (NULL);
}

int
main(int argc, char * argv[])
{
    boolean_t			active = FALSE;
    struct command_info *	command;
    port_t			server;
    kern_return_t		status;

    progname = argv[0];
    if (argc < 2)
	usage();

    argv++; argc--;

#if 0
    {
	struct in_addr	dhcp_ni_addr;
	char		dhcp_ni_tag[128];
	int		len;
	kern_return_t	status;

	len = sizeof(dhcp_ni_addr);
	status = ipconfig_get_option(server, IPCONFIG_IF_ANY,
				     112, (void *)&dhcp_ni_addr, &len);
	if (status != KERN_SUCCESS) {
	    fprintf(stderr, "get 112 failed, %s\n", mach_error_string(status));
	}
	else {
	    fprintf(stderr, "%d bytes:%s\n", len, inet_ntoa(dhcp_ni_addr));
	}
	len = sizeof(dhcp_ni_tag) - 1;
	status = ipconfig_get_option(server, IPCONFIG_IF_ANY,
				     113, (void *)&dhcp_ni_tag, &len);
	if (status != KERN_SUCCESS) {
	    fprintf(stderr, "get 113 failed, %s\n", mach_error_string(status));
	}
	else {
	    dhcp_ni_tag[len] = '\0';
	    fprintf(stderr, "%d bytes:%s\n", len, dhcp_ni_tag);
	}
	
    }
#endif 0
    command = S_lookup_command(argv[0], argc - 1);
    if (command == NULL)
	usage();
    argv++; argc--;
    if (command->no_server == 0) {
	status = ipconfig_server_port(&server, &active);
	if (active == FALSE) {
	    fprintf(stderr, "ipconfig server not active\n");
	    /* start it maybe??? */
	    exit(1);
	}
	if (status != BOOTSTRAP_SUCCESS) {
	    mach_error("ipconfig_server_port failed", status);
	    exit(1);
	}
    }
    exit ((*command->func)(server, argc, argv));
}
