
/*
 * Copyright (c) 2001-2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*
 * nbimages.h
 * - NetBoot image list routines
 */

#ifndef _S_NBIMAGES_H
#define _S_NBIMAGES_H

#include "nbsp.h"
#include "bsdp.h"
#include <sys/types.h>

typedef enum {
    kNBImageTypeNone = 0,
    kNBImageTypeClassic,
    kNBImageTypeNFS,
    kNBImageTypeHTTP,
    kNBImageTypeBootFileOnly,
} NBImageType;

typedef union {
    struct {
	boolean_t	indirect;
	const char *	root_path;
    } nfs;
    struct {
	boolean_t	indirect;
	const char *	root_path;
    } http;
    struct {
	const char *	shared;
	const char *	private;
    } classic;
} NBImageTypeInfo;

typedef struct {
    NBSPEntry		sharepoint;
    const char *	arch;
    const char * *	archlist;
    int			archlist_count;
    char *		dir_name;	/* relative to sharepoint */
    char *		name;
    int			name_length;
    bsdp_image_id_t	image_id;
    const char *	bootfile;
    boolean_t		ppc_bootfile_no_subdir;
    NBImageType		type;
    NBImageTypeInfo	type_info;
    boolean_t		is_default;
    boolean_t		diskless;
    boolean_t		filter_only;
    const char * *	sysids;
    int			sysids_count;
} NBImageEntry, * NBImageEntryRef;

boolean_t	NBImageEntry_supported_sysid(NBImageEntryRef entry, 
					     const char * arch,
					     const char * sysid);
struct NBImageList_s;
typedef struct NBImageList_s * NBImageListRef;

int		NBImageList_count(NBImageListRef list);
NBImageEntryRef	NBImageList_element(NBImageListRef list, int i);
NBImageEntryRef NBImageList_elementWithID(NBImageListRef list, bsdp_image_id_t);
NBImageListRef	NBImageList_init(NBSPListRef sharepoints);
void		NBImageList_free(NBImageListRef * list);
void		NBImageList_print(NBImageListRef images);
NBImageEntryRef NBImageList_default(NBImageListRef images, 
				    const char * arch, const char * sysid,
				    const u_int16_t * attrs, int n_attrs);
boolean_t	NBImageEntry_attributes_match(NBImageEntryRef entry,
					      const u_int16_t * attrs,
					      int n_attrs);

#endif _S_NBIMAGES_H
