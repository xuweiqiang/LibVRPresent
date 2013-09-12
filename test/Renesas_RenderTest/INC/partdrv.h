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
/* *=* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF */
/* *=* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO */
/* *=* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A */
/* *=* PARTICULAR PURPOSE. */
#ifndef __PARTDRV_
#define __PARTDRV_


#include <windef.h>
#include <winnt.h>
#include <storemgr.h>

/*****************************************************************************/
/* information about a store */
typedef struct _PD_STOREINFO
{
    DWORD      cbSize;                  /* sizeof(PD_STOREINFO)                      */
    SECTORNUM  snNumSectors;            /* number of sectors on store             */
    DWORD      dwBytesPerSector;        /* number of bytes per sector             */
    SECTORNUM  snFreeSectors;           /* number of unallocated sectors          */
    SECTORNUM  snBiggestPartCreatable;  /* biggest partition currently creatable  */
    FILETIME   ftCreated;               /* last time store was formatted          */
    FILETIME   ftLastModified;          /* last time partition table was modified */
    DWORD      dwAttributes;            /* store attributes, see below            */
} PD_STOREINFO, *PPD_STOREINFO;


/*****************************************************************************/
/* information about a partition */

typedef struct _PD_PARTINFO
{
    DWORD     cbSize;                     /* sizeof(PD_PARTINFO)             */
    TCHAR     szPartitionName[PARTITIONNAMESIZE];   /* name of partition                 */
    SECTORNUM snNumSectors;               /* number of sectors in partition    */
    FILETIME  ftCreated;                  /* creation time of partition        */
    FILETIME  ftLastModified;             /* last time partition was modified  */
    DWORD     dwAttributes;               /* partition attributes, see below   */
    BYTE      bPartType;                  
} PD_PARTINFO, *PPD_PARTINFO;


/*****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/* store management */
DWORD PD_OpenStore(HANDLE hDisk, LPDWORD pdwStoreId);
void  PD_CloseStore(DWORD dwStoreId);


/* stores */
DWORD PD_FormatStore(DWORD dwStoreId);
DWORD PD_IsStoreFormatted(DWORD dwStoreId); 
DWORD PD_GetStoreInfo(DWORD dwStoreId, PD_STOREINFO *pInfo);

/* partitions management */
DWORD PD_CreatePartition(DWORD dwStoreId, LPCTSTR szPartName, BYTE bPartType, SECTORNUM numSectors, BOOL bAuto);
DWORD PD_DeletePartition(DWORD dwStoreId, LPCTSTR szPartName);
DWORD PD_RenamePartition(DWORD dwStoreId, LPCTSTR szOldName, LPCTSTR szNewName);
DWORD PD_SetPartitionAttrs(DWORD dwStoreId, LPCTSTR szPartName, DWORD dwAttr);
DWORD PD_SetPartitionSize(DWORD dwStoreId, LPCTSTR szPartName, SECTORNUM numSectors);
DWORD PD_FormatPartition(DWORD dwStoreId, LPCTSTR szPartName, BYTE bPartType, BOOL bAuto);
DWORD PD_GetPartitionInfo(DWORD dwStoreId, LPCTSTR szPartName, PD_PARTINFO *pInfo);
DWORD PD_FindPartitionStart(DWORD dwStoreId, LPDWORD pdwSearchId);
DWORD PD_FindPartitionNext(DWORD dwSearchId, PD_PARTINFO *pInfo);
void  PD_FindPartitionClose(DWORD dwSearchId);

/* partition I/O */
DWORD PD_OpenPartition(DWORD dwStoreId, LPCTSTR szPartName, LPDWORD pdwPartitionId);
void  PD_ClosePartition(DWORD dwPartitionId);
DWORD PD_DeviceIoControl(DWORD dwPartitionId, DWORD dwCode, PBYTE pInBuf, DWORD nInBufSize, PBYTE pOutBuf, DWORD nOutBufSize, PDWORD pBytesReturned);

#ifdef __cplusplus
}
#endif


#endif /* __PARTDRV__ */
