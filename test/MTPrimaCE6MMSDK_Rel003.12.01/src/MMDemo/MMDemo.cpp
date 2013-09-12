/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/




// MMDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MMDemo.h"
#include <windowsx.h>
#include <commctrl.h>
#include <Commdlg.h>
#include "PlayThread.h"
#include "MediaFile.h"
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#ifndef DIM
#define DIM(_array) ((sizeof(_array)/sizeof(_array[0])))
#endif 

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle


struct DemoData
{
	bool bLoop;
	bool bUseSurfMgr;
	bool bDeinterlace;
	bool bFullScreen;
	bool bToOverlay;
	VR_REN_SYNC_MODE smSyncMode;
	bool bLogToFile;	
	WCHAR wcsFile[MAX_PATH];		//file to play
	bool bRender;
	VR_DISP_MODE_DESC displayMode;
	double fAlpha;
} g_demo;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	OverlayAlpha	(HWND, UINT, WPARAM, LPARAM);
void UpdateMenuCheck();


void GetConfigPath(WCHAR* pPath)
{
	GetModuleFileName(NULL, pPath, MAX_PATH );
	WCHAR* pSlash;
	if (pSlash = wcsrchr(pPath, L'\\'))
	{
		pSlash++;
		wcscpy(pSlash, L"demo.cfg");
	}
	else
	{
		wcscpy(pPath, L"\\demo.cfg");
		
	}


}
void LoadConfig()
{
	WCHAR path[MAX_PATH];
	GetConfigPath(path);
	
	FILE* fp;
	if (fp = _wfopen(path, L"rb"))
	{
		DemoData tmp;
		if (fread(&tmp, 1, sizeof(tmp), fp) == sizeof(tmp))
		{
			g_demo = tmp;
			g_demo.wcsFile[MAX_PATH-1] = L'\0';						//force null terminal
			fclose(fp);
			return;
		}
		fclose(fp);
	}
	memset(&g_demo, 0, sizeof(g_demo));
	g_demo.bUseSurfMgr = true;
	g_demo.bFullScreen = true;
	g_demo.bRender	   = true;
	g_demo.bToOverlay  = true;
	g_demo.displayMode.rmode = VR_REN_NORMAL;
	g_demo.smSyncMode = VR_ASYNC_WAITVSYNC;
	g_demo.fAlpha = 0.6;
	
}

void SaveConfig()
{
	WCHAR path[MAX_PATH];
	GetConfigPath(path);

	FILE* fp;
	if (fp = _wfopen(path, L"wb"))
	{
		fwrite(&g_demo, 1, sizeof(g_demo), fp);
		fclose(fp);
	}
	

}

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	

	LoadConfig();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MMDEMO);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	SaveConfig();

	return msg.wParam;
}
#define COLORKEY2COLORREF(ck)  RGB((ck&0xff0000)>>16, (ck&0xff00)>>8, ck&0xff)

HBRUSH GetColorKeyBrush()
{
	static HBRUSH theBrush;
	
	if (!theBrush)
	{
		LONG status;
		HKEY hRegKey;
		status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Drivers\\Display\\AT_LCD1"), 0, 0, &hRegKey);
		if (ERROR_SUCCESS == status)
		{
			DWORD dwUseDefaultTopLayerCK = 0, dwTopLayerCK = 0;
			DWORD dwSize =  sizeof(DWORD);
			RegQueryValueEx(hRegKey, TEXT("UseDefaultTopLayerColorKey"), NULL, NULL, (BYTE*)&dwUseDefaultTopLayerCK, &dwSize);
			RegQueryValueEx(hRegKey, TEXT("TopLayerColorKey"), NULL, NULL, (BYTE*)&dwTopLayerCK, &dwSize);
			if (dwUseDefaultTopLayerCK)
			{
				theBrush =  CreateSolidBrush(COLORKEY2COLORREF(dwTopLayerCK));		//let os delete it
			}
			RegCloseKey(hRegKey);
		}
	}

	if (!theBrush)
		theBrush = (HBRUSH) GetStockObject(WHITE_BRUSH);
	return theBrush;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    It is important to call this function so that the application 
//    will get 'well formed' small icons associated with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MMDEMO));
    wc.hCursor			= 0;
    //wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.hbrBackground    = GetColorKeyBrush(); 
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

	return RegisterClass(&wc);
}

