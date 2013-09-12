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

#ifndef __GDI_HPP_INCLUDED__
#define __GDI_HPP_INCLUDED__

class Gdi
{

public:

	static
	BOOL
	Initialize(
		HINSTANCE	hinst
		);

	static
	void
	ProcessCleanup(
		HPROCESS	hprc
		);


	static
	WINGDIAPI int WINAPI
	AddFontResourceW_I(
		const WCHAR* lpszFilename
		);

	static
	WINGDIAPI BOOL WINAPI
	BitBlt_I(HDC, int, int, int, int, HDC, int, int, DWORD);

	static
	WINGDIAPI int WINAPI
	CombineRgn_I(HRGN, HRGN, HRGN, int);

	static
	WINGDIAPI HDC WINAPI
	CreateCompatibleDC_I(
		HDC hdcIn
		);

	static
	WINGDIAPI HBRUSH WINAPI
	CreateDIBPatternBrushPt_I(const void *, unsigned int);

	static
	unsigned int
	DibByteWidth(
		unsigned int	N,
		unsigned int	W
		);

	static
	unsigned int
	DibSize(
		unsigned int	N,
		unsigned int	W,
		int				H
		);


	static
	WINGDIAPI HBITMAP WINAPI
	CreateDIBSectionStub(
		HDC   hdc,
		BOOL  fAllowNullDC,
		const BITMAPINFO *pbmi,
		void* pvBits,
		BOOL  fFreeBits,
		unsigned int  iUsage);

	static
	HBITMAP WINAPI
	CreatePermanentDIBSection(
		HDC              hdc,
		CONST BITMAPINFO *pbmi,
		UINT             iUsage,
		void             **ppvBits,
		HANDLE           hSection,
		DWORD            dwOffset);

    static 
    HBITMAP 
    CreateMonoBitmapFromDIB(
        const BITMAPINFO *pbmi,
        void  *pvBits);
        
	static 
	WINGDIAPI UINT WINAPI 
	GetDIBColorTable_I(
		HDC				hdc, 
		UINT			uStartIndex, 
		UINT			cEntries, 
		RGBQUAD *		pColors);

	static
	WINGDIAPI UINT WINAPI
	SetDIBColorTable_I(
		HDC 			hdc,
		UINT 			uStartIndex,
		UINT 			cEntries,
	    CONST RGBQUAD *		pColors);

	static
	WINGDIAPI int WINAPI	
	StretchDIBits_I(
		HDC				hdc,
		int 			XDest,
		int				YDest,
		int 			nDestWidth,
		int 			nDestHeight,
		int				XSrc,
		int				YSrc,
		int				nSrcWidth,
		int				nSrcHeight,
		CONST VOID*	 	lpBits,
		CONST BITMAPINFO *lpBitsInfo,
		UINT            iUsage,
		DWORD 			dwRop);
        
	static
	WINGDIAPI int WINAPI
	SetDIBitsToDevice_I(
        HDC             hdc,                 
        int             XDest,              
        int             YDest,           
        DWORD           dwWidth,          
        DWORD           dwHeight,          
        int             XSrc,               
        int             YSrc, 
        UINT            uStartScan,
        UINT            cScanLines,
        CONST VOID *lpvBits,
        CONST BITMAPINFO *lpbmi, 
        UINT fuColorUse);

	
	static
	WINGDIAPI
	HFONT
	WINAPI
	CreateFontIndirectW_I(
		const	LOGFONTW*
		);

	static
	WINGDIAPI HRGN WINAPI
	CreateRectRgnIndirect_I(
		const	RECT*
		);

	static
	WINGDIAPI HPEN WINAPI
	CreatePenIndirect_I(
		const LOGPEN *lplgpn
		);

	static
	WINGDIAPI HBRUSH WINAPI
	CreateSolidBrush_I(
		COLORREF crColor
		);

	static
	WINGDIAPI BOOL WINAPI
	DeleteDC_I(
		HDC hdc
		);

	static
	WINGDIAPI
	BOOL
	WINAPI
	DeleteObject_I(
		HGDIOBJ hgdiobj
		);

	static
	WINUSERAPI BOOL WINAPI
	DrawEdge_I(HDC, RECT*, unsigned int, unsigned int);

