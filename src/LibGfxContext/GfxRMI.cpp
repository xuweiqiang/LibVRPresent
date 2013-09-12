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

#include "../../inc/GfxBuildDef.h"


#ifdef RMI
#include <windows.h>
#include "GfxRMI.h"

#pragma comment(lib, "Mmtimer.lib")

CGfxRMI::CGfxRMI()
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));
	m_hwnd = 0;
	m_dwOldWidth = m_dwOldHeight = 0;
	m_Oldhwnd = 0;
}

CGfxRMI::~CGfxRMI()
{
	Close();
}

STDMETHODIMP CGfxRMI::open_mae_driver()
{
	if((m_filehandle = CreateFile(TEXT("MAE1:"),
	      GENERIC_READ | GENERIC_WRITE,
	      0,
	      0,
	      OPEN_EXISTING,
	      FILE_ATTRIBUTE_NORMAL,
	      0)) == INVALID_HANDLE_VALUE)
	{
		m_filehandle = NULL;
		return E_FAIL;
	}

	return S_OK;
}


STDMETHODIMP CGfxRMI::close_mae_driver()
{
	//only close the driver if we actually opened it
	if (m_filehandle != NULL)
	{
		CloseHandle(m_filehandle);
		m_filehandle = NULL;
	}
	return S_OK;
}

STDMETHODIMP CGfxRMI::init_mae_driver()
{
	DRV_ESCAPE_MAE_CONFIG config;	
	HDC hDC;
	
	config.x = 0;
	config.y = 0;
	config.w = m_dwWidth;
	config.h = m_dwHeight;
	config.bpp = 32;
	config.colorkey  = RGB(0x00,0x08,0x00); 
	config.backcolor = RGB(0x04,0x04,0x04);
	config.fullscreen = 1;
	hDC = GetDC(NULL);
	ExtEscape(hDC,DRV_ESCAPE_MAE_ENABLE,sizeof(config),(char*)&config,0,NULL);
	ReleaseDC(NULL,hDC);
	return S_OK;
}


STDMETHODIMP CGfxRMI::mae_writetimestamp(LONGLONG pts, LONGLONG stc)
{
	ContextTime_t tContextTime;

	tContextTime.pts = pts;
	tContextTime.stc = stc;

	if (m_filehandle != NULL)
	{
	if ((DeviceIoControl(m_filehandle, AU1XXXMAE_WRITE_TIMESTAMP, (LPVOID) &tContextTime, sizeof(tContextTime),(LPVOID)&m_DB, sizeof(DebugBuffers), &m_BytesReturned ,NULL)) != TRUE)
		return E_FAIL;
	} 
	else
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CGfxRMI::mae_run(long ts)
{
	long time;
	time = ts;

	if (m_filehandle != NULL)
	{
		if ((DeviceIoControl(m_filehandle, AU1XXXMAE_RUN, (LPVOID)&time, sizeof(long),(LPVOID)&m_DB, sizeof(DebugBuffers), &m_BytesReturned ,NULL)) != TRUE)
			return E_FAIL;
	}
	else
		return E_FAIL;

	return S_OK;
}


STDMETHODIMP CGfxRMI::mae_setdeinterlacemode(DWORD deinterlacemode)
{
	if (m_filehandle != NULL)
	{
		if ((DeviceIoControl(m_filehandle, AU1XXXMAE_SETDECIMATIONFACTOR, (LPVOID) &deinterlacemode, sizeof(unsigned int),(LPVOID)&m_DB, sizeof(DebugBuffers),&m_BytesReturned ,NULL)) != TRUE)
			return E_FAIL;
	}
	else
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CGfxRMI::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	HRESULT ret = E_FAIL;
	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));


	ret = open_mae_driver();
	if(FAILED(ret))
		return ret;

	ret = mae_setdeinterlacemode(0x1);
	if(FAILED(ret))
		return ret;

	ret = init_mae_driver();
	if(FAILED(ret))
		return ret;
	
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	return S_OK;
}

STDMETHODIMP CGfxRMI::Close()
{

	HRESULT ret = close_mae_driver();
	if(FAILED(ret))
		return ret;

	return S_OK;
}

STDMETHODIMP CGfxRMI::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	return Update();
}