//
//  FUNCTION: InitInstance(HANDLE, int)
//
//  PURPOSE: Saves instance handle and creates main window
//
//  COMMENTS:
//
//    In this function, we save the instance handle in a global variable and
//    create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND	hWnd;
	TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name

	hInst = hInstance;		// Store instance handle in our global variable
	// Initialize global strings
	LoadString(hInstance, IDC_MMDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE|WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

	return TRUE;
}


// Mesage handler for the About box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	
	switch (message)
	{
	case WM_INITDIALOG:
		// trying to center the About dialog
		if (GetWindowRect(hDlg, &rt1)) {
			GetClientRect(GetParent(hDlg), &rt);
			DlgWidth	= rt1.right - rt1.left;
			DlgHeight	= rt1.bottom - rt1.top ;
			NewPosX		= (rt.right - rt.left - DlgWidth)/2;
			NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
			
			// if the About box is larger than the physical screen 
			if (NewPosX < 0) NewPosX = 0;
			if (NewPosY < 0) NewPosY = 0;
			SetWindowPos(hDlg, 0, NewPosX, NewPosY,
				0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		return TRUE;
		
	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
    return FALSE;
}



struct OverlayAlphaValue
{
	double  fAlpha;
} m_OverlayAlpha;


#define ALPHA_MIN 0
#define ALPHA_MAX 100

#define ALPHA2INT(a) ((int)(a*ALPHA_MAX))
#define INT2ALPHA(i)  (((float)i)/ALPHA_MAX)

void SetAlphaValue(HWND hwnd, double a)
{
	HWND hwBuddy;
	hwBuddy = GetDlgItem(hwnd, IDC_ALPHA_VALUE);

	WCHAR wszAlpha[16];
	wsprintf(wszAlpha, L"%.2f", a);
	Static_SetText(hwBuddy, wszAlpha) ;


}

void OverlayAlpha_OnHScroll(HWND hwnd, HWND hwndTrack, UINT code, int pos)
{
	DWORD dwAlpha;
	dwAlpha = SendMessage(hwndTrack, TBM_GETPOS, 0, 0); 
	if (dwAlpha != ALPHA2INT(m_OverlayAlpha.fAlpha))
	{
		m_OverlayAlpha.fAlpha =  INT2ALPHA(dwAlpha);
		SetAlphaValue(hwnd, m_OverlayAlpha.fAlpha);
		
	}

	
}


// Mesage handler for the About box.
LRESULT CALLBACK OverlayAlpha(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	HWND hwndTrack;

	switch (message)
	{
	case WM_INITDIALOG:
		// trying to center the About dialog
		if (GetWindowRect(hDlg, &rt1)) {
			GetClientRect(GetParent(hDlg), &rt);
			DlgWidth	= rt1.right - rt1.left;
			DlgHeight	= rt1.bottom - rt1.top ;
			NewPosX		= (rt.right - rt.left - DlgWidth)/2;
			NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;

			// if the About box is larger than the physical screen 
			if (NewPosX < 0) NewPosX = 0;
			if (NewPosY < 0) NewPosY = 0;

			SetWindowPos(hDlg, 0, NewPosX, NewPosY,
				0, 0, SWP_NOZORDER | SWP_NOSIZE);


			hwndTrack = GetDlgItem(hDlg, IDC_ALPHA_SLIDER);
			

			SendMessage(hwndTrack, TBM_SETRANGE, 
				(WPARAM) TRUE,                   // redraw flag 
				(LPARAM) MAKELONG(ALPHA_MIN, ALPHA_MAX));  // min. & max. positions 
			SendMessage(hwndTrack, TBM_SETPAGESIZE, 
				0, (LPARAM) 4);                  // new page size 

			SendMessage(hwndTrack, TBM_SETPOS, 
				(WPARAM) TRUE,                   // redraw flag 
				(LPARAM)ALPHA2INT(m_OverlayAlpha.fAlpha)); 
			SetFocus(hwndTrack); 

			SetAlphaValue(hDlg, m_OverlayAlpha.fAlpha);


		}
		return TRUE;

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	HANDLE_MSG (hDlg, WM_HSCROLL, OverlayAlpha_OnHScroll);
	}
	return FALSE;
}

PlayThread* g_pPlayer = 0; 

void Demo_OnFileOpen(HWND hWnd)
{
	OPENFILENAME of;
	const LPTSTR pszOpenFilter = L"Supported Files, (*.rcv, *.mp4, *.avi, *.m2v, *.m4v, *.264, *.yuv)\0*.rcv;*.mp4;*.avi;*.m2v;*.m4v;*.264;*.yuv\0 m2v Files (*.m2v)\0*.m2v\0 m4v Files (*.m4v)\0*.m4v\0 h264 Files (*.264)\0*.264\0 YUV Files (*.yuv)\0*.yuv\0 mp4 Files (*.mp4)\0*.mp4\0 rcv Files (*.rcv)\0*.rcv\0\0";
		//L"Avi or Raw file(*.avi,*.m2v,*.m4v,*.264)|*.avi;*.m2v;*.m4v;*.264";
	memset(&of, 0, sizeof(of));
	of.lStructSize = sizeof(of);
	of.hwndOwner = hWnd;
	if (g_demo.wcsFile[0] == '\0')
		wcscpy(g_demo.wcsFile, L"\\residentflash\\03.avi");
	of.lpstrFile = g_demo.wcsFile;
	of.nMaxFile = DIM(g_demo.wcsFile);

	WCHAR wcsInitDir[DIM(g_demo.wcsFile)];
	memcpy(wcsInitDir, g_demo.wcsFile, DIM(g_demo.wcsFile));
	WCHAR* pSlash;
	if (pSlash = wcsrchr(wcsInitDir, L'\\'))
	{
		if (wcsInitDir == pSlash)
		{
			//current dir is root
			pSlash++;
		}
		*pSlash = '\0';
		of.lpstrInitialDir = wcsInitDir;
	}
	
	of.lpstrFilter = pszOpenFilter;
	of.Flags = 0;
	if (GetOpenFileName(&of))
	{
		PostMessage(hWnd, WM_COMMAND, MAKEWPARAM((UINT)(IDM_DEMO_PLAY),(UINT)(0)), 0);
	}
	
}


#define VIDEOFILE_CREATE(type) VIDEOFILE_CREATE_##type
#define IMP_VIDEOFILE_CREATE(type) inline VideoFile* VIDEOFILE_CREATE(type)() {return new type;}
IMP_VIDEOFILE_CREATE(VideoFileAvi)
IMP_VIDEOFILE_CREATE(VideoFileMpeg2)
IMP_VIDEOFILE_CREATE(VideoFileMpeg4)
IMP_VIDEOFILE_CREATE(VideoFileH264)
IMP_VIDEOFILE_CREATE(VideoFileYuv)
IMP_VIDEOFILE_CREATE(VideoFileMp4)
IMP_VIDEOFILE_CREATE(VideoFileRcv)

struct MediaFormatEntry
{
	WCHAR* Ext;
	VideoFile* (*pfnCreator)();
};

const static MediaFormatEntry MediaFormatList[] =
{
	L".avi", VIDEOFILE_CREATE(VideoFileAvi),
		L".m2v", VIDEOFILE_CREATE(VideoFileMpeg2),
		L".m4v", VIDEOFILE_CREATE(VideoFileMpeg4),
		L".264", VIDEOFILE_CREATE(VideoFileH264),
		L".yuv",VIDEOFILE_CREATE(VideoFileYuv),
		L".mp4", VIDEOFILE_CREATE(VideoFileMp4),
		L".rcv", VIDEOFILE_CREATE(VideoFileRcv),
};







VideoFile* OpenFile(WCHAR* wcsFileName)
{
	WCHAR* pDot;
	WCHAR  wszExt[MAX_PATH];
	if (pDot = wcsrchr(wcsFileName, L'.'))
	{
		wcscpy(wszExt, pDot);
		_wcslwr(wszExt);

		for (int i =0; i < DIM(MediaFormatList); i++)
		{
			if (wcscmp(wszExt, MediaFormatList[i].Ext) == 0)
			{
				VideoFile* p;
				if (p= MediaFormatList[i].pfnCreator())
				{
					if (p->Open(wcsFileName))
						return p;
					delete p;
				}
				
			}
		}

	}
	return NULL;
	

}

#define RANGE(min_, max_, a) (assert(min_ <= max_), ((min_<= a) &&(max_>=a)))

bool Demo_OnModeRange(int id)
{
	if (RANGE(IDM_MODE_DISPLAY_NORMAL, IDM_MODE_DISPLAY_JUST, id))
	{
		g_demo.displayMode.dmode = (VR_DISP_MODE)(id - IDM_MODE_DISPLAY_NORMAL);
	}
	else if (RANGE(IDM_MODE_SCREEN_NORMAL, IDM_MODE_SCREEN_FLIP, id))
	{
		g_demo.displayMode.smode = (VR_SCR_MODE)(id - IDM_MODE_SCREEN_NORMAL);

	}
	else if (RANGE(IDM_MODE_RENDER_FULL, IDM_MODE_RENDER_NORMAL, id))
	{
		g_demo.displayMode.rmode = (VR_RENDER_MODE)(id - IDM_MODE_RENDER_FULL);
	}
	else
	{
		return false;
	}
	UpdateMenuCheck();
	return true;
}
//
//  Process WM_COMMAND message for window/dialog: Demo
//
void Demo_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
				case IDM_HELP_ABOUT:
					DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					break;
				case IDM_OVERLAY_ALPHA:
					m_OverlayAlpha.fAlpha = g_demo.fAlpha;
					if (DialogBox(hInst, (LPCTSTR)IDD_OVERLAY_ALPHA, hWnd, (DLGPROC)OverlayAlpha) == IDOK)
						g_demo.fAlpha = m_OverlayAlpha.fAlpha;
					break;
				case IDM_FILE_OPEN:
					Demo_OnFileOpen(hWnd);
					break;
				case IDM_FILE_EXIT:
					DestroyWindow(hWnd);
					break;
				case IDM_DEMO_PLAY:
					if (!g_pPlayer)
					{
						SaveConfig();
						g_pPlayer = new PlayThread();
						ASSERT(g_pPlayer);
						g_pPlayer->m_dwSreenWidth = GetSystemMetrics(SM_CXSCREEN);
						g_pPlayer->m_dwSreenHeight = GetSystemMetrics(SM_CYSCREEN);
						g_pPlayer->m_bLoop = g_demo.bLoop;
						g_pPlayer->m_bRender = g_demo.bRender;
						g_pPlayer->m_dwSurfaceMgrMode = g_demo.bUseSurfMgr?SURFACE_MGR_RENDER:SURFACE_MGR_DRIVER;
						g_pPlayer->m_bDeinterlace = g_demo.bDeinterlace;
						g_pPlayer->m_bFullScreen = g_demo.bFullScreen;
						g_pPlayer->m_rlLayer     = g_demo.bToOverlay?VR_LAYER_OVERLAY:VR_LAYER_PRIMARY;
						g_pPlayer->m_fAlpha		 = g_demo.fAlpha;
						g_pPlayer->m_smSyncMode = g_demo.smSyncMode;
						g_pPlayer->m_displayMode = g_demo.displayMode;
						//g_pPlayer->m_hWnd = hWnd;
						
						if (g_pPlayer->m_pFile = OpenFile(g_demo.wcsFile))
						{
							WCHAR tmp[DIM(g_demo.wcsFile)];
							memcpy(tmp, g_demo.wcsFile, sizeof(g_demo.wcsFile));
							WCHAR* pDot;
							if (pDot = wcsrchr(tmp, L'.'))
							{
								pDot++;
								*pDot++ = 'l';
								*pDot++ = 'o';
								*pDot++ = 'g';
								*pDot++ = '\0';
							}
							g_pPlayer->m_pPerMon = new PerMon(g_demo.bLogToFile?tmp:NULL);
							g_pPlayer->FirePlay();
						}
						else
						{
							g_pPlayer->ReportErr(L"Can't Open MediaFile");
						}
					}
					break;
				case IDM_LOOP:
					g_demo.bLoop = !g_demo.bLoop;
					UpdateMenuCheck();
					break;
				case IDM_RENDER:
					g_demo.bRender  = !g_demo.bRender;
					if (!g_demo.bRender)
						g_demo.bUseSurfMgr = false;
					UpdateMenuCheck();
					break;

				case IDM_USE_SURF_MGR:
					g_demo.bUseSurfMgr = !g_demo.bUseSurfMgr;
					UpdateMenuCheck();
					break;
				case IDM_DEINTERLACE:
					g_demo.bDeinterlace = !g_demo.bDeinterlace;
					UpdateMenuCheck();
					break;
				case IDM_FULL_SCREEN:
					g_demo.bFullScreen = !g_demo.bFullScreen;
					UpdateMenuCheck();
					break;
				case IDM_OVERLAY:
					g_demo.bToOverlay = !g_demo.bToOverlay;
					UpdateMenuCheck();
					break;
				case IDM_VR_ASYNC_WAITVSYNC:
				case IDM_VR_SYNC_WAITVSYNC:
				case IDM_VR_NOWAIT_VSYNC:
					g_demo.smSyncMode =(VR_REN_SYNC_MODE)(id - IDM_VR_ASYNC_WAITVSYNC);
					UpdateMenuCheck();
					break;

				case IDM_LOG_TO_FILE:
					g_demo.bLogToFile = !g_demo.bLogToFile;
					UpdateMenuCheck();
				
					break;
				default:
					if (RANGE(IDM_MODE_DISPLAY_NORMAL, IDM_MODE_RENDER_NORMAL, id)&& Demo_OnModeRange(id))
					{
						/* nothing */
					}
					else
					{
						FORWARD_WM_COMMAND(hWnd,id,hwndCtl,codeNotify, DefWindowProc);
					}
	}
}


