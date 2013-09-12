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

#include "../../inc/GfxBuildDef.h"

#ifdef GFX_EGL


#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"


#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <android/native_window.h> // requires ndk r5 or newer
#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>
#include <GLES/glext.h>



static GLuint getMiValue(GLuint width)
{
    switch (width) {
        case 2:
        case 4:
        case 8:
        case 16:
        case 32:
        case 64:
        case 128:
        case 256:
        case 512:
        case 1024:
        case 2056:
            return width;
        default:
            break;
    } 
    GLuint mi =0;
    
    while(width >0)
    {
        mi++;
        width >>= 1;
    }
    return (2<<(mi-1));
}

static GLfloat getMiPercent(GLuint width)
{
    GLuint mivalue = getMiValue(width);
    
    return (GLfloat)((GLfloat)width/(GLfloat)mivalue);
}

#define EGL_BACKBUFFERS 4
class CGfxEGL : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxEGL();
	virtual ~CGfxEGL();
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

	STDMETHODIMP SetAlpha(DWORD dwAlpha);
	STDMETHODIMP SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey);
	STDMETHODIMP SetDstColorKey(BOOL bEnable, DWORD dwDstCkey);
	STDMETHODIMP GetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP GetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP SetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP SetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP GetDDCaps(LONG* pCaps)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP IsOpen();
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetWindowHandle(HWND hwnd)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetWindowHandle(HWND& hwnd)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP SetWindowHandle(Window hwnd)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetWindowHandle(Window& hwnd)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf, int mode)
	{
		if(!m_bOpen)	
		{
			DP("SetHWRenderSurf not opened \n");
			return E_FAIL;
		}

	        VR_ANDROID_ISUR_PARAM * surfParams = (VR_ANDROID_ISUR_PARAM *)pSurf;

		if(_window)
		{
			//_width = surfParams->scnWidth;
			//_height= surfParams->scnHeight;
			DP("_WINDOW already set \n");
			return E_FAIL;
		}

		DP("Set ANativeWindow* = %p \n",(surfParams->pAndroidSFBuf));


		_window = (ANativeWindow*)(surfParams->pAndroidSFBuf);
//		_msg = MSG_WINDOW_SET;
                initialize();


		DP("Set ANativeWindow* SUCCESS\n");
		return S_OK;
	}
	
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}
	STDMETHODIMP UIFlip(LPVOID pHandle)
	{
		return S_OK;
	}
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  


protected:

	STDMETHODIMP Update();

	bool m_bOpen;
	unsigned char * m_pBuff[EGL_BACKBUFFERS];
	RECT				m_rectSrc, m_rectDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD				m_textureW, m_textureH;
	DWORD m_dwFourCC;


    enum RenderThreadMessage {
        MSG_NONE = 0,
        MSG_WINDOW_SET,
        MSG_RENDER_LOOP_EXIT
    };

//    pthread_t _threadId;
//    pthread_mutex_t _mutex;

    
    // android window, supported by NDK r5 and newer
    ANativeWindow* _window;

    EGLDisplay _display;
    EGLSurface _surface;
    EGLContext _context;
    int _width;
    int _height;
    GLfloat _angle;

	GLuint m_textueId;

    void renderLoop();
    static void* threadStartCallback(void *myself);

	bool initialize();
	void destroy() ;
	void drawFrame(int num);

	bool m_updated;
	int m_updatedFrrameIdx;
	int m_dwglWidth;
	int m_dwglHeight;
	float m_bytePerPixcel;
	GLenum m_glFormat;
};


CGfxEGL::CGfxEGL():
m_bOpen(false),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0)
{
	memset(&m_rectSrc, 0,sizeof(m_rectSrc));
	memset(&m_rectDst, 0,sizeof(m_rectDst));
	memset(&m_pBuff, 0,sizeof(m_pBuff));
 
 //	pthread_mutex_init(&_mutex, 0);    
	_display = 0;
	_surface = 0;
	_context = 0;
	m_updated = false;

}

CGfxEGL::~CGfxEGL()
{

//    pthread_mutex_destroy(&_mutex);
}

