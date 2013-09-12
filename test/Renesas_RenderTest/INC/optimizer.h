//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++


Module Name: optimizer.h

--*/

//
//	#define or -D__OPT_DISABLE to disable optimization tuning
//
//	#define __OPT_PLAT_FLAG for the particular architecture you are
//	having trouble with.  For example:
//		#define __OPT_PLAT_FLAG (defined(ARMV4I) || defined(ARMV4T))
//	This flag is cleared automatically.
//
//	You must #define __OPT_VER_OFF and include this file to turn
//  off optimization and #define __OPT_VER_RESTORE and include this
//	file to turn it back on.  These flags are cleared automatically.
//
//	You must #define __OPT_BUGNUMSTRING to the string representing
//	the compiler bug number.  For example:
//		#define	__OPT_BUGNUMSTRING "12345"
//	If you don't define this macro, you will get a compiler error.
//

#ifndef __OPT_DISABLE

#if __OPT_PLAT_FLAG

//
//	If __OPT_VER_OFF, disable optimization.  This flag is cleared
//	once it has been detected as set.
//
#if defined (__OPT_VER_OFF)

//
#ifndef __OPT_BUGNUMSTRING
#error Please specify a compiler bug number.
#endif	// __OPT_BUGNUMSTRING

#pragma optimize ("", off)
#pragma message ("Optimization turned off - bug " __OPT_BUGNUMSTRING)

#undef __OPT_VER_OFF

#endif 	// __OPT_VER_OFF

//
//	If __OPT_VER_RESTORE, restore optimization to default
//	settings, but only for specified version. We need
//	to revisit optimization structure for the next version.
//
#if defined (__OPT_VER_RESTORE)

#ifndef __OPT_BUGNUMSTRING
#error Please specify a compiler bug number.
#endif	// __OPT_BUGNUMSTRING

#pragma optimize ("", on)
#pragma message ("Optimization settings restored - bug " __OPT_BUGNUMSTRING)

#undef __OPT_VER_RESTORE

#endif 	// __OPT_VER_RESTORE

#endif	// __OPT_PLAT_FLAG

// clear this flag to avoid confusion if we have to suppress optimization 
// for multiple architectures.
#undef __OPT_PLAT_FLAG

#endif // _OPTIMIZER_H_