void CheckSubMenuItem(HMENU pSubMenu, int min_, int max_, int checked)
{
	checked += min_;
	assert(min_ <= max_);
	assert(checked <= max_);
	for (int i = min_; i <= max_; i++)
	{
		::CheckMenuItem(pSubMenu, i, (i == checked) ? MF_CHECKED : MF_UNCHECKED);
	}
}



void ExclusiveCheck(HMENU pMenu, UINT* ids, int count, UINT id_tocheck)
{
	for (int i =0; i < count; i++)
	{
		::CheckMenuItem(pMenu, ids[i], id_tocheck==ids[i]?MF_CHECKED : MF_UNCHECKED);
	}
}

void EnableItems(HMENU pMenu, UINT* ids, int count, bool enable)
{
	for (int i =0; i < count; i++)
	{
		::EnableMenuItem(pMenu, ids[i], enable?MF_ENABLED:MF_GRAYED);
	}
}

void UpdateMenuCheck()
{
	HMENU pMainMenu = CommandBar_GetMenu(hwndCB,0);  		
	HMENU pSubMenu = GetSubMenu(pMainMenu, 2);
	
	::CheckMenuItem(pSubMenu,IDM_LOOP,g_demo.bLoop ? MF_CHECKED : MF_UNCHECKED);

	::CheckMenuItem(pSubMenu,IDM_RENDER,g_demo.bRender ? MF_CHECKED : MF_UNCHECKED);
	::CheckMenuItem(pSubMenu,IDM_FULL_SCREEN,g_demo.bFullScreen ? MF_CHECKED : MF_UNCHECKED);
	::CheckMenuItem(pSubMenu,IDM_DEINTERLACE,g_demo.bDeinterlace ? MF_CHECKED : MF_UNCHECKED);
	::CheckMenuItem(pSubMenu,IDM_USE_SURF_MGR,g_demo.bUseSurfMgr ? MF_CHECKED : MF_UNCHECKED);
	::CheckMenuItem(pSubMenu,IDM_OVERLAY,g_demo.bToOverlay ? MF_CHECKED : MF_UNCHECKED);



	UINT items1[] = {IDM_FULL_SCREEN, IDM_DEINTERLACE, IDM_USE_SURF_MGR, IDM_OVERLAY, IDM_OVERLAY_ALPHA};
	EnableItems(pSubMenu, items1, DIM(items1), g_demo.bRender);

	pSubMenu = GetSubMenu(pMainMenu, 3);
	::CheckMenuItem(pSubMenu,IDM_LOG_TO_FILE,g_demo.bLogToFile ? MF_CHECKED : MF_UNCHECKED);

	pSubMenu = GetSubMenu(pMainMenu, 4);

	CheckSubMenuItem(pSubMenu, IDM_MODE_DISPLAY_NORMAL, IDM_MODE_DISPLAY_JUST, (int)g_demo.displayMode.dmode);
	CheckSubMenuItem(pSubMenu, IDM_MODE_SCREEN_NORMAL, IDM_MODE_SCREEN_FLIP, (int)g_demo.displayMode.smode);
	CheckSubMenuItem(pSubMenu, IDM_MODE_RENDER_FULL, IDM_MODE_RENDER_NORMAL, (int)g_demo.displayMode.rmode);


	UINT mode_items[] = { IDM_MODE_DISPLAY_NORMAL,IDM_MODE_DISPLAY_FULL,IDM_MODE_DISPLAY_JUST,
					    IDM_MODE_SCREEN_NORMAL,IDM_MODE_SCREEN_CK90, IDM_MODE_SCREEN_CCK90, IDM_MODE_SCREEN_MIRROR,IDM_MODE_SCREEN_FLIP,
						IDM_MODE_RENDER_FULL, IDM_MODE_RENDER_NORMAL};
	EnableItems(pSubMenu, mode_items, DIM(mode_items), g_demo.bRender);
	

	pSubMenu = GetSubMenu(pMainMenu, 5);
	UINT wait_items[] = {IDM_VR_ASYNC_WAITVSYNC,	IDM_VR_SYNC_WAITVSYNC,	IDM_VR_NOWAIT_VSYNC};
	ExclusiveCheck(pSubMenu, wait_items, DIM(wait_items), g_demo.smSyncMode+IDM_VR_ASYNC_WAITVSYNC);
	EnableItems(pSubMenu, wait_items, DIM(wait_items), g_demo.bRender);

	



}

