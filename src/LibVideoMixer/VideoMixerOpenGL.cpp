//=============================================================================
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
// 	Copyright (c) 2010  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------
#ifdef VM_SUPPORT
#include <windows.h>
#include <tchar.h>
#include <../../inc/McCom.h>
#include "VideoMixerOpenGLGLee.h"
#include <../../3rdparty/freeglut_lib/GL/glut.h>
#include <../../3rdparty/freeglut_lib/GL/freeglut.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/DMCP.h"
#include "../../inc/dp.h"
#include "../../inc/GfxVideoWindow.h"
#include "VideoMixerOpenGL.h"


static CVideoMixerOpenGL* g_pVideoMixer = NULL;
static LARGE_INTEGER st,ed;
static LARGE_INTEGER coutFreq;

static int g_wndWidth= 0;
static int g_wndHeight= 0;

#include "math.h"

void glutDispMsg(char* pStr)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);
	glRasterPos2f(0.0f,0.0f);

	for(int i=0;pStr[i] != '\0';i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,pStr[i]);

	glEnable(GL_TEXTURE_2D);
	glPopAttrib();
	return;
}

void glutDisplay(void)   
{
	static DWORD frames = 0;
	DWORD elapsed = 0;
	static bool bInit = false;
	static LARGE_INTEGER tSetposStart,tSetposEnd;

	if(!g_pVideoMixer)	return;

	if(!bInit)
	{
		bInit  = true;
	 	QueryPerformanceFrequency(&coutFreq);
		QueryPerformanceCounter (&tSetposStart);
	}

	QueryPerformanceCounter (&tSetposEnd);
	elapsed= 1000*(tSetposEnd.QuadPart - tSetposStart.QuadPart)/coutFreq.QuadPart ;


	static char prtinfo[MAX_PATH] = {0};
	if(elapsed >2000)
	{
		sprintf(prtinfo, "fps:%d \n", frames*1000/(elapsed));
		DP(prtinfo);
		frames = 0;
		QueryPerformanceCounter (&tSetposStart);
	}
	
	frames++;

	QueryPerformanceCounter (&st);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	static GLfloat rotate = 0.0f;
	rotate += 0.03f;
	if(rotate > 360)
		rotate = 0;
	
	for(int i=0;i<4;i++)
	{
		switch(i)
		{
			case 0:
				glViewport( 0, g_wndHeight/2, g_wndWidth/2, g_wndHeight/2);
				glMatrixMode(GL_PROJECTION);	
				glLoadIdentity();					
				glOrtho(-0.5,0.5, -0.5, 0.5,  0.0,100.0);

				break;
			case 1:
				glViewport( g_wndWidth/2, g_wndHeight/2, g_wndWidth/2, g_wndHeight/2);
				glMatrixMode(GL_PROJECTION);	
				glLoadIdentity();					

				gluPerspective(20,(float)g_wndWidth/(float)g_wndWidth,0.0,10.0);
				gluLookAt(0.0f,0.0f,1.0f, 
					0.0f,0.0f,0.0f, 
					0.0f,1.0f,0.0f );
				break;
			case 2:
				glViewport( 0, 0, g_wndWidth/2, g_wndHeight/2);
				glMatrixMode(GL_PROJECTION);	
				glLoadIdentity();					
				gluPerspective(20,(float)g_wndWidth/(float)g_wndWidth,0.0,10.0);
				gluLookAt(0.0f + 10*cos(rotate),0.0f,10*sin(rotate), 
					0.0f,0.0f,0.0f, 
					0.0f,1.0f,0.0f );

				break;
			case 3:
				glViewport( g_wndWidth/2, 0, g_wndWidth/2, g_wndHeight/2);
				glMatrixMode(GL_PROJECTION);	
				glLoadIdentity();					
				gluPerspective(20,(float)g_wndWidth/(float)g_wndWidth,0.0,10.0);
				gluLookAt(0.0f,4.0f,4.0f,
					0.0f,0.0f,-4.0f, 
					0.0f,0.0f,-1.0f );

				break;
			default:
				break;
		}

		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();		

		g_pVideoMixer->Display();


	}


	glutDispMsg(prtinfo);
	glutSwapBuffers();

	QueryPerformanceCounter (&ed);
	//DP("c:%d \n", 1000*(ed.QuadPart - st.QuadPart)/coutFreq.QuadPart );
}

void glutTimer( int data)
{
	glutPostRedisplay();
	glutTimerFunc(10,glutTimer,0);
}

void glutShutDown(GLvoid)
{

}

