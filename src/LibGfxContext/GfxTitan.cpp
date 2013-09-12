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


#ifdef TITAN
#include <windows.h>
#include "GfxTitan.h"

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib,"..\\..\\3rdparty\\TitianEVB\\GenericVideoRender.lib")

CGfxTitan::CGfxTitan()
{
	m_pVr = 0;
	m_pBackBuffer = 0;
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	m_bUpdateBltDst = FALSE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));
	m_hwnd = 0;
	m_dwOldWidth = m_dwOldHeight = 0;
	m_Oldhwnd = 0;
	m_sx = 1;
	m_sy = 1;
	m_bUseDSP = TRUE; // use the DSP
	m_dwSurfacePad = 0;
}

CGfxTitan::~CGfxTitan()
{
	Close();
}

STDMETHODIMP CGfxTitan::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	if(m_bUseDSP)
	{	// hack because of poor DSP memory allocation
		if(dwWidth*dwHeight >= 704*480)
			m_dwMaxBuffers = min(4,dwBuffers); //dwBuffers;
		else
			m_dwMaxBuffers = min(8,dwBuffers); //dwBuffers;
	}
	else
		m_dwMaxBuffers = dwBuffers;
	return CreateSurfaces();
}

STDMETHODIMP CGfxTitan::Close()
{
	ReleaseSurfaces();
	return S_OK;
}

STDMETHODIMP CGfxTitan::SetDisplayMode(int disp_mode)
{
	VR_RESULT hr;
	static const VR_DISP_MODE disp_mode_tbl[3] = {VR_DISP_NORMAL, VR_DISP_FULL, VR_DISP_JUST}; 
	VR_DISP_MODE_DESC mode = {VR_SCR_NORMAL, disp_mode_tbl[disp_mode]};

	hr = CVR_SetDisplayMode(m_pVr,mode);
	return hr==VR_OK ? S_OK : E_FAIL;
}

static void clearoutsiderect(unsigned char *pb, int width, int height, int stride, int val, int xl, int xr, int yt, int yb)
{
	int j;

	for(j=0;j<yt;j++)
	{
		memset(pb,val,xl+width+xr);
		pb += stride;
	}
	for(;j<yt+height;j++)
	{
		memset(pb,val,xl);
		memset(pb+xl+width,val,xr);
		pb += stride;
	}
	for(;j<yt+height+yb;j++)
	{
		memset(pb,val,xl+width+xr);
		pb += stride;
	}
}

STDMETHODIMP CGfxTitan::CreateSurfaces()
{
	HRESULT hr;
	int i;
	DWORD alignx,aligny,height,width,buffers,hpad,wpad;

	if(m_dwWidth==0 || m_dwHeight==0 /*|| m_hwnd==0*/)
		return E_FAIL;
	if(m_pVr && m_dwWidth==m_dwOldWidth && m_dwHeight==m_dwOldHeight && m_hwnd==m_Oldhwnd)
		return S_OK;
	if(m_pVr)
		ReleaseSurfaces();
	m_dwOldWidth = m_dwWidth;
	m_dwOldHeight = m_dwHeight;

	/* create interface*/
	hr = CVR_CreateVideoRender(&m_pVr, m_hwnd);
	if(hr!=VR_OK)
		return E_FAIL;

	m_Oldhwnd = m_hwnd;

	if(m_dwHeight<480)
		m_sy = 2;		// double the height!
	else
		m_sy = 1;
	if(m_bUseDSP)
	{	// DSP renderer
		alignx = 16;	// 16 is required (spec doesn't specify)
		aligny = 4;
		hr = CVR_EscapeCtrl(m_pVr,TITAN_DSP_RENDERER,0,0,0);
		if(hr!=VR_OK)
		{
			// do something here.

		}
	}
	else
	{
		alignx = 4; /* actually 2, but we prefer this for our averaging ops */
		aligny = 1;
	}
	buffers = m_dwMaxBuffers+1;
	wpad = ~(m_dwWidth-1) & (alignx-1);
	width = m_dwWidth+wpad;
	height = m_dwHeight*m_sy;
	hpad = ~(height-1) & (aligny-1);
	height = height+hpad;
	//width = width + (width&0xf);
	//height = height + (height&0xf);
	SetDisplayMode(1);
	m_iYPitch = width;
	m_iUVPitch = m_iYPitch>>1;
	hr = CVR_SetVideoFormat(m_pVr, 0, width, height, 0, &m_iYPitch, &m_iUVPitch);
	m_pBackBuffer = new LPVR_FRAME_SURFACE[buffers];
	ZeroMemory(m_pBackBuffer,sizeof(m_pBackBuffer[0])*buffers);
	for(i=0;i<(signed)buffers;i++)
	{
		hr = CVR_CreateSurface(m_pVr,&m_pBackBuffer[i],1);
		if(hr!=VR_OK)
			break;
	}
	if(i==0)
	{
		delete[] m_pBackBuffer;
		m_pBackBuffer = 0;
		CVR_DeleteVideoRender(m_pVr);
		m_pVr = 0;
		return E_FAIL;
	}
	if(i>1)
	{		// make sure we have at least one surface available for background scratch, otherwise hang.
		CVR_DestroySurface(m_pVr,m_pBackBuffer[--i]);
		ZeroMemory(&m_pBackBuffer[i],sizeof(m_pBackBuffer[i]));
	}
	m_dwBackBuffers = i;
	
	// clear out buffers
	unsigned char *pb;
	LONG lstride;
	int xl,xr,yt,yb,ht;

	yt = hpad>>1&~1;
	yb = hpad - yt;
	xl = 0; // (wpad>>1)&~3;
	xr = wpad -xl;
	ht = m_dwHeight*m_sy;
	m_dwSurfacePad = 0;
	for(i=0;i<(signed)m_dwBackBuffers;i++)
	{
		if(SUCCEEDED(Lock(i, (LPVOID *)&pb, &lstride, 0)))
		{
			clearoutsiderect(pb,m_dwWidth,ht,lstride,0,xl,xr,yt,yb);
			clearoutsiderect(pb+height*width,m_dwWidth>>1,ht>>1,lstride>>1,128,xl>>1,xr>>1,yt>>1,yb>>1);
			clearoutsiderect(pb+height*width+(height*width>>2),m_dwWidth>>1,ht>>1,lstride>>1,128,xl>>1,xr>>1,yt>>1,yb>>1);
			Unlock(i);
		}
	}
	return S_OK;
}

