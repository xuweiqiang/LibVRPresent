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

#ifdef GFX_ANDROID_ANativeWindow

#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"
#include "../../inc/GfxPresentPrepare.h"

#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>


#include "ci_codec_type.h"
#include "ci_imageproc.h"

#include <android/log.h>

#include <android/native_window_jni.h>

struct fields_t 
{
	JavaVM *    jvm;
	jobject 	jniObj;
	jfieldID    context;
	jfieldID    surface;
	jfieldID    surface_native;
	jmethodID   post_event;
	jfieldID snapshotPath;
};


class CAutoAttachThread
{
public:
	CAutoAttachThread(JavaVM * javaVM) : m_javaVM(javaVM), m_needDetach(false)
	{
	}

	JNIEnv * GetEnv()
	{
		JNIEnv* env;
		m_needDetach = false;
		m_javaVM->GetEnv((void**) &env, JNI_VERSION_1_6);
		if (env == NULL)
		{
			m_needDetach = true;
			int status = m_javaVM->AttachCurrentThread(&env, NULL);
			if (status < 0)
			{
				DP("Attach Current thread failed!! Error %d", status);
			}
		}
		return env;
	}

	~CAutoAttachThread()
	{
		if (m_needDetach)
		{
			m_javaVM->DetachCurrentThread();
		}
	}

private:
	JavaVM * m_javaVM;
	bool m_needDetach;
};
class CGfxAndroidANativeWindow : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidANativeWindow();
	virtual ~CGfxAndroidANativeWindow();
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

	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
	  	if(!m_bOpen)	
        	return E_FAIL;
        
		if(pSurf == NULL)
		{
			DP("SetHWRenderSurf Invalid params \n");
			return E_FAIL;
		}

		VR_ANDROID_ISUR_PARAM*	surface = reinterpret_cast<VR_ANDROID_ISUR_PARAM*>(pSurf);

		DP("SetHWRenderSurf jvm:%p jobect:%p \n",surface->jvm, surface->pAndroidSFBuf);

		if((mode == HW_RENDER_ANDROID_ANATIVEWINDOW) || (mode == 0))
		{
			CAutoAttachThread attach((JavaVM *)surface->jvm);
			JNIEnv* env = attach.GetEnv();

			m_NativeWindow = (ANativeWindow *)ANativeWindow_fromSurface(env, (jobject)surface->pAndroidSFBuf);

//	    	m_NativeWindow = (ANativeWindow*)surface->pAndroidSFBuf;
			DP("SetHWRenderSurf ANativeWindow:%p \n",m_NativeWindow);
		}
		else if(mode == HW_RENDER_ANDROID_SURFACE)
		{
			if(surface->jvm == 0 || surface->pAndroidSFBuf == 0)
				return E_FAIL;

			JNIEnv* env;
			m_fields.jvm = (JavaVM*)surface->jvm;
  			 int status = m_fields.jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
  			 if(status <0)    
  			 {
				DP("SetHWRenderSurf GetEnv failed \n");
				return E_FAIL;
			}	

			m_fields.jniObj = env->NewGlobalRef((jobject)surface->pAndroidSFBuf);
			if(m_fields.jvm->AttachCurrentThread(&env, NULL)<0)
  			 {
				DP("SetHWRenderSurf AttachCurrentThread failed \n");
				return E_FAIL;
  			 }

	     	m_NativeWindow = ANativeWindow_fromSurface(env, m_fields.jniObj);
			DP("SetHWRenderSurf mode:%d, win:%p \n",mode, m_NativeWindow);
		}	
		else
		{
			DP("SetHWRenderSurf Invalid params mode =%p \n",mode);
			return E_FAIL;
		}	

    	ANativeWindow_acquire(m_NativeWindow);

    	if(m_dwFourCC  == MAKEFOURCC('Y','V','1','2'))
    	{
			m_fmt = WINDOW_FORMAT_RGB_565;
    	}
    	else if(m_dwFourCC  == MAKEFOURCC('R','5','6','5'))
    	{
			m_fmt = WINDOW_FORMAT_RGB_565;
    	}
    	else if(m_dwFourCC  == MAKEFOURCC('R','G','B','A'))
    	{
			m_fmt = WINDOW_FORMAT_RGBX_8888;
    	}

    	int32_t  ret = ANativeWindow_setBuffersGeometry(m_NativeWindow, m_dwWidth, m_dwHeight, m_fmt );

        DP("SetHWRenderSurf Out\n");
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
	STDMETHODIMP UIFlip(LPVOID pHandle);
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}

	DWORD				m_dwWidth, m_dwHeight;
	CRITICAL_SECTION m_csDisplay; 