	static
	WINUSERAPI BOOL WINAPI
	DrawFocusRect_I(HDC, const RECT *);

	static
	WINUSERAPI int WINAPI
	DrawTextW_I(HDC, const WCHAR*, int, RECT *, unsigned int);


	static
	WINGDIAPI BOOL WINAPI
	Ellipse_I(HDC, int, int, int, int);

	static
	WINGDIAPI int WINAPI
	EnumFontFamiliesW_I(HDC, const WCHAR*, FONTENUMPROC, LPARAM);

	static
	WINGDIAPI int WINAPI
	EnumFontsW_I(
		HDC hdc,
		const WCHAR* lpFaceName,
		FONTENUMPROC lpFontFunc,
		LPARAM lParam
		);

	static
	WINGDIAPI int WINAPI
	ExcludeClipRect_I(HDC, int, int, int, int);


	static
	WINGDIAPI BOOL WINAPI
	ExtTextOutW_I(
		HDC         hdc,
		int         X,
		int         Y,
		unsigned int        fuOptions,
		const RECT *lprc,
		const WCHAR*     lpszString,
		unsigned int        cbCount,
		const int   *lpDx);

	static 
	WINGDIAPI BOOL WINAPI
	GetCharWidth32_I(
		HDC       	hdc,
		UINT 		iFirstChar,
		UINT 		iLastChar,
		LPINT		lpBuffer);

	static
	WINGDIAPI BOOL WINAPI
	GetCharABCWidths_I(
		HDC         hdc,
		UINT        uFirstChar,
        UINT        uLastChar,  
  		LPABC       lpabc
	);

	static
	WINGDIAPI BOOL WINAPI
	GetCharABCWidthsI_I(
		HDC hdc,         
		UINT giFirst,    
		UINT cgi,        
		LPWORD pgi,      
		LPABC lpabc      
		);

	static 
	WINGDIAPI UINT WINAPI	
	SetTextAlign_I(
		HDC         hdc,
		UINT        fMode
		);

	static 
	WINGDIAPI UINT WINAPI	
	GetTextAlign_I(
		HDC   	    hdc
		);

	static
	WINGDIAPI int WINAPI
	SetTextCharacterExtra_I(
		HDC hdc,
		int nCharExtra
		);

	static
	WINGDIAPI int WINAPI
	GetTextCharacterExtra_I(
		HDC hdc
		);
    
	static
	WINUSERAPI int WINAPI
	FillRect_I(
		HDC,
		const RECT *,
		HBRUSH
		);

	static
	WINGDIAPI COLORREF WINAPI
	GetBkColor_I(
		HDC hdc
		);

	static
	WINGDIAPI int WINAPI
	GetBkMode_I(
		HDC hdc
		);

	static
	WINGDIAPI int WINAPI
	GetClipRgn_I(HDC, HRGN);

	static
	WINGDIAPI HGDIOBJ WINAPI
	GetCurrentObject_I(
		HDC hdc,
		unsigned int uObjectType
		);

	static
	WINGDIAPI int WINAPI
	GetDeviceCaps_I(
		HDC hdc,
		int nIndex
		);

	static
	WINGDIAPI COLORREF WINAPI
	GetNearestColor_I(HDC, COLORREF);

	static
	WINGDIAPI int WINAPI
	GetObjectW_I(
		HGDIOBJ hgdiobj,
		int cbBuffer,
		void* lpvObject
		);

	static
	WINGDIAPI DWORD WINAPI
	GetObjectType_I(
		HGDIOBJ hgdiobj
		);

	static
	WINGDIAPI COLORREF WINAPI
	GetPixel_I(HDC, int, int nYPos);

	static
	WINGDIAPI DWORD WINAPI
	GetRegionData_I(HRGN, DWORD, RGNDATA*);

	static
	WINGDIAPI int WINAPI
	GetRgnBox_I(HRGN, RECT *);

	static
	BOOL WINAPI
	PatBlt_I(HDC, int, int, int, int, DWORD);

	static
	WINGDIAPI COLORREF WINAPI
	GetTextColor_I(
		HDC hdc
		);

