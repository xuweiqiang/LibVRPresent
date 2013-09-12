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
#include "GfxPresentProcSW.h"
#include "GfxPresentStreamSW.h"

#define MAX_ALPHA_INTERNAL 100
#define MIN_ALPHA_INTERNAL 0

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter2.0")

extern HRESULT ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin);


CVRPresentOvlProcessorSW ::CVRPresentOvlProcessorSW () :
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_pPresent(NULL),
	m_Init(false),
	m_streamCount(0),
	m_pAlphaBlend(0),
	m_zoom_mode(VP_ZOOM_MODE_STRETCH),
	m_canvasBuf(NULL),
	m_bcanChanged(false),
	m_pVideoMixer(NULL)
{
	ZeroMemory(&m_openParams, sizeof(m_openParams));	
	ZeroMemory(&m_openParamsVR1, sizeof(m_openParamsVR1));	
	ZeroMemory(&m_pStmArrar, sizeof(m_pStmArrar));	
	ZeroMemory(&m_rSrc, sizeof(m_rSrc));	
	ZeroMemory(&m_rDst, sizeof(m_rDst));	
	InitializeCriticalSection(&m_csDisplay);
	m_canvasFourCC = MAKE_FOURCC('Y','V','1','2');
	m_canvasResizeBuf = NULL;

	m_pDll = 0;
	m_VRopened = false;
	memset(&m_android_param, 0,sizeof(m_android_param));


#ifndef __linux__
#ifndef ANDROID
		LoadV1Library(m_pDll,&m_pPresent);
#endif
#else
		m_pPresent = CreateVideoPresenter();
#endif

}

CVRPresentOvlProcessorSW ::~CVRPresentOvlProcessorSW ()
{
	#ifndef ANDROID
	#ifndef __linux__
	if(m_pPresent)
	{
		UnLoadV1Library(m_pDll, &m_pPresent);
	}
	#endif
	#endif
	DeleteCriticalSection(&m_csDisplay);
}	


//-------------------------------------------------------------------
// InitializeWindow
//
// Creates the application window.
//-------------------------------------------------------------------

STDMETHODIMP CVRPresentOvlProcessorSW ::Open(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize)
{
	HRESULT hr = S_OK;
	if(pOpenParams==NULL )	return E_INVALIDARG;
	if(m_Init)
		return E_UNEXPECTED;

	ZeroMemory(&m_openParamsVR1, sizeof(m_openParamsVR1));	
	m_pVideoMixer = 0;
	memset(&m_android_param, 0,sizeof(m_android_param));
	#ifdef ANDROID_VR_JNI
	memset(&m_android_surfparam, 0 ,sizeof(m_android_surfparam));
    #endif
	if(!m_pAlphaBlend )
	{
		m_pAlphaBlend = new CAlphaBlending();
		if(m_pAlphaBlend  == NULL)
			return E_FAIL;
	}

	memcpy(&m_openParams, pOpenParams, dwOpenSize);


	//Use invalid cropWidth/height to trigger initilizaton
	m_cropwidth  = 1000;
	m_cropheight  = 1;

	m_streamCount = 0;
	m_lFlipVideoMode  = VP_FLIPVH_NO;


	m_Init = true;
	return hr;
}

STDMETHODIMP CVRPresentOvlProcessorSW ::Close()
{
	if(!m_Init)
		return E_UNEXPECTED;
		
	if(m_pPresent)
	{
		m_pPresent->Close();
	}

	/*
	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid && m_pStmArrar[i].pBuf)	
		{
			EnterCriticalSection(&m_csDisplay);
			m_pStmArrar[i].pStream->Release();
			free(m_pStmArrar[i].pBuf);
			ZeroMemory(&m_pStmArrar[i], sizeof(SSTREAM_INFO));
			LeaveCriticalSection(&m_csDisplay);
			m_streamCount--;
		}
	}*/
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

	m_VRopened = false;
	m_Init = false;
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorSW ::Render(DWORD dwRenderFlags)
{
	HRESULT hr = S_OK;
	if(!m_Init)	return VR_FAIL_UNOPEN;

	if(!m_pPresent)return VR_FAIL_UNCREATEDSTREAM;
    DWORD tick[10];
    tick[0] = GetTickCount();
#if 1//def _WIN32_WCE
	if(m_streamCount== 1)
	{
		for(int i=0;i<MAX_STREAM ;i++)
		{
			if(m_pStmArrar[i].valid)
			{
				VP_DEINTERLACE_MODE deinterlace = VP_DEINTERLACE_WEAVE;
				hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_DEINTERLACE_MODE,0,0,&deinterlace,0,0);
				if(SUCCEEDED(hr))
				{
					if(m_pPresent)
						hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_DEINTERLACE_MODE,0,0,&deinterlace,0);
				}

				VP_ZOOM_MODE zoomMode;
				hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_ZOOM,0,0,&zoomMode,0,0);
				if(SUCCEEDED(hr))
				{
					if(m_pPresent)
						hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ZOOM,0,0,&zoomMode,0);
				}

				VP_FLIPVIDEO_MODE flipMode;
				hr = m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_FLIPVIDEO,0,0,&flipMode,0,0);
				if(SUCCEEDED(hr))
				{
					if(m_pPresent)
						hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_FLIPVIDEO,0,0,&flipMode,0);
				}

			}

		}
		return S_OK;
	}