void glutReshape(int w,int h)			
{
	g_wndWidth = w;
	g_wndHeight = h;
	
	glViewport( 0, 0, w, h );
	
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();					
	//if ( h==0 )							
	//	gluPerspective(80,(float)w,1.0,5000.0);
	//else
	//	gluPerspective(80,(float)w/(float)h,1.0,5000.0);

	glOrtho(-0.5,0.5, -0.5, 0.5,  0.0,100.0);


	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();		

}


void glutKeyboard(unsigned char key,int x,int y)  
{
	switch(key)
	{
	case 27:				// When Escape Is Pressed...
		glutShutDown();
		break;				
	default:				
		break;
	}
}


DWORD WINAPI glutThreadProc( LPVOID lpParameter)
{
	CVideoMixerOpenGL* pThis = (CVideoMixerOpenGL*)lpParameter;
	if(!pThis)	return -1;

	EnterCriticalSection(&(pThis->m_csDisplay));

	int argc = 0;
	glutInit(&argc, 0);
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);		

	glutInitWindowPosition(pThis->m_rViewPort.left,pThis->m_rViewPort.top);
	glutInitWindowSize(pThis->m_rViewPort.right - pThis->m_rViewPort.left,pThis->m_rViewPort.bottom - pThis->m_rViewPort.top);
	glutSetWindow((int)pThis ->m_hWndParent);
	
	glutCreateWindow( "OpenGL VideoMixer" );
	// GLee is used to setup all the extensions available
	GLeeInit();

#ifdef WGL_EXT_swap_control
	wglSwapIntervalEXT (1);
#endif

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);					
	glEnable(GL_DEPTH_TEST);			
	glDepthFunc(GL_LEQUAL);				

	// set up one-byte alignment for pixel storage (saves memory)
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//glEnable(GL_CULL_FACE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glEnable(GL_TEXTURE_2D);

	// Setup the various call back functions GLUT requires
	glutDisplayFunc     ( glutDisplay );  
	glutReshapeFunc     (glutReshape );
	glutKeyboardFunc    (glutKeyboard );
	//glutIdleFunc		(glutIdle );
	glutTimerFunc(10,glutTimer,0);
	LeaveCriticalSection(&(pThis->m_csDisplay));

	glutMainLoop( );			// Run the main GLUT loop for rendering
	return 0;
}

COpenGLVideoContext::COpenGLVideoContext(HWND hWndParent, HWND hWndNotify, DWORD dwNotifyID, RECT *prPos)
{
	if(prPos)
	{
		m_rPos = *prPos;
		m_bShow = TRUE;
	}
	else
	{
		SetRect(&m_rPos, 0,0,64,64);
		m_bShow = FALSE;
	}
	
	m_hWndNotify = hWndNotify;
	m_dwNotifyID = dwNotifyID;
	m_textureId = -1;
	m_pVideoPresenter = 0;
	m_pBuff = NULL;
	memset(&m_openOptions, 0,sizeof(m_openOptions));
	m_contentUpdated = false;
	InitializeCriticalSection(&m_csDisplay);
	m_lZoomMode = VP_ZOOM_MODE_STRETCH;
	m_pVideoWindow = 0;
	m_bpp =	4 ;
	m_textureMgr = CTextureManager::GetSingleton();
	m_bBringToTop = false;
	m_bPopUp = false;
	memset(&m_tSetposStart, 0,sizeof(m_tSetposStart));
	m_setposSpeed = 500;
	m_zOrder = 0.0f;
 	QueryPerformanceFrequency(&m_coutFreq);
}

COpenGLVideoContext::~COpenGLVideoContext()
{
	DeleteCriticalSection(&m_csDisplay);
	m_bShow = FALSE;
}

HRESULT COpenGLVideoContext::GetPos2D(RECT *prPos)
{
	if(prPos==NULL)
		return E_POINTER;
	*prPos = m_rPos;
	return S_OK;
}

HRESULT COpenGLVideoContext::SetPos2D(RECT *prPos)
{
	if(prPos==NULL)
		return E_POINTER;

	QueryPerformanceCounter (&m_tSetposStart);

	m_rLastPos = m_rPos ;
	
	m_rPos = *prPos;

	if(m_pVideoWindow)
	{
		HRESULT hr = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&m_rPos , sizeof(m_rPos ));
		if(FAILED(hr))		return hr;
	}

	return S_OK;
}
HRESULT COpenGLVideoContext::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return S_OK;
}
HRESULT COpenGLVideoContext::BringToTop2D()
{
	m_bBringToTop = true;
	return S_OK;
}

