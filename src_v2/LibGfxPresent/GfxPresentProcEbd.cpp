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
#include "GfxPresentProcEbd.h"
#include "GfxPresentStreamEbd.h"

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter2.0")

CVRPresentProcessorEbd ::CVRPresentProcessorEbd () :
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_pPresent(NULL),
	m_Init(false),
	m_streamCount(0)
{
	ZeroMemory(&m_dll, sizeof(m_dll));	
	ZeroMemory(&m_openParams, sizeof(m_openParams));	
	ZeroMemory(&m_pStmArrar, sizeof(m_pStmArrar));	
	memset(&m_rDst, 0 ,sizeof(m_rDst));
	m_pPresent = NULL;
	InitializeCriticalSection(&m_csDisplay);
}

CVRPresentProcessorEbd ::~CVRPresentProcessorEbd ()
{
	Close();
	DeleteCriticalSection(&m_csDisplay);
}	


//-------------------------------------------------------------------
// InitializeWindow
//
// Creates the application window.
//-------------------------------------------------------------------

STDMETHODIMP CVRPresentProcessorEbd ::Open(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize)
{
	HRESULT hr = S_OK;
	if(m_Init)
		return E_UNEXPECTED;

	memcpy(&m_openParams, pOpenParams, dwOpenSize);

	//if(!m_openParams.canvasWidth)

	SetRect(&m_rWnd, 0,0,0,0);
	CheckCanvasChange();

//		m_openParams.canvasWidth = GetSystemMetrics(SM_CXSCREEN);
//		m_openParams.canvasHeight= GetSystemMetrics(SM_CYSCREEN);

	m_Init = true;
	return hr;
}

STDMETHODIMP CVRPresentProcessorEbd ::Close()
{
	if(!m_Init)
		return E_UNEXPECTED;

	m_Init = false;
	return S_OK;
}


STDMETHODIMP CVRPresentProcessorEbd ::CheckCanvasChange()
{
    	if(m_openParams.hWnd)
    	{
    		RECT rWnd;
    		::GetClientRect(m_openParams.hWnd, &rWnd);
		if((m_rWnd.right - m_rWnd.left != rWnd.right - rWnd.left) ||
			(m_rWnd.bottom - m_rWnd.top != rWnd.bottom - rWnd.top))
		{
			m_rWnd = rWnd;
			m_openParams.canvasWidth = m_rWnd.right - m_rWnd.left;
			m_openParams.canvasHeight = m_rWnd.bottom - m_rWnd.top;
			m_bcanChanged = true;
		}
    	}
	else
	{
		if((m_rWnd.right - m_rWnd.left != m_rDst.right - m_rDst.left) ||
			(m_rWnd.bottom - m_rWnd.top != m_rDst.bottom - m_rDst.top))
		{
			m_rWnd = m_rDst;
			m_openParams.canvasWidth = m_rWnd.right - m_rWnd.left;
			m_openParams.canvasHeight = m_rWnd.bottom - m_rWnd.top;
			m_bcanChanged = true;
		}
	}
	return S_OK;
}

STDMETHODIMP CVRPresentProcessorEbd ::Render(DWORD dwRenderFlags)
{
	HRESULT hr = S_OK;
	
	CheckCanvasChange();

	if(m_bcanChanged)
		return VR_FAIL_CANVASCHANGED;

	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid)
		{
			RECT rDst = {0};
			hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_RECTDST,0,0,&rDst,0,0);
			if(SUCCEEDED(hr))
			{
				hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenter, MC_PROPID_VP_VP_HND,0,0,&m_pPresent,0,0);
				if(SUCCEEDED(hr))
				{
					RECT rCanvas;
					SetRect(&rCanvas, 0,0,m_openParams.canvasWidth,  m_openParams.canvasHeight);
					IntersectRect(&rDst, &rDst, &rCanvas);

					POINT p={0,0};
					if(m_openParams.hWnd)
					{
						::ClientToScreen(m_openParams.hWnd, &p);
						OffsetRect(&rDst,p.x,p.y);
					}
					else
					{
						OffsetRect(&rDst,m_rDst.left,m_rDst.top);
					}
/*
					float fleft = (float)(rDst.left/m_openParams.canvasWidth);
					float ftop = (float)(rDst.top/m_openParams.canvasHeight);
					float fright = (float)(rDst.right/m_openParams.canvasWidth);
					float fbottom= (float)(rDst.bottom /m_openParams.canvasHeight);
					rDst.left = (long)(m_rDst.left + fleft* (m_rDst.right - m_rDst.left));
					rDst.top = (long)(m_rDst.top + ftop* (m_rDst.bottom  - m_rDst.top));
					rDst.right = (long)(m_rDst.left + fright* (m_rDst.right - m_rDst.left));
					rDst.bottom  = (long)(m_rDst.top + fbottom* (m_rDst.bottom - m_rDst.top));
*/
					hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTDST ,0,0,&rDst,0);
					//if(FAILED(hr))	DP("SetDest %d,%d,%d,%d failed\b", m_rDst.left, m_rDst.top, m_rDst.right , m_rDst.bottom);
				}
			bool bEnable = false;
			hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_ENABLE,0,0,&bEnable,0,0);
			if(SUCCEEDED(hr))
				m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ENABLE,0,0,&bEnable,0);
				
			}

		}

	}

	return S_OK;
}


