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
#include "../../inc/CProtect.h"
#include "tchar.h"
#include "GfxPresentProcEbdSW.h"
#include "GfxPresentStreamEbdSW.h"
#include "../../src/LibGfxContext/GfxOverlay.h"

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter2.0")

#ifdef _WIN32_WCE
#define DEAULT_CANVAS_WIDTH		320
#define DEAULT_CANVAS_HEIGHT	240
#else
#define DEAULT_CANVAS_WIDTH		720
#define DEAULT_CANVAS_HEIGHT	576
#endif

extern HRESULT ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin);


CVRPresentOvlProcessorEbdSW  ::CVRPresentOvlProcessorEbdSW() :
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_pPresent(NULL),
	m_Init(false),
	m_streamCount(0),
	m_pAlphaBlend(0),
	m_vp_enable(true),
	m_zoom_mode(VP_ZOOM_MODE_CROPPING),
	m_pDll(NULL),
	m_canvasBuf(NULL),
	m_bcanChanged(false),
	m_pVideoMixer(NULL)
{
	ZeroMemory(&m_openParams, sizeof(m_openParams));	
	ZeroMemory(&m_pStmArrar, sizeof(m_pStmArrar));	
	ZeroMemory(&m_rSrc, sizeof(m_rSrc));	
	ZeroMemory(&m_rDst, sizeof(m_rDst));	
	InitializeCriticalSection(&m_csDisplay);
	m_canvasFourCC = MAKE_FOURCC('Y','V','1','2');
	m_canvasResizeBuf = NULL;
	memset(&m_android_param, 0,sizeof(m_android_param));

	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	m_pVideoWnd = CreateVideoWindow();

}

CVRPresentOvlProcessorEbdSW  ::~CVRPresentOvlProcessorEbdSW  ()
{
	if(m_pVideoWnd)
	{
		m_pVideoWnd->Release();
		m_pVideoWnd = NULL;
	}

	if(m_pDll)
	{
	#ifndef ANDROID
	#ifndef __linux__
		UnLoadV1Library(*m_pDll, &m_pPresent);
	#endif
	#endif
		delete m_pDll;
		m_pDll = NULL;
	}
	DeleteCriticalSection(&m_csDisplay);
}	


//-------------------------------------------------------------------
// InitializeWindow
//
// Creates the application window.
//-------------------------------------------------------------------

STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::Open(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize)
{
	HRESULT hr = S_OK;
	if(pOpenParams==NULL )	return E_INVALIDARG;
	if(m_Init)
		return E_UNEXPECTED;

	#ifdef ANDROID_VR_JNI
	memset(&m_android_surfparam, 0 ,sizeof(m_android_surfparam));
    #endif

	m_pVideoMixer = 0;
	memset(&m_android_param, 0,sizeof(m_android_param));
	
	if(!m_pAlphaBlend )
	{
		m_pAlphaBlend = new CAlphaBlending();
		if(m_pAlphaBlend  == NULL)
			return E_FAIL;
	}

	memcpy(&m_openParams, pOpenParams, dwOpenSize);

	m_streamCount = 0;
	memset(&m_ginfo, 0,sizeof(m_ginfo));
	m_Init = true;
	return hr;
}


STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::Close()
{
	if(!m_Init)
		return E_UNEXPECTED;
		
	if(m_pPresent)
	{
		m_pPresent->Close();
		m_pPresent->Release();
		m_pPresent = NULL;
	}

	memset(m_pStmArrar, 0,sizeof(m_pStmArrar));

	if(m_canvasBuf)
	{
		free(m_canvasBuf);
		m_canvasBuf = NULL;
	}
	if(m_canvasResizeBuf)
	{
		free(m_canvasResizeBuf);
		m_canvasResizeBuf = NULL;
	}

	if(m_pAlphaBlend)
	{
		delete m_pAlphaBlend;
		m_pAlphaBlend = NULL;
	}
	m_Init = false;
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::Render(DWORD dwRenderFlags)
{
	HRESULT hr = S_OK;

	if(!m_Init)	return VR_FAIL_UNOPEN;
	if(!m_pPresent)	return VR_FAIL_UNCREATEDSTREAM;

	hr = UpdateDestination();
	if(FAILED(hr))	return hr;
	
	if(m_bcanChanged)
		return VR_FAIL_CANVASCHANGED;

	CProtect proc(&m_csDisplay);

	DWORD surfaceNum = 0;


	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid)
		{
			RECT stmsrc = {0};
			RECT stmdst = {0};
			BOOL enable = false;
			LONG alphaVal;
			VP_ZOOM_MODE zoom_mode;

			VP_DEINTERLACE_MODE deinterlace = VP_DEINTERLACE_WEAVE;
			VP_FLIPVIDEO_MODE flipMode = VP_FLIPVH_NO;

			m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_ENABLE,0,0,&enable,0,0);
			if(!enable)
				continue;

			m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_RECTSRC,0,0,&stmsrc ,0,0);
			m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_RECTDST,0,0,&stmdst,0,0);

			m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_ZOOM,0,0,&zoom_mode,0,0);
			m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_FLIPVIDEO,0,0,&flipMode,0,0);

			if(m_openParams.canvasWidth == m_pStmArrar[i].param.dwWidth && m_openParams.canvasHeight == m_pStmArrar[i].param.dwHeight)
			{

				m_zoom_mode = zoom_mode;
				if(m_pPresent)
					hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ZOOM,0,0,&m_zoom_mode,0);
			}
			else
			{
				if(zoom_mode == VP_ZOOM_MODE_LETTERBOX)
				{
					RECT rV1;
					hr =  m_pPresent->Get(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTDST,0,0,&rV1,0,0);
					if(SUCCEEDED(hr))
					{

						RECT rOut;
						ScaleRect(&rV1,&m_rDst,&stmdst,&rOut);


						RECT rSurface;
						SetRect(&rSurface, 0,0, m_pStmArrar[i].param.dwWidth ,m_pStmArrar[i].param.dwHeight);

						VE_Options tVEOpen;
						ZeroMemory(&tVEOpen, sizeof(tVEOpen));
						tVEOpen.mode = (VE_MODE)zoom_mode;
						tVEOpen.rSrcClip= tVEOpen.rSrc= rSurface;
						tVEOpen.rDst = rOut;
						tVEOpen.aspect.dwAspectX = m_pStmArrar[i].param.dwAspectX;
						tVEOpen.aspect.dwAspectY = m_pStmArrar[i].param.dwAspectY;
						hr = m_pVideoWnd->Open(&tVEOpen,sizeof(tVEOpen));
						if(FAILED(hr)) 	
							return hr;

						hr = m_pVideoWnd->Get(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&stmdst, sizeof(stmdst),NULL);
						if(FAILED(hr))
							return hr;

						m_pVideoWnd->Close();
					}

				}
			}

			RECT rCanvas;
			SetRect(&rCanvas, 0,0, m_openParams.canvasWidth,m_openParams.canvasHeight);
			IntersectRect(&stmdst, &stmdst, &rCanvas);

			//Scaling doen'st support that it's not 4 pixcels aligned.
			DWORD tWidth = stmdst.right - stmdst.left;
			DWORD tHeight = stmdst.bottom  - stmdst.top;
			
			tWidth &= (~(4-1));
			tHeight &= (~(4-1));
			stmdst.right =  stmdst.left + tWidth ;
			stmdst.bottom =  stmdst.top + tHeight;
			
			RECT rSrc = {0};
			DWORD srcStride = 0;
			DWORD srcHeight = 0;
			
			if((deinterlace == VP_DEINTERLACE_BOB_SINGLE_FIELD) || (deinterlace == VP_DEINTERLACE_BOB_DOUBLE_FIELD))
			{
				srcStride = m_pStmArrar[i].param.dwWidth*2;
				srcHeight = m_pStmArrar[i].param.dwHeight/2;
				SetRect(&rSrc, stmsrc.left, stmsrc.top/2, stmsrc.right, stmsrc.top/2 + (stmsrc.bottom -  stmsrc.top)/2);
			}
			else
			{
				srcStride = m_pStmArrar[i].param.dwWidth;
				srcHeight = m_pStmArrar[i].param.dwHeight;
				rSrc = stmsrc;
			}

			surfaceNum ++;

 
			LPBYTE pCanvasBuf = NULL;
			DWORD canvasStride = 0;
			hr = m_pPresent->LockSurface(&pCanvasBuf , &canvasStride );
			if(FAILED(hr))
				return hr;


			if(surfaceNum == 1)
			{
				hr = ColorConvertion_Resize(m_pStmArrar[i].pBuf, pCanvasBuf, srcStride, 
						srcHeight, m_pStmArrar[i].param.dwFourCC, 
						m_openParams.canvasWidth, m_openParams.canvasHeight, 
						m_canvasFourCC, &rSrc, &stmdst,false,flipMode);
			}
			else
			{
				hr = ColorConvertion_Resize(m_pStmArrar[i].pBuf, m_canvasResizeBuf, 
						srcStride, srcHeight, m_pStmArrar[i].param.dwFourCC, 
						stmdst.right - stmdst.left, stmdst.bottom - stmdst.top, 
						m_canvasFourCC, &rSrc, &stmdst,true,flipMode);
				if(FAILED(hr))
					return hr;

				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_ALPHA,0,0,&alphaVal,0,0);
				ConvertPrecision(alphaVal, MAX_ALPHA_INTERNAL, MIN_ALPHA_INTERNAL, MAX_ALPHA_VAL,MIN_ALPHA_VAL);

				hr = TransparentOverwrite(pCanvasBuf,m_canvasResizeBuf, m_pStmArrar[i].param.dwTranColor,m_openParams.canvasWidth ,
					m_openParams.canvasHeight,stmdst.right - stmdst.left, stmdst.bottom - stmdst.top,
					&stmdst, ((float)alphaVal/MAX_ALPHA_INTERNAL));
				if(FAILED(hr)) 
					return hr;
			}
			
			if(FAILED(m_pPresent->UnlockSurface()))
				return hr;


		}
	}

	return m_pPresent->FlipSurface();

}


STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	switch (dwPropID)
	{
		case MC_PROPID_VP_DEINTERLACE_MODE:
			return E_PROP_ID_UNSUPPORTED;

		case MC_PROPID_VP_ENABLE:
			return ReturnComLong(m_vp_enable,pPropData,cbPropData,pcbReturned);

		case MC_PROPID_VP_CANVAS_INFO:
			{
				if(pPropData == NULL)	return E_INVALIDARG;
				if(!m_Init)	return VR_FAIL_UNOPEN;
				VP_CANVASINFO_PARAMS* pArgs = (VP_CANVASINFO_PARAMS*)pPropData;
				pArgs->dwWidth = m_openParams.canvasWidth;
				pArgs->dwHeight = m_openParams.canvasHeight;
				m_bcanChanged = false;
				return S_OK;
			}
		
		case MC_PROPID_VP_ZOOM:
			return ReturnComLong(m_zoom_mode,pPropData,cbPropData,pcbReturned);
		case MC_PROPID_VP_COLORSYS_CAP:
		case MC_PROPID_VP_BRIGHTNESS:
		case MC_PROPID_VP_CONTRAST:
		case MC_PROPID_VP_HUE:
		case MC_PROPID_VP_SATURATION:
		case MC_PROPID_VP_GAMMA:
		case MC_PROPID_VP_COLORENABLE:
		case MC_PROPID_VP_SHARPNESS:
			if(m_pPresent == NULL)
				return E_UNEXPECTED;
			
			return (m_pPresent->Get(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData,pcbReturned));

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
		default:
			break;
	}
	return E_PROP_ID_UNSUPPORTED;
}


STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	HRESULT hr = E_PROP_ID_UNSUPPORTED;
	if(!pPropData)
		return E_FAIL;

	switch (dwPropID)
	{
		case MC_PROPID_VP_DEINTERLACE_MODE:
			return E_PROP_ID_UNSUPPORTED;
		case MC_PROPID_VP_RECTDST:
		{
			if(m_openParams.hWnd)
				return S_OK;
			
			m_rDst = *reinterpret_cast<RECT *>(pPropData);
			if(m_pPresent && (m_streamCount== 1))
				hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,&m_rDst,0);
			else
				hr = S_OK;
		}
		break;
		case MC_PROPID_VP_ENABLE:
		{
			m_vp_enable = *reinterpret_cast<BOOL*>(pPropData);
			if(m_pPresent)
				hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,&m_vp_enable,0);
			else
				hr = S_OK;
		}
		break;
		case MC_PROPID_VP_VM_WND:
		{
			m_pVideoMixer = reinterpret_cast<ICivmVideoMixer*>(pPropData);
			hr = S_OK;
		}
		break;


		case MC_PROPID_VP_ANDROID_ISURFACE:
		{
			DP("=====PROCEBD set MC_PROPID_VP_ANDROID_ISURFACE :%d \n", pPropData);
			if(pPropData)
			{
			m_android_param = *reinterpret_cast<VR_ANDROID_ISUR_PARAM*>(pPropData);
			if(m_pPresent)
				hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData);
			}
		}
		break;
		case MC_PROPID_VP_WND:
			{
	           		m_openParams.hWnd= *((HWND*)pPropData);

				if(m_pPresent)
					hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData);
			}
		break;

	#ifdef ANDROID_VR_JNI
		case MC_PROPID_VP_ANDROID_SURFACE:
		{
			DP("=====PROC set MC_PROPID_VP_ANDROID_SURFACE :%d \n", pPropData);
			if(pPropData)
			{

				m_android_surfparam = *reinterpret_cast<VR_ANDROID_SURF_PARAM*>(pPropData);
				if(m_pPresent)
				{
					hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData);
				}
			}
		}
		break;
    #endif

		default:
		{
			if(m_pPresent)
				hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData);
			else
				hr = S_OK;
		}
		break;
	}
	return hr;
}


STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream)
{
	HRESULT hr = S_OK;
	if(!pOpenParams || !ppStream )
	{
    	DP("%s () :L:%d E_INVALIDARG\n",__FUNCTION__, __LINE__);
		return E_INVALIDARG;
	}

	if(!m_Init)	
	{
    	DP("%s () :L:%d VR_FAIL_UNOPEN\n",__FUNCTION__, __LINE__);
      return VR_FAIL_UNOPEN;
	}
//Init a free stream	
	UINT i;
	for(i=0;i<MAX_STREAM ;i++)
	{
		if(!m_pStmArrar[i].valid)	break;
	}

	if(i>= MAX_STREAM)	
	{
    	DP("%s () :L:%d E_UNEXPECTED\n",__FUNCTION__, __LINE__);
      return E_UNEXPECTED;
	}
	memcpy(&m_pStmArrar[i].param, pOpenParams, sizeof(VP_OPEN_STREAM_PARAMS));

	DP("chromeformat:0x%x \n", pOpenParams->dwFourCC);
	if(isSW_YV12(pOpenParams->dwFourCC))
		m_pStmArrar[i].param.dwFourCC = MAKE_FOURCC('Y','V','1','2');
	else if(isSW_NV12(pOpenParams->dwFourCC))
		m_pStmArrar[i].param.dwFourCC = MAKE_FOURCC('N','V','1','2');
	else if(isSW_RGB565(pOpenParams->dwFourCC))
		m_pStmArrar[i].param.dwFourCC = MAKE_FOURCC('R','5','6','5');

	switch(m_pStmArrar[i].param.dwFourCC )
	{
		case MAKE_FOURCC('Y','V','1','2'):
		case MAKE_FOURCC('N','V','1','2'):
			m_pStmArrar[i].pBuf = (LPBYTE)malloc(pOpenParams->dwWidth*pOpenParams->dwHeight*3/2);
			if(!m_pStmArrar[i].pBuf ) 
			{
              DP("%s () :L:%d E_FAIL\n",__FUNCTION__, __LINE__);
              return E_FAIL;
			}
			memset(m_pStmArrar[i].pBuf, 0x00,pOpenParams->dwWidth*pOpenParams->dwHeight);
			memset(m_pStmArrar[i].pBuf+pOpenParams->dwWidth*pOpenParams->dwHeight, 0x80,pOpenParams->dwWidth*pOpenParams->dwHeight/2);
			break;
		case MAKE_FOURCC('R','5','6','5'):
			m_pStmArrar[i].pBuf = (LPBYTE)malloc(pOpenParams->dwWidth*pOpenParams->dwHeight*2);
			if(!m_pStmArrar[i].pBuf ) 
			{
              DP("%s () :L:%d E_FAIL\n",__FUNCTION__, __LINE__);
              return E_FAIL;
			}
			memset(m_pStmArrar[i].pBuf, 0x00,pOpenParams->dwWidth*pOpenParams->dwHeight*2);

			break;
		default:
            DP("%s () :L:%d E_FAIL pOpenParams->dwFourCC:%p \n",__FUNCTION__, __LINE__,pOpenParams->dwFourCC);
			return E_FAIL;
	}


	if(!m_pDll)
		m_pDll = new HMODULE;

	if(m_pPresent == NULL)
	{

#ifndef __linux__
#ifndef ANDROID
		hr = LoadV1Library(*m_pDll,&m_pPresent);
		if(FAILED(hr))	
      {
            DP("%s () :L:%d hr:%d \n",__FUNCTION__, __LINE__,hr);
      		return hr;
      }
#endif
#else
		m_pPresent = CreateVideoPresenter();
#endif
	}

	CProtect proc(&m_csDisplay);
	if(m_streamCount == 0 ||(pOpenParams->dwWidth * pOpenParams->dwHeight > m_openParams.canvasWidth*m_openParams.canvasHeight))
	{
      m_openParams.canvasWidth = pOpenParams->dwWidth;
      m_openParams.canvasHeight = pOpenParams->dwHeight;

        DP("CANVAS size:%d,%d \n",m_openParams.canvasWidth,m_openParams.canvasHeight);
		hr = CreateV1VR(&m_openParams);
		if(FAILED(hr))
		{
			goto RET;
		}

    	memset(&m_ginfo, 0 ,sizeof(m_ginfo));
    	m_ginfo.dwSize = sizeof(m_ginfo);
    	if(FAILED(m_pPresent->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_INTRNAL_SURFINFO, 
    	    0,0,&m_ginfo,0,0)))
    	{
          DP("%s-%s-L:%d failed \n",__FILE__,__FUNCTION__,__LINE__);
          return E_FAIL;
    	}
        m_openParams.canvasFourCC = m_canvasFourCC = m_ginfo.dwFourCC;
        DP("canvas type:%p \n",m_canvasFourCC );

		#ifndef PLATFROM_CROPPING_SUPPORT
        DP("CANVAS size:%d,%d \n",m_openParams.canvasWidth,m_openParams.canvasHeight);
		hr = CreateCanvas(m_openParams.canvasWidth, m_openParams.canvasHeight);
		if(FAILED(hr))
		{
			goto RET;
		}
		#endif

	}


	m_streamCount++;

	m_pStmArrar[i].pStream = new CVRPresenterStreamEbdSW(m_pStmArrar[i].pBuf, &m_csDisplay, &m_pStmArrar[i].param, &m_pPresent);
	if(!m_pStmArrar[i].pStream)	goto RET;

	m_pStmArrar[i].valid = true;

//Return them
	if(ppStream)
		*ppStream = static_cast<IMcVideoPresenterStream*>(m_pStmArrar[i].pStream);

    DP("%s(): %d format:%p\n",__FUNCTION__,i,pOpenParams->dwFourCC);
	return hr;
	
RET:
	if(m_pStmArrar[i].pBuf)
		free(m_pStmArrar[i].pBuf);
	DP("%s () :L:%d E_FAIL \n",__FUNCTION__, __LINE__);

	return E_FAIL;
}


STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::DestroyStream(IN IMcVideoPresenterStream *ppStream)
{
	if(!ppStream)	return E_FAIL;
	for(int i=0;i<MAX_STREAM;i++)
	{
		if(m_pStmArrar[i].pStream == ppStream)
		{
			DP("%s(): %d \n",__FUNCTION__,i);

			CProtect proc(&m_csDisplay);
			m_pStmArrar[i].pStream->Release();
			free(m_pStmArrar[i].pBuf);
			ZeroMemory(&m_pStmArrar[i], sizeof(SSTREAM_INFO));
			m_streamCount--;
			return S_OK;
		}
	}
	return E_NOTIMPL;
}

