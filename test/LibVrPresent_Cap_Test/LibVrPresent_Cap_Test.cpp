// LibVrPresent_Cap_Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LibVrPresent_Cap_Test.h"
#include "ddraw.h"
#include "stdio.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
static DWORD dwMonCount = 0;

#define MONITORS_AVAIL_NUM	9
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HRESULT Probe(HWND hWnd, DWORD dwRowNumber, DWORD LINENUMBER);


#ifdef _WIN32_WCE
#ifdef DIRECTDRAW_VERSION
#define IDirectDraw7 IDirectDraw4
#define IDirectDrawSurface7 IDirectDrawSurface4
#define IID_IDirectDraw7 IID_IDirectDraw4
#define LPDIRECTDRAWSURFACE7 LPDIRECTDRAWSURFACE4
#define DDFLIP_WAITVSYNC DDFLIP_WAIT
#define DDBLT_WAITVSYNC DDBLT_WAIT
#define DDDEVICEIDENTIFIER2 DDDEVICEIDENTIFIER
#else
#define IDirectDraw7 IDirectDraw
#define IDirectDrawSurface7 IDirectDrawSurface
#define IID_IDirectDraw7 IID_IDirectDraw
#define LPDIRECTDRAWSURFACE7 LPDIRECTDRAWSURFACE
#define LPDDSURFACEDESC2 LPDDSURFACEDESC
#define DDSURFACEDESC2 DDSURFACEDESC
#define DDDEVICEIDENTIFIER2 DDDEVICEIDENTIFIER
#endif
#endif

struct GfxMonitorInfo
{
	GUID guid;
	HMONITOR hm;
	MONITORINFO info;
};

DWORD				m_dwNumMonitors = 0;
GfxMonitorInfo		m_aMonitors[MONITORS_AVAIL_NUM];

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LIBVRPRESENT_CAP_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LIBVRPRESENT_CAP_TEST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIBVRPRESENT_CAP_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LIBVRPRESENT_CAP_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int  cxChar, cxCaps, cyChar, cxClient, cyClient, iVscrollPos,iHscrollPos, NUMLINES, CHARNUMINLINE;        
	HDC hdc;
	TEXTMETRIC  tm ;

	switch (message)
	{        
	case WM_CREATE:

		hdc = GetDC (hWnd) ;        
		GetTextMetrics (hdc, &tm) ;        
		cxChar = tm.tmAveCharWidth ;        
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;        
		cyChar = tm.tmHeight + tm.tmExternalLeading ;
		ReleaseDC (hWnd, hdc) ;
		SetScrollRange (hWnd, SB_VERT, 0, NUMLINES - 1, FALSE) ;        
		SetScrollPos   (hWnd, SB_VERT, iVscrollPos, TRUE) ;
		return 0;

	case WM_SIZE:
		cyClient = HIWORD (lParam);
		cxClient = LOWORD (lParam);
		NUMLINES = cyClient / cyChar; 
		CHARNUMINLINE = 2500;//cxClient / cxChar;
		SetScrollRange (hWnd, SB_VERT, 0, NUMLINES - 1, FALSE) ;        
		SetScrollPos   (hWnd, SB_VERT, iVscrollPos, TRUE);
		SetScrollRange (hWnd, SB_HORZ, 0, CHARNUMINLINE, FALSE);
		SetScrollPos   (hWnd, SB_HORZ, iHscrollPos, TRUE);
		return 0;

	case WM_VSCROLL:
		switch (LOWORD (wParam))
		{
		case SB_LINEUP:
			iVscrollPos -= 1 ;
			break;

		case SB_LINEDOWN:
			iVscrollPos += 1 ;
			break;

		case SB_PAGEUP:
			iVscrollPos -= cyClient / cyChar ;
			break;

		case SB_PAGEDOWN:
			iVscrollPos += cyClient / cyChar ;
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			iVscrollPos = HIWORD (wParam) ;
			break;

		default :
			break;
		}
		iVscrollPos = max (0, min (iVscrollPos, NUMLINES - 1)) ;
		if (iVscrollPos != GetScrollPos (hWnd, SB_VERT))
		{
			SetScrollPos (hWnd, SB_VERT, iVscrollPos, TRUE) ;
			InvalidateRect (hWnd, NULL, TRUE) ;
		}
		return 0;


	case WM_HSCROLL:
		switch (LOWORD (wParam))
		{
		case SB_LINELEFT:
			iHscrollPos -= 300 ;
			break;

		case SB_LINERIGHT:
			iHscrollPos += 300 ;
			break;

		case SB_PAGELEFT:
			iHscrollPos -= cxClient;// / cxChar ;
			break;

		case SB_PAGERIGHT:
			iHscrollPos += cyClient;// / cyChar ;
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			iHscrollPos = HIWORD (wParam) ;
			break;

		default :
			break;
		}
		iHscrollPos = max (0, min (iHscrollPos, CHARNUMINLINE - 1)) ;
		if (iHscrollPos != GetScrollPos (hWnd, SB_HORZ))
		{
			SetScrollPos (hWnd, SB_HORZ, iHscrollPos, TRUE) ;
			InvalidateRect (hWnd, NULL, TRUE) ;
		}
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		iVscrollPos = GetScrollPos(hWnd, SB_VERT);
		iHscrollPos = GetScrollPos(hWnd, SB_HORZ);
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		Probe(hWnd, -iHscrollPos, -iVscrollPos);
		return 0;

	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0;
	}

	return DefWindowProc (hWnd, message, wParam, lParam) ;

}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL WINAPI EnumMonitorCallbackEx(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext, HMONITOR hm)
{
	GfxMonitorInfo *pMon;

	if(m_dwNumMonitors >=MONITORS_AVAIL_NUM)
		return FALSE;
	pMon = &m_aMonitors[m_dwNumMonitors++];
	if(lpGUID)
		pMon->guid = *lpGUID;
	else
		ZeroMemory(&pMon->guid,sizeof(pMon->guid));
	pMon->hm = hm;
	ZeroMemory(&pMon->info,sizeof(pMon->info));
	pMon->info.cbSize = sizeof(pMon->info);
	GetMonitorInfo(hm,&pMon->info);
	return (m_dwNumMonitors<MONITORS_AVAIL_NUM);
}


