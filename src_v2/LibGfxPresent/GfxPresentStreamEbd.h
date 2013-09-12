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

extern STDMETHODIMP LoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);
extern STDMETHODIMP UnLoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);


class CVRPresenterStreamEbd : 
	public IMcVideoPresenterStream,
	public CMcUnknown
{
public:
	CVRPresenterStreamEbd( CRITICAL_SECTION csDisplay,  
		VP_OPEN_STREAM_PARAMS *pOpenParams, HWND wnd, IMcVideoPresenterAdv* pProc);
	virtual ~CVRPresenterStreamEbd ();

	//IUNKNOWN
	DECLARE_IUNKNOWN

	STDMETHODIMP Open();
	STDMETHODIMP Close();
	STDMETHODIMP Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned);
	STDMETHODIMP Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData);
	STDMETHODIMP LockSurface(LPBYTE *ppData, DWORD *pdwStride);
	STDMETHODIMP UnlockSurface();
	STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP StartAnimation(VR_ANIMATION_PARAMS* pParms) {return E_NOTIMPL;};
	STDMETHODIMP  StopAnimation() {return E_NOTIMPL;};


	VP_OPEN_STREAM_PARAMS m_open_param;
private:

	IMcVideoPresenter* m_pPresent;
#ifdef LOADLIBRARY_ONCE
	static HMODULE m_dll;
#else
	HMODULE m_dll;
#endif
	bool m_bEnable;
	RECT m_rectSrc;
	RECT m_rectDst;
	RECT m_rectScn;

	CRITICAL_SECTION m_csDisplay; 
	bool m_bOpen;
	IMcVideoPresenterAdv* m_pProc;
	LONG m_Alpha;
	DWORD	m_SrcColorKey;
};


