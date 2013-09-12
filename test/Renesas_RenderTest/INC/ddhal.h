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
//------------------------------------------------------------------------------
//
//  Header:  ddhal.h
//
//  This header defines structures and constant for HAL/OAL based flat 
//  display drivers. IOCTL codes are defined in pkfuncs.h file.
//
#ifndef __DDHAL_IOCTL_H
#define __DDHAL_IOCTL_H

#if __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
//
//  Define:  DDHAL_xxx
//
//  Define function required by display driver to be provided by HAL.
//
#define DDHAL_COMMAND_GET_NUM_MODES     1
#define DDHAL_COMMAND_GET_MODE_INFO     2
#define DDHAL_COMMAND_SET_MODE          3
#define DDHAL_COMMAND_SET_PALETTE       4
#define DDHAL_COMMAND_POWER             5

//------------------------------------------------------------------------------
//
//  Define:  EGPE_FORMAT_xxx
//
//  This is equivalent of EGPEFormat enumeration. We use separate definition
//  to avoid include display driver related header files to HAL/OAL code.
//
#define EGPE_FORMAT_1BPP                0
#define EGPE_FORMAT_2BPP                1
#define EGPE_FORMAT_4BPP                2
#define EGPE_FORMAT_8BPP                3
#define EGPE_FORMAT_16BPP               4
#define EGPE_FORMAT_24BPP               5
#define EGPE_FORMAT_32BPP               6
#define EGPE_FORMAT_16YCRCB             7
#define EGPE_FORMAT_COMPATIBLE          8
#define EGPE_FORMAT_UNDEFINED           9

//------------------------------------------------------------------------------
//
//  Define:  EGPE_FORMAT_xxx
//
//  This is equivalent of EGPEFormat enumeration. We use separate definition
//  to avoid include display driver related header files to HAL/OAL code.
//
#define EGPE_FORMAT_1BPP                0
#define EGPE_FORMAT_2BPP                1
#define EGPE_FORMAT_4BPP                2
#define EGPE_FORMAT_8BPP                3
#define EGPE_FORMAT_16BPP               4
#define EGPE_FORMAT_24BPP               5
#define EGPE_FORMAT_32BPP               6
#define EGPE_FORMAT_16YCRCB             7
#define EGPE_FORMAT_COMPATIBLE          8
#define EGPE_FORMAT_UNDEFINED           9

//------------------------------------------------------------------------------
//
//  Define:  PAL_xxx
// 
#define PAL_INDEXED                     0x00000001
#define PAL_BITFIELDS                   0x00000002
#define PAL_RGB                         0x00000004
#define PAL_BGR                         0x00000008

//------------------------------------------------------------------------------
//
//  Define:  DDHAL_xxx
//
//  This values define flags passed in mode info
// 

// Hardware supports display rotation
#define DDHAL_HW_ROTATE                 (1 << 0)

// Frame buffer can be mapped as cached memory
#define DDHAL_CACHE_FRAME_MEMORY        (1 << 1)

//------------------------------------------------------------------------------
//
//  Type: DDHAL_MODE_INFO
//
//  This structure is used on output in IOCTL_HAL_DDI_GET_MODE_INFO call.
//  It is followed by palette colors (each color is DWORD/UINT32 and count
//  is specified by entries parameter).
//
typedef struct {
    UINT32 modeId;              // Mode identification
    UINT32 flags;               // Option flags
    UINT32 width;               // Display width in pixels
    UINT32 height;              // Display height in pixels
    UINT32 bpp;                 // Color deep (bits per pixel)
    UINT32 frequency;           // Mode refresh rate
    UINT32 format;              // EGPE color format
    UINT64 phFrameBase;         // Frame buffer physical address
    UINT32 frameSize;           // Frame buffer size
    UINT32 frameStride;         // Frame stride (bytes per line)
    UINT32 paletteMode;         // Palette mode
    UINT32 entries;             // Number of color entries
} DDHAL_MODE_INFO;

//------------------------------------------------------------------------------
//
//  Type: DDHAL_SET_PALETTE
//
//  This structure is used on input in IOCTL_HAL_DDI_SET_PALETTE call. It is
//  followed by palette colors (each color is DWORD/UINT32 and count is
//  specified by entries parameter).
//
typedef struct {
    UINT16 firstEntry;
    UINT16 entries;
} DDHAL_SET_PALETTE;

//------------------------------------------------------------------------------

#if __cplusplus
}
#endif

#endif