HRESULT COpenGLVideoContext::Connect(REFIID riid, LPUNKNOWN pUnk)
{
	if(riid == IID_IMcVideoPresenter)
	{
		HRESULT hr = S_OK;
		m_pVideoPresenter = (IMcVideoPresenter*)pUnk;

		if(!m_pVideoPresenter)	return E_FAIL;

		EnterCriticalSection(&m_csDisplay);
		m_pVideoPresenter->SetInterface(1,IID_ICivmVideoContext,this);
		LeaveCriticalSection(&m_csDisplay);

		m_pVideoPresenter->Get(IID_ICivmVideoContext,MC_PROPID_VP_OPENOPTIONS,0,0,&m_openOptions,0,0);
		m_pBuff = (unsigned char*)malloc(m_openOptions.dwWidth*m_openOptions.dwHeight*4);

		/*Init ZOOM MODE SUPPORT*/

		
		if(m_pVideoWindow)
		{
			m_pVideoWindow->Close();
			m_pVideoWindow->Release();
			m_pVideoWindow  = NULL;
		}
		m_pVideoWindow = CreateVideoWindow();
		if(!m_pVideoWindow )	return E_FAIL;
		VE_Options tVEOpen;
		ZeroMemory(&tVEOpen, sizeof(tVEOpen));

		tVEOpen.mode = (VE_MODE)m_lZoomMode;
		tVEOpen.rSrcClip.right = tVEOpen.rDst.right = tVEOpen.rSrc.right = m_openOptions.dwWidth;
		tVEOpen.rSrcClip.bottom = tVEOpen.rDst.bottom =  tVEOpen.rSrc.bottom = m_openOptions.dwHeight;
		tVEOpen.aspect.dwAspectX = m_openOptions.dwAspectX;
		tVEOpen.aspect.dwAspectY = m_openOptions.dwAspectY;

		hr = m_pVideoWindow->Open(&tVEOpen,sizeof(tVEOpen));
		if(FAILED(hr))	return hr;

		hr = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&m_rPos , sizeof(m_rPos ));
		if(FAILED(hr))		return hr;

		AddRef();
		return S_OK;
	}
	return E_INVALIDARG;
}
HRESULT COpenGLVideoContext::DisConnect(REFIID riid, LPUNKNOWN pUnk) 
{
	if(riid == IID_IMcVideoPresenter)
	{
		EnterCriticalSection(&m_csDisplay);
		m_pVideoPresenter->SetInterface(0,IID_ICivmVideoContext,this);

		if(m_pVideoWindow)
		{
			m_pVideoWindow->Close();
			m_pVideoWindow->Release();
			m_pVideoWindow  = NULL;
		}

		if(m_pBuff)
		{
			delete m_pBuff ;
			m_pBuff  = NULL;
		}
		LeaveCriticalSection(&m_csDisplay);

		m_pVideoPresenter = NULL;
		Release();
		return S_OK;
	}
	return E_INVALIDARG;
}


HRESULT COpenGLVideoContext::PrepareFrame(void)
{
	if(!m_pVideoPresenter)	return E_FAIL;
	if(!m_bShow)	return S_OK;

	glTranslatef(0.0f,0.0f,m_zOrder);

	if(m_bPopUp)
	{
		long eclispeTick = 0;
		static long lastTick = GetTickCount();
		static long currTick = GetTickCount();

		static float sFact = 0.0f;
		static float sflag = 1.0f;
		currTick = GetTickCount();
		
		eclispeTick  = currTick -lastTick ;

		sFact  += ((float)eclispeTick/1000.0f)/2.0f*sflag;
		if(sFact > 1.0f)
			sflag = -1.0f;

		if(sFact < 0.0f)
			sflag = 1.0f;

		if(sFact > 1.0f)	sFact = 1.0f;
		else if(sFact < -1.0f)	sFact = -1.0f;

		glScalef(sFact,sFact,1.0f);

		lastTick = GetTickCount();
	}


	LARGE_INTEGER setposEnd;
	QueryPerformanceCounter (&setposEnd);

	long setposEclipse = 1000*(setposEnd.QuadPart - m_tSetposStart.QuadPart)/m_coutFreq.QuadPart ;
	float passPercent = (float)setposEclipse /m_setposSpeed;
	if(passPercent < 1.0f)
	{
	//	glRotatef(360.0f*passPercent,0.0f,1.0f,0.0f);
	}

	if(!m_contentUpdated)
	{
		if(m_textureId == -1)
			return S_OK;
		glBindTexture (GL_TEXTURE_2D, m_textureId);

	}
	else
	{
		EnterCriticalSection(&m_csDisplay);
		m_textureId = m_textureMgr.LoadTextureFromMemory(m_pBuff, m_openOptions.dwWidth, m_openOptions.dwHeight,m_bpp,  m_textureId);
		m_contentUpdated = false;
		LeaveCriticalSection(&m_csDisplay);
	}

	glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5,  0.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5, -0.5,  0.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5,  0.5,  0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5,  0.5,  0.5);
	glEnd();

	return S_OK;
}