protected:
	STDMETHODIMP Update();

	bool m_bOpen;
	DWORD m_dwFourCC;
	bool m_bFirstTime;

	RECT m_rDst;
	VP_ZOOM_MODE m_zoomMode;
	GfxPrepareDataV2 m_PrepareData;
	bool m_bshow;
	int m_skipframes;
	int surfWidth;
	int surfHeight;
	ANativeWindow* m_NativeWindow;
	ANativeWindow_Buffer m_NativeOutBuffer;
   	int32_t m_fmt;
   	fields_t m_fields;
};


CGfxAndroidANativeWindow::CGfxAndroidANativeWindow():
m_bOpen(false),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0),
m_bFirstTime(true)
{
	m_NativeWindow = 0;
	InitializeCriticalSection(&m_csDisplay);
}

CGfxAndroidANativeWindow::~CGfxAndroidANativeWindow()
{
	Close();
  	DeleteCriticalSection(&m_csDisplay);
}


STDMETHODIMP CGfxAndroidANativeWindow::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int i = 0;
#ifdef DEDIPROG
	m_skipframes = 5;
#else
	m_skipframes = 0;
#endif
	if(m_bOpen || pdwFourCC == NULL)	
	{
		DP("OPEN Invalid params \n");
		return E_FAIL;
	}

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	SetRect(&m_rDst,0,0,m_dwWidth,m_dwHeight);
  	m_bshow = true;

	DP("CGfxAndroidANativeWindow::Open - frame size %dx%d \n", dwWidth, dwHeight);

	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
		// if(pdwFourCC[i] == MAKEFOURCC('R','G','B','A'))
		// {
		// 	break;
		// }
		if(pdwFourCC[i] == MAKEFOURCC('R','5','6','5'))
		{
			break;
		}
	}

	if(!pdwFourCC[i])
		return E_FAIL;

	memset(&m_fields, 0 ,sizeof(m_fields));

	m_dwFourCC = pdwFourCC[i];
	m_NativeWindow = 0;
	m_bOpen = true;
	DP("CGfxAndroidANativeWindow::Open - OK m_dwFourCC:%p\n",m_dwFourCC);
	return S_OK;
}

STDMETHODIMP CGfxAndroidANativeWindow::Close()
{
	if(!m_bOpen)	
	{
		return E_FAIL;
	}

	if(m_NativeWindow)
	{
		ANativeWindow_release(m_NativeWindow);
		m_NativeWindow = 0;
	}

	if(m_fields.jvm)
		m_fields.jvm->DetachCurrentThread();


	m_bOpen = false;
	return S_OK;
}


STDMETHODIMP CGfxAndroidANativeWindow::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(!m_bOpen) 
	    return E_FAIL;
	if(!m_NativeWindow)
		return E_FAIL;

	int32_t ret = ANativeWindow_lock(m_NativeWindow, &m_NativeOutBuffer,0);

	if(ppBuf)
		*ppBuf = (unsigned char *)m_NativeOutBuffer.bits;
	if(pStride)
		*pStride = m_NativeOutBuffer.stride*2;
	if(pPixCount)
		*pPixCount = m_NativeOutBuffer.stride/m_NativeOutBuffer.width;
	return S_OK;
}

STDMETHODIMP CGfxAndroidANativeWindow::Unlock(LONG num)
{
	if(!m_bOpen) 
	    return E_FAIL;

	if(!m_NativeWindow)
		return E_FAIL;

	ANativeWindow_unlockAndPost(m_NativeWindow);
	return S_OK;
}

STDMETHODIMP CGfxAndroidANativeWindow::Flip(LONG num, DWORD dwFlags)
{
	if(!m_bOpen) 
	    return E_FAIL;

	return S_OK;
}

STDMETHODIMP CGfxAndroidANativeWindow::UIFlip(LPVOID pHandle)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidANativeWindow::Show(BOOL bShow)
{
	EnterCriticalSection(&m_csDisplay);
	m_bshow = bShow;
	LeaveCriticalSection(&m_csDisplay);
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
 	return E_NOTIMPL;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetSrcRect(const RECT *pRect)
{
 	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetDstRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::GetScnRect(RECT *pRect)
{
    return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidANativeWindow::SetWindowHandle(HWND hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidANativeWindow::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxAndroidANativeWindow::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidANativeWindow::Update()
{
	return S_OK;
}


IMcGfxOverlay *CreateOverlayAndroidANativeWindow()
{
	return new CGfxAndroidANativeWindow();
}

#endif
