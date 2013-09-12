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
 * </CTL_AMD_STD>  */
/* <DOC_AMD_STD>
 * </DOC_AMD_STD>  */


#ifndef SECURE_SERIAL_H
#define SECURE_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif


#include <tchar.h>


#define MAX_SERIAL_NUM_SIZE 32
#define MIN_SERIAL_NUM_SIZE 20
#define MAX_DIXV_KEY_SIZE 32

#define SERIAL_STATUS_OK        0
#define SERIAL_STATUS_CORRUPT   1
#define SERIAL_FILE_ERROR       2
#define SERIAL_PARSING_ERROR    3
#define SERIAL_HASH_NOT_FOUND   4
#define SERIAL_HASH_MISMATCH    5
#define SERIAL_SOCKET_ERROR     6
#define SERIAL_HOSTNAME_ERROR   7
#define SERIAL_TOO_MANY_RETRY   8
#define SERIAL_HOSTID_MISMATCH  9
#define SERIAL_MAE_DEV_OPEN    10
#define SERIAL_MAE_DEV_IOCTL   11


#define ESERIAL_OK         0
#define ESERIAL_FILE_OPEN  1

// obfuscate names using strings common to all executables
// removed for CE

#ifdef secservlib_EXPORTS
#define secservlib_API __declspec(dllexport)
#else
#define secservlib_API __declspec(dllimport)
#endif

secservlib_API TCHAR *getSerialNum(int *errorCode);
int   getHwSecret(TCHAR **, int *);

#ifdef UNDER_CE
typedef unsigned int u_int32_t;
typedef unsigned long uint32;
typedef unsigned int u_int;
typedef unsigned long long u_int64_t;
typedef unsigned char u_int8_t;
#endif

#ifdef __cplusplus
}
#endif

#endif /* SECURE_SERIAL_H */