#endif

	float aspwnd = 0.0f;
#if 1
	#ifndef ANDROID
    	#ifndef __linux
    	if(m_openParams.hWnd)
    	{
    		::GetClientRect(m_openParams.hWnd, &m_rWnd);
    		AdjustCanvasSize(m_rWnd.right - m_rWnd.left, m_rWnd.bottom  - m_rWnd.top, &aspwnd);
    	}
    	#endif
	#else

	#ifndef GFX_ANDROID_ANativeWindow
        RECT rSurfSize= {0,0,0,0};
		hr = m_pPresent->Get(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECT_SCREEN,
		0,0,&rSurfSize,sizeof(rSurfSize),0);
      if(SUCCEEDED(hr))
      {
//        DP("MC_PROPID_VP_RECT_SCREEN:%d,%d \n",rSurfSize.right - rSurfSize.left, rSurfSize.bottom  - rSurfSize.top);
    		AdjustCanvasSize(rSurfSize.right - rSurfSize.left, rSurfSize.bottom  - rSurfSize.top, &aspwnd);
      }
	#endif
	#endif
#endif
	if(m_bcanChanged)
		return VR_FAIL_CANVASCHANGED;

	if(m_canvasFourCC == MAKE_FOURCC('Y','V','1','2'))
	{
		memset(m_canvasBuf, 0x0 ,m_cropwidth *m_cropheight);
		memset(m_canvasBuf + m_cropwidth *m_cropheight, 0x80 ,m_cropwidth *m_cropheight/2);
	}

	CProtect proc(&m_csDisplay);

	UINT iRenderLayer= 0;
	UINT iMaxRenderLayer= 0;
	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid)
		{
			m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_LAYER_ID,0,0,&iRenderLayer,0,0);
			if(iRenderLayer > iMaxRenderLayer)
				iMaxRenderLayer = iRenderLayer;
		}
	}

	DWORD surfaceNum = 0;
	for(iRenderLayer= 0;iRenderLayer<= iMaxRenderLayer;iRenderLayer++)
	{

		for(int i=0;i<MAX_STREAM ;i++)
		{
			if(m_pStmArrar[i].valid)
			{
				UINT currLayerId = 0;
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,MC_PROPID_VP_LAYER_ID,0,0,&currLayerId,0,0);
				if(currLayerId != iRenderLayer)
					continue;
				
				RECT stmsrc = {0,0,0,0};
				RECT stmdst = {0,0,0,0};
				BOOL enable = false;
				LONG alphaVal = 255;
				VP_DEINTERLACE_MODE deinterlace = VP_DEINTERLACE_WEAVE;
				VP_ZOOM_MODE zoom = VP_ZOOM_MODE_STRETCH;
				VP_FLIPVIDEO_MODE flipMode = VP_FLIPVH_NO;
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,
					MC_PROPID_VP_RECTSRC,0,0,&stmsrc ,0,0);
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,
					MC_PROPID_VP_RECTDST,0,0,&stmdst,0,0);
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,
					MC_PROPID_VP_ENABLE,0,0,&enable,0,0);
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,
					MC_PROPID_VP_ALPHA,0,0,&alphaVal,0,0);
				ConvertPrecision(alphaVal, MAX_ALPHA_INTERNAL, MIN_ALPHA_INTERNAL, 
					MAX_ALPHA_VAL,MIN_ALPHA_VAL);
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,
					MC_PROPID_VP_DEINTERLACE_MODE,0,0,&deinterlace,0,0);
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,
					MC_PROPID_VP_ZOOM,0,0,&zoom,0,0);
				m_pStmArrar[i].pStream->Get(IID_IMcVideoPresenterAdv,
					MC_PROPID_VP_FLIPVIDEO,0,0,&flipMode,0,0);