//
//  Process WM_CREATE message for window/dialog: Demo
//
BOOL Demo_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	hwndCB = CommandBar_Create(hInst, hWnd, 1);			
	CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
	CommandBar_AddAdornments(hwndCB, 0, 0);
	UpdateMenuCheck();
	return 1L;
}

//
//  Process WM_DESTROY message for window/dialog: Demo
//
void Demo_OnDestroy(HWND hWnd)
{
	CommandBar_Destroy(hwndCB);
	PostQuitMessage(0);
}


#define HANDLE_WM_USER_PLAY_ERROR(hwnd, wParam, lParam, fn) \
((fn)(hwnd), 0L)

#define HANDLE_WM_USER_PLAY_FINISHED(hwnd, wParam, lParam, fn) \
    ((fn)(hwnd), 0L)


void Demo_OnPlayError(HWND hWnd)
{
	if (g_pPlayer)
	{
		WCHAR msg[MAX_PATH];
		wsprintf(msg, L"%s", g_pPlayer->GetErrMsg());
		SendMessage(hWnd, WM_USER_PLAY_FINISHED, 0, 0);
		MessageBox(hWnd,msg, L"Error", MB_ICONSTOP|MB_OK );


	}
	
}


LRESULT CALLBACK DummyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY:
		break;
	default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}


