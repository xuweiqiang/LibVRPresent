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

#include "GfxPresentStreamSW.h"
#include "../../inc/dp.h"


CVRPresenterStream::CVRPresenterStream(LPBYTE pSurf, 	CRITICAL_SECTION* pCsDisplay,  VP_OPEN_STREAM_PARAMS *pOpenParams, IMcVideoPresenter**  ppPresenter):
m_pBuf(pSurf),
m_bEnable(false),
m_pCsDisplay(pCsDisplay),
m_pVideoWnd(0),
m_layerId(0),
m_deinterlace(VP_DEINTERLACE_WEAVE),
m_pPresent(*ppPresenter),
m_oneSufMode(false),
m_pVideoMixerContext(0)
{
	if(pOpenParams)
		memcpy(&m_open_param, pOpenParams, sizeof(m_open_param));
	m_alpha = (DWORD)(m_open_param.fAlpha*MAX_ALPHA_VAL);
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	m_zoom_mode = VP_ZOOM_MODE_STRETCH;
	m_bOpen = false;
	return;
}

CVRPresenterStream::~CVRPresenterStream()
{
	if(m_pVideoWnd)
	{
		m_pVideoWnd->Release();
		m_pVideoWnd = 0;
	}

	if(m_pVideoMixerContext)
	{
		m_pVideoMixerContext->DisConnect(IID_IMcVideoPresenterStream,0);
	}
	return;
}

STDMETHODIMP CVRPresenterStream::Open()
{
	HRESULT hr;
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	m_zoom_mode = VP_ZOOM_MODE_STRETCH;
	m_deinterlace  = VP_DEINTERLACE_WEAVE;
	if(m_bOpen)	return E_UNEXPECTED;

	m_pVideoMixerContext = 0;
	
	if(!m_pVideoWnd )
		m_pVideoWnd = CreateVideoWindow();
	if(!m_pVideoWnd )	return E_FAIL;

	VE_Options tVEOpen;
	ZeroMemory(&tVEOpen, sizeof(tVEOpen));
	tVEOpen.mode = (VE_MODE)m_zoom_mode;
	tVEOpen.rSrcClip.right = tVEOpen.rDst.right = tVEOpen.rSrc.right = m_open_param.dwWidth;
	tVEOpen.rSrcClip.bottom = tVEOpen.rDst.bottom =  tVEOpen.rSrc.bottom = m_open_param.dwHeight;
	tVEOpen.aspect.dwAspectX = m_open_param.dwAspectX;
	tVEOpen.aspect.dwAspectY = m_open_param.dwAspectY;
	hr = m_pVideoWnd->Open(&tVEOpen,sizeof(tVEOpen));
	if(FAILED(hr)) 	return E_FAIL;

	Update();

	#ifdef DUMPSTREAM
	gStream = 0;
	if(m_open_param.type == eSUBSTREAMS)
	{
		char streamName[MAX_PATH];
		sprintf(streamName, "d:\\vrStream%dx%d.yuv",m_open_param.dwWidth, m_open_param.dwHeight);
		gStream = fopen(streamName, "a+");
		if(!gStream)
			DP("failed to create file:%s \n",streamName);
	}
	#endif

	m_lFlipVideoMode = VP_FLIPVH_NO;
	m_bOpen = true;

	return S_OK;
};

STDMETHODIMP CVRPresenterStream::Close()
{
	if(!m_bOpen)	return E_UNEXPECTED;
	
	if(m_pVideoWnd)
	{
		m_pVideoWnd->Close();
		m_pVideoWnd->Release();
		m_pVideoWnd = 0;
	}
	#ifdef DUMPSTREAM
		if(gStream)
			 fclose(gStream);
	#endif

	m_bOpen = false;
	return S_OK;
};
	