STDMETHODIMP CVRPresentOvlProcessorEbdSW ::ColorConvertion_Resize(void* pInput,
  void* pOutput,DWORD input_width, DWORD input_height, DWORD input_format,
  DWORD output_width, DWORD output_height, DWORD output_format, RECT* pCropRect,
  RECT* pDstRect, bool forTranspRent, LONG flipMode)
{
	if((pInput == NULL)||(pOutput == NULL))
		return E_FAIL;
	
	CI_IMAGEPROC_SIZE sSrcSize,sDstSize;
	memset(&sSrcSize, 0,sizeof(sSrcSize));
	memset(&sDstSize, 0,sizeof(sDstSize));

	
	sSrcSize.s32Width = pCropRect->right - pCropRect->left;// input_width;
	sSrcSize.s32Height = pCropRect->bottom - pCropRect->top;//input_height;

	sDstSize.s32Width = pDstRect->right - pDstRect->left;
	sDstSize.s32Height = pDstRect->bottom - pDstRect->top;

	if(input_format == MAKEFOURCC('Y','V','1','2'))
	{
	    if(output_format == MAKEFOURCC('Y','V','1','2'))
	    {
          	int srcStride[3]; 
          	srcStride[0] = (int)input_width;
          	srcStride[1] = (int)input_width>>1;
          	srcStride[2] = (int)input_width>>1;


          	unsigned char *SrcBuf[3];
          	SrcBuf[0] = (unsigned char *)pInput  +pCropRect->top*input_width + pCropRect->left;
          	SrcBuf[2] = (unsigned char *)pInput + input_width*input_height + pCropRect->top/2*input_width/2 + pCropRect->left/2;
          	SrcBuf[1] = (unsigned char *)pInput + input_width*input_height*5/4 + pCropRect->top/2*input_width/2 + pCropRect->left/2;

          	unsigned char *DstBuf[3];
          	if(forTranspRent)
          	{
          		DstBuf[0] = (unsigned char *)pOutput ;
          		DstBuf[2] = (unsigned char *)pOutput + output_width*output_height ;
          		DstBuf[1] = (unsigned char *)pOutput + output_width*output_height*5/4 ;
          	}
          	else
          	{
          		DstBuf[0] = (unsigned char *)pOutput +pDstRect->top*output_width + pDstRect->left; ;
          		DstBuf[2] = (unsigned char *)pOutput + output_width*output_height + pDstRect->top/2*output_width /2 + pDstRect->left/2;
          		DstBuf[1] = (unsigned char *)pOutput + output_width*output_height*5/4 + pDstRect->top/2*output_width /2 + pDstRect->left/2;
          	}


          	int dstStride[3]; 
          	dstStride[0] = (int)output_width;
          	dstStride[1] = (int)output_width>>1;
          	dstStride[2] = (int)output_width>>1;

          	return (m_PrepareData.PrepareBufferYV12Scaled(SrcBuf,srcStride, sSrcSize, DstBuf, 
          		dstStride, sDstSize, CI_IMAGEPROC_ROTATION_DISABLE,flipMode));
	    }
  	    else if(output_format == MAKEFOURCC('R','5','6','5'))
  	    {
          	DWORD srcStride[3]; 
          	srcStride[0] = (int)input_width;
          	srcStride[1] = (int)input_width>>1;
          	srcStride[2] = (int)input_width>>1;


          	unsigned char *SrcBuf[3];
          	SrcBuf[0] = (unsigned char *)pInput  +pCropRect->top*input_width + pCropRect->left;
          	SrcBuf[2] = (unsigned char *)pInput + input_width*input_height + pCropRect->top/2*input_width/2 + pCropRect->left/2;
          	SrcBuf[1] = (unsigned char *)pInput + input_width*input_height*5/4 + pCropRect->top/2*input_width/2 + pCropRect->left/2;

          	unsigned char *DstBuf[3];
          	if(forTranspRent)
          	{
          		DstBuf[0] = (unsigned char *)pOutput ;
          	}
          	else
          	{
          		DstBuf[0] = (unsigned char *)pOutput +pDstRect->top*output_width + pDstRect->left; ;
          	}


          	int dstStride[3]; 
          	dstStride[0] = (int)output_width*2;

          	return (m_PrepareData.PrepareBufferRGB565Scaled(DstBuf[0], output_width*2,
               	SrcBuf,srcStride, pCropRect, pDstRect,0, CI_IMAGEPROC_ROTATION_DISABLE,
				VP_DEINTERLACE_None,VP_FLIPVH_NO));

  	    }

	}

	return E_FAIL;
}

