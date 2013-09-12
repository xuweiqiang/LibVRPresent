//////////////////////////////////////////////////////////////////////
// 
// Settings.h
//
// Contains application constants.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) CIDANA Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct ColorInfo
{
    BOOL    bBT709;             // If FALSE, YCbCr is BT-601
    BOOL    bRgbRange16_235;    // If FALSE, RGB is full range
};

// Frame rate
const UINT VIDEO_FPS     = 60;
const UINT VIDEO_MSPF    = (1000 + VIDEO_FPS / 2) / VIDEO_FPS;
const UINT VIDEO_100NSPF = VIDEO_MSPF * 10000;

// Formats
const D3DFORMAT VIDEO_RENDER_TARGET_FORMAT = D3DFMT_X8R8G8B8;

const UINT VIDEO_SUB_VX  = 3;   // horizontal velocity
const UINT VIDEO_SUB_VY  = 2;   // vertical velocity

// Default alpha values
const BYTE DEFAULT_PLANAR_ALPHA_VALUE = 0xFF;
const BYTE DEFAULT_PIXEL_ALPHA_VALUE  = 0x80;

// Miscellaneous
const UINT DWM_BUFFER_COUNT  = 4;

// Settings for color-space conversions.
const ColorInfo EX_COLOR_INFO[] =
{
    // SDTV ITU-R BT.601 YCbCr to studio RGB [16...235]
    {FALSE, TRUE},

    // SDTV ITU-R BT.601 YCbCr to computer RGB [0...255]
    {FALSE, FALSE},

    // HDTV ITU-R BT.709 YCbCr to studio RGB [16...235]
    {TRUE, TRUE},

    // HDTV ITU-R BT.709 YCbCr to computer RGB [0...255]
    {TRUE, FALSE}
};

const DWORD NUM_EX_COLORS = ARRAYSIZE(EX_COLOR_INFO);

typedef enum 
{
	eCOLOR_WHITE,
	eCOLOR_RED,
	eCOLOR_YELLOW,
	eCOLOR_GREEN,
	eCOLOR_CYAN,
	eCOLOR_BLUE,
	eCOLOR_MAGENTA,
	eCOLOR_BLACK,
}eBACK_COLOR;

// Background colors
const DXVAHD_COLOR_RGBA BACKGROUND_COLORS[] =
{
    // White         // Red         // Yellow       // Green     
    { 1, 1, 1, 1 }, { 1, 0, 0, 1 }, { 1, 1, 0, 1 }, { 0, 1, 0, 1},

    // Cyan         // Blue         // Magenta      // Black
    { 0, 1, 1, 1 }, { 0, 0, 1, 1 }, { 1, 0, 1, 1 }, { 0, 0, 0, 0 }

};

const DWORD NUM_BACKGROUND_COLORS = ARRAYSIZE(BACKGROUND_COLORS);

// ProcAmp filters
const DXVAHD_FILTER PROCAMP_FILTERS[] = 
{
    DXVAHD_FILTER_BRIGHTNESS,
    DXVAHD_FILTER_CONTRAST,
    DXVAHD_FILTER_HUE,
    DXVAHD_FILTER_SATURATION
};

const DWORD NUM_FILTERS = ARRAYSIZE(PROCAMP_FILTERS); 
