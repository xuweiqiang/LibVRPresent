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

#ifdef GFX_ANDROID_OPENGL_ES

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

#define LINUX
#include <stdlib.h>
#include <dlfcn.h>
static void *sGLESSO = NULL;

#include <android/log.h>

#define Float2Fixed(fl) ((GLfixed)((fl)*65536.0f))
#undef FN
#define FNPTR(name) funcPtr_##name

#define IMPORTGL_NO_FNPTR_DEFS
#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL
#include "importgl.h"


int importGLInit()
{
    int result = 1;

#ifndef DISABLE_IMPORTGL

#undef IMPORT_FUNC

#ifdef WIN32
    sGLESDLL = LoadLibrary(_T("libGLES_CM.dll"));
    if (sGLESDLL == NULL)
        sGLESDLL = LoadLibrary(_T("libGLES_CL.dll"));
    if (sGLESDLL == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite DLL.

    /* The following fetches address to each egl & gl function call
     * and stores it to the related function pointer. Casting through
     * void * results in warnings with VC warning level 4, which
     * could be fixed by casting to the true type for each fetch.
     */
#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)GetProcAddress(sGLESDLL, _T(#funcName)); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // WIN32

#ifdef LINUX
#ifdef ANDROID_NDK
    sGLESSO = dlopen("libGLESv1_CM.so", RTLD_NOW);
#else /* !ANDROID_NDK */
    sGLESSO = dlopen("libGLES_CM.so", RTLD_NOW);
    if (sGLESSO == NULL)
        sGLESSO = dlopen("libGLES_CL.so", RTLD_NOW);
#endif /* !ANDROID_NDK */
    if (sGLESSO == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite SO.

#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)dlsym(sGLESSO, #funcName); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // LINUX

#ifndef ANDROID_NDK
    IMPORT_FUNC(eglChooseConfig);
    IMPORT_FUNC(eglCreateContext);
    IMPORT_FUNC(eglCreateWindowSurface);
    IMPORT_FUNC(eglDestroyContext);
    IMPORT_FUNC(eglDestroySurface);
    IMPORT_FUNC(eglGetConfigAttrib);
    IMPORT_FUNC(eglGetConfigs);
    IMPORT_FUNC(eglGetDisplay);
    IMPORT_FUNC(eglGetError);
    IMPORT_FUNC(eglInitialize);
    IMPORT_FUNC(eglMakeCurrent);
    IMPORT_FUNC(eglSwapBuffers);
    IMPORT_FUNC(eglTerminate);
#endif /* !ANDROID_NDK */

    IMPORT_FUNC(glBlendFunc);
    IMPORT_FUNC(glClear);
    IMPORT_FUNC(glClearColorx);
    IMPORT_FUNC(glColor4x);
    IMPORT_FUNC(glColorPointer);
    IMPORT_FUNC(glDisable);
    IMPORT_FUNC(glDisableClientState);
    IMPORT_FUNC(glDrawArrays);
    IMPORT_FUNC(glEnable);
    IMPORT_FUNC(glEnableClientState);
    IMPORT_FUNC(glFrustumx);
    IMPORT_FUNC(glGetError);
    IMPORT_FUNC(glLightxv);
    IMPORT_FUNC(glLoadIdentity);
    IMPORT_FUNC(glMaterialx);
    IMPORT_FUNC(glMaterialxv);
    IMPORT_FUNC(glMatrixMode);
    IMPORT_FUNC(glMultMatrixx);
    IMPORT_FUNC(glNormalPointer);
    IMPORT_FUNC(glPopMatrix);
    IMPORT_FUNC(glPushMatrix);
    IMPORT_FUNC(glRotatex);
    IMPORT_FUNC(glScalex);
    IMPORT_FUNC(glShadeModel);
    IMPORT_FUNC(glTranslatex);
    IMPORT_FUNC(glVertexPointer);
    IMPORT_FUNC(glViewport);

#endif /* DISABLE_IMPORTGL */

    return result;
}


static long
_getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}


void importGLDeinit()
{
#ifndef DISABLE_IMPORTGL
#ifdef WIN32
    FreeLibrary(sGLESDLL);
#endif

#ifdef LINUX
    dlclose(sGLESSO);
#endif
#endif /* DISABLE_IMPORTGL */
}

class CGfxAndroidOGL : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidOGL();
	virtual ~CGfxAndroidOGL();
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
	STDMETHODIMP SetWindowHandle(Window hwnd)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetWindowHandle(Window& hwnd)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf)
	{
		return E_NOTIMPL;
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
	STDMETHODIMP UIFlip(LPVOID pHandle);
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}
    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
		return E_FAIL;
	}


protected:
	STDMETHODIMP Update();
	STDMETHODIMP CalculateGLStride(DWORD input, DWORD* pOut);

	bool m_bOpen;
	unsigned char *m_pBuff;
	unsigned char *m_pBuff2;
	unsigned char *m_pRGBBuff;
	RECT				m_rectSrc, m_rectDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD m_dwFourCC;
	bool m_bFirstTime;
	GLuint m_textueId;
	bool m_lockFlag;
};


