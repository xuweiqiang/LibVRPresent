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
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

romldr.h

Abstract:

ROM loader structures

Functions:


Notes: 


--*/

#ifndef __ROMLDR_H__
#define __ROMLDR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define KFLAG_DISALLOW_PAGING   0x00000001
#define KFLAG_NOTALLKMODE       0x00000002
#define KFLAG_TRUSTROMONLY      0x00000010

#ifdef x86
#define KFLAG_NOFLUSHPT         0x00000020  // use this flag to not flushing soft TLB on x86.
                                            // potential security hole if using this flag,
                                            // but improve RT performance
#endif

#define KFLAG_HONOR_DLL_BASE    0x00000040  // honor DLL's preferred load address.
                                            // Loading DLL at its preferred load address will release kernel
                                            // from reloacating the DLL. However, there might be potential Backward
                                            // compatibility issue since there might be existing DLL binaries
                                            // out there that sets it's preferred load address really low and thus
                                            // not able to be loaded.

#define ROM_SIGNATURE_OFFSET   0x40         // Offset from the image's physfirst address to the ROM signature.
#define ROM_SIGNATURE          0x43454345
#define ROM_TOC_POINTER_OFFSET 0x44         // Offset from the image's physfirst address to the TOC pointer.
#define ROM_TOC_OFFSET_OFFSET  0x48         // Offset from the image's physfirst address to the TOC offset (from physfirst).

#ifndef ASM_ONLY

#define ROM_EXTRA 9

typedef struct e32_rom {
    unsigned short  e32_objcnt;     /* Number of memory objects            */
    unsigned short  e32_imageflags; /* Image flags                         */
    unsigned long   e32_entryrva;   /* Relative virt. addr. of entry point */
    unsigned long   e32_vbase;      /* Virtual base address of module      */
    unsigned short  e32_subsysmajor;/* The subsystem major version number  */
    unsigned short  e32_subsysminor;/* The subsystem minor version number  */
    unsigned long   e32_stackmax;   /* Maximum stack size                  */
    unsigned long   e32_vsize;      /* Virtual size of the entire image    */
    unsigned long   e32_sect14rva;  /* section 14 rva */
    unsigned long   e32_sect14size; /* section 14 size */
    unsigned long   e32_timestamp;  /* Time EXE/DLL was created/modified   */
    struct info     e32_unit[ROM_EXTRA]; /* Array of extra info units      */
    unsigned short  e32_subsys;     /* The subsystem type                  */
} e32_rom;

typedef struct o32_rom {
    unsigned long       o32_vsize;      /* Virtual memory size              */
    unsigned long       o32_rva;        /* Object relative virtual address  */
    unsigned long       o32_psize;      /* Physical file size of init. data */
    unsigned long       o32_dataptr;    /* Image pages offset               */
    unsigned long       o32_realaddr;   /* pointer to actual                */
    unsigned long       o32_flags;      /* Attribute flags for the object   */
} o32_rom;

#define IOCTL_BIN_GET_E32   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 0x1, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_BIN_GET_O32   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 0x2, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_BIN_GET_XIP_PAGES   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 0x3, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_BIN_SET_E32   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 0x4, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_BIN_SET_O32   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 0x5, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_BIN_GET_ROMHDR   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 0x11, METHOD_BUFFERED, FILE_WRITE_ACCESS)


typedef struct _ROMINFO {
    DWORD   dwSlot_0_DllBase;
    DWORD   dwSlot_1_DllBase;
    int     nROMs;
    // hi/lo pair follows
    // LARGE_INTEGER    rwInfo[nROMs];
} ROMINFO, *PROMINFO;


#endif

#define ROM_KERNEL_DEBUGGER_ENABLED 0x00000001
#define ROM_CONTAINS_KERNEL         0x00000002

