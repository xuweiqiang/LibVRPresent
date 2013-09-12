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

#ifdef GFX_ANDROID_SURFACE

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
#include <utils/threads.h>
#include "utils/Errors.h"  // for status_t

#include "jni.h"

#ifdef ANDROID21
#include <ui/Surface.h>
#elif defined(ANDROID22) ||defined(ANDROID23)
#include <surfaceflinger/Surface.h>
#else
#include <Surface.h>
#endif

#include "ci_codec_type.h"
#include "ci_imageproc.h"

using namespace android;

#ifdef RENESAS_ANDROID
#define FORCE_SCALLING_TO_FULLSURFACE
#endif

int RenderThreadAndroid(void * param);

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

class CGfxAndroidSurface : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidSurface();
	virtual ~CGfxAndroidSurface();
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

      DP("SetHWRenderSurf In mode :%d \n",mode );
      if(mode ==0)
		{	
		VR_ANDROID_ISUR_PARAM * surfParams = (VR_ANDROID_ISUR_PARAM *)pSurf;
		if(surfParams->pAndroidSFBuf == m_surface)
			return S_OK;
		EnterCriticalSection(&m_csDisplay);

		m_surface = (Surface *)(surfParams->pAndroidSFBuf);
		DP(">>>> Surface =%p \n", m_surface);

		if(m_surface == NULL)
		{
			DP("CGfxAndroidSurface::Set Surface == NULL\n");
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}


		status_t ret = m_surface->lock(&m_surInfo);
		if(ret == 0)
		{
			memset(m_surInfo.bits, 0 , m_surInfo.s*m_surInfo.h*2);
		}
		m_surface->unlockAndPost();

		LeaveCriticalSection(&m_csDisplay);
	}
      else if(mode ==1)
      {
        m_surfParam = *(VR_ANDROID_SURF_PARAM *)pSurf;

        if (!createThread(RenderThreadAndroid, this))
        {
          DP("Render thread create failed \n");
          return E_FAIL;
        }
        Sleep(100);
      }
		else if(mode == HW_RENDER_ANDROID_SURFACE)
		{
			VR_ANDROID_ISUR_PARAM*	surfaceInput = reinterpret_cast<VR_ANDROID_ISUR_PARAM*>(pSurf);

			if(surfaceInput->jvm == 0 || surfaceInput->pAndroidSFBuf == 0)
				return E_FAIL;

			JNIEnv* env;
			m_fields.jvm = (JavaVM*)surfaceInput->jvm;
  			 int status = m_fields.jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
  			 if(status <0)    
  			 {
				DP("SetHWRenderSurf GetEnv failed \n");
				return E_FAIL;
			}	

			m_fields.jniObj = env->NewGlobalRef((jobject)surfaceInput->pAndroidSFBuf);
			if(m_fields.jvm->AttachCurrentThread(&env, NULL)<0)
  			 {
				DP("SetHWRenderSurf AttachCurrentThread failed \n");
				return E_FAIL;
  			 }

			jclass clazz;
			clazz = env->FindClass("cidana/media/MediaPlayer");
			if (clazz == NULL) {
				LOGE("class cidana/media/MediaPlayer not found");
				return -1;
			}

			m_fields.surface = env->GetFieldID(clazz, "mSurface", "Landroid/view/Surface;");
			if (m_fields.surface == NULL) {
				LOGE("Can't find mSurface in MediaPlayer");
				return -1;
			}

			jclass surface = env->FindClass("android/view/Surface");
			if (surface == NULL) {
				LOGE("Can't find android/view/Surface");
				return -1;
			}
			m_fields.surface_native = env->GetFieldID(surface, "mSurface", "I");
			if (m_fields.surface_native == NULL)
			{
				m_fields.surface_native = env->GetFieldID(surface, "mNativeSurface", "I");
				if (m_fields.surface_native == NULL)
				{
					LOGE("Can't find Native Surface field from Surface class");
					return -1;
				}
			}

			m_surface = (Surface*)env->GetIntField(m_fields.jniObj, m_fields.surface_native);

		}
	surfWidth = 0;
	surfHeight = 0;

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

	VR_ANDROID_SURF_PARAM m_surfParam;
	DWORD				m_dwWidth, m_dwHeight;
	CRITICAL_SECTION m_csDisplay; 
	Surface * m_surface;

