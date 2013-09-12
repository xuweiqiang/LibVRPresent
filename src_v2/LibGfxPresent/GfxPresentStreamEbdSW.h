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
#include "../../inc/GfxVideoWindow.h"
#include "../../inc/McCom.h"
#include "../../inc/GfxPresentPrepare.h"

class CVRPresenterStreamEbdSW: 
	public IMcVideoPresenterStream,
	public CMcUnknown
{
public:
	CVRPresenterStreamEbdSW(LPBYTE pSurf, CRITICAL_SECTION* pCsDisplay,  VP_OPEN_STREAM_PARAMS *pOpenParams,IMcVideoPresenter**  ppPresenter);
	virtual ~CVRPresenterStreamEbdSW ();

	//IUNKNOWN
	DECLARE_IUNKNOWN

	STDMETHODIMP Open();
	STDMETHODIMP Close();
	STDMETHODIMP Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned);
	STDMETHODIMP Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData);
	STDMETHODIMP LockSurface(LPBYTE *ppData, DWORD *pdwStride);
	STDMETHODIMP UnlockSurface();
	STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);

	STDMETHODIMP StartAnimation(VR_ANIMATION_PARAMS* pParms);
	STDMETHODIMP StopAnimation();

	VP_OPEN_STREAM_PARAMS m_open_param;
private:

	LPBYTE m_pBuf;
	GfxPrepareDataV2 m_PrepareData;
	RECT m_rectSrc, m_rectDst, m_rectScn;
	bool m_bEnable;
	CRITICAL_SECTION* m_pCsDisplay; 
	DWORD m_alpha;
	VP_ZOOM_MODE m_zoom_mode;
	DWORD m_layerId;
	bool m_bOpen;
	VP_DEINTERLACE_MODE m_deinterlace;
	IMcVideoPresenter* m_pPresent;
	ICivmVideoContext* m_pVideoMixerContext;
	LONG m_lFlipVideoMode;
};


