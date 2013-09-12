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


#ifdef SAMSUNG6410
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"
extern "C"
{
#include "../../3rdparty/SAMSUNG6410/cmm_vde.h"
}
#pragma comment(lib, "../../3rdparty/SAMSUNG6410/cmm_vde.lib")


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
	
class CGfxSAMSUNG6410 : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxSAMSUNG6410();
	virtual ~CGfxSAMSUNG6410();
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
	bool LoadOSDll(void)
	{
		g_hOsDll = LoadLibrary(TEXT("\\Windows\\DriverAPI.dll")) ;

		if(g_hOsDll)
		{
			m_fnOpenMedia = (FN_TvOutOpen)GetProcAddress(g_hOsDll, _T("TvOutOpen_MediaWin"));
			m_fnCloseMedia = (FN_TvOutClose)GetProcAddress(g_hOsDll, _T("TvOutClose_MediaWin"));
			m_fnOpenSys = (FN_TvOut)GetProcAddress(g_hOsDll, _T("TvOutOpen_SysWin"));
			m_fnCloseSys = (FN_TvOut)GetProcAddress(g_hOsDll, _T("TvOutClose_SysWin"));
		}
		if(m_fnOpenMedia && m_fnCloseMedia && m_fnOpenSys && m_fnCloseSys)
		{
			return true ;
		}
		return false ;
	}

	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow;
	HWND				m_hwnd;
	BOOL m_bOpen; 	
	unsigned char m_uAlpha; //0-0xf

	UINT8* g_cmm;

	//VP_TVOUT_MODE m_bTVOut; 	
	FN_TvOutOpen			m_fnOpenMedia;			// 打开视频输出
	FN_TvOutClose			m_fnCloseMedia ;		// 关闭视频输出
	FN_TvOut			m_fnOpenSys;			// 打开系统输出
	FN_TvOut			m_fnCloseSys;			// 关闭系统输出
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	0
};

CGfxSAMSUNG6410::CGfxSAMSUNG6410() : 
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
	m_uAlpha = 0xf;
//	m_bTVOut = MC_VP_TVOUT_MODE_ONINNER_OFFOUTER;
	m_fnOpenMedia  = NULL;			// 打开视频输出
	m_fnCloseMedia = NULL;		// 关闭视频输出
	m_fnOpenSys = NULL;			// 打开系统输出
	m_fnCloseSys = NULL;			// 关闭系统输出

	LoadOSDll();
}

CGfxSAMSUNG6410::~CGfxSAMSUNG6410()
{
	Close();
}

STDMETHODIMP CGfxSAMSUNG6410::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	UINT32 CamIFState = 0;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)
	{
		DP("[GFXSAMSUNG]Invalid call \n");
		return E_FAIL;
	}

	m_uAlpha = 0xf;

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
	
	DP("[GFXSAMSUNG]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);

	int i;
	for(i=0;m_pdwFourCCList[i];i++)
	{
		if(m_pdwFourCCList[i] == MAKEFOURCC('Y','V','1','2'))
			break;
	
	}
	if(!m_pdwFourCCList[i] )	return E_FAIL;
	m_dwFourCC = m_pdwFourCCList[i];
	g_cmm = init_cmm();//CMM_DisplayInit();
	memset(g_cmm, 0x00, m_dwWidth*m_dwHeight);
	memset(g_cmm + m_dwWidth*m_dwHeight, 0x80, m_dwWidth*m_dwHeight/2);
	
	init_windows0();
	if(g_cmm == NULL)
		return E_FAIL;
	
	set_video_alpha(m_uAlpha);	
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::Close()
{
	if(!m_bOpen)	return E_UNEXPECTED;
	uinit_windows0();
	uinit_cmm();
	g_cmm = NULL;
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::SetDstRect(const RECT *pRect)
{
	if(!pRect)	return E_INVALIDARG;
	RECT rTmp = m_rectDst;
	IntersectRect(&rTmp, pRect, &m_rectScn);
	rTmp.left &= ~(8-1);
	rTmp.top &= ~(8-1);
	rTmp.right &= ~(8-1);
	rTmp.bottom &= ~(8-1);

	if(((rTmp.left == m_rectDst.left)&&
		(rTmp.right == m_rectDst.right)&&
		(rTmp.top == m_rectDst.top)&&
		(rTmp.bottom == m_rectDst.bottom)) ||
		((rTmp.left == 0)&&
		(rTmp.right == 0)&&
		(rTmp.top == 0)&&
		(rTmp.bottom == 0)) 
		)
		return S_OK;
	m_rectDst = rTmp;
	uinit_windows0();
	init_windows0();
	set_video_alpha(m_uAlpha);	

	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::SetScnRect(const RECT *pRect)
{
	return S_OK;	
}

STDMETHODIMP CGfxSAMSUNG6410::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::SetAlpha(DWORD dwAlpha)
{
	if(	m_uAlpha == (unsigned char)dwAlpha*0xf/MAX_ALPHA_VAL)
		return S_OK;
	m_uAlpha = (unsigned char)dwAlpha*0xf/MAX_ALPHA_VAL;
	if(m_uAlpha > 0xf)
		m_uAlpha = 0xf;

	uinit_windows0();
	init_windows0();
	set_video_alpha(m_uAlpha);
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG6410::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG6410::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG6410::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG6410::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
		*ppBuf = g_cmm;
	if(pStride)
		*pStride = m_dwWidth;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::Flip(LONG num, DWORD dwFlags)
{
	if(m_rectDst.left == 0 && m_rectDst.right == 0 && m_rectDst.top == 0 && m_rectDst.bottom == 0)
		return S_OK;
	StartDisplay(m_rectDst.left,m_rectDst.top,m_rectDst.right - m_rectDst.left,m_rectDst.bottom - m_rectDst.top,g_cmm,2,m_dwWidth,m_dwHeight);
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxSAMSUNG6410::Update()
{
	if(m_bShow)
		SetAlpha(MAX_ALPHA_VAL);
	else
		SetAlpha(MIN_ALPHA_VAL);

	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG6410::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG6410::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG6410::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxSAMSUNG6410::IsOpen()
{
	return m_bOpen;
}

STDMETHODIMP CGfxSAMSUNG6410::OpenSys()
{
	if(m_fnCloseMedia && m_fnOpenSys)
	{
		m_fnCloseMedia() ;
		m_fnOpenSys() ;
		return S_OK;
	}
	
	return E_FAIL;
}

STDMETHODIMP CGfxSAMSUNG6410::OpenMedia()
{
	if(m_fnCloseSys && m_fnOpenMedia)
	{
		// 这里ws是视频区域的大小
		WINSIZE ws = {0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN)} ;
		m_fnCloseSys() ;
		m_fnOpenMedia(ws) ;
		return S_OK;
	}

	return E_FAIL;
}

IMcGfxOverlay *CreateOverlaySAMSUNG6410()
{
	return new CGfxSAMSUNG6410();
}
#endif