protected:
	STDMETHODIMP Update();

	bool m_bOpen;
	DWORD m_dwFourCC;
	bool m_bFirstTime;

	LPBYTE m_pBuf;
	RECT m_rDst;
	VP_ZOOM_MODE m_zoomMode;
	Surface::SurfaceInfo m_surInfo;
	GfxPrepareDataV2 m_PrepareData;
	bool m_bshow;
	int m_skipframes;
	int surfWidth;
	int surfHeight;
   	fields_t m_fields;

};


CGfxAndroidSurface::CGfxAndroidSurface():
m_bOpen(false),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0),
m_bFirstTime(true),
m_surface(NULL),
m_pBuf(0)
{
//	memset(&m_screenRect, 0, sizeof(m_screenRect));
//	memset(&m_rectDst, 0,sizeof(m_rectDst));
    memset(&m_surInfo, 0,sizeof(m_surInfo));
	InitializeCriticalSection(&m_csDisplay);
}

CGfxAndroidSurface::~CGfxAndroidSurface()
{
	Close();
  Sleep(100);
	DeleteCriticalSection(&m_csDisplay);
}


STDMETHODIMP CGfxAndroidSurface::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int i = 0;
#ifdef DEDIPROG
	m_skipframes = 5;
#else
	m_skipframes = 0;
#endif
	DP("=======Android surface Mode!!!!");
	if(m_bOpen || pdwFourCC == NULL)	
	{
		DP("OPEN Invalid params \n");
		return E_FAIL;
	}

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	SetRect(&m_rDst,0,0,m_dwWidth,m_dwHeight);
  	m_bshow = true;

    memset(&m_surfParam, 0,sizeof(m_surfParam));
	DP("CGfxAndroidSurface::Open - frame size %dx%d \n", dwWidth, dwHeight);

	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
		if(pdwFourCC[i] == MAKEFOURCC('Y','V','1','2'))
		{
			DP("CGfxAndroidSurface::Open - alloc buffer\n");
			break;
		}
	}

	if(!pdwFourCC[i])
	{
		return E_FAIL;
	}

	m_pBuf = (unsigned char* )malloc(m_dwWidth *m_dwHeight*3/2);
	if(!m_pBuf)
	{
		DP("error!!!! android surface malloc failed \n");
        return E_FAIL;
	}
	memset(m_pBuf, 0x0 , m_dwWidth*m_dwHeight);
	memset(m_pBuf +m_dwWidth*m_dwHeight, 0x80 , m_dwWidth*m_dwHeight/2);

	memset(&m_fields, 0 ,sizeof(m_fields));
	m_dwFourCC = pdwFourCC[i];
	m_bOpen = true;
	DP("CGfxAndroidSurface::Open - OK\n");
	return S_OK;
}

STDMETHODIMP CGfxAndroidSurface::Close()
{
	if(!m_bOpen)	
	{
		DP("Close() -- already opened\n");
		return E_FAIL;
	}

	if(m_fields.jvm)
		m_fields.jvm->DetachCurrentThread();


	if(m_pBuf)
	{
		free(m_pBuf);
		m_pBuf = NULL;
	}
	m_surface = 0;
	m_bOpen = false;
	return S_OK;
}


STDMETHODIMP CGfxAndroidSurface::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(!m_bOpen) 
	    return E_FAIL;

	if(ppBuf)
		*ppBuf = (unsigned char *)m_pBuf;
	if(pStride)
		*pStride = m_dwWidth;
	if(pPixCount)
		*pPixCount = 3/2;
	return S_OK;
}

STDMETHODIMP CGfxAndroidSurface::Unlock(LONG num)
{
	if(!m_bOpen) 
	    return E_FAIL;

	return S_OK;
}

