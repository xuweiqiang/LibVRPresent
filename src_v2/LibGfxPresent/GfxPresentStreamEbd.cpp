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

#include "GfxPresentStreamEbd.h"
//#include "../../inc/dp.h"


#ifdef LOADLIBRARY_ONCE
HMODULE CVRPresenterStreamEbd::m_dll = 0;
#endif

CVRPresenterStreamEbd::CVRPresenterStreamEbd(CRITICAL_SECTION csDisplay,  
VP_OPEN_STREAM_PARAMS *pOpenParams, HWND wnd, IMcVideoPresenterAdv* pProc):
m_bEnable(false),
m_csDisplay(csDisplay),
m_bOpen(false),
m_pPresent(NULL),
m_pProc(pProc)
{
	if(pOpenParams)
		memcpy(&m_open_param, pOpenParams, sizeof(m_open_param));

	memset(&m_rectDst, 0, sizeof(m_rectDst));
	memset(&m_rectSrc, 0, sizeof(m_rectSrc));
	memset(&m_rectScn, 0, sizeof(m_rectScn));
	return;
}

CVRPresenterStreamEbd::~CVRPresenterStreamEbd()
{
	UnLoadV1Library(m_dll, &m_pPresent);
	return;
}

STDMETHODIMP CVRPresenterStreamEbd::Open()
{
	HRESULT hr = S_OK; 
	if(m_bOpen)	return E_FAIL;

	EnterCriticalSection(&m_csDisplay);

	hr = LoadV1Library(m_dll, &m_pPresent);
	if(FAILED(hr))
	{
		LeaveCriticalSection(&m_csDisplay);
		return hr;
	}
	VP_OpenOptions opt;
	ZeroMemory(&opt, sizeof(opt));
	opt.dwWidth = m_open_param.dwWidth;
	opt.dwHeight = m_open_param.dwHeight;
	opt.dwFourCC = m_open_param.dwFourCC;
	opt.dwAspectX = m_open_param.dwAspectX;
	opt.dwAspectY = m_open_param.dwAspectY;
	if(m_open_param.type == eMAINSTREAM)
	{
#ifdef TITAN_II	
		opt.dwSurfType  = VP_OPEN_FLAG_TITIAN_II;
#else
		opt.dwSurfType = VP_OPEN_FLAG_DDRAW;
#endif
	}
	else
	{
#ifdef TITAN_II	
		opt.dwSurfType  = VP_OPEN_FLAG_TITIAN_II;
		opt.dwFlags = VP_OPEN_FLAG_OSD;
#else
		opt.dwSurfType = VP_OPEN_FLAG_DDRAW_OSD;
#endif
	}
	hr = m_pPresent->Open(&opt,sizeof(opt));
	if(FAILED(hr))		
	{
		LeaveCriticalSection(&m_csDisplay);
		return hr;
	}


	m_Alpha = (LONG)(m_open_param.fAlpha* MAX_ALPHA_VAL);
	if(m_Alpha  != MAX_ALPHA_VAL)
	{
		m_pPresent->Set(EMC_PROPSET_GFXCTXT, MC_PROPID_VP_ALPHA, 0, 0, 
			&m_Alpha , sizeof(m_Alpha ));
	}

	if(m_open_param.type == eSUBSTREAMS)
	{
		m_SrcColorKey = m_open_param.dwTranColor;
		m_pPresent->Set(EMC_PROPSET_GFXCTXT, MC_PROPID_VP_COLORKEYSRC, 0, 0, 
				&m_SrcColorKey, sizeof(m_SrcColorKey));
 	}
	LeaveCriticalSection(&m_csDisplay);
	m_bOpen = TRUE;
	return S_OK;
};

STDMETHODIMP CVRPresenterStreamEbd::Close()
{
	if(!m_bOpen)	return E_FAIL;

	EnterCriticalSection(&m_csDisplay);

	if(m_pPresent)
	{
		m_pPresent->Close();
		m_pPresent->Release();
		m_pPresent = NULL;
	}
	LeaveCriticalSection(&m_csDisplay);

	m_bOpen = FALSE;
	return S_OK;
};
	
STDMETHODIMP CVRPresenterStreamEbd::Get(IN REFGUID guidPropSet, IN DWORD dwPropID,
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	if(!m_bOpen)	return E_FAIL;

	if(dwPropID == MC_PROPID_VP_RECTDST && pPropData)
	{
		*reinterpret_cast<RECT *>(pPropData) = m_rectDst;
		return S_OK;
	}
	else if(dwPropID == MC_PROPID_VP_VP_HND && pPropData)
	{
		*reinterpret_cast<IMcVideoPresenter**>(pPropData) = m_pPresent;
		return S_OK;
	}
	else if(dwPropID == MC_PROPID_VP_ENABLE  && pPropData)
	{
		*reinterpret_cast<bool*>(pPropData) = m_bEnable;
		return S_OK;
	}
	else if(dwPropID == MC_PROPID_VP_PREFERRED_FOURCC && pPropData)
	{
#if defined(MT3353) || defined(PRIMAII) 
		if(m_open_param.type == eSUBSTREAMS)
		{
			*reinterpret_cast<DWORD*>(pPropData) = MAKEFOURCC('R','5','6','5');
			return S_OK;
		}
#else
		return E_NOTIMPL;
#endif
	}
	else
		return (m_pPresent->Get(guidPropSet, dwPropID, pInstanceData, cbInstanceData, pPropData, cbPropData, pcbReturned));

}

STDMETHODIMP CVRPresenterStreamEbd::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, 
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	if(!m_bOpen)	return E_FAIL;

	if(dwPropID == MC_PROPID_VP_RECTDST && pPropData)
	{
		m_rectDst = *(RECT*)pPropData;
		m_pProc->Render(0);
		return S_OK;
	}
	if(dwPropID == MC_PROPID_VP_ENABLE && pPropData)
	{
		m_bEnable = *(bool*)pPropData;
		m_pProc->Render(0);
		return S_OK;
	}

	return (m_pPresent->Set(guidPropSet, dwPropID, pInstanceData, cbInstanceData, pPropData, cbPropData));
}

STDMETHODIMP CVRPresenterStreamEbd::LockSurface(LPBYTE *ppData, DWORD *pdwStride)
{
	if(!m_bOpen)	return E_FAIL;
	return S_OK;
}
STDMETHODIMP CVRPresenterStreamEbd::UnlockSurface()
{
	if(!m_bOpen)	return E_FAIL;
	return  S_OK;
}

STDMETHODIMP CVRPresenterStreamEbd::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)	return E_FAIL;

	return m_pPresent->Present(ppbPlanes, pdwStrides, dwFlags);
}

