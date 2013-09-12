/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/

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

