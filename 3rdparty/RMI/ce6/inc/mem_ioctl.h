/**********************************************************************
* Copyright 2007 RMI Corp. All Rights Reserved.
*
* Unless otherwise designated in writing, this software and any related
* documentation are the confidential proprietary information of RMI
* Corp.
*
* THESE MATERIALS ARE PROVIDED "AS IS" WITHOUT ANY
* UNLESS OTHERWISE NOTED IN WRITING, EXPRESS OR IMPLIED WARRANTY OF ANY
* KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE AND IN NO
* EVENT SHALL RMI COPR. OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES
* WHATSOEVER.
*
* RMI Corp. does not assume any responsibility for any errors which may
* appear in the Materials nor any responsibility to support or update
* the Materials. RMI Corp. retains the right to modify the Materials
* at any time, without notice, and is not obligated to provide such
* modified Materials to you. RMI Corp. is not obligated to furnish,
* support, or make any further information available to you.
***********************************************************************/

#ifndef __MEMIOCTL_H_
#define __MEMIOCTL_H_

// These probably belong in a platform-specific area...
#define REGION_MAE				0
#define REGION_ITE				1
#define REGION_LCD				2
#define REGION_OGL				REGION_LCD

#define IOCTLCode(f,m) \
  CTL_CODE ( FILE_DEVICE_UNKNOWN, (f), (m), FILE_ANY_ACCESS )

#define	MEM_MAGIC(n)			(n|0x100)

typedef struct
{
	HANDLE hMem;
	PVOID pVirtual;
	PVOID pPhysical;
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwRegion;
	DWORD dwReserved1;
	DWORD dwReserved2;
} MEM_IOCTL, *PMEM_IOCTL;

#define MEM_REQUEST_BLOCK	IOCTLCode(MEM_MAGIC(1), METHOD_BUFFERED)
#define MEM_FREE_BLOCK 		IOCTLCode(MEM_MAGIC(2), METHOD_BUFFERED)
#define MEM_GET_REGION_SIZE IOCTLCode(MEM_MAGIC(3), METHOD_BUFFERED)

#define MEM_FLAG_NO_MAPVIRTUAL	1 << 1
#define MEM_FLAG_SD_TILING		1 << 2
#define	MEM_FLAG_NO_CACHE		1 << 3

#endif __MEMIOCTL_H_