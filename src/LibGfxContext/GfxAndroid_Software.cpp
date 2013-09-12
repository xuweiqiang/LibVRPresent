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

#ifdef GFX_ANDROID_SOFTWARE

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

#include <utils/Log.h>

#include <media/stagefright/openmax/OMX_IVCommon.h>
#include <utils/RefBase.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryHeapPmem.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/MetaData.h>
#include <ui/android_native_buffer.h>
#include <ui/GraphicBufferMapper.h>
#include <gui/ISurfaceTexture.h>

#include "ci_codec_type.h"
#include "ci_imageproc.h"

using namespace android;

#ifdef RENESAS_ANDROID
#define FORCE_SCALLING_TO_FULLSURFACE
#endif

class CGfxAndroidSoftware : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidSoftware();
	virtual ~CGfxAndroidSoftware();
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

	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf, int mode)
	{
		int err = 0, usage = 0;
		int bufWidth, bufHeight;
		sp<ANativeWindow>window;
		
		DP("CGfxAndroidSoftware::SetHWRenderSurf start \n");
	  	if(!m_bOpen)	
	  	{
			DP("CGfxAndroidSoftware::SetHWRenderSurf not opened \n");
       		return E_FAIL;
	  	}
        
		if(pSurf == NULL)
		{
			DP("CGfxAndroidSoftware::SetHWRenderSurf Invalid params \n");
			return E_FAIL;
		}
		
		VR_ANDROID_ISUR_PARAM * surfParams = (VR_ANDROID_ISUR_PARAM *)pSurf;
		if(surfParams->pAndroidSFBuf == m_nativewindow)
			return S_OK;
		
		EnterCriticalSection(&m_csDisplay);

		m_nativewindow =  surfParams->pAndroidSFBuf;
		window = (ANativeWindow*)m_nativewindow;
		DP("CGfxAndroidSoftware::SetHWRenderSurf m_nativewindow =%p \n", m_nativewindow);

		if(m_nativewindow == NULL)
		{
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}

		DP("CGfxAndroidSoftware::SetHWRenderSurf init start \n");
		switch (m_dwColorFormat) 
		{
		case OMX_COLOR_FormatYUV420Planar:
		case OMX_TI_COLOR_FormatYUV420PackedSemiPlanar:
			m_dwHalFormat = HAL_PIXEL_FORMAT_YV12;
			bufWidth = (m_dwWidth + 1) & ~1;
			bufHeight = (m_dwHeight + 1) & ~1;
			break;
		default:
			m_dwHalFormat = HAL_PIXEL_FORMAT_RGB_565;
			bufWidth = m_dwWidth;
			bufHeight = m_dwHeight;
			break;
		}

		usage = 	GRALLOC_USAGE_SW_READ_NEVER | 
				GRALLOC_USAGE_SW_WRITE_OFTEN | 
				GRALLOC_USAGE_HW_TEXTURE | 
				GRALLOC_USAGE_EXTERNAL_DISP;
		
		DP("CGfxAndroidSoftware::SetHWRenderSurf native_window_set_usage..");
  		err = native_window_set_usage(window.get(), usage);
		if(err)
		{
			DP("failed ret = 0x%x \n", err);
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}
		else
		{
			DP("done \n");
		}

		DP("CGfxAndroidSoftware::SetHWRenderSurf native_window_set_scaling_mode..");
		err = native_window_set_scaling_mode(window.get(), NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
		if(err)
		{
			DP("failed ret = 0x%x \n", err);
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}
		else
		{
			DP("done \n");
		}

		DP("CGfxAndroidSoftware::SetHWRenderSurf native_window_set_buffers_geometry..");
		// Width must be multiple of 32???
		err = native_window_set_buffers_geometry(window.get(), bufWidth, bufHeight, m_dwHalFormat);
		if(err)
		{
			DP("failed ret = 0x%x \n", err);
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}
		else
		{
			DP("done \n");
		}

		switch (m_dwRotate) {
			case 0: m_dwTrans = 0; break;
			case 90: m_dwTrans = HAL_TRANSFORM_ROT_90; break;
			case 180: m_dwTrans = HAL_TRANSFORM_ROT_180; break;
			case 270: m_dwTrans = HAL_TRANSFORM_ROT_270; break;
			default: m_dwTrans = 0; break;
		}

		if (m_dwTrans) 
		{
			DP("CGfxAndroidSoftware::SetHWRenderSurf native_window_set_buffers_transform..");
			err = native_window_set_buffers_transform(window.get(), m_dwTrans);
			if(err)
			{
				DP("failed ret = 0x%x \n", err);
				LeaveCriticalSection(&m_csDisplay);
				return E_FAIL;
			}
			else
			{
				DP("done \n");
			}
		}
		
		LeaveCriticalSection(&m_csDisplay);

		surfWidth = bufWidth;
		surfHeight = bufHeight;

		DP("CGfxAndroidSoftware::SetHWRenderSurf finish \n");

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
	DWORD m_dwWidth, m_dwHeight;
	CRITICAL_SECTION m_csDisplay; 
	LPVOID m_nativewindow;
	LPVOID m_ANativeWindowBuffer;

protected:
	STDMETHODIMP Update();

	bool m_bOpen;
	DWORD m_dwFourCC;
	bool m_bFirstTime;

	VP_ZOOM_MODE m_zoomMode;
	GfxPrepareDataV2 m_PrepareData;
	bool m_bshow;
	int surfWidth;
	int surfHeight;
	RECT m_rDst;

	DWORD m_dwRotate;
	DWORD m_dwTrans;
	DWORD m_dwHalFormat;
	DWORD m_dwColorFormat;
	
};


CGfxAndroidSoftware::CGfxAndroidSoftware():
m_bOpen(false),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0),
m_bFirstTime(true),
m_nativewindow(NULL),
m_ANativeWindowBuffer(NULL)
{
	InitializeCriticalSection(&m_csDisplay);
}

