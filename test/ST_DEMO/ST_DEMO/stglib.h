#ifndef __STGLIB_H
#define __STGLIB_H

#ifdef _WIN32_WCE
#include <windows.h>
#endif	

#include "sga_types.h"

typedef struct s_vtx {
	sga_coordinate x;
	sga_coordinate y;
} t_vtx;

typedef struct s_width {
	sga_coordinate xW;
	sga_coordinate yW;
} t_width;

typedef struct s_polygonList{
	sga_int32	length;
	t_width		*width;
	t_vtx		*vtx;
} t_polygonList;

typedef struct sgaFont {
	sga_uint32			m_handleID;
	sga_uint32			charNum;				// The number of chars in the font
	sga_uint32			charStep;				// The distance in pixels between two characters (X axis)
	sga_uint32	 		charHeight;				// The Height of one character
	sga_uint32			charWidth;				// The Width of each character
	sga_bool 			fontTransparency;		// Boolean
	sga_uint8			alphaMax;
	sga_uint8			alphaMin;
	sga_uint32			color;					// font color
	sga_uint32			bkGround;
	t_sgaSurface		fontBitmap;				// The Bitmap containing the font pixels
} t_sgaFont;

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


typedef HANDLE								STHDC;
typedef HANDLE								STHBITMAP;
typedef HANDLE								STHFONT;
typedef HANDLE								STHGDIOBJ;

// Sga pixels types
#define SGA_PIX_FORMAT_MONO_1BPP			0x00
#define SGA_PIX_FORMAT_MONO_2BPP			0x01
#define SGA_PIX_FORMAT_MONO_4BPP			0x02
#define SGA_PIX_FORMAT_MONO_8BPP			0x03
#define SGA_PIX_FORMAT_RGB8					0x04
#define SGA_PIX_FORMAT_RGBA15				0x07   // 5-5-5-1
#define SGA_PIX_FORMAT_RGB16				0x08   // 5-6-5
#define SGA_PIX_FORMAT_ARGB15				0x09   // 1-5-5-5
#define SGA_PIX_FORMAT_ARGB24				0x0A   // A888, Can be packed as 888, see below
#define SGA_PIX_FORMAT_RGBA24				0x0B   // 888A, actually rgb32
#define SGA_PIX_FORMAT_YUV422				0x0C
#define SGA_PIX_FORMAT_YUV420				0x40

#define SGA_PIX_FORMAT_LITTLE_ENDIAN		0x0010 // BIG_E is 0
#define SGA_PIX_FORMAT_BGR					0x0020
#define SGA_PIX_FORMAT_STENCIL_MODE_OFF		0x0000
#define SGA_PIX_FORMAT_STENCIL_MODE_1BPP	0x0080
#define SGA_PIX_FORMAT_PACK_RGB24			0x4000 