STDMETHODIMP CGfxEGL::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int i = 0;
	if(m_bOpen || pdwFourCC == NULL)	return E_FAIL;

	if(dwWidth==0 ||dwHeight==0)	return E_FAIL;
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;


	m_textureW = getMiValue(m_dwWidth);
	m_textureH  = getMiValue(m_dwHeight);


	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
	/*
		if(pdwFourCC[i] == MAKEFOURCC('X','R','G','B'))
		{
			m_bytePerPixcel = 4.0;
			m_glFormat = GL_RGBA;

			for(int j=0;j<EGL_BACKBUFFERS;j++)
			{
				m_pBuff[j] = (unsigned char*)malloc(m_textureW*m_textureH*m_bytePerPixcel);
				if(!m_pBuff[j])	
				{
					for(j=j-1;j>=0;j--)
						free(m_pBuff[j]);
					DP("malloc buffer failed \n");
					return E_FAIL;
				}
				memset(m_pBuff[j], 0, m_textureW*m_textureH*m_bytePerPixcel);
			}
			m_dwFourCC = pdwFourCC[i];
			break;
		}
		*/
		if(pdwFourCC[i] == MAKEFOURCC('R','8','8','8'))
		{
			m_bytePerPixcel = 3.0;
			m_glFormat = GL_RGB;
			for(int j=0;j<EGL_BACKBUFFERS;j++)
			{
				m_pBuff[j] = (unsigned char*)malloc(m_textureW*m_textureH*m_bytePerPixcel);
				if(!m_pBuff[j])	
				{
					for(j=j-1;j>=0;j--)
						free(m_pBuff[j]);
					DP("malloc buffer failed \n");
					return E_FAIL;
				}
				memset(m_pBuff[j], 0, m_textureW*m_textureH*m_bytePerPixcel);
			}
			m_dwFourCC = pdwFourCC[i];
			break;
		}

	}
	
	if(!pdwFourCC[i])
		return E_FAIL;


	DP("w:%d h:%d \n",dwWidth, dwHeight);
	DP("mi:%d,%d \n",m_textureW, m_textureH);

	_window = 0;
	_display = 0;
	_surface = 0;
	_context = 0;
//       _msg = MSG_NONE;
		
	m_updatedFrrameIdx = 0;
	m_updated = false;

//	pthread_create(&_threadId, 0, threadStartCallback, this);
	m_bOpen = true;


	DP("GfxAndroid_EGL: Open successed \n");
	return S_OK;
}
STDMETHODIMP CGfxEGL::Close()
{
	if(!m_bOpen)	return E_FAIL;
	DP("start close \n");

	// send message to render thread to stop rendering

	//pthread_mutex_lock(&_mutex);

//	_msg = MSG_RENDER_LOOP_EXIT;
	destroy();



	//pthread_mutex_unlock(&_mutex);    

//	pthread_join(_threadId, 0);



	DP("close \n");

	for(int j=0;j<EGL_BACKBUFFERS;j++)
	{
		if(m_pBuff[j])	
			free(m_pBuff[j]);
	}


	m_bOpen = false;

	DP("GfxAndroid_EGL: Close SUCCEEDED\n");
	return S_OK;
}


STDMETHODIMP CGfxEGL::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
    if(ppBuf)
	    *ppBuf = m_pBuff[num];
    if(pStride)
	    *pStride = m_textureW*m_bytePerPixcel;
    if(pPixCount)
	    *pPixCount = 1;

	return S_OK;
}

STDMETHODIMP CGfxEGL::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxEGL::Flip(LONG num, DWORD dwFlags)
{
	if(_surface)
	{
		m_updatedFrrameIdx = num;
		m_updated = true;
		drawFrame(m_updatedFrrameIdx);
		if (!eglSwapBuffers(_display, _surface)) {
		DP("eglSwapBuffers() returned error %d", eglGetError());
		}
 	}
	return S_OK;
}

STDMETHODIMP CGfxEGL::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return E_NOTIMPL;

/*
	*pdwFlags = GFXOVERLAY_SCALE_X | GFXOVERLAY_SCALE_Y;
	SetRect(pRSrcRect,0,0,m_dwWidth,m_dwHeight);
	SetRect(pDstRect,0,0,m_textureW,m_textureH);
*/
	return S_OK;

}
STDMETHODIMP CGfxEGL::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::SetDstRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxEGL::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}

STDMETHODIMP CGfxEGL::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = EGL_BACKBUFFERS;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0;
	return S_OK;

}
STDMETHODIMP CGfxEGL::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxEGL::Update()
{
	return S_OK;
}

void CGfxEGL::renderLoop()
{
    
    DP("renderLoop()");

    while (1) {
       switch (MSG_WINDOW_SET/*_msg*/) 
	{
            case MSG_WINDOW_SET:

//                initialize();

                break;

            case MSG_RENDER_LOOP_EXIT:

//                destroy();

                break;

            default:
                break;
        }

//	if(_msg == MSG_RENDER_LOOP_EXIT)
	{
		break;
	}
//        _msg = MSG_NONE;

        
        if (_display) {
            drawFrame(m_updatedFrrameIdx);
            if (!eglSwapBuffers(_display, _surface)) {
                DP("eglSwapBuffers() returned error %d", eglGetError());
            }
		usleep(5*1000);
        }
		else
			usleep(20*1000);

    }

    if (_display) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (!eglSwapBuffers(_display, _surface)) {
            DP("eglSwapBuffers() returned error %d", eglGetError());
        }
  
    }
    DP("Render loop exits");
    
    return;}

