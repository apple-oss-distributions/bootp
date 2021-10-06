/*
 * Copyright (c) 1999-2003 Apple Computer, Inc. All rights reserved.
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
 * genoptionfiles.c
 * - generates dhcp-related header files
 * - its purpose is to avoid the inevitable errors that arise when trying 
 *   to keep separate but related tables in synch by hand
 */

/*
 * Modification History
 * 12 Dec 1997	Dieter Siegmund (dieter@apple)
 *		- created
 */

#include <mach/boolean.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char copyright_string[] =
"/*\n"
" * Copyright (c) 1999-2003 Apple Computer, Inc. All rights reserved.\n"
" *\n"
" * @APPLE_LICENSE_HEADER_START@\n"
" * \n"
" * The contents of this file constitute Original Code as defined in and\n"
" * are subject to the Apple Public Source License Version 1.1 (the\n"
" * \"License\").  You may not use this file except in compliance with the\n"
" * License.  Please obtain a copy of the License at\n"
" * http://www.apple.com/publicsource and read it before using this file.\n"
" * \n"
" * This Original Code and all software distributed under the License are\n"
" * distributed on an \"AS IS\" basis, WITHOUT WARRANTY OF ANY KIND, EITHER\n"
" * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,\n"
" * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,\n"
" * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the\n"
" * License for the specific language governing rights and limitations\n"
" * under the License.\n"
" * \n"
" * @APPLE_LICENSE_HEADER_END@\n"
" */\n";


#define COMMENT		0xffff
#define END		0xfffe

#define LAST_TAG	255

struct {
    unsigned char *		name;
    unsigned int		size;
    unsigned char *		multiple_of;

} types[] = {
    { "none",		0,	0 },
    { "opaque",		0,	0 },
    { "bool",		1,	0 },
    { "uint8",		1,	0 },
    { "uint16",		2,	0 },
    { "uint32",		4,	0 },
    { "int32",		4,	0 },
    { "uint8_mult",	1,	"uint8" },
    { "uint16_mult",	2,	"uint16" },
    { "string",		0,	0 },
    { "ip",		4,	0 },
    { "ip_mult",	4,	"ip" },
    { "ip_pairs",	8,	"ip" },
    { 0, 0, 0 },
};

