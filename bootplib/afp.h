#ifndef _S_AFP_H
#define _S_AFP_H
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
 * AFP_PASSWORD_LEN
 * - fixed length, NULL padded to 8 bytes total length
 */
#define AFP_PASSWORD_LEN		8

/*
 * AFP_PORT_NUMBER
 * - which port the afp server will be listening on
 */
#define AFP_PORT_NUMBER		548

/*
 * AFP_DIRID_NULL
 * - means no directory id
 */
#define AFP_DIRID_NULL		0

/*
 * AFP_DIRID_ROOT
 * - constant value of root directory id
 */
#define AFP_DIRID_ROOT		2

#endif _S_AFP_H
