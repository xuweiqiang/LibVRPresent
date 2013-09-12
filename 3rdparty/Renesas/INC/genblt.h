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

    genblt.h

Abstract:

    Functions, structures, and definitions used by GeneratedBlt.

--*/

#ifndef __GENBLT_H__
#define __GENBLT_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//
// This structure is limited to using 5 bits.
// Note that the ARGB/ABGR/BGR/RGB tags represent the order within a DWORD
// and do not correspond to the standard Windows RGB/BGR ordering.
//
enum GenBltFormat
{
    Format_Invalid    = 0,
    Format_ARGB8888   = 1,
    Format_ABGR8888   = 2,
    Format_RGB888     = 3,
    Format_BGR888     = 4,
    Format_RGB565     = 5,
    Format_RGB555     = 6,
    Format_Palette    = 7,
    Format_Mono       = 8,
    Format_Masks      = 9,
};

enum GenBltMemoryType
{
    VideoMemory       = 0,
    SystemMemory      = 1
};

enum GenBltColorKeyType
{
    NoColorKey        = 0,
    SrcColorKey       = 1,
    DstColorKey       = 2,
    SrcAndDstColorKey = 3
};

enum GenBltPatternType
{
    SolidPattern      = 0,
    BitmapPattern     = 1
};

enum GenBltAlphaType
{
    NoAlpha           = 0,
    SrcConstAlpha     = 1,
    SrcAlpha          = 2,
    BothAlpha         = 3
};

enum GenBltStretchType
{
    NoStretchOrShrink = 0,
    StretchX          = 1,
    StretchY          = 2,
    ShrinkX           = 4,
    ShrinkY           = 8
};

struct GenBltColorSpace
{
    ULONG LowValue;
    ULONG HighValue;
};

// The GenBltSignature structure is 48 bits wide.  The first 32 bits represent
// information about the blt, and the last 16 bits represent the ROP4 code.
struct GenBltSignature
{
    union
    {
        struct
        {
            union
            {
                struct
                {
                    DWORD DstFormat     : 4;  //  0 -  3
                    DWORD SrcFormat     : 4;  //  4 -  7
                    DWORD BltMode       : 5;  //  8 - 12
                    DWORD PatternType   : 1;  // 13
                    DWORD ScanXPositive : 1;  // 14
                    DWORD ScanYPositive : 1;  // 15
                    DWORD UseStretch    : 4;  // 16 - 19
                    DWORD UseAlpha      : 2;  // 20 - 21
                    DWORD ColorKeyType  : 2;  // 22 - 23
                    DWORD DstMemory     : 1;  // 24
                    DWORD SrcMemory     : 1;  // 25
                    DWORD PatMemory     : 1;  // 26
                    DWORD MaskMemory    : 1;  // 27
                    DWORD MirrorX       : 1;  // 28
                    DWORD MirrorY       : 1;  // 29
                    DWORD Lookup        : 1;  // 30
                    DWORD Convert       : 1;  // 31
                };

                DWORD Dword;
            };

            // This holds information about the source and destination bpp.
            union
            {
                struct
                {
                WORD Dst8Bpp  : 1;
                WORD Dst16Bpp : 1;
                WORD Dst24Bpp : 1;
                WORD Dst32Bpp : 1;
                WORD Src24Bpp : 1;
                };

                WORD Word;
            };

            // This holds the ROP4 information.
            union
            {
                struct
                {
                    BYTE ForegroundRop;
                    BYTE BackgroundRop;
                };

                WORD Rop;
            };

        };

        ULONGLONG Ulonglong;
    };
};

// This function pointer type is used to describe the blt functions.

struct PCodeInstruction_t;
struct PCodeRegisters_t;
struct GPEBltParms;
enum   EGPEFormat;

typedef void (*PFN_BltFunction)(PCodeRegisters_t * pRegs, void * pMCode);

// This is the generic blt function
struct GenBltPCode
{
    GenBltSignature Sig;
    DWORD           Length;
    DWORD           pCode[1024];
};

struct GenBltFunction
{
    GenBltSignature   Sig;
    int               Index;
    BYTE            * pCpuCode;
    PFN_BltFunction   pfnBlt;
    DWORD             LastUsed;
    DWORD             Optimized;
    DWORD             UseCount;
    DWORD             Length;
};

// This structure represents the functions that are CPU specific.
typedef PFN_BltFunction (*PFN_PCodeToCpuCode)(int FnIndex, DWORD * pPCode, GenBltSignature * pBltSig, void * pBltFn);
typedef BOOL            (*PFN_CpuOptimize)(int FnIndex, void * pMCode, DWORD Level);

#define GENBLT_OPTIMIZE_LEVELS 4

struct GenBltCpuFuncs
{
    PFN_PCodeToCpuCode pfnPCodeToCpuCode;
    PFN_CpuOptimize    pfnCpuOptimize;
    DWORD              dwOptimizeTimes[GENBLT_OPTIMIZE_LEVELS];
};

// One page is always 4K in Windows CE.

#define ONE_PAGE 4096

// 16 is a magic number. We know that VirtualAlloc, when requesting a single
// 4K page, will actually return 64K. However, we only need/want a single
// page per function. Therefore, we use our 64K to populate the entry cache.

#define CACHE_ENTRIES 16

// The first three entries in the cache are for SrcCopy, SolidFill, and
// the text ROP (0xAAF0) with the bit-depth of the primary.

#define CACHE_SOLID_FILL 0
#define CACHE_SRCCOPY    1
#define CACHE_TEXT_OUT   2
#define CACHE_BEGIN      3

//
// Functions exported from the CPU specific portion of GenBlt.
//

extern
BOOL
GenBltCpuInitialize(
    GenBltCpuFuncs * pCpuFuncs
    );

//
// GeneratedBlt functions.
//

extern
BOOL
GenBltInitialize();

extern
BOOL
GenBltSetMode(
    EGPEFormat   GpeFormat,
    ULONG      * pMasks
    );

extern
DWORD
InstructionFetchAndDecode(
    DWORD              * pdwPCode,
    PCodeInstruction_t * pInstr
    );

extern
void
GeneratedBltSelect(
    GPEBltParms * pParms
    );

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __GENBLT_H_