STDMETHODIMP CVRPresentOvlProcessorEbdSW ::TransparentOverwrite(void* pBackground,void* pSrc,DWORD dwSrcTransColor, DWORD dwBackWidth, DWORD dwBackHeight,
DWORD dwSrcWidth,DWORD dwSrcHeight, RECT* pSrcOffset, FLOAT alpha)
{
	if(pBackground== NULL || pSrc==NULL )
		return E_FAIL;

	if(alpha == 0.0f)	return S_OK;

	unsigned char* background_buf[3];
	unsigned char* src_buf[3];
	background_buf[0] = (unsigned char*)LPBYTE(pBackground);
	background_buf[1] = (unsigned char*)LPBYTE(pBackground) + dwBackWidth *dwBackHeight ;
	background_buf[2] = (unsigned char*)LPBYTE(pBackground) + dwBackWidth *dwBackHeight*5/4;


	src_buf[0] = (unsigned char*)LPBYTE(pSrc);
	src_buf[1] = (unsigned char*)LPBYTE(pSrc)+ dwSrcWidth *dwSrcHeight;
	src_buf[2] = (unsigned char*)LPBYTE(pSrc)+ dwSrcWidth*dwSrcHeight*5/4;

	if(alpha == 1.0f)
	{
		DWORD offsetY = pSrcOffset->top*dwBackWidth + pSrcOffset->left;
		DWORD offsetUV = pSrcOffset->top/2*dwBackWidth/2 + pSrcOffset->left/2;

		for(DWORD i=0;i<dwSrcHeight;i++)
			memcpy(background_buf[0] + dwBackWidth*i + offsetY ,src_buf[0] + dwSrcWidth*i,  dwSrcWidth );

		for(DWORD i=0;i<dwSrcHeight/2;i++)
			memcpy(background_buf[1] + dwBackWidth/2*i + offsetUV,src_buf[1] + dwSrcWidth/2*i,  dwSrcWidth/2 );

		for(DWORD i=0;i<dwSrcHeight/2;i++)
			memcpy(background_buf[2] + dwBackWidth/2*i + offsetUV,src_buf[2] + dwSrcWidth/2*i,  dwSrcWidth/2 );

	}
	else
	{
		m_pAlphaBlend->SetAlphaBlendingYUV(dwBackWidth, dwBackHeight,
			dwSrcWidth, dwSrcHeight, pSrcOffset->left,pSrcOffset->top,0,0,0,alpha);

		if(m_canvasFourCC == MAKEFOURCC('Y','V','1','2'))
		{
			unsigned char y = 0,u = 0,v = 0;
			y =(unsigned char)((dwSrcTransColor&0xff0000)>>16);
			u =(unsigned char)((dwSrcTransColor&0xff00)>>8);
			v =(unsigned char)dwSrcTransColor&0xff;
			m_pAlphaBlend->SetAlphaBlendingBackgroundColor(y,u,v);
		}

		m_pAlphaBlend->DoAlphaBlendingYUV(background_buf[0], background_buf[1], background_buf[2],
			src_buf[0] , src_buf[1] , src_buf[2]);

	}
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorEbdSW ::GetFourCCLength(DWORD fourCC, DWORD *plength)
{
	if(plength == NULL)
		return E_FAIL;
	
	switch(fourCC)
	{
		case MAKEFOURCC('Y','V','1','2'):
		case MAKEFOURCC('N','V','1','2'):
		case MAKEFOURCC('Y','U','Y','2'):
			*plength = 1;
			break;
		case MAKEFOURCC('R','5','6','5'):
			*plength = 2;
			break;
		default:
			break;
	}
	
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorEbdSW  ::CreateCanvas(DWORD w, DWORD h)
{
	if(m_canvasBuf)
	{
		delete m_canvasBuf;
		m_canvasBuf = NULL;
	}

	if(m_canvasResizeBuf)
	{
		delete m_canvasResizeBuf;
		m_canvasResizeBuf  = NULL;
	}
	
	switch(m_canvasFourCC)
	{
		case MAKE_FOURCC('Y','V','1','2'):
			m_canvasBuf = (LPBYTE)malloc(w*h*3/2);
			if(!m_canvasBuf )	return E_FAIL;
			memset(m_canvasBuf, 0x0 ,w*h);
			memset(m_canvasBuf + w*h, 0x80 ,w*h/2);

			m_canvasResizeBuf = (LPBYTE)malloc(w*h*3/2);
			if(!m_canvasResizeBuf )	return E_FAIL;
			memset(m_canvasResizeBuf , 0x0 ,w*h);
			memset(m_canvasResizeBuf + w*h, 0x80 ,w*h/2);

			break;
		case MAKE_FOURCC('R','5','6','5'):
			m_canvasBuf = (LPBYTE)malloc(w*h*2);
			if(!m_canvasBuf )	return E_FAIL;
			memset(m_canvasBuf, 0 ,w*h*2);			

			m_canvasResizeBuf = (LPBYTE)malloc(w*h*2);
			if(!m_canvasResizeBuf )	return E_FAIL;
			memset(m_canvasResizeBuf , 0 ,w*h*2);			
			break;
		default:
			return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CVRPresentOvlProcessorEbdSW::CreateV1VR(struct VP_OpenOptionsAdv* pOpenParm)
{
	if(!m_pPresent)	return E_FAIL;
	
	m_pPresent->Close();

	VP_OpenOptions sInit;
	ZeroMemory(&sInit, sizeof(sInit));
	sInit.dwWidth = pOpenParm->canvasWidth;
	sInit.dwHeight= pOpenParm->canvasHeight;
	sInit.dwFourCC = 0;
	sInit.hWnd = m_openParams.hWnd;
	HRESULT hr = m_pPresent->Open(&sInit, sizeof(sInit));
	if(FAILED(hr))	return E_FAIL;

	UpdateDestination();
	Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&m_vp_enable,0);
	Set(IID_IMcVideoPresenter,MC_PROPID_VP_ZOOM,0,0,&m_zoom_mode,0);	
	m_bcanChanged = true;
	

	if(m_android_param.pAndroidSFBuf)
	{
		m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ANDROID_ISURFACE,0,0,&m_android_param,0);
	}

	#ifdef ANDROID_VR_JNI
	if(m_android_surfparam.m_pJvm)
	{
		if(m_pPresent)
			m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ANDROID_SURFACE,0,0,&m_android_surfparam,0);
	}
    #endif

	return S_OK;
}

STDMETHODIMP CVRPresentOvlProcessorEbdSW::UpdateDestination()
{
#ifndef ANDROID
	if(m_openParams.hWnd)
	{
		::GetClientRect(m_openParams.hWnd, &m_rWnd);
		m_rDst = m_rWnd;
	}
#endif
	if(m_pPresent)
		m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTDST,0,0,&m_rDst,0);

	return S_OK;
}

