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
#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/GfxContext.h"
#include "../LibGfxContext/GfxOverlay.h"
#include "../../inc/McCom.h"


class CVRPresentDUMMY : 
	public IMcVideoPresenter ,
	public CMcUnknown
{
public:
	CVRPresentDUMMY();
	virtual ~CVRPresentDUMMY();

	//IUNKNOWN
	DECLARE_IUNKNOWN
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid == IID_IMcVideoPresenter)
			return GetInterface(static_cast<IMcVideoPresenter*>(this), ppvObj);
		else
			return CMcUnknown::NonDelegatingQueryInterface(riid, ppvObj);
	}
	//Interface 2.5
	STDMETHODIMP Open(VP_OpenOptions *pOptions, DWORD dwOptionSize);
	STDMETHODIMP Close();
	STDMETHODIMP  CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam);
	STDMETHODIMP  DestroySurface(VR_SURFACE_TYPE type);
	STDMETHODIMP Reset();
	STDMETHODIMP SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk);
	STDMETHODIMP SetVBI(VP_VIDEOBUFFER_INFO *pVBI);
	STDMETHODIMP SetFPS(DWORD dwFPS);
	STDMETHODIMP GetBackBuffers(DWORD *pdwBackBuffers);
	STDMETHODIMP Restore();
	STDMETHODIMP GetStatus(DWORD *pdwStatus);
	STDMETHODIMP GetCaps(VP_CAPS *pCaps);

	STDMETHODIMP Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStride, DWORD dwFlags);
	STDMETHODIMP PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags);
	STDMETHODIMP Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStride, DWORD dwFlags);
	STDMETHODIMP PresentBuffer(LONG lNum, DWORD dwFlags);
	STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP PresentRect(LPBYTE *ppbPlanes, DWORD *pdwStrides,DWORD dwFlags ,RECT* pSrcRgn, RECT* pDstRgn); 


	STDMETHODIMP LockSurface( LPBYTE *ppData, DWORD *pdwStride) 
	{
		return S_OK;
	}

	STDMETHODIMP UnlockSurface() 
	{
		return S_OK;
	}

	STDMETHODIMP FlipSurface(){
		return S_OK;
	}

};


#define VIDEO_PRESENTER_NAME TEXT("Video Presenter")

CVRPresentDUMMY::CVRPresentDUMMY() :
	CMcUnknown(VIDEO_PRESENTER_NAME,0)
{
	return;
}

CVRPresentDUMMY::~CVRPresentDUMMY()
{
	return;
}

STDMETHODIMP CVRPresentDUMMY::Open(VP_OpenOptions *pOptions, DWORD dwOptionSize)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::Close()
{
	return S_OK;
}

STDMETHODIMP  CVRPresentDUMMY::CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam)
{
	return S_OK;
}

STDMETHODIMP  CVRPresentDUMMY::DestroySurface(VR_SURFACE_TYPE type)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::Reset()
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentDUMMY::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
	DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
	DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	return S_OK;
}

HRESULT CVRPresentDUMMY::Restore()
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return S_OK;
}

HRESULT CVRPresentDUMMY::PresentRect(LPBYTE *ppbPlanes, DWORD *pdwStrides,DWORD dwFlags ,RECT* pSrcRgn, RECT* pDstRgn) 
{
	return S_OK;
}


STDMETHODIMP CVRPresentDUMMY::PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::PresentBuffer(LONG lNum, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::SetVBI(VP_VIDEOBUFFER_INFO *pVBI)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::SetFPS(DWORD dwFPS)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::GetBackBuffers(DWORD *pdwBackBuffers)
{
	*pdwBackBuffers = 1;
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::GetStatus(DWORD *pdwStatus)
{
	return S_OK;
}

STDMETHODIMP CVRPresentDUMMY::GetCaps(VP_CAPS *pCaps)
{
	return S_OK;
}

IMcVideoPresenter *CreateVideoPresenter()
{
	return new CVRPresentDUMMY();
}

/*
IMcVideoPresenterAdv *CreateVideoPresenterAdv()
{
	return NULL;
}
*/
IMcVideoPresenterAnimation*	CreateVideoAnimation()
{
	return NULL;
}