	static
	WINGDIAPI BOOL WINAPI
	GetTextExtentExPointW_I(
		HDC     hdc,
		const WCHAR* lpszStr,
		int     cchString,
		int     nMaxExtent,
		int*   lpnFit,
		int*   alpDx,
		SIZE*  lpSize);

	static
	WINGDIAPI int WINAPI
	GetTextFaceW_I(HDC, int, WCHAR*);


	static
	WINGDIAPI
	BOOL
	WINAPI
	GetTextMetricsW_I(
		HDC hdc,
		TEXTMETRIC* lptm);

    static
    WINGDIAPI
    UINT
    WINAPI
    GetOutlineTextMetricsW_I(
        HDC hdc,
        UINT cbData,
        LPOUTLINETEXTMETRICW lpOTM
        );
    
	static
	WINGDIAPI BOOL WINAPI
	MaskBlt_I(HDC, int, int, int, int, HDC, int, int, HBITMAP, int, int, DWORD);

	static
	WINGDIAPI int WINAPI
	OffsetRgn_I(HRGN, int, int);

	static 
	WINGDIAPI BOOL WINAPI
	GetCurrentPositionEx_I(HDC,POINT *);
	
	static 
	WINGDIAPI BOOL WINAPI 
	MoveToEx_I(HDC,int,int,POINT *);

	static
	WINGDIAPI BOOL WINAPI
	LineTo_I(HDC,int,int);
	
	static
	WINGDIAPI BOOL WINAPI
	Polygon_I(HDC, const POINT *, int);

	static
	WINGDIAPI BOOL WINAPI
	Polyline_I(HDC, const POINT *, int);

	static
	WINGDIAPI BOOL WINAPI
	PtInRegion_I(HRGN, int, int);

	static
	WINGDIAPI BOOL WINAPI
	Rectangle_I(HDC, int, int, int, int);

	static
	WINGDIAPI BOOL WINAPI
	RectInRegion_I(HRGN, const RECT *);

	static
	WINGDIAPI BOOL WINAPI
	RemoveFontResourceW_I(
		const WCHAR* lpFileName
		);

	static
	WINGDIAPI BOOL WINAPI
	RestoreDC_I(HDC, int);

	static
	WINGDIAPI BOOL WINAPI
	RoundRect_I(HDC, int, int, int, int, int, int);

	static
	WINGDIAPI int WINAPI
	SaveDC_I(HDC);

	static
	WINGDIAPI int WINAPI
	SelectClipRgn_I(
		HDC hdc,
		HRGN hrgn
		);

	static
	WINGDIAPI HGDIOBJ WINAPI
	SelectObject_I(
		HDC hdc,
		HGDIOBJ hgdiobj
		);

	static
	WINGDIAPI COLORREF WINAPI
	SetBkColor_I(
		HDC hdc,
		COLORREF crColor
		);

	static
	WINGDIAPI int WINAPI
	SetBkMode_I(
		HDC hdc,
		int iBkMode
		);

	static
	WINGDIAPI BOOL WINAPI
	SetBrushOrgEx_I(HDC, int, int, POINT*);

	static
	WINGDIAPI COLORREF WINAPI
	SetPixel_I(HDC, int, int, COLORREF);


	static
	WINGDIAPI COLORREF WINAPI
	SetTextColor_I(
		HDC hdc,
		COLORREF crColor
		);

	static
	WINGDIAPI BOOL WINAPI
	StretchBlt_I(HDC, int, int, int, int, HDC, int, int, int, int, DWORD);

	static
	WINGDIAPI HBITMAP WINAPI
	CreateBitmap_I(int, int, unsigned int, unsigned int, const void *);

	static
	WINGDIAPI LONG WINAPI
	SetBitmapBits_I(HBITMAP, DWORD, CONST VOID *);


	static
	WINGDIAPI HBITMAP WINAPI
	CreateCompatibleBitmap_I(HDC, int, int);

	static
	WINGDIAPI HBRUSH WINAPI
	GetSysColorBrush_I(int);

	static
	WINGDIAPI int WINAPI
	IntersectClipRect_I(HDC, int, int, int, int);

	static
	WINGDIAPI int WINAPI
	GetClipBox_I(HDC, RECT *);

	static
	WINGDIAPI BOOL WINAPI
	CeRemoveFontResource_I(
		CEOID oid
		);

