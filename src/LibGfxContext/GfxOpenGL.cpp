#include "../../inc/GfxBuildDef.h"

#ifdef GFX_OPENGL
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "GfxOverlay.h"
#include "tchar.h"


#include <gl\gl.h> 
#include <gl\glu.h> 
#pragma comment(lib, "openGL32.lib")
#pragma comment(lib, "GLu32.lib")

class CGfxOpenGl: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxOpenGl();
	virtual ~CGfxOpenGl();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);		
	STDMETHODIMP Close();
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount);
	STDMETHODIMP Unlock(LONG num);
	STDMETHODIMP Flip(LONG num, DWORD dwFlags);
	STDMETHODIMP Show(BOOL bShow);
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags);
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags);
	STDMETHODIMP SetSrcRect(const RECT *pRect);
	STDMETHODIMP SetDstRect(const RECT *pRect);
	STDMETHODIMP SetScnRect(const RECT *pRect);
	STDMETHODIMP GetScnRect(RECT *pRect);

	STDMETHODIMP SetWindowHandle(HWND hwnd);
	STDMETHODIMP GetWindowHandle(HWND& hwnd);
	STDMETHODIMP SetAlpha(DWORD dwAlpha);
	STDMETHODIMP SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey);
	STDMETHODIMP SetDstColorKey(BOOL bEnable, DWORD dwDstCkey);
	STDMETHODIMP GetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP GetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP SetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP SetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP GetDDCaps(LONG*pCaps);
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize);
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP IsOpen();

	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP CreateGLWindow(HINSTANCE hInstance,int width, int height,int bits );
	STDMETHODIMP ReSizeGLScene(GLsizei width, GLsizei height);				
	STDMETHODIMP KillGLWindow();

	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}


private:
	STDMETHODIMP GetFourCCLength(DWORD fourCC, DWORD *plength);

	HWND m_hwnd;	
	BOOL m_bOpen;
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	int				m_dwBackBuffers;
	bool m_fullscreen;
	DWORD m_fourCC;
	HGLRC mhRC;
	HDC mhDC ;
	LPBYTE m_pbuff;
	GLuint		mtexture ;							// 存储一个纹理
	//HWND m_hCusWnd;
};

static CGfxOpenGl* pThis = NULL;

CGfxOpenGl::CGfxOpenGl(): CMcUnknown(0,0),
	m_fullscreen(false),
	m_fourCC(NULL),
	mhRC(NULL),
	mhDC(NULL),
	m_pbuff(NULL),
	mtexture(NULL)
	//m_hCusWnd(NULL)
{
	m_hwnd = 0;
	m_bOpen = FALSE;
	memset(&m_rectSrc, 0 ,sizeof(m_rectSrc));
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	memset(&m_rectScn, 0 ,sizeof(m_rectScn));

	m_dwWidth = 0;
	m_dwHeight = 0;
	m_dwFourCC = 0;
	m_dwBackBuffers = 0;

	pThis = this;
}

CGfxOpenGl::~CGfxOpenGl()
{
	pThis = NULL;
}

#define DEFAULT_BACK_BUFFER	1

