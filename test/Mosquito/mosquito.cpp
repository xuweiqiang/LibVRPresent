//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//-----------------------------------------------------------------------------
// File: Mosquito.cpp
//
// Desc: Mosquito is a DirectDraw sample application that demonstates the
//       use of video overlay. It creates a flipable overlay, loads a small
//       animation into the various back buffers, then flips the buffers as
//       it moves the overlay around the screen. Press F12 or ESC to quit.
//
//-----------------------------------------------------------------------------

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <windows.h>
#include <ddraw.h>
#include "resource.h"
#include "winuserm.h"

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                TEXT("YV12WndClass")
#define TITLE               TEXT("YV12Test")

#define VIDEO_WIDTH           1280
#define VIDEO_HEIGHT          720

#define DST_WIDTH           800
#define DST_HEIGHT          480

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
LPDIRECTDRAW                g_pDD = NULL;             // DirectDraw object
LPDIRECTDRAWSURFACE         g_pDDSPrimary = NULL;     // Primary Surface.
LPDIRECTDRAWSURFACE         g_pDDSOverlay = NULL;     // The overlay primary.
LPDIRECTDRAWSURFACE         g_pDDSBackbuffer = NULL;     // The overlay backbuffer.
BOOL                        g_bActive = FALSE;        // Is application active?
LPCTSTR                     g_szErrorMessage = NULL;  // Error message to display.
// Overlay position and velocity data.

int g_nOverlayWidth, g_nOverlayHeight;

// Our instance handle.

HINSTANCE g_hInstance;

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
static TCHAR                szImg1[] = TEXT("IDB_BUGIMAGE1");
static TCHAR                szImg2[] = TEXT("IDB_BUGIMAGE2");
static TCHAR                szImg3[] = TEXT("IDB_BUGIMAGE3");

static const TCHAR          szDDrawError[] = TEXT("DirectDraw Error");
static const TCHAR          szDDrawFailedMsg[] = TEXT("DirectDrawCreate failed.");
static const TCHAR          szSetCooperativeFailMsg[] = TEXT("SetCooperativeLevel failed.");
static const TCHAR          szNoOverlaysMsg[] = TEXT("Overlays are not supported in hardware!");
static const TCHAR          szCreateOverlayFailMsg[] = TEXT("Unable to create overlay surface!");
static const TCHAR          szLoadOverlayFailMsg[] = TEXT("Unable to load images to overlay surface!");
static const TCHAR          szShowOverlayFailMsg[] = TEXT("Unable to show overlay surface!");
static const TCHAR          szNoFlipSurfacesMsg[] =TEXT("Display driver doesn't support flipping surfaces.");
static const TCHAR          szEnumAttachedSurfacesFailMsg[] = TEXT("EnumAttachedSurfaces failed.");
static const TCHAR          szCreateSurfaceFailMsg[] = TEXT("CreateSurface failed.");
static const TCHAR          szSetTimerFailMsg[] = TEXT("SetTimer failed.");
static const TCHAR          szGetCapsFailMsg[] = TEXT("GetCaps FAILED");

// YV12 format
static DDPIXELFORMAT ddpfOverlayFormat = 
//    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','V','1','2'),0,0,0,0,0};
    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','V'),0,0,0,0,0};

static DDPIXELFORMAT ddpfOverlayFormat_RGB = 
    {sizeof(DDPIXELFORMAT), DDPF_RGB, MAKEFOURCC('R','5','6','5'),16,0xf800,0x07e0,0x001f,0};

#define PF_TABLE_SIZE (sizeof(ddpfOverlayFormats) / sizeof(ddpfOverlayFormats[0]))

static RECT rs;
static RECT rd;

const DWORD TIMER_ID = 1;
const DWORD TIMER_RATE = 20;

static unsigned char* s_imageBuffer[3];
static int s_currentImage;

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void ReleaseAllObjects(void);
HRESULT InitFail(HWND, HRESULT, LPCTSTR, ...);
HRESULT RestoreAllSurfaces();
long FAR PASCAL WindowProc(HWND, UINT, WPARAM, LPARAM);
BOOL CopyBitmapToYUVSurface(LPDIRECTDRAWSURFACE, HBITMAP);
HRESULT WINAPI EnumSurfacesCallback(LPDIRECTDRAWSURFACE, LPDDSURFACEDESC, LPVOID);
HRESULT InitApp(HINSTANCE hInstance, int nCmdShow);