    static
    WINGDIAPI DWORD WINAPI
    GetFontData_I(
        HDC     hdc,
        DWORD   dwTable,
        DWORD   dwOffset,
        LPVOID  lpvBuffer,
        DWORD   cbData
        );

	static
	BOOL
	WINAPI
	EnableEUDC_I(BOOL fEnableEUDC);


	static
	WINGDIAPI HDC   WINAPI
	CreateEnhMetaFileW_I(HDC, const WCHAR*, const RECT *, const WCHAR*);

	static
	WINGDIAPI BOOL  WINAPI
	DeleteEnhMetaFile_I(HENHMETAFILE);

	static
	WINGDIAPI BOOL  WINAPI
	PlayEnhMetaFile_I(HDC, HENHMETAFILE, const RECT *);


	static
	WINGDIAPI HPALETTE WINAPI
	CreatePalette_I(const LOGPALETTE *);


	static
	WINGDIAPI HPALETTE WINAPI
	SelectPalette_I(HDC, HPALETTE, BOOL);

	static
	WINGDIAPI unsigned int WINAPI
	RealizePalette_I(HDC);

	static
	WINGDIAPI unsigned int WINAPI
	GetPaletteEntries_I(HPALETTE, unsigned int, unsigned int, PALETTEENTRY*);

	static
	WINGDIAPI unsigned int WINAPI
	SetPaletteEntries_I(HPALETTE, unsigned int, unsigned int, const PALETTEENTRY *);

	static
	WINGDIAPI unsigned int WINAPI
	GetSystemPaletteEntries_I(HDC, unsigned int, unsigned int, PALETTEENTRY*);


	static
	WINGDIAPI unsigned int WINAPI
	GetNearestPaletteIndex_I(HPALETTE, COLORREF);


	static
	WINGDIAPI HPEN WINAPI
	CreatePen_I(int, int, COLORREF);


	static
	WINGDIAPI int WINAPI
	StartDocW_I(HDC, const DOCINFOW *);

	static
	WINGDIAPI int WINAPI
	EndDoc_I(HDC);

	static
	WINGDIAPI int WINAPI
	StartPage_I(HDC);

	static
	WINGDIAPI int WINAPI
	EndPage_I(HDC);

	static
	WINGDIAPI int WINAPI
	AbortDoc_I(HDC);

	static
	WINGDIAPI int WINAPI
	SetAbortProc_I(HDC, ABORTPROC);

	static
	WINGDIAPI HDC WINAPI
	CreateDCW_I(const WCHAR*, const WCHAR*, const WCHAR*, const DEVMODEW *);

	static
	WINGDIAPI HRGN WINAPI
	CreateRectRgn_I(int, int, int, int);

	static 
        WINGDIAPI HRGN WINAPI
        ExtCreateRegion_I(const XFORM *,DWORD,const RGNDATA *);
	
	static
	WINGDIAPI BOOL WINAPI
	FillRgn_I(HDC, HRGN, HBRUSH);

	static
	WINGDIAPI int WINAPI
	SetROP2_I(HDC, int);

	static
	WINGDIAPI BOOL WINAPI
	SetRectRgn_I(HRGN, int, int, int, int);

	static
	WINGDIAPI BOOL WINAPI
	RectVisible_I(HDC, const RECT *);

	static
	WINGDIAPI HBRUSH WINAPI
	CreatePatternBrush_I(HBITMAP);

	static
	WINGDIAPI HBITMAP WINAPI
	CreateBitmapFromPointer_I(const BITMAPINFO *, int, void*);

	static
	WINGDIAPI BOOL WINAPI
	SetViewportOrgEx_I(
		HDC hdc,
		int nXOrg,
		int nYOrg,
		POINT* lppt
		);

	static
	WINGDIAPI BOOL WINAPI
	TransparentImage_I(HDC, int, int, int, int, HANDLE, int, int, int, int, COLORREF);

	static
	BOOL
	GdiSetObjectOwner_I(HGDIOBJ, HPROCESS);


	static
	WINGDIAPI BOOL WINAPI
	TranslateCharsetInfo_I(
	DWORD *lpSrc,
	CHARSETINFO* lpCs,
	DWORD dwFlags);

