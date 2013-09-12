/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 1995-1998  Microsoft Corporation
Copyright (c) 2000,2003  BSQUARE Corporation.  All rights reserved.

Module Name:
    bootarg.h

Abstract:  
    Definitions for arguments passed by loadcepc at boot time.
    
Notes: 

Revision History:

     9-Mar-2000 Helmut Tischer

        -   Added some Bitfields for Debugging Communication across serial/parallel to BOOT_ARGS

--*/
#ifndef _BOOTARG_H
#define _BOOTARG_H
#include <halether.h>

// Driver globals version (should be updated on *any* driver globals structure change).
//
#define DRVGLB_MAJOR_VER        1
#define DRVGLB_MINOR_VER        0

typedef struct _BOOT_ARGS {
    USHORT MajorVer;              // Driver globals major version.
    USHORT MinorVer;              // Driver globals minor version.

    UCHAR ucVideoMode;

    //
    // We stuff additional settings into the original ucComPort field.
    // Define values that way, that loadcepc provided values match the original behaviour.
    //      (loadcepc provides 0 for ucParPort..ucCeshCom)
    // ucComPort: 00=COM4_BASE 01=COM1_BASE 10=COM2_BASE 11=COM3_BASE
    //      ( Offset above is for compatility to LOADCEPC values.
    //        Wrap around to 00 for COM4_BASE to fit this in 2 bits )
    // ucParPort: 00=PARALLEL1_BASE 01=PARALLEL2_BASE 10=PARALLEL3_BASE
    // ucComDisable: 1=Disable serial debug communication. 0=Enable
    // ucParDisable: 1=Disable parallel debug communication.  0=Enable
    // ucdbgprintfPar: 1=redirect dbgprintf to DebugParallel. 0=send dbgprintf to DebugSerial
    // ucCeshCom: 1=redirect cesh communication over DebugSerial. 0=over DebugParallel
    //
    // Note:
    //  A bit field consumes always so many Bytes as indicated by the base type.
    //  So be sure to use UCHAR to fit everything in a single Byte, in order to not break
    //  memory layout of the BOOT_ARGS structure.
    //  Unnamed struct is only for clarification - works also without this braces.
    //
    // UCHAR   ucComPort;
    //

    struct {
        UCHAR ucComPort:2;
        UCHAR ucParPort:2;
        UCHAR ucComDisable:1;
        UCHAR ucParDisable:1;
        UCHAR ucdbgprintfPar:1;
        UCHAR ucCeshCom:1;
    };

    UCHAR   ucBaudDivisor;
    UCHAR   ucPCIConfigType;

    // The following args are not set by older versions of loadcepc,
    // so include a sig to verify that the remaining params are valid. 
    // Also, include a length to allow expansion in the future.
    DWORD   dwSig;
    #define BOOTARG_SIG  0x544F4F42 // "BOOT"    
    DWORD   dwLen;              // Total length of boot args struct
    UCHAR   ucLoaderFlags;      // Flags set by loader
    UCHAR   ucEshellFlags;      // Flags from eshell
    UCHAR   ucEdbgAdapterType;  // Type of debug Ether adapter
    UCHAR   ucEdbgIRQ;          // IRQ line to use for debug Ether adapter
    DWORD   dwEdbgBaseAddr;     // Base I/O address for debug Ether adapter
    DWORD   dwEdbgDebugZone;    // Allow EDBG debug zones to be turned on from loadcepc
    // The following is only valid if LDRFL_ADDR_VALID is set
    EDBG_ADDR EdbgAddr;         // IP/ether addr to use for debug Ethernet
    // The following addresses are only valid if LDRFL_JUMPIMG is set, and corresponding bit in
    // ucEshellFlags is set (configured by eshell, bit definitions in ethdbg.h).
    EDBG_ADDR EshellHostAddr;   // IP/ether addr and UDP port of host running eshell
    EDBG_ADDR DbgHostAddr;      // IP/ether addr and UDP port of host receiving dbg msgs
    EDBG_ADDR CeshHostAddr;     // IP/ether addr and UDP port of host running ether text shell
    EDBG_ADDR KdbgHostAddr;     // IP/ether addr and UDP port of host running kernel debugger
    ULONG     NICBase;          // Indicate either MAC 0 or MAC 1
    UCHAR     DeviceName[32];   // The name used be eboot
    DWORD     KitlTransport;    // Transport to use for KITL
    DWORD     DHCPLeaseTime;
    DWORD     SubnetMask;
    DWORD     dwLaunchAddr;  // Store the last used jump address
    DWORD     BootupType;       // Store the last boot type - Cold/Warm/Resume
} BOOT_ARGS, *PBOOT_ARGS;


#endif // _BOOTARG_H
