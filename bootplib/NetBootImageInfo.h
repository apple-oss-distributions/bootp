
#include <CoreFoundation/CFString.h>

#define kNetBootImageInfoIndex		CFSTR("Index")		/* Number */
#define kNetBootImageInfoIsEnabled	CFSTR("IsEnabled") 	/* Boolean */
#define kNetBootImageInfoIsInstall	CFSTR("IsInstall")	/* Boolean */
#define kNetBootImageInfoName		CFSTR("Name")		/* String */
#define kNetBootImageInfoType		CFSTR("Type")		/* String */
#define kNetBootImageInfoBootFile	CFSTR("BootFile")	/* String */
#define kNetBootImageInfoIsDefault	CFSTR("IsDefault")	/* Boolean */
#define kNetBootImageInfoKind		CFSTR("Kind")		/* Number */
#define kNetBootImageInfoSupportsDiskless CFSTR("SupportsDiskless") /* Boolean */
#define kNetBootImageInfoEnabledSystemIdentifiers CFSTR("EnabledSystemIdentifiers") /* Array[String] */
#define kNetBootImageInfoFilterOnly 	CFSTR("FilterOnly")	/* Boolean */


/* Type values */
#define kNetBootImageInfoTypeClassic	CFSTR("Classic")
#define kNetBootImageInfoTypeNFS	CFSTR("NFS")
#define kNetBootImageInfoTypeHTTP	CFSTR("HTTP")
#define kNetBootImageInfoTypeBootFileOnly CFSTR("BootFileOnly")

/* Classic specific keys */
#define kNetBootImageInfoPrivateImage	CFSTR("PrivateImage")	/* String */
#define kNetBootImageInfoSharedImage	CFSTR("SharedImage")	/* String */

/* NFS, HTTP specific keys */
#define kNetBootImageInfoRootPath	CFSTR("RootPath")	/* String */


