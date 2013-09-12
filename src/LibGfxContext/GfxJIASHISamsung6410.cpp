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
// 1) Check directdraw surface lost through resolution change - done
// 2) Check multimon support (multiple monitors) - done
// 3) Check overlay failure (creation) - done
// 4) Implement interlacing support
#include "../../inc/GfxBuildDef.h"


#ifdef JIASHI_SAMSUNG6410
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"
extern "C"
{
#include "../../3rdparty/JIASHI_SAMSUNG6410/cmm_vde.h"
}
#pragma comment(lib, "../../3rdparty/JIASHI_SAMSUNG6410/cmm_vde.lib")


// WINSIZE
typedef struct tagWINSIZE
{
	unsigned int uiStartPointX;
	unsigned int uiStartPointY;
	unsigned int uiWidth;
	unsigned int uiHeight;
} WINSIZE, *PWINSIZE;
typedef BOOL (WINAPI *FN_TvOutOpen)(WINSIZE stTVoutContext);
typedef BOOL (WINAPI *FN_TvOutClose)();
typedef BOOL (WINAPI *FN_TvOut)();

static HMODULE 			g_hOsDll = 0;
	
class CGfxJIASHISAMSUNG6410 : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxJIASHISAMSUNG6410();
	virtual ~CGfxJIASHISAMSUNG6410();
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
	STDMETHODIMP GetDDCaps(LONG*pCaps)
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
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
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
	STDMETHODIMP OpenSys();
	STDMETHODIMP OpenMedia();

protected:
	STDMETHODIMP Update();
	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow;
	HWND				m_hwnd;
	BOOL m_bOpen; 	

	UINT8* g_cmm;
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	0
};

CGfxJIASHISAMSUNG6410::CGfxJIASHISAMSUNG6410() : 
	CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	m_hwnd = 0;
	m_bOpen = FALSE;
	m_dwFourCC = 0;
	g_cmm = 0;
}

CGfxJIASHISAMSUNG6410::~CGfxJIASHISAMSUNG6410()
{
	Close();
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	UINT32 CamIFState = 0;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)
	{
		DP("[GFXJIASHI_SAMSUNG]Invalid call \n");
		return E_FAIL;
	}


	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
	
	DP("[GFXJIASHI_SAMSUNG]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);

	int i;
	for(i=0;m_pdwFourCCList[i];i++)
	{
		if(m_pdwFourCCList[i] == MAKEFOURCC('Y','V','1','2'))
			break;
	
	}
	if(!m_pdwFourCCList[i] )	return E_FAIL;
	m_dwFourCC = m_pdwFourCCList[i];
	g_cmm = init_cmm();
	memset(g_cmm, 0x00, m_dwWidth*m_dwHeight);
	memset(g_cmm + m_dwWidth*m_dwHeight, 0x80, m_dwWidth*m_dwHeight/2);
	
	init_windows0();
	if(g_cmm == NULL)
		return E_FAIL;
	
	SetRect(&m_rectScn,0,0,800,480);
	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::Close()
{
	if(!m_bOpen)	return E_UNEXPECTED;
	uinit_windows0();
	uinit_cmm();
	g_cmm = NULL;
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetDstRect(const RECT *pRect)
{
	if(!pRect)	return E_INVALIDARG;
	m_rectDst  = *pRect;
	m_rectDst.left &= ~(8-1);
	m_rectDst.top &= ~(8-1);
	m_rectDst.right &= ~(8-1);
	m_rectDst.bottom &= ~(8-1);
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetScnRect(const RECT *pRect)
{
	return S_OK;	
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
		*ppBuf = g_cmm;
	if(pStride)
		*pStride = m_dwWidth;
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::Flip(LONG num, DWORD dwFlags)
{
	if(m_rectDst.left == 0 && m_rectDst.right == 0 && m_rectDst.top == 0 && m_rectDst.bottom == 0)
		return S_OK;
	StartDisplay(m_rectDst.left,m_rectDst.top,m_rectDst.right - m_rectDst.left,
	  m_rectDst.bottom - m_rectDst.top,g_cmm,2,m_dwWidth,m_dwHeight);
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::Update()
{
	return S_OK;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxJIASHISAMSUNG6410::IsOpen()
{
	return m_bOpen;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::OpenSys()
{
	return E_FAIL;
}

STDMETHODIMP CGfxJIASHISAMSUNG6410::OpenMedia()
{
	return E_FAIL;
}

IMcGfxOverlay *CreateJIASHI_SAMSUNG6410()
{
	return new CGfxJIASHISAMSUNG6410();
}

#endif
