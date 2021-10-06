/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
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
 * hostlist.h
 * - definitions for host list structures and functions
 */

struct hosts {
	struct hosts	*next;
	struct hosts	*prev;
	struct in_addr	iaddr;		/* internet address */
	u_char		htype;		/* hardware type */
	u_char		hlen;		/* hardware length */
	union {				/* hardware address */
	    struct ether_addr 	en;
	    u_char		generic[256];
	} haddr;
	char *		hostname;	/* host name (and suffix) */
	char *		bootfile;	/* default boot file name */
	struct timeval	tv;		/* time-in */

        u_long		lease;		/* lease (dhcp only) */
};

struct hosts * 	hostadd(struct hosts * * hosts, struct timeval * tv_p, 
			int htype, char * haddr, int hlen, 
			struct in_addr * iaddr_p, char * host_name,
			char * bootfile);
void		hostfree(struct hosts * * hosts, struct hosts * hp);
void		hostinsert(struct hosts * * hosts, struct hosts * hp);
void		hostprint(struct hosts * hp);
void		hostremove(struct hosts * * hosts, struct hosts * hp);
void		hostlistfree(struct hosts * * hosts);

static __inline__ struct hosts *
hostbyip(struct hosts * hosts, struct in_addr iaddr)
{
    struct hosts * hp;
    for (hp = hosts; hp; hp = hp->next) {
	if (iaddr.s_addr == hp->iaddr.s_addr)
	    return (hp);
    }
    return (NULL);
}

static __inline__ struct hosts *
hostbyaddr(struct hosts * hosts, u_char hwtype, void * hwaddr, int hwlen)
{
    struct hosts * hp;

    for (hp = hosts; hp; hp = hp->next) {
	if (hwtype == hp->htype 
	    && hwlen == hp->hlen
	    && bcmp(hwaddr, &hp->haddr, hwlen) == 0) {
	    return (hp);
	}
    }
    return (NULL);
}