STDMETHODIMP CGfxOpenGl::GetFourCCLength(DWORD fourCC, DWORD *plength)
{
	if(plength == NULL)
		return E_FAIL;
	
	switch(fourCC)
	{
		case MAKEFOURCC('Y','V','1','2'):
		case MAKEFOURCC('N','V','1','2'):
		case MAKEFOURCC('Y','U','Y','2'):
			*plength = 1;
			break;
		case MAKEFOURCC('R','5','6','5'):
			*plength = 2;
			break;
		case MAKEFOURCC('R','8','8','8'):
			*plength = 3;
			break;
		default:
			return E_FAIL;
	}
	
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int ret  = 0;
	HRESULT hr = S_OK;
	if(m_bOpen || (dwBuffers==0) || (pdwFourCC == NULL))
		return E_UNEXPECTED;

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	if(dwBuffers == 0)
		m_dwBackBuffers = DEFAULT_BACK_BUFFER;
	else
		m_dwBackBuffers = dwBuffers;
	
	m_fourCC = NULL;
	int i =0;
	for( i=0;pdwFourCC[i];i++)
	{
		if(pdwFourCC[i] == MAKEFOURCC('R','8','8','8'))
		{
			m_fourCC = pdwFourCC[i] ;
			break;
		}

	}
	
	if(!pdwFourCC[i])
		return E_FAIL;

	m_dwFourCC = pdwFourCC[i];
	DWORD bytes;
	hr = GetFourCCLength(m_fourCC, &bytes);
	if(FAILED(hr))	return hr;
	//hr = CreateGLWindow(NULL, m_dwWidth, m_dwHeight ,bytes*8);
	//if(FAILED(hr))	return hr;


	PIXELFORMATDESCRIPTOR pfd={0};	// pixel descriptor struct

	if (!(mhDC=GetDC(m_hwnd)))							
		goto FAIL;

	// set the pixel format for the DC
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 0;		// BPP is ignored for windowed mode
	pfd.cDepthBits = 16;			// 16-bit z-buffer
	pfd.iLayerType = PFD_MAIN_PLANE;

	int PixelFormat = 0;
	if (!(PixelFormat=ChoosePixelFormat(mhDC,&pfd)))				
		goto FAIL;


	if(!SetPixelFormat(mhDC,PixelFormat,&pfd))				
		goto FAIL;


	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	if (!(mhRC=wglCreateContext(mhDC)))					
		goto FAIL;

	if(!wglMakeCurrent(mhDC,mhRC))						
		goto FAIL;

	glShadeModel(GL_SMOOTH);					// enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// black background/clear color

	// set-up the depth buffer
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glDepthFunc(GL_LEQUAL);

	// set up one-byte alignment for pixel storage (saves memory)
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// use backface culling (this is 2D, so we'll never see the back faces anyway)
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	// perspective calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	m_pbuff = (LPBYTE)malloc(m_dwWidth*m_dwHeight*3);
	if(!m_pbuff )	return E_FAIL;
	
	SetRect(&m_rectSrc, 0 ,0, m_dwWidth,m_dwHeight);
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	m_bOpen = TRUE;
	return S_OK;

FAIL:

	Close();
	return E_FAIL;
}

