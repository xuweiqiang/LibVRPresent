//===================================== ========================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------


#include "../../inc/GfxBuildDef.h"

#ifdef VM_OPENGL

#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "tchar.h"
#include "GfxPresentProcOpenGL.h"
#include "GfxPresentStreamOpenGL.h"

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter OpenGL")
#define MAX_STREAM 16

typedef struct
{
	bool valid;
	VP_OPEN_STREAM_PARAMS param;
	IMcVideoPresenterStream *pStream;
}SOGL_STREAM_INFO;


static void VR_OGL_ChangeSize(int w, int h);
static void VR_OGL_RenderScene(void);
static void VR_OGL_Idle(void);
static void VR_OGL_Motion(int x, int y);
static void VR_OGL_processMouse(int button, int state, int x, int y) ;

SOGL_STREAM_INFO 			g_pStmArray[MAX_STREAM] = {0};
static HANDLE 				g_event = 0;
static VP_CANVASINFO_PARAMS 	g_canvasInfo = {0};
static int 					g_mainVer = 0;
static int 					g_subVer = 0;
static int 					g_releaseVer =0;
static HWND 					g_hWnd = 0;
static CRITICAL_SECTION 		g_csOGL = {0};
static bool 					g_bCanvasChanged = false;

static DWORD WINAPI VR_OGL_ThreadProc( LPVOID dParam )
{
	int argc = 0;
	CVRPresentProcOpenGL* pThis = (CVRPresentProcOpenGL*)dParam;

	CProtect proc(&g_csOGL);
	glutInit(&argc, 0);

	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE );		// Display Mode
	glutInitWindowPosition(0,0);
	glutInitWindowSize(g_canvasInfo.dwWidth,g_canvasInfo.dwHeight);

	glutSetParentWindow(g_hWnd);
	glutCreateWindow("VideoPresenter_OGL");	

	//Print out version info
	char* sInfo= (char*)glGetString(GL_VENDOR); 
	DP("Vendor:%s\n", sInfo);
	sInfo= (char*)glGetString(GL_RENDERER); 
	DP("Render: %s\n", sInfo);
	sInfo = (char*)glGetString(GL_VERSION);
	DP("OpenGL Ver:%s\n", sInfo);
	sscanf(sInfo, "%d.%d.%d", &g_mainVer, &g_subVer, &g_releaseVer);


	GLeeInit();
	
	glShadeModel(GL_SMOOTH);	
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);					
	glEnable(GL_DEPTH_TEST);			
	glDepthFunc(GL_LEQUAL);				

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//glEnable(GL_CULL_FACE);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glEnable(GL_TEXTURE_2D);


	g_bCanvasChanged = false;

	glutReshapeFunc(VR_OGL_ChangeSize);	
	glutDisplayFunc(VR_OGL_RenderScene);
	//glutPassiveMotionFunc(VR_OGL_Motion);
	//glutMotionFunc(VR_OGL_Motion);
	glutMouseFunc(VR_OGL_processMouse);
	glutIdleFunc	(VR_OGL_Idle );
	
	glutMainLoop();	
	return 0;
};


static void VR_OGL_Idle(void)
{
	if(g_event && (WaitForSingleObject(g_event,0) == WAIT_OBJECT_0))
	{
/*		
		static DWORD fmtCnt = 0;
		fmtCnt ++;
		static DWORD SetTick = GetTickCount();
		if(GetTickCount() -SetTick > 1000 )
		{
			char buf[256];
			sprintf(buf,"idle:%f \n",(float)fmtCnt/ ((GetTickCount() -SetTick)/1000));
#ifndef _WIN32_WCE
			OutputDebugStringA(buf);
#endif
			SetTick = GetTickCount();
			fmtCnt = 0;
		}
*/

		glutPostRedisplay();
	}
	else
	{
		glutPostRedisplay();
		Sleep(40);
	}


	return;
}

static void VR_OGL_ChangeSize(int w, int h)
{

/*		if(gw == w && gh ==h)
			return ;
		gw = w;
		gh = h;
		*/
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);


	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	g_canvasInfo.dwWidth = w;
	g_canvasInfo.dwHeight = h;

	for(int i=0;i<MAX_STREAM;i++)
	{
		if(g_pStmArray[i].valid)
			g_pStmArray[i].pStream->Set(IID_IMcVideoPresenterAdv, MC_PROPID_VP_CANVAS_INFO, 0,0,&g_canvasInfo,0);
	}
	glOrtho(-0.5,0.5, -0.5, 0.5,  0.0,100.0);
//	glOrtho(0.0,1.0, 0.0, 1.0,  0.0,100.0);
//	glOrtho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0,5000.0); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	g_bCanvasChanged = true;
}
static void VR_OGL_Motion(int x,int y)
{
	if(g_event)
		SetEvent(g_event);
	return;
}

static void VR_OGL_RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i=0;i<MAX_STREAM;i++)
	{
		if(g_pStmArray[i].valid)
		{
			glPushMatrix();
			g_pStmArray[i].pStream->Present(0,0,OGL_FLIP_FLAG);
			glPopMatrix();
		}
	}

    	glutSwapBuffers();
}


void VR_OGL_processMouse(int button, int state, int x, int y) 
{
	VP_RECT rMouse = {x,y};
	for(int i=0;i<MAX_STREAM;i++)
	{
		if(g_pStmArray[i].valid)
		{
			g_pStmArray[i].pStream->Set(IID_IMcVideoPresenterAdv, MC_PROPID_VP_MOUSE_CLICK, 0,0,&rMouse,0);
		}
	}
	return;
}
	