//
//  ROM Header Structure - pTOC in NK points here
//
#ifdef ASM_ONLY
#define ROMHDR_dllfirst         0
#define ROMHDR_dlllast          4
#define ROMHDR_physfirst        8
#define ROMHDR_physlast         12
#define ROMHDR_nummods          16
#define ROMHDR_ulRAMStart       20
#define ROMHDR_ulRAMFree        24
#define ROMHDR_ulRAMEnd         28
#define ROMHDR_ulCopyEntries    32
#define ROMHDR_ulCopyOffset     36
#define ROMHDR_ulProfileLen     40
#define ROMHDR_ulProfileOffset  44
#define ROMHDR_numfiles         48
#define ROMHDR_ulObjstoreStart  52
#define ROMHDR_ulObjstoreLen    56
#define ROMHDR_ulDrivglobStart  60
#define ROMHDR_ulDrivglobLen    64
#define ROMHDR_usCPUType        68
#define ROMHDR_usMiscFlags      70
#define ROMHDR_pExtensions       72
#define ROMHDR_ulTrackingStart  76
#define ROMHDR_ulTrackingLen    80
#define SIZEOF_ROMHDR           84
#else
typedef struct ROMHDR {
    ULONG   dllfirst;               // first DLL address
    ULONG   dlllast;                // last DLL address
    ULONG   physfirst;              // first physical address
    ULONG   physlast;               // highest physical address
    ULONG   nummods;                // number of TOCentry's
    ULONG   ulRAMStart;             // start of RAM
    ULONG   ulRAMFree;              // start of RAM free space
    ULONG   ulRAMEnd;               // end of RAM
    ULONG   ulCopyEntries;          // number of copy section entries
    ULONG   ulCopyOffset;           // offset to copy section
    ULONG   ulProfileLen;           // length of PROFentries RAM 
    ULONG   ulProfileOffset;        // offset to PROFentries
    ULONG   numfiles;               // number of FILES
    ULONG   ulKernelFlags;          // optional kernel flags from ROMFLAGS .bib config option
    ULONG   ulFSRamPercent;         // Percentage of RAM used for filesystem 
                                        // from FSRAMPERCENT .bib config option
                                        // byte 0 = #4K chunks/Mbyte of RAM for filesystem 0-2Mbytes 0-255
                                        // byte 1 = #4K chunks/Mbyte of RAM for filesystem 2-4Mbytes 0-255
                                        // byte 2 = #4K chunks/Mbyte of RAM for filesystem 4-6Mbytes 0-255
                                        // byte 3 = #4K chunks/Mbyte of RAM for filesystem > 6Mbytes 0-255

    ULONG   ulDrivglobStart;        // device driver global starting address
    ULONG   ulDrivglobLen;          // device driver global length
    USHORT  usCPUType;              // CPU (machine) Type
    USHORT  usMiscFlags;            // Miscellaneous flags
    PVOID   pExtensions;            // pointer to ROM Header extensions
    ULONG   ulTrackingStart;        // tracking memory starting address
    ULONG   ulTrackingLen;          // tracking memory ending address
} ROMHDR;

typedef struct ROMChain_t {
    struct ROMChain_t *pNext;
    ROMHDR *pTOC;
} ROMChain_t;

#endif //ASM_ONLY

//
// ROM Header extension: PID
//
#define PID_LENGTH 10

#ifdef ASM_ONLY
#define ROMPID_dwPID 0
#define ROMPID_pNextExt (sizeof(DWORD) * PID_LENGTH))
#else
typedef struct ROMPID {
  union{
    DWORD dwPID[PID_LENGTH];        // PID
    struct{
      char  name[(PID_LENGTH - 4) * sizeof(DWORD)];
      DWORD type;
      PVOID pdata;
      DWORD length;
      DWORD reserved;
    };
  };
  PVOID pNextExt;                 // pointer to next extension if any
} ROMPID, EXTENSION;

#endif // ASM_ONLY

//
//  Module Table of Contents - follows ROMHDR in image
//
#ifdef ASM_ONLY
#define TOCentry_dwFileAttributes 0
#define TOCentry_ftTime         4
#define TOCentry_lpszFileSize   12
#define TOCentry_lpszFileName   16
#define TOCentry_ulE32Offset    20
#define TOCentry_ulO32Offset    24
#define TOCentry_ulLoadOffset   28
#define SIZEOF_TOCentry         32
#else
typedef struct TOCentry {           // MODULE BIB section structure
    DWORD dwFileAttributes;
    FILETIME ftTime;
    DWORD nFileSize;
    LPSTR   lpszFileName;
    ULONG   ulE32Offset;            // Offset to E32 structure
    ULONG   ulO32Offset;            // Offset to O32 structure
    ULONG   ulLoadOffset;           // MODULE load buffer offset
} TOCentry, *LPTOCentry;
#endif

//
//  Files Section Structure
//
#ifdef ASM_ONLY
#else
typedef struct FILESentry {         // FILES BIB section structure
    DWORD dwFileAttributes;
    FILETIME ftTime;
    DWORD nRealFileSize;
    DWORD nCompFileSize;
    LPSTR   lpszFileName;
    ULONG   ulLoadOffset;           // FILES load buffer offset
} FILESentry, *LPFILESentry;
#endif


//
//  Copy Section Structure
//
#ifdef ASM_ONLY
#define COPYentry_ulSource      0
#define COPYentry_ulDest        4
#define COPYentry_ulCopyLen     8
#define COPYentry_ulDestLen     12
#define SIZEOF_COPYentry        16
#else
typedef struct COPYentry {
    ULONG   ulSource;               // copy source address
    ULONG   ulDest;                 // copy destination address
    ULONG   ulCopyLen;              // copy length
    ULONG   ulDestLen;              // copy destination length 
                                    // (zero fill to end if > ulCopyLen)
} COPYentry;
#endif

//
//  Profile Code Section Structure
//
#ifdef ASM_ONLY
#define PROFentry_ulModNum      0
#define PROFentry_ulSectNum     4
#define PROFentry_ulStartAddr   8
#define PROFentry_ulEndAddr     12
#define PROFentry_ulHits        16
#define PROFentry_ulNumSym      20
#define PROFentry_ulHitAddress  24
#define PROFentry_ulSymAddress  28
#define SIZEOF_PROFentry        32
#else
typedef struct PROFentry {          // code section profile entry
    ULONG   ulModNum;               // module number in table of contents
    ULONG   ulSectNum;              // section number in o32
    ULONG   ulStartAddr;            // starting address of section
    ULONG   ulEndAddr;              // ending address of section
    ULONG   ulHits;                 // number of hits in section
    ULONG   ulNumSym;               // number of symbols in section
    ULONG   ulHitAddress;           // address to hit table 
    ULONG   ulSymAddress;           // address to symbol table 
} PROFentry;
#endif