BOOL WINAPI EnumMonitorCallback(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext)
{
	GfxMonitorInfo *pMon;

	pMon = &m_aMonitors[m_dwNumMonitors++];
	pMon->guid = *lpGUID;
	pMon->hm = 0;
	return (m_dwNumMonitors<MONITORS_AVAIL_NUM);
}

STDMETHODIMP EnumerateMonitors()
{
	HRESULT hr;
	HINSTANCE hlib;
	LPDIRECTDRAWENUMERATEEX pDirectDrawEnumerateEx;

	hlib = LoadLibrary(TEXT("ddraw.dll"));
	if(hlib==0)
		return E_FAIL;

	m_dwNumMonitors = 0;
#ifdef _WIN32_WCE
    pDirectDrawEnumerateEx = (LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,L"DirectDrawEnumerateExW");
#else
    pDirectDrawEnumerateEx = (LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,"DirectDrawEnumerateExA");
#endif
    if(pDirectDrawEnumerateEx)
    {
        hr = pDirectDrawEnumerateEx(EnumMonitorCallbackEx, NULL,
                                DDENUM_ATTACHEDSECONDARYDEVICES |
                                DDENUM_DETACHEDSECONDARYDEVICES |
                                DDENUM_NONDISPLAYDEVICES);
    }
    else
		hr = DirectDrawEnumerate(EnumMonitorCallback,	NULL);
	FreeLibrary(hlib);
	return SUCCEEDED(hr) ? S_OK : E_FAIL;
}

#define LINE_GAP_PIXEL	15