VOID ForceDesktopRedraw()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc;
	HWND  hWnd;
	
	/* Set up window Class */
	wc.style   = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc  = (WNDPROC) DummyWndProc;
	wc.cbClsExtra  = 0;
	wc.cbWndExtra  = 0;
	wc.hInstance  = hInstance;
	wc.hIcon   = 0;
	wc.hCursor   = 0;
	wc.lpszMenuName  = 0;
	// wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.hbrBackground = NULL;
	wc.lpszClassName = TEXT("DummyWindow");
	
	/* Now Register the class */
	RegisterClass(&wc);
	
	
	
	/* Create the fullscreen dummy window */
	hWnd = CreateWindowEx(WS_EX_TOPMOST|WS_EX_NOANIMATION,
        TEXT("DummyWindow"),
        NULL, 
        WS_VISIBLE,
        0, 
        0,  
        GetSystemMetrics(SM_CXSCREEN), 
        GetSystemMetrics(SM_CYSCREEN), 
        NULL, 
        NULL, 
        hInstance, 
        NULL);
	
	
	/* Now display then destoy the window */
	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
	DestroyWindow(hWnd);
}

void Demo_OnPlayFinished(HWND hWnd)
{
		if (g_pPlayer)
		{
			delete g_pPlayer;
			g_pPlayer = NULL;
		//	ForceDesktopRedraw();

		}
}

