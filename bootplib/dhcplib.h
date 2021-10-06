
#ifndef _S_DHCPLIB_H
#define _S_DHCPLIB_H
/*
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
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

#include <mach/boolean.h>
#include <sys/time.h>
#import <netinet/bootp.h>
#include "dhcp_options.h"
#include "gen_dhcp_tags.h"

void	dhcp_print_packet(struct dhcp *dp, int pkt_len);

static __inline__ struct timeval
timeval_from_secs(dhcp_time_secs_t secs)
{
    struct timeval tv;
    tv.tv_sec = secs;
    tv.tv_usec = 0;
    return (tv);
}

/*
 * Function: is_dhcp_packet
 *
 * Purpose:
 *   Return whether packet is a DHCP packet.
 *   If the packet contains DHCP message ids, then its a DHCP packet.
 */
static __inline__ boolean_t
is_dhcp_packet(dhcpol_t * options, dhcp_msgtype_t * msgtype)
{
    if (options) {
	u_char * opt;
	int opt_len;

	opt = dhcpol_find(options, dhcptag_dhcp_message_type_e,
			  &opt_len, NULL);
	if (opt != NULL) {
	    if (msgtype)
		*msgtype = *opt;
	    return (TRUE);
	}
    }
    return (FALSE);
}

boolean_t
dhcp_packet_match(struct bootp * packet, unsigned long xid, 
		  u_char hwtype, void * hwaddr, int hwlen);

#endif _S_DHCPLIB_H
