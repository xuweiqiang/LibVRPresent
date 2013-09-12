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

    fwtblidx.h

Abstract:

    include file defines firmware entries.

Notes:


--*/
#ifndef _FWTBLIDX_H_
#define _FWTBLIDX_H_    1

#ifdef __cplusplus
extern "C" {
#endif

#define _FWTBL_FLUSHI           1
#define _FWTBL_FLUSHD           2
#define _FWTBL_OUTPUTS          3
#define _FWTBL_OUTPUTFORMATS    4
#define _FWTBL_SERIALREAD       5
#define _FWTBL_SERIALWRITE      6
#define _FWTBL_PARALLELREAD     7
#define _FWTBL_PARALLELWRITE    8
#define _FWTBL_ENTERDEBUGGER    9

//
// Firmware table entry
//
#if MIPS
#define FWBASE  0xad400270
#else
#define FWBASE  0xa0001000
#endif

#ifndef FWBASE
#error FWBASE not defined
#endif

#if MIPS
#define FWTBL_FLUSHI        (8*_FWTBL_FLUSHI + FWBASE)
#define FWTBL_FLUSHD        (8*_FWTBL_FLUSHD + FWBASE)
#define FWTBL_OUTPUTS       (8*_FWTBL_OUTPUTS + FWBASE)
#define FWTBL_OUTPUTFORMATS (8*_FWTBL_OUTPUTFORMATS + FWBASE)
#define FWTBL_SERIALREAD    (8*_FWTBL_SERIALREAD + FWBASE)
#define FWTBL_SERIALWRITE   (8*_FWTBL_SERIALWRITE + FWBASE)
#define FWTBL_PARALLELREAD  (8*_FWTBL_PARALLELREAD + FWBASE)
#define FWTBL_PARALLELWRITE (8*_FWTBL_PARALLELWRITE + FWBASE)
#define FWTBL_ENTERDEBUGGER (8*_FWTBL_ENTERDEBUGGER + FWBASE)
#else
#define FWTBL_OUTPUTS       (8*_FWTBL_OUTPUTS + FWBASE)
#define FWTBL_OUTPUTFORMATS (8*_FWTBL_OUTPUTFORMATS + FWBASE)
#define FWTBL_SERIALREAD    (8*_FWTBL_SERIALREAD + FWBASE)
#define FWTBL_SERIALWRITE   (8*_FWTBL_SERIALWRITE + FWBASE)
#define FWTBL_PARALLELREAD  (8*_FWTBL_PARALLELREAD + FWBASE)
#define FWTBL_PARALLELWRITE (8*_FWTBL_PARALLELWRITE + FWBASE)
#endif

#ifdef __cplusplus
}
#endif

#endif  // #ifndef  _FWTBLIDX_H_