HRESULT COpenGLVideoContext::Get(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData) 
{
	if(!pPropData)	return E_INVALIDARG;
	switch(dwPropID)
	{
		case MC_PROPID_VP_ZOOM:
			return ReturnComLong(m_lZoomMode,pPropData,cbPropData,0);
		case MC_PROPID_VP_FOURCC:
			*((DWORD*)pPropData) = MAKEFOURCC('A','R','G','B');
			return S_OK;
		case MC_PROPID_VM_IS_TOPLAYER:
			*reinterpret_cast<bool *>(pPropData) = m_bBringToTop;
			m_bBringToTop = false;
			return S_OK;
		case MC_PROPID_VM_ZORDER:
			*reinterpret_cast<float *>(pPropData) = m_zOrder;
			return S_OK;
		default:
			return E_NOTIMPL;
	}
};

HRESULT COpenGLVideoContext::Set(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData) 
{
	if(!pPropData)	return E_INVALIDARG;
	HRESULT hr = S_OK;
	switch(dwPropID)
	{
		case MC_PROPID_VP_ZOOM:
			if(m_pVideoWindow)
			{
				m_lZoomMode = *reinterpret_cast<VP_ZOOM_MODE*>(pPropData);			
				hr = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_MODE,0,0,&m_lZoomMode, sizeof(m_lZoomMode));
				return hr;
			}
		case MC_PROPID_VM_ZORDER:
			m_zOrder = *reinterpret_cast<float *>(pPropData) ;
			return S_OK;

		default:
			break;
	}

	return E_NOTIMPL;

};

STDMETHODIMP COpenGLVideoContext::Lock(LPBYTE *ppData, DWORD *pdwStride)
{
	if(!ppData ||!pdwStride )	return E_INVALIDARG;
	if(!m_pBuff)	return E_FAIL;
	
	EnterCriticalSection(&m_csDisplay);
	*ppData  = m_pBuff;
	*pdwStride = m_openOptions.dwWidth*4;
	return S_OK;
}

STDMETHODIMP COpenGLVideoContext::Unlock()
{
	LeaveCriticalSection(&m_csDisplay);

	m_contentUpdated = true;
	return S_OK;
}

HRESULT COpenGLVideoContext::GetActualPos2D(RECT* pDst)
{
	if(!pDst)	return E_INVALIDARG;
	
	if(m_pVideoWindow)
	{
		HRESULT	hr = m_pVideoWindow->Get(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,pDst, sizeof(RECT),NULL);
		if(FAILED(hr))	return hr;
	}
	else
		*pDst= m_rPos;



	LARGE_INTEGER setposEnd;
	QueryPerformanceCounter (&setposEnd);

	long setposEclipse = 1000*(setposEnd.QuadPart - m_tSetposStart.QuadPart)/m_coutFreq.QuadPart ;
	float passPercent = (float)setposEclipse /m_setposSpeed;

	if(passPercent < 1.0f)
	{
		long leftoffset = m_rPos.left - m_rLastPos.left;
		long topoffset = m_rPos.top- m_rLastPos.top;
		long rightoffset = m_rPos.right - m_rLastPos.right;
		long bottomoffset = m_rPos.bottom - m_rLastPos.bottom;
		
		SetRect(pDst ,m_rLastPos.left + leftoffset*passPercent,
			m_rLastPos.top + topoffset*passPercent,
			m_rLastPos.right + rightoffset*passPercent,
			m_rLastPos.bottom + bottomoffset*passPercent
			);
	}
	else if(passPercent >= 1.0f && passPercent <= 2.0f)
	{
		long test = 0;
		test = 1;
	}
	return S_OK;
}

HRESULT COpenGLVideoContext::StartShakeEffect()
{
	m_bPopUp = true;
	return S_OK;
}

HRESULT COpenGLVideoContext::StopShakeEffect()
{
	m_bPopUp = false;
	return S_OK;
}

#define MACRO_ZORDER_STEP		-0.1f
CVideoMixerOpenGL::CVideoMixerOpenGL()
{
	m_bShow = FALSE;
	SetRect(&m_rViewPort, 0,0,0,0);
	memset(m_contextArrary,0,  sizeof(m_contextArrary));
	m_pDMCP = NULL;
	m_DMCP_ACP_flag = 0;
	m_DMCP_CGMS_flag = 0;
	m_DMCP_HDCP_flag = 0;
	m_bOpen = false;
	m_hThread = 0;
	InitializeCriticalSection(&m_csDisplay);
}

CVideoMixerOpenGL::~CVideoMixerOpenGL()
{
	DeleteCriticalSection(&m_csDisplay);
	Close();
}