	static
	WINGDIAPI int WINAPI
	ExtEscape_I(HDC, int, int, const char*, int, char*);


	static
	WINUSERAPI BOOL WINAPI
	InvertRect_I(HDC, const RECT *);


	static
	int 
	InternalScrollDC(
		HDC		hdc, 
		int		dx, 
		int		dy, 
		const RECT*	prcSrc,				// client coordinates
		const RECT*	prcClip,			// client coordinates
		HRGN	hrgnInvalid,		// screen coordinates
		HRGN	hrgnUpdate,			// output as either
		RECT*	prcUpdate,			// output as client coordinates
		BOOL	fWantClientCoords	// does caller want hrgnUpdate in client coordinates?
		);

	static
	WINGDIAPI BOOL WINAPI
	ScrollDC_I(
		HDC hdc,
		int dx,
		int dy,
		const RECT *lprcScroll,
		const RECT *lprcClip ,
		HRGN hrgnUpdate,
		RECT* lprcUpdate
		);

	static
	BOOL
	IsValidRgn_I(
		HRGN	hrgn
		);

	static
	void
	LogFontFromRegistry(
		LOGFONT*	pLogFont,
		WCHAR*		pRegKey,
		LONG		DefaultWeight
		);



	static
	BOOL
	WINAPI
	EnumDisplayMonitors_I(
		HDC				hdcPaint,
		LPCRECT			lprcPaint,
		MONITORENUMPROC	lpfnEnum,
		LPARAM			lData,
		HPROCESS 		hProcCallingContext
		);
	
	static
	BOOL
	WINAPI
	GetMonitorInfo_I(
		HMONITOR		hMonitor,
		LPMONITORINFO	lpmi
		);
	
	
	
	static
	HMONITOR
	WINAPI
	MonitorFromWindow_I(
		HWND	hwnd,
		DWORD	dwFlag
		);
	
	
	static
	HMONITOR
	WINAPI
	MonitorFromRect_I(
		LPCRECT		lprc,
		DWORD		dwFlag
		);
	
	static
	HMONITOR
	WINAPI
	MonitorFromPoint_I(
		POINT	pt,
		DWORD	dwFlag
		);

	static
	int
	ClearAllGlyphCaches(
		);

	static
	BOOL
	GradientFill_I(
		HDC         hdc,
		PTRIVERTEX  pVertex,
		ULONG       nVertex,
		PVOID       pMesh,
		ULONG       nCount,
		ULONG       ulMode
		);

	static
	BOOL
	EnumDisplaySettings_I(
		LPCTSTR    lpszDeviceName,
		DWORD      iModeNum,
		LPDEVMODEW lpDevMode
		);

	static
	BOOL
	EnumDisplayDevices_I(
		LPCTSTR         lpDevice,
		DWORD           iModeNum,
		PDISPLAY_DEVICE lpDevMode,
		DWORD           dwFlags
		);

	static
	int
	GetStretchBltMode_I(
		HDC	hdc
		);

	static
	int
	SetStretchBltMode_I(
		HDC	hdc,
		int	mode
		);

	static
	BOOL
	AlphaBlend_I(
		HDC hdcDest,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int nHeightDest,
		HDC hdcSrc,
		int nXOriginSrc,
		int nYOriginSrc,
		int nWidthSrc,
		int nHeightSrc,
		BLENDFUNCTION blendFunction
		);

	static
	int
	EnumFontFamiliesExW_I(
		HDC          hdc,
		LPLOGFONTW   pLogFont,
		FONTENUMPROC pfnFontEnumProc,
		LPARAM       lParam,
		DWORD        dwFlags
		);

	static
	DWORD
	SetLayout_I(
		HDC hdc,
		DWORD dwLayout
		);

	static
	DWORD
	GetLayout_I(
		HDC hdc
		);

	static
	WINGDIAPI BOOL WINAPI
	DeleteDCStub_I(
		HDC hdc
		);

	static
	WINGDIAPI
	BOOL
	WINAPI
	DeleteObjectStub_I(
		HGDIOBJ hgdiobj
		);
};

WINGDIAPI HENHMETAFILE WINAPI
CloseEnhMetaFile_I(HDC);


#endif

