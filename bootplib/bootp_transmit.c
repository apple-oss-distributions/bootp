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
 * bootp_transmit.c
 * - send a bootp reques using a socket or BPF
 */
/* 
 * Modification History
 *
 * May 11, 2000		Dieter Siegmund (dieter@apple.com)
 * - created
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <ctype.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <net/firewire.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <syslog.h>

#include "bootp_transmit.h"
#include "bpflib.h"
#include "in_cksum.h"

extern void
my_log(int priority, const char *message, ...);

static char link_broadcast[8] = { 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

typedef struct {
    struct ip 		ip;
    struct udphdr 	udp;
} ip_udp_header_t;

typedef struct {
    struct in_addr	src_ip;
    struct in_addr	dest_ip;
    char		zero;
    char		proto;
    unsigned short	length;
} udp_pseudo_hdr_t;


static int 
get_bpf_fd(char * if_name)
{
    int bpf_fd;

    bpf_fd = bpf_new();
    if (bpf_fd < 0) {
	/* BPF transmit unavailable */
	my_log(LOG_ERR, "Transmitter: bpf_fd() failed, %s (%d)",
	       strerror(errno), errno);
    }
    else if (bpf_filter_receive_none(bpf_fd) < 0) {
	my_log(LOG_ERR,  "Transmitter: failed to set filter, %s (%d)",
	       strerror(errno), errno);
	bpf_dispose(bpf_fd);
	bpf_fd = -1;
    }
    else if (bpf_setif(bpf_fd, if_name) < 0) {
	my_log(LOG_ERR, "Transmitter: bpf_setif(%s) failed: %s (%d)", if_name,
	       strerror(errno), errno);
	bpf_dispose(bpf_fd);
	bpf_fd = -1;
    }

    return (bpf_fd);
}

int
bootp_transmit(int sockfd, char sendbuf[2048],
	       char * if_name, int hwtype, void * hwaddr, int hwlen,
	       struct in_addr dest_ip,
	       struct in_addr src_ip,
	       u_short dest_port,
	       u_short src_port,
	       void * data, int len)
{
    static int	first = 1;
    static int 	ip_id = 0;
    int		bpf_fd = -1;
    int 	status = 0;

    if (first) {
	first = 0;
	ip_id = random();
    }

    if ((hwtype == ARPHRD_ETHER || hwtype == ARPHRD_IEEE1394)
	&& (ntohl(dest_ip.s_addr) == INADDR_BROADCAST
	    || hwaddr != NULL)) {
	bpf_fd = get_bpf_fd(if_name);
	if (bpf_fd < 0) {
	    status = -1;
	}
	else {
	    int				frame_length;
	    ip_udp_header_t *		ip_udp;
	    char *			payload;
	    udp_pseudo_hdr_t *		udp_pseudo;

	    switch (hwtype) {
	    default:
	    case ARPHRD_ETHER:
		{
		    struct ether_header *	eh_p;

		    eh_p = (struct ether_header *)sendbuf;
		    ip_udp = (ip_udp_header_t *)(sendbuf + sizeof(*eh_p));
		    udp_pseudo = (udp_pseudo_hdr_t *)(((char *)&ip_udp->udp)
						      - sizeof(*udp_pseudo));
		    payload = sendbuf + sizeof(*eh_p) + sizeof(*ip_udp);
		    /* fill in the ethernet header */
		    if (ntohl(dest_ip.s_addr) == INADDR_BROADCAST) {
			bcopy(link_broadcast, eh_p->ether_dhost,
			      sizeof(eh_p->ether_dhost));
		    }
		    else {
			bcopy(hwaddr, eh_p->ether_dhost,
			      sizeof(eh_p->ether_dhost));
		    }
		    eh_p->ether_type = htons(ETHERTYPE_IP);
		    frame_length = sizeof(*eh_p) + sizeof(*ip_udp) + len;
		    break;
		}
	    case ARPHRD_IEEE1394:
		{
		    struct firewire_header *	fh_p;
		    
		    /* fill in the firewire header */
		    fh_p = (struct firewire_header *)sendbuf;
		    bcopy(link_broadcast, fh_p->firewire_dhost,
			  sizeof(fh_p->firewire_dhost));
		    fh_p->firewire_type = htons(ETHERTYPE_IP);
		    ip_udp = (ip_udp_header_t *)(sendbuf + sizeof(*fh_p));
		    udp_pseudo = (udp_pseudo_hdr_t *)(((char *)&ip_udp->udp)
						      - sizeof(*udp_pseudo));
		    payload = sendbuf + sizeof(*fh_p) + sizeof(*ip_udp);
		    frame_length = sizeof(*fh_p) + sizeof(*ip_udp) + len;
		    break;
		}
	    }
	    
	    /* copy the data */
	    bcopy(data, payload, len);

	    /* fill in udp pseudo header */
	    udp_pseudo->src_ip = src_ip;
	    udp_pseudo->dest_ip = dest_ip;
	    udp_pseudo->zero = 0;
	    udp_pseudo->proto = IPPROTO_UDP;
	    udp_pseudo->length = htons(sizeof(ip_udp->udp) + len);

	    /* fill in UDP header */
	    ip_udp->udp.uh_sport = htons(src_port);
	    ip_udp->udp.uh_dport = htons(dest_port);
	    ip_udp->udp.uh_ulen = htons(sizeof(ip_udp->udp) + len);
	    ip_udp->udp.uh_sum = 0;
	    ip_udp->udp.uh_sum = in_cksum(udp_pseudo, sizeof(*udp_pseudo) + 
					  sizeof(ip_udp->udp) + len);

	    /* fill in IP header */
	    bzero(ip_udp, sizeof(ip_udp->ip));
	    ip_udp->ip.ip_v = IPVERSION;
	    ip_udp->ip.ip_hl = sizeof(struct ip) >> 2;
	    ip_udp->ip.ip_ttl = MAXTTL;
	    ip_udp->ip.ip_p = IPPROTO_UDP;
	    ip_udp->ip.ip_src = src_ip;
	    ip_udp->ip.ip_dst = dest_ip;
	    ip_udp->ip.ip_len = htons(sizeof(*ip_udp) + len);
	    ip_udp->ip.ip_id = htons(ip_id++);
	    /* compute the IP checksum */
	    ip_udp->ip.ip_sum = 0; /* needs to be zero for checksum */
	    ip_udp->ip.ip_sum = in_cksum(&ip_udp->ip, sizeof(ip_udp->ip));
	    
	    status = bpf_write(bpf_fd, sendbuf, frame_length);
	    if (status < 0) {
		my_log(LOG_ERR, 
		       "bootp_session_transmit: bpf_write(%s) failed: %s (%d)",
		       if_name, strerror(errno), errno);
	    }
	}
    }
    else if (sockfd >= 0) { /* send using socket */
	struct sockaddr_in 	dst;
	ssize_t			send_status;

	bzero(&dst, sizeof(dst));
	dst.sin_len = sizeof(struct sockaddr_in);
	dst.sin_family = AF_INET;
	dst.sin_port = htons(dest_port);
	dst.sin_addr = dest_ip;
	send_status = sendto(sockfd, data, len, 0,
			     (struct sockaddr *)&dst, 
			     sizeof(struct sockaddr_in));
	if (send_status < len)
	    status = -1;

    }
    else {
	my_log(LOG_ERR, 
	       "bootp_session_transmit: neither bpf nor socket send available");
    }

    if (bpf_fd >= 0) {
	bpf_dispose(bpf_fd);
    }
    return (status);
}
		       