//Unknow code
				/*
				if(zoom ==VP_ZOOM_MODE_CROPPING )
				{
					DWORD srcWidth = stmsrc.right - stmsrc.left;
					DWORD srcHeight = stmsrc.bottom - stmsrc.top;
					
					float srcAsp = (float)((float)srcWidth/(float)srcHeight);
					if(aspwnd > srcAsp)
					{
						DWORD NewSrcHeight = (long)(srcWidth/aspwnd);
						DWORD offset = (srcHeight - NewSrcHeight)/2;
						stmsrc.top += offset;
						stmsrc.bottom -= offset;
					}
					else
					{
						DWORD NewSrcWidth = (long)(srcHeight*aspwnd);
						DWORD offset = (srcWidth - NewSrcWidth)/2;
						stmsrc.left += offset;
						stmsrc.right -= offset;
					}
				}
				*/

				if(!enable)
					continue;

				RECT shareArea;
				RECT rCanvas;
				SetRect(&rCanvas, 0,0,m_cropwidth  ,m_cropheight);
				IntersectRect(&shareArea, &stmdst, &rCanvas);
				if(shareArea.left == 0 && shareArea.top ==0 && shareArea.right == 0 &&shareArea.bottom == 0)
					continue;
				
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
					if(stmdst.bottom > (LONG)m_cropheight)	stmdst.bottom =m_cropheight;
					if(stmdst.left < 0)	stmdst.left =0;
					if(stmdst.right > (LONG)m_cropwidth )	stmdst.right =m_cropwidth ;

					//Can do flash while area is out side 
					if(stmdst.right < 0 || stmdst.bottom < 0 ||   
						stmdst.left >(LONG)m_cropwidth  || stmdst.top > (LONG)m_cropheight)	continue;

					
					stmsrc.left = stmsrc.left/2*2;
					stmsrc.top = stmsrc.top/2*2;
					stmsrc.right = stmsrc.right/2*2;
					stmsrc.bottom = stmsrc.bottom/2*2;

					stmdst.left = stmdst.left/2*2;
					stmdst.top = stmdst.top/2*2;
					stmdst.right = stmdst.right/2*2;
					stmdst.bottom = stmdst.bottom/2*2;
				}


				//Scaling doen'st support that it's not 4 pixcels aligned.
				DWORD tWidth = stmdst.right - stmdst.left;
				DWORD tHeight = stmdst.bottom  - stmdst.top;
				
				tWidth &= (~(4-1));
				tHeight &= (~(4-1));
				stmdst.right =  stmdst.left + tWidth ;
				stmdst.bottom =  stmdst.top + tHeight;
				
				RECT rSrc = {0,0,0,0};
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

				stmdst.left &= ~(2-1);
				stmdst.top &= ~(2-1);
				stmdst.right &= ~(2-1);
				stmdst.bottom &= ~(2-1);

				surfaceNum ++;
				if(surfaceNum == 1)
				{

	                tick[1] = GetTickCount();
					hr = ColorConvertion_Resize(m_pStmArrar[i].pBuf, m_canvasBuf, 
							srcStride, srcHeight, m_pStmArrar[i].param.dwFourCC, 
							m_cropwidth , m_cropheight, 
							m_canvasFourCC, &rSrc, &stmdst,false,flipMode);
	                tick[2] = GetTickCount();
				}
				else
				{
					//TransparentOverwrite doesn't support stride. So 
					//stmdst.left = 0;stmdst.right = m_cropwidth ;
					tick[1] = GetTickCount();
					hr = ColorConvertion_Resize(m_pStmArrar[i].pBuf, m_canvasResizeBuf,
							srcStride, srcHeight, m_pStmArrar[i].param.dwFourCC, 
							stmdst.right - stmdst.left, stmdst.bottom - stmdst.top, 
							m_canvasFourCC, &rSrc, &stmdst,true,flipMode);
					tick[2] = GetTickCount();

					if(FAILED(hr)) return hr;

					hr = TransparentOverwrite(m_canvasBuf,m_canvasResizeBuf, m_pStmArrar[i].param.dwTranColor,m_cropwidth  ,
						m_cropheight,stmdst.right - stmdst.left, stmdst.bottom - stmdst.top,
						&stmdst, ((float)alphaVal/MAX_ALPHA_INTERNAL));
					if(FAILED(hr)) return hr;

					tick[3] = GetTickCount();
				}
	
			}
		}

	}

	LPBYTE curr[3];
	curr[0] = (LPBYTE )m_canvasBuf;
	curr[1] = LPBYTE(m_canvasBuf) + m_cropwidth *m_cropheight;
	curr[2] = LPBYTE(m_canvasBuf) + m_cropwidth *m_cropheight*5/4;

	DWORD strides[3];
	strides[0] = m_cropwidth ;
	strides[1] = strides[2] = m_cropwidth >>1;

	hr =m_pPresent->Present(curr,strides, 0);

    tick[9] = GetTickCount();
#ifdef DUMP_FPS
    if(tick[9]-tick[0] > 30)
    {
      DP("Render too slow:%d msec \n",tick[9] - tick[0]);
      DP("   -Prepare cost :%d msec \n",tick[1] - tick[0]);
      DP("   -CC&Rez cost :%d msec \n",tick[2] - tick[1]);
      DP("   -Transparent cost :%d msec \n",tick[3] - tick[2]);
    }
#endif    
	return hr;
}