//
//  Process WM_LBUTTONDBLCLK message for window/dialog: Demo
//
void Demo_OnLButtonDblClk(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (g_pPlayer)
	{
		g_pPlayer->UnPause();
		g_pPlayer->Kill();
		Demo_OnPlayFinished(hwnd);

	}
}

//
//  Process WM_LBUTTONDOWN message for window/dialog: Demo
//
void Demo_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (g_pPlayer)
	{
		if (g_pPlayer->IsPaused())
			g_pPlayer->UnPause();
		else
			g_pPlayer->Pause();

	}
}


char g_VersionInfo[32*1024];

void GetVesionString(const wchar_t* name, wchar_t* version)
{
	DWORD size,junk;
	wchar_t SubBlock[MAX_PATH];
	wchar_t LanguageID[] = L"000004b0";
	void*   pVersion;
	UINT  sizVersion;

	wsprintf(SubBlock, TEXT("\\StringFileInfo\\%s\\InternalName"),LanguageID);
	wchar_t* szDllName = (wchar_t*)name;
	
	size = GetFileVersionInfoSize((wchar_t*)name, &junk);
	if (size && GetFileVersionInfo(szDllName, 0, sizeof(g_VersionInfo), g_VersionInfo) && VerQueryValue(g_VersionInfo, SubBlock, &pVersion, &sizVersion))
	{
		memcpy(version, pVersion, sizVersion*sizeof(wchar_t));
	}
	else
	{
		swprintf(version, L"%s", L"No Version Info");
	}
}


