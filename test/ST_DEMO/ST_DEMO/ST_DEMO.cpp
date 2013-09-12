// ST_DEMO.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ST_DEMO.h"
#include <windows.h>
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

/************				NEW CODE	START	*******************/
#include "stglib.h"
#include "sga_types.h"
#pragma comment(lib, "sgalib.lib")

typedef struct
{
	PVOID pBuf;
	int iBufSize;
}STBUFFER_S;

FILE* m_fp = NULL;
STHDC m_hdc;
HWND m_hwnd;	
RECT				m_rectSrc, m_rectDst, m_rectScn;
DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
int				m_dwBackBuffers;
STHBITMAP* m_hbitYUV_Block;
STBUFFER_S* m_pbuff;

LPBYTE m_srcBuff = NULL;
#define DEFAULT_BACKBUFFER	1
#define SRC_WID 320
#define SRC_HEIGHT 240
#define DST_WID 320
#define DST_HEIGHT 240


int TestST(HWND hwnd)
{

	m_hwnd = 0;
	memset(&m_rectSrc, 0 ,sizeof(m_rectSrc));
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	memset(&m_rectScn, 0 ,sizeof(m_rectScn));

	m_dwWidth = 0;
	m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_hbitYUV_Block = NULL;
	m_pbuff = NULL;

	m_hwnd = hwnd;
	m_hdc = STGetDC(m_hwnd);
	if (!m_hdc)
		return E_FAIL;

	m_dwBackBuffers = DEFAULT_BACKBUFFER;

	m_dwWidth = SRC_WID;
	m_dwHeight = SRC_HEIGHT;

	m_hbitYUV_Block = (STHBITMAP*)malloc(sizeof(STHBITMAP)*m_dwBackBuffers);
	if(m_hbitYUV_Block == NULL)
		return E_FAIL;

	int ret = STCreateVideoBuffers(m_hdc, m_dwWidth, m_dwHeight, SGA_DIS_FMT_YUV420, &m_dwBackBuffers, m_hbitYUV_Block);
	if(ret == FALSE)
		return E_FAIL;

	m_pbuff = (STBUFFER_S*)malloc(sizeof(STBUFFER_S)* m_dwBackBuffers);
	if(m_pbuff == NULL)
		return E_FAIL;

	for(int i=0;i<m_dwBackBuffers;i++)
	{
		ret = STGetVideoBufferAddress(m_hdc, m_hbitYUV_Block[i], &(m_pbuff[i].pBuf));
		ret &= STGetVideoBufferSize(m_hdc, m_hbitYUV_Block[i], &(m_pbuff[i].iBufSize));
		if(ret == FALSE)
			return E_FAIL;
	}

	STSetScreenOffset(m_hdc, 0, 0);


	fopen_s(&m_fp , "\\Storage Card\\cmmbvideo_qvga_ref.yuv", "rb");
	if(m_fp == NULL)
		return E_FAIL;

	fseek(m_fp, 0 , SEEK_SET);

	//m_srcBuff = (LPBYTE)malloc(m_dwWidth * m_dwHeight *3/2);
	//if(m_srcBuff == NULL)
	//	return E_FAIL;
	int id = 0;
	while(!feof(m_fp))
	{
		int iRead = fread(m_pbuff[id].pBuf ,sizeof(BYTE),m_dwWidth * m_dwHeight *3/2,m_fp);
		if(iRead < m_dwWidth * m_dwHeight *3/2)
			break;
		STSetScreenOffset(m_hdc, 0, 0);
		//STFillRect(m_hdc, 0, 0, m_dwWidth, m_dwHeight, 0x00000000);

		//STGetSGAStatistics(m_hdc);
		STSetVideoBufferWindow(m_hdc, m_hbitYUV_Block[id], 0, 0, DST_WID, DST_HEIGHT);
		STRenderVideoBuffer(m_hdc, m_hbitYUV_Block[id], 0, 0, false);
		//STGetSGAStatistics(m_hdc);
		STFinish(m_hdc);
		STFlip(m_hdc);

		id++;
		if(id>= m_dwBackBuffers)
			id = 0;
	}

	STDestroyVideoBuffers(m_hdc);
	STReleaseDC(m_hdc);
	return S_OK;
	
}

/************				NEW CODE	END	*******************/
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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ST_DEMO));

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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ST_DEMO));
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
    LoadString(hInstance, IDC_ST_DEMO, szWindowClass, MAX_LOADSTRING);


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


	MoveWindow(hWnd, 0 ,0, 800,480 ,TRUE);
	TestST(hWnd);
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
            
            // TODO: Add any drawing code here...
            
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