STDMETHODIMP CVRPresentOvlProcessorSW ::Get(IN REFGUID guidPropSet, IN DWORD dwPropID,
	IN LPVOID pInstanceData, 	IN DWORD cbInstanceData, IN LPVOID pPropData, 
	IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	switch (dwPropID)
	{
		case MC_PROPID_VP_DEINTERLACE_MODE:
			return E_PROP_ID_UNSUPPORTED;
			
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
		case MC_PROPID_VP_CANVAS_INFO:
			{
				if(pPropData == NULL)	return E_INVALIDARG;
				if(!m_Init)	return VR_FAIL_UNOPEN;
				VP_CANVASINFO_PARAMS* pArgs = (VP_CANVASINFO_PARAMS*)pPropData;
				pArgs->dwWidth = m_cropwidth;
				pArgs->dwHeight = m_cropheight;
				m_bcanChanged = false;
				return S_OK;
			}
		
		case MC_PROPID_VP_ZOOM:
			return ReturnComLong(m_zoom_mode,pPropData,cbPropData,pcbReturned);
		case MC_PROPID_VP_FLIPVIDEO:
			return ReturnComLong(m_lFlipVideoMode,pPropData,cbPropData,pcbReturned);

		case MC_PROPID_VP_FRAMEBUFFER_LOCK:
			{
				if(pPropData == NULL)	return E_INVALIDARG;
				if(!m_Init)	return VR_FAIL_UNOPEN;
				VP_FRAMEBUFF_PARAMS* pParams = (VP_FRAMEBUFF_PARAMS*)pPropData ;
				memset(pParams, 0 ,sizeof(VP_FRAMEBUFF_PARAMS));

				pParams->fourCC = m_canvasFourCC;
				pParams->stride  = pParams->width = m_cropwidth ;
				pParams->height = m_cropheight;
				pParams->pBuf = m_canvasBuf;
				
				EnterCriticalSection(&m_csDisplay);
				return S_OK;
			}
		case MC_PROPID_VP_FRAMEBUFFER_UNLOCK:
			{
				LeaveCriticalSection(&m_csDisplay);
				return S_OK;
			}
	    case MC_PROPID_VP_COLORSYS_CAP:
  	    {
  			if(m_pPresent)
  				return (m_pPresent->Get(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData,pcbReturned));
            else
            {
              DP("Get MC_PROPID_VP_COLORSYS_CAP == m_pPresent = NULL \n");
              return E_UNEXPECTED;
            }
  	    }
  	    break;
		default:
			break;
	}
	return E_PROP_ID_UNSUPPORTED;
}


STDMETHODIMP CVRPresentOvlProcessorSW ::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
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
			m_rDst = *reinterpret_cast<RECT *>(pPropData);
			if(m_streamCount && m_pPresent)
				hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,&m_rDst,0);
			else
				hr = S_OK;
		}
		break;
		case MC_PROPID_VP_ZOOM:
		{
			m_zoom_mode = *reinterpret_cast<long *>(pPropData);
			if(m_streamCount && m_pPresent)
				hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,&m_zoom_mode,0);
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
			DP("=====PROCSW set MC_PROPID_VP_ANDROID_ISURFACE :%d \n", pPropData);
			if(pPropData)
			{
				m_android_param = *reinterpret_cast<VR_ANDROID_ISUR_PARAM*>(pPropData);
				if(m_streamCount && m_pPresent)
				{
					hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData);
				}
				else
				{
                DP("m_pPresent = NULL");
				}
			}
		}
		break;

		case MC_PROPID_VP_ANDROID_SURFACE:
		{
			DP("=====PROC set MC_PROPID_VP_ANDROID_SURFACE :%d \n", pPropData);
	#ifdef ANDROID_VR_JNI
			if(pPropData)
			{

				m_android_surfparam = *reinterpret_cast<VR_ANDROID_SURF_PARAM*>(pPropData);
				if(m_streamCount && m_pPresent)
				{
					hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData);
				}
			}
    #endif
		}
		break;
		case MC_PROPID_VP_FLIPVIDEO:
		{
			LONG lMode = *(LONG *)pPropData;
			if((lMode != VP_FLIPH) && (lMode != VP_FLIPV) && (lMode != VP_FLIPVH_NO) )
				return E_INVALIDARG;
				
			m_lFlipVideoMode = lMode;
		}
		break;
		default:
		{
			if(m_streamCount && m_pPresent)
				hr =  m_pPresent->Set(EMC_PROPSET_GFXCTXT,dwPropID,0,0,pPropData,cbPropData);
			else
				hr = S_OK;
		}
		break;
	}
	return hr;
}


STDMETHODIMP CVRPresentOvlProcessorSW ::CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream)
{
	HRESULT hr = S_OK;
	if(!pOpenParams || !ppStream )
		return E_INVALIDARG;

	if(!m_Init)	return VR_FAIL_UNOPEN;

#if defined(MT3353) || defined(PRIMAII)
	if(m_streamCount == 1)
		return E_FAIL;
#endif

//Init a free stream	                        
	UINT i;
	for(i=0;i<MAX_STREAM ;i++)
	{
		if(!m_pStmArrar[i].valid)	break;
	}

	if(i>= MAX_STREAM)	return E_UNEXPECTED;
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
			if(!m_pStmArrar[i].pBuf ) return E_FAIL;
			memset(m_pStmArrar[i].pBuf, 0x00,pOpenParams->dwWidth*pOpenParams->dwHeight);
			memset(m_pStmArrar[i].pBuf+pOpenParams->dwWidth*pOpenParams->dwHeight, 0x80,pOpenParams->dwWidth*pOpenParams->dwHeight/2);
			break;
		case MAKE_FOURCC('R','5','6','5'):
			m_pStmArrar[i].pBuf = (LPBYTE)malloc(pOpenParams->dwWidth*pOpenParams->dwHeight*2);
			if(!m_pStmArrar[i].pBuf ) return E_FAIL;
			memset(m_pStmArrar[i].pBuf, 0x00,pOpenParams->dwWidth*pOpenParams->dwHeight*2);
			break;
		case MAKE_FOURCC('P', 'B', 'S', '1'):
			break;
		default:
			DP("Invalid fourcc !!!\n");
			return E_FAIL;
	}


	CProtect proc(&m_csDisplay);
	m_streamCount++;

	m_pStmArrar[i].pStream = new CVRPresenterStream(m_pStmArrar[i].pBuf, &m_csDisplay, &m_pStmArrar[i].param, &m_pPresent);
	if(!m_pStmArrar[i].pStream)	goto RET;

	m_pStmArrar[i].valid = true;