STDMETHODIMP CVRPresenterStream::Get(IN REFGUID guidPropSet, IN DWORD dwPropID,
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	if(pPropData==0)
		return E_POINTER;

	switch(dwPropID)
	{
		case MC_PROPID_VP_RECTSRC:
			{
				*reinterpret_cast<RECT *>(pPropData) = m_rectSrc;
				if(pcbReturned)
					*pcbReturned = sizeof(RECT);
				return S_OK;
			}
		case MC_PROPID_VP_RECTDST:
			{
				if(m_pVideoMixerContext)
				{
					RECT rDst = {0,0,0,0};
					if(FAILED(m_pVideoMixerContext->GetPos2D(&rDst)))
						DP("GetPos2D failed \n");
					
					*reinterpret_cast<RECT *>(pPropData) = rDst;
					m_rectDst = rDst;
				}
				else
					*reinterpret_cast<RECT *>(pPropData) = m_rectDst;
				if(pcbReturned)
					*pcbReturned = sizeof(RECT);
				return S_OK;
			}
		case MC_PROPID_VP_ENABLE:
			{
				*reinterpret_cast<bool *>(pPropData) = m_bEnable;
				if(pcbReturned)
					*pcbReturned = sizeof(bool);
				return S_OK;
			}
		case MC_PROPID_VP_ALPHA:
			{
				*reinterpret_cast<DWORD*>(pPropData) = m_alpha;
				return S_OK;
			}
		case MC_PROPID_VP_ZOOM:
			{
				*reinterpret_cast<DWORD*>(pPropData) = m_zoom_mode ;
				return S_OK;
			}
		case MC_PROPID_VP_LAYER_ID:
			{
				*reinterpret_cast<DWORD*>(pPropData) = m_layerId;
				return S_OK;
			}
			break;
		case MC_PROPID_VP_DEINTERLACE_MODE:
			{
				*reinterpret_cast<VP_DEINTERLACE_MODE*>(pPropData) = m_deinterlace ;
				return S_OK;
			}
			break;
	case MC_PROPID_VP_FLIPVIDEO:
			{
				*reinterpret_cast<VP_FLIPVIDEO_MODE*>(pPropData) = m_lFlipVideoMode;
				return S_OK;
			}

			
	case MC_PROPID_VP_RECTDST_DEVICE:
			{
				*reinterpret_cast<RECT*>(pPropData) = m_rectDst;
				return S_OK;
			}
			
		default:
			break;
	}
	return E_NOTIMPL;
}

STDMETHODIMP CVRPresenterStream::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, 
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	HRESULT hr = S_OK;
	//OneSurf mode will be set before Open();
	if(!m_bOpen && dwPropID != MC_PROPID_VP_ONESURF_MODE )	return E_UNEXPECTED;
	
	switch (dwPropID)
	{
	case MC_PROPID_VP_RECTSRC:
		{
			if(!pPropData)	return E_INVALIDARG;
			RECT *pr = reinterpret_cast<RECT *>(pPropData);
			m_rectSrc = *pr;

			hr = m_pVideoWnd->Set(IID_IMcVideoPresenter, VE_COMMAND_SRCRECT,0,0,pr, sizeof(RECT));
			hr = Update();
			if(FAILED(hr))	return hr;
		}
		break;
	case MC_PROPID_VP_RECTDST:
		{
			if(!pPropData)	return E_INVALIDARG;
			RECT* pr = reinterpret_cast<RECT *>(pPropData);

//			if(m_oneSufMode)
//				m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,0);

			hr = m_pVideoWnd->Set(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,pr, sizeof(RECT));
			if(FAILED(hr))	return hr;

			hr = Update();
			if(FAILED(hr))	return hr;

		}
		break;
	case MC_PROPID_VP_ENABLE:
		{
			if(!pPropData)	return E_INVALIDARG;
			m_bEnable = *(reinterpret_cast<bool*>(pPropData));

			if(m_oneSufMode)
			{
   				return m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,&m_bEnable,0);;
			}
		}
		break;
	case MC_PROPID_VP_ALPHA:
		{
			if(!pPropData)	return E_INVALIDARG;
			m_alpha   = *(reinterpret_cast<DWORD*>(pPropData));
		}
		break;
	case MC_PROPID_VP_ZOOM:
		{
			if(!pPropData)	return E_INVALIDARG;
		
			m_zoom_mode = *reinterpret_cast<VP_ZOOM_MODE*>(pPropData);			

			if(m_oneSufMode && (m_open_param.type == eMAINSTREAM))
			{
				m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,&m_zoom_mode,0);
				VP_ZOOM_MODE  zoom = VP_ZOOM_MODE_STRETCH;
				m_pVideoWnd->Set(IID_IMcVideoPresenter, VE_COMMAND_MODE,0,0,&zoom, sizeof(VP_ZOOM_MODE));
			}
			else
				m_pVideoWnd->Set(IID_IMcVideoPresenter, VE_COMMAND_MODE,0,0,pPropData, sizeof(VP_ZOOM_MODE));
			hr = Update();
			if(FAILED(hr))	return hr;

		}
		break;
	case MC_PROPID_VP_LAYER_ID:
		if(!pPropData)	return E_INVALIDARG;
		m_layerId = *(reinterpret_cast<DWORD*>(pPropData));
		break;
	case MC_PROPID_VP_CLEAR:
			if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
			{
				//Malloc Buffer & Feed black screen data				
				LPBYTE pbuf = 0;
				pbuf = (LPBYTE)malloc(m_open_param.dwWidth*m_open_param.dwHeight*3/2);
				if(pbuf == 0)
					return E_FAIL;
				char y,u,v;
				if(!pPropData)
				{
					y = '\x00';
					u = '\x80';
					v = '\x80';	
				}
				else
				{
					DWORD yuv = *reinterpret_cast<DWORD*>(pPropData);
					y = (char)((yuv >>16)&0xff);
					u = (char)((yuv >>8)&0xff);
					v = (char)((yuv )&0xff);
				}
				memset(pbuf, y,m_open_param.dwWidth*m_open_param.dwHeight);
				memset(pbuf+m_open_param.dwWidth*m_open_param.dwHeight, u,m_open_param.dwWidth*m_open_param.dwHeight/4);
				memset(pbuf+m_open_param.dwWidth*m_open_param.dwHeight*5/4, v,m_open_param.dwWidth*m_open_param.dwHeight/4);

				//Present
				DWORD strides[3];
				strides[0]= m_open_param.dwWidth; 
				strides[1] = strides[2] = m_open_param.dwWidth/2;
				LPBYTE bufStrides[3];
				bufStrides[0] = pbuf; 
				bufStrides[1] = pbuf+m_open_param.dwWidth*m_open_param.dwHeight;
				bufStrides[2] = pbuf+m_open_param.dwWidth*m_open_param.dwHeight*5/4;
				Present(bufStrides, strides,0);
				//FreeBuffer
				delete pbuf;
				return S_OK;
			}
			else if(m_open_param.dwFourCC == MAKE_FOURCC('R','5','6','5'))
			{
				//Malloc Buffer & Feed black screen data				
				LPBYTE pbuf = 0;
				pbuf = (LPBYTE)malloc(m_open_param.dwWidth*m_open_param.dwHeight);
				if(pbuf == 0)
					return E_FAIL;

				char rgb;
				if(!pPropData)
					rgb = '\xff';
				else
					rgb = *reinterpret_cast<char*>(pPropData);

				memset(pbuf, rgb,m_open_param.dwWidth*m_open_param.dwHeight);


				//Present
				DWORD strides[3];
				strides[0] = strides[1] = strides[2] = m_open_param.dwWidth; 
				LPBYTE bufStrides[3];
				bufStrides[0] = bufStrides[1] = bufStrides[2] = pbuf; 
				Present(bufStrides, strides,0);

				//FreeBuffer
				delete pbuf;
				return S_OK;
			}
			else
				return E_NOTIMPL;
		break;
