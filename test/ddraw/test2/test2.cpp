// test2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "test2.h"
#include <windows.h>
#include <commctrl.h>

//#include "LibCpuIdle.h"
//#pragma comment(lib, "LibCpuIdle.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#pragma comment(lib, "ddraw.lib")
//#pragma comment(lib, "dxguid.lib")


static HRESULT initSurface(UINT32 u4Width,UINT32 u4Height);
static HRESULT Draw(VOID *prInbuf,UINT32 u4Size,UINT32 u4Width,UINT32 u4Height);
static void deinitSurface();

#include "ddraw.h"
LPDIRECTDRAW         g_pDD = NULL;
LPDIRECTDRAWSURFACE  g_pDDSOverlay = NULL;
LPDIRECTDRAWSURFACE  g_pDDSPrimary = NULL;
DDOVERLAYFX          ovfx;
DDCAPS               ddcaps;



typedef struct _DDSARRAY
{
	UINT32  u4Count;
	LPDIRECTDRAWSURFACE lpSurf[10];
} DDSARRAY_T;

DDSARRAY_T rDDSArray;

static HRESULT PASCAL EnumFunction(LPDIRECTDRAWSURFACE pSurface,
								   LPDDSURFACEDESC lpSurfaceDesc,
								   LPVOID  lpContext)
{
	DDSARRAY_T *prArray = (DDSARRAY_T *)lpContext;

	prArray->lpSurf[prArray->u4Count] = pSurface;
	prArray->lpSurf[prArray->u4Count]->AddRef();
	prArray->u4Count++;

	return DDENUMRET_OK;
}

/*
static DDPIXELFORMAT ddpfOverlayFormats[] = {
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('M','T','K','1'), 0, 0, 0, 0, 0}
};
*/

//static DDPIXELFORMAT ddpfOverlayFormats[] = {
//    {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16, 0xF800, 0x07E0, 0x001F, 0} //RGB Overlay
//};

//static DDPIXELFORMAT ddpfOverlayFormats[] = {
//    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','V')} 
//};
static DDPIXELFORMAT ddpfOverlayFormats[] = {
    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','V','1','2')} 
};


static HRESULT initSurface(UINT32 u4Width,UINT32 u4Height)
{
	HRESULT hr = S_FALSE;
	DDSURFACEDESC ddsd;

	hr = DirectDrawCreate(NULL, &g_pDD, NULL);
	if (hr != DD_OK)
	{
		return (S_FALSE);
	}

	hr = g_pDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	if (hr != DD_OK)
	{
		if (g_pDD)
		{
			g_pDD->Release();
			g_pDD = NULL;
		}
		
		return (S_FALSE);
	}

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hr = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
	if (hr != DD_OK)
	{
		if (g_pDD)
		{
			g_pDD->Release();
			g_pDD = NULL;
		}
		return (S_FALSE);
	}

	memset(&ddcaps, 0, sizeof(ddcaps));
	ddcaps.dwSize = sizeof(ddcaps);
	hr = g_pDD->GetCaps(&ddcaps, NULL);
	if (hr != DD_OK || (ddcaps.dwOverlayCaps == 0))
	{
		if (g_pDDSPrimary)
		{
			g_pDDSPrimary->Release();
			g_pDDSPrimary = NULL;
		}
		if (g_pDD)
		{
			g_pDD->Release();
			g_pDD = NULL;
		}
		
		return (S_FALSE);
	}


	DDCAPS				m_Caps;
	ZeroMemory(&m_Caps,sizeof(m_Caps));
	m_Caps.dwSize = sizeof(m_Caps);
	if(FAILED(g_pDD->GetCaps(&m_Caps,0)))
	{
		return E_FAIL;
	}
		DWORD				m_dwNumFourCCCodes;
		DWORD				*m_pdwFourCCCodes;

	if(m_Caps.dwNumFourCCCodes)
	{

		m_dwNumFourCCCodes = m_Caps.dwNumFourCCCodes;
		m_pdwFourCCCodes = new DWORD[m_Caps.dwNumFourCCCodes];
		g_pDD->GetFourCCCodes(&m_dwNumFourCCCodes,m_pdwFourCCCodes);
		//OutputDebugStringA((char *)m_pdwFourCCCodes);
	}



	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT |
		DDSD_PIXELFORMAT;
	ddsd.dwWidth = u4Width;
	ddsd.dwHeight = u4Height;
	ddsd.dwBackBufferCount = 1;
	ddsd.ddpfPixelFormat = ddpfOverlayFormats[0];
	hr = g_pDD->CreateSurface(&ddsd, &g_pDDSOverlay, NULL);
	if (hr != DD_OK)
	{
		if (g_pDDSPrimary)
		{
			g_pDDSPrimary->Release();
			g_pDDSPrimary = NULL;
		}
		if (g_pDD)
		{
			g_pDD->Release();
			g_pDD = NULL;
		}
		return (S_FALSE);
	}

	rDDSArray.u4Count = 0;

	hr = g_pDDSOverlay->EnumAttachedSurfaces(&rDDSArray, EnumFunction);
	if (hr != DD_OK)
	{
		if (g_pDDSOverlay)
		{
			g_pDDSOverlay->Release();
			g_pDDSOverlay = NULL;
		}
		if (g_pDDSPrimary)
		{
			g_pDDSPrimary->Release();
			g_pDDSPrimary = NULL;
		}
		if (g_pDD)
		{
			g_pDD->Release();
			g_pDD = NULL;
		}
		return (S_FALSE);
	}

	return (S_OK);
}