//Return them

	hr = CreateV1VR(&m_openParams);
	if(FAILED(hr))
	{
		m_streamCount--;
		goto RET;
	}

	if(ppStream)
		*ppStream = static_cast<IMcVideoPresenterStream*>(m_pStmArrar[i].pStream);

	return hr;
	
RET:

	m_pStmArrar[i].pStream->Release();

	if(m_pStmArrar[i].pBuf)
	{
		free(m_pStmArrar[i].pBuf);
		m_pStmArrar[i].pBuf = NULL;
	}
	ZeroMemory(&m_pStmArrar[i], sizeof(SSTREAM_INFO));
	
	return E_FAIL;
}


STDMETHODIMP CVRPresentOvlProcessorSW ::DestroyStream(IN IMcVideoPresenterStream *ppStream)
{
	if(!ppStream)	return E_FAIL;
	for(int i=0;i<MAX_STREAM;i++)
	{
		if(m_pStmArrar[i].pStream == ppStream)
		{
			CProtect proc(&m_csDisplay);
			m_pStmArrar[i].pStream->Release();
			if(m_pStmArrar[i].pBuf)
				free(m_pStmArrar[i].pBuf);
			ZeroMemory(&m_pStmArrar[i], sizeof(SSTREAM_INFO));
			m_streamCount--;

			if(m_streamCount == 1)
			{
				bool boneSurf = true;
				for(unsigned int j=0;j<MAX_STREAM;j++)
				{
					if(m_pStmArrar[j].pStream)
						m_pStmArrar[j].pStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ONESURF_MODE,0,0,&boneSurf ,0);
				}
			}

			return S_OK;
		}
	}
	return E_NOTIMPL;
}