struct {
    int 			code;
    unsigned char *		type;
    unsigned char *		name;
} option[] = {
    { COMMENT, "/* rfc 1497 vendor extensions: 0..18, 255 */", 0 },
    /* value	name 		type */
    { 0,	"none",		"pad" },
    { 255,	"none",		"end" },
    { 1,	"ip",		"subnet_mask" },
    { 2,	"int32",	"time_offset" },
    { 3,	"ip_mult",	"router" },
    { 4,	"ip_mult",	"time_server" },
    { 5,	"ip_mult",	"name_server" },
    { 6,	"ip_mult",	"domain_name_server" },
    { 7,	"ip_mult",	"log_server" },
    { 8,	"ip_mult",	"cookie_server" },
    { 9,	"ip_mult",	"lpr_server" },
    { 10,	"ip_mult",	"impress_server" },
    { 11,	"ip_mult",	"resource_location_server" },
    { 12,	"string",	"host_name" },
    { 13,	"uint16",	"boot_file_size" },
    { 14,	"string",	"merit_dump_file" },
    { 15,	"string",	"domain_name" },
    { 16,	"ip",		"swap_server" },
    { 17,	"string",	"root_path" },
    { 18,	"string",	"extensions_path" },
    { COMMENT,  "/* ip layer parameters per host: 19..25 */", 0 },
    { 19,	"bool",		"ip_forwarding" },
    { 20,	"bool",		"non_local_source_routing" },
    { 21,	"ip_pairs",	"policy_filter" },
    { 22,	"uint16",	"max_dgram_reassembly_size" },
    { 23,	"uint8",	"default_ip_time_to_live" },
    { 24,	"uint32",	"path_mtu_aging_timeout" },
    { 25,	"uint16_mult",	"path_mtu_plateau_table" },
    { COMMENT, "/* ip layer parameters per interface: 26..33 */", 0 },
    { 26,	"uint16",	"interface_mtu" },
    { 27,	"bool",		"all_subnets_local" },
    { 28,	"ip",		"broadcast_address" },
    { 29,	"bool",		"perform_mask_discovery" },
    { 30,	"bool",		"mask_supplier" },
    { 31,	"bool",		"perform_router_discovery" },
    { 32,	"ip",		"router_solicitation_address" },
    { 33,	"ip_pairs",	"static_route" },
    { 34,	"bool",		"trailer_encapsulation" },
    { 35,	"uint32",	"arp_cache_timeout" },
    { 36,	"bool",		"ethernet_encapsulation" },
    { COMMENT, "/* tcp parameters: 37..39 */", 0 },
    { 37,	"uint8",	"default_ttl" },
    { 38,	"uint32",	"keepalive_interval" },
    { 39,	"bool",		"keepalive_garbage" },
    { COMMENT, "/* application & service parameters: 40..49, 64, 65, 68..76, 78, 79, 95 */", 0},
    { 40,	"string",	"nis_domain" },
    { 41,	"ip_mult",	"nis_servers" },
    { 42,	"ip_mult",	"network_time_protocol_servers" },
    { 43,	"opaque",	"vendor_specific" },
    { 44,	"ip_mult",	"nb_over_tcpip_name_server" },
    { 45,	"ip_mult",	"nb_over_tcpip_dgram_dist_server" },
    { 46,	"uint8",	"nb_over_tcpip_node_type" },
    { 47,	"string",	"nb_over_tcpip_scope" },
    { 48,	"ip_mult",	"x_windows_font_server" },
    { 49,	"ip_mult",	"x_windows_display_manager" },
    { 64,	"string",	"nis_plus_domain" },
    { 65,	"ip_mult",	"nis_plus_servers" },
    { 68,	"ip_mult",	"mobile_ip_home_agent" },
    { 69,	"ip_mult",	"smtp_server" },
    { 70,	"ip_mult",	"pop3_server" },
    { 71,	"ip_mult",	"nntp_server" },
    { 72,	"ip_mult",	"default_www_server" },
    { 73,	"ip_mult",	"default_finger_server" },
    { 74,	"ip_mult",	"default_irc_server" },
    { 75,	"ip_mult",	"streettalk_server" },
    { 76,	"ip_mult",	"stda_server" },
    { 78, 	"opaque", 	"slp_directory_agent" },
    { 79, 	"opaque", 	"slp_service_scope" },
    { 95,	"string",	"ldap_url" },
    { 108,	"string", 	"swap_path" },
    { 114,	"string", 	"url" },
    { COMMENT, "/* dhcp-specific extensions: 50..61, 66, 67 */", 0 },
    { 50,	"ip",		"requested_ip_address" },
    { 51,	"uint32",	"lease_time" },
    { 52,	"uint8",	"option_overload" },
    { 53,	"uint8",	"dhcp_message_type" },
    { 54,	"ip",		"server_identifier" },
    { 55,	"uint8_mult",	"parameter_request_list" },
    { 56,	"string",	"message" },
    { 57,	"uint16",	"max_dhcp_message_size" },
    { 58,	"uint32",	"renewal_t1_time_value" },
    { 59,	"uint32",	"rebinding_t2_time_value" },
    { 60,	"string",	"vendor_class_identifier" },
    { 61,	"uint8_mult",	"client_identifier" },
    { 66,	"ip_mult",	"tftp_server_name" },
    { 67,	"string",	"bootfile_name" },
    { COMMENT, "/* netinfo parent tags: 112, 113 */", 0 },
    { 112, 	"ip_mult",	"netinfo_server_address" },
    { 113, 	"string",	"netinfo_server_tag" },
    { COMMENT, "/* ad-hoc network disable option */", 0 },
    { 116,	"uint8",	"auto_configure" },
    { COMMENT, "/* proxy auto discovery */", 0 }, /* http://www.wpad.com/draft-ietf-wrec-wpad-01.txt */
    { 252,	"string",	"proxy_auto_discovery_url" },
    { END, 0, 0 },
};

int
find_option(int code)
{
    int i;
    for (i = 0; option[i].code != END; i++) {
	if (option[i].code == code)
	    return (i);
    }
    return (-1);
}

unsigned char *
make_option(unsigned char * name)
{
    static unsigned char buf[80];

    sprintf(buf, "dhcptag_%s_e", name);
    return (buf);
}

unsigned char *
make_type(unsigned char * name)
{
    static unsigned char buf[80];
    sprintf(buf, "dhcptype_%s_e", name);
    return (buf);
}

static void
S_upper_case(unsigned char * name)
{
    while (*name) {
	*name = toupper(*name);
	name++;
    }
}

unsigned char *
make_option_define(unsigned char * name)
{
    static unsigned char buf[80];
    sprintf(buf, "DHCPTAG_%s", name);
    S_upper_case(buf);
    return (buf);
}

static void
print_copyright_header(const char * program, const char * arg)
{
    
    printf("%s", copyright_string);
    printf("/*\n"
	   " * This file was auto-generated by %s %s, do not edit!\n"
	   " */\n", program, arg);
}

enum {
    gen_table,
    gen_tag,
    gen_type,
    gen_mandoc,
};

