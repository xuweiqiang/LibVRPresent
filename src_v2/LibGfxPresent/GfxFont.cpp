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

class CVRFONT: 
	public IMcFont,
	public CMcUnknown
{
public:
	CVRFONT();
	virtual ~CVRFONT();

	//IUNKNOWN
	DECLARE_IUNKNOWN
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid == IID_IMcFont)
			return GetInterface(static_cast<IMcFont*>(this), ppvObj);
		else
			return CMcUnknown::NonDelegatingQueryInterface(riid, ppvObj);
	}

	STDMETHODIMP Open(VP_FONT_OPEN_PARAMS* pOptions, DWORD dwOptionSize) 
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP Close() 
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetNormalFont(unsigned char font,unsigned char height, unsigned char  orient, unsigned char weight, unsigned char italic, unsigned char underline, unsigned char  strikeout, unsigned char  charSet, unsigned char* fontName)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP Printf(bool replace, RECT* pDst, unsigned char* string, DWORD duration, unsigned char* pStrId)
	{
		return E_NOTIMPL;
	}

};

CVRFONT::CVRFONT():
	CMcUnknown(TEXT("GfxFont"),0)
{
	return;
}

CVRFONT::~CVRFONT()
{
	return;
}
IMcFont*	CreateVideoPresenterFont()
{
	return new CVRFONT;
}