HRESULT CVideoMixerOpenGL::CreateVideoContext(ICivmVideoContext **ppVideoContext, DWORD dwVideoType, RECT *prPos)
{
	if(!m_bOpen)	return E_FAIL;
	
	if(ppVideoContext==NULL)
		return E_POINTER;

	int iCount =0;
	for(iCount =0;iCount<sizeof(m_contextArrary)/sizeof(COpenGLVideoContext*);iCount++)
	{
		if(!m_contextArrary[iCount])
			break;
	}
	if(iCount>=sizeof(m_contextArrary)/sizeof(COpenGLVideoContext*))		return E_FAIL;

	EnterCriticalSection(&m_csDisplay);
	COpenGLVideoContext *pVideo = new COpenGLVideoContext(0, m_hWndParent, m_dwNotifyID, prPos);
	if(pVideo==NULL)
	{
		LeaveCriticalSection(&m_csDisplay);
		return E_FAIL;
	}
	m_contextArrary[iCount] = pVideo;
	*ppVideoContext = static_cast<ICivmVideoContext *>(pVideo);
	float zOrder = iCount*MACRO_ZORDER_STEP;
	m_contextArrary[iCount]->Set(MC_PROPID_VM_ZORDER, &zOrder, 0);
	
	LeaveCriticalSection(&m_csDisplay);
	return S_OK;
}

HRESULT CVideoMixerOpenGL::DestroyVideoContext(ICivmVideoContext *ppVideoContext)
{
	int iCount =0;
	if(!ppVideoContext)	return E_INVALIDARG;
		
	EnterCriticalSection(&m_csDisplay);
	for(iCount =0;iCount<sizeof(m_contextArrary)/sizeof(COpenGLVideoContext*);iCount++)
	{
		if(m_contextArrary[iCount] == ppVideoContext)
		{
			m_contextArrary[iCount] = NULL;
			LeaveCriticalSection(&m_csDisplay);
			return S_OK;
		}
	}
	
	LeaveCriticalSection(&m_csDisplay);
	return E_FAIL;
}

HRESULT CVideoMixerOpenGL::GetViewPort(RECT *prPort)
{
	if(prPort==NULL)
		return E_POINTER;

	*prPort = m_rViewPort;
	return S_OK;
}
HRESULT CVideoMixerOpenGL::SetViewPort(RECT *prPort)
{
	if(!prPort)	return E_INVALIDARG;
	if(m_hWndParent)	return E_UNEXPECTED;

	return SetViewPortPri(prPort);
}

HRESULT CVideoMixerOpenGL::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;

	if(m_bShow && m_bCanDisp)
		glutShowWindow();
	else
		glutHideWindow();

	return S_OK;
}

HRESULT CVideoMixerOpenGL::Connect(REFIID riid, LPUNKNOWN pUnk)
{	
	return E_INVALIDARG;
}
HRESULT CVideoMixerOpenGL::DisConnect(REFIID riid, LPUNKNOWN pUnk) 
{
	return E_INVALIDARG;
}
HRESULT CVideoMixerOpenGL::SetViewPortPri(RECT* prPort)
{
	if(prPort==NULL)
		return E_POINTER;

	if((m_rViewPort .left  == prPort->left)&&
		(m_rViewPort .top  == prPort->top) &&
		(m_rViewPort .right  == prPort->right) &&
		(m_rViewPort .bottom  == prPort->bottom))
		return S_OK;

	m_rViewPort  = *prPort;
	
	glutReshapeWindow(prPort->right-prPort->left,prPort->bottom-prPort->top);
	glutPositionWindow(prPort->left,prPort->top );


	if(m_pDMCP)
	{
		HRESULT hr = m_pDMCP->SetTargetRect(&m_rViewPort);
		if(FAILED(hr))
			DP("[GFXPRESENT] SetTargetRect failed :0x%x \n",hr);

		m_bCanDisp = m_pDMCP->CanDisplay(FALSE);

		if(m_bShow && m_bCanDisp)
			glutShowWindow();
		else
			glutHideWindow();

	}

	return S_OK;
}


