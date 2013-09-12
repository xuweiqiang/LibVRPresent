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

#ifdef GFX_ANDROID_OPENCORE


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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include <android/log.h>
#include "GfxAndroid_OpenCoreMyRenderer.h"

static bool bInit = false;
class CGfxAndroidOCE : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidOCE();
	virtual ~CGfxAndroidOCE();
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
		if(!pSurf)	return E_FAIL;

		m_iSurfparams = (*(VR_ANDROID_ISUR_PARAM *)pSurf);
		DP("L:%d :%p \n", __LINE__,m_iSurfparams.pAndroidSFBuf);
		m_isurface = (ISurface*)(m_iSurfparams.pAndroidSFBuf);

		const char * mioLib = "libopencorehw.so";
		const char* VIDEO_MIO_FACTORY_NAME = "createVideoMio";
		typedef AndroidSurfaceOutput* (*VideoMioFactory)();

		AndroidSurfaceOutput * surfaceOutput = NULL;
		m_hMioLib = ::dlopen(mioLib, RTLD_NOW);
		DP("GfxAndroid_OpenCore- Line:%d \n",__LINE__);
		if (m_hMioLib != NULL) 
		{
			VideoMioFactory f = (VideoMioFactory)::dlsym(m_hMioLib, VIDEO_MIO_FACTORY_NAME);
			if (f != NULL) 
			{
				surfaceOutput = f();
			}
		}

		if (surfaceOutput)
		{
			DP("HW renderer used\n");
		}
		else
		{
			DP("Default renderer used\n");
		}

#ifdef ROTATION_TEST
		int m_dpFormat = PIXEL_FORMAT_RGB_565;
		int bytesPerPixel = 2;

		sp<MemoryHeapBase> 	m_FrameHeap = 
			new MemoryHeapBase(m_iSurfparams.scnWidth*m_iSurfparams.scnHeight*bytesPerPixel);
		if(m_FrameHeap == NULL){ DP("CGfxAndroidSF::Set m_FrameHeap == NULL\n"); }

		status_t status;
		ISurface::BufferHeap buffers(m_iSurfparams.scnWidth, m_iSurfparams.scnHeight, 
				m_iSurfparams.scnWidth, m_iSurfparams.scnHeight, m_dpFormat, m_FrameHeap);

		status = m_isurface->registerBuffers(buffers);
		if(status)
		{
			DP("CGfxAndroidSF::Set isurface->registerBuffers error status %x\n", status);
		}

#endif

		m_myRenderer.Init(m_isurface, surfaceOutput);
		bool isRendererInited = m_myRenderer.SetVideoFormat(m_dwWidth, m_dwHeight, m_dwWidth, m_dwHeight);
		if (isRendererInited)
		{
			DP("Renderer set video format OK - video size %d x %d\n", m_dwWidth, m_dwHeight);
		}
		else
		{
			DP("Renderer set video format failed - video size %d x %d\n", m_dwWidth, m_dwHeight);
			return E_FAIL;
		}
		
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
	unsigned char *m_pBuff;
	RECT				m_rectSrc, m_rectDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD m_dwFourCC;

	VR_ANDROID_ISUR_PARAM m_iSurfparams;
	sp<ISurface> m_isurface;
	CGfxAndroid_MyRenderer m_myRenderer;
	int m_frameSize ;
	void* m_hMioLib;
};


CGfxAndroidOCE::CGfxAndroidOCE():
m_bOpen(false),
m_pBuff(0),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0)
{
	memset(&m_rectSrc, 0,sizeof(m_rectSrc));
	memset(&m_rectDst, 0,sizeof(m_rectDst));
	m_pBuff = NULL;
	m_frameSize  = 0;
	m_hMioLib = 0;
}

CGfxAndroidOCE::~CGfxAndroidOCE()
{
}

STDMETHODIMP CGfxAndroidOCE::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int i = 0;
	if(m_bOpen || pdwFourCC == NULL)	return E_FAIL;

	DP("GfxAndroid_OpenCore: start open\n");
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
		if(pdwFourCC[i] == MAKEFOURCC('Y','V','1','2'))
		{
			m_frameSize = m_dwWidth* m_dwHeight * 3 / 2;
			m_pBuff= (unsigned char *)malloc(m_frameSize);
			if(!m_pBuff)	return E_FAIL;
			
			m_dwFourCC = pdwFourCC[i];
			break;
		}

	}
	
	if(!pdwFourCC[i])
		return E_FAIL;
	
	m_bOpen = true;
	DP("GfxAndroid_OpenCore: Open successed \n");
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::Close()
{
	if(!m_bOpen)	return E_FAIL;
	DP("GfxAndroid_OpenCore: Close \n");

	if(m_pBuff)
	{
		free(m_pBuff);
		m_pBuff = NULL;
	}

	if (m_hMioLib != NULL)
	{
		::dlclose(m_hMioLib);
		m_hMioLib = NULL;
	}

	m_bOpen = false;
	return S_OK;
}
static bool bLock = false;


long g_tick = 0;

STDMETHODIMP CGfxAndroidOCE::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
    if(ppBuf)
	    *ppBuf = m_pBuff;
    if(pStride)
	    *pStride = m_dwWidth;
    if(pPixCount)
	    *pPixCount = 1;

	return S_OK;
}

STDMETHODIMP CGfxAndroidOCE::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidOCE::Flip(LONG num, DWORD dwFlags)
{
//	memset(m_pBuff, 0x00, m_dwWidth*m_dwHeight*3/2);
	m_myRenderer.SendData(m_pBuff, m_frameSize);
//	usleep(1000*10);
	return S_OK;
}

STDMETHODIMP CGfxAndroidOCE::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetDstRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidOCE::SetWindowHandle(HWND hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidOCE::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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
STDMETHODIMP CGfxAndroidOCE::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOCE::Update()
{
	return S_OK;
}

IMcGfxOverlay *CreateOverlayAndroidOCE()
{
	return new CGfxAndroidOCE();
}
#endif
