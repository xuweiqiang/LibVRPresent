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
#include "GfxPresentProcDXVA.h"
#include "GfxPresentStreamDXVA.h"


#define VIDEO_PRESENTER_NAME TEXT("Video Presenter2.0")
extern RECT ScaleRectangle(const RECT& input, const RECT& src, const RECT& dst);

extern HRESULT ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin);

CVRPresentOvlProcessorDXVA ::CVRPresentOvlProcessorDXVA () :
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_pPresent(NULL),
	m_Init(false),
	m_mainSurf_Init(false),
	m_streamCount(0),
	m_pAlphaBlend(0),
	m_bytes_per_pixcel(0),
	m_pDxvaHnd(NULL),
	m_lRenderEnabled(true),
	m_lZoomMode(0),
	m_pVideoWindow(NULL),
	m_dll(NULL)
{
	ZeroMemory(&m_openParams, sizeof(m_openParams));	
	ZeroMemory(&m_pMainSurf, sizeof(m_pMainSurf));	
	ZeroMemory(&m_pStmArrar, sizeof(m_pStmArrar));	
	ZeroMemory(&m_rSrc, sizeof(m_rSrc));	
	ZeroMemory(&m_rDst, sizeof(m_rDst));	
	InitializeCriticalSection(&m_csDisplay);
}

CVRPresentOvlProcessorDXVA ::~CVRPresentOvlProcessorDXVA ()
{
	if(m_dll)
	{
		UnLoadV1Library(m_dll, &m_pPresent);
		m_dll = NULL;
	}
	DeleteCriticalSection(&m_csDisplay);
}	


//-------------------------------------------------------------------
// InitializeWindow
//
// Creates the application window.
//-------------------------------------------------------------------

STDMETHODIMP CVRPresentOvlProcessorDXVA ::Open(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize)
{
	HRESULT hr = S_OK;
	if(m_Init)
		return E_UNEXPECTED;

	if(!m_pDxvaHnd)
		m_pDxvaHnd = new CDXVAApplication(pOpenParams->hWnd);
	if(!m_pDxvaHnd) 	return E_FAIL;

	memcpy(&m_openParams, pOpenParams, dwOpenSize);

	if(!m_openParams.canvasWidth)
		m_openParams.canvasWidth = GetSystemMetrics(SM_CXSCREEN);

	if(!m_openParams.canvasHeight)
		m_openParams.canvasHeight= GetSystemMetrics(SM_CYSCREEN);

	hr = m_pDxvaHnd->DXVACreateCanvas(m_openParams.canvasWidth, m_openParams.canvasHeight);
	if(FAILED(hr))	return hr;
	
	m_pVideoWindow = CreateVideoWindow();
	if(m_pVideoWindow == NULL)
		return E_FAIL;
	
	m_Init = true;
	return hr;
}

STDMETHODIMP CVRPresentOvlProcessorDXVA ::Close()
{
	if(!m_Init)
		return E_UNEXPECTED;

	if(m_pVideoWindow)
	{
		m_pVideoWindow->Release();
		m_pVideoWindow = NULL;
	}
		
	if(m_pDxvaHnd)
	{
		delete m_pDxvaHnd;
		m_pDxvaHnd = NULL;
	}
	m_Init = false;
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorDXVA ::Render(DWORD dwRenderFlags)
{
	RECT rectCurr = {NULL};
	CProtect proc(&m_csDisplay);

	GetClientRect(m_openParams.hWnd, &rectCurr );
	if(memcmp(&rectCurr , &m_rDst,sizeof(RECT)) != 0)
	{
		m_rDst =rectCurr ;
		m_pDxvaHnd->OnSize();
	}

	for(UINT i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid)
		{
			HRESULT  hr = S_OK;
//Set Enable
			bool bEnable = false;
			hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_ENABLE,0,0,&bEnable ,0,0);
			if(FAILED(hr))	continue;
			hr = m_pDxvaHnd->DXVAAppSetEnable(m_pStmArrar[i].stream_id, &bEnable);
			if(FAILED(hr))	continue;
			if(!bEnable)	continue;

//Adjust src,dest rectangel
			RECT stmdst, stmsrc;
			hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_RECTSRC,0,0,&stmsrc ,0,0);
			if(FAILED(hr))	continue;
			hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_RECTDST,0,0,&stmdst,0,0);
			if(FAILED(hr))	continue;

			RECT shareArea;
			RECT rCanvas;
			SetRect(&rCanvas, 0,0,m_openParams.canvasWidth ,m_openParams.canvasHeight);
			IntersectRect(&shareArea, &stmdst, &rCanvas);
			if(memcmp(&shareArea, &stmdst,sizeof(RECT)) != 0)
			{
				DWORD shareWidth = shareArea.right - shareArea.left;
				DWORD shareHeight = shareArea.bottom - shareArea.top;
				float xScale  = (float)shareWidth/(float)(stmdst.right - stmdst.left);
				float yScale   = (float)shareHeight/(float)(stmdst.bottom - stmdst.top);

				if(stmdst.left < shareArea.left)
					stmsrc.left = stmsrc.right - (LONG)(stmsrc.right*xScale);
				if(stmdst.right > shareArea.right)
					stmsrc.right = (LONG)(stmsrc.right*xScale);
				if(stmdst.top < shareArea.top)
					stmsrc.top = stmsrc.bottom - (LONG)(stmsrc.bottom*yScale);
				if(stmdst.bottom > shareArea.bottom)
					stmsrc.bottom = (LONG)(stmsrc.bottom*yScale);

				if(stmdst.top < 0)	stmdst.top =0;
				if(stmdst.bottom > (LONG)m_openParams.canvasHeight)	stmdst.bottom =m_openParams.canvasHeight;
				if(stmdst.left < 0)	stmdst.left =0;
				if(stmdst.right > (LONG)m_openParams.canvasWidth)	stmdst.right =m_openParams.canvasWidth;
				
				stmsrc.left = stmsrc.left/2*2;
				stmsrc.top = stmsrc.top/2*2;
				stmsrc.right = stmsrc.right/2*2;
				stmsrc.bottom = stmsrc.bottom/2*2;

				stmdst.left = stmdst.left/2*2;
				stmdst.top = stmdst.top/2*2;
				stmdst.right = stmdst.right/2*2;
				stmdst.bottom = stmdst.bottom/2*2;
			}

			hr = m_pDxvaHnd->DXVAAppSetSrc(m_pStmArrar[i].stream_id, &stmsrc);
			if(FAILED(hr))	continue;

			hr = m_pDxvaHnd->DXVAAppSetDest(m_pStmArrar[i].stream_id, &stmdst);
			if(FAILED(hr))	continue;