//
//  Profile Symbol Section
//
//  Profile symbol section contains function address and hit counter for
//  each function followed by ASCIIZ symbols.
//
#ifdef ASM_ONLY
#define SYMentry_ulFuncAddress  0
#define SYMentry_ulFuncHits     4
#define SIZEOF_SYMentry         8
#else
typedef struct SYMentry {
    ULONG   ulFuncAddress;          // function starting address
    ULONG   ulFuncHits;             // function hit counter
} SYMentry;
#endif

//
// Multiple-XIP data structures
//
#define MAX_ROM                 32      // max numbler of XIPs
#define XIP_NAMELEN             32      // max name length of XIP
#define ROM_CHAIN_OFFSET        0       // offset for XIPCHAIN_INFO

//
// flags for XIP entries
//
#define ROMXIP_OK_TO_LOAD       0x0001
#define ROMXIP_IS_SIGNED        0x0002

#include <pshpack1.h>
//
// XIP chain entry
//
#ifdef ASM_ONLY
#define XIPCHAIN_ENTRY_pvAddr           0
#define XIPCHAIN_ENTRY_dwLength         4
#define XIPCHAIN_ENTRY_dwMaxLength      8
#define XIPCHAIN_ENTRY_usOrder          12
#define XIPCHAIN_ENTRY_usFlags          14
#define XIPCHAIN_ENTRY_dwVersion        16
#define XIPCHAIN_ENTRY_szName           20
#define XIPCHAIN_ENTRY_dwAlgoFlags      (20 + sizeof(CHAR) * XIP_NAMELEN)
#define XIPCHAIN_ENTRY_dwKeyLen         (24 + sizeof(CHAR) * XIP_NAMELEN)
#define XIPCHAIN_ENTRY_byPublicKey      (28 + sizeof(CHAR) * XIP_NAMELEN)
#define SIZEOF_XIPCHAIN_ENTRY           656

#define XIPCHAIN_INFO_cXIPs             0
#define XIPCHAIN_INFO_xipEntryStart     4
#define SIZEOF_XIPCHAIN_INFO            (SIZEOF_XIPCHAIN_ENTRY + 4)

#else
typedef struct _XIPCHAIN_ENTRY {
    LPVOID  pvAddr;                 // address of the XIP
    DWORD   dwLength;               // the size of the XIP
    DWORD   dwMaxLength;            // the biggest it can grow to
    USHORT  usOrder;                // where to put into ROMChain_t
    USHORT  usFlags;                // flags/status of XIP
    DWORD   dwVersion;              // version info
    CHAR    szName[XIP_NAMELEN];    // Name of XIP, typically the bin file's name, w/o .bin
    DWORD   dwAlgoFlags;            // algorithm to use for signature verification
    DWORD   dwKeyLen;               // length of key in byPublicKey
    BYTE    byPublicKey[596];       // public key data
} XIPCHAIN_ENTRY, *PXIPCHAIN_ENTRY;

typedef struct _XIPCHAIN_SUMMARY {
    LPVOID  pvAddr;                 // address of the XIP
    DWORD   dwMaxLength;            // the biggest it can grow to
    USHORT  usOrder;                // where to put into ROMChain_t
    USHORT  usFlags;                // flags/status of XIP
    DWORD   reserved;               // for future use
}XIPCHAIN_SUMMARY, *PXIPCHAIN_SUMMARY;

typedef struct _XIPCHAIN_INFO {
    DWORD cXIPs;
    //
    // may contain more than one entry, but we only need the address of first one
    //
    XIPCHAIN_ENTRY xipEntryStart;
} XIPCHAIN_INFO, *PXIPCHAIN_INFO;
#include <poppack.h>
#endif

//
// Compressed RAM-image data structures
//
#define RAMIMAGE_COMPRESSION_VERSION       5
#define RAMIMAGE_COMPRESSION_BLOCK_SIZE    4096
#define RAMIMAGE_COMPRESSION_SIGNATURE     0x58505253

#ifdef ASM_ONLY
#else
typedef struct __COMPRESSED_RAMIMAGE_HEADER {
    BYTE bReserved[48];
    DWORD dwVersion;
    DWORD dwHeaderSize;
    DWORD dwCompressedBlockCount;
    DWORD dwCompressedBlockSize;
    DWORD dwSignature;              // keep at ROM_OFFSET (64 bytes)
    WORD  wBlockSizeTable[2];       // block size table
} COMPRESSED_RAMIMAGE_HEADER, *PCOMPRESSED_RAMIMAGE_HEADER;
#endif // ASM_ONLY

#ifdef __cplusplus
}
#endif

#endif

