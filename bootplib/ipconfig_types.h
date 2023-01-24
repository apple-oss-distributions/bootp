/*
 * Copyright (c) 2000-2021 Apple Inc. All rights reserved.
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

#ifndef _S_IPCONFIG_TYPES_H
#define _S_IPCONFIG_TYPES_H

#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include "cfutil.h"
#include "util.h"

#ifdef mig_external
#undef mig_external
#endif
#define mig_external __private_extern__

#define IPCONFIG_IF_ANY		""


#define kInterfaceNameSize	IF_NAMESIZE

#ifndef kSCEntNetDHCPv6
#define kSCEntNetDHCPv6				CFSTR("DHCPv6")
#endif /* kSCEntNetDHCPv6 */

#ifndef kSCValNetIPv6ConfigMethodDHCPv6PD
#define kSCValNetIPv6ConfigMethodDHCPv6PD	CFSTR("DHCPv6PD")
#endif /* kSCValNetIPv6ConfigMethodDHCPv6PD */

#ifndef kSCPropNetIPv6RequestedPrefix
#define kSCPropNetIPv6RequestedPrefix		CFSTR("RequestedPrefix")
#endif /* kSCPropNetIPv6RequestedPrefix */

#ifndef kSCPropNetIPv6RequestedPrefixLength
#define kSCPropNetIPv6RequestedPrefixLength 	CFSTR("RequestedPrefixLength")
#endif /* kSCPropNetIPv6RequestedPrefixLength */

#ifndef kSCPropNetIPv6DelegatedPrefix
#define kSCPropNetIPv6DelegatedPrefix		CFSTR("DelegatedPrefix")
#endif /* kSCPropNetIPv6DelegatedPrefix */

#ifndef kSCPropNetIPv6DelegatedPrefixLength
#define kSCPropNetIPv6DelegatedPrefixLength	CFSTR("DelegatedPrefixLength")
#endif /* kSCPropNetIPv6DelegatedPrefixLength */

#ifndef kSCPropNetIPv6DelegatedPrefixValidLifetime
#define kSCPropNetIPv6DelegatedPrefixValidLifetime	\
    CFSTR("DelegatedPrefixValidLifetime")
#endif /* kSCPropNetIPv6DelegatedPrefixValidLifetime */

#ifndef kSCPropNetIPv6DelegatedPrefixPreferredLifetime
#define kSCPropNetIPv6DelegatedPrefixPreferredLifetime	\
    CFSTR("DelegatedPrefixPreferredLifetime")
#endif /* kSCPropNetIPv6DelegatedPrefixPreferredLifetime */

#ifndef kSCPropNetDHCPLeaseStartTime
#define kSCPropNetDHCPLeaseStartTime		CFSTR("LeaseStartTime")
#endif /* kSCPropNetDHCPLeaseStartTime */

#ifndef kSCPropNetDHCPLeaseExpirationTime
#define kSCPropNetDHCPLeaseExpirationTime	CFSTR("LeaseExpirationTime")
#endif /* kSCPropNetDHCPLeaseExpirationTime */

#ifndef kSCPropNetDHCPv6LeaseStartTime
#define kSCPropNetDHCPv6LeaseStartTime		CFSTR("LeaseStartTime")
#endif /* kSCPropNetDHCPv6LeaseStartTime */

#ifndef kSCPropNetDHCPv6LeaseExpirationTime
#define kSCPropNetDHCPv6LeaseExpirationTime	CFSTR("LeaseExpirationTime")
#endif /* kSCPropNetDHCPv6LeaseExpirationTime */

#define kDHCPOptionFormat		CFSTR("Option_%d")

typedef char	InterfaceName[kInterfaceNameSize];

static inline char *
InterfaceNameNulTerminate(InterfaceName name)
{
    name[kInterfaceNameSize - 1] = '\0';
    return (name);
}

static inline void
InterfaceNameClear(InterfaceName name)
{
    memset(name, 0, kInterfaceNameSize);
}

static inline void
InterfaceNameInit(InterfaceName name, const char * cstr)
{
    InterfaceNameClear(name);
    strlcpy(name, cstr, kInterfaceNameSize);
}

static inline void
InterfaceNameInitWithCFString(InterfaceName name, CFStringRef str)
{
    InterfaceNameClear(name);
    my_CFStringToCStringAndLength(str, name, kInterfaceNameSize);
}

#define kServiceIDSize		128

typedef char	ServiceID[kServiceIDSize];

static inline char *
ServiceIDNulTerminate(ServiceID service_id)
{
    service_id[kServiceIDSize - 1] = '\0';
    return (service_id);
}

static inline void
ServiceIDClear(ServiceID service_id)
{
    memset(service_id, 0, kServiceIDSize);
}

static inline void
ServiceIDInit(ServiceID service_id, const char * cstr)
{
    ServiceIDClear(service_id);
    strlcpy(service_id, cstr, kServiceIDSize);
}

static inline void
ServiceIDInitWithCFString(ServiceID service_id, CFStringRef str)
{
    ServiceIDClear(service_id);
    my_CFStringToCStringAndLength(str, service_id, kServiceIDSize);
}

static inline CFStringRef
ServiceIDCreateCFString(ServiceID service_id)
{
    return (CFStringCreateWithCString(NULL, (const char *)service_id,
				      kCFStringEncodingUTF8));
}

typedef uint8_t * xmlData_t;
typedef uint8_t * xmlDataOut_t;
typedef uint8_t * dataOut_t;
typedef uint32_t ip_address_t;
typedef uint32_t DHCPIAID;

typedef enum {
    ipconfig_status_success_e = 0,
    ipconfig_status_permission_denied_e = 1,
    ipconfig_status_interface_does_not_exist_e = 2,
    ipconfig_status_invalid_parameter_e = 3,
    ipconfig_status_invalid_operation_e = 4,
    ipconfig_status_allocation_failed_e = 5,
    ipconfig_status_internal_error_e = 6,
    ipconfig_status_operation_not_supported_e = 7,
    ipconfig_status_address_in_use_e = 8,
    ipconfig_status_no_server_e = 9,
    ipconfig_status_server_not_responding_e = 10,
    ipconfig_status_lease_terminated_e = 11,
    ipconfig_status_media_inactive_e = 12,
    ipconfig_status_server_error_e = 13,
    ipconfig_status_no_such_service_e = 14,
    ipconfig_status_duplicate_service_e = 15,
    ipconfig_status_address_timed_out_e = 16,
    ipconfig_status_not_found_e = 17,
    ipconfig_status_resource_unavailable_e = 18,
    ipconfig_status_network_changed_e = 19,
    ipconfig_status_lease_expired_e = 20,
    ipconfig_status_dhcp_waiting_e = 21
} ipconfig_status_t;

static __inline__ const char *
ipconfig_status_string(ipconfig_status_t status)
{
    static const char * str[] = {
	"success",
	"permission denied",
	"interface doesn't exist",
	"invalid parameter",
	"invalid operation",
	"allocation failed",
	"internal error",
	"operation not supported",
	"address in use",
	"no server",
	"server not responding",
	"lease terminated",
	"media inactive",
	"server error",
	"no such service",
	"duplicate service",
	"address timed out",
	"not found",
	"resource unavailable",
	"network changed",
	"lease expired",
	"DHCP waiting"
    };
    int str_count = countof(str);
    if (status >= str_count) {
	return ("<unknown>");
    }
    return (str[status]);
}
    
#endif /* _S_IPCONFIG_TYPES_H */