#if 0
	case MC_PROPID_VP_SCAN_MODE:
		{
			VP_SCAN_MODE scan = *(reinterpret_cast<VP_SCAN_MODE*>(pPropData));
			if(scan == VP_SCAN_PROGRESSIVE)
				m_deinterlace  = VP_DEINTERLACE_WEAVE;
			else if(scan == VP_SCAN_INTERLACED_TOPFIELED_FIRST)
				m_deinterlace  = VP_DEINTERLACE_BOB_SINGLE_FIELD;
			else if(scan == VP_SCAN_INTERLACED_BOTTOMFIELED_FIRST)
				m_deinterlace  = VP_DEINTERLACE_BOB_DOUBLE_FIELD;
		}
		break;
#endif
	case MC_PROPID_VP_DEINTERLACE_MODE:
		{
			if(!pPropData)	return E_INVALIDARG;
			m_deinterlace  = *(reinterpret_cast<VP_DEINTERLACE_MODE*>(pPropData));
		}
		break;
	case MC_PROPID_VP_ONESURF_MODE :
		{
			if(!pPropData)	return E_INVALIDARG;
			m_oneSufMode  = *(reinterpret_cast<bool*>(pPropData));

			if(!m_oneSufMode )
			{
				VP_ZOOM_MODE  zoom = VP_ZOOM_MODE_STRETCH;
				m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,&zoom,0);
			}

		}
		break;

	case MC_PROPID_VP_VM_CONTEXT:
		{
			m_pVideoMixerContext  = (reinterpret_cast<ICivmVideoContext*>(pPropData));
		}
		break;
	case MC_PROPID_VP_FLIPDELAY:
		{
			m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,0);
		}
		break;
	case MC_PROPID_VP_FLIPVIDEO:
		{
			VP_FLIPVIDEO_MODE lMode = *(VP_FLIPVIDEO_MODE*)pPropData;

			if((lMode != VP_FLIPH) && (lMode != VP_FLIPV) && (lMode != VP_FLIPVH_NO) )
				return E_INVALIDARG;
				
			m_lFlipVideoMode = lMode;

			return S_OK;
		}
	case MC_PROPID_VP_RECTDST_DEVICE:
		{
			m_rectDst = *(RECT*)pPropData;
			return S_OK;
		}
	default:
		return E_FAIL;
	}
	return S_OK;
 }