//Set Alpha
			DWORD dwAlpha;
			hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_ALPHA,0,0,&dwAlpha,0,0);
			if(FAILED(hr))	continue;

			float fAlpha = (float)dwAlpha/MAX_ALPHA_VAL;
			hr = m_pDxvaHnd->AdjustAlphaSetting(m_pStmArrar[i].stream_id, &fAlpha );
			if(FAILED(hr))	continue;

//Set progressive/deinterlace mode 
			VP_SCAN_MODE mode;
			DXVAHD_FRAME_FORMAT scanFmt = DXVAHD_FRAME_FORMAT_PROGRESSIVE;
			hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_SCAN_MODE,0,0,&mode,0,0);
			if(FAILED(hr))	continue;
			if(mode == VP_SCAN_PROGRESSIVE)
				scanFmt = DXVAHD_FRAME_FORMAT_PROGRESSIVE;
			else if(mode == VP_SCAN_INTERLACED_TOPFIELED_FIRST)
				scanFmt = DXVAHD_FRAME_FORMAT_INTERLACED_TOP_FIELD_FIRST;
			else if(mode == VP_SCAN_INTERLACED_BOTTOMFIELED_FIRST)
				scanFmt = DXVAHD_FRAME_FORMAT_INTERLACED_BOTTOM_FIELD_FIRST;
				
			hr = m_pDxvaHnd->DXVAAppSetDeInterlaceMode(m_pStmArrar[i].stream_id,  scanFmt);
			if(FAILED(hr))	continue;

		}
	}

	if(!m_pDxvaHnd->ProcessVideoFrame())
	{
		return E_FAIL;
	}

	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorDXVA ::Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	switch (dwPropID)
	{
		case MC_PROPID_VP_ENABLE:
			return ReturnComLong(m_lRenderEnabled,pPropData,cbPropData,pcbReturned);

		case MC_PROPID_VP_CANVAS_INFO:
			{
				if(pPropData==0)
					return E_POINTER;
				if(!m_pDxvaHnd)	return E_FAIL;
				VP_CANVASINFO_PARAMS* pArgs = (VP_CANVASINFO_PARAMS*)pPropData;
				pArgs->dwWidth = m_pDxvaHnd->m_canvasInfo.open_params.dwWidth;
				pArgs->dwHeight = m_pDxvaHnd->m_canvasInfo.open_params.dwHeight;
				return S_OK;
			}
		case MC_PROPID_VP_COLORSYS_CAP:
		{
			if(pPropData==0)
				return E_POINTER;
			if(!m_pDxvaHnd)	return E_FAIL;
			VR_FILTERCONTROL sColorCaps;
			memset(&sColorCaps, 0 ,sizeof(sColorCaps));
			ProcAmpInfo filter;

			if(SUCCEEDED(m_pDxvaHnd->DXVAGetFilter(DXVAHD_FILTER_BRIGHTNESS, &filter)))
			{
				if(filter.bSupported)
				{
					sColorCaps.dwFlags |= FILTER_BRIGHTNESS;
					sColorCaps.lBrightness = filter.CurrentValue;
					ConvertPrecision(sColorCaps.lBrightness,MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, filter.Range.Maximum, filter.Range.Minimum);
				}
			}
			if(SUCCEEDED(m_pDxvaHnd->DXVAGetFilter(DXVAHD_FILTER_CONTRAST, &filter)))
			{
				if(filter.bSupported)
				{
					sColorCaps.dwFlags |= FILTER_CONTRAST;
					sColorCaps.lContrast= filter.CurrentValue;
					ConvertPrecision(sColorCaps.lContrast,MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, filter.Range.Maximum, filter.Range.Minimum);
				}
			}
			if(SUCCEEDED(m_pDxvaHnd->DXVAGetFilter(DXVAHD_FILTER_HUE, &filter)))
			{
				if(filter.bSupported)
				{
					sColorCaps.dwFlags |= FILTER_HUE;
					sColorCaps.lHue = filter.CurrentValue;
					ConvertPrecision(sColorCaps.lHue ,MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, filter.Range.Maximum, filter.Range.Minimum);
				}
			}
			if(SUCCEEDED(m_pDxvaHnd->DXVAGetFilter(DXVAHD_FILTER_SATURATION, &filter)))
			{
				if(filter.bSupported )
				{
					sColorCaps.dwFlags |= FILTER_SATURATION;
					sColorCaps.lSaturation= filter.CurrentValue;
					ConvertPrecision(sColorCaps.lSaturation,MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, filter.Range.Maximum, filter.Range.Minimum);
				}
			}
#if 0		
			if(SUCCEEDED(m_pDxvaHnd->DXVAGetFilter(DXVAHD_FILTER_NOISE_REDUCTION, &filter)))
			{
				if(filter.bSupported)
				{
					sColorCaps.dwFlags |= FILTER_NOISE_REDUCTION;
					sColorCaps.lNoiseRedct = filter.CurrentValue;
					ConvertPrecision(sColorCaps.lNoiseRedct ,MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, filter.Range.Maximum, filter.Range.Minimum);
				}
			}
			if(SUCCEEDED(m_pDxvaHnd->DXVAGetFilter(DXVAHD_FILTER_EDGE_ENHANCEMENT, &filter)))
			{
				if(filter.bSupported)
				{
					sColorCaps.dwFlags |= FILTER_EDGE_ENHANCE;
					sColorCaps.lEdge= filter.CurrentValue;
					ConvertPrecision(sColorCaps.lEdge,MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, filter.Range.Maximum, filter.Range.Minimum);
				}
			}
			if(SUCCEEDED(m_pDxvaHnd->DXVAGetFilter(DXVAHD_FILTER_ANAMORPHIC_SCALING, &filter)))
			{
				if(filter.bSupported)
				{
					sColorCaps.dwFlags |= FILTER_ANAMORPHIC_SCALING;
					sColorCaps.lAnaScale = filter.CurrentValue;
					ConvertPrecision(sColorCaps.lAnaScale ,MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, filter.Range.Maximum, filter.Range.Minimum);
				}
			}
#endif
			*reinterpret_cast<VR_FILTERCONTROL*>(pPropData) = sColorCaps;
			if(pcbReturned)
				*pcbReturned = sizeof(VR_FILTERCONTROL);
			return S_OK;
		}

		default:
			break;
	}
	return E_PROP_ID_UNSUPPORTED;
}