CVRPresentProcOpenGL  ::CVRPresentProcOpenGL () :
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_Init(false),
	m_hThread(0),
	m_pVideoMixer(0)
{
	TEXMANAGER
	memset(&m_openParams, 0,sizeof(m_openParams));
}

CVRPresentProcOpenGL ::~CVRPresentProcOpenGL  ()
{
	Close();
	DESTROY_TEXMANAGER
}	


//-------------------------------------------------------------------
// InitializeWindow
//
// Creates the application window.
//-------------------------------------------------------------------

STDMETHODIMP CVRPresentProcOpenGL  ::Open(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize)
{
	HRESULT hr = S_OK;
	if(m_Init)
		return E_UNEXPECTED;
	if(!m_openParams.hWnd)
		return E_INVALIDARG;
	
	memcpy(&m_openParams, pOpenParams, dwOpenSize);

	g_hWnd = m_openParams.hWnd;
	RECT rWnd = {0,0,0,0}; 
	::GetClientRect((HWND)g_hWnd, &rWnd);
	g_canvasInfo.dwWidth = rWnd.right - rWnd.left;
	g_canvasInfo.dwHeight = rWnd.bottom - rWnd.top;


	if(!g_event)
	{
		g_event = CreateEvent(NULL, FALSE, TRUE, NULL); 
		if(!g_event)	return E_FAIL;
	}
	

	InitializeCriticalSection(&g_csOGL);
	m_hThread = ::CreateThread(NULL, 0, VR_OGL_ThreadProc, this, 0, NULL);
	if(!m_hThread)	return E_FAIL;

	m_pVideoMixer = 0;
	m_Init = true;
	return hr;
}

STDMETHODIMP CVRPresentProcOpenGL  ::Close()
{
	if(!m_Init)
		return E_UNEXPECTED;

	glutExit();

	if(m_hThread)
	{
		::TerminateThread(m_hThread,0);
		m_hThread = NULL;
	}


	 DeleteCriticalSection(&g_csOGL);

	if(g_event)
	{
		CloseHandle(g_event);
		g_event = 0;
	}
	
	for(int i=0;i<MAX_STREAM ;i++)
	{
		DestroyStream(g_pStmArray[i].pStream);
	}
	memset(&g_pStmArray[0],0,sizeof(g_pStmArray));

	m_Init = false;
	return S_OK;
}


STDMETHODIMP CVRPresentProcOpenGL  ::Render(DWORD dwRenderFlags)
{
	if(!m_Init)
		return E_FAIL;

	if(g_bCanvasChanged)
		return VR_FAIL_CANVASCHANGED;

	if(g_event)
		SetEvent(g_event);
	return S_OK;
}


STDMETHODIMP CVRPresentProcOpenGL  ::Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	switch (dwPropID)
	{
		case MC_PROPID_VP_RECTDST:
			return S_OK;
			
		case MC_PROPID_VP_CANVAS_INFO:
			{
				if(pPropData == NULL)	return E_INVALIDARG;
				if(!m_Init)	return VR_FAIL_UNOPEN;
				VP_CANVASINFO_PARAMS* pArgs = (VP_CANVASINFO_PARAMS*)pPropData;
				pArgs->dwWidth = g_canvasInfo.dwWidth;
				pArgs->dwHeight = g_canvasInfo.dwHeight;
				g_bCanvasChanged = false;
				return S_OK;
			}		
		default:
			break;
	}
	return E_PROP_ID_UNSUPPORTED;
}


STDMETHODIMP CVRPresentProcOpenGL  ::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	HRESULT hr = E_PROP_ID_UNSUPPORTED;
	if((!m_Init)||(!pPropData))
		return E_FAIL;

	
	switch (dwPropID)
	{
		case MC_PROPID_VP_RECTDST:
			return E_UNEXPECTED;

		case MC_PROPID_VP_VM_WND:
			m_pVideoMixer = (ICivmVideoMixer*)pPropData;
			break;

		default:
			break;
	}

	return hr;
}

STDMETHODIMP CVRPresentProcOpenGL  ::CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream)
{
	HRESULT hr = S_OK;
	int ret = true;
	if((!m_Init) || !ppStream)
		return E_FAIL;


	UINT i;
	for(i=0;i<MAX_STREAM ;i++)
	{
		if(!g_pStmArray[i].valid)	break;
	}

	if(i>= MAX_STREAM)	return E_FAIL;
	memcpy(&g_pStmArray[i].param, pOpenParams, sizeof(VP_OPEN_STREAM_PARAMS));

	g_pStmArray[i].valid = true;
	g_pStmArray[i].pStream = new CVRPresenterStreamOpenGL(pOpenParams); 
	if(!(g_pStmArray[i].pStream))	{ return E_FAIL; };

	hr = g_pStmArray[i].pStream->Set(IID_IMcVideoPresenterAdv, MC_PROPID_VP_CANVAS_INFO, 0,0,&g_canvasInfo,0);
	if(FAILED(hr))	return hr;

	float zOrder = float(i) -50.0f;
	hr = g_pStmArray[i].pStream->Set(IID_IMcVideoPresenterAdv, MC_PROPID_VP_ZORDER_ID, 0,0,&zOrder,0);
	if(FAILED(hr))	return hr;

	*ppStream  = g_pStmArray[i].pStream ;
	return hr;
}


STDMETHODIMP CVRPresentProcOpenGL  ::DestroyStream(IN IMcVideoPresenterStream *ppStream)
{
	if(!ppStream)
		return E_FAIL;

	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(g_pStmArray[i].valid)
		{
			if(g_pStmArray[i].pStream == ppStream)
			{
				ppStream->Release();
				memset(&g_pStmArray[i],0,sizeof(SOGL_STREAM_INFO));
				break;
			}
		}
	}
	
	return S_OK;
}


#endif
