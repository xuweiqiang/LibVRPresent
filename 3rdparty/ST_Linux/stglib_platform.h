/*************************************************************************
**
** STGLIB, Accelerated 2D Graphics Library for Cartesio Plus SGA Engine.
**
** Copyright (C) 2009-2010 STMicroelectronics.  All rights reserved.
**
** This software is the confidential and/or proprietary information of
** STMicroelectronics. You shall not disclose such information and shall
** use it only in accordance with the terms of the license agreement you
** entered into with STMicroelectronics.
**
*************************************************************************/

#ifndef __STGLIB_PLATFORM_H
#define __STGLIB_PLATFORM_H

#ifdef _WIN32_WCE
#include <windows.h>
#include <Cmnintrin.h>
#define STGLIB_API_CALL						__declspec(dllexport)
#define vsnprintf(buf, size, format, ap)	_vsnprintf_s(buf, size, size, format, ap)
#endif

#ifdef _GNU_LINUX
typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HWND;
#define STGLIB_API_CALL
#endif

// Basic types definition
typedef unsigned long		sga_instruction;
typedef short				sga_coordinate;
typedef unsigned long		sga_address;
typedef unsigned char		sga_bool;
typedef signed long long	sga_int64;
typedef unsigned long long	sga_uint64;
typedef signed int			sga_int32;
typedef unsigned int		sga_uint32;
typedef signed short		sga_int16;
typedef unsigned short		sga_uint16;
typedef signed char			sga_int8;
typedef unsigned char		sga_uint8;

#endif /* __STGLIB_PLATFORM_H */
