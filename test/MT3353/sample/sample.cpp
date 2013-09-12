// sample.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "sample.h"
#include <windows.h>
#include <commctrl.h>
#include "HW_Vdec.h"
#include <stdio.h>
#include <string>
using namespace std;
#define MAX_LOADSTRING 100

#define MPEG4_W    1280
#define MPEG4_H    720
#define MPEG4_CODEC VCODEC_ID_MP4V
#define MPEG4_PATH L"File\\MPEG4\\"

#define MPEG2_W    1280
#define MPEG2_H    720
#define MPEG2_CODEC VCODEC_ID_MP2V
#define MPEG2_PATH L"File\\MPEG2\\"

#define RV40_W    1280
#define RV40_H    720
#define RV40_CODEC VCODEC_ID_RV40
#define RV40_PATH L"File\\RV40\\"

#define H264_W    720
#define H264_H    480
#define H264_CODEC VCODEC_ID_H264
#define H264_PATH L"File\\H264\\"

#define VC1_W    720
#define VC1_H    480
#define VC1_CODEC VCODEC_ID_VC1
#define VC1_PATH L"File\\VC1\\"

#define PATH MPEG4_PATH
#define WIDTH MPEG4_W
#define HEIGHT MPEG4_H
#define VCODEC MPEG4_CODEC

typedef enum SampleErrorCode
{
    ERR_OK,
    ERR_INITSURFACE,
    ERR_CREATEFAIL,
    ERR_READFILE,
    ERR_DECODE,
    ERR_GETOUTPUTINFO,
    ERR_UPDATEPIC,
    ERR_DRAW,
    MAX_OF_SAMPLE_ERRORCODE
} E_SAMPLE_ERRORCODE_T;

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

#include <ddraw.h>
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
// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

GBOOL g_fgDraw=FALSE;
GVOID* g_preOutbuff=NULL;
GBOOL  g_preFlag = FALSE;
HWND   g_hMainWnd = NULL;




static E_SAMPLE_ERRORCODE_T onDecode(HVDECINST hVdecInst,GTCHAR *szPath,VDECINFO_INPUT_T *prVdecInfo);
static HRESULT initSurface(UINT32 u4Width,UINT32 u4Height);
static HRESULT Draw(VOID *prInbuf,UINT32 u4Size,UINT32 u4Width,UINT32 u4Height);
static GVOID deinitSurface();
static HRESULT updataPic(VOID *prInbuf,UINT32 u4Size);
static void test(GTCHAR *testPath,VCODECID_T eCodecId,UINT32 u4Width,UINT32 u4Height);
string WszStrToStr(LPCWSTR wstr)
{
    string str;
    str.resize(256);

    // Convert filename from UNICODE to ANSI
    //
    if (0 == WideCharToMultiByte(CP_ACP, 0, wstr, -1,
                                 &str[0], str.size(), NULL, NULL))
    {
        ASSERT(0);
    }

    return str;
}

string WstrToStr(const wstring& wstr)
{
    return WszStrToStr(wstr.c_str());
}
wstring GetWorkDirectoryWide()
{
    wstring fullpath;
    fullpath.resize(256);
    
    GetModuleFileName(NULL, &fullpath[0], fullpath.size());

    wstring::size_type pos = fullpath.find_last_of(L"\\");

    return fullpath.substr(0, pos + 1);
}

string GetWorkDirectoryChar()
{
    return WstrToStr(GetWorkDirectoryWide());
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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAMPLE));

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

static void setPictype(VDECINFO_INPUT_T *prVdecInfo,VCODECID_T eCodecId,GUINT32  index)
{
	if(eCodecId == VCODEC_ID_MP2V)
	{
		switch(index)
		{
		 case 0:
			 prVdecInfo->u4PicType = I_TYPE;
			 break;
		 case 1:
			 prVdecInfo->u4PicType = P_TYPE;
			 break;
		 case 2:
			 prVdecInfo->u4PicType = B_TYPE;
			 break;
		 case 3:
			 prVdecInfo->u4PicType = B_TYPE;
			 break;
		 case 4:
			 prVdecInfo->u4PicType = P_TYPE;
			 break;
		 default:
			 break;
		}
	}
	if(eCodecId == VCODEC_ID_H264)
	{
		switch(index)
		{
		 case 0:
			 prVdecInfo->u4PicType = SEQ_PS | PIC_PS | IDR_PIC | I_ALL_SLICE | 0x00000c00;
			 break;
		 case 1:
			 prVdecInfo->u4PicType = REF_PIC | P_ALL_SLICE | 0x00000c00;
			 break;
		 case 2:
			 prVdecInfo->u4PicType = REF_PIC | P_ALL_SLICE | 0x00000c00;
			 break;
		 case 3:
			 prVdecInfo->u4PicType = REF_PIC | P_ALL_SLICE | 0x00000c00;
			 break;
		 case 4:
			 prVdecInfo->u4PicType = REF_PIC | P_ALL_SLICE | 0x00000c00;
		 case 5:
			 prVdecInfo->u4PicType = REF_PIC | P_ALL_SLICE | 0x00000c00;
			 break;
		 case 6:
			 prVdecInfo->u4PicType = REF_PIC | P_ALL_SLICE  | 0x00000c00 ;
			 break;
		 case 7:
			 prVdecInfo->u4PicType = REF_PIC | P_ALL_SLICE | 0x00000c00;
			 break;
		 default:
			 break;
		}
	}
}