void* CGfxEGL::threadStartCallback(void *myself)
{
    CGfxEGL *renderer = (CGfxEGL*)myself;

    renderer->renderLoop();
	DP("threadStartCallback ! \n");
    pthread_exit(0);
  	DP("threadStartCallback2 ! \n");
    return 0;
}


bool CGfxEGL::initialize()
{
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };
    EGLDisplay display;
    EGLConfig config;    
    EGLint numConfigs;
    EGLint format;
    EGLSurface surface;
    EGLContext context;
    EGLint width;
    EGLint height;
    GLfloat ratio;
    
    DP("Initializing context");
    
    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        DP("eglGetDisplay() returned error %d", eglGetError());
        return false;
    }
    if (!eglInitialize(display, 0, 0)) {
        DP("eglInitialize() returned error %d", eglGetError());
        return false;
    }

    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
        DP("eglChooseConfig() returned error %d", eglGetError());
        destroy();
        return false;
    }

    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
        DP("eglGetConfigAttrib() returned error %d", eglGetError());
        destroy();
        return false;
    }

    ANativeWindow_setBuffersGeometry(_window, 0, 0, format);

    if (!(surface = eglCreateWindowSurface(display, config, _window, 0))) {
        DP("eglCreateWindowSurface() returned error %d", eglGetError());
        destroy();
        return false;
    }
    
    if (!(context = eglCreateContext(display, config, 0, 0))) {
        DP("eglCreateContext() returned error %d", eglGetError());
        destroy();
        return false;
    }
    
    if (!eglMakeCurrent(display, surface, surface, context)) {
        DP("eglMakeCurrent() returned error %d", eglGetError());
        destroy();
        return false;
    }

    if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(display, surface, EGL_HEIGHT, &height)) {
        DP("eglQuerySurface() returned error %d", eglGetError());
        destroy();
        return false;
    }

    _display = display;
    _surface = surface;
    _context = context;
    _width = width;
    _height = height;


    glDisable(GL_DITHER);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glClearColor(0, 0, 0, 0);
//    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

	glEnable (GL_TEXTURE_2D);
	glGenTextures (1, &m_textueId);
	glBindTexture (GL_TEXTURE_2D, m_textueId);
	glTexImage2D(GL_TEXTURE_2D, 0,m_glFormat, m_textureW, m_textureH, 0, m_glFormat,GL_UNSIGNED_BYTE, m_pBuff[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    glViewport(0, 0, width, height);

    ratio = (GLfloat) width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-ratio, ratio, -1, 1, 1, 10);

    DP("Initializing context SUCCESSED2");

    return true;
}

void CGfxEGL::destroy() {
    DP("Destroying context");

    eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(_display, _context);
    eglDestroySurface(_display, _surface);
    eglTerminate(_display);
    
    _display = EGL_NO_DISPLAY;
    _surface = EGL_NO_SURFACE;
    _context = EGL_NO_CONTEXT;
    _width = 0;
    _height = 0;
	if(_window)
	{
		ANativeWindow_release(_window);
		_window = 0;
	}

    return;
}

void CGfxEGL::drawFrame(int num)
{
#if 0//DUMP_FPS
	static int g_frames = 0;
	static DWORD g_startTick = 0;
	if(g_frames == 0)
	{
		 g_startTick = GetTickCount();
	}
	g_frames ++;

	if(g_frames%10 == 0)
		DP("DISP FPS:%d \n",g_frames*1000/(GetTickCount() -g_startTick )
	);
#endif
/*
	eglQuerySurface(_display, _surface, EGL_WIDTH, &_width);
	eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);


	glViewport(0, 0, _width, _height);
*/
	GLfloat  ratio = (GLfloat) _width / _height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumf(-ratio, ratio, -1, 1, 1, 10);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float texcoord[2*4] = {
	0, 1.0f,
	0, 0, 
	1.0f, 0,
	1.0f, 1.0f
	};

	glBindTexture (GL_TEXTURE_2D, m_textueId);

//	if(m_updated)
	{
		glTexImage2D( GL_TEXTURE_2D, 0, m_glFormat,  m_textureW, m_textureH, 0, 
			m_glFormat, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D (GL_TEXTURE_2D, 0, 0,0, m_textureW,m_textureH, 
			m_glFormat, GL_UNSIGNED_BYTE, m_pBuff[num]);
//		m_updated = false;
	}

	int rectCrop[4] = {0, m_dwHeight, m_dwWidth, m_dwHeight*(-1.0f)}; 
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, rectCrop);

	glDrawTexiOES(0, 0, 0, _width, _height);
}


IMcGfxOverlay *CreateEGL()
{
	return new CGfxEGL();
}
#endif
