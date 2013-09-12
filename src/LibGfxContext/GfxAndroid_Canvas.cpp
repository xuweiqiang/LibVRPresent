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

#ifdef GFX_ANDROID_CANVAS


#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"


#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <utils/threads.h>
#include "utils/Errors.h"  // for status_t

#include <core/SkBitmap.h>

class CGfxAndroidCanvas : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidCanvas();
	virtual ~CGfxAndroidCanvas();
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
		if(pSurf == NULL)
			return E_FAIL;

		VR_ANDROID_ISUR_PARAM * surfParams = (VR_ANDROID_ISUR_PARAM *)pSurf;
		m_bitmap = (SkBitmap *)(surfParams->pAndroidSFBuf);
		DP(">>>> mBitmap =%p \n", m_bitmap);

		if(m_bitmap  == NULL)
		{
			DP("CGfxAndroidCanvas::Set mBitmap == NULL\n");
			return E_FAIL;
		}

		unsigned char * data = (unsigned char *)m_bitmap->getPixels();
		if (data == NULL)
		{
			if (!m_bitmap->allocPixels(NULL, NULL))
			{
				DP("CGfxAndroidCanvas::UIFlip - failed to allocPixels!");
			}
		}
	
	//	DP("CGfxAndroidCanvas::Set m_rectScn.right %d, m_rectScn.bottom %d\n",  m_surfparams.scnWidth, m_surfparams.scnHeight);

		DP("CGfxAndroidOCE::SetHWRenderSurf end\n");
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

protected:
	STDMETHODIMP Update();

	bool m_bOpen;
	RECT				m_rectSrc, m_rectDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD m_dwFourCC;
	bool m_bFirstTime;

	SkBitmap * m_bitmap;

	CRITICAL_SECTION  m_csBuf;
};


CGfxAndroidCanvas::CGfxAndroidCanvas():
m_bOpen(false),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0),
m_bFirstTime(true),
m_bitmap(NULL)
{
	memset(&m_rectSrc, 0,sizeof(m_rectSrc));
	memset(&m_rectDst, 0,sizeof(m_rectDst));

	InitializeCriticalSection(&m_csBuf);
}

CGfxAndroidCanvas::~CGfxAndroidCanvas()
{
	DeleteCriticalSection(&m_csBuf);
	Close();
}


STDMETHODIMP CGfxAndroidCanvas::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int i = 0;
	if(m_bOpen || pdwFourCC == NULL)	
	{
		return E_FAIL;
	}

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	DP("CGfxAndroidCanvas::Open - frame size %dx%d \n", dwWidth, dwHeight);

	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
		if(pdwFourCC[i] == MAKEFOURCC('Y','V','1','2'))
		{
			DP("CGfxAndroidCanvas::Open - alloc buffer\n");
			break;
		}
	}

	if(!pdwFourCC[i])
	{
		return E_FAIL;
	}

	m_dwFourCC = pdwFourCC[i];
	m_bOpen = true;
	DP("CGfxAndroidCanvas::Open - OK\n");
	return S_OK;
}

STDMETHODIMP CGfxAndroidCanvas::Close()
{
	if(!m_bOpen)	
	{
		return E_FAIL;
	}


	m_bOpen = false;
	return S_OK;
}

STDMETHODIMP CGfxAndroidCanvas::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
//	DP("Lock - num %d\n", num);
	EnterCriticalSection(&m_csBuf);
	if(ppBuf)
		*ppBuf = (unsigned char *)m_bitmap->getPixels();
	if(pStride)
		*pStride = m_bitmap->width()*2;
	if(pPixCount)
		*pPixCount = 12;
	return S_OK;
}

STDMETHODIMP CGfxAndroidCanvas::Unlock(LONG num)
{
//	DP("Unlock - num %d\n", num);
	LeaveCriticalSection(&m_csBuf);
	return S_OK;
}

STDMETHODIMP CGfxAndroidCanvas::Flip(LONG num, DWORD dwFlags)
{
//	DP("Flip - num %d, flag %u\n", num, dwFlags);
	return S_OK;
}

STDMETHODIMP CGfxAndroidCanvas::UIFlip(LPVOID pHandle)
{
//	LOGI("CGfxAndroidCanvas::UIFlip");
	if(!m_bOpen)	
	{
		return E_FAIL;
	}

	if (m_bitmap == NULL)
	{
		DP("CGfxAndroidCanvas::UIFlip - bitmap not initialized!\n");
		return E_FAIL;
	}
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	*pdwFlags = GFXOVERLAY_SCALE_X | GFXOVERLAY_SCALE_Y;
	SetRect(pRSrcRect,0,0,m_dwWidth,m_dwHeight);
	SetRect(pDstRect,0,0,m_bitmap->width(),m_bitmap->height());
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetDstRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidCanvas::SetWindowHandle(HWND hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidCanvas::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = 0;
	pInfo->dwFourCC = MAKEFOURCC('R','5','6','5');
	pInfo->dwCaps = 0;
	return S_OK;

}

STDMETHODIMP CGfxAndroidCanvas::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidCanvas::Update()
{
	return S_OK;
}

IMcGfxOverlay *CreateOverlayAndroidCanvas()
{
	return new CGfxAndroidCanvas();
}

#endif