STDMETHODIMP CVRPresentOvlProcessorSW::ColorConvertion_Resize(void* pInput,
	void* pOutput,DWORD input_width, DWORD input_height, DWORD input_format,
	DWORD output_width, DWORD output_height, DWORD output_format, 
	RECT* pSrcRect, RECT* pDstRect, bool forTranspRent, LONG flipMode)
{
	if((pInput == NULL)||(pOutput == NULL))
		return E_FAIL;
	
	CI_IMAGEPROC_SIZE sSrcSize,sDstSize;
	memset(&sSrcSize, 0,sizeof(sSrcSize));
	memset(&sDstSize, 0,sizeof(sDstSize));

	
	sSrcSize.s32Width = pSrcRect->right - pSrcRect->left;// input_width;
	sSrcSize.s32Height = pSrcRect->bottom - pSrcRect->top;//input_height;

	sDstSize.s32Width = pDstRect->right - pDstRect->left;
	sDstSize.s32Height = pDstRect->bottom - pDstRect->top;

	if((input_format == MAKE_FOURCC('R','5','6','5'))&&(output_format == MAKE_FOURCC('Y','V','1','2')))
	{
		unsigned char *pTmpYV12Buf = (unsigned char *)malloc(input_width*input_height*3/2);
		if(pTmpYV12Buf == 0)
			return E_UNEXPECTED;

		LONG lStride = input_width;

		unsigned char *ppbPlanes[3];
		ppbPlanes[0] = (unsigned char *)pInput;
		ppbPlanes[1] = (unsigned char *)pInput;
		ppbPlanes[2] = (unsigned char *)pInput;

		DWORD pdwStrides[3] = {input_width*2,input_width*2,input_width*2};
		DWORD srcWidth[3];
		DWORD srcHeight[3];
		srcWidth[0] =srcWidth[1] = srcWidth[2] = input_width;
		srcHeight[0] = srcHeight[1] = srcHeight[2] = input_height;

		HRESULT hr = m_PrepareData.PrepareBufferRGB5652YV12(pTmpYV12Buf, lStride, ppbPlanes, pdwStrides,
				srcWidth, srcHeight, 0,CI_IMAGEPROC_ROTATION_DISABLE,VP_DEINTERLACE_Linear,flipMode);
		if(FAILED(hr))
			DP("PrepareBufferRGB5652YV12 FAILED \n");

		int srcStride[3]; 
		srcStride[0] = (int)input_width;
		srcStride[1] = (int)input_width>>1;
		srcStride[2] = (int)input_width>>1;


		unsigned char *SrcBuf[3];
		SrcBuf[0] = (unsigned char *)pTmpYV12Buf  +pSrcRect->top*input_width + pSrcRect->left;
		SrcBuf[1] = (unsigned char *)pTmpYV12Buf + input_width*input_height + pSrcRect->top/2*input_width/2 + pSrcRect->left/2;
		SrcBuf[2] = (unsigned char *)pTmpYV12Buf + input_width*input_height*5/4 + pSrcRect->top/2*input_width/2 + pSrcRect->left/2;

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

		hr = m_PrepareData.PrepareBufferYV12Scaled(SrcBuf,srcStride, sSrcSize, DstBuf, 
			dstStride, sDstSize, CI_IMAGEPROC_ROTATION_DISABLE,flipMode);
		if(FAILED(hr))
			DP("PrepareBufferYV12Scaled FAILED \n");
	
		free(pTmpYV12Buf);

		return S_OK;
	}
			
	if((input_format == MAKE_FOURCC('Y','V','1','2'))&&(output_format == MAKE_FOURCC('Y','V','1','2')))
	{
		int srcStride[3]; 
		srcStride[0] = (int)input_width;
		srcStride[1] = (int)input_width>>1;
		srcStride[2] = (int)input_width>>1;


		unsigned char *SrcBuf[3];
		SrcBuf[0] = (unsigned char *)pInput  +pSrcRect->top*input_width + pSrcRect->left;
		SrcBuf[1] = (unsigned char *)pInput + input_width*input_height + pSrcRect->top/2*input_width/2 + pSrcRect->left/2;
		SrcBuf[2] = (unsigned char *)pInput + input_width*input_height*5/4 + pSrcRect->top/2*input_width/2 + pSrcRect->left/2;

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

	return E_FAIL;
}

STDMETHODIMP CVRPresentOvlProcessorSW::TransparentOverwrite(void* pBackground,
void* pSrc,DWORD dwSrcTransColor, DWORD dwBackWidth, DWORD dwBackHeight,
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

#if 1
//	if(alpha == 1.0f)
	{
		DWORD offsetY = pSrcOffset->top*dwBackWidth + pSrcOffset->left;
		DWORD offsetUV = pSrcOffset->top/2*dwBackWidth/2 + pSrcOffset->left/2;

		unsigned char y = 0,u = 0,v = 0;
		y =(unsigned char)((dwSrcTransColor&0xff0000)>>16);
		u =(unsigned char)((dwSrcTransColor&0xff00)>>8);
		v =(unsigned char)dwSrcTransColor&0xff;

		for(int i=0;i<dwSrcHeight;i++)
		{
			for(int j=0;j<dwSrcWidth;j++)
			{
				if(!((src_buf[0][i*dwSrcWidth+j] == y)&&
					(src_buf[1][i/2*dwSrcWidth/2+j/2] == u)&&
					(src_buf[2][i/2*dwSrcWidth/2+j/2] == v)))
				{
					if(alpha <0.001f)
					{
						background_buf[0][i*dwBackWidth + offsetY+j]= src_buf[0][i*dwSrcWidth+j];
						background_buf[1][i/2*dwBackWidth/2 + offsetUV+j/2]= src_buf[1][i/2*dwSrcWidth/2+j/2];
						background_buf[2][i/2*dwBackWidth/2 + offsetUV+j/2]= src_buf[2][i/2*dwSrcWidth/2+j/2];
					}
					else
					{
						background_buf[0][i*dwBackWidth + offsetY+j]= 
							background_buf[0][i*dwBackWidth + offsetY+j]*alpha + 
							src_buf[0][i*dwSrcWidth+j]*(1.0f -alpha);
						background_buf[1][i/2*dwBackWidth/2 + offsetUV+j/2]= 
							background_buf[1][i/2*dwBackWidth/2 + offsetUV+j/2]*alpha + 
							src_buf[1][i/2*dwSrcWidth/2+j/2]*(1.0f -alpha);
						background_buf[2][i/2*dwBackWidth/2 + offsetUV+j/2]= 
						background_buf[2][i/2*dwBackWidth/2 + offsetUV+j/2]*alpha+
							src_buf[2][i/2*dwSrcWidth/2+j/2]*(1.0f -alpha);
					}
				}
			}
		}

/*
		for(DWORD i=0;i<dwSrcHeight;i++)
			memcpy(background_buf[0] + dwBackWidth*i + offsetY ,src_buf[0] + dwSrcWidth*i,  dwSrcWidth );

		for(DWORD i=0;i<dwSrcHeight/2;i++)
			memcpy(background_buf[1] + dwBackWidth/2*i + offsetUV,src_buf[1] + dwSrcWidth/2*i,  dwSrcWidth/2 );

		for(DWORD i=0;i<dwSrcHeight/2;i++)
			memcpy(background_buf[2] + dwBackWidth/2*i + offsetUV,src_buf[2] + dwSrcWidth/2*i,  dwSrcWidth/2 );
*/
	}
#else
	{
		m_pAlphaBlend->SetAlphaBlendingYUV(dwBackWidth, dwBackHeight,
			dwSrcWidth, dwSrcHeight, pSrcOffset->left,pSrcOffset->top,0,0,0,alpha);

		if(m_canvasFourCC == MAKE_FOURCC('Y','V','1','2'))
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
#endif
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorSW::GetFourCCLength(DWORD fourCC, DWORD *plength)
{
	if(plength == NULL)
		return E_FAIL;
	
	switch(fourCC)
	{
		case MAKE_FOURCC('Y','V','1','2'):
		case MAKE_FOURCC('N','V','1','2'):
		case MAKE_FOURCC('Y','U','Y','2'):
			*plength = 1;
			break;
		case MAKE_FOURCC('R','5','6','5'):
			*plength = 2;
			break;
		default:
			break;
	}
	
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorSW ::CreateCanvas()
{
	if(m_canvasBuf)
	{
		free(m_canvasBuf);
		m_canvasBuf = NULL;
	}

	if(m_canvasResizeBuf)
	{
		free(m_canvasResizeBuf);
		m_canvasResizeBuf  = NULL;
	}
	
	switch(m_canvasFourCC)
	{
		case MAKE_FOURCC('Y','V','1','2'):
			m_canvasBuf = (LPBYTE)malloc(m_cropwidth *m_cropheight*3/2);
			if(!m_canvasBuf )	return E_FAIL;
			memset(m_canvasBuf, 0x0 ,m_cropwidth *m_cropheight);
			memset(m_canvasBuf + m_cropwidth *m_cropheight, 0x80 ,m_cropwidth *m_cropheight/2);

			m_canvasResizeBuf = (LPBYTE)malloc(m_cropwidth *m_cropheight*3/2);
			if(!m_canvasResizeBuf )	return E_FAIL;
			break;
		case MAKE_FOURCC('R','5','6','5'):
			m_canvasBuf = (LPBYTE)malloc(m_cropwidth *m_cropheight*2);
			if(!m_canvasBuf )	return E_FAIL;
			memset(m_canvasBuf, 0 ,m_cropwidth *m_cropheight*2);			

			m_canvasResizeBuf = (LPBYTE)malloc(m_cropwidth *m_cropheight*2);
			if(!m_canvasResizeBuf )	return E_FAIL;
			break;
		default:
			return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CVRPresentOvlProcessorSW ::CreateV1VR(struct VP_OpenOptionsAdv* pOpenParm)
{

	//Save two times memory to optimize the renderer performance.
	bool boneSurf = false; 

#if 1//def _WIN32_WCE
	if(m_streamCount == 1)
	{
		m_pPresent->Close();
		m_VRopened = false;
		UINT i;
		for(i=0;i<MAX_STREAM ;i++)
		{
			if(m_pStmArrar[i].valid)	break;
		}

		boneSurf = true;
		m_pStmArrar[i].pStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ONESURF_MODE,0,0,&boneSurf ,0);

		VP_OpenOptions sInit;
		ZeroMemory(&sInit, sizeof(sInit));
		sInit.dwWidth = m_pStmArrar[i].param.dwWidth;
		sInit.dwHeight= m_pStmArrar[i].param.dwHeight;
		sInit.dwAspectX = m_pStmArrar[i].param.dwAspectX;
		sInit.dwAspectY = m_pStmArrar[i].param.dwAspectY;
		sInit.dwFourCC = m_pStmArrar[i].param.dwFourCC;

 		if(pOpenParm->dwSurfType == 0)
		{
#if defined(VM_OPENGL)
			sInit.dwSurfType = VP_OPEN_FLAG_OPENGL;
#elif defined(GFX_D3D)
			sInit.dwSurfType = VP_OPEN_FLAG_D3D | VP_OPEN_FLAG_GDI;
#endif		
		}
	      else
		      sInit.dwSurfType = pOpenParm->dwSurfType ;

		sInit.hWnd = pOpenParm->hWnd;
		HRESULT hr = m_pPresent->Open(&sInit, sizeof(sInit));
		if(FAILED(hr))	
		{
			DP("m_pPresent->Open failed \n");
			return E_FAIL;
		}

		Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&m_rDst,0);
		Set(IID_IMcVideoPresenter,MC_PROPID_VP_ZOOM,0,0,&m_zoom_mode,0);	
		m_VRopened = true;
		m_openParamsVR1 = *pOpenParm;

	}
	else
#endif
	{
		bool bEnable = true;
		if(m_pStmArrar[0].pStream)
			m_pStmArrar[0].pStream->Get(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&bEnable ,0,0);

		boneSurf = false;
		for(unsigned int j=0;j<MAX_STREAM;j++)
		{
			if(m_pStmArrar[j].pStream)
				m_pStmArrar[j].pStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ONESURF_MODE,0,0,&boneSurf ,0);
		}


		if((m_VRopened)&& (pOpenParm->canvasWidth == m_openParamsVR1.canvasWidth)&&
			(pOpenParm->canvasHeight == m_openParamsVR1.canvasHeight))
			return S_OK;

		
		m_pPresent->Close();
		m_VRopened = false;
		
		VP_OpenOptions sInit;
		ZeroMemory(&sInit, sizeof(sInit));

#if defined(GFX_ANDROID_SURFACE) || defined(GFX_ANDROID_ANativeWindow)
		sInit.dwWidth = m_pStmArrar[0].param.dwWidth;
		sInit.dwHeight = m_pStmArrar[0].param.dwHeight;
#else

		sInit.dwWidth = m_cropwidth;
		sInit.dwHeight= m_cropheight;
#endif
		sInit.dwFourCC = m_canvasFourCC;

		if(pOpenParm->dwSurfType == 0)
		{
#if defined(VM_OPENGL)
		sInit.dwSurfType = VP_OPEN_FLAG_OPENGL;
#elif defined(GFX_D3D)
		sInit.dwSurfType = VP_OPEN_FLAG_D3D | VP_OPEN_FLAG_GDI;
#endif		
		}
      else
	      sInit.dwSurfType = pOpenParm->dwSurfType ;

		sInit.hWnd = pOpenParm->hWnd;
		HRESULT hr = m_pPresent->Open(&sInit, sizeof(sInit));
		if(FAILED(hr))	
		{
			DP("m_pPresent->Open failed2 \n");
			return E_FAIL;
		}

		m_VRopened = true;
		m_openParamsVR1 = *pOpenParm;
		Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&m_rDst,0);
		Set(IID_IMcVideoPresenter,MC_PROPID_VP_ZOOM,0,0,&m_zoom_mode,0);	
		Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&bEnable,0);	
	}

	Set(IID_IMcVideoPresenter,MC_PROPID_VP_STREAM_COUNT,0,0,&m_streamCount,0);	

#ifdef GFX_ANDROID_OPENGL_ES_SHADER
    DP("GFX_ANDROID_OPENGL_ES_SHADER createV1VR MC_PROPID_VP_ANDROID_ISURFACE");
		if(m_pPresent)
			m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ANDROID_ISURFACE,0,0,&m_android_param,0);
#else
  if(m_android_param.pAndroidSFBuf)
	{
    DP("GFXPRESENTPROCSW createV1VR MC_PROPID_VP_ANDROID_ISURFACE");
		if(m_pPresent)
			m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ANDROID_ISURFACE,0,0,&m_android_param,0);
	}