// Sga display types 
static const t_sgaColorFormat SGA_DIS_FMT_MONO_1BPP 	= { SGA_PIX_FORMAT_MONO_1BPP, 32 }; //PpW
static const t_sgaColorFormat SGA_DIS_FMT_MONO_2BPP 	= { SGA_PIX_FORMAT_MONO_2BPP, 16 }; //PpW
static const t_sgaColorFormat SGA_DIS_FMT_MONO_4BPP 	= { SGA_PIX_FORMAT_MONO_4BPP, 8 };  //PpW
static const t_sgaColorFormat SGA_DIS_FMT_MONO_8BPP 	= { SGA_PIX_FORMAT_MONO_8BPP, 1 };  //BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGB8   		= { SGA_PIX_FORMAT_RGB8, 1};		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGBA15   		= { SGA_PIX_FORMAT_RGBA15, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGB16     	= { SGA_PIX_FORMAT_RGB16, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_ARGB15    	= { SGA_PIX_FORMAT_ARGB15, 2 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_ARGB24_PKT    = { SGA_PIX_FORMAT_ARGB24 | SGA_PIX_FORMAT_PACK_RGB24, 3 }; //BpP
static const t_sgaColorFormat SGA_DIS_FMT_ARGB24_UNPKT  = { SGA_PIX_FORMAT_ARGB24, 4 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_RGBA24    	= { SGA_PIX_FORMAT_RGBA24, 4 };		//BpP
static const t_sgaColorFormat SGA_DIS_FMT_YUV422		= { SGA_PIX_FORMAT_YUV422, 2 };		//BpP 
static const t_sgaColorFormat SGA_DIS_FMT_YUV420		= { SGA_PIX_FORMAT_YUV420, 2 };		//BpP 

#ifndef STGLIB_API_CALL
#define STGLIB_API_CALL			__declspec(dllexport)
#endif

#if __cplusplus
extern "C" {
#endif

STGLIB_API_CALL STHBITMAP		STSetPolygonPattern(STHDC hdc, STHBITMAP pSrc);

STGLIB_API_CALL STHDC			STCreateDC(HWND hWnd, const int bb);
STGLIB_API_CALL STHDC			STCreateCompatibleDC( DWORD m_surfCX, DWORD m_surfCY);
STGLIB_API_CALL STHDC			STGetDC(HWND hWnd);
STGLIB_API_CALL int				STReleaseDC(STHDC hDC);

STGLIB_API_CALL int				STFlush(STHDC hDC);
STGLIB_API_CALL int				STFinish(STHDC hDC);
STGLIB_API_CALL int				STFlip(STHDC hDC);
STGLIB_API_CALL int				STIsBltDone(STHDC hdc);

STGLIB_API_CALL STHBITMAP		STCreateBitmap(int nWidth, int nHeight, \
											   t_sgaColorFormat format, CONST VOID* lpvBits);

STGLIB_API_CALL STHBITMAP		STCreateBitmapFromClientBuffer(	int nWidth, int nHeight, \
																t_sgaColorFormat format, \
																void* virtualAddress);

STGLIB_API_CALL STHBITMAP		STLoadBitmap(int nWidth, int nHeight, t_sgaColorFormat format, LPCTSTR fname);

STGLIB_API_CALL STHFONT			STCreateFont(t_sgaFont *font);
STGLIB_API_CALL int				STModifyFont(STHFONT hFont, t_sgaFont font);
STGLIB_API_CALL int				STDeleteObject(STHGDIOBJ hObject);

STGLIB_API_CALL int				STBitBlt(STHDC hdc,int x0, int y0, int x1, int y1, STHBITMAP pSrc,int nXSrc, int nYSrc, STROP dwRop);
STGLIB_API_CALL int				STFillRect(STHDC hdc, int x0, int y0, int x1, int y1, uint color);
STGLIB_API_CALL int				STDrawTextMap(STHDC hdc, int x0, int y0, STHBITMAP pSrc, const float orientation);
STGLIB_API_CALL int				STDrawGlyph(STHDC hdc, int Sx, int Sy, STHBITMAP pSrc, const float orientation, \
											const int Cx, const int Cy,   const int color );

STGLIB_API_CALL int				STTextOut(STHDC hdc, int x0, int y0, char * text, STHFONT font);
STGLIB_API_CALL int				STTextOutStretch(STHDC hdc, int x0, int y0, char * text, float hStretch, float vStretch, STHFONT font);

STGLIB_API_CALL int				STLineTo(  STHDC hdc,int X, int Y );

STGLIB_API_CALL int				STDrawPolyline(STHDC hdc, t_polygonList pList);
STGLIB_API_CALL int				STDrawPolylineAA(STHDC hdc, t_polygonList pList);

STGLIB_API_CALL int				STDrawPolygon(STHDC hdc, t_polygonList pList, STPOLYTYPE type);

STGLIB_API_CALL uint			STSetColor(STHDC hdc, uint color);
STGLIB_API_CALL uint			STSetTextColor(STHDC hdc, uint color);
STGLIB_API_CALL uint			STGetColor(STHDC hdc);
STGLIB_API_CALL uint			STSetBkColor(STHDC hdc, uint color);
STGLIB_API_CALL int				STSetBkMode(STHDC hdc, int mode);
STGLIB_API_CALL int				STSetLineThickness(STHDC hdc, int thickness);
STGLIB_API_CALL uint			STSetLineMode(STHDC hdc, uint mode);
STGLIB_API_CALL uint			STGetLineMode(STHDC hdc);
STGLIB_API_CALL int				STSetClipRgn(  STHDC hdc,int nLeftRect,int nTopRect,int nRightRect,int nBottomRect );
STGLIB_API_CALL int				STSetPosition(  STHDC hdc,int X, int Y );
STGLIB_API_CALL HGDIOBJ			STSetPenReference(STHDC hdc, HGDIOBJ hgdiobj);
STGLIB_API_CALL HGDIOBJ			STSetBrushReference(STHDC hdc, HGDIOBJ hgdiobj);
//STGLIB_API_CALL sgaFont*		STSetFont(STHDC hdc, sgaFont *font);
//STGLIB_API_CALL sgaFont*		STGetFont(STHDC hdc);

STGLIB_API_CALL int				STGetScreenOffset(STHDC hdc, int* x, int* y);
STGLIB_API_CALL int				STSetScreenOffset(STHDC hdc, const int x, const int y);


STGLIB_API_CALL int				STCreateVideoBuffers(STHDC hdc, \
													 int width, int height, \
													 t_sgaColorFormat format, \
													 int* num_buffer, \
													 STHBITMAP* buffers);
STGLIB_API_CALL int				STDestroyVideoBuffers(STHDC hdc);
STGLIB_API_CALL int				STSetVideoBufferWindow(STHDC hdc, \
													   STHBITMAP buffer, \
													   const int x, const int y, \
													   const int w, const int h);
STGLIB_API_CALL int				STRenderVideoBuffer(STHDC hdc, \
													STHBITMAP buffer, \
													int blockW, \
													int blockH, \
													bool waitVSync);
STGLIB_API_CALL int				STGetVideoBufferAddress(STHDC hdc, \
														STHBITMAP buffer, \
														PVOID* address);
STGLIB_API_CALL int				STGetVideoBufferSize(STHDC hdc, \
													 STHBITMAP buffer, \
													 int* byteSize);

STGLIB_API_CALL int				STDumpDisplay(STHDC hdc, char* fileName);
STGLIB_API_CALL t_sgaStatistics* STGetSGAStatistics(STHDC hdc);
	

#if __cplusplus
}
#endif

#endif // __STGLIB_H__


