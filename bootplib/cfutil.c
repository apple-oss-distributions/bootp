/*
 * Copyright (c) 1999-2015 Apple Inc. All rights reserved.
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
 * cfutil.c
 * - CF utility functions
 */

/* 
 * Modification History
 *
 * February 15, 2002 	Dieter Siegmund (dieter@apple.com)
 * - broken out of ipconfigd.c
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <SystemConfiguration/SCValidation.h>
#include <CoreFoundation/CFData.h>
#include "util.h"
#include "cfutil.h"

#include "symbol_scope.h"

PRIVATE_EXTERN void
my_CFRelease(void * t)
{
    void * * obj = (void * *)t;
    if (obj && *obj) {
	CFRelease(*obj);
	*obj = NULL;
    }
    return;
}

static void *
read_file(const char * filename, size_t * data_length)
{
    void *		data = NULL;
    size_t		len = 0;
    int			fd = -1;
    struct stat		sb;

    *data_length = 0;
    if (stat(filename, &sb) < 0)
	goto done;
    len = sb.st_size;
    if (len == 0)
	goto done;

    data = malloc(len);
    if (data == NULL)
	goto done;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
	goto done;

    if (read(fd, data, len) != len) {
	goto done;
    }
 done:
    if (fd >= 0)
	close(fd);
    if (data) {
	*data_length = len;
    }
    return (data);
}

static int
write_file(const char * filename, const void * data, size_t data_length,
	   mode_t permissions)
{
    char		path[MAXPATHLEN];
    int			fd = -1;
    int			ret = 0;

    snprintf(path, sizeof(path), "%s-", filename);
    fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, permissions);
    if (fd < 0) {
	ret = -1;
	goto done;
    }

    if (write(fd, data, data_length) != data_length) {
	ret = -1;
	goto done;
    }
    rename(path, filename);
 done:
    if (fd >= 0) {
	close(fd);
    }
    return (ret);
}

PRIVATE_EXTERN CFPropertyListRef 
my_CFPropertyListCreateFromFile(const char * filename)
{
    void *		buf;
    size_t		bufsize;
    CFDataRef		data = NULL;
    CFPropertyListRef	plist = NULL;

    buf = read_file(filename, &bufsize);
    if (buf == NULL) {
	return (NULL);
    }
    data = CFDataCreateWithBytesNoCopy(NULL, buf, bufsize, kCFAllocatorNull);
    if (data == NULL) {
	goto done;
    }
    plist = CFPropertyListCreateWithData(NULL,
					 data, 
					 kCFPropertyListImmutable,
					 NULL,
					 NULL);
 done:
    if (data)
	CFRelease(data);
    if (buf)
	free(buf);
    return (plist);
}

PRIVATE_EXTERN int
my_CFPropertyListWriteFile(CFPropertyListRef plist, const char * filename,
			   mode_t permissions)
{
    CFDataRef	data;
    int		ret;

    if (plist == NULL)
	return (0);

    data = CFPropertyListCreateData(NULL,
				    plist,
				    kCFPropertyListXMLFormat_v1_0,
				    0,
				    NULL);
    if (data == NULL) {
	return (0);
    }
    ret = write_file(filename, 
		     (const void *)CFDataGetBytePtr(data),
		     CFDataGetLength(data),
		     permissions);
    CFRelease(data);
    return (ret);
}

PRIVATE_EXTERN int
my_CFStringToCStringAndLengthExt(CFStringRef cfstr, char * str, int len,
				 boolean_t is_external)
{
    CFIndex		ret_len = 0;

    CFStringGetBytes(cfstr, CFRangeMake(0, CFStringGetLength(cfstr)),
		     kCFStringEncodingUTF8, 0, is_external,
		     (UInt8 *)str, len - 1, &ret_len);
    if (str != NULL) {
	str[ret_len] = '\0';
    }
    return ((int)ret_len + 1); /* leave 1 byte for nul-termination */
}

PRIVATE_EXTERN Boolean
my_CFStringArrayToCStringArray(CFArrayRef arr, void * buffer, int * buffer_size,
			       int * ret_count)
{
    CFIndex	count = CFArrayGetCount(arr);
    int 	i;
    char *	offset = NULL;	
    int		space;
    char * *	strlist = NULL;

    space = (int)count * sizeof(char *);
    if (buffer != NULL) {
	if (*buffer_size < space) {
	    /* not enough space for even the pointer list */
	    return (FALSE);
	}
	strlist = (char * *)buffer;
	offset = buffer + space; /* the start of the 1st string */
    }
    for (i = 0; i < count; i++) {
	int		len = 0;
	CFStringRef	str;

	str = CFArrayGetValueAtIndex(arr, i);
	if (isA_CFString(str) == NULL) {
	    return (FALSE);
	}
	if (buffer != NULL) {
	    len = *buffer_size - space;
	    if (len < 0) {
		return (FALSE);
	    }
	}
	len = my_CFStringToCStringAndLength(str, offset, len);
	if (buffer != NULL) {
	    strlist[i] = offset;
	    offset += len;
	}
	space += len;
    }
    *buffer_size = roundup(space, sizeof(char *));
    *ret_count = (int)count;
    return (TRUE);
}