HRESULT CVideoMixerOpenGL::Open(HWND hWndParent, DWORD dwNotifyID, DWORD dwMixerType, RECT *prPort)
{
	if(m_bOpen)
		return E_FAIL;

	memset(m_contextArrary,0,  sizeof(m_contextArrary));

	//Use m_hWndParent>prPort>(64,64)
	m_hWndParent = hWndParent;
	memset(&m_rViewPort, 0,sizeof(m_rViewPort));
	if(m_hWndParent)
	{
		::GetClientRect(m_hWndParent,&m_rViewPort);

		POINT p={0,0};
		::ClientToScreen(m_hWndParent , &p);
		OffsetRect(&m_rViewPort,p.x,p.y);
	}
	else if(prPort)
	{
		m_rViewPort = *prPort;
	}
	else
	{
		SetRect(&m_rViewPort , 0,0,64,64);
	}

	
	//Init dmcp
	m_pDMCP = FindDmcpDisplay();
	if(m_pDMCP != 0)
	{
		DMCP_DisplayOpenParams dmcp_params;
		ZeroMemory(&dmcp_params,sizeof(dmcp_params));
		dmcp_params.pInfoFunc = NULL;
		dmcp_params.pInfoFuncCtxt = NULL;
		HRESULT res = m_pDMCP->Open(&dmcp_params,sizeof(dmcp_params));
		if(FAILED(res))
		{
			DP("[GFXPRESENT]DMCP Open failed \n");
			m_pDMCP->Release();
			m_pDMCP = NULL;
			m_bCanDisp = true;
		}
		else
		{
			DMCP_DisplayState WrStatus;
			ZeroMemory(&WrStatus,sizeof(WrStatus));
			WrStatus.dwSize = sizeof(WrStatus);
			WrStatus.dwCPFlag = DMCP_CPFLAG_ACP | DMCP_CPFLAG_CGMSA | DMCP_CPFLAG_HDCP;
			WrStatus.dwACP = m_DMCP_ACP_flag;
			WrStatus.dwCGMSA= m_DMCP_CGMS_flag;
			WrStatus.dwHDCP= m_DMCP_HDCP_flag;
			
			res = m_pDMCP->SetDisplayState(&WrStatus);
			if(FAILED(res))
				DP("[GFXPRESENT]DMCP Set failed : res:%d __ %d \n",res, WrStatus.dwErrorCPFlag);

			/*SetDestination*/			
			HRESULT hr = m_pDMCP->SetTargetRect(&m_rViewPort);
			if(FAILED(hr))
				DP("[GFXPRESENT] SetTargetRect failed :0x%x \n",hr);
			m_bCanDisp= m_pDMCP->CanDisplay(FALSE);
	
		}

	}
	else
	{
		m_bCanDisp = true;
		DP("[GFXPRESENT_INFO]  DMCP isn't found \n");
	}

	m_bShow = FALSE;

	m_hThread = ::CreateThread(NULL, 0, glutThreadProc, this, 0, NULL);
	SetThreadPriority(m_hThread,THREAD_PRIORITY_HIGHEST);

	m_dwNotifyID = dwNotifyID;

	m_bOpen = true;
	return S_OK;
}

HRESULT CVideoMixerOpenGL::Close()
{
	if(!m_bOpen)	return E_FAIL;
	
	if(m_pDMCP)
	{
		m_pDMCP->Close();
		m_pDMCP->Release();
		m_pDMCP = NULL;
	}

	EnterCriticalSection(&m_csDisplay);
	
	glutExit();
	if(m_hThread)
	{
		::TerminateThread(m_hThread,0);
		m_hThread = NULL;
	}
	LeaveCriticalSection(&m_csDisplay);

	m_hWndParent = NULL;
	m_bOpen = false;
	return S_OK;
}