STDMETHODIMP CGfxOpenGl::Close()
{
	if(!m_bOpen)
		return E_UNEXPECTED;

	SendMessage(m_hwnd, WM_DESTROY, NULL,NULL);
	if (mhRC)								
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,_T("Failed to release binding"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(mhRC))					
		{
			MessageBox(NULL,_T("Failed to release rendering context"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		}
		mhRC=NULL;							
	}

	if (mhDC && !ReleaseDC(m_hwnd,mhDC))					
	{
		MessageBox(NULL,_T("Failed to release device context"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		mhDC=NULL;							
	}

	if(m_pbuff )
	{
		delete m_pbuff ;
		m_pbuff  = NULL;
	}
	
	m_hwnd=NULL;
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxOpenGl::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(num >= m_dwBackBuffers)
		return E_FAIL;
	if(ppBuf)
		*ppBuf = m_pbuff;
	if(pStride)
		*pStride = m_dwWidth*3;
	if(pPixCount)
	{
		if(m_dwFourCC == MAKEFOURCC('R','8','8','8'))
			*pPixCount = 3;
	}
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::Unlock(LONG num)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::Flip(LONG num, DWORD dwFlags)
{
	if(num >= m_dwBackBuffers)
		return E_FAIL;

	static GLfloat boxcol[6][3]=				// 盒子的颜色数组
	{
		// 亮:红，橙，黄，绿，蓝
		{1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
	};


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// clear screen and depth buffers

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();							
	glOrtho(-1.0f,1.0f,-1.0f,1.0f,0.1f,10.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	if(!mtexture)
		glGenTextures(1, &mtexture);					
	glBindTexture(GL_TEXTURE_2D, mtexture);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
	glTexImage2D(GL_TEXTURE_2D, 0,3, m_dwWidth, m_dwHeight, 0, GL_BGR_EXT,GL_UNSIGNED_BYTE, m_pbuff);
	glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0f);					// 法线指向观察者

	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
	glEnd();

	SwapBuffers(mhDC);			

	return S_OK;

}
STDMETHODIMP CGfxOpenGl::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxOpenGl::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxOpenGl::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;

	return S_OK;
}

STDMETHODIMP CGfxOpenGl::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
	m_rectDst = *pRect;

	ReSizeGLScene( m_rectDst.right - m_rectDst .left, m_rectDst .bottom - m_rectDst .top);
	//MoveWindow(m_hwnd,0, 0, m_rectDst.right - m_rectDst .left, m_rectDst .bottom - m_rectDst .top,false);

	return S_OK;
}
STDMETHODIMP CGfxOpenGl::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetScnRect(RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetDDCaps(LONG*pCaps)
{
	if(pCaps==0)
		return E_POINTER;
	*pCaps = GFXOVERLAY_CAPS_CROPPING_SUPPORT;

	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{

	return S_OK;
}
STDMETHODIMP CGfxOpenGl::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(m_bOpen==0 || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0; 

	return S_OK;
}

STDMETHODIMP CGfxOpenGl::IsOpen()
{
	return m_bOpen? S_OK : S_FALSE;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
     case WM_SIZE:        // Change the window size.
	     if(pThis)
			pThis->ReSizeGLScene( LOWORD(lParam), HIWORD(lParam) );
		break;
	case WM_DESTROY:
	     if(pThis)
			pThis->KillGLWindow();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

STDMETHODIMP CGfxOpenGl::ReSizeGLScene(GLsizei width, GLsizei height)				
{

	if (height==0)								
		height=1;							

	glViewport(0, 0, width, height);					

//	glMatrixMode(GL_PROJECTION);						
//	glLoadIdentity();							
//
//	
////	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
//
//	glMatrixMode(GL_MODELVIEW);						
//	glLoadIdentity();

	return S_OK;
}

STDMETHODIMP CGfxOpenGl::CreateGLWindow(HINSTANCE hInstance,int width, int height,int bits )
{
	WNDCLASS	wc;							
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		
	wc.lpfnWndProc		= (WNDPROC) WndProc;				
	wc.cbClsExtra		= 0;						
	wc.cbWndExtra		= 0;						
	wc.hInstance		= GetModuleHandle(NULL);//hInstance;//					
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;						
	wc.lpszMenuName		= NULL;						
	wc.lpszClassName	= _T("OpenG");

	if (!RegisterClass(&wc))						
	{
		MessageBox(NULL,_T("RegisterClass failed"),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if (m_fullscreen)								
	{
		DEVMODE dmScreenSettings;						
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));			
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,_T("Faild to switch to fullscreen mode\nUse Window mode？"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				m_fullscreen=FALSE;				
			}
			else
			{
				MessageBox(NULL,_T("To be closed"),_T("Error"),MB_OK|MB_ICONSTOP);
				return E_FAIL;					
			}
		}
	}

	DWORD		dwExStyle;						
	DWORD		dwStyle;						
	if (m_fullscreen)								
	{
		dwExStyle=WS_EX_APPWINDOW;					
		dwStyle=WS_POPUP;						
		ShowCursor(FALSE);						
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle= WS_CHILD;
	}

	RECT WindowRect;							
	WindowRect.left=(long)0;						
	WindowRect.right=(long)width;						
	WindowRect.top=(long)0;							
	WindowRect.bottom=(long)height;					
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		

	if (!(m_hwnd=CreateWindowEx(	dwExStyle,				
					_T("OpenG"),				
					NULL,					
					WS_CLIPSIBLINGS |			
					WS_CLIPCHILDREN |			
					dwStyle,				
					0, 0,					
					WindowRect.right-WindowRect.left,	
					WindowRect.bottom-WindowRect.top,	
					NULL/*m_hCusWnd*/,					
					NULL,					
					hInstance,				
					NULL)))							
	{
		KillGLWindow();							
		MessageBox(NULL,_T("Failed to create window"),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return E_FAIL;							
	}

	return S_OK;
}


STDMETHODIMP CGfxOpenGl::KillGLWindow()
{
	if (m_fullscreen)								
	{
		ChangeDisplaySettings(NULL,0);				
		ShowCursor(TRUE);						
	}


	if (m_hwnd && !DestroyWindow(m_hwnd))					
	{
		MessageBox(NULL,_T("Failed to destryo window"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		m_hwnd=NULL;							
	}
#if 0
	if(m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}

	if(m_pbuf)
	{
		free(m_pbuf);
		m_pbuf = NULL;
	}	
	if(m_pSwapbuf)
	{
		free(m_pSwapbuf);
		m_pSwapbuf = NULL;
	}	
#endif
	return S_OK;
}
IMcGfxOverlay *CreateOverlayOPENGL()
{
	return new CGfxOpenGl();
}
#endif
