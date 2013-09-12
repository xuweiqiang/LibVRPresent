/*************************************************************************
**
** STGLIB, Accelerated 2D Graphics Library for Cartesio Plus SGA Engine.
**
** Copyright (C) 2009-2010 STMicroelectronics.  All rights reserved.
**
** This software is the confidential and/or proprietary information of
** STMicroelectronics. You shall not disclose such information and shall
** use it only in accordance with the terms of the license agreement you
** entered into with STMicroelectronics.
**
*************************************************************************/

#ifndef __STGLIB_H
#define __STGLIB_H

typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HWND;
typedef unsigned int DWORD;
typedef unsigned char bool;

#define STGLIB_API_CALL

// Basic types definition
typedef unsigned long		sga_instruction;
typedef short				sga_coordinate;
typedef unsigned long		sga_address;
typedef unsigned char		sga_bool;
typedef signed long long	sga_int64;
typedef unsigned long long	sga_uint64;
typedef signed int			sga_int32;
typedef unsigned int		sga_uint32;
typedef signed short		sga_int16;
typedef unsigned short		sga_uint16;
typedef signed char			sga_int8;
typedef unsigned char		sga_uint8;

typedef HANDLE			STHDC;
typedef HANDLE			STHBITMAP;
typedef HANDLE			STHFONT;

typedef struct {
	sga_coordinate		x;
	sga_coordinate		y;
} t_vtx;

typedef struct {
	sga_coordinate		xW;
	sga_coordinate		yW;
} t_width;

typedef struct {
	sga_int32			length;
	t_width				*width;
	t_vtx				*vtx;
} t_polygonList;

typedef struct {
	sga_uint32			charNum;			// The number of chars in the font
	sga_uint32			charStep;			// The distance in pixels between two characters (X axis)
	sga_uint32			charHeight;			// The Height of one character
	sga_uint32			charWidth;			// The Width of each character
	sga_bool			fontTransparency;	// Boolean
	sga_uint8			alphaMax;
	sga_uint8			alphaMin;
	sga_uint32			color;				// Font color
	sga_uint32			bkGround;
	STHBITMAP			m_fontBitmap;		// The bitmap containing the font pixels
} t_sgaFont;

typedef struct {
	sga_uint32			bufferFmt;
	sga_uint8			colorDepth;
} t_sgaColorFormat;

typedef struct {
	sga_uint32			Triangles;
	sga_uint32			FragmentsRaw;
	sga_uint32			FragmentsZTested;
	sga_uint32			TexelsFetched;
	sga_uint32			PixelsReads;
	sga_uint32			TextureCacheMiss;
	sga_uint32			InstrCacheMiss;
	sga_uint32			CyclesCounter;
} t_sgaStatistics;

typedef enum {
	STSRCCOPY,
	STSCREENMIRROR,
	STSRCBLEND,
	STSRCBLENDROTATED,
	STSRCBLENDSTRETCHED,
	STSRCTRANSPCOLOR,
} STROP;

typedef enum {
	STPOLYFAN,
	STPOLYLIST,
	STPOLYSTRIP,
} STPOLYTYPE;

// Sga pixels types
#define SGA_PIX_FORMAT_MONO_1BPP			0x00
#define SGA_PIX_FORMAT_MONO_2BPP			0x01
#define SGA_PIX_FORMAT_MONO_4BPP			0x02
#define SGA_PIX_FORMAT_MONO_8BPP			0x03
#define SGA_PIX_FORMAT_RGB8					0x04
#define SGA_PIX_FORMAT_RGBA15				0x07   // 5-5-5-1
#define SGA_PIX_FORMAT_RGB16				0x08   // 5-6-5
#define SGA_PIX_FORMAT_ARGB15				0x09   // 1-5-5-5
#define SGA_PIX_FORMAT_ARGB24				0x0A   // A888, can be packed as 888, see below
#define SGA_PIX_FORMAT_RGBA24				0x0B   // 888A, actually rgb32
#define SGA_PIX_FORMAT_YUV422				0x0C
#define SGA_PIX_FORMAT_YUV420				0x40

