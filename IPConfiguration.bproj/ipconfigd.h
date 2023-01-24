/*
 * Copyright (c) 2000-2022 Apple Inc. All rights reserved.
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
#ifndef _S_IPCONFIGD_H
#define _S_IPCONFIGD_H

#include "ipconfigd_types.h"

/**
 ** Routines in support of MiG interface
 **/
ipconfig_status_t
ipconfig_method_info_from_plist(CFPropertyListRef plist,
				ipconfig_method_info_t info);
int
get_if_count();

ipconfig_status_t
get_if_addr(const char * name, ip_address_t * addr);

ipconfig_status_t
get_if_option(const char * name, int option_code, 
	      dataOut_t * option_data,
	      mach_msg_type_number_t *option_dataCnt);

ipconfig_status_t
get_if_packet(const char * name, dataOut_t * packet,
	      mach_msg_type_number_t *packetCnt);

ipconfig_status_t
get_if_v6_packet(const char * name, dataOut_t * packet,
		 mach_msg_type_number_t *packetCnt);

ipconfig_status_t
set_if(const char * name, ipconfig_method_info_t info);

ipconfig_status_t
add_service(const char * name,
	    ipconfig_method_info_t info,
	    ServiceID service_id,
	    CFDictionaryRef plist, pid_t pid);

ipconfig_status_t
set_service(const char * name,
	    ipconfig_method_info_t info,
	    ServiceID service_id);

ipconfig_status_t
remove_service_with_id(const char * name,
		       ServiceID service_id);

ipconfig_status_t
find_service(const char * name,
	     boolean_t exact,
	     ipconfig_method_info_t info,
	     ServiceID service_id);

ipconfig_status_t
remove_service(const char * name,
	       ipconfig_method_info_t info);

ipconfig_status_t
refresh_service(const char * name,
		ServiceID service_id);

ipconfig_status_t
is_service_valid(const char * name, ServiceID service_id);

ipconfig_status_t
forget_network(const char * name, CFStringRef ssid);

ipconfig_status_t
get_if_ra(const char * name, xmlDataOut_t * ra_data,
	  mach_msg_type_number_t *ra_data_cnt);

ipconfig_status_t
copy_if_summary(const char * name, CFDictionaryRef * summary);

ipconfig_status_t
copy_interface_list(CFArrayRef * list);

ipconfig_status_t
get_dhcp_duid(dataOut_t * dhcp_duid, mach_msg_type_number_t * dhcp_duid_cnt);

ipconfig_status_t
get_dhcp_ia_id(const char * name, DHCPIAID * ia_id_p);

#endif /* _S_IPCONFIGD_H */
