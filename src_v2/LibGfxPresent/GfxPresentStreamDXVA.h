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
#include "../../inc/McCom.h"
#include "../../inc/GfxPresentPrepare.h"
#include "d3d9.h"
#include "../../inc/GfxVideoWindow.h"

extern STDMETHODIMP LoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);
extern STDMETHODIMP UnLoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);


class CVRPresenterStreamDXVA : 
	public IMcVideoPresenterStream,
	public CMcUnknown
{
public:
	CVRPresenterStreamDXVA( CRITICAL_SECTION csDisplay,  VP_OPEN_STREAM_PARAMS *pOpenParams, IDirect3DSurface9* pStream);
	virtual ~CVRPresenterStreamDXVA ();

	//IUNKNOWN
	DECLARE_IUNKNOWN

	STDMETHODIMP Open();
	STDMETHODIMP Close();
	STDMETHODIMP Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned);
	STDMETHODIMP Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData);
	STDMETHODIMP LockSurface(LPBYTE *ppData, DWORD *pdwStride);
	STDMETHODIMP UnlockSurface();
	STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP Update();

	VP_OPEN_STREAM_PARAMS m_open_param;
private:

	bool m_bEnable;
	RECT m_rectSrc;
	RECT m_rectDst;
	RECT m_rectScn;
	DWORD m_Alpha;
	VP_ZOOM_MODE m_zoom_mode;
	CRITICAL_SECTION m_csDisplay; 
	IDirect3DSurface9* m_pStream;
	GfxPrepareDataV2 m_PrepareData;
	VP_DEINTERLACE_MODE m_deinterlace;
	IMcVideoWindow * m_pVideoWnd;
	bool m_bOpen;
};