static void test(GTCHAR *testPath,VCODECID_T eCodecId,UINT32 u4Width,UINT32 u4Height)
{
	HVDECINST hVdecInst = NULL;
	GTCHAR path[100]={0};
	int i=0;
    HRESULT hRet = E_FAIL;
	hVdecInst = Vdec_CreateInstance(eCodecId);
	hRet = initSurface(u4Width, u4Height);
    if (S_OK != hRet)
    {
        MessageBox(g_hMainWnd, L"Attention: InitSurface fail!", L"Error!", MB_OK);
        return;
    }

	VDECINFO_INPUT_T rVdecInfo;
	memset(&rVdecInfo,0,sizeof(VDECINFO_INPUT_T));
	rVdecInfo.u4Height = u4Height;
	rVdecInfo.u4Width  = u4Width;
	rVdecInfo.ucPicStruct = FRM_PIC;

	for(i=0;i<5;i++)
	{
		swprintf( path, L"%sAU[%d].raw", testPath,i); 
		memset(&rVdecInfo,0,sizeof(VDECINFO_INPUT_T));
		rVdecInfo.u4Height = u4Height;
		rVdecInfo.u4Width  = u4Width;
		rVdecInfo.ucPicStruct = FRM_PIC;
		setPictype(&rVdecInfo,eCodecId,i);
		E_SAMPLE_ERRORCODE_T eRet = onDecode(hVdecInst,path,&rVdecInfo);
        switch(eRet)
        {
        case ERR_CREATEFAIL:
            MessageBox(g_hMainWnd, L"Attention: onDecode fail for CreateFile!", L"Error!", MB_OK);
            break;
        case ERR_READFILE:
            MessageBox(g_hMainWnd, L"Attention: onDecode fail for ReadFile!", L"Error!", MB_OK);
            break;
        case ERR_DECODE:
            MessageBox(g_hMainWnd, L"Attention: onDecode fail for Vdec_SetInputData!", L"Error!", MB_OK);
            break;
        case ERR_GETOUTPUTINFO:
            MessageBox(g_hMainWnd, L"Attention: onDecode fail for Vdec_GetOutputData!", L"Error!", MB_OK);
            break;
        case ERR_DRAW:
            MessageBox(g_hMainWnd, L"Attention: onDecode fail for Draw!", L"Error!", MB_OK);
            break;
        case ERR_UPDATEPIC:
            MessageBox(g_hMainWnd, L"Attention: onDecode fail for UpdatePic!", L"Error!", MB_OK);
            break;
        case 0:
            break;
        }
        if (ERR_OK != eRet)
            break;
		Sleep(1000);
	}
    
	g_fgDraw = false;
	if(NULL != g_preOutbuff)
	{
		
		Vdec_FreeOutputBuf(hVdecInst,g_preOutbuff);
	}
	deinitSurface();
	Vdec_ReleaseInstance(hVdecInst);
}

static E_SAMPLE_ERRORCODE_T onDecode(HVDECINST hVdecInst,GTCHAR *szPath,VDECINFO_INPUT_T *prVdecInfo)
{
	BYTE *inputBuf=NULL;
	UINT32 u4Read=0;
	UINT32 u4ReadSize=0;
	UINT32 u4OutSize=0;
	BOOL fgRet=FALSE;
	void *pvOutputBuf=NULL;
	HANDLE hFile=NULL;
	VDECINFO_OUTPUT_T rVdecOutPutInfo;
	wchar_t resultFilename[256];
	 
	memset(&rVdecOutPutInfo,0,sizeof(VDECINFO_OUTPUT_T));

	memset(resultFilename, 0, sizeof(wchar_t) * 256);
    swprintf(resultFilename, L"%S\\%s", GetWorkDirectoryChar().c_str(), szPath);

	hFile = CreateFile(resultFilename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

	//hFile = CreateFile(szPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(hFile);
		return ERR_CREATEFAIL;
	}

	u4ReadSize = GetFileSize(hFile, NULL);
	inputBuf = (BYTE*)malloc(u4ReadSize*4);

	fgRet = ReadFile(hFile, (LPVOID)inputBuf, (DWORD)u4ReadSize, (LPDWORD)&u4Read, NULL);
	if (!fgRet || u4Read != u4ReadSize)
	{
		 CloseHandle(hFile);
		 return ERR_READFILE;
	}
	
    if(NULL != g_preOutbuff)
	{
		Vdec_FreeOutputBuf(hVdecInst,g_preOutbuff);
	}

    prVdecInfo->u4DataSize = u4ReadSize;
    prVdecInfo->pvInputBuff = inputBuf;
	if(!g_preFlag)
	{
	   if (!Vdec_SetInputData(hVdecInst,prVdecInfo))
        {
			 CloseHandle(hFile);
            return ERR_DECODE;
        }
    }
    if (!Vdec_GetOutputData(hVdecInst,&rVdecOutPutInfo))
    {
		CloseHandle(hFile);
        return ERR_GETOUTPUTINFO;
    }
	g_preOutbuff = rVdecOutPutInfo.pvOutputBuff;
	g_preFlag    = rVdecOutPutInfo.fgNoNeedData;

	if(!rVdecOutPutInfo.fgDisplay)
	{
	    CloseHandle(hFile);
		return ERR_OK;
	}

	if(!g_fgDraw )
	{
		  if (S_OK != Draw(rVdecOutPutInfo.pvOutputBuff,rVdecOutPutInfo.u4OutBufSize,prVdecInfo->u4Width,prVdecInfo->u4Height))
        {
        
			CloseHandle(hFile);
             return ERR_DRAW;
         }
		g_fgDraw =TRUE;
	}
	else 
	{
		 if (S_OK != updataPic(rVdecOutPutInfo.pvOutputBuff,rVdecOutPutInfo.u4OutBufSize))
         {
			CloseHandle(hFile);
            return ERR_UPDATEPIC;
         }
	}

	free(inputBuf);
    CloseHandle(hFile);

	return ERR_OK;
}