#endif

	#ifdef ANDROID_VR_JNI
	if(m_android_surfparam.m_pJvm)
	{
		if(m_pPresent)
			m_pPresent->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ANDROID_SURFACE,0,0,&m_android_surfparam,0);
	}
    #endif
	return S_OK;
}


STDMETHODIMP CVRPresentOvlProcessorSW ::AdjustCanvasSize(unsigned int w, unsigned int h,
	float* pAspwnd)
{
  		//if(w != m_openParams.canvasWidth || h != m_openParams.canvasHeight)
  		//{
  		//	m_openParams.canvasWidth = w;
  		//	m_openParams.canvasHeight = h;
  		//	hr = CreateCanvas();
  		//	if(FAILED(hr))	DP("ERROR!! CreateCanvas FAILED\n");
  		//	hr = CreateV1VR();
  		//	if(FAILED(hr))	DP("ERROR!! CreateV1VR FAILED\n");

  		//	SetRect(&m_rWnd,0,0,m_openParams.canvasWidth ,  m_openParams.canvasHeight);
  		//	for(int i=0;i<MAX_STREAM ;i++)
  		//	{
  		//		if(m_pStmArrar[i].valid)
  		//			m_pStmArrar[i].pStream->Set(IID_IMcVideoPresenterAdv,MC_PROPID_VP_RECTDST,0,0,&m_rWnd,0);
  		//	}

  		//	canvasChanged = true;
  		//}
		if( h == 0 || m_cropheight ==0)
			return E_INVALIDARG;
  		float aspwnd = ((float)w/(float)h);
  		float aspSrc = ((float)m_cropwidth/(float)m_cropheight);
  		float aspAbs = aspwnd- aspSrc;
  		if(aspAbs >0.01 ||aspAbs <-0.01)
  		{
			w &= ~(2-1);
			h &= ~(2-1);

			DP("CanvasSize changed (%d,%d)to(%d,%d)\n ",m_cropwidth,m_cropheight,w,h);
			m_cropwidth = w;
			m_cropheight = h;
			HRESULT hr = CreateCanvas();
			if(FAILED(hr))	DP("ERROR!! CreateCanvas FAILED\n");

			struct VP_OpenOptionsAdv params;
			params = m_openParams;
			params.canvasWidth = w;
			params.canvasHeight= h;
			hr = CreateV1VR(&params);
			if(FAILED(hr))	
			DP("ERROR!! CreateV1VR FAILED\n");

  			m_bcanChanged = true;
  		}
  if(pAspwnd )
    *pAspwnd = aspwnd;
  return S_OK;
}


int CVRPresentOvlProcessorSW::GetStreamNumber()
{
	int streamNum = 0;
	for(int i=0;i<MAX_STREAM ;i++)
	{
		if(m_pStmArrar[i].valid)	
			streamNum++;
	}
	return streamNum;
}