STDMETHODIMP CVRPresentOvlProcessorDXVA ::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	HRESULT hr = E_PROP_ID_UNSUPPORTED;
	if((!m_Init)||(!pPropData))
		return E_FAIL;

	
	switch (dwPropID)
	{
	case MC_PROPID_VP_RECTDST:
		return S_OK;
	case MC_PROPID_VP_ENABLE:
		{
			CProtect proc(&m_csDisplay);
			bool lEnable = *(bool *)pPropData;
			if(m_lRenderEnabled!=lEnable)
			{
				m_lRenderEnabled = lEnable;
				hr = m_pDxvaHnd->DXVAAppSetProcEnable(&m_lRenderEnabled);				
			}
			return hr;
		}
	case MC_PROPID_VP_ZOOM:
		{
			CProtect proc(&m_csDisplay);
			LONG lZoomMode = *(LONG *)pPropData;
			if(m_lZoomMode!=lZoomMode)
			{
				m_lZoomMode = lZoomMode;
				m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_MODE,0,0,&m_lZoomMode, sizeof(m_lZoomMode));

			}
			return S_OK;
		}
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_NOISE_REDUCTION:
	case MC_PROPID_VP_EDGE_ENHANCEMENT:
	case MC_PROPID_VP_ANAMOPHIC_SCALING:
		{
			if(!m_pDxvaHnd)	return E_FAIL;

			LONG val = *(LONG *)pPropData;
			if(val > MAX_COLORCONTROL_VAL)
				val = MAX_COLORCONTROL_VAL;
			if(val < MIN_COLORCONTROL_VAL)
				val = MIN_COLORCONTROL_VAL;
			
			DXVAHD_FILTER filter;
			ProcAmpInfo info;
			ZeroMemory(&info, sizeof(info));
			if(dwPropID == MC_PROPID_VP_BRIGHTNESS)
				filter = DXVAHD_FILTER_BRIGHTNESS;
			else if(dwPropID == MC_PROPID_VP_CONTRAST)
				filter = DXVAHD_FILTER_CONTRAST ;
			else if(dwPropID == MC_PROPID_VP_HUE)
				filter = DXVAHD_FILTER_HUE;
			else if(dwPropID == MC_PROPID_VP_SATURATION)
				filter = DXVAHD_FILTER_SATURATION;
			else if(dwPropID == MC_PROPID_VP_NOISE_REDUCTION)
				filter = DXVAHD_FILTER_NOISE_REDUCTION;
			else if(dwPropID == MC_PROPID_VP_EDGE_ENHANCEMENT)
				filter = DXVAHD_FILTER_EDGE_ENHANCEMENT;
			else if(dwPropID == MC_PROPID_VP_ANAMOPHIC_SCALING)
				filter = DXVAHD_FILTER_ANAMORPHIC_SCALING;
			else
				return E_NOTIMPL;
			if(FAILED(m_pDxvaHnd->DXVAGetFilter(filter, &info)))
				return E_FAIL;

			if(!info.bSupported)
				return E_FAIL;
			
			ConvertPrecision(val,info.Range.Maximum, info.Range.Minimum, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL);
			return (m_pDxvaHnd->DXVASetFilter(filter, val));
		}
	default:
		break;

	}

	return hr;
}