STDMETHODIMP CVRPresentProcessorEbd ::Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	HRESULT hr = E_PROP_ID_UNSUPPORTED;
	switch (dwPropID)
	{
		case MC_PROPID_VP_INTRNAL_SURFINFO:
			{
#ifdef MTK3353_HW
				{
					VP_INTRNAL_SURFINFO* pArgs = (VP_INTRNAL_SURFINFO*)pPropData;
					pArgs->dwFourCC = MAKEFOURCC('M','T','K','1');
					return S_OK;
				}
#else
				break;
#endif
			}

		case MC_PROPID_VP_CANVAS_INFO:
			{
				if(pPropData == NULL)	return E_INVALIDARG;
				VP_CANVASINFO_PARAMS* pArgs = (VP_CANVASINFO_PARAMS*)pPropData;
				pArgs->dwWidth = m_openParams.canvasWidth;
				pArgs->dwHeight = m_openParams.canvasHeight;
				m_bcanChanged = false;
				return S_OK;
			}
			
		case MC_PROPID_VP_ALLSTREAMS:
		{
			IMcVideoPresenterStream** pArray = (IMcVideoPresenterStream**)pPropData;
			int intputArraySize = cbPropData/ sizeof(IMcVideoPresenterStream*);
			if(pArray == 0)	
				return E_INVALIDARG;
			
			if(GetStreamNumber()> intputArraySize)
				return E_OUTOFMEMORY;

			int i = 0;
			int j = 0;
			for(i=0;i<MAX_STREAM ;i++)
			{
				if(m_pStmArrar[i].valid)	
				{
					pArray[j++] = m_pStmArrar[i].pStream;
				}
			}
			if(pcbReturned)
				*pcbReturned = j; 
			return S_OK;
		}
		case MC_PROPID_VP_COLORSYS_CAP:
		case MC_PROPID_VP_BRIGHTNESS:
		case MC_PROPID_VP_CONTRAST:
		case MC_PROPID_VP_HUE:
		case MC_PROPID_VP_SATURATION:
		case MC_PROPID_VP_GAMMA:
		case MC_PROPID_VP_COLORENABLE:
		case MC_PROPID_VP_SHARPNESS:
			{
				for(int i=0;i<MAX_STREAM ;i++)
				{
					if(m_pStmArrar[i].valid)
					{
						hr = m_pStmArrar[i].pStream->Get(EMC_PROPSET_GFXCTXT,
							dwPropID,0,0,pPropData,cbPropData,pcbReturned);
						break;
					}
				}
			}
			return S_OK;
		default:
			break;
	}
	return E_PROP_ID_UNSUPPORTED;
}


STDMETHODIMP CVRPresentProcessorEbd ::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	HRESULT hr = E_PROP_ID_UNSUPPORTED;

	if (dwPropID==MC_PROPID_VP_VSYNC)
	{
		for(int i=0;i<MAX_STREAM ;i++)
		{
			if(m_pStmArrar[i].valid)
			{
				hr = m_pStmArrar[i].pStream->Set(guidPropSet,dwPropID,pInstanceData,cbInstanceData,pPropData,cbPropData);
				if(FAILED(hr))
					break;
			}
		}
		return hr;
	}

	if((!m_Init)||(!pPropData))
		return E_FAIL;

	switch (dwPropID)
	{
		case MC_PROPID_VP_DEINTERLACE_MODE:
			return E_PROP_ID_UNSUPPORTED;

		case MC_PROPID_VP_BRIGHTNESS:
		case MC_PROPID_VP_CONTRAST:
		case MC_PROPID_VP_HUE:
		case MC_PROPID_VP_SATURATION:
		case MC_PROPID_VP_GAMMA:
		case MC_PROPID_VP_COLORENABLE:
		case MC_PROPID_VP_SHARPNESS:
		case MC_PROPID_VP_ENABLE:
			{
				for(int i=0;i<MAX_STREAM ;i++)
				{
					if(m_pStmArrar[i].valid)
					{
						hr = m_pStmArrar[i].pStream->Set(EMC_PROPSET_GFXCTXT,
							dwPropID,0,0,pPropData,cbPropData);
					}
				}
			}
			return S_OK;
		case MC_PROPID_VP_RECTDST:
		{
			m_rDst = *reinterpret_cast<RECT *>(pPropData);
			return S_OK;
		}
		break;
		case MC_PROPID_VP_WND:
		{		
			for(int i=0;i<MAX_STREAM;i++)
			{
				if(m_pStmArrar[i].valid && m_pStmArrar[i].param.type == eMAINSTREAM)
				{
					m_pStmArrar[i].pStream->Set(IID_IMcVideoPresenter, 
						dwPropID,0,0,pPropData,0);
					m_openParams.hWnd = *((HWND*)pPropData);
				}
			}
		}
		break;
		default:
			break;
	}
	return hr;
}