STDMETHODIMP CVRPresentOvlProcessorEbdSW ::ScaleRect(RECT* pSrc1Small,RECT* pSrc2Big,RECT* pDst1Big, RECT* pDst2Small)
{
	float WidthFactor = (float)(pSrc1Small->right- pSrc1Small->left)/(float)(pSrc2Big->right - pSrc2Big->left);
	float HeightFactor = (float)(pSrc1Small->bottom- pSrc1Small->top)/(float)(pSrc2Big->bottom - pSrc2Big->top);

	DWORD newWidth = (DWORD)WidthFactor*(pDst1Big->right - pDst1Big->left);
	DWORD newHeight = (DWORD)HeightFactor*(pDst1Big->bottom - pDst1Big->top);

	DWORD newX =(pDst1Big->right + pDst1Big->left)/2 - newWidth/2;
	DWORD newY = (pDst1Big->bottom + pDst1Big->top)/2 - newHeight/2;

	pDst2Small->left = newX;
	pDst2Small->top = newY;
	pDst2Small->right= newX+ newWidth;
	pDst2Small->bottom= newY + newHeight;
	
	return S_OK;
}

int CVRPresentOvlProcessorEbdSW::GetStreamNumber()
{
	int streamNum = 0;
	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid)	
			streamNum++;
	}
	return streamNum;
}