STDMETHODIMP CVRPresentOvlProcessorDXVA ::CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream)
{
	HRESULT hr = S_OK;
	int ret = true;
	if((!m_Init))
		return E_FAIL;

	UINT i;
	for(i=0;i<MAX_STREAM ;i++)
	{
		if(!m_pStmArrar[i].valid)	break;
	}

	if(i>= MAX_STREAM)	return E_FAIL;
	memcpy(&m_pStmArrar[i].param, pOpenParams, sizeof(VP_OPEN_STREAM_PARAMS));


	IDirect3DSurface9* pStream = NULL;
	ret = m_pDxvaHnd->CreateStream(pOpenParams, dwOpenSize,pStream,m_pStmArrar[i].stream_id);
	if(!ret)	{ hr = E_FAIL; goto RET;};

	m_pStmArrar[i].pStream = new CVRPresenterStreamDXVA(m_csDisplay,&m_pStmArrar[i].param, pStream);
	if(!m_pStmArrar[i].pStream)	{ hr = E_FAIL; goto RET;};
	m_pStmArrar[i].valid = true;

	//Return them
	if(ppStream)
		*ppStream = static_cast<IMcVideoPresenterStream*>(m_pStmArrar[i].pStream);

	RET:
	for(UINT i=0;i<MAX_STREAM;i++)
	{
		if(m_pDxvaHnd->m_streamInfo[i].m_pStream)
			m_pStmArrar[i].pStream->Set(IID_IMcVideoPresenterAdv, MC_PROPID_VP_SURFACE, 0,0,m_pDxvaHnd->m_streamInfo[i].m_pStream,0);
	}
	return hr;
}


STDMETHODIMP CVRPresentOvlProcessorDXVA ::DestroyStream(IN IMcVideoPresenterStream *ppStream)
{
	int ret = true;
	if((!m_Init))
		return E_UNEXPECTED;
	for(UINT i=0;i<MAX_STREAM;i++)
	{
		if(ppStream == m_pStmArrar[i].pStream)
		{
			ret = m_pDxvaHnd->DestroyStream(m_pStmArrar[i].stream_id);
			memset(&m_pStmArrar[i],0,sizeof(SSTREAM_INFO) );
			if(ret)	
			{
				for(UINT i=0;i<MAX_STREAM;i++)
				{
					if(m_pDxvaHnd->m_streamInfo[i].m_pStream)
						m_pStmArrar[i].pStream->Set(IID_IMcVideoPresenterAdv, MC_PROPID_VP_SURFACE, 0,0,m_pDxvaHnd->m_streamInfo[i].m_pStream,0);
				}
				return S_OK;
			}
			
			return E_ABORT;
		}
	}
	return E_FAIL;
}