CGfxAndroidSoftware::~CGfxAndroidSoftware()
{
	Close();
	Sleep(100);
	DeleteCriticalSection(&m_csDisplay);
}


STDMETHODIMP CGfxAndroidSoftware::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int i = 0;
	DP("CGfxAndroidSoftware::Open start \n");
	if(m_bOpen || pdwFourCC == NULL)	
	{
		DP("CGfxAndroidSoftware::Open Invalid params \n");
		return E_FAIL;
	}

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	SetRect(&m_rDst,0,0,m_dwWidth,m_dwHeight);
	m_bshow = true;

	memset(&m_surfParam, 0,sizeof(m_surfParam));
	DP("CGfxAndroidSoftware::Open - frame size %dx%d \n", dwWidth, dwHeight);

	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
		if(pdwFourCC[i] == MAKEFOURCC('Y','V','1','2'))
		{
			break;
		}
	}

	if(!pdwFourCC[i])
	{
		DP("CGfxAndroidSoftware::Open - invalid fourcc\n");
		return E_FAIL;
	}

	m_dwRotate = 0;
	m_dwTrans = 0;
	//m_dwColorFormat = OMX_COLOR_Format16bitRGB565;
	m_dwColorFormat = OMX_COLOR_FormatYUV420Planar;
	
	m_dwFourCC = pdwFourCC[i];
	m_bOpen = true;
	
	DP("CGfxAndroidSoftware::Open finish\n");
	return S_OK;
}

STDMETHODIMP CGfxAndroidSoftware::Close()
{
	DP("CGfxAndroidSoftware::Close start\n");
	
	if(!m_bOpen)	
	{
		DP("CGfxAndroidSoftware::Close not opened\n");
		return E_FAIL;
	}

	m_nativewindow = 0;
	m_bOpen = false;
	
	DP("CGfxAndroidSoftware::Close finish\n");
	return S_OK;
}


