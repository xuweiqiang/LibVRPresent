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

#ifdef GFX_OPENGLES


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


static bool bInit = false;
class CGfxOpenGLES : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxOpenGLES();
	virtual ~CGfxOpenGLES();
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

};


CGfxOpenGLES::CGfxOpenGLES():
m_bOpen(false),
m_pBuff(0),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0)
{
	memset(&m_rectSrc, 0,sizeof(m_rectSrc));
	memset(&m_rectDst, 0,sizeof(m_rectDst));
	m_pBuff = NULL;
}

CGfxOpenGLES::~CGfxOpenGLES()
{
}

STDMETHODIMP CGfxOpenGLES::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
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
STDMETHODIMP CGfxOpenGLES::Close()
{
	if(!m_bOpen)	return E_FAIL;
	DP("GfxAndroid_OpenCore: Close \n");

	m_bOpen = false;
	return S_OK;
}


STDMETHODIMP CGfxOpenGLES::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
    if(ppBuf)
	    *ppBuf = m_pBuff;
    if(pStride)
	    *pStride = m_dwWidth;
    if(pPixCount)
	    *pPixCount = 1;

	return S_OK;
}

STDMETHODIMP CGfxOpenGLES::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxOpenGLES::Flip(LONG num, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxOpenGLES::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetDstRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxOpenGLES::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}

STDMETHODIMP CGfxOpenGLES::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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
STDMETHODIMP CGfxOpenGLES::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxOpenGLES::Update()
{
	return S_OK;
}

IMcGfxOverlay *CreateOverlayOpenGLES()
{
	return new CGfxOpenGLES();
}
#endif
