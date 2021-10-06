
#ifndef _S_BPFLIB_H
#define _S_BPFLIB_H
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

int bpf_get_blen(int fd, u_int * blen);
int bpf_new();
int bpf_dispose(int fd);
int bpf_setif(int fd, char * en_name);
int bpf_set_immediate(int fd, u_int value);
int bpf_filter_receive_none(int fd);
int bpf_arp_filter(int fd, int type_offset, int type, int packet_size);
int bpf_set_timeout(int fd, struct timeval * tv_p);
int bpf_write(int fd, void * pkt, int len);


#endif _S_BPFLIB_H
