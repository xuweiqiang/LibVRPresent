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


Module Name:

    romxip.h

Abstract:

    Definition of IOCTLs that are used in XIP updates

Author:

    Randy Ramig (RandyRam) 1/12/01

Revision History:

--*/

#pragma once


#include <winioctl.h>

//
// The max number of XIP regions to support
// Note that this is not the absolute max, but
// has been chosen so that all XIP related code 
// uses the same one.
//

#define MAX_ROM                 32
#define XIP_NAMELEN             32  // max len of name of xip
#define XIPFILE_HEADER_SIZE     2*sizeof(DWORD)

//
// Flags for XIP entries (see XIPCHAIN_ENTRY)
//

#define ROMXIP_OK_TO_LOAD       0x0001
#define ROMXIP_IS_SIGNED        0x0002

//
// Flags for handling the Chain Set call
//

#define ROMXIP_SETFLAG_INVALIDATE_ALL   0x00000001

//
// Value that tells OAL to not read the chain
// This is read at boot up-- it needs to be significant
// because we don't know the initial state of RAM on cold boot.
//

#define ROMXIP_DONT_USE_CHAIN  0x4B415459

#pragma pack(push)
#pragma pack(1)

#define IOCTL_HAL_OAK_START                 5000
#define IOCTL_HAL_XIP_BASE                  IOCTL_HAL_OAK_START + 500

/*++

	IOCTL_HAL_WRITE_XIP

	Writes an XIP region to flash
	
	Input Buffer is a WRITE_XIP_HEADER followed immediately by dwNumRecords
    of XIP_RECORD_INFO structures

	Output buffer is unused.

--*/


// #define IOCTL_HAL_WRITE_XIP       CTL_CODE(FILE_DEVICE_HAL, IOCTL_HAL_XIP_BASE + 1, METHOD_BUFFERED, FILE_ANY_ACCESS)

        
typedef struct _WRITE_XIP_HEADER
{
	LPVOID  pvPhysAddr;     // physical starting address of XIP
    DWORD	dwLength;       // length of XIP
    DWORD   dwNumRecords;   // number of records in XIP

} WRITE_XIP_HEADER, *PWRITE_XIP_HEADER;

typedef struct _XIP_RECORD_INFO {

    LPVOID  pvPhysStart;    // physical starting address of record, dest addr
    DWORD   dwPhysLen;      // physical len, in bytes of record
    DWORD   dwCheckSum;     // sum of all the bytes in the record data
    VOID UNALIGNED * pvRAMStart;     // virtual address of record in RAM, source addr

}   XIP_RECORD_INFO, *PXIP_RECORD_INFO;

/*++

	IOCTL_HAL_XIPCHAIN

	Used to GET or SET XIP chain.
	
    For a SET, the output buffer pointer is null, and the
	input buffer is a XIPCHAIN_HEADER followed immediately by the chain
    of XIPCHAIN_ENTRY structures

    For a GET, the input buffer pointer is null, and the
	Output buffer is a XIPCHAIN_HEADER followed immediately by the chain
    of XIPCHAIN_ENTRY structures

--*/

// #define IOCTL_HAL_XIPCHAIN        CTL_CODE(FILE_DEVICE_HAL, IOCTL_HAL_XIP_BASE + 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _XIPCHAIN_HEADER {

    DWORD   dwSetChainFlags;    // flags on how to handle the setting of the chain
    DWORD   dwNumXIPs;          // number of XIPs in chain

} XIPCHAIN_HEADER, *PXIPCHAIN_HEADER;

#pragma pack(pop)