static void deinitSurface()
{
	if (g_pDDSOverlay)
	{
		g_pDDSOverlay->UpdateOverlay(NULL, g_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
		g_pDDSOverlay->Release();
		g_pDDSOverlay = NULL;
	}
	if (g_pDDSPrimary)
	{
		g_pDDSPrimary->Release();
		g_pDDSPrimary = NULL;
	}
	if (g_pDD)
	{
		g_pDD->Release();
		g_pDD = NULL;
	}
}


static HRESULT Draw(VOID *prInbuf,UINT32 u4Size,UINT32 u4Width,UINT32 u4Height)
{
		HRESULT hr;
		DWORD dwUpdateFlags = DDOVER_SHOW;
		LPBYTE pSurfaceBuf = NULL;
		DDSURFACEDESC     ddsdSrc;
		
		static int bufferidx = 0;
		memset(&ddsdSrc, 0, sizeof(ddsdSrc));
		ddsdSrc.dwSize = sizeof(ddsdSrc);

		hr = rDDSArray.lpSurf[0]->Lock(NULL, &ddsdSrc, DDLOCK_WAITNOTBUSY, NULL);
		if (hr != DD_OK)
		{
			return S_FALSE;
		}

		pSurfaceBuf = (LPBYTE)ddsdSrc.lpSurface;

		memcpy(pSurfaceBuf, prInbuf, u4Size);
		//memcpy(pSurfaceBuf, m_puPicData, rImageSize.cx * rImageSize.cy * 2);

		rDDSArray.lpSurf[0]->Unlock(NULL);
		pSurfaceBuf = NULL;

		
		g_pDDSOverlay->Flip(rDDSArray.lpSurf[0], 0);

//		g_pDDSOverlay->Flip(NULL, 0);


		return S_OK;
}



int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST2));

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
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST2));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
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
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

    g_hInst = hInstance; // Store instance handle in our global variable


    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_TEST2, szWindowClass, MAX_LOADSTRING);


    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if (g_hWndCommandBar)
    {
        CommandBar_Show(g_hWndCommandBar, TRUE);
    }

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
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
            g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
            CommandBar_InsertMenubar(g_hWndCommandBar, g_hInst, IDR_MENU, 0);
            CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
			{

				long width = 800;
				long height = 480;
				long buffer_size  = width*height*3/2;
				unsigned char* pBuff = (unsigned char*)malloc(buffer_size );
				HRESULT hRet = E_FAIL;
				hRet = initSurface(width, height);


				RECT rsRect = {0, 0, 0, 0};
				RECT dstRect = {0, 0, 800, 480};

				rsRect.left = 0;
				rsRect.top = 0;
				rsRect.right = width;
				rsRect.bottom = height;
			
				HRESULT hr = g_pDDSOverlay->UpdateOverlay(&rsRect, g_pDDSPrimary, &dstRect/*&rd*/, DDOVER_SHOW, NULL);
				if (hr != DD_OK)
				{
					return S_FALSE;
				}


				int j=0;

				long long frameCount = 0;
				long long startTick = GetTickCount();

				long preferedFrameRate = 30;
				long preferedFrameGap = 1000/preferedFrameRate;
				long loopstartTick = 0;
				long loopendTick = 0;

				while(1)
				{

					loopstartTick  = GetTickCount();
					j++;
					if(j>= 16)
						j=0;
					frameCount++;
					memset(pBuff, rand(), buffer_size);

					Draw(pBuff + j*4,buffer_size,width,height);

					if(frameCount%30 == 29)
					{
						float fps = frameCount*1000/(GetTickCount() - startTick);
						printf("fps:%02f \n",fps);

					}
					if(frameCount > preferedFrameRate *10)
						break;

					loopendTick  = GetTickCount();
					if(loopendTick - loopstartTick <preferedFrameGap)
						Sleep((preferedFrameGap- (loopendTick - loopstartTick )));
				}

				deinitSurface();
				free(pBuff);


				exit(0);
			}
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndCommandBar);
            PostQuitMessage(0);
            break;


        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            RECT rectChild, rectParent;
            int DlgWidth, DlgHeight;	// dialog width and height in pixel units
            int NewPosX, NewPosY;

            // trying to center the About dialog
            if (GetWindowRect(hDlg, &rectChild)) 
            {
                GetClientRect(GetParent(hDlg), &rectParent);
                DlgWidth	= rectChild.right - rectChild.left;
                DlgHeight	= rectChild.bottom - rectChild.top ;
                NewPosX		= (rectParent.right - rectParent.left - DlgWidth) / 2;
                NewPosY		= (rectParent.bottom - rectParent.top - DlgHeight) / 2;
				
                // if the About box is larger than the physical screen 
                if (NewPosX < 0) NewPosX = 0;
                if (NewPosY < 0) NewPosY = 0;
                SetWindowPos(hDlg, 0, NewPosX, NewPosY,
                    0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return TRUE;

    }
    return (INT_PTR)FALSE;
}
