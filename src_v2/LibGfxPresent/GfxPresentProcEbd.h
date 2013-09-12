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
#include "../../inc/GfxPresentPrepare.h"
#include "../../src/LibGfxPresent/AlphaBlending.h"

#define MAX_STREAM 8

extern STDMETHODIMP LoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);
extern STDMETHODIMP UnLoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);

class CVRPresentProcessorEbd : 
	public IMcVideoPresenterAdv,
	public CMcUnknown
{
public:
	CVRPresentProcessorEbd ();
	virtual ~CVRPresentProcessorEbd ();

	//IUNKNOWN
	DECLARE_IUNKNOWN
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid == IID_IMcVideoPresenter)
		{
			LoadV1Library(m_dll,&m_pPresent);
			if(m_pPresent)
				return GetInterface(static_cast<IMcVideoPresenter*>(m_pPresent), ppvObj);
		}

		if(riid == IID_IMcVideoPresenterAdv)
			return GetInterface(static_cast<IMcVideoPresenterAdv*>(this), ppvObj);
		
		return CMcUnknown::NonDelegatingQueryInterface(riid, ppvObj);
	}

	//Interface 3.0
	STDMETHODIMP Open(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize);
	STDMETHODIMP Close();
	STDMETHODIMP Render(DWORD dwRenderFlags);
	STDMETHODIMP Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
		IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned);
	STDMETHODIMP Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
		IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData);

	STDMETHODIMP CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream);
	STDMETHODIMP DestroyStream(IN IMcVideoPresenterStream *ppStream);

private:
	int GetStreamNumber();
	bool m_Init;
	HMODULE m_dll;
	struct VP_OpenOptionsAdv m_openParams;
	IMcVideoPresenter* m_pPresent;

	typedef struct
	{
		bool valid;
		VP_OPEN_STREAM_PARAMS param;
		IMcVideoPresenterStream *pStream;
	}SSTREAM_INFO;
	SSTREAM_INFO m_pStmArrar[MAX_STREAM];

	DWORD m_streamCount;
	CRITICAL_SECTION m_csDisplay; 

	RECT m_rDst;		// destination rectangle to store the image, relative to video window/port.
	RECT m_rWnd;
	bool m_bcanChanged;

	STDMETHODIMP CheckCanvasChange();

};



