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

    OsAxsFlexi.h

Abstract:

    This header file documents the id numbers and context structures reported
    by the OsAxs FlexiAPI call.

--*/

#pragma once

#ifndef OSAXS_FLEXIAPI_H
#define OSAXS_FLEXIAPI_H

#pragma pack(push, 4)

#define PROCESSOR_FAMILY_X86   (0)
#define PROCESSOR_FAMILY_SH3   (1)
#define PROCESSOR_FAMILY_SH4   (2)
#define PROCESSOR_FAMILY_MIPS  (3)
#define PROCESSOR_FAMILY_ARM   (4)
#define PROCESSOR_FAMILY_PPC   (5)
#define PROCESSOR_FAMILY_SHX   (6) 
#define PROCESSOR_FAMILY_MIPS4 (7) 
#define PROCESSOR_FAMILY_UNK   (0xFFFFFFFF)

#if defined(SH3)
#define TARGET_CODE_CPU (PROCESSOR_FAMILY_SH3)
#elif defined(SH4)
#define TARGET_CODE_CPU (PROCESSOR_FAMILY_SH4)
#elif defined(x86)
#define TARGET_CODE_CPU (PROCESSOR_FAMILY_X86)
#elif defined(MIPS)
#if defined(MIPSIV)
#define TARGET_CODE_CPU (PROCESSOR_FAMILY_MIPS4)
#else
#define TARGET_CODE_CPU (PROCESSOR_FAMILY_MIPS)
#endif
#elif defined(ARM)
#define TARGET_CODE_CPU (PROCESSOR_FAMILY_ARM)
#else
#define TARGET_CODE_CPU (PROCESSOR_FAMILY_UNK)
#endif

//
// Context information
//

#define OSAXS_NUM_ARM_R_REGS 13
typedef struct OsAxsArmIntContext
{
    ULONG32 Psr;
    ULONG32 R[OSAXS_NUM_ARM_R_REGS];
    ULONG32 Sp;
    ULONG32 Lr;
    ULONG32 Pc;
} OsAxsArmIntContext;

#define OSAXS_NUM_ARM_VFP_REGS 32
#define OSAXS_NUM_ARM_EXTRA_CONTROL_REGS 8
typedef struct OsAxsArmVfpContext
{
    ULONG32 Fpscr;
    ULONG32 FpExc;
    ULONG32 S[OSAXS_NUM_ARM_VFP_REGS + 1];                  // fstmx/fldmx requires n+1 registers
    ULONG32 FpExtra[OSAXS_NUM_ARM_EXTRA_CONTROL_REGS];
} OsAxsArmVfpContext;

#define OSAXS_NUM_ARM_CONCAN_WR 16
#define OSAXS_NUM_ARM_CONCAN_WCGR 4
typedef struct OsAxsArmConcanContext
{
    ULONG64 Wr[OSAXS_NUM_ARM_CONCAN_WR];
    ULONG32 WCID;
    ULONG32 WCon;
    ULONG32 WCASF;
    ULONG32 ReservedA[4];
    ULONG32 WCGR[OSAXS_NUM_ARM_CONCAN_WCGR];
    ULONG32 ReservedB[4];
} OsAxsArmConcanContext;

typedef struct OsAxsMips32IntContext
{
    ULONG32 IntAt;
    ULONG32 IntV0;
    ULONG32 IntV1;
    ULONG32 IntA0;
    ULONG32 IntA1;
    ULONG32 IntA2;
    ULONG32 IntA3;
    ULONG32 IntT0;
    ULONG32 IntT1;
    ULONG32 IntT2;
    ULONG32 IntT3;
    ULONG32 IntT4;
    ULONG32 IntT5;
    ULONG32 IntT6;
    ULONG32 IntT7;
    ULONG32 IntS0;
    ULONG32 IntS1;
    ULONG32 IntS2;
    ULONG32 IntS3;
    ULONG32 IntS4;
    ULONG32 IntS5;
    ULONG32 IntS6;
    ULONG32 IntS7;
    ULONG32 IntT8;
    ULONG32 IntT9;
    ULONG32 IntK0;
    ULONG32 IntK1;
    ULONG32 IntGp;
    ULONG32 IntSp;
    ULONG32 IntS8;
    ULONG32 IntRa;
    ULONG32 IntLo;
    ULONG32 IntHi;

    ULONG32 IntFir;
    ULONG32 IntPsr;
} OsAxsMips32IntContext;

