/*************************************************************************
**
** STGLIB, Accelerated 2D Graphics Library for Cartesio Plus SGA Engine.
**
** Copyright (C) 2009 STMicroelectronics.  All rights reserved.
** 
** This software is the confidential and/or proprietary information of
** STMicroelectronics. You shall not disclose such information and shall
** use it only in accordance with the terms of the license agreement you
** entered into with STMicroelectronics.
** 
*************************************************************************/

#ifndef __STGLIB_H
#define __STGLIB_H

#include "stglib_platform.h"
//#include "vip_params.h"

typedef HANDLE			STHDC;
typedef HANDLE			STHBITMAP;
typedef HANDLE			STHFONT;
typedef HANDLE			STHVIDEOMAP;

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
	sga_uint32	 		charHeight;			// The Height of one character
	sga_uint32			charWidth;			// The Width of each character
	sga_bool 			fontTransparency;	// Boolean
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
	STSRCCOPYTRANSP,
} STROP;

typedef enum {
	STPOLYFAN,
	STPOLYLIST,
	STPOLYSTRIP,
} STPOLYTYPE;

typedef enum {
	STNOINTERLACING,
	STDEINTERLACINGBOB,
	STDEINTERLACINGWEAVE,
} STVIDEOINDEINTERLACING;

typedef enum
{
    CLCD_BPP1                           = 0,
    CLCD_BPP2                           = 1,
    CLCD_BPP4                           = 2,
    CLCD_BPP8                           = 3,
    CLCD_BPP12                          = 4,
    CLCD_BPP15                          = 12,
    CLCD_BPP16                          = 20,

    /* 24BPP reserved to TFT panel ONLY */
    CLCD_BPP_24_NONPACKED               = 5,            /* 32 bit data word contains one pixel plus one unused byte */
    CLCD_BPP_24_PACKED                  = 6             /* all Four bytes of data word contains usefull data */
} t_clcd_bits_per_pixel;

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
static const t_sgaColorFormat SGA_DIS_FMT_RGB565    	= { SGA_PIX_FORMAT_RGB16, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGBA15   		= { SGA_PIX_FORMAT_RGBA15, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_ARGB15    	= { SGA_PIX_FORMAT_ARGB15, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGB888		= { SGA_PIX_FORMAT_ARGB24 | 0x4000, 3 }; //BpP
static const t_sgaColorFormat SGA_DIS_FMT_ARGB8888		= { SGA_PIX_FORMAT_ARGB24, 4 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGBA8888    	= { SGA_PIX_FORMAT_RGBA24, 4 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_YUV422		= { SGA_PIX_FORMAT_YUV422, 2 };		//BpP 
static const t_sgaColorFormat SGA_DIS_FMT_YUV420		= { SGA_PIX_FORMAT_YUV420, 2 };		//BpP 

//#if __cplusplus
//extern "C" {
//#endif

STGLIB_API_CALL STHDC		STCreateDC(HWND hWnd, const int nBackBuffers);
STGLIB_API_CALL STHDC		STCreateCompatibleDC(int m_surfCX, int m_surfCY);
STGLIB_API_CALL STHDC		STCreateBufferDC(int dwWidth, int dwHeight, t_sgaColorFormat colorFormat, int swapRB, STHBITMAP* surfHandle);

STGLIB_API_CALL STHDC		STGetDC(HWND hWnd);
STGLIB_API_CALL int			STReleaseDC(STHDC hDC);

STGLIB_API_CALL int			STFlush(STHDC hDC);
STGLIB_API_CALL int			STFinish(STHDC hDC);

STGLIB_API_CALL int			STCreateFlipBuffers(STHDC hDC, int nBackBuffers);
STGLIB_API_CALL int			STUpdateOverlay(STHDC hDC, DWORD dwUpdateFlags);
STGLIB_API_CALL int			STDestroyFlipBuffers(STHDC hDC);
STGLIB_API_CALL int			STLock(STHDC hDC);
STGLIB_API_CALL int			STUnlock(STHDC hDC);

STGLIB_API_CALL int			STFlip(STHDC hDC);
STGLIB_API_CALL int			STFlip(STHDC hDC, STVIDEOINDEINTERLACING dwMethod, unsigned int *pFIndex);
STGLIB_API_CALL int			STIsBltDone(STHDC hDC);

STGLIB_API_CALL STHBITMAP	STCreateBitmap(STHDC hDC, int nWidth, int nHeight, t_sgaColorFormat format, const void *lpvBits);
STGLIB_API_CALL STHBITMAP	STCreateBitmapFromClientBuffer(int nWidth, int nHeight, t_sgaColorFormat format, void *virtualAddress);
STGLIB_API_CALL STHBITMAP	STLoadBitmap(STHDC hDC, int nWidth, int nHeight, t_sgaColorFormat format, const char *fname);
STGLIB_API_CALL int			STDestroyBitmap(STHDC hDC, STHBITMAP hBitmap);
STGLIB_API_CALL int			STGetSurfaceBufferAddress(STHDC hDC, void** address);

STGLIB_API_CALL STHVIDEOMAP STCreateVideoMap(STHDC hDC, int video_buf_num, int nWidth, int nHeight, t_sgaColorFormat format, const void *lpvBits);
STGLIB_API_CALL int			STDestroyVideoMap(STHDC hDC, STHVIDEOMAP hVideoMap);
STGLIB_API_CALL int			STBltVideoMapIndexed(STHDC hDC, int x0, int y0, int x1, int y1, STHVIDEOMAP pSrc, int nXSrc, int nYSrc, STVIDEOINDEINTERLACING dwMethod, unsigned int *pFIndex);

STGLIB_API_CALL int			STBindVideoMapToVideoIn(STHVIDEOMAP hVideoMap, sga_uint32 VipControlReg);
STGLIB_API_CALL int			STUnbindVideoMapToVideoIn(STHVIDEOMAP hVideoMap);

STGLIB_API_CALL int			STSetCropVideoMap(STHVIDEOMAP pSrc, const int x, const int y, const int w, const int h);

STGLIB_API_CALL int			STBindBitmapToVideoIn(STHBITMAP hBitmap, sga_uint32 VipControlReg);
STGLIB_API_CALL int			STUnbindBitmapToVideoIn(STHBITMAP hBitmap);

STGLIB_API_CALL int			STSetCropWindow(STHBITMAP pSrc, const int x, const int y, const int w, const int h);

STGLIB_API_CALL STHFONT		STCreateFont(STHDC hDC, t_sgaFont *font, STHBITMAP fontBitmap);
STGLIB_API_CALL int			STModifyFont(STHDC hDC, STHFONT hFont, t_sgaFont font);
STGLIB_API_CALL int			STDestroyFont(STHDC hDC, STHFONT hFont);

STGLIB_API_CALL int			STBitBlt(STHDC hDC, int x0, int y0, int x1, int y1, STHBITMAP pSrc, int nXSrc, int nYSrc, STROP dwRop);
STGLIB_API_CALL int			STFillRect(STHDC hDC, int x0, int y0, int x1, int y1, sga_uint32 color);
STGLIB_API_CALL int			STDrawTextMap(STHDC hDC, int x0, int y0, STHBITMAP pSrc, const float orientation);
STGLIB_API_CALL int			STDrawGlyph(STHDC hDC, int Sx, int Sy, STHBITMAP pSrc, const float orientation, const int Cx, const int Cy, const int color);

STGLIB_API_CALL int			STTextOut(STHDC hDC, int x0, int y0, char *text, STHFONT font);
STGLIB_API_CALL int			STTextOutStretch(STHDC hDC, int x0, int y0, char *text, float hStretch, float vStretch, STHFONT font);

STGLIB_API_CALL int			STDrawLine(STHDC hDC,int x0, int y0, int x1, int y1);

STGLIB_API_CALL int			STDrawPolyline(STHDC hDC, t_polygonList pList);
STGLIB_API_CALL int			STDrawPolylineAA(STHDC hDC, t_polygonList pList);

STGLIB_API_CALL STHBITMAP	STSetPolygonPattern(STHDC hDC, STHBITMAP pSrc);
STGLIB_API_CALL int			STDrawPolygon(STHDC hDC, t_polygonList pList, STPOLYTYPE type);

STGLIB_API_CALL sga_uint32	STSetColor(STHDC hDC, sga_uint32 color);
STGLIB_API_CALL sga_uint32	STSetTextColor(STHDC hDC, sga_uint32 color);
STGLIB_API_CALL sga_uint32	STGetColor(STHDC hDC);
STGLIB_API_CALL sga_uint32	STSetBkColor(STHDC hDC, sga_uint32 color);
STGLIB_API_CALL int			STSetLineThickness(STHDC hDC, int thickness);
STGLIB_API_CALL sga_uint32	STSetLineMode(STHDC hDC, sga_uint32 mode);
STGLIB_API_CALL sga_uint32	STGetLineMode(STHDC hDC);

STGLIB_API_CALL int			STGetScreenOffset(STHDC hDC, int *x, int *y);
STGLIB_API_CALL int			STSetScreenOffset(STHDC hDC, const int x, const int y);

STGLIB_API_CALL int			STCreateVideoBuffers(STHDC hDC, int width, int height, t_sgaColorFormat format, int *num_buffer, STHBITMAP* buffers);
STGLIB_API_CALL int			STDestroyVideoBuffers(STHDC hDC);
STGLIB_API_CALL int			STSetVideoBufferWindow(STHDC hDC, STHBITMAP buffer, const int x, const int y, const int w, const int h);
STGLIB_API_CALL int			STRenderVideoBuffer(STHDC hDC, STHBITMAP buffer, int blockW, int blockH, bool waitVSync);
STGLIB_API_CALL int			STGetVideoBufferAddress(STHDC hDC, STHBITMAP buffer, void** address);
STGLIB_API_CALL int			STGetVideoBufferSize(STHDC hDC, STHBITMAP buffer, int *byteSize);

STGLIB_API_CALL sga_uint32	STGetVideoSramBuffer(STHDC hDC,	sga_uint32 dwSize);
STGLIB_API_CALL sga_uint32	STGetVideoTCMBuffer(STHDC hDC,	sga_uint32 dwSize);

STGLIB_API_CALL int			STDumpDisplay(STHDC hDC, char *fileName);
STGLIB_API_CALL int			STDumpBitmap(STHBITMAP hBMP, char *fileName);
STGLIB_API_CALL int			STGetSGAStatistics(STHDC hDC, t_sgaStatistics *stats);

STGLIB_API_CALL int			STDumpBitmapFromVideoIn(STHVIDEOMAP hVideoMap, char *fileName);

STGLIB_API_CALL int			STGetBufferAddress(STHDC hDC, STHBITMAP buffer, PVOID* virtualAddr, PVOID* phyAddr);
STGLIB_API_CALL STHBITMAP	STCreateDMABitmap(STHDC hDC, int nWidth, int nHeight, t_sgaColorFormat format, int size);
STGLIB_API_CALL int			STDestroyDMABitmap(STHDC hDC, STHBITMAP hBitmap);
STGLIB_API_CALL int			STSetFlipAddress(STHDC hDC, DWORD yBuffer, DWORD uOffset, DWORD vOffset, DWORD yStride, DWORD uStride, DWORD vStride);

//#if __cplusplus
//}
//#endif

#endif /* __STGLIB_H */