HRESULT CVideoMixerOpenGL::Display()
{
	long elapsed = 0;
	LARGE_INTEGER g_tSetposEnd;

	if(m_hWndParent)
	{
		RECT rCurrView;
		::GetClientRect(m_hWndParent,&rCurrView);

		POINT p={0,0};
		::ClientToScreen(m_hWndParent , &p);
		OffsetRect(&rCurrView,p.x,p.y);
		SetViewPortPri(&rCurrView);
	}

	int iCount =0;
	bool bTop = false;
	for(iCount =0;iCount<sizeof(m_contextArrary)/sizeof(COpenGLVideoContext*);iCount++)
	{
		if(!m_contextArrary[iCount])	break;
		HRESULT hr = m_contextArrary[iCount]->Get(MC_PROPID_VM_IS_TOPLAYER	,&bTop, 0);
		if(SUCCEEDED(hr) && bTop)
			break;
	}

	if(bTop)
	{
		float zOrder = 0.0f;
		m_contextArrary[iCount]->Set(MC_PROPID_VM_ZORDER,&zOrder, 0);
		for(int i=0;i<sizeof(m_contextArrary)/sizeof(COpenGLVideoContext*);i++)
		{
			if(!m_contextArrary[i])	break;
			if(i != iCount)
			{
				m_contextArrary[i]->Get(MC_PROPID_VM_ZORDER, &zOrder, 0);
				zOrder += MACRO_ZORDER_STEP;
				m_contextArrary[i]->Set(MC_PROPID_VM_ZORDER, &zOrder, 0);
			}
		}

	}

	glEnable(GL_TEXTURE_2D);
	glTranslatef(0.0f,0.0f,-2.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	EnterCriticalSection(&m_csDisplay);
	for(int i =0;i<sizeof(m_contextArrary)/sizeof(COpenGLVideoContext*);i++)
	{
		if(!m_contextArrary[i])	break;

		glPushMatrix();
		{
			RECT rDst2d = {0};
			if(SUCCEEDED(m_contextArrary[i]->GetActualPos2D(&rDst2d)))
				SetContextDest(&rDst2d);
		}
		m_contextArrary[i]->PrepareFrame();
		glPopMatrix();
	}
	LeaveCriticalSection(&m_csDisplay);

	QueryPerformanceCounter (&g_tSetposEnd);
	elapsed= 1000*(g_tSetposEnd.QuadPart - st.QuadPart)/coutFreq.QuadPart ;
	//DP("cost[%d] %d \n",__LINE__,elapsed);

//	glutSwapBuffers();

	QueryPerformanceCounter (&g_tSetposEnd);
	elapsed= 1000*(g_tSetposEnd.QuadPart - st.QuadPart)/coutFreq.QuadPart ;
	//DP("cost[%d] %d \n",__LINE__,elapsed);

	return S_OK;
}

HRESULT CVideoMixerOpenGL::Set(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData)
{
	switch(dwPropID)
	{
	case MC_PROPID_VP_DMCP_ACP:
	case MC_PROPID_VP_DMCP_CGMS:
	case MC_PROPID_VP_DMCP_HDCP:
		{
			if(pPropData == NULL)
				return E_INVALIDARG;

			if(m_pDMCP == NULL)
				return E_ACCESSDENIED;

			DWORD dmcp_flag = *(DWORD*)pPropData;
			DMCP_DisplayState WrStatus;
			ZeroMemory(&WrStatus,sizeof(WrStatus));
			WrStatus.dwSize = sizeof(WrStatus);

			if(dwPropID == MC_PROPID_VP_DMCP_ACP)
			{
				WrStatus.dwCPFlag = DMCP_CPFLAG_ACP;
				switch(dmcp_flag)
				{
					case VP_DMCP_ACP_Level0:
						dmcp_flag = DMCP_ACP_Level0;
						break;
					case VP_DMCP_ACP_Level1:
						dmcp_flag = DMCP_ACP_Level1;
						break;
					case VP_DMCP_ACP_Level2:
						dmcp_flag = DMCP_ACP_Level2;
						break;
					case VP_DMCP_ACP_Level3:
						dmcp_flag = DMCP_ACP_Level3;
						break;
					default:
						return E_INVALIDARG;
				}
				WrStatus.dwACP = dmcp_flag;
				m_DMCP_ACP_flag = dmcp_flag;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_CGMS)
			{
				WrStatus.dwCPFlag = DMCP_CPFLAG_CGMSA;
				switch(dmcp_flag)
				{
					case VP_DMCP_CGMSA_Disabled:
						dmcp_flag = DMCP_CGMSA_Disabled;
						break;
					case VP_DMCP_CGMSA_CopyFreely:
						dmcp_flag = DMCP_CGMSA_CopyFreely;
						break;
					case VP_DMCP_CGMSA_CopyNoMore:
						dmcp_flag = DMCP_CGMSA_CopyNoMore;
						break;
					case VP_DMCP_CGMSA_CopyOneGeneration:
						dmcp_flag = DMCP_CGMSA_CopyOneGeneration;
						break;
					case VP_DMCP_CGMSA_CopyNever:
						dmcp_flag = DMCP_CGMSA_CopyNever;
						break;
					default:
						return E_INVALIDARG;
				}
				WrStatus.dwCGMSA= dmcp_flag;
				m_DMCP_CGMS_flag = dmcp_flag;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_HDCP)
			{
				WrStatus.dwCPFlag = DMCP_CPFLAG_HDCP;
				switch(dmcp_flag)
				{
					case VP_DMCP_HDCP_Level0 :
						dmcp_flag = DMCP_HDCP_Level0 ;
						break;
					case VP_DMCP_HDCP_Level1:
						dmcp_flag = DMCP_HDCP_Level1;
						break;
					default:
						return E_INVALIDARG;
				}
				WrStatus.dwHDCP= dmcp_flag;
				m_DMCP_HDCP_flag = dmcp_flag;
			}
			else 
				return E_FAIL;

			HRESULT hr = m_pDMCP->SetDisplayState(&WrStatus);
			return hr;
		}
		default:
			break;
	}
	return S_OK;
}


HRESULT CVideoMixerOpenGL::Get(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData)
{
	switch(dwPropID)
	{
	case MC_PROPID_VP_DMCP_ACP:
	case MC_PROPID_VP_DMCP_CGMS:
	case MC_PROPID_VP_DMCP_HDCP:
		{
			if(pPropData == NULL)
				return E_INVALIDARG;

			if(m_pDMCP == NULL)
				return E_ACCESSDENIED;

			DWORD dmcp_flag = 0;
			DMCP_DisplayState RdStatus;
			ZeroMemory(&RdStatus,sizeof(RdStatus));
			RdStatus.dwSize = sizeof(RdStatus);
			m_pDMCP->GetDisplayState(&RdStatus);
			if(dwPropID == MC_PROPID_VP_DMCP_ACP)
			{
				if(RdStatus.dwCPFlag & DMCP_CPFLAG_ACP)
				{
					switch(RdStatus.dwACP)
					{
						case DMCP_ACP_Level0:
							dmcp_flag = VP_DMCP_ACP_Level0;
							break;
						case DMCP_ACP_Level1:
							dmcp_flag = VP_DMCP_ACP_Level1;
							break;
						case DMCP_ACP_Level2:
							dmcp_flag = VP_DMCP_ACP_Level2;
							break;
						case DMCP_ACP_Level3:
							dmcp_flag = VP_DMCP_ACP_Level3;
							break;
						default:
							return E_ACCESSDENIED;
					}

					*reinterpret_cast<DWORD*>(pPropData) = dmcp_flag;
					return S_OK;
				}
				else
					return E_ACCESSDENIED;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_CGMS)
			{
				if(RdStatus.dwCPFlag & DMCP_CPFLAG_CGMSA)
				{
					switch(RdStatus.dwCGMSA)
					{
						case DMCP_CGMSA_Disabled :
							dmcp_flag = VP_DMCP_CGMSA_Disabled ;
							break;
						case DMCP_CGMSA_CopyFreely:
							dmcp_flag = VP_DMCP_CGMSA_CopyFreely;
							break;
						case DMCP_CGMSA_CopyNoMore:
							dmcp_flag = VP_DMCP_CGMSA_CopyNoMore;
							break;
						case DMCP_CGMSA_CopyOneGeneration:
							dmcp_flag = VP_DMCP_CGMSA_CopyOneGeneration;
						case DMCP_CGMSA_CopyNever:
							dmcp_flag = VP_DMCP_CGMSA_CopyNever;
							break;
						default:
							return E_ACCESSDENIED;
					}
					*reinterpret_cast<DWORD*>(pPropData) = dmcp_flag;
					return S_OK;
				}
				else
					return E_ACCESSDENIED;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_HDCP)
			{
				if(RdStatus.dwCPFlag & DMCP_CPFLAG_HDCP)
				{
					switch(RdStatus.dwHDCP)
					{
						case DMCP_HDCP_Level0:
							dmcp_flag = VP_DMCP_HDCP_Level0;
							break;
						case DMCP_HDCP_Level1:
							dmcp_flag = VP_DMCP_HDCP_Level1;
							break;
						default:
							return E_ACCESSDENIED;
					}
					*reinterpret_cast<DWORD*>(pPropData) = dmcp_flag;
					return S_OK;
				}
				else
					return E_ACCESSDENIED;
			}
			else
				return E_FAIL;
		}

		default:
			break;
	}
	
	return E_NOTIMPL;
}

HRESULT CVideoMixerOpenGL::SetContextDest(RECT* pDst)
{
	if(!pDst)	return E_INVALIDARG;
	
	GLfloat posX=0;
	GLfloat posY=0; 
	GLfloat sizeX=0;
	GLfloat sizeY=0; 

	long canvasWidth = m_rViewPort.right - m_rViewPort.left;
	long canvasHeight = m_rViewPort.bottom - m_rViewPort.top;
	
	posX = (GLfloat)(pDst->left + pDst->right)/2 ;
	posY = (GLfloat)(pDst->bottom + pDst->top)/2;

	posX = (GLfloat)(posX - canvasWidth/2)/(GLfloat)(canvasWidth) ;
	posY = (GLfloat)(canvasHeight/2 - posY)/(GLfloat)(canvasHeight);

	sizeX = (GLfloat)(pDst->right - pDst->left) /canvasWidth;
	sizeY = (GLfloat)(pDst->bottom - pDst->top )/canvasHeight;
	
	glTranslatef(posX, posY, 0.0f);
	glScalef(sizeX,sizeY,1.0f);

	return S_OK;
	
}

ICivmVideoMixer *CreateVideoMixerOpenGL()
{
	if(!g_pVideoMixer)
		g_pVideoMixer = new CVideoMixerOpenGL();
	return static_cast<ICivmVideoMixer *>(g_pVideoMixer);
}
#endif