// Sga display types
static const t_sgaColorFormat SGA_DIS_FMT_MONO_8BPP 	= { SGA_PIX_FORMAT_MONO_8BPP, 1 };  //BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGB565		= { SGA_PIX_FORMAT_RGB16, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGBA15		= { SGA_PIX_FORMAT_RGBA15, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_ARGB15		= { SGA_PIX_FORMAT_ARGB15, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGB888		= { SGA_PIX_FORMAT_ARGB24 | 0x4000, 3 }; //BpP
static const t_sgaColorFormat SGA_DIS_FMT_ARGB8888		= { SGA_PIX_FORMAT_ARGB24, 4 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGBA8888		= { SGA_PIX_FORMAT_RGBA24, 4 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_YUV422		= { SGA_PIX_FORMAT_YUV422, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_YUV420		= { SGA_PIX_FORMAT_YUV420, 2 };		//BpP

#if __cplusplus
extern "C" {
#endif

STGLIB_API_CALL STHDC		STCreateDC(HWND hWnd, int nBackBuffers, void** address);
STGLIB_API_CALL STHDC		STCreateCompatibleDC(int m_surfCX, int m_surfCY);
STGLIB_API_CALL STHDC		STGetDC(HWND hWnd);
STGLIB_API_CALL int			STReleaseDC(STHDC hDC);

STGLIB_API_CALL int			STFlush(STHDC hDC);
STGLIB_API_CALL int			STFinish(STHDC hDC);
STGLIB_API_CALL int			STFlip(STHDC hDC);
STGLIB_API_CALL int			STIsBltDone(STHDC hDC);

STGLIB_API_CALL STHBITMAP	STCreateBitmap(STHDC hDC, int nWidth, int nHeight, t_sgaColorFormat format, const void *lpvBits);
STGLIB_API_CALL STHBITMAP	STCreateBitmapFromClientBuffer(int nWidth, int nHeight, t_sgaColorFormat format, void *virtualAddress);
STGLIB_API_CALL STHBITMAP	STLoadBitmap(STHDC hDC, int nWidth, int nHeight, t_sgaColorFormat format, const char *fname);
STGLIB_API_CALL int			STDestroyBitmap(STHDC hDC, STHBITMAP hBitmap);

STGLIB_API_CALL int			STBindBitmapToVideoIn(STHBITMAP buffer);
STGLIB_API_CALL int			STUnbindBitmapToVideoIn(STHBITMAP buffer);

STGLIB_API_CALL int			STSetCropWindow(STHBITMAP pSrc, const int x, const int y, const int w, const int h);

STGLIB_API_CALL STHFONT		STCreateFont(STHDC hDC, t_sgaFont *font, STHBITMAP fontBitmap);
STGLIB_API_CALL int			STModifyFont(STHDC hDC, STHFONT hFont, t_sgaFont font);
STGLIB_API_CALL int			STDestroyFont(STHDC hDC, STHFONT hFont);

STGLIB_API_CALL int			STBitBlt(STHDC hDC, int x0, int y0, int x1, int y1, STHBITMAP pSrc, int nXSrc, int nYSrc, STROP dwRop);
STGLIB_API_CALL int			STFillRect(STHDC hDC, int x0, int y0, int x1, int y1, sga_uint32 color);
STGLIB_API_CALL int			STDrawTextMap(STHDC hDC, int x0, int y0, STHBITMAP pSrc, const float orientation);
STGLIB_API_CALL int			STDrawGlyph(STHDC hDC, int Sx, int Sy, STHBITMAP pSrc, float orientation, int Cx, int Cy, sga_uint32 color);

STGLIB_API_CALL int			STTextOut(STHDC hDC, int x0, int y0, char *text, STHFONT font);
STGLIB_API_CALL int			STTextOutStretch(STHDC hDC, int x0, int y0, char *text, float hStretch, float vStretch, STHFONT font);

STGLIB_API_CALL int			STDrawLine(STHDC hDC, int x0, int y0, int x1, int y1);

STGLIB_API_CALL int			STDrawPolyline(STHDC hDC, t_polygonList pList);
STGLIB_API_CALL int			STDrawPolylineAA(STHDC hDC, t_polygonList pList);

STGLIB_API_CALL STHBITMAP	STSetPolygonPattern(STHDC hDC, STHBITMAP pSrc);
STGLIB_API_CALL int			STDrawPolygon(STHDC hDC, t_polygonList pList, STPOLYTYPE type);

STGLIB_API_CALL sga_uint32	STSetColor(STHDC hDC, sga_uint32 color);
STGLIB_API_CALL sga_uint32	STGetColor(STHDC hDC);
STGLIB_API_CALL int			STSetLineThickness(STHDC hDC, int thickness);
STGLIB_API_CALL sga_uint32	STSetLineMode(STHDC hDC, sga_uint32 mode);
STGLIB_API_CALL sga_uint32	STGetLineMode(STHDC hDC);

STGLIB_API_CALL int			STGetScreenOffset(STHDC hDC, int *x, int *y);
STGLIB_API_CALL int			STSetScreenOffset(STHDC hDC, int x, int y);

STGLIB_API_CALL int			STCreateVideoBuffers(STHDC hDC, int width, int height, t_sgaColorFormat format, int *num_buffer, STHBITMAP* buffers);
STGLIB_API_CALL int			STDestroyVideoBuffers(STHDC hDC);
STGLIB_API_CALL int			STSetVideoBufferWindow(STHDC hDC, STHBITMAP buffer, int x, int y, int w, int h);
STGLIB_API_CALL int			STRenderVideoBuffer(STHDC hDC, STHBITMAP buffer, unsigned long physAddr, int blockW, int blockH, bool waitVSync, int u_offset, int v_offset, int yStride);
STGLIB_API_CALL int			STGetVideoBufferAddress(STHDC hDC, STHBITMAP buffer, PVOID* address);
STGLIB_API_CALL int			STGetVideoBufferSize(STHDC hDC, STHBITMAP buffer, int *byteSize);

STGLIB_API_CALL HANDLE		STGetVideoSramBuffer(STHDC hDC,	int dwSize);

STGLIB_API_CALL int			STDumpDisplay(STHDC hDC, char *fileName);
STGLIB_API_CALL int			STGetSGAStatistics(STHDC hDC, t_sgaStatistics *stats);
STGLIB_API_CALL int     STGetDMABufferAddress(STHDC hDC, STHBITMAP bitmap, PVOID* address, unsigned long *physAddr);
STGLIB_API_CALL STHBITMAP STCreateDMABuffer(STHDC hDC, int size);
STGLIB_API_CALL int STDestroyDMABuffer(STHDC hDC, STHBITMAP hBitmap);

#if __cplusplus
}
#endif

#endif /* __STGLIB_H */