#define OSAXS_NUM_MIPS_FPU_REGS 32
typedef struct OsAxsMips32FpContext
{
    ULONG32 FltF[OSAXS_NUM_MIPS_FPU_REGS];
    ULONG32 Fsr;
} OsAxsMips32FpContext;

typedef struct OsAxsMips64IntContext
{
    ULONG64 IntAt;
    ULONG64 IntV0;
    ULONG64 IntV1;
    ULONG64 IntA0;
    ULONG64 IntA1;
    ULONG64 IntA2;
    ULONG64 IntA3;
    ULONG64 IntT0;
    ULONG64 IntT1;
    ULONG64 IntT2;
    ULONG64 IntT3;
    ULONG64 IntT4;
    ULONG64 IntT5;
    ULONG64 IntT6;
    ULONG64 IntT7;
    ULONG64 IntS0;
    ULONG64 IntS1;
    ULONG64 IntS2;
    ULONG64 IntS3;
    ULONG64 IntS4;
    ULONG64 IntS5;
    ULONG64 IntS6;
    ULONG64 IntS7;
    ULONG64 IntT8;
    ULONG64 IntT9;
    ULONG64 IntK0;
    ULONG64 IntK1;
    ULONG64 IntGp;
    ULONG64 IntSp;
    ULONG64 IntS8;
    ULONG64 IntRa;
    ULONG64 IntLo;
    ULONG64 IntHi;

    ULONG32 IntFir;
    ULONG32 IntPsr;
   
} OsAxsMips64IntContext;

typedef struct OsAxsMips64FpContext
{
    ULONG64 FltF[OSAXS_NUM_MIPS_FPU_REGS];
    ULONG32 Fsr;
} OsAxsMips64FpContext;

#define OSAXS_NUM_SHX_R_REGS 16
typedef struct OsAxsShxIntContext
{
    ULONG32 PR;
    ULONG32 MACH;
    ULONG32 MACL;
    ULONG32 GBR;
    ULONG32 R[OSAXS_NUM_SHX_R_REGS];
    ULONG32 Fir;
    ULONG32 Psr;
} OsAxsShxIntContext;


#define OSAXS_NUM_SH4_F_REGS 16
#define OSAXS_NUM_SH4_XF_REGS 16
typedef struct OsAxsSh4FpContext
{
    ULONG32 Fpscr;
    ULONG32 Fpul;
    ULONG32 FRegs[OSAXS_NUM_SH4_F_REGS];
    ULONG32 xFRegs[OSAXS_NUM_SH4_XF_REGS];
} OsAxsSh4FpContext;


typedef struct OsAxsX86IntContext
{
    ULONG32 Gs;
    ULONG32 Fs;
    ULONG32 Es;
    ULONG32 Ds;
    ULONG32 Edi;
    ULONG32 Esi;
    ULONG32 Ebx;
    ULONG32 Edx;
    ULONG32 Ecx;
    ULONG32 Eax;
    ULONG32 Ebp;
    ULONG32 Eip;
    ULONG32 Cs;
    ULONG32 Eflags;
    ULONG32 Esp;
    ULONG32 Ss;
} OsAxsX86IntContext;


/* X86 has 8 80-bit FP registers.  No native 80-bit type. */
#define OSAXS_SIZE_OF_X86_FPU_REGS 80
typedef struct OsAxsX86FpContext
{
    ULONG32 ControlWord;
    ULONG32 StatusWord;
    ULONG32 TagWord;
    ULONG32 ErrorOffset;
    ULONG32 ErrorSelector;
    ULONG32 DataOffset;
    ULONG32 DataSelector;
    BYTE RegisterArea[OSAXS_SIZE_OF_X86_FPU_REGS];
    DWORD Cr0NpxState;
} OsAxsX86FpContext;


// Process field identifiers
typedef
    enum _PROC_FIELD_IDS
    {
        pfiStructAddr       = (0L), // address to the process structure itself
        pfiProcessSlot      = (1L), // Slot number
        pfiStartOfAddrSpace = (2L), // VM Address space (slot) first address
        pfiDefaultAccessKey = (3L), // Default thread Access keys
        pfiBasePtr          = (4L), // First exe module load address
        pfiCurDbgZoneMasks  = (5L), // Current Debug Zone mask
        pfiName             = (6L), // EXE Name
        pfiCmdLine          = (7L), // Command line
        pfiTrustLevel       = (8L), // Trust level
        pfiHandle           = (9L), // Process handle
        pfiTlsUsageBitMaskL = (10L), // First 32 TLS slots usage bit mask
        pfiTlsUsageBitMaskH = (11L), // Second 32 TLS slots usage bit mask
        pfiUserDefined      = (-1L)  // field identified by its label
    } 
    PROC_FIELD_IDS;


