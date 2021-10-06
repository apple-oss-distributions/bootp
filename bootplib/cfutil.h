/*
 * Copyright (c) 2003-2015 Apple Inc. All rights reserved.
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

#ifndef _S_CFUTIL_H
#define _S_CFUTIL_H

#include <CoreFoundation/CoreFoundation.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#include "symbol_scope.h"

#if NO_SYSTEMCONFIGURATION
static __inline__ CFTypeRef
isA_CFType(CFTypeRef obj, CFTypeID type)
{
	if (obj == NULL)
		return (NULL);

	if (CFGetTypeID(obj) != type)
		return (NULL);

	return (obj);
}

static __inline__ CFTypeRef
isA_CFArray(CFTypeRef obj)
{
	return (isA_CFType(obj, CFArrayGetTypeID()));
}

static __inline__ CFTypeRef
isA_CFBoolean(CFTypeRef obj)
{
	return (isA_CFType(obj, CFBooleanGetTypeID()));
}

static __inline__ CFTypeRef
isA_CFData(CFTypeRef obj)
{
	return (isA_CFType(obj, CFDataGetTypeID()));
}

static __inline__ CFTypeRef
isA_CFDictionary(CFTypeRef obj)
{
	return (isA_CFType(obj, CFDictionaryGetTypeID()));
}

static __inline__ CFTypeRef
isA_CFNumber(CFTypeRef obj)
{
	return (isA_CFType(obj, CFNumberGetTypeID()));
}

static __inline__ CFTypeRef
isA_CFString(CFTypeRef obj)
{
	return (isA_CFType(obj, CFStringGetTypeID()));
}

#else /* NO_SYSTEMCONFIGURATION */
#include <SystemConfiguration/SCValidation.h>

#endif /* NO_SYSTEMCONFIGURATION */

void
my_CFRelease(void * t);

CFPropertyListRef 
my_CFPropertyListCreateFromFile(const char * filename);

int
my_CFPropertyListWriteFile(CFPropertyListRef plist, const char * filename,
			   mode_t perms);

Boolean
my_CFStringArrayToCStringArray(CFArrayRef arr, void * buffer, int * buffer_size,
			       int * ret_count);
Boolean
my_CFStringArrayToEtherArray(CFArrayRef array, char * buffer, int * buffer_size,
			     int * ret_count);
bool
my_CFStringToIPAddress(CFStringRef str, struct in_addr * ret_ip);

bool
my_CFStringToIPv6Address(CFStringRef str, struct in6_addr * ret_ip);

int
my_CFStringToCStringAndLengthExt(CFStringRef cfstr, char * str, int len,
				 boolean_t is_external);
INLINE int
my_CFStringToCStringAndLength(CFStringRef cfstr, char * str, int len)
{
    return (my_CFStringToCStringAndLengthExt(cfstr, str, len, FALSE));
}

bool
my_CFTypeToNumber(CFTypeRef element, uint32_t * l_p);

bool
my_CFStringToNumber(CFStringRef str, uint32_t * ret_val);

void
my_CFDictionarySetTypeAsArrayValue(CFMutableDictionaryRef dict,
				   CFStringRef prop, CFTypeRef val);

void
my_CFDictionarySetIPAddressAsString(CFMutableDictionaryRef dict,
				    CFStringRef prop,
				    struct in_addr ip_addr);
void
my_CFDictionarySetIPAddressAsArrayValue(CFMutableDictionaryRef dict,
					CFStringRef prop,
					struct in_addr ip_addr);
void
my_CFArrayAppendUniqueValue(CFMutableArrayRef arr, CFTypeRef new);

CFStringRef
my_CFStringCopyComponent(CFStringRef path, CFStringRef separator, 
			 CFIndex component_index);
CFStringRef
my_CFStringCreateWithIPAddress(const struct in_addr ip);

CFStringRef
my_CFStringCreateWithIPv6Address(const void * ip6_addr);

void
my_CFStringAppendBytesAsHex(CFMutableStringRef str, const uint8_t * bytes,
			    int length, char sep);
char *
my_CFStringToCString(CFStringRef cfstr, CFStringEncoding encoding);

void
my_CFStringPrint(FILE * f, CFStringRef str);

Boolean
my_CFEqual(CFTypeRef val1, CFTypeRef val2);

#define STRING_APPEND(__string, __format, ...)		\
    CFStringAppendFormat(__string, NULL,		\
			 CFSTR(__format),		\
			 ## __VA_ARGS__)

CFArrayRef
my_CFStringArrayCreate(const char * * strings, CFIndex strings_count);

CFStringRef
my_CFUUIDStringCreate(CFAllocatorRef alloc);

#endif /* _S_CFUTIL_H */