//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------
static void
ReleaseAllObjects(void)
{
    if (g_pDDSBackbuffer != NULL)
    {
        g_pDDSBackbuffer->Release();
        g_pDDSBackbuffer = NULL;
    }

    if (g_pDDSOverlay != NULL)
    {
        // Use UpdateOverlay() with the DDOVER_HIDE flag to remove an overlay 
        // from the display.
        g_pDDSOverlay->UpdateOverlay(NULL, g_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
        g_pDDSOverlay->Release();
        g_pDDSOverlay = NULL;
    }

    if (g_pDDSPrimary != NULL)
    {
        g_pDDSPrimary->Release();
        g_pDDSPrimary = NULL;
    }

    if (g_pDD != NULL)
    {
        g_pDD->Release();
        g_pDD = NULL;
    }
}

//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
static HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
    ReleaseAllObjects();
    DestroyWindow(hWnd);
    g_szErrorMessage = szError;
    return hRet;
}

//-----------------------------------------------------------------------------
// Name: RestoreAllSurfaces
// Desc: Called in case we lose our surface's vram.
//-----------------------------------------------------------------------------
static HRESULT 
RestoreAllSurfaces()
{
    HRESULT hRet;

    // Try Restoring the primary surface.

    hRet = g_pDDSPrimary->Restore();
    if (hRet != DD_OK)
        return hRet;

    // Try Restoring the overlay surface.

    hRet = g_pDDSOverlay->Restore();
    if (hRet != DD_OK)
        return hRet;

    hRet = g_pDDSBackbuffer->Restore();
    if (hRet != DD_OK)
        return hRet;

    // Show the overlay.

    hRet = g_pDDSOverlay->UpdateOverlay(&rs, g_pDDSPrimary, &rs, DDOVER_SHOW, NULL);
    return hRet;
}



//-----------------------------------------------------------------------------
// Name: MoveOverlay()
// Desc: Called on the timer, this function moves the overlay around the
//       screen, periodically calling flip to animate the mosquito.
//-----------------------------------------------------------------------------
static void 
SwapOverlay()
{
    HRESULT hRet;

    // Flip.
    DDSURFACEDESC ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

    hRet=g_pDDSBackbuffer->Lock( NULL, &ddsd, 0, NULL);

    int size = VIDEO_WIDTH*VIDEO_HEIGHT;
    memcpy(ddsd.lpSurface, s_imageBuffer[s_currentImage], size + (size >> 1));
    s_currentImage++;
    if (s_currentImage >= 3)
        s_currentImage = 0;

    g_pDDSBackbuffer->Unlock(NULL);

    while (TRUE)
    {
        hRet = g_pDDSOverlay->Flip(NULL, 0);
        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
            hRet = RestoreAllSurfaces();
            if (hRet != DD_OK)
                break;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            break;
    }
}


//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
#ifdef UNDER_CE
        case WM_ACTIVATE:
#else
        case WM_ACTIVATEAPP:
#endif
            // Pause if minimized or not the top window
            g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
            return 0L;

        case WM_KILLFOCUS:
            // We do not allow anyone else to have the keyboard focus until
            // we are done.
            SetFocus(hWnd);
            return 0L;

        case WM_DESTROY:
            // Clean up and close the app
            ReleaseAllObjects();
            PostQuitMessage(0);
            return 0L;

        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
                case VK_ACTION:
                case VK_RETURN:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
            }

        case WM_TIMER:
            if (g_bActive && TIMER_ID == wParam)
            {
                SwapOverlay();
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



static unsigned char* 
CopyBitmapToYUVMemory(int width, int height, HBITMAP hbm)
{
    HDC                 hdcImage;
    HRESULT             ddrval;
    DDSURFACEDESC       ddsd;
    DWORD               x, y, dwWidth, dwHeight;
    LPBYTE              pSurf;
    COLORREF            color;
    BYTE                R,G,B, Y0,Y1,U,V;
    BOOL                bRet = FALSE;

    if (hbm == NULL)
        return NULL;
    
    //
    //  select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    SelectObject(hdcImage, hbm);

    int size = width * height;
    unsigned char* block = new unsigned char[size + (size>>1)];

    dwWidth=width;
    dwHeight=height;
    pSurf=(LPBYTE)block;

    for(y=0; y<dwHeight; y++)
    {
        for(x=0; x<dwWidth; x++)
        {
            color=GetPixel(hdcImage, x,y);
            R=GetRValue(color);
            G=GetGValue(color);
            B=GetBValue(color);
            Y0= (BYTE)(0.29*R + 0.59*G + 0.14*B);
            *(pSurf++) = Y0;
        }
    }

    for(y=0; y<dwHeight; y+=2)
    {
        for(x=0; x<dwWidth; x+=2)
        {
            color=GetPixel(hdcImage, x,y);
            R=GetRValue(color);
            G=GetGValue(color);
            B=GetBValue(color);
            U= (BYTE)(128.0 - 0.14*R - 0.29*G + 0.43*B);
            *(pSurf++) = U;
        }
    }

    for(y=0; y<dwHeight; y+=2)
    {
        for(x=0; x<dwWidth; x+=2)
        {
            color=GetPixel(hdcImage, x,y);
            R=GetRValue(color);
            G=GetGValue(color);
            B=GetBValue(color);
            V= (BYTE)(128.0 + 0.36*R - 0.29*G - 0.07*B);
            *(pSurf++) = V;
        }
    }

CleanUp:
    if(hdcImage)
    DeleteDC(hdcImage);

    return block;
}

//-----------------------------------------------------------------------------
// Name: EnumSurfacesCallback()
// Desc: Used by LoadBugImages to aid it loading all three bug images.
//-----------------------------------------------------------------------------
static HRESULT WINAPI 
EnumSurfacesCallback(LPDIRECTDRAWSURFACE lpDDSurface,  
                     LPDDSURFACEDESC lpDDSurfaceDesc,  
                     LPVOID lpContext)
{
    int * CallCount = (int *)lpContext;
    HRESULT hr = (HRESULT)DDENUMRET_OK;

    // Load the Bug Image appropriate...

    if (*CallCount == 0) {
        g_pDDSBackbuffer = lpDDSurface;
    }
    else {
        // Eh?
        hr = (HRESULT)DDENUMRET_CANCEL;
        lpDDSurface->Release();
        goto exit;
    }

    // Bump the count.
    (*CallCount)++;

exit:
    return hr;
}



//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow)
{
    HWND                        hWnd;
    WNDCLASS                    wc;
    DDSURFACEDESC               ddsd;
    DDCAPS                      ddcaps;
    HRESULT                     hRet;
    DWORD                       dwUpdateFlags = 0;
    DDOVERLAYFX                 ovfx;

    DEVMODE                     DevMode;

    // Check for rotation support by getting the rotation angles supported.
 
    memset(&DevMode, 0, sizeof(DevMode));
    DevMode.dmSize = sizeof(DevMode);
    DevMode.dmFields = DM_DISPLAYQUERYORIENTATION;
    DWORD g_RotationAngles;
    DWORD g_CurrentAngle;

    if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL)) {

        g_RotationAngles = DevMode.dmDisplayOrientation;
    }
    else {

        OutputDebugString(L"MOSQUITO: Device does not support any rotation modes. Rotation disabled.");
        g_RotationAngles = -1;
    }

    // Get the current rotation angle.

    memset(&DevMode, 0, sizeof (DevMode));
    DevMode.dmSize = sizeof (DevMode);
    DevMode.dmFields = DM_DISPLAYORIENTATION;

    if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL)) {

        g_CurrentAngle = DevMode.dmDisplayOrientation;
    }
    else {

        OutputDebugString(L"MOSQUITO: Unable to read current rotation. Rotation disabled.");
        g_CurrentAngle = -1;
    }

    // Set up and register window class.

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NAME;
    RegisterClass(&wc);

    // Create a window.

    hWnd = CreateWindowEx(WS_EX_TOPMOST,
                          NAME,
                          TITLE,
                          WS_POPUP,
                          0,
                          0,
                          GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    if (!hWnd)
        return FALSE;
    // We never show the window, only set focus to it.
    SetFocus(hWnd);

    // Create the main DirectDraw object

    hRet = DirectDrawCreate(NULL, &g_pDD, NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, szDDrawFailedMsg);

    // Get normal mode.

    hRet = g_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, szSetCooperativeFailMsg);

    // Get a primary surface interface pointer (only needed for init.)

    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, szCreateSurfaceFailMsg);

    // See if we can support overlays.

    memset(&ddcaps, 0, sizeof(ddcaps));
    ddcaps.dwSize = sizeof(ddcaps);
    hRet = g_pDD->GetCaps(&ddcaps, NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, szGetCapsFailMsg);