static DDPIXELFORMAT ddpfOverlayFormats[] = {
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('M','T','K','1'), 0, 0, 0, 0, 0}
};

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

static HRESULT initSurface(UINT32 u4Width,UINT32 u4Height)
{
	HRESULT hr = S_FALSE;
	DDSURFACEDESC ddsd;
	RECT rs;

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

	rs.left = 0;
	rs.top = 0;
	rs.right = 800;
	rs.bottom = 480;

	if (ddcaps.dwAlignSizeSrc != 0)
		rs.right += rs.right % ddcaps.dwAlignSizeSrc;

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

static GVOID deinitSurface()
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
		RECT rsRect = {0, 0, 0, 0};
		RECT dstRect = {0, 0, 800, 480};
		

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

		g_pDDSOverlay->Flip(NULL, 0);

		rsRect.left = 0;
		rsRect.top = 0;
		rsRect.right = u4Width;
		rsRect.bottom = u4Height;
		
		hr = g_pDDSOverlay->UpdateOverlay(&rsRect, g_pDDSPrimary, &dstRect/*&rd*/, dwUpdateFlags, NULL);
		if (hr != DD_OK)
		{
			return S_FALSE;
		}
		return S_OK;
}

static HRESULT updataPic(VOID *prInbuf,UINT32 u4Size)
{
	    HRESULT hr;
		DWORD dwUpdateFlags = DDOVER_SHOW;
		LPBYTE pSurfaceBuf = NULL;
		DDSURFACEDESC     ddsdSrc;

		memset(&ddsdSrc, 0, sizeof(ddsdSrc));
		ddsdSrc.dwSize = sizeof(ddsdSrc);

		hr = rDDSArray.lpSurf[0]->Lock(NULL, &ddsdSrc, DDLOCK_WAITNOTBUSY, NULL);
		if (hr != DD_OK)
		{
			return S_FALSE;
		}
		pSurfaceBuf = (LPBYTE)ddsdSrc.lpSurface;

		memcpy(pSurfaceBuf, prInbuf, u4Size);

		rDDSArray.lpSurf[0]->Unlock(NULL);
		pSurfaceBuf = NULL;

		g_pDDSOverlay->Flip(NULL, 0);

		return S_OK;
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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAMPLE));
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
    LoadString(hInstance, IDC_SAMPLE, szWindowClass, MAX_LOADSTRING);


    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

	hWnd= CreateDialog( g_hInst, (LPCTSTR)IDD_VDEC_SAMPLE ,NULL, (DLGPROC )WndProc); 

    if (!hWnd)
    {
        return FALSE;
    }


    g_hMainWnd = hWnd;
    
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
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
				case IDC_VC1:
					g_fgDraw=FALSE;
					test(VC1_PATH,VC1_CODEC,VC1_W,VC1_H);
					break;
				case IDC_MPEG4:
					g_fgDraw=FALSE;
					test(MPEG4_PATH,MPEG4_CODEC,MPEG4_W,MPEG4_H);
					break;
				case IDC_MPEG2:
					g_fgDraw=FALSE;
					test(MPEG2_PATH,MPEG2_CODEC,MPEG2_W,MPEG2_H);
					break;
				case IDC_RV40:
					g_fgDraw=FALSE;
					test(RV40_PATH,RV40_CODEC,RV40_W,RV40_H);
					break;
				case IDC_H264:
					g_fgDraw=FALSE;
					test(H264_PATH,H264_CODEC,H264_W,H264_H);
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
            g_hMainWnd = NULL;
            break;


        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
