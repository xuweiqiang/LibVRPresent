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

#ifdef GFX_ANDROID_NATIVE_BITMAP

#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "GfxPresent.h"

#include "tchar.h"

class CGfxAndroidNativeBitmap : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidNativeBitmap();
	virtual ~CGfxAndroidNativeBitmap();
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

	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf);
	
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
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}


protected:
	STDMETHODIMP Update();
	
	RECT				m_rectSrc, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow;
	HWND				m_hwnd;
	BOOL m_bOpen; 	
	FILE *m_logfile;
	RECT				 m_rectDst;
	float m_bytesPerPixel;
	DWORD m_inputFourCC;
	UINT  SURF_SCN_WIDTH;
	UINT SURF_SCN_HEIGHT;
	unsigned char* m_pbuf;
	VR_ANDROID_ISUR_PARAM m_params;

};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('R','5','6','5'),
	0
};

CGfxAndroidNativeBitmap::CGfxAndroidNativeBitmap() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 2;
	m_bShow = m_bLastShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
 
	m_hwnd = 0;
	m_bOpen = FALSE;
	m_dwFourCC = 0;
	m_logfile = NULL;

	m_bytesPerPixel = 0.0f;
	m_inputFourCC = 0;
	SURF_SCN_WIDTH = 480;
	SURF_SCN_HEIGHT = 800;
	m_pbuf = NULL;
	memset(&m_params, 0,sizeof(m_params));
}


CGfxAndroidNativeBitmap::~CGfxAndroidNativeBitmap()
{
	Close();
}

STDMETHODIMP CGfxAndroidNativeBitmap::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	DP("[CGfxAndroidNativeBitmap::Open]\n");
	int res = 0;

	if(dwWidth==0 || dwHeight==0)
	{
		DP("[CGfxAndroidNativeBitmap::Open] dwWidth==0 || dwHeight==0\n");
		return E_FAIL;
	}

	if(m_bOpen)
	{
		DP("[CGfxAndroidNativeBitmap::Open] Invalid call \n");
		return E_FAIL;
	}

	m_dwFourCC = 0;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	SetRect(&m_rectScn,0,0,SURF_SCN_WIDTH,SURF_SCN_HEIGHT);

	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));

	SetRect(&m_rectSrc, 0, 0 ,m_dwWidth, m_dwHeight);

	m_rectDst.right = m_rectScn.right;
	m_rectDst.bottom = m_rectScn.bottom;
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;

	m_bytesPerPixel = 0;
	m_inputFourCC = 0;
	int i=0;
	for(i=0;m_dwFourCC=m_pdwFourCCList[i];i++)
	{
		switch(m_dwFourCC)
		{
			case MAKEFOURCC('R','5','6','5'):
				m_bytesPerPixel = 2;
				m_inputFourCC = MAKEFOURCC('R','5','6','5');
				break;
			default:
				break;
		}
		if(m_inputFourCC  != 0)
			break;
	}

	if(m_inputFourCC == 0)
		return E_FAIL;

	m_pbuf = (unsigned char*)malloc(SURF_SCN_WIDTH*SURF_SCN_HEIGHT* m_bytesPerPixel);
	if(!m_pbuf)
	{
		DP("[CGfxAndroidNativeBitmap]malloc :%d failed \n",SURF_SCN_WIDTH*SURF_SCN_HEIGHT* m_bytesPerPixel);
		return E_FAIL;
	}
	DP("[CGfxAndroidNativeBitmap]Open w:%d h:%d fmt:%d  \n",m_dwWidth, m_dwHeight,m_inputFourCC);

	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::Close()
{
	if(m_pbuf)
	{
		free(m_pbuf);
		m_pbuf = NULL;
	}
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetSrcRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetDstRect(const RECT *pRect)
{
	RECT rd;

	DP("[CGfxAndroidNativeBitmap]Set destRect (%d,%d, %d, %d) \n", pRect->left, pRect->top, pRect->right, pRect->bottom);
	if(EqualRect(pRect,&m_rectDst) ){
		DP("[CGfxAndroidNativeBitmap]Set destRect equal rect\n");
		return S_OK;
	}
	
	DWORD width = pRect->right - pRect->left;
	DWORD height = pRect->bottom - pRect->top;

	if((width == 0) || (height ==0 )){
		DP("[CGfxAndroidNativeBitmap]Set destRect (width == 0) || (height ==0 )\n");
		return E_FAIL;
	}

	rd = *pRect;
	IntersectRect(&m_rectDst,&rd,&m_rectScn);

	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetScnRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn;
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
		*ppBuf = m_pbuf;

	if(pPixCount)
		*pPixCount = m_bytesPerPixel;
	
	if(pStride)
		*pStride = m_dwWidth;

	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::Flip(LONG num, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::UIFlip(LPVOID pHandle)
{
	if(!m_params.pAndroidSFBuf)
		return E_FAIL;

	DP("[CGfxAndroidNativeBitmap::UIFlip Start]\n");
	memcpy(m_params.pAndroidSFBuf, m_pbuf, SURF_SCN_WIDTH*SURF_SCN_HEIGHT*m_bytesPerPixel);
	DP("[CGfxAndroidNativeBitmap::UIFlip End]\n");
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;

	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxAndroidNativeBitmap::Update()
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidNativeBitmap::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if((pInfo==0) ||(pInfo->dwSize!=sizeof(*pInfo)))
		return E_FAIL;

	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0; 
	return S_OK;
}

STDMETHODIMP CGfxAndroidNativeBitmap::IsOpen()
{
	return m_bOpen;
}

STDMETHODIMP CGfxAndroidNativeBitmap::SetHWRenderSurf(LPVOID pSurf)
{
	if(pSurf == NULL)
		return E_FAIL;

	m_params = (*(VR_ANDROID_ISUR_PARAM *)pSurf);

	DP("CGfxAndroidSF::Set CI_CFG_ANDROID_ISURFACE not null %p\n", m_params.pAndroidSFBuf);		
	SURF_SCN_WIDTH = m_params.scnWidth;
	SURF_SCN_HEIGHT = m_params.scnHeight;
	DP("CGfxAndroidSF::Set CI_CFG_ANDROID_ISURFACE SURF_SCN_WIDTH :%d , %d \n", SURF_SCN_WIDTH,SURF_SCN_HEIGHT  );		
	
	DP("CGfxAndroidSF::SetHWRenderSurf  end\n");
	return S_OK;
}

IMcGfxOverlay *CreateOverlayAndroidNativeBitmap()
{
	return new CGfxAndroidNativeBitmap();
}
#endif