int
main(int argc, char * argv[])
{
    int 		i;
    int 		which;

    if (argc != 2)
	exit(0);

    if (strcmp(argv[1], "-table") == 0) {
	which = gen_table;
    }
    else if (strcmp(argv[1], "-dhcptag") == 0) {
	which = gen_tag;
    }
    else if (strcmp(argv[1], "-dhcptype") == 0) {
	which = gen_type;
    }
    else if (strcmp(argv[1], "-mandoc") == 0) {
	which = gen_mandoc;
    }
    else
	exit(1);

    switch (which) {
    case gen_tag:
	print_copyright_header(argv[0], argv[1]);
	printf("#ifndef _S_DHCP_TAG\n"
	       "#define _S_DHCP_TAG\n"
	       "typedef enum {\n");
	for (i = 0; option[i].code != END; i++) {
	    if (option[i].code == COMMENT)
		printf("\n    %s\n", option[i].type);
	    else {
		printf("    %-35s\t= %d,\n", make_option(option[i].name), 
		       option[i].code);
	    }
	}
	printf("\n    /* undefined */\n");

	for (i = 0; i <= 127; i++) {
	    if (find_option(i) == -1) {
		char buf[32];

		sprintf(buf, "%d", i);
		printf("    %-35s\t= %d,\n", make_option(buf), i);
	    }
	}
	printf("\n    /* site-specific 128..254 */\n");
	for (i = 128; i <= 254; i++) {
	    char buf[32];
	    sprintf(buf, "%d", i);
	    printf("    %-35s\t= %d,\n", make_option(buf), i);
	}
	printf("} dhcptag_t;\n\n");
    
	printf("\n/* defined tags */\n");
	for (i = 0; option[i].code != END; i++) {
	    if (option[i].code != COMMENT) {
		printf("#define %-35s\t\"%s\"\n", 
		       make_option_define(option[i].name),
		       option[i].name);
	    }
	}
	printf("\n/* undefined */\n");
	for (i = 0; i <= 127; i++) {
	    if (find_option(i) == -1) {
		char buf[32];

		sprintf(buf, "%d", i);
		printf("#define %-35s\t\"%s\"\n", 
		       make_option_define(buf), buf);
	    }
	}
	printf("\n/* site-specific 128..254 */\n");
	for (i = 128; i <= 254; i++) {
	    char buf[32];
	    sprintf(buf, "%d", i);
	    printf("#define %-35s\t\"%s\"\n", 
		   make_option_define(buf), buf);
	}
	printf("#endif _S_DHCP_TAG\n");
	break;

    case gen_type:
	print_copyright_header(argv[0], argv[1]);
	printf("#ifndef _S_DHCP_TYPE\n"
	       "#define _S_DHCP_TYPE\n");
	printf("\ntypedef enum {\n");
	for (i = 0; types[i].name; i++) {
	    if (i == 0) {
		printf("    %-20s\t = %d,\n", make_type("first"), i);
		printf("    %-20s\t =", make_type(types[i].name));
		printf(" %s,\n", make_type("first"));
	    }
	    else
		printf("    %-20s\t = %d,\n", make_type(types[i].name), i);
	}
	printf("    %-20s\t =", make_type("last"));
	printf(" %s,\n", make_type(types[i ? (i - 1) : 0].name));
    
	printf("} dhcptype_t;\n\n");
	printf("typedef struct {\n"
	       "    dhcptype_t	type;\n"
	       "    unsigned char *	name;\n"
	       "} dhcptag_info_t;\n\n");
	printf("typedef struct {\n"
	       "    int		size;  /* in bytes */\n"
	       "    int		multiple_of; /* type of element */\n"
	       "    unsigned char * name;\n"
	       "} dhcptype_info_t;\n\n");
	printf("#endif _S_DHCP_TYPE\n");
	break;

    case gen_table:
	print_copyright_header(argv[0], argv[1]);
	printf("#ifndef _S_DHCP_PARSE_TABLE\n"
	       "#define _S_DHCP_PARSE_TABLE\n");
	printf("static dhcptag_info_t dhcptag_info_table[] = {\n");
	for (i = 0; i <= LAST_TAG; i++) {
	    int 	opt;
	    int 	type;
	    
	    opt = find_option(i);
	    if (opt < 0) {
		char	buf[128];
		
		sprintf(buf, "option_%d", i);
		printf("  /* %3d */ { %-20s, \"%s\" },\n", i, make_type("opaque"),
		       buf);
	    }
	    else
		printf("  /* %3d */ { %-20s, \"%s\" },\n", i,
		       make_type(option[opt].type), option[opt].name);
	}
	printf("};\n\n");
	
	printf("static dhcptype_info_t dhcptype_info_table[] = {\n");
	for (i = 0; types[i].name; i++) {
	    char * type = types[i].multiple_of;
	    
	    printf("  /* %2d */ { %d, %s, \"%s\"},\n", i, types[i].size, 
		   make_type((type != 0) ? type : "none"), types[i].name);
	}
	printf("};\n");
	printf("#endif _S_DHCP_PARSE_TABLE\n");
	break;
    case gen_mandoc:
	for (i = 1; i < LAST_TAG; i++) {
	    int 	opt;
	    int 	type;
	    
	    opt = find_option(i);
	    if (opt < 0) {
		continue;
	    }
	    printf(".It Sy dhcp_%s\n", option[opt].name);
	    printf("Option code %d, type is %s.\n", i, option[opt].type);
	}

	break;
    default:
	break;
    }
    exit(0);
    return (0);
}
