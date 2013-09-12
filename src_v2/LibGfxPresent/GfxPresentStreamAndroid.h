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

#ifndef _GFXPRESENTSTREAMANDROID_H_
#define _GFXPRESENTSTREAMANDROID_H_

#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/GfxVideoWindow.h"
#include "../../inc/McCom.h"
#include "../../inc/GfxPresentPrepare.h"
#include "../../src/LibGfxContext/GfxOverlay.h"

class CVRStmAndroid : 
	public IMcVideoPresenterStream,
	public CMcUnknown
{
public:
	CVRStmAndroid(VP_OPEN_STREAM_PARAMS *pOpenParams);
	virtual ~CVRStmAndroid ();

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
	IMcGfxOverlay *m_pStream;
    GfxOverlayInfo m_stream_info;
	GfxPrepareDataV2 m_PrepareData;
	bool m_bEnable;
	bool m_bOpen;
	RECT m_rDst;
	VP_DEINTERLACE_MODE m_deinterlace;
};

#endif