STDMETHODIMP CGfxRMI::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
	m_rectDst = *pRect;

	if ((DeviceIoControl(m_filehandle, AU1XXXMAE_SETDISPLAYRECT, (LPVOID)&m_rectDst, sizeof(RECT),(LPVOID)&m_DB, sizeof(DebugBuffers), &m_BytesReturned ,NULL)) != TRUE)
		return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CGfxRMI::SetScnRect(const RECT *pRect)
{
	if(pRect==0)
	{
		m_bScnClip = FALSE;
		return S_OK;
	}
	if(EqualRect(pRect,&m_rectScn))// && m_bScnClip)
		return S_OK;
	m_rectScn = *pRect;
	m_bScnClip = TRUE;
	return Update();
}


STDMETHODIMP CGfxRMI::GetScnRect(RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::GetWindowHandle(HWND& hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	if(m_bSrcCkey!=bEnable)
	{
		m_bSrcCkey = bEnable;
		if(bEnable)
			m_dwSrcCkey = dwSrcCkey;
		return Update();
	}
	if(bEnable && m_dwSrcCkey!=dwSrcCkey)
	{
		m_dwSrcCkey = dwSrcCkey;
		return Update();
	}
	return S_OK;
}

STDMETHODIMP CGfxRMI::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	return S_OK;
}

STDMETHODIMP CGfxRMI::Unlock(LONG num)
{
	return S_OK;
}


STDMETHODIMP CGfxRMI::Flip(LONG num, DWORD dwFlags)
{
	HRESULT ret = mae_writetimestamp(40,timeGetTime());
	if(FAILED(ret))
		return ret;
	
	if(!m_run)
	{
		ret = mae_run(0);
		if(FAILED(ret))
			return ret;
		m_run = 1;
	}

	return S_OK;
}

STDMETHODIMP CGfxRMI::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return S_OK;
}

STDMETHODIMP CGfxRMI::Update()
{
	return S_OK;
#if 0
	HRESULT hr = S_OK;
	RECT rs, rd;

	SetRect(&rs,0,0,m_dwWidth,m_dwHeight);
	IntersectRect(&rs,&rs,&m_rectSrc);
	rd = m_rectDst;
	// handle alignment
#if 0
	if(m_Caps.dwAlignSizeDest>1)
	{
		rd.right &= ~(m_Caps.dwAlignSizeDest-1);
		rd.left &= ~(m_Caps.dwAlignSizeDest-1);
	}
#endif
	if(m_bScnClip)
	{
		IntersectRect(&rd,&rd,&m_rectScn);
		if(!EqualRect(&rd,&m_rectDst))
		{				// translate the clipped destination rectangle to clipping for source rectangle
			RECT rn;	// normalization 
			int wd,ws,hd,hs;

			rn = rd;
			OffsetRect(&rn,-m_rectDst.left,-m_rectDst.top);
			wd = m_rectDst.right-m_rectDst.left;
			hd = m_rectDst.bottom-m_rectDst.top;
			ws = rs.right-rs.left;
			hs = rs.bottom-rs.top;
			if(wd<=0 || hd<=0)
				SetRect(&rs,0,0,0,0);
			else
			{
				rn.left = rn.left*ws/wd;
				rn.right = rn.right*ws/wd;
				rn.top = rn.top*hs/hd;
				rn.bottom = rn.bottom*hs/hd;
				OffsetRect(&rn,rs.left,rs.top);
				IntersectRect(&rs,&rs,&rn);
			}
		}
	}
//	m_rectBltSrc = rs;
	if(m_rectScn.right>0 && m_rectScn.bottom>0 && !IsRectEmpty(&rd))
	{
		m_rectBltDst.left = (float)rd.left/m_rectScn.right;
		m_rectBltDst.right = (float)rd.right/m_rectScn.right;
		m_rectBltDst.top = (float)rd.top/m_rectScn.bottom;
		m_rectBltDst.bottom = (float)rd.bottom/m_rectScn.bottom;
	}
	else
	{
		m_rectBltDst.left = (float)0;
		m_rectBltDst.right = (float)1.;
		m_rectBltDst.top = (float)0;
		m_rectBltDst.bottom = (float)1.;
	}
	m_bUpdateBltDst = TRUE;
//	if(m_bLastShow==FALSE && m_bShow==FALSE)
//		return S_OK;
//	m_bLastShow = m_bShow;
	return SUCCEEDED(hr) ? S_OK : E_FAIL;
#endif
}

STDMETHODIMP CGfxRMI::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMI::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;

	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = MAKEFOURCC('X','X','X','X');
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxRMI::IsOpen()
{
	return E_FAIL;
}


IMcGfxOverlay *CreateOverlayRMI()
{
	return new CGfxRMI;
}
#endif