STDMETHODIMP CGfxTitan::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	return Update();
}

STDMETHODIMP CGfxTitan::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
	m_rectDst = *pRect;
	return Update();
}

STDMETHODIMP CGfxTitan::SetScnRect(const RECT *pRect)
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

STDMETHODIMP CGfxTitan::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxTitan::SetWindowHandle(HWND hwnd)
{
#if 0
	if(m_hwnd!=hwnd)
	{
		m_hwnd = hwnd;
		Update();
	}
#endif
	return S_OK;
}

STDMETHODIMP CGfxTitan::GetWindowHandle(HWND& hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
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

STDMETHODIMP CGfxTitan::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

VR_FRAME_SURFACE *CGfxTitan::GetSurface(LONG num)
{	
	if(num>=0 && num<(signed)m_dwBackBuffers)
		return m_pBackBuffer[num];
	return 0;
}

STDMETHODIMP CGfxTitan::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	HRESULT hr;
	VR_FRAME_SURFACE *pSurf;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	hr = CVR_LockSurface(m_pVr,pSurf);
	if(hr!=VR_OK)
		return E_FAIL;
	if(ppBuf)
		*ppBuf = pSurf->pFrameBuf + m_dwSurfacePad;
	if(pStride)
		*pStride = pSurf->Y_stride;
	if(pPixCount)
		*pPixCount = 12;
	return S_OK;
}

STDMETHODIMP CGfxTitan::Unlock(LONG num)
{
	HRESULT hr;
	VR_FRAME_SURFACE *pSurf;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	hr = CVR_UnlockSurface(m_pVr,pSurf);
	return hr==VR_OK ? S_OK : E_FAIL;
}



STDMETHODIMP CGfxTitan::Flip(LONG num, DWORD dwFlags)
{
	VR_FRAME_SURFACE *pSurf;
	HRESULT hr;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(m_bUpdateBltDst)	// this should be done all at once in the same thread...
	{
		hr = CVR_SetDstRect(m_pVr,m_rectBltDst);
		m_bUpdateBltDst = FALSE;
	}
	hr = CVR_Render(m_pVr,pSurf,TRUE);
	hr = CVR_WaitComplete(m_pVr);
	return hr==VR_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CGfxTitan::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return S_OK;
}

STDMETHODIMP CGfxTitan::Update()
{
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
}

STDMETHODIMP CGfxTitan::ReleaseSurfaces()
{
	if(m_pBackBuffer)
	{
		int i;

		for(i=0;i<(signed)m_dwBackBuffers;i++)
			CVR_DestroySurface(m_pVr,m_pBackBuffer[i]);
		delete[] m_pBackBuffer;
		m_pBackBuffer = 0;
	}
	if(m_pVr)
	{
		CVR_DeleteVideoRender(m_pVr);
		m_pVr = 0;
	}
	m_dwBackBuffers = 0;
	return S_OK;
}


STDMETHODIMP CGfxTitan::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitan::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(m_pBackBuffer==0 || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	if(m_sy==2)
		pInfo->dwFourCC = MAKEFOURCC('Y','U','2','4');
	else
		pInfo->dwFourCC = MAKEFOURCC('Y','U','1','2');
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxTitan::IsOpen()
{
	return m_pBackBuffer ? S_OK : S_FALSE;
}


IMcGfxOverlay *CreateOverlayTitan()
{
	return new CGfxTitan;
}
#endif


