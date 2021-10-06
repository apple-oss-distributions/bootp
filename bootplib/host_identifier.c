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
 * host_identifier.c
 * - contains the routines identifierFromString() and identifierToString()
 *   that convert between printable/readable strings and internal storage
 *   formats for the identifier property
 *
 * Examples:
 * 1. The ethernet hardware address 0:5:2:3e:4:30 is encoded as
 *    "1,0:5:2:3e:4:30".  
 * 2. The opaque client identifier "siegdi4" (no trailing NULL) is
 *    encoded as "0,73:69:65:67:64:69:34".
 */

/*
 * Modification History
 *
 * June 4, 1998 Dieter Siegmund (dieter@apple.com)
 * - initial revision
 */

#import <stdlib.h>
#import <stdio.h>
#import <unistd.h>
#import <sys/types.h>
#import <mach/boolean.h>
#import <string.h>
#import "host_identifier.h"

#define SEPARATOR	','

#define BASE_16		16

u_char *
identifierToString(u_char type, const void * identifier, int len)
{
    return (identifierToStringWithBuffer(type, identifier, len, NULL, 0));
}

u_char *
identifierToStringWithBuffer(u_char type, const void * identifier, int len,
			     u_char * buf, int buf_len)
{
    int 	i;
    u_char *	idstr = (u_char *)identifier;
    int 	max_encoded_len;
    
    /*
     * The encoding is:
     * %x<SEPARATOR>%x:%x:...:%x
     * Which has an approximate maximum string length of:
     *   (2 chars per byte + 1 char separator) * (len bytes + 1 byte type)
     *    + terminating NULL = 3 * (len + 1) + 1
     */
    max_encoded_len = 3 * (len + 1) + 1;
    if (buf == NULL || max_encoded_len > buf_len) {
	buf = malloc(max_encoded_len);
    }
    if (buf == NULL) {
	return buf;
    }
    sprintf(buf, "%x%c", type, SEPARATOR);
    for (i = 0; i < len; i++) {
	u_char tmp[4];
	sprintf(tmp, "%s%x", (i > 0) ? ":" : "", idstr[i]);
	strcat(buf, tmp);
    }
    return (buf);
}

void *
identifierFromString(const u_char * str, u_char * type, int * len)
{
    int		buf_pos;
    u_char * 	buf = NULL;
    boolean_t	done = FALSE;
    int		max_decoded_len;
    const u_char * scan;
    int 	slen = strlen(str);

    *len = 0;

    { /* copy the type */
	u_char		tmp[4];
	u_char *	sep;

	sep = strchr(str, SEPARATOR);
	if (sep == NULL)
	    return (NULL);
	if ((sep - str) > (sizeof(tmp) - 1))
	    return (NULL);
	strncpy(tmp, str, sizeof(tmp) - 1);
	tmp[sizeof(tmp) - 1] = '\0';
	*type = strtoul(tmp, NULL, BASE_16);
	scan = sep + 1; /* skip past the sep */
    }
    /*
     * the worst case we turn "1:2:3:4:5:6" into 6 bytes
     * strlen("1:2:3:4:5:6") = 11
     * so to get the approximate decoded length, 
     * we want strlen(str) / 2 + 1
     */
    max_decoded_len = (slen / 2) + 1;
    buf = malloc(max_decoded_len);
    if (buf == NULL)
	return (buf);
    for (buf_pos = 0; buf_pos < max_decoded_len && !done; buf_pos++) {
	u_char		tmp[4];
	const u_char *	colon;

	colon = strchr(scan, ':');
	if (colon == NULL) {
	    done = TRUE;
	    colon = str + slen;
	}
	if ((colon - scan) > (sizeof(tmp) - 1))
	    goto err;
	strncpy(tmp, scan, colon - scan);
	tmp[colon - scan] = '\0';
	buf[buf_pos] = (u_char)strtol(tmp, NULL, BASE_16);
	scan = colon + 1;
    }
    *len = buf_pos;
    return (buf);
  err:
    if (buf)
	 free(buf);
    return(NULL);
}

#ifdef TESTING
/* compile this using cc -o t -DTESTING host_identifier.c */
void
printData(u_char * data_p, int n_bytes)
{
#define CHARS_PER_LINE 	16
    char		line_buf[CHARS_PER_LINE + 1];
    int			line_pos;
    int			offset;

    for (line_pos = 0, offset = 0; offset < n_bytes; offset++, data_p++) {
	if (line_pos == 0)
	    printf("%04d ", offset);

	line_buf[line_pos] = isprint(*data_p) ? *data_p : '.';
	printf(" %02x", *data_p);
	line_pos++;
	if (line_pos == CHARS_PER_LINE) {
	    line_buf[CHARS_PER_LINE] = '\0';
	    printf("  %s\n", line_buf);
	    line_pos = 0;
	}
    }
    if (line_pos) { /* need to finish up the line */
	for (; line_pos < CHARS_PER_LINE; line_pos++) {
	    printf("   ");
	    line_buf[line_pos] = ' ';
	}
	line_buf[CHARS_PER_LINE] = '\0';
	printf("  %s\n", line_buf);
    }
}

main(int argc, char * argv[])
{
    u_char * str;
    void * decoded;
    int    decoded_len;
    u_char type;

    if (argc != 2) {
	fprintf(stderr, "useage: %s identifier\n", *argv);
	exit(1);
    }
    decoded = identifierFromString(argv[1], &type, &decoded_len);
    if (decoded == NULL) {
	printf("decode failed\n");
	exit(2);
    }
    printf("%s is decoded into type %x and a buf with length %d\n",
	   argv[1], type, decoded_len);
    printData(decoded, decoded_len);

    str = identifierToString(type, decoded, decoded_len);
    if (str == 0) {
	printf("encode failed\n");
	exit(1);
    }
    printf("%s is encoded as %s, len %d\n", argv[1], str, strlen(str));
    if (strcmp(argv[1], str) == 0) {
	printf("test passed\n");
    }
    else {
	printf("they are not equal - test failed\n");
    }

    free(decoded);
    free(str);

    exit(0);
}
#endif TESTING