// Thread field identifiers
typedef
    enum _THREAD_FIELD_IDS
    {
         tfiStructAddr                  = (0L), // address to the thread structure itself
         tfiRunState                    = (1L), // Running / Sleeping / Blocked / Killed states of the thread
         tfiAddrSpaceAccessKey          = (2L), // Current access key for handles and memory access
         tfiHandleCurrentProcessRunIn   = (3L), // Current process running in
         tfiSleepCount                  = (4L), // Sleep count
         tfiSuspendCount                = (5L), // Suspend count
         tfiCurrentPriority             = (6L), // Current priority
         tfiInfo                        = (7L), // Information status bits
         tfiBasePriority                = (8L), // Base priority
         tfiWaitState                   = (9L), // Wait state
         tfiHandleOwnerProc             = (10L), // Handle to the process owning the thread
         tfiTlsPtr                      = (11L), // Thread local storage block pointer
         tfiKernelTime                  = (12L), // Accumulated time spend in kernel mode
         tfiUserTime                    = (13L), // Accumulated time spend in user mode
         tfiHandle                      = (14L), // Thread handle
         tfiLastError                   = (15L), // Last error
         tfiStackBase                   = (16L), // Stack base address
         tfiStackLowBound               = (17L), // Lower bound of commited stack space
         tfiCreationTimeMSW             = (18L), // MSW of Creation timestamp
         tfiCreationTimeLSW             = (19L), // LSW of Creation timestamp
         tfiQuantum                     = (20L), // Quantum
         tfiQuantumLeft                 = (21L), // Quantum left
         tfiPC                          = (22L), // Program Counter / Instruction Pointer
         tfiNcrPtr                      = (23L), // Pointer to Exception List on x86
         tfiStkRetAddr                  = (24L), // Return Address
         tfiUserDefined                 = (-1L) // field identified by its label
    } 
    THREAD_FIELD_IDS;


// Module field identifiers
typedef
    enum _MODULE_FIELD_IDS
    {
        mfiName                         = 0,
        mfiBasePointer,
        mfiModuleSize,
        mfiRdWrDataStart,
        mfiRdWrDataEnd,
        mfiTimeStamp,
        mfiPdbFormat,
        mfiPdbGuid,
        mfiPdbAge,
        mfiDllHandle,
        mfiInUse,
        mfiFlags,
        mfiTrustLevel,
        mfiRefCount,
        mfiStructAddr,
        mfiUserDefined                  = (-1L) // field identified by its label
    }
    MODULE_FIELD_IDS;

typedef
    enum _CONTEXT_FIELD_IDS
    {
        cfiThreadId                     = 0,
        cfiArmConcanCxt,
        cfiArmIntCxt,
        cfiArmVfpCxt,
        cfiMips32FpCxt,
        cfiMips32IntCxt,
        cfiMips64FpCxt,
        cfiMips64IntCxt,
        cfiSHxIntCxt,
        cfiSH4FpCxt,
        cfiX86IntCxt,
        cfiX86FpCxt,
        cfiUserDefined                  = (-1L)
    } CONTEXT_FIELD_IDS;


typedef enum _CONTEXT_FIELD_SIZES
{
    cfsThreadId = sizeof (DWORD),
    cfsArmConcanCxt = sizeof (OsAxsArmConcanContext),
    cfsArmIntCxt = sizeof (OsAxsArmIntContext),
    cfsArmVfpCxt = sizeof (OsAxsArmVfpContext),
    cfsMips32FpCxt = sizeof (OsAxsMips32FpContext),
    cfsMips32IntCxt = sizeof (OsAxsMips32IntContext),
    cfsMips64FpCxt = sizeof (OsAxsMips64FpContext),
    cfsMips64IntCxt = sizeof (OsAxsMips64IntContext),
    cfsSHxIntCxt = sizeof (OsAxsShxIntContext),
    cfsSH4FpCxt = sizeof (OsAxsSh4FpContext),
    cfsX86IntCxt = sizeof (OsAxsX86IntContext),
    cfsX86FpCxt = sizeof (OsAxsX86FpContext),
} CONTEXT_FIELD_SIZES;

#pragma pack(pop)

#endif
