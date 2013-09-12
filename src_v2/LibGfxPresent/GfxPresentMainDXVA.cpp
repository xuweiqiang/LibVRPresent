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
#include "../../inc/dp.h"
#include "tchar.h"
#include "GfxPresentProcSW.h"
#include "GfxPresentProcEbd.h"
#include "GfxPresentProcDXVA.h"

#pragma warning(disable : 4995)

HRESULT ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin)
{
	LONG lSrcNum = lSrcMax - lSrcMin;
	LONG lDstNum = lDstMax - lDstMin;

	if(lSrcVal>lSrcMax)
		lSrcVal = lDstMax;
	else if(lSrcVal < lSrcMin)
		lSrcVal = lDstMin;
	else		
		lSrcVal = lDstMin + (lSrcVal-lSrcMin)*lDstNum/lSrcNum;
	return S_OK;
}

RECT ScaleRectangle(const RECT& input, const RECT& src, const RECT& dst)
{
    RECT rect;

    UINT src_dx = src.right - src.left;
    UINT src_dy = src.bottom - src.top;

    UINT dst_dx = dst.right - dst.left;
    UINT dst_dy = dst.bottom - dst.top;

    // Scale the input rectangle by dst / src.

    rect.left   = input.left   * dst_dx / src_dx;
    rect.right  = input.right  * dst_dx / src_dx;
    rect.top    = input.top    * dst_dy / src_dy;
    rect.bottom = input.bottom * dst_dy / src_dy;

    return rect;
}


STDMETHODIMP LoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter)
{
	HRESULT hr = S_OK;
	h_dll = LoadLibrary(LIB_NAME_V1); 
	if(h_dll == NULL)
		return E_FAIL;

	if(ppPresenter)
	{
		fpCreateVideoPresenter fpProcCreate;
		fpProcCreate = (fpCreateVideoPresenter)GetProcAddress(h_dll, ENTRY_FUNC_NAME_V1);
		if(fpProcCreate==0)
			{hr = E_FAIL; goto RET;}
		*ppPresenter = fpProcCreate();
		if(*ppPresenter ==0)
			{hr = E_FAIL; goto RET;}
		return S_OK;
	RET:
		FreeLibrary(h_dll);
		return hr;

	}
	return S_OK;
}

STDMETHODIMP UnLoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter)
{
	if(ppPresenter && *ppPresenter)
	{
		IMcVideoPresenter* pPresent = *ppPresenter;
		pPresent->Close();
		pPresent->Release();
	}
	FreeLibrary(h_dll);
	return S_OK;
}

IMcVideoPresenterAdv* CreateVideoPresenterAdv()
{
	IMcVideoPresenterAdv* pObj = NULL;
	pObj = new CVRPresentOvlProcessorDXVA();
	return pObj;
}


