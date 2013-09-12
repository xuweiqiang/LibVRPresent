/* <LIC_AMD_STD>
 * Copyright (C) 2003-2005 Advanced Micro Devices, Inc.  All Rights Reserved.
 * 
 * Unless otherwise designated in writing, this software and any related 
 * documentation are the confidential proprietary information of AMD. 
 * THESE MATERIALS ARE PROVIDED "AS IS" WITHOUT ANY
 * UNLESS OTHERWISE NOTED IN WRITING, EXPRESS OR IMPLIED WARRANTY OF ANY 
 * KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE AND IN NO 
 * EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER. 
 * 
 * AMD does not assume any responsibility for any errors which may appear 
 * in the Materials nor any responsibility to support or update the
 * Materials.  AMD retains the right to modify the Materials at any time, 
 * without notice, and is not obligated to provide such modified 
 * Materials to you. AMD is not obligated to furnish, support, or make
 * any further information available to you.
 * </LIC_AMD_STD>  */
/* <CTL_AMD_STD>
 * File Id and RCS Info:
 * $Source: /home/cvs/travis/env/types.h,v $
 * $Id: types.h,v 1.8 2004/04/13 23:49:24 nathans Exp $
 * </CTL_AMD_STD>  */
/* <DOC_AMD_STD>
 * </DOC_AMD_STD>  */


#ifndef _types_h__
#define _types_h__

#if defined(WINNT_ONLY) || defined(UNDER_CE)
typedef unsigned __int64 uint64;
typedef __int64 LONGLONG;
#else
typedef unsigned long long uint64;
#endif

typedef unsigned int       uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
typedef unsigned char      BMType[8];

#if defined(WINNT_ONLY) || defined(UNDER_CE)
typedef __int64 int64;
#ifndef INLINE
  #define INLINE __inline
#endif
#else
typedef   signed long long  int64;
#ifndef INLINE
  #define INLINE inline
#endif
#endif

typedef   signed int        int32;
typedef   signed short      int16;
typedef   signed char       int8;

#ifdef WINNT_ONLY
  // Win player  : Little endian
  #define __BYTE_ORDER 3210
  #define __BIG_ENDIAN 0123
  #define __LITTLE_ENDIAN 3210
#elif defined(OS_Linux) || defined(VPI_Linux)
  // Linux player: Little endian
  // Linux model : Little endian
  #include <endian.h>
#elif defined(VPI_SunOS)
  // SunOS model : Big endian
  #define __BYTE_ORDER 0123
  #define __BIG_ENDIAN 0123
  #define __LITTLE_ENDIAN 3210
#endif

#endif