PRIVATE_EXTERN Boolean
my_CFStringArrayToEtherArray(CFArrayRef array, char * buffer, int * buffer_size,
			     int * ret_count)
{
    CFIndex		count = CFArrayGetCount(array);
    int 		i;
    struct ether_addr * list = NULL;
    int			space;

    space = roundup((int)count * sizeof(*list), sizeof(char *));
    if (buffer != NULL) {
	if (*buffer_size < space) {
	    /* not enough space for all elements */
	    return (FALSE);
	}
	list = (struct ether_addr *)buffer;
    }
    for (i = 0; i < count; i++) {
	struct ether_addr * 	eaddr;
	CFStringRef		str = CFArrayGetValueAtIndex(array, i);
	char			val[64];

	if (isA_CFString(str) == NULL) {
	    return (FALSE);
	}
	if (CFStringGetCString(str, val, sizeof(val), kCFStringEncodingASCII)
	    == FALSE) {
	    return (FALSE);
	}
	eaddr = ether_aton((char *)val);
	if (eaddr == NULL) {
	    return (FALSE);
	}
	if (list != NULL) {
	    list[i] = *eaddr;
	}
    }
    *buffer_size = space;
    *ret_count = (int)count;
    return (TRUE);
}

PRIVATE_EXTERN CFArrayRef
my_CFStringArrayCreate(const char * * strings, CFIndex strings_count)
{
    CFIndex		i;
    CFMutableArrayRef	ret_list;

    ret_list = CFArrayCreateMutable(NULL, strings_count,
				    &kCFTypeArrayCallBacks);
    for (i = 0; i < strings_count; i++) {
	CFStringRef	str;

	str = CFStringCreateWithCString(NULL, strings[i],
					kCFStringEncodingUTF8);
	if (str != NULL) {
	    CFArrayAppendValue(ret_list, str);
	    CFRelease(str);
	}
    }
    return (ret_list);
}

PRIVATE_EXTERN bool
my_CFStringToIPAddress(CFStringRef str, struct in_addr * ret_ip)
{
    char		buf[64];

    ret_ip->s_addr = 0;
    if (isA_CFString(str) == NULL) {
	return (FALSE);
    }
    if (CFStringGetCString(str, buf, sizeof(buf), kCFStringEncodingASCII)
	== FALSE) {
	return (FALSE);
    }
    if (inet_aton(buf, ret_ip) == 1) {
	return (TRUE);
    }
    return (FALSE);
}

PRIVATE_EXTERN bool
my_CFStringToNumber(CFStringRef str, uint32_t * ret_val)
{
    char		buf[64];
    unsigned long	val;

    my_CFStringToCStringAndLength(str, buf, sizeof(buf));
    val = strtoul(buf, NULL, 0);
    if (val != ULONG_MAX && errno != ERANGE) {
	*ret_val = (uint32_t)val;
	return (TRUE);
    }
    return (FALSE);
}

PRIVATE_EXTERN bool
my_CFTypeToNumber(CFTypeRef element, uint32_t * l_p)
{
    if (isA_CFString(element) != NULL) {
	if (my_CFStringToNumber(element, l_p) == FALSE) {
	    return (FALSE);
	}
    }
    else if (isA_CFBoolean(element) != NULL) {
	*l_p = CFBooleanGetValue(element);
    }
    else if (isA_CFNumber(element) != NULL) {
	if (CFNumberGetValue(element, kCFNumberSInt32Type, l_p) 
	    == FALSE) {
	    return (FALSE);
	}
    }
    else {
	return (FALSE);
    }
    return (TRUE);
}

PRIVATE_EXTERN void
my_CFDictionarySetTypeAsArrayValue(CFMutableDictionaryRef dict,
				   CFStringRef prop, CFTypeRef val)
{
    CFArrayRef	array;

    array = CFArrayCreate(NULL, (const void **)&val, 1,
			  &kCFTypeArrayCallBacks);
    if (array != NULL) {
	CFDictionarySetValue(dict, prop, array);
	CFRelease(array);
    }
    return;
}

