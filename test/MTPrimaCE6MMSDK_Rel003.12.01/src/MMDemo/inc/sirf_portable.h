

#ifndef SIRF_PORTABLE_H_
#define SIRF_PORTABLE_H_

/*
 *  general types
 */

#if defined( _WIN32_WCE ) || defined( WIN32 )		// win

#include <windows.h>

#else

#ifndef	UINT32
#define	UINT32		unsigned long
#endif

#ifndef	UINT16
#define	UINT16		unsigned short
#endif

#ifndef	UINT8
#define	UINT8		unsigned char
#endif

#ifndef	UCHAR
#define	UCHAR		unsigned char
#endif

#ifndef	INT32
#define	INT32		signed long
#endif

#ifndef	INT16
#define	INT16		signed short
#endif

#ifndef	INT8
#define	INT8			signed char
#endif

#ifndef	CHAR
#define	CHAR		signed char
#endif

typedef long LARGE_INTEGER;
#endif

#endif	// SIRF_PORTABLE_H_