STDMETHODIMP CGfxAndroidSoftware::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	int err;
	void *dst;
	sp<ANativeWindow>window = (ANativeWindow*)m_nativewindow;
	ANativeWindowBuffer *buf;
	
	DP("CGfxAndroidSoftware::Lock start  \n");
	DP("CGfxAndroidSoftware::Lock m_nativewindow = 0x%x  \n", m_nativewindow);
	DP("CGfxAndroidSoftware::Lock m_ANativeWindowBuffer = 0x%x  \n", m_ANativeWindowBuffer);
	
	if(!m_bOpen) 
	{
		DP("CGfxAndroidSoftware::Lock not opened\n");
		return E_FAIL;
	}

	if(m_nativewindow == 0 || m_ANativeWindowBuffer != 0)
	{
		DP("CGfxAndroidSoftware::Open Invalid params \n");
		return E_FAIL;
	}


	DP("CGfxAndroidSoftware::Lock dequeueBuffer ..");
	err = window->dequeueBuffer(window.get(), &buf);
	if(err)
	{
		DP("failed ret = 0x%x \n", err);
		return E_FAIL;
	}
	else
	{
		DP("done \n");
	}

	DP("CGfxAndroidSoftware::Lock lockBuffer ..");
	err = window->lockBuffer(window.get(), buf);
	if(err)
	{
		DP("failed ret = 0x%x \n", err);
		return E_FAIL;
	}
	else
	{
		DP("done \n");
	}
	
	GraphicBufferMapper &mapper = GraphicBufferMapper::get();
	Rect bounds(m_dwWidth, m_dwHeight);

	DP("CGfxAndroidSoftware::Lock mapper.lock ..");
	err = mapper.lock(buf->handle, GRALLOC_USAGE_SW_WRITE_OFTEN, bounds, &dst);
	if(err)
	{
		DP("failed ret = 0x%x \n", err);
		return E_FAIL;
	}
	else
	{
		DP("done \n");
	}
	
	DP("CGfxAndroidSoftware::Lock get buf = 0x%x \n", dst);
	
	if(ppBuf)
		*ppBuf = (unsigned char *)dst;
	if(pStride)
		*pStride = (m_dwWidth + 31) & ~31;		//trick here, seems it need align to 32
	if(pPixCount)
		*pPixCount = 3/2;

	 m_ANativeWindowBuffer = (LPVOID)buf;
	
	DP("CGfxAndroidSoftware::Lock finish  \n");
	return S_OK;
}

STDMETHODIMP CGfxAndroidSoftware::Unlock(LONG num)
{
	int err;
	ANativeWindowBuffer *buf = (ANativeWindowBuffer*)m_ANativeWindowBuffer;
	
	DP("CGfxAndroidSoftware::Unlock start  \n");
	DP("CGfxAndroidSoftware::Unlock m_ANativeWindowBuffer = 0x%x  \n", m_ANativeWindowBuffer);
	
	if(!m_bOpen) 
	{
		DP("CGfxAndroidSoftware::Unlock not opened\n");
		return E_FAIL;
	}

	if(buf == 0)
	{
		DP("CGfxAndroidSoftware::Unlock buf is null \n");
		return E_FAIL;
	}
	
	GraphicBufferMapper &mapper = GraphicBufferMapper::get();

	DP("CGfxAndroidSoftware::Unlock mapper.unlock ..");
	err = mapper.unlock(buf->handle);
	if (err)
	{
		DP("failed ret = 0x%x \n", err);
		return E_FAIL;
	}
	else
	{
		DP("done \n");
	}
	
	DP("CGfxAndroidSoftware::Unlock finish  \n");
	return S_OK;
}

STDMETHODIMP CGfxAndroidSoftware::Flip(LONG num, DWORD dwFlags)
{
	int err;
	sp<ANativeWindow>window = (ANativeWindow*)m_nativewindow;
	ANativeWindowBuffer *buf = (ANativeWindowBuffer*)m_ANativeWindowBuffer;
	
	DP("CGfxAndroidSoftware::Flip start  \n");
	
	if(!m_bOpen) 
	{
		DP("CGfxAndroidSoftware::Flip not opened\n");
		return E_FAIL;
	}

	if(!m_bshow)
	{
		DP("CGfxAndroidSoftware::Flip not show\n");
		return S_OK;
	}

	if(window == 0 || buf == 0)
	{
		DP("CGfxAndroidSoftware::Flip Invalid params \n");
		return E_FAIL;
	}
	
	EnterCriticalSection(&m_csDisplay);

	DP("CGfxAndroidSoftware::Flip queueBuffer ..");
	err = window->queueBuffer(window.get(), buf);
	if(err)
	{
		DP("failed ret = 0x%x \n", err);
		LeaveCriticalSection(&m_csDisplay);
		return E_FAIL;
	}
	else
	{
		DP("done \n");
	}

	m_ANativeWindowBuffer = NULL;
	LeaveCriticalSection(&m_csDisplay);

	DP("CGfxAndroidSoftware::Flip finish  \n");
	return S_OK;
}