void GetVersions(const wchar_t** names, int size, vector<wstring>& versions)
{
	versions.clear();
	wchar_t ver[MAX_PATH*2];
	for (int i = 0; i < size; i++)
	{
		GetVesionString(names[i], ver);
		ver[MAX_PATH] = L'\0';
		wstring verstr = ver;
		versions.push_back(verstr);
	}

}


BOOL Demo_OnPaint(HWND hwnd)
{
	
	const wchar_t* binarys[] = 
	{
		L"BHVidRenderer.dll",
		L"BHSurfMgr.dll",
		L"mpeg2decdll.dll",
		L"h264decdll.dll",
		L"mpeg4decdll.dll",
		L"wmvdecdll.dll",
	};
	static vector<wstring> versions;
	if (!versions.size())
	{
		GetVersions(binarys, DIM(binarys), versions);
	}
	HDC hdc;
	PAINTSTRUCT ps;
	wchar_t tmp[MAX_PATH*2];
	
	
	TEXTMETRIC tm;
	RECT rect;
	GetClientRect(hwnd, &rect);
	int    y = rect.top + 30;
	

	hdc = BeginPaint(hwnd, &ps);

	int   fontheight = 0;
	GetTextMetrics(hdc, &tm);
	fontheight = tm.tmHeight + tm.tmExternalLeading;
	
	wchar_t mmdemo[] = L"MMDemo.exe";
	ExtTextOut(hdc, 5, y, 0, NULL, mmdemo,  wcslen(mmdemo), NULL);
	ExtTextOut(hdc, 120, y, 0, NULL, MMDEMO_VERSION_STRING,  wcslen(MMDEMO_VERSION_STRING), NULL);
	y += fontheight;

	
	for (size_t i = 0; i < versions.size(); i++)
	{
	//	RETAILMSG (1, (L"%s", tmp));
	//	swprintf(tmp, L"%-25s %s", binarys[i], versions[i].c_str());
	//	ExtTextOut(hdc, 5, y, 0, NULL, tmp,  wcslen(tmp), NULL);
		ExtTextOut(hdc, 5, y, 0, NULL, binarys[i],  wcslen(binarys[i]), NULL);
		ExtTextOut(hdc, 120, y, 0, NULL, versions[i].c_str(),  wcslen(versions[i].c_str()), NULL);
//		GetTextExtentPoint32(hdc, tmp, wcslen(tmp), &s);
		y += fontheight;
	}

	EndPaint(hwnd, &ps);

	return 1;




}





//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
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

	switch (message)
	{
		HANDLE_MSG (hWnd, WM_COMMAND, Demo_OnCommand);
		HANDLE_MSG (hWnd, WM_CREATE, Demo_OnCreate);
		HANDLE_MSG (hWnd, WM_DESTROY, Demo_OnDestroy);
		HANDLE_MSG(hWnd,  WM_USER_PLAY_ERROR,  Demo_OnPlayError);
		HANDLE_MSG(hWnd,  WM_USER_PLAY_FINISHED,Demo_OnPlayFinished);
		HANDLE_MSG (hWnd, WM_LBUTTONDBLCLK, Demo_OnLButtonDblClk);
		HANDLE_MSG (hWnd, WM_LBUTTONDOWN, Demo_OnLButtonDown);
		HANDLE_MSG(hWnd,  WM_PAINT, Demo_OnPaint);


		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	
	}
   return 0;
}