HRESULT PrintCapsColorKey(HDC& hdc, DDCAPS* pCaps, DWORD& dwRowNumber, DWORD& dwLineNumber)
{
	TCHAR buffer[256];
	//if(pCaps->dwCKeyCaps&DDCKEYCAPS_BOTHBLT)
	//{
	//	_stprintf_s(buffer, _T("\t \t Supports transparent blitting with for both source and destination surfaces.\n"));
	//	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	//}
	if(pCaps->dwCKeyCaps&DDCKEYCAPS_DESTBLT )
	{
		_stprintf_s(buffer, _T("----DDCKEYCAPS_DESTBLT-- Supports transparent blitting with a color key that identifies the replaceable bits of the destination surface for RGB colors. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCKeyCaps&DDCKEYCAPS_DESTBLTCLRSPACE  )
	{
		_stprintf_s(buffer, _T("----DDCKEYCAPS_DESTBLTCLRSPACE-- Supports transparent blitting with a color space that identifies the replaceable bits of the destination surface for RGB colors. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCKeyCaps&DDCKEYCAPS_DESTBLTCLRSPACEYUV )
	{
		_stprintf_s(buffer, _T("----DDCKEYCAPS_DESTBLTCLRSPACEYUV--Supports transparent blitting with a color space that identifies the replaceable bits of the destination surface for YUV colors."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCKeyCaps&DDCKEYCAPS_SRCBLT )
	{
		_stprintf_s(buffer, _T("----DDCKEYCAPS_SRCBLT-- Supports transparent blitting using the color key for the source with this surface for RGB colors.  "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCKeyCaps&DDCKEYCAPS_SRCBLTCLRSPACE  )
	{
		_stprintf_s(buffer, _T("----DDCKEYCAPS_SRCBLTCLRSPACE-- Supports transparent blitting using a color space for the source with this surface for RGB colors.  "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer,(int) _tcslen(buffer));
	}
	if(pCaps->dwCKeyCaps&DDCKEYCAPS_SRCBLTCLRSPACEYUV  )
	{
		_stprintf_s(buffer, _T("----DDCKEYCAPS_SRCBLTCLRSPACEYUV-- Supports transparent blitting using a color space for the source with this surface for YUV colors"));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}

	return S_OK;
}

HRESULT PrintCapsDwCaps(HDC& hdc, DDCAPS* pCaps, DWORD& dwRowNumber, DWORD& dwLineNumber)
{
	TCHAR buffer[1024];
	_stprintf_s(buffer, _T("-- Driver-specific capabilities1:[0x%x]  "), pCaps->dwCaps );
	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));

	if(pCaps->dwCaps&DDCAPS_3D)
	{
		_stprintf_s(buffer, _T("--DDCAPS_3D-- Indicates that the display hardware has 3-D acceleration. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_ALIGNBOUNDARYSRC  )
	{
		_stprintf_s(buffer, _T("--DDCAPS_ALIGNBOUNDARYSRC-- Indicates that DirectDraw will support only those overlay destination rectangles with the x-axis aligned to the dwAlignBoundaryDest boundaries of the surface. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		_stprintf_s(buffer, _T("------ dwAlignBoundaryDest: %d "), pCaps->dwAlignBoundaryDest);
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		 
	}
	if(pCaps->dwCaps&DDCAPS_ALIGNSIZEDEST )
	{
		_stprintf_s(buffer, _T("--DDCAPS_ALIGNSIZEDEST-- Indicates that DirectDraw will support only those overlay source rectangles with the x-axis aligned to the dwAlignBoundarySrc boundaries of the surface. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		_stprintf_s(buffer, _T("------ dwAlignBoundarySrc : %d "), pCaps->dwAlignBoundarySrc );
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_ALIGNSIZESRC )
	{
		_stprintf_s(buffer, _T("--DDCAPS_ALIGNSIZESRC-- Indicates that DirectDraw will support only those overlay source rectangles whose x-axis sizes, in pixels, are dwAlignSizeSrc multiples. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		_stprintf_s(buffer, _T("------ dwAlignSizeSrc : %d "), pCaps->dwAlignSizeSrc );
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_ALIGNSTRIDE )
	{
		_stprintf_s(buffer, _T("--DDCAPS_ALIGNSTRIDE-- Indicates that DirectDraw will create display memory surfaces that have a stride alignment equal to the dwAlignStrideAlign value. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		_stprintf_s(buffer, _T("------ dwAlignStrideAlign  : %d "), pCaps->dwAlignStrideAlign  );
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_ALPHA )
	{
		_stprintf_s(buffer, _T("--DDCAPS_ALPHA-- Indicates that the display hardware supports alpha-only surfaces. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_BANKSWITCHED)
	{
		_stprintf_s(buffer, _T("--DDCAPS_BANKSWITCHED-- Indicates that the display hardware is bank-switched and is potentially very slow at random access to display memory. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_BLT )
	{
		_stprintf_s(buffer, _T("--DDCAPS_BLT-- Indicates that display hardware is capable of blit operations. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_BLTCOLORFILL )
	{
		_stprintf_s(buffer, _T("--DDCAPS_BLTCOLORFILL-- Indicates that display hardware is capable of color filling with a blitter.  "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_BLTDEPTHFILL )
	{
		_stprintf_s(buffer, _T("--DDCAPS_BLTDEPTHFILL-- DDCAPS_BLTDEPTHFILL : Not supported. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_BLTFOURCC )
	{
		_stprintf_s(buffer, _T("--DDCAPS_BLTFOURCC-- Indicates that display hardware is capable of color-space conversions during blit operations. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_BLTQUEUE )
	{
		_stprintf_s(buffer, _T("--DDCAPS_BLTQUEUE-- Indicates that display hardware is capable of asynchronous blit operations. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_BLTSTRETCH )
	{
		_stprintf_s(buffer, _T("--DDCAPS_BLTSTRETCH-- Indicates that display hardware is capable of stretching during blit operations. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_CANBLTSYSMEM  )
	{
		_stprintf_s(buffer, _T("--DDCAPS_CANBLTSYSMEM--  Indicates that display hardware is capable of blitting to or from system memory.  "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_CANCLIP  )
	{
		_stprintf_s(buffer, _T("--DDCAPS_CANCLIP-- Indicates that display hardware is capable of clipping with blitting. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_CANCLIPSTRETCHED  )
	{
		_stprintf_s(buffer, _T("--DDCAPS_CANCLIPSTRETCHED-- Indicates that display hardware is capable of clipping while stretch-blitting. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_COLORKEY  )
	{
		_stprintf_s(buffer, _T("--DDCAPS_COLORKEY-- Supports some form of color key in either overlay or blit operations. More specific color key capability information can be found in the dwCKeyCaps member. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		PrintCapsColorKey(hdc, pCaps, dwRowNumber, dwLineNumber);
	}
	if(pCaps->dwCaps&DDCAPS_GDI )
	{
		_stprintf_s(buffer, _T("--DDCAPS_GDI-- Indicates that display hardware is shared with GDI"));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_NOHARDWARE   )
	{
		_stprintf_s(buffer, _T("--DDCAPS_NOHARDWARE-- Indicates that there is no hardware support. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_OVERLAY   )
	{
		_stprintf_s(buffer, _T("--DDCAPS_OVERLAY-- Indicates that display the hardware supports overlays. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_OVERLAYCANTCLIP   )
	{
		_stprintf_s(buffer, _T("--DDCAPS_OVERLAYCANTCLIP-- Indicates that display the hardware supports overlays but cannot clip them. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_OVERLAYFOURCC   )
	{
		_stprintf_s(buffer, _T("--DDCAPS_OVERLAYFOURCC-- Indicates that overlay hardware is capable of color-space conversions during overlay operations. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_OVERLAYSTRETCH    )
	{
		_stprintf_s(buffer, _T("--DDCAPS_OVERLAYSTRETCH-- Indicates that overlay hardware is capable of stretching. The dwMinOverlayStretch and dwMaxOverlayStretch members contain valid data."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		_stprintf_s(buffer, _T("------ dwMinOverlayStretch : %d "), pCaps->dwMinOverlayStretch );
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		_stprintf_s(buffer, _T("------ dwMaxOverlayStretch : %d "), pCaps->dwMaxOverlayStretch );
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_PALETTE   )
	{
		_stprintf_s(buffer, _T("--DDCAPS_PALETTE-- Indicates that DirectDraw is capable of creating and supporting DirectDrawPalette objects for more surfaces than only the primary surface. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_PALETTEVSYNC   )
	{
		_stprintf_s(buffer, _T("--DDCAPS_PALETTEVSYNC-- Indicates that DirectDraw is capable of updating a palette synchronized with the vertical refresh. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_READSCANLINE    )
	{
		_stprintf_s(buffer, _T("--DDCAPS_READSCANLINE-- Indicates that display hardware is capable of returning the current scan line. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	//if(pCaps->dwCaps&DDCAPS_STEREOVIEW    )
	//{
	//	_stprintf_s(buffer, _T("--DDCAPS_STEREOVIEW-- Indicates that display hardware has stereo vision capabilities. "));
	//	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	//}
	if(pCaps->dwCaps&DDCAPS_VBI    )
	{
		_stprintf_s(buffer, _T("--DDCAPS_VBI-- Indicates that display hardware is capable of generating a vertical-blank interrupt. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_ZBLTS     )
	{
		_stprintf_s(buffer, _T("--DDCAPS_ZBLTS-- Not supported."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps&DDCAPS_ZOVERLAYS     )
	{
		_stprintf_s(buffer, _T("--DDCAPS_ZOVERLAYS-- Supports the use of the IDirectDrawSurface5::UpdateOverlayZOrder method as a Z-value for overlays to control their layering"));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}

	_stprintf_s(buffer, _T(" "), pCaps->dwCaps );
	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	return S_OK;
}

HRESULT PrintCapsDwCaps2(HDC& hdc, DDCAPS* pCaps, DWORD& dwRowNumber, DWORD& dwLineNumber)
{
	TCHAR buffer[1024];
	_stprintf_s(buffer, _T("-- Additional driver-specific capabilities:[0x%x]  "), pCaps->dwCaps2 );
	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));

	if(pCaps->dwCaps2&DDCAPS2_AUTOFLIPOVERLAY     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_AUTOFLIPOVERLAY--The overlay can be automatically flipped to the next surface in the flip chain each time a video port VSYNC occurs, allowing \
			the video port and the overlay to double buffer the video without CPU overhead. 	This option is only valid when the surface is receiving data from a video port. If the \
			video port data is non-interlaced or non-interleaved, it will flip on every VSYNC. 	If the data is being interleaved in memory, it will flip on every other VSYNC. 	"));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_CANBOBHARDWARE     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_CANBOBHARDWARE-- The overlay hardware can display each field of an interlaced video stream individually."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_CANBOBINTERLEAVED      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_CANBOBINTERLEAVED -- The overlay hardware can display each field of an interlaced video stream individually while it is interleaved \
			in memory without causing any artifacts that might normally occur without special hardware support. This option is only valid when the surface is receiving data \
			from a video port and is only valid when the video is zoomed at least two times in the vertical direction."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_CANBOBNONINTERLEAVED      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_CANBOBNONINTERLEAVED -- The overlay hardware can display each field of an interlaced video stream individually while it is not	\
			interleaved in memory without causing any artifacts that might normally occur without special hardware support. This option is only valid when the surface is \
			receiving data from a video port and is only valid when the video is zoomed at least two times in the vertical direction."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_CANDROPZ16BIT      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_CANDROPZ16BIT -- 16-bit RGBZ values can be converted into 16-bit RGB values. (The system does not support 8-bit conversions.)"));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_CANFLIPODDEVEN     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_CANFLIPODDEVEN-- The driver is capable of performing odd and even flip operations, as specified by the DDFLIP_ODD and DDFLIP_EVEN	\
			flags used with the IDirectDrawSurface5::Flip method."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_CANRENDERWINDOWED     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_CANRENDERWINDOWED-- The driver is capable of rendering in windowed mode."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_CERTIFIED      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_CERTIFIED -- Indicates that display hardware is certified.  "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_COLORCONTROLPRIMARY      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_COLORCONTROLPRIMARY -- The primary surface contains color controls (for instance, gamma).  "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_COLORCONTROLOVERLAY      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_COLORCONTROLOVERLAY -- The overlay surface contains color controls, such as brightness and sharpness."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_COPYFOURCC     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_COPYFOURCC-- Indicates that the driver supports blitting any FOURCC surface to another surface of the same FOURCC."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_FLIPINTERVAL     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_FLIPINTERVAL-- Indicates that the driver will respond to the DDFLIP_INTERVAL* flags. (see IDirectDrawSurface5::Flip)."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_FLIPNOVSYNC     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_FLIPNOVSYNC-- Indicates that the driver will respond to the DDFLIP_NOVSYNC flag (see IDirectDrawSurface5::Flip)."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_NO2DDURING3DSCENE      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_NO2DDURING3DSCENE -- Indicates that 2-D operations such as IDirectDrawSurface5::Blt and IDirectDrawSurface5::Lock \
			cannot be performed on any surfaces that Microsoft? Direct3D? Mobile is using between calls to the IDirect3DMobileDevice::BeginScene and IDirect3DMobileDevice::EndScene methods."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_NONLOCALVIDMEM     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_NONLOCALVIDMEM-- Indicates that the display driver supports surfaces in non-local video memory. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_NONLOCALVIDMEMCAPS     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_NONLOCALVIDMEMCAPS-- Indicates that blit capabilities for non-local video memory surfaces differ from local video memory surfaces. \
			If this flag is present, the DDCAPS2_NONLOCALVIDMEM flag will also be present."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_NOPAGELOCKREQUIRED     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_NOPAGELOCKREQUIRED-- DMA blit operations are supported on system memory surfaces that are not page locked."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_PRIMARYGAMMA     )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_PRIMARYGAMMA-- Supports dynamic gamma ramps for the primary surface."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_VIDEOPORT      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_VIDEOPORT -- Indicates that display hardware supports live video. "));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}
	if(pCaps->dwCaps2&DDCAPS2_WIDESURFACES      )
	{
		_stprintf_s(buffer, _T("--DDCAPS2_WIDESURFACES -- Indicates that the display surfaces supports surfaces wider than the primary surface."));
		TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	}

	_stprintf_s(buffer, _T(" ") );
	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	return S_OK;
}

HRESULT PrintFourCCInfo(HDC& hdc, DDCAPS* pCaps, DWORD& dwRowNumber, DWORD& dwLineNumber, DWORD dwNumFourCCCodes, DWORD *pdwFourCCCodes)
{
	TCHAR buffer[1024];
	for(DWORD j=0;j<dwNumFourCCCodes;j++)
	{
		int i;
		for(i=0;i<sizeof(sFmt)/sizeof(FOURCC_FORMAT);i++)
		{
			if(sFmt[i].code == pdwFourCCCodes[j])
			{
				_stprintf_s(buffer, _T("--%s %s FOURCC: 0x%x"), sFmt[i].Fmt, sFmt[i].Label,sFmt[i].code);
				TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
				break;
			}
		}
		if(i == sizeof(sFmt)/sizeof(FOURCC_FORMAT))
		{
			_stprintf_s(buffer, _T("--Unknown FOURCC: 0x%x"), pdwFourCCCodes[j]);
			TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
		}
	}
	return S_OK;
}
HRESULT PrintCapsOverlay(HDC& hdc, DDCAPS* pCaps, DWORD& dwLineNumber)
{
	TCHAR buffer[1024];
	_stprintf_s(buffer, _T("-- Overlay capabilities:  "));
	TextOut(hdc, 0,LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));

	_stprintf_s(buffer, _T("---- Total amount of display memory :  %d Mbytes"), pCaps->dwVidMemTotal/1024/1024);
	TextOut(hdc, 0,LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	_stprintf_s(buffer, _T("---- Free display memory :  %d Mbytes"), pCaps->dwVidMemFree/1024/1024 );
	TextOut(hdc, 0,LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));

	_stprintf_s(buffer, _T("---- Maximum number of visible overlays or overlay sprites.:  %d "), pCaps->dwMaxVisibleOverlays  );
	TextOut(hdc, 0,LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	_stprintf_s(buffer, _T("---- Current number of visible overlays or overlay sprites:  %d "), pCaps->dwCurrVisibleOverlays );
	TextOut(hdc, 0,LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));

	_stprintf_s(buffer, _T(" ") );
	TextOut(hdc, 0,LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	return S_OK;
}


HRESULT PrintDetailInfo(DDCAPS* pCaps, DWORD dwNumFourCCCodes, DWORD *pdwFourCCCodes, DWORD dwRowNumber, DWORD dwLineNumber, HWND hWnd)
{
	if(pCaps == NULL)
		return  E_FAIL;

	TCHAR buffer[256];
	HDC hdc;
	hdc= GetDC(hWnd);

	//Show Capablity of DDRAW
	_stprintf_s(buffer, _T("Monitor[%d] capabilities: "),dwMonCount  );
	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));
	_stprintf_s(buffer, _T(" "), pCaps->dwCaps );
	TextOut(hdc, dwRowNumber, LINE_GAP_PIXEL*dwLineNumber++,buffer, (int)_tcslen(buffer));

	PrintCapsDwCaps(hdc, pCaps, dwRowNumber, dwLineNumber);
	PrintCapsDwCaps2(hdc, pCaps, dwRowNumber, dwLineNumber);
	PrintFourCCInfo(hdc, pCaps, dwRowNumber, dwLineNumber, dwNumFourCCCodes, pdwFourCCCodes);
	
	return S_OK;
}

HRESULT	PrintMonitorInfo(GfxMonitorInfo	*m_pMon, DWORD dwRowNumber, DWORD dwLineNumber, HWND hWnd)
{
	DDCAPS				m_Caps;
	IDirectDraw7		*m_pDDraw = NULL;
	DWORD				m_dwNumFourCCCodes = 0;
	DWORD				*m_pdwFourCCCodes = NULL;


	if(m_pMon == NULL)
		return  E_FAIL;

	ZeroMemory(&m_Caps,sizeof(m_Caps));
	m_Caps.dwSize = sizeof(m_Caps);

#ifdef _WIN32_WCE
	if(FAILED(DirectDrawCreate(0,(LPDIRECTDRAW*)&m_pDDraw,0)))
		return E_FAIL;
#else
	if(FAILED(DirectDrawCreateEx(m_pMon?&m_pMon->guid:0,(void**)&m_pDDraw,IID_IDirectDraw7,0)))
		return E_FAIL;
#endif
	if(FAILED(m_pDDraw->GetCaps(&m_Caps,0)))
	{
		m_pDDraw->Release();
		m_pDDraw = 0;
		return E_FAIL;
	}


	if(m_Caps.dwNumFourCCCodes)
	{
		m_dwNumFourCCCodes = m_Caps.dwNumFourCCCodes;
		m_pdwFourCCCodes = new DWORD[m_Caps.dwNumFourCCCodes];
		m_pDDraw->GetFourCCCodes(&m_dwNumFourCCCodes,m_pdwFourCCCodes);
		//OutputDebugStringA((char *)m_pdwFourCCCodes);
	}
	
	PrintDetailInfo(&m_Caps, m_dwNumFourCCCodes, m_pdwFourCCCodes, dwRowNumber, dwLineNumber, hWnd);

	if(m_pdwFourCCCodes)
		delete m_pdwFourCCCodes;
	
	m_pDDraw->Release();
	m_pDDraw = 0;
	return S_OK;
}

HRESULT Probe(HWND hWnd, DWORD dwRowNumber, DWORD dwLineNumber)
{
	if(FAILED(EnumerateMonitors()))
		return -1;

	PrintMonitorInfo(&m_aMonitors[dwMonCount], dwRowNumber, dwLineNumber, hWnd);

	if(dwMonCount >= m_dwNumMonitors)
		dwMonCount=0;
	else
		dwMonCount ++;

	return S_OK;	
}


