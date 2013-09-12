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
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

// Things to do: (achung)
// 1) Check multimon support (multiple monitors)
// 2) Implement interlacing support
#include "../../inc/GfxBuildDef.h"

#ifdef GFX_GX
#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "gx.h"

#ifdef _DEBUG
static void __cdecl DP(LPCSTR szFormat, ...)
{
	char szBuffer[256]; 
	va_list vl; 
	va_start(vl, szFormat);

#ifdef __linux
	vsnprintf(szBuffer,255,szFormat,vl);
#else
	_vsnprintf(szBuffer,255,szFormat,vl);
#ifdef _DP_FILE
	FILE* pfDPFile = fopen(".\\dp.txt", "a");
	fprintf(pfDPFile, szBuffer);
	fclose(pfDPFile);
#else
#endif
	szBuffer[255]=0;
#ifdef _WIN32_WCE
	{
#ifndef _DP_FILE
		WCHAR szBufferW[256];

		MultiByteToWideChar(CP_ACP,0,szBuffer,-1,szBufferW,255);
		szBufferW[255]=0;
		OutputDebugString(szBufferW);
#endif
	}
#else
#ifdef __linux__
	fprintf(stderr,"%s",szBuffer); fflush(stderr);
#else
	OutputDebugStringA(szBuffer);
#endif
#endif
#endif	
	va_end(vl);
}
#else
#define DP(x, ...)
#endif

class CGfxGx: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxGx();
	virtual ~CGfxGx();
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
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}


protected:
	STDMETHODIMP Update();

	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight;
	BOOL				m_bOpen, m_bShow;
	GXKeyList m_KeyList;
	GXDisplayProperties m_Properties;
};

CGfxGx::CGfxGx() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	m_bOpen = FALSE;
	m_bShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_KeyList,sizeof(m_KeyList));
	ZeroMemory(&m_Properties,sizeof(m_Properties));

}

CGfxGx::~CGfxGx()
{
}

STDMETHODIMP CGfxGx::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)
		return S_OK;
	m_bOpen = TRUE;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	DP("[GFXGx]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);
	GXOpenDisplay(NULL, GX_FULLSCREEN);
	m_KeyList		= GXGetDefaultKeys(GX_NORMALKEYS);
	m_Properties	= GXGetDisplayProperties();

	return S_OK;
}

STDMETHODIMP CGfxGx::Close()
{
	if(!m_bOpen)
		return S_OK;
	GXCloseDisplay();
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxGx::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	DP("[GFXGx]Set SourceRect (%d,%d, %d, %d) \n", m_rectSrc.left, m_rectSrc.top, m_rectSrc.right, m_rectSrc.bottom);
	return Update();
}

STDMETHODIMP CGfxGx::SetDstRect(const RECT *pRect)
{
	//GfxMonitorInfo *pinfo;
	
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
	m_rectDst = *pRect;

	Update();
	return S_OK;
}

STDMETHODIMP CGfxGx::SetScnRect(const RECT *pRect)
{
	return S_OK;	// ignored
}


STDMETHODIMP CGfxGx::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxGx::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::GetWindowHandle(HWND& hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
		*ppBuf = GXBeginDraw(); 

	if(pStride)
		*pStride = m_Properties.cbyPitch;
	if(pPixCount)
		*pPixCount = m_Properties.cBPP;
	return S_OK;
}

STDMETHODIMP CGfxGx::Unlock(LONG num)
{
	int res = GXEndDraw();
	return S_OK;
}

STDMETHODIMP CGfxGx::Flip(LONG num, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxGx::Show(BOOL bShow)
{
	int res;
	if(m_bShow==bShow)
		return S_OK;
	
	m_bShow = bShow;
	if(!m_bShow)
		res = GXSuspend(); 
	else
		res = GXResume(); 

	return Update();
}

STDMETHODIMP CGfxGx::Update()
{
	if(GXIsDisplayDRAMBuffer())
		GXSetViewport( m_rectDst.top, m_rectDst.bottom - m_rectDst.top, 0, 0 ); 
	return S_OK;
}

STDMETHODIMP CGfxGx::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	if(pSrc)
		*pSrc = m_rectSrc;
	if(pDst)
		*pDst = m_rectDst;
	if(!EqualRect(&m_rectSrc,&m_rectDst))
		*pdwFlags = 1;
	else
		*pdwFlags = 0;
	return S_OK;
}

STDMETHODIMP CGfxGx::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxGx::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGx::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo == NULL)
		return E_FAIL;
	pInfo->dwFourCC = MAKEFOURCC('R','5','6','5');
	pInfo->dwBackBuffers = 0;
	return S_OK;
}

STDMETHODIMP CGfxGx::IsOpen()
{
	return m_bOpen ? S_OK : S_FALSE;
}

IMcGfxOverlay *CreateOverlayGx()
{
	return new CGfxGx;
}
#endif