CGfxAndroidOGL::CGfxAndroidOGL():
m_bOpen(false),
m_pBuff(0),
m_pBuff2(0),
m_pRGBBuff(0),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0),
m_bFirstTime(true),
m_lockFlag(false)
{
	memset(&m_rectSrc, 0,sizeof(m_rectSrc));
	memset(&m_rectDst, 0,sizeof(m_rectDst));
	importGLInit	();
}

CGfxAndroidOGL::~CGfxAndroidOGL()
{
	importGLDeinit();
}

STDMETHODIMP CGfxAndroidOGL::CalculateGLStride(DWORD input, DWORD* pOut)
{
	if(!pOut ||input>1024 )	return E_FAIL;
	if(input >512)	*pOut = 512<<1;
	else if(input >512>>1) 	*pOut = 512;
	else if(input >512>>2) 	*pOut = 512>>1;
	else if(input >512>>3) 	*pOut = 512>>2;
	else if(input >512>>4) 	*pOut = 512>>3;
	else 
		*pOut = 512>>4;
	
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGL::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int i = 0;
	if(m_bOpen || pdwFourCC == NULL)	return E_FAIL;
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	//CalculateGLStride(m_dwWidth, &m_glWidth);	
	//CalculateGLStride(m_dwHeight, &m_glHeight);


	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
		if(pdwFourCC[i] == MAKEFOURCC('R','8','8','8'))
		{
			m_pBuff = (unsigned char * )malloc(m_dwWidth  *m_dwHeight *4);
			m_pBuff2 = (unsigned char * )malloc(m_dwWidth  *m_dwHeight *4);
			memset(m_pBuff, 0,m_dwWidth *m_dwHeight*2);
			break;
		}

	}
	
	if(!pdwFourCC[i])
		return E_FAIL;


	__android_log_print(ANDROID_LOG_ERROR, "VRP", "Open (%d,%d)", dwWidth, dwHeight);

	if(!m_pBuff )	return E_FAIL;

	m_dwFourCC = pdwFourCC[i];
	m_bOpen = true;
	m_bFirstTime = 1;
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::Close()
{
	if(!m_bOpen)	return E_FAIL;

	if(m_pBuff)
	{
		free(m_pBuff);
		m_pBuff = NULL;
	}
	
	if(m_pBuff2)
	{
		free(m_pBuff2);
		m_pBuff2 = NULL;
	}


	m_bOpen = false;
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGL::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	m_lockFlag = true;

	    if(ppBuf)
		    *ppBuf = m_pBuff;
	    if(pStride)
		    *pStride = m_dwWidth*3;
	    if(pPixCount)
		    *pPixCount = 12;

	return S_OK;
}

STDMETHODIMP CGfxAndroidOGL::Unlock(LONG num)
{
	m_lockFlag = false;
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGL::Flip(LONG num, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGL::UIFlip(LPVOID pHandle)
{

	while(m_lockFlag)
	{
		Sleep(1);
	}

	if(m_bFirstTime)
	{
		m_bFirstTime = 0;
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);
		glEnable (GL_TEXTURE_2D);

		glGenTextures (1, &m_textueId);
		glBindTexture (GL_TEXTURE_2D, m_textueId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,  m_dwWidth , m_dwHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		     
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glShadeModel (GL_FLAT);
		glDisable (GL_LINE_SMOOTH);
		glDisable (GL_POINT_SMOOTH);
	
	}

       glBindTexture (GL_TEXTURE_2D, m_textueId);

	int i=0;
	for(i=0;i<m_dwHeight/2;i++)
	{
//		memcpy(m_pBuff2+ m_dwWidth*3*m_dwHeight/2- i*m_dwWidth*3, m_pBuff +i*2*m_dwWidth*3, m_dwWidth*3);	
		memcpy(m_pBuff2+i*m_dwWidth*3, m_pBuff +i*2*m_dwWidth*3, m_dwWidth*3);	
	}
 	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,  m_dwWidth , m_dwHeight/2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexSubImage2D (GL_TEXTURE_2D, 0, 0,0, m_dwWidth ,m_dwHeight/2, GL_RGB, GL_UNSIGNED_BYTE, m_pBuff2);

	int rectCrop[4] = {0, 0, m_dwWidth, m_dwHeight/2}; 
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, rectCrop);

	glDrawTexiOES(0, 0, 0, 800, 480);

	GLenum glRet = glGetError();
	__android_log_print(ANDROID_LOG_ERROR, "VRP", "UIFlip gl status %d ", glRet);

	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetDstRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGL::SetWindowHandle(HWND hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = 0;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0;
	return S_OK;

}
STDMETHODIMP CGfxAndroidOGL::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGL::Update()
{
	return S_OK;
}


IMcGfxOverlay *CreateOverlayAndroidOGL()
{
	return new CGfxAndroidOGL();
}
#endif