STDMETHODIMP CVRPresentProcessorEbd ::CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream)
{
	HRESULT hr = S_OK;

	UINT i;
	for(i=0;i<MAX_STREAM ;i++)
	{
		if(!m_pStmArrar[i].valid)	break;
	}

	if(i>= MAX_STREAM)	return E_FAIL;

	EnterCriticalSection(&m_csDisplay);
	memcpy(&m_pStmArrar[i].param, pOpenParams, sizeof(VP_OPEN_STREAM_PARAMS));

	DP("chromeformat:0x%x \n", pOpenParams->dwFourCC);
	if(isSW_YV12(pOpenParams->dwFourCC))
		m_pStmArrar[i].param.dwFourCC = MAKE_FOURCC('Y','V','1','2');
	else if(isSW_NV12(pOpenParams->dwFourCC))
		m_pStmArrar[i].param.dwFourCC = MAKE_FOURCC('N','V','1','2');
	else if(isSW_RGB565(pOpenParams->dwFourCC))
		m_pStmArrar[i].param.dwFourCC = MAKE_FOURCC('R','5','6','5');

	m_pStmArrar[i].pStream = new CVRPresenterStreamEbd(m_csDisplay, 
		&m_pStmArrar[i].param,m_openParams.hWnd,this);
	if(!m_pStmArrar[i].pStream)	
	{
		LeaveCriticalSection(&m_csDisplay);
		return E_FAIL;
	}
	m_pStmArrar[i].valid = true;
#ifdef TITAN_II	
	if(m_pStmArrar[i].param.type == eSUBSTREAMS)
	{
		UINT mainId = 0;
		for(mainId =0;mainId< MAX_STREAM;mainId++)
		{
			if(m_pStmArrar[i].valid && m_pStmArrar[i].param.type == eMAINSTREAM)
				break;
		}
		if(mainId>=MAX_STREAM)	
		{
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}

		PVOID titian_hnd;
		hr = m_pStmArrar[mainId].pStream->Get(IID_IMcVideoPresenter, MC_PROPID_VP_TITIANII_HND,0,0,&titian_hnd,0,0);
		if(FAILED(hr))
		{
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}

		hr = m_pStmArrar[i].pStream->Set(IID_IMcVideoPresenter, MC_PROPID_VP_TITIANII_HND,0,0,titian_hnd,0);
		if(FAILED(hr))
		{
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}
	}
#endif
//Return them
	if(ppStream)
		*ppStream = static_cast<IMcVideoPresenterStream*>(m_pStmArrar[i].pStream);
	m_streamCount++;

	LeaveCriticalSection(&m_csDisplay);
	return hr;
	
}


STDMETHODIMP CVRPresentProcessorEbd ::DestroyStream(IN IMcVideoPresenterStream *ppStream)
{
	if(!ppStream)	return E_FAIL;

	EnterCriticalSection(&m_csDisplay);

	for(int i=0;i<MAX_STREAM;i++)
	{
		if(m_pStmArrar[i].pStream == ppStream)
		{
			ppStream->Close();
			ppStream->Release();
			ZeroMemory(&m_pStmArrar[i], sizeof(SSTREAM_INFO));
			LeaveCriticalSection(&m_csDisplay);
			return S_OK;
		}
	}
	LeaveCriticalSection(&m_csDisplay);
	return E_NOTIMPL;
}

int CVRPresentProcessorEbd ::GetStreamNumber()
{
	int streamNum = 0;
	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid)	
			streamNum++;
	}
	return streamNum;
}
