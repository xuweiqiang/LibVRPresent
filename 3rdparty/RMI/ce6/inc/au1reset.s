/*
Copyright:

    Copyright (c) 2004 BSQUARE Corporation. All rights reserved.

Module Name:

    au1reset.s

Abstract:

    Platform specific reset/boot code

Author:

*/

/*
 *	SOC_XXXXX is defined in sources.cmn file
 */

#if defined( SOC_AU1200 )

#if defined( BOARD_PB1200 )
	#include "reset_pb1200.S"
#else
#include "reset_db1200.S"
#endif

#elif defined ( SOC_AU13XX )
#include "reset_db13xx.S"
#endif
