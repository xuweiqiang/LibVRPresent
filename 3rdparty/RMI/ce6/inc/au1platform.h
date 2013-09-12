/*
Copyright:

    Copyright (c) 2004 BSQUARE Corporation. All rights reserved.

Module Name:

    au1platform.h

Abstract:

    Platform specific definitions

Author:

*/


/* Useful macro for marking TODOs to be parsed from BUILD.log */
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : TODO Msg: "

/*
 *	SOC_XXXXX is defined in sources.cmn file
 */

#if defined( SOC_AU1200 )

#if defined( BOARD_PB1200 )
	#include "pb1200.h"
#else
	#include "db1200.h"
#endif

#elif defined ( SOC_AU13XX )
	#include "db13xx.h"
#endif