STDMETHODIMP CGfxAndroidSoftware::UIFlip(LPVOID pHandle)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidSoftware::Show(BOOL bShow)
{
	EnterCriticalSection(&m_csDisplay);
	m_bshow = bShow;
	LeaveCriticalSection(&m_csDisplay);
 	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
 	return E_NOTIMPL;
}
STDMETHODIMP CGfxAndroidSoftware::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetSrcRect(const RECT *pRect)
{
	DP ("CGfxAndroidSoftware::SetSrcRect %d,%d \n",pRect->right - pRect->left, pRect->bottom - pRect->top);
	
 	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetDstRect(const RECT *pRect)
{
	int err = 0;
	int bufWidth, bufHeight;
	sp<ANativeWindow>window = (ANativeWindow*)m_nativewindow;
	ANativeWindowBuffer *buf = (ANativeWindowBuffer*)m_ANativeWindowBuffer;
	
	DP ("CGfxAndroidSoftware::SetDest %d,%d \n",pRect->right - pRect->left, pRect->bottom - pRect->top);
	
	return S_OK;

	if(!m_bOpen)
	{
		DP ("CGfxAndroidSoftware::SetDest not opened \n");
		return E_FAIL;
	}
	
	if(m_nativewindow = 0)
	{
		DP ("CGfxAndroidSoftware::SetDest nativewindow is null \n");
		return E_FAIL;
	}

	EnterCriticalSection(&m_csDisplay);
	
	if(buf)
	{
		GraphicBufferMapper &mapper = GraphicBufferMapper::get();

		DP("CGfxAndroidSoftware::SetDest mapper.unlock ..");
		err = mapper.unlock(buf->handle);
		
		DP("CGfxAndroidSoftware::SetDest queueBuffer ..");
		err = window->queueBuffer(window.get(), buf);

		m_ANativeWindowBuffer = NULL;
	}
	
	m_dwWidth = m_rDst.right - m_rDst.left;
	m_dwHeight = m_rDst.bottom - m_rDst.top;
		
	switch (m_dwColorFormat) 
	{
	case OMX_COLOR_FormatYUV420Planar:
	case OMX_TI_COLOR_FormatYUV420PackedSemiPlanar:
		m_dwHalFormat = HAL_PIXEL_FORMAT_YV12;
		bufWidth = (m_dwWidth + 1) & ~1;
		bufHeight = (m_dwHeight + 1) & ~1;
		break;
	default:
		m_dwHalFormat = HAL_PIXEL_FORMAT_RGB_565;
		bufWidth = m_dwWidth;
		bufHeight = m_dwHeight;
		break;
	}
	
	DP("CGfxAndroidSoftware::SetDstRect native_window_set_buffers_geometry..");
	// Width must be multiple of 32???
	err = native_window_set_buffers_geometry(window.get(), bufWidth, bufHeight, m_dwHalFormat);
	if(err)
	{
		DP("failed ret = 0x%x \n", err);
		LeaveCriticalSection(&m_csDisplay);
		return E_FAIL;
	}
	else
	{
		DP("done \n");
	}

	surfWidth = bufWidth;
	surfHeight = bufHeight;
	
	LeaveCriticalSection(&m_csDisplay);
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetScnRect(const RECT *pRect)
{
	DP ("CGfxAndroidSoftware::SetScnRect %d,%d \n",pRect->right - pRect->left, pRect->bottom - pRect->top);
	
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::GetScnRect(RECT *pRect)
{
	if(!m_bOpen) 
		return E_FAIL;
	
  	SetRect(pRect, 0, 0, m_dwWidth,m_dwHeight);

  return S_OK;
}

STDMETHODIMP CGfxAndroidSoftware::SetWindowHandle(HWND hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidSoftware::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = 0;
	pInfo->dwFourCC = MAKEFOURCC('Y','V','1','2');
	//pInfo->dwFourCC = MAKEFOURCC('R','5','6','5');
	pInfo->dwCaps = 0;
	return S_OK;

}

STDMETHODIMP CGfxAndroidSoftware::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidSoftware::Update()
{
	return S_OK;
}

IMcGfxOverlay *CreateOverlayAndroidSoftware()
{
	return new CGfxAndroidSoftware();
}

#endif