//    if (ddcaps.dwOverlayCaps == 0)
//        return InitFail(hWnd, hRet, szNoOverlaysMsg);

    rs.left = 0;
    rs.top = 0;
    rs.right = VIDEO_WIDTH;
    rs.bottom = VIDEO_HEIGHT;
    if (ddcaps.dwAlignSizeSrc != 0)
        rs.right += rs.right % ddcaps.dwAlignSizeSrc;
    
    // Create the overlay flipping surface. We will attempt the pixel formats
    // in our table one at a time until we find one that jives.

    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP;
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT | 
                   DDSD_PIXELFORMAT;
    ddsd.dwWidth = rs.right;
    ddsd.dwHeight = rs.bottom;
    ddsd.dwBackBufferCount = 1;

    ddsd.ddpfPixelFormat = ddpfOverlayFormat;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSOverlay, NULL);

    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, szCreateOverlayFailMsg);

    // Finish setting up the overlay.
    DWORD CallCount = 0;
    hRet = g_pDDSOverlay->EnumAttachedSurfaces((LPVOID)&CallCount,EnumSurfacesCallback);

    // Set the flags we'll send to UpdateOverlay
    dwUpdateFlags = DDOVER_SHOW;
    memset(&ovfx, 0, sizeof(ovfx));
    ovfx.dwSize = sizeof(ovfx);
    //if (ddcaps.dwOverlayCaps & DDOVERLAYCAPS_CKEYSRC)
    //{
    //    dwUpdateFlags |= DDOVER_KEYSRCOVERRIDE;

    //    // Create an overlay FX structure so we can specify a source color key.
    //    // This information is ignored if the DDOVER_SRCKEYOVERRIDE flag 
    //    // isn't set.
    //    ovfx.dckSrcColorkey.dwColorSpaceLowValue=0; // black as the color key
    //    ovfx.dckSrcColorkey.dwColorSpaceHighValue=0;
    //}

    // Create a timer to flip the pages.
    HBITMAP hbm = (HBITMAP)LoadImage(g_hInstance, szImg1, IMAGE_BITMAP, 0, 0, 0);
    s_imageBuffer[0] = CopyBitmapToYUVMemory(VIDEO_WIDTH, VIDEO_HEIGHT, hbm);

    hbm = (HBITMAP)LoadImage(g_hInstance, szImg2, IMAGE_BITMAP, 0, 0, 0);
    s_imageBuffer[1] = CopyBitmapToYUVMemory(VIDEO_WIDTH, VIDEO_HEIGHT, hbm);

    hbm = (HBITMAP)LoadImage(g_hInstance, szImg3, IMAGE_BITMAP, 0, 0, 0);
    s_imageBuffer[2] = CopyBitmapToYUVMemory(VIDEO_WIDTH, VIDEO_HEIGHT, hbm);

    s_currentImage = 0;


	rd.left = 0;
	rd.top = 0;
    rd.right = DST_WIDTH;
    rd.bottom = DST_HEIGHT;

    hRet = g_pDDSOverlay->UpdateOverlay(&rs, g_pDDSPrimary, &rd, dwUpdateFlags, &ovfx);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, szShowOverlayFailMsg);
/*
    hRet = g_pDDSOverlay->SetOverlayPosition(32, 64);
    if (hRet == DDERR_SURFACELOST)
    {
        if (FAILED(RestoreAllSurfaces())) 
            return InitFail(hWnd, hRet, szShowOverlayFailMsg);
    }
*/
    if (TIMER_ID != SetTimer(hWnd, TIMER_ID, TIMER_RATE, NULL))
        return InitFail(hWnd, hRet, szSetTimerFailMsg);

    return DD_OK;
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
#ifdef UNDER_CE
        LPWSTR lpCmdLine,
#else
        LPSTR lpCmdLine,
#endif
        int nCmdShow)
{
    MSG                         msg;

    g_hInstance = hInstance;

    InitApp(hInstance, nCmdShow);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (g_szErrorMessage != NULL){
        MessageBox(NULL, g_szErrorMessage, szDDrawError, MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }

    ReleaseAllObjects();

    return msg.wParam;
}