STDMETHODIMP CVRPresenterStream::LockSurface(LPBYTE *ppData, DWORD *pdwStride)
{
	HRESULT hr  = S_OK;
	if(ppData)
		*ppData = (LPBYTE)m_pBuf;
	
	if(pdwStride)
	{
		DWORD bytePerPixel = 0;
		switch(m_open_param.dwFourCC)
		{
			case MAKE_FOURCC('R', '5', '6', '5'):
				bytePerPixel = 2;
				break;
			case MAKE_FOURCC('Y', 'V', '1', '2'):
			case MAKE_FOURCC('N', 'V', '1', '2'):
				bytePerPixel = 1;
				break;
			default:
				return E_FAIL;
		}
		*pdwStride = m_open_param.dwWidth * bytePerPixel;
	}

	EnterCriticalSection(m_pCsDisplay);
	return hr;

}
STDMETHODIMP CVRPresenterStream::UnlockSurface()
{
	LeaveCriticalSection(m_pCsDisplay);
	return  S_OK;
}

STDMETHODIMP CVRPresenterStream::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	LPBYTE pBuf = 0;
	DWORD stride = 0;
	HRESULT hr  = S_OK;

	#ifdef DUMPSTREAM
	if(gStream)
	{
		for(int i=0;i<m_open_param.dwHeight;i++)
			fwrite(ppbPlanes[0] + i*pdwStrides[0],sizeof(BYTE),m_open_param.dwWidth,gStream);
		for(int i=0;i<m_open_param.dwHeight/2;i++)
			fwrite(ppbPlanes[1] + i*pdwStrides[1],sizeof(BYTE),m_open_param.dwWidth/2 ,gStream);
		for(int i=0;i<m_open_param.dwHeight/2;i++)
			fwrite(ppbPlanes[2] + i*pdwStrides[2],sizeof(BYTE),m_open_param.dwWidth/2,gStream);
	}
	#endif

	if(m_oneSufMode)
	{
   	   return m_pPresent->Present(ppbPlanes, pdwStrides, dwFlags);
	}
 	
	hr = LockSurface(&pBuf , &stride);
	if(FAILED(hr))	return hr;

	DWORD srcWidth[3];
	DWORD srcHeight[3];
	srcWidth[0] = m_open_param.dwWidth;
	srcWidth[1] = srcWidth[2] = m_open_param.dwWidth>>1;

	srcHeight[0] = m_open_param.dwHeight;
	srcHeight[1] = srcHeight[2] = m_open_param.dwHeight>>1;

	if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
		hr  = m_PrepareData.PrepareBufferYV12(pBuf, stride, ppbPlanes, 
			pdwStrides, srcWidth, srcHeight, dwFlags, 0,0,0,m_deinterlace, VP_FLIPVH_NO);
	else if(m_open_param.dwFourCC == MAKE_FOURCC('R','5','6','5'))
	{
		RECT rScaleSrc, rScaleDst;
		SetRect(&rScaleSrc, 0,0, srcWidth[0],srcHeight[0]);
		SetRect(&rScaleDst, 0,0, srcWidth[0],srcHeight[0]);
		hr = m_PrepareData.PrepareBufferR565ToR565Scaled(pBuf, stride, ppbPlanes, 
    			pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, 0,VP_FLIPVH_NO);
	}
	else if(m_open_param.dwFourCC == MAKE_FOURCC('N','V','1','2'))
	{
		if(stride == srcWidth[0])
		{
			memcpy(pBuf, 							ppbPlanes[0], srcWidth[0]*srcHeight[0]);
			memcpy(pBuf + srcWidth[0]*srcHeight[0], 	ppbPlanes[1], srcWidth[0]*srcHeight[0]/2);
		}
	}

	//if(m_open_param.dwInputFourCC == D3DFORMAT('VUYA'))
	//{
	//	memset(pBuf, 0xFF, stride*m_open_param.dwHeight*3/2);
	//}
	hr = UnlockSurface();
	if(FAILED(hr))	return hr;
	return S_OK;
}

STDMETHODIMP CVRPresenterStream::Update()
{
	HRESULT ret = S_OK;
	RECT rSrc, rDst;
	ret = m_pVideoWnd->Get(IID_IMcVideoPresenter, VE_COMMAND_SRCRECT,0,0,&rSrc, sizeof(rSrc),0);
	if(FAILED(ret))
		return ret;
	m_rectSrc = rSrc;
	
	ret = m_pVideoWnd->Get(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&rDst, sizeof(rDst),0);
	if(FAILED(ret))
		return ret;

	m_rectDst = rDst;

	return S_OK;
}

STDMETHODIMP CVRPresenterStream::StartAnimation(VR_ANIMATION_PARAMS* pParms)
{
	return E_NOTIMPL;
}

STDMETHODIMP CVRPresenterStream::StopAnimation()
{
	return E_NOTIMPL;
}


