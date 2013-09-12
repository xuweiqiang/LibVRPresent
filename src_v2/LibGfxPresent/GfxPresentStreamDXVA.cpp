//===================================== ========================================
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

#include "GfxPresentStreamDXVA.h"
#include "../../inc/dp.h"


CVRPresenterStreamDXVA::CVRPresenterStreamDXVA(CRITICAL_SECTION csDisplay,  VP_OPEN_STREAM_PARAMS *pOpenParams, IDirect3DSurface9* pStream):
m_bEnable(false),
m_csDisplay(csDisplay),
m_pStream(pStream),
m_Alpha(MAX_ALPHA_VAL),
m_pVideoWnd(NULL),
m_deinterlace(VP_DEINTERLACE_None),
m_zoom_mode(VP_ZOOM_MODE_STRETCH),
m_bOpen(false)
{
	if(pOpenParams)
		memcpy(&m_open_param, pOpenParams, sizeof(m_open_param));
	SetRect(&m_rectSrc, 0,0, m_open_param.dwWidth, m_open_param.dwHeight);
	SetRect(&m_rectDst, 0,0, m_open_param.dwWidth, m_open_param.dwHeight);
	return;
}

CVRPresenterStreamDXVA::~CVRPresenterStreamDXVA()
{
	if(m_pVideoWnd)
	{
		m_pVideoWnd->Release();
		m_pVideoWnd = NULL;
	}

	return;
}

STDMETHODIMP CVRPresenterStreamDXVA::Open()
{
	if(m_bOpen)	return E_UNEXPECTED;
	
	if(!m_pVideoWnd )
		m_pVideoWnd = CreateVideoWindow();
	if(!m_pVideoWnd )	return E_FAIL;

	VE_Options tVEOpen;
	ZeroMemory(&tVEOpen, sizeof(tVEOpen));
	tVEOpen.mode = (VE_MODE)m_zoom_mode;
	tVEOpen.rSrcClip.right = tVEOpen.rDst.right = tVEOpen.rSrc.right = m_open_param.dwWidth;
	tVEOpen.rSrcClip.bottom = tVEOpen.rDst.bottom =  tVEOpen.rSrc.bottom = m_open_param.dwHeight;
	tVEOpen.aspect.dwAspectX = m_open_param.dwAspectX;
	tVEOpen.aspect.dwAspectY = m_open_param.dwAspectY;
	HRESULT hr = m_pVideoWnd->Open(&tVEOpen,sizeof(tVEOpen));
	if(FAILED(hr)) 	return E_FAIL;

	m_bOpen = true;
	return S_OK;	
}

STDMETHODIMP CVRPresenterStreamDXVA::Close()
{
	if(!m_bOpen)	return E_FAIL;

	
	if(m_pVideoWnd)
	{
		m_pVideoWnd->Close();
		m_pVideoWnd->Release();
		m_pVideoWnd = NULL;
	}
	m_bOpen = false;
	return S_OK;
}
	
STDMETHODIMP CVRPresenterStreamDXVA::Get(IN REFGUID guidPropSet, IN DWORD dwPropID,
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	if(!m_bOpen)	return E_UNEXPECTED;
	
	switch(dwPropID)
	{
		case MC_PROPID_VP_RECTSRC:
			{
				*reinterpret_cast<RECT *>(pPropData) = m_rectSrc;
				if(pcbReturned)
					*pcbReturned = sizeof(RECT);
				return S_OK;
			}
		case MC_PROPID_VP_RECTDST:
			{
				if(pPropData==0)
					return E_POINTER;
				*reinterpret_cast<RECT *>(pPropData) = m_rectDst;
				if(pcbReturned)
					*pcbReturned = sizeof(m_rectDst);
				return S_OK;
			}
		case MC_PROPID_VP_ENABLE:
			{
				if(pPropData==0)
					return E_POINTER;
				*reinterpret_cast<bool *>(pPropData) = m_bEnable;
				if(pcbReturned)
					*pcbReturned = sizeof(m_bEnable);
				return S_OK;
			}
		case MC_PROPID_VP_ALPHA:
			{
				if(pPropData==0)
					return E_POINTER;
				*reinterpret_cast<DWORD *>(pPropData) = m_Alpha;
				if(pcbReturned)
					*pcbReturned = sizeof(DWORD);
				return S_OK;
			}
		case MC_PROPID_VP_SCAN_MODE:
			{
				if(pPropData==0)
					return E_POINTER;
				*reinterpret_cast<VP_DEINTERLACE_MODE*>(pPropData) = m_deinterlace;
				if(pcbReturned)
					*pcbReturned = sizeof(m_deinterlace);
				return S_OK;
			}

		default:
			break;
	}
	return E_NOTIMPL;
}