STDMETHODIMP CGfxAndroidSurface::Flip(LONG num, DWORD dwFlags)
{
	if(!m_bOpen) 
	    return E_FAIL;

	if(m_surface == 0 || m_pBuf == 0)
	{
		DP("Flip Invalid params %d, %d\n", m_surface, m_pBuf);
		return E_FAIL;
	}

    if(!m_bshow)
      return S_OK;

	if(m_skipframes)
	{
		m_skipframes--;
		return S_OK;
	}
    DWORD tick[5];
EnterCriticalSection(&m_csDisplay);

      tick[0] = GetTickCount();

 	status_t ret = m_surface->lock(&m_surInfo);
      tick[1] = GetTickCount();
	if(ret == 0)
	{
	if((surfWidth != m_surInfo.w)||(surfHeight != m_surInfo.h))
	{
      DP("[CGfxAndroidSurface] surface changed from %d,%d -> %d,%d \n",surfWidth  ,surfHeight , m_surInfo.w,m_surInfo.h);
	DP("[CGfxAndroidSurface] surface format:%d \n",m_surInfo.format);

      surfWidth = m_surInfo.w;
      surfHeight = m_surInfo.h;
	}
	  RECT pFull ;
	  SetRect(&pFull ,0,0,m_surInfo.w, m_surInfo.h);
		if(m_surInfo.w == m_dwWidth && m_surInfo.h == m_dwHeight)
		{
			RECT pInterdst;
			IntersectRect(&pInterdst,&m_rDst,&pFull);

			memset(m_surInfo.bits, 0 , m_surInfo.s*m_surInfo.h*2);
			LPBYTE srcBuff[3] = {m_pBuf, m_pBuf + m_dwWidth*m_dwHeight*5/4,m_pBuf + m_dwWidth*m_dwHeight};
			DWORD srcStride[3] = {m_dwWidth, m_dwWidth/2,m_dwWidth/2};
			RECT rSrc = {0,0,m_dwWidth, m_dwHeight};

	//		memset(m_pBuf, rand(), m_dwWidth*m_dwHeight*3/2);
			HRESULT hr = m_PrepareData.PrepareBufferRGB565Scaled((LPBYTE)m_surInfo.bits, m_surInfo.s*2
#ifdef FORCE_SCALLING_TO_FULLSURFACE
				, srcBuff, srcStride, &rSrc, &pFull, dwFlags, 0,VP_DEINTERLACE_None);
#else
				, srcBuff, srcStride, &rSrc, &pInterdst, dwFlags, 0,VP_DEINTERLACE_None);
#endif
			if(FAILED(hr))	DP("Preparedata failed hr:%d \n",hr);

		}
		else
		{
			DP("SetFixedSize is not set properly !!!!! \n");
		}
      tick[2] = GetTickCount();

		m_surface->unlockAndPost();
      tick[3] = GetTickCount();
	}
  LeaveCriticalSection(&m_csDisplay);


#ifdef DUMP_FPS
if(tick[3]-tick[0] > 30)
    {
      DP("Flip() too slow:%d msec \n",tick[3] - tick[0]);
      DP("    lock cost:%d msec \n",tick[1] - tick[0]);
      DP("    copy cost:%d msec \n",tick[2] - tick[1]);
      DP("    unlock cost:%d msec \n",tick[3] - tick[2]);
    }
#endif
	return S_OK;
}

STDMETHODIMP CGfxAndroidSurface::UIFlip(LPVOID pHandle)
{
//	LOGI("CGfxAndroidSurface::UIFlip");
	return S_OK;
}