PRIVATE_EXTERN void
my_CFDictionarySetIPAddressAsArrayValue(CFMutableDictionaryRef dict,
					CFStringRef prop,
					struct in_addr ip_addr)
{
    CFStringRef		str;

    str = CFStringCreateWithFormat(NULL, NULL, CFSTR(IP_FORMAT),
				   IP_LIST(&ip_addr));
    my_CFDictionarySetTypeAsArrayValue(dict, prop, str);
    CFRelease(str);
    return;
}

PRIVATE_EXTERN void
my_CFArrayAppendUniqueValue(CFMutableArrayRef arr, CFTypeRef new)
{
    CFIndex count;
    int i;

    count = CFArrayGetCount(arr);
    for (i = 0; i < count; i++) {
	CFStringRef element = CFArrayGetValueAtIndex(arr, i);
	if (CFEqual(element, new)) {
	    return;
	}
    }
    CFArrayAppendValue(arr, new);
    return;
}

PRIVATE_EXTERN Boolean
my_CFEqual(CFTypeRef val1, CFTypeRef val2)
{
    if (val1 == NULL) {
	if (val2 == NULL) {
	    return (TRUE);
	}
	return (FALSE);
    }
    if (val2 == NULL) {
	return (FALSE);
    }
    if (CFGetTypeID(val1) != CFGetTypeID(val2)) {
	return (FALSE);
    }
    return (CFEqual(val1, val2));
}


/* 
 * Function: my_CFStringCopyComponent
 * Purpose:
 *    Separates the given string using the given separator, and returns
 *    the component at the specified index.
 * Returns:
 *    NULL if no such component exists, non-NULL component otherwise
 */
PRIVATE_EXTERN CFStringRef
my_CFStringCopyComponent(CFStringRef path, CFStringRef separator, 
			 CFIndex component_index)
{
    CFArrayRef		arr;
    CFStringRef		component = NULL;

    arr = CFStringCreateArrayBySeparatingStrings(NULL, path, separator);
    if (arr == NULL) {
	goto done;
    }
    if (CFArrayGetCount(arr) <= component_index) {
	goto done;
    }
    component = CFRetain(CFArrayGetValueAtIndex(arr, component_index));

 done:
    my_CFRelease(&arr);
    return (component);

}

CFStringRef
my_CFStringCreateWithIPAddress(const struct in_addr ip)
{
    return (CFStringCreateWithFormat(NULL, NULL, 
				     CFSTR(IP_FORMAT), IP_LIST(&ip)));
}

CFStringRef
my_CFStringCreateWithIPv6Address(const void * ip6_addr)
{
    char 		ntopbuf[INET6_ADDRSTRLEN];
    const char *	c_str;

    c_str = inet_ntop(AF_INET6, ip6_addr, ntopbuf, sizeof(ntopbuf));
    return (CFStringCreateWithCString(NULL, c_str, kCFStringEncodingASCII));
}

void
my_CFStringAppendBytesAsHex(CFMutableStringRef str, const uint8_t * bytes,
			    int length, char separator)
{
    int i;

    for (i = 0; i < length; i++) {
	char  	sep[3];

	if (i == 0) {
	    sep[0] = '\0';
	}
	else {
	    if ((i % 8) == 0 && separator == ' ') {
		sep[0] = sep[1] = ' ';
		sep[2] = '\0';
	    }
	    else {
		sep[0] = separator;
		sep[1] = '\0';
	    }
	}
	CFStringAppendFormat(str, NULL, CFSTR("%s%02x"), sep, bytes[i]);
    }
    return;
}

char *
my_CFStringToCString(CFStringRef cfstr, CFStringEncoding encoding)
{
    CFIndex		l;
    CFRange		range;
    uint8_t *		str;

    range = CFRangeMake(0, CFStringGetLength(cfstr));
    CFStringGetBytes(cfstr, range, encoding,
		     0, FALSE, NULL, 0, &l);
    if (l <= 0) {
	return (NULL);
    }
    str = (uint8_t *)malloc(l + 1);
    CFStringGetBytes(cfstr, range, encoding, 0, FALSE, str, l, &l);
    str[l] = '\0';
    return ((char *)str);
}

PRIVATE_EXTERN CFStringRef
my_CFUUIDStringCreate(CFAllocatorRef alloc)
{
    CFUUIDRef 	uuid;
    CFStringRef	uuid_str;

    uuid = CFUUIDCreate(alloc);
    uuid_str = CFUUIDCreateString(alloc, uuid);
    CFRelease(uuid);
    return (uuid_str);
}