STDMETHODIMP CVRPresenterStreamDXVA::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, 
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	HRESULT hr = S_OK;
	if(!m_bOpen)	return E_UNEXPECTED;
	switch (dwPropID)
	{
	case MC_PROPID_VP_RECTSRC:
		{
			RECT *pr = reinterpret_cast<RECT *>(pPropData);
			m_rectSrc = *pr;
			hr = m_pVideoWnd->Set(IID_IMcVideoPresenter, VE_COMMAND_SRCRECT,0,0,pr, sizeof(RECT));
			hr = Update();
			if(FAILED(hr))	return hr;

		}
		break;
	case MC_PROPID_VP_RECTDST:
		{
			RECT* pr = reinterpret_cast<RECT *>(pPropData);
			m_rectDst = *pr;
			hr = m_pVideoWnd->Set(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,pr, sizeof(RECT));
			if(FAILED(hr))	return hr;

			hr = Update();
			if(FAILED(hr))	return hr;

		}
		break;
	case MC_PROPID_VP_ENABLE:
		{
			m_bEnable = *(reinterpret_cast<bool*>(pPropData));
		}
		break;
	case MC_PROPID_VP_COLORKEYSRC:
		{
			DWORD* pr;
			pr = reinterpret_cast<DWORD*>(pPropData);
		}
		break;
	case MC_PROPID_VP_ALPHA:
		{
			m_Alpha = *(reinterpret_cast<DWORD*>(pPropData)); 
		}
		break;
	case MC_PROPID_VP_SURFACE:
		{
			m_pStream = reinterpret_cast<IDirect3DSurface9*>(pPropData);
		}
		break;
	case MC_PROPID_VP_DEINTERLACE_MODE:
	case MC_PROPID_VP_SCAN_MODE:
		{
			m_deinterlace = *(reinterpret_cast<VP_DEINTERLACE_MODE*>(pPropData));
		}
		break;
	case MC_PROPID_VP_ZOOM:
		{
			m_pVideoWnd->Set(IID_IMcVideoPresenter, VE_COMMAND_MODE,0,0,pPropData, sizeof(VP_ZOOM_MODE));
			hr = Update();
			if(FAILED(hr))	return hr;
			m_zoom_mode = *reinterpret_cast<VP_ZOOM_MODE*>(pPropData);			
		}
		break;
		
	default:
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CVRPresenterStreamDXVA::LockSurface(LPBYTE *ppData, DWORD *pdwStride)
{
	HRESULT hr;
	D3DLOCKED_RECT lr;
	hr = m_pStream->LockRect(&lr, NULL, D3DLOCK_NOSYSLOCK);
	if (FAILED(hr)) 
	{ 
		return hr;
	}

	if(ppData)
		*ppData = (LPBYTE)lr.pBits;
	if(pdwStride )
		*pdwStride = lr.Pitch;
	
	return S_OK;
}

STDMETHODIMP CVRPresenterStreamDXVA::UnlockSurface()
{
	if(!m_pStream)
		return E_FAIL;
	return  (m_pStream->UnlockRect());
}

STDMETHODIMP CVRPresenterStreamDXVA::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	LPBYTE pBuf = NULL;
	DWORD stride = 0;
	HRESULT hr = S_OK;
	hr = LockSurface(&pBuf , &stride);
	if(FAILED(hr))	return hr;


	DWORD srcWidth[3];
	DWORD srcHeight[3];
	srcWidth[0] = m_open_param.dwWidth;
	srcWidth[1] = srcWidth[2] = m_open_param.dwWidth>>1;

	srcHeight[0] = m_open_param.dwHeight;
	srcHeight[1] = srcHeight[2] = m_open_param.dwHeight>>1;

	if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
		hr  = m_PrepareData.PrepareBufferYV12(pBuf, stride, ppbPlanes, pdwStrides, 
		srcWidth, srcHeight, dwFlags, 0,0,0,m_deinterlace);
	else if(m_open_param.dwFourCC == MAKE_FOURCC('R','5','6','5'))
	{
		
	}

	hr = UnlockSurface();
	return  hr;
}


STDMETHODIMP CVRPresenterStreamDXVA::Update()
{
	HRESULT ret = S_OK;
	RECT rSrc, rDst;
	ret = m_pVideoWnd->Get(IID_IMcVideoPresenter, VE_COMMAND_SRCRECT,0,0,&rSrc, sizeof(rSrc),NULL);
	if(FAILED(ret))
		return ret;
	m_rectSrc = rSrc;
	
	ret = m_pVideoWnd->Get(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&rDst, sizeof(rDst),NULL);
	if(FAILED(ret))
		return ret;

	m_rectDst = rDst;
	return S_OK;
}