STDMETHODIMP CGfxAndroidSurface::Show(BOOL bShow)
{
  EnterCriticalSection(&m_csDisplay);
  m_bshow = bShow;
  LeaveCriticalSection(&m_csDisplay);
 	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
 	return E_NOTIMPL;
}
STDMETHODIMP CGfxAndroidSurface::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::SetSrcRect(const RECT *pRect)
{
 	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::SetDstRect(const RECT *pRect)
{
	return S_OK;
/*
  EnterCriticalSection(&m_csDisplay);
//	m_rDst = *pRect;
//  DP ("[CGfxAndroidSurface] SetDest %d,%d \n",m_rDst.right - m_rDst.left, m_rDst.bottom - m_rDst.top);

  LeaveCriticalSection(&m_csDisplay);
  Flip(0,0);
  return S_OK;
*/
}
STDMETHODIMP CGfxAndroidSurface::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::GetScnRect(RECT *pRect)
{
	if(!m_bOpen) 
	    return E_FAIL;
#ifdef ANDROID40
  SetRect(pRect, 0, 0, m_dwWidth,m_dwHeight);
#else

  if(m_surInfo.w == 0 || m_surInfo.h ==0)
    return E_FAIL;

  SetRect(pRect, 0, 0, m_surInfo.w,m_surInfo.h);
#endif

  return S_OK;
}

STDMETHODIMP CGfxAndroidSurface::SetWindowHandle(HWND hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidSurface::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = 0;
	pInfo->dwFourCC = MAKEFOURCC('Y','V','1','2');
	pInfo->dwCaps = 0;
	return S_OK;

}

STDMETHODIMP CGfxAndroidSurface::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSurface::Update()
{
	return S_OK;
}

int RenderThreadAndroid(void * param)
{
  CGfxAndroidSurface* pThis = (CGfxAndroidSurface*)param;
  DP("%s l:%d JVM: %p, holder:%p\n",__FUNCTION__, __LINE__,
  pThis ->m_surfParam.m_pJvm,pThis ->m_surfParam.m_holder);
   EnterCriticalSection(&(pThis->m_csDisplay));

   /*      SET FIXED SIZE  */
  JNIEnv *env = NULL;
  int status = pThis->m_surfParam.m_pJvm->GetEnv((void**)&env, JNI_VERSION_1_4);
  if(status <0)    
  {    
    DP("error %d \n",__LINE__);			    
    LeaveCriticalSection(&(pThis->m_csDisplay));
    return E_FAIL;   
  }

 
  if(pThis->m_surfParam.m_pJvm->AttachCurrentThread(&env, NULL)<0)
  {    
    DP("error %d \n",__LINE__);  			    
    LeaveCriticalSection(&(pThis->m_csDisplay));
    return E_FAIL;   
  }

 
  jclass interfaceClass = env->GetObjectClass(pThis->m_surfParam.m_holder);
  if(interfaceClass == 0)
  {    
    DP("error %d \n",__LINE__);			    
    LeaveCriticalSection(&(pThis->m_csDisplay));
    return E_FAIL;   
  }

 /*
  
  jmethodID method = env->GetMethodID(interfaceClass, "setFixedSize", "(II)V");
  if(method == 0)
  {
    DP("error %d \n",__LINE__);			
    LeaveCriticalSection(&(pThis->m_csDisplay));
    return E_FAIL;   

  }
  env->CallVoidMethod(pThis->m_surfParam.m_holder,method,pThis->m_dwWidth,pThis->m_dwHeight);
  */

   /*      GET surface */
  jmethodID method = env->GetMethodID(interfaceClass, "getSurface", "()Landroid/view/Surface;");
  if(method == 0)
  {    
    DP("error %d \n",__LINE__);        
    LeaveCriticalSection(&(pThis->m_csDisplay));
    return E_FAIL;   
  }


   jobject surfaceObj = env->CallObjectMethod(pThis->m_surfParam.m_holder, method);
 
  jclass surface_class = env->FindClass("android/view/Surface");
  if (surface_class == NULL) 
  {
    DP("Can't find android/view/Surface");    
    LeaveCriticalSection(&(pThis->m_csDisplay));
    return E_FAIL;   
  }


 #if ANDROID_VER==22
  jfieldID    surface_native = env->GetFieldID(surface_class, "mSurface", "I");
#else
  jfieldID    surface_native = env->GetFieldID(surface_class, "mNativeSurface", "I");
#endif
   if (surface_native == NULL) 
  {
 #if ANDROID_VER==22
    DP("Can't find Surface.mSurface");        
#else
    DP("Can't find Surface.mNativeSurface");        
#endif
    LeaveCriticalSection(&(pThis->m_csDisplay));
    return E_FAIL;   
  }

   pThis->m_surface = (Surface*)env->GetIntField(surfaceObj, surface_native);
  DP(">>>> Surface =%p \n", pThis->m_surface);
 LeaveCriticalSection(&(pThis->m_csDisplay));
  return 0;
}

IMcGfxOverlay *CreateOverlayAndroidSurface()
{
	return new CGfxAndroidSurface();
}

#endif
