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

#ifdef VM_SUPPORT
#include "../../inc/GfxBuildDef.h"

#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresentPrepare.h"
#include "../../inc/GfxVideoWindow.h"

#ifdef ANDROID
#include "VideomixerInterface.h"
#else
#include "../../src/LibVideoMixer/VideomixerInterface.h"
#endif

#ifndef WINCE
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "tchar.h"

#include "../../3rdparty/WIN32/D3DMixer/D3DMixer.h"
#pragma comment(lib, "../../3rdparty/WIN32/D3DMixer/D3DMixer.lib")
#endif

#include "../../inc/VDecRnd.h"


#include "GfxPresentDef.h"

#ifdef  USE_IPP_ROTATION
#include "ipp.h"

inline int WidthRect(const RECT *r)
{
	return r->right - r->left;
}

inline int HeightRect(const RECT *r)
{
	return r->bottom - r->top;
}

#endif

#define DEFAULT_INPUT_FOURCC	MAKEFOURCC('Y','V','1','2')
const DWORD CVRPresent::const_InputFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('Y','V','1','6'),
	MAKEFOURCC('N','V','1','2'),
	MAKEFOURCC('P','B','S','1'),
	MAKEFOURCC('R','5','6','5'),
	MAKEFOURCC('Y','U','Y','2'),
	MAKEFOURCC('X','R','G','B'),
	0
};

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter")
#define HARDWARE_VIDEO_EFFECT_INIT_VALUE	128
#define SOFTWARE_VIDEO_EFFECT_INIT_VALUE	128

CVRPresent::CVRPresent() :
	m_lRotationMode(VP_ROTATION_0),
	m_lDeblockMode(0),
	m_lResampleMode(0),
	m_dwStatus(NULL),
	m_dwPresentIndex(0),
	m_overlayType(0),
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_bOpen(FALSE),
	m_bGoMixer(FALSE),
	pVideoMemoryInfo(NULL),
	m_bSoftScale(0),
 	m_dwCkeySrc(0x00008080)
{
	ZeroMemory(&m_rSrcSize,sizeof(m_rSrcSize));
	ZeroMemory(&m_rDst,sizeof(m_rDst));
	ZeroMemory(&m_OpenOptions,sizeof(m_OpenOptions));
	ZeroMemory(&m_videoEffect, sizeof(m_videoEffect));
 	ZeroMemory(&m_surfHandle, sizeof(m_surfHandle));
	ZeroMemory(&m_bufferMemoryInfo, sizeof(m_bufferMemoryInfo));
	ZeroMemory(&m_SoftOpenOptions, sizeof(m_SoftOpenOptions));
	ZeroMemory(&rVideoWnd, sizeof(rVideoWnd));
	ZeroMemory(&m_absoluteRectBak, sizeof(m_absoluteRectBak));
	InitializeCriticalSection(&m_csDisplay);

 	m_FPS = 0;
	m_vsyncMode = VP_VSYNC_MODE_ASYNC;
	m_StreamCount = 0;
	m_dstSurfFourCC = 0;
	memset(&m_ContextArray,0,sizeof(m_ContextArray));
}

CVRPresent::~CVRPresent()
{
	Close();
	
	if(pVideoMemoryInfo)
	{
		delete pVideoMemoryInfo;
		pVideoMemoryInfo = NULL;
	}
	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CVRPresent::Open(VP_OpenOptions *pOptions, DWORD dwOptionSize)
{
	HRESULT res = S_OK;
	DWORD i =0;
	if((pOptions==0)||(sizeof(m_OpenOptions)!=dwOptionSize))
		return E_INVALIDARG;

	if(m_bOpen)
	{
		DP("[GFXPRESENTOVL] Invalid call. Video Presenter is opened already \n");
		return E_FAIL;
	}

	if(pOptions->dwWidth == 0 || pOptions->dwHeight == 0)
		return E_INVALIDARG;


	if(pOptions->dwHeight%2)
	{
		return E_INVALIDARG;
	}

	CProtect proc(&m_csDisplay);

	m_StreamCount = 0;
	ZeroMemory(&m_rDst,sizeof(m_rDst));
	ZeroMemory(&rVideoWnd, sizeof(rVideoWnd));
	ZeroMemory(&m_absoluteRectBak, sizeof(m_absoluteRectBak));
	m_bSoftScale = FALSE;

	m_Deinterlaced_Mode = VP_DEINTERLACE_WEAVE;
	m_FPS = 0;
	m_vsyncMode = VP_VSYNC_MODE_ASYNC;
	m_dwCkeySrc = 0x00008080;
	m_dstSurfFourCC = 0;
	memset(&m_ContextArray,0,sizeof(m_ContextArray));

	ZeroMemory(&m_SoftOpenOptions, sizeof(m_SoftOpenOptions));

	if(pOptions != &m_OpenOptions)
		memcpy(&m_OpenOptions, pOptions, sizeof(m_OpenOptions));

/*********************FourCC Check**************************************/
//Check whether input fourCC is supportted 
	if(m_OpenOptions.dwFourCC == 0)
		m_OpenOptions.dwFourCC = DEFAULT_INPUT_FOURCC;
	
	for(i=0;const_InputFourCC[i] != 0;i++)
	{
		if(const_InputFourCC[i] == m_OpenOptions.dwFourCC)
			break;
	}

	//Invalid input fourCC
	if(const_InputFourCC[i] == 0)
	{
		return E_FAIL;
	}

	surfaceFourCC = const_InputFourCC[i];

/*********************FourCC Check End**************************************/

	
	m_dwPresentIndex = 0;

	SetRect(&m_rSrcSize, 0, 0, m_OpenOptions.dwWidth, m_OpenOptions.dwHeight);

/*********************Init VideoEffect**************************************/
	m_videoEffect.s32Brightness = SOFTWARE_VIDEO_EFFECT_INIT_VALUE;
	m_videoEffect.s32Contrast = SOFTWARE_VIDEO_EFFECT_INIT_VALUE;
	m_videoEffect.s32Saturation = SOFTWARE_VIDEO_EFFECT_INIT_VALUE;

/*********************Init VideoEffect End**************************************/
 	m_bOpen = TRUE;
	m_bGoMixer = FALSE;

	return S_OK;
}

STDMETHODIMP CVRPresent::Close()
{
	if(!m_bOpen)
		return S_OK;

	CProtect proc(&m_csDisplay);

	for(int i=VP_SURF_LAYERTOP;i<=VP_SURF_LAYERBOTTOM;i++)
		DestroySurface((VR_SURFACE_TYPE)i);

	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CVRPresent::Reset()
{
	HRESULT res = S_OK;
	res = Close();
	if(FAILED(res))
		return E_FAIL;
	
	res = Open(&m_OpenOptions,sizeof(m_OpenOptions));
	if(FAILED(res))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CVRPresent::SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk)
{
	if(riid==IID_ICivmVideoContext)
	{
		CProtect proc(&m_csDisplay);

		ICivmVideoContext* pVideoContext = (ICivmVideoContext*)pUnk;
		if(dwRoute == 1)
		{
			int count =0;
			for(count =0;m_ContextArray[count] && count < sizeof(m_ContextArray)/sizeof(ICivmVideoContext*);count++);
			if(count >= sizeof(m_ContextArray)/sizeof(ICivmVideoContext*))	return E_FAIL;
			
			m_ContextArray[count] = pVideoContext;
			AddRef();
		}
		else
		{
			int count =0;
			for(count =0;m_ContextArray[count] && count < sizeof(m_ContextArray)/sizeof(ICivmVideoContext*);count++)
			{
				if(m_ContextArray[count] == pVideoContext)
				{
					m_ContextArray[count] = NULL;
					Release();
					break;
				}
			}
		}

		return S_OK;
	}
	
	return E_NOTIMPL;
}

STDMETHODIMP CVRPresent::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{

	switch (dwPropID)
	{
	case MC_PROPID_VP_VSYNC:
		return ReturnComLong(m_vsyncMode ,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ROTATION:
		return ReturnComLong(m_lRotationMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_DEBLOCK:
		return ReturnComLong(m_lDeblockMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_RESAMPLE:
		return ReturnComLong(m_lResampleMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_TYPE:
		return ReturnComLong(m_overlayType,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_DEINTERLACE_MODE:
		{
			return E_NOTIMPL;
		}
	case MC_PROPID_VP_RECTDST:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rDst;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_RECTSRC:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rSrcSize;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
 	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			if(pPropData==0)
				return E_POINTER;

			//Software video effect
			if((dwPropID == MC_PROPID_VP_BRIGHTNESS))
			{
				*reinterpret_cast<LONG *>(pPropData) = m_videoEffect.s32Brightness;
				if(pcbReturned)
					*pcbReturned = sizeof(LONG);
				return S_OK;
			}
			if((dwPropID == MC_PROPID_VP_CONTRAST))
			{
				*reinterpret_cast<LONG *>(pPropData) = m_videoEffect.s32Contrast;
				if(pcbReturned)
					*pcbReturned = sizeof(LONG);
				return S_OK;
			}
			if((dwPropID == MC_PROPID_VP_SATURATION))
			{
				*reinterpret_cast<LONG *>(pPropData) = m_videoEffect.s32Saturation;
				if(pcbReturned)
					*pcbReturned = sizeof(LONG);
				return S_OK;
			}


			int index = dwPropID - MC_PROPID_VP_BRIGHTNESS;

			CProtect proc(&m_csDisplay);
			*reinterpret_cast<LONG *>(pPropData) = m_alCC[index];
			if(pcbReturned)
				*pcbReturned = sizeof(LONG);
			return S_OK;
		}
	case MC_PROPID_VP_WND:
		{
			return E_NOTIMPL;
		}
	case MC_PROPID_VP_OPENOPTIONS:
		{
			if(pPropData==0)
				return E_POINTER;

			*reinterpret_cast<VP_OpenOptions*>(pPropData) = m_OpenOptions;
			return S_OK;
		}
	}
	return E_PROP_ID_UNSUPPORTED;
}

STDMETHODIMP CVRPresent::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	if(!m_bOpen)
		return E_FAIL;

	switch (dwPropID)
	{
	case MC_PROPID_VP_ROTATION:
		{
			LONG lRotationMode = *(LONG *)pPropData;
			CProtect proc(&m_csDisplay);
			if(m_lRotationMode!=lRotationMode)
			{
				m_lRotationMode = lRotationMode;
			}
			return S_OK;
		}
		
#ifndef NO_ROT_SCL
	case MC_PROPID_VP_RECTSRC:
		{
			RECT *pr = reinterpret_cast<RECT *>(pPropData);
			CProtect proc(&m_csDisplay);
			if(!EqualRect(&m_rSrcSize,pr))
			{
				m_rSrcSize = *pr;
			}
			return S_OK;
		}
#endif
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			LONG lValue= *(LONG *)pPropData;
			int index = dwPropID - MC_PROPID_VP_BRIGHTNESS;

			if(lValue > MAX_COLORCONTROL_VAL)
				lValue = MAX_COLORCONTROL_VAL;
			if(lValue < MIN_COLORCONTROL_VAL)
				lValue = MIN_COLORCONTROL_VAL;

			if(((dwPropID == MC_PROPID_VP_BRIGHTNESS))
			||((dwPropID == MC_PROPID_VP_CONTRAST))
			||((dwPropID == MC_PROPID_VP_SATURATION)))
			//It's not supported
			{
				if(dwPropID == MC_PROPID_VP_BRIGHTNESS)	//Do software conversion. Notice: the performance comes down.
				{
					m_videoEffect.s32Brightness = lValue;
				}
				else if(dwPropID == MC_PROPID_VP_CONTRAST)
				{
					m_videoEffect.s32Contrast = lValue;
				}
				else if(dwPropID == MC_PROPID_VP_SATURATION)
				{
					m_videoEffect.s32Saturation = lValue;
				}
				m_PrepareData.Set(MC_PROPID_GFXPREPARE_VIDEOEFFECT,&m_videoEffect,sizeof(m_videoEffect));
				return S_OK;			

			}

			return S_OK;
		}

	case MC_PROPID_VP_RESAMPLE:
		{
			LONG lResampleMode = *(LONG *)pPropData;

			CProtect proc(&m_csDisplay);
			if(m_lResampleMode!=lResampleMode)
			{
				m_lResampleMode = lResampleMode;
			}
			return S_OK;
		}
	case MC_PROPID_VP_DEBLOCK:
		{
			LONG lDeblockMode = *(LONG *)pPropData;

			m_lDeblockMode = lDeblockMode;
			return S_OK;
		}
		
	case MC_PROPID_VP_CLEAR:
		{
			if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
			{
				//Malloc Buffer & Feed black screen data				
				LPBYTE pbuf = NULL;
				pbuf = (LPBYTE)malloc(m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*3/2);
				if(pbuf == NULL)
					return E_FAIL;
				unsigned char y = (unsigned char)(m_dwCkeySrc>>16 & 0x000000ff);
				unsigned char u = (unsigned char)(m_dwCkeySrc>>8 & 0x000000ff);
				unsigned char v = (unsigned char)(m_dwCkeySrc & 0x000000ff);
				
				memset(pbuf, y ,m_OpenOptions.dwWidth*m_OpenOptions.dwHeight);
				memset(pbuf+m_OpenOptions.dwWidth*m_OpenOptions.dwHeight, u ,m_OpenOptions.dwWidth*m_OpenOptions.dwHeight/4);
				memset(pbuf+m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*5/4, v ,m_OpenOptions.dwWidth*m_OpenOptions.dwHeight/4);

				//Present
				DWORD strides[3];
				strides[0]= m_OpenOptions.dwWidth; 
				strides[1] = strides[2] = m_OpenOptions.dwWidth/2;
				LPBYTE bufStrides[3];
				bufStrides[0] = pbuf; 
				bufStrides[1] = pbuf+m_OpenOptions.dwWidth*m_OpenOptions.dwHeight;
				bufStrides[2] = pbuf+m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*5/4;
				Present(bufStrides, strides,NULL);
				//FreeBuffer
				delete pbuf;
				return S_OK;
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
			{
				//Malloc Buffer & Feed black screen data				
				LPBYTE pbuf = NULL;
				pbuf = (LPBYTE)malloc(m_OpenOptions.dwWidth*m_OpenOptions.dwHeight);
				if(pbuf == NULL)
					return E_FAIL;
				memset(pbuf, 0xff,m_OpenOptions.dwWidth*m_OpenOptions.dwHeight);


				//Present
				DWORD strides[3];
				strides[0] = strides[1] = strides[2] = m_OpenOptions.dwWidth; 
				LPBYTE bufStrides[3];
				bufStrides[0] = bufStrides[1] = bufStrides[2] = pbuf; 
				Present(bufStrides, strides,NULL);

				//FreeBuffer
				delete pbuf;
				return S_OK;
			}
			else
				return E_NOTIMPL;
		}
		case MC_PROPID_VP_DEINTERLACE_MODE:
		{
			m_Deinterlaced_Mode = *((VP_DEINTERLACE_MODE* )pPropData);
			return S_OK;
		}
		default:
			return E_NOTIMPL;
	}
	return E_PROP_ID_UNSUPPORTED;
}

HRESULT CVRPresent::SetSrcSize(DWORD dwWidth, DWORD dwHeight)
{
	RECT r;
	SetRect(&r, 0, 0, dwWidth, dwHeight);
	if(EqualRect(&r,&m_rSrcSize))
		return S_OK;
	m_rSrcSize = r;
	return S_OK;
}


STDMETHODIMP CVRPresent::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	if((!(PRESENT_FLAG_NO_COPY_DATA & dwFlags))&&((ppbPlanes == NULL)||(pdwStrides==NULL)))
	{
		return E_FAIL;
	}

	#ifdef DUMP_FILE
	{
	        FILE* fp = fopen("1.yuv", "wb");
		for(int i =m_OpenOptions.dwHeight - 2;i<m_OpenOptions.dwHeight;i++)
		{
		    fwrite(ppbPlanes[0] + pdwStrides[0]*i, 1, m_OpenOptions.dwWidth, fp);
		}

		for(int i =(m_OpenOptions.dwHeight>>1) - 1;i<m_OpenOptions.dwHeight>>1;i++)
		{
		    fwrite(ppbPlanes[1] + pdwStrides[1]*i, 1, m_OpenOptions.dwWidth>>1, fp);
		}
		
		for(int i =(m_OpenOptions.dwHeight>>1 )- 1;i<m_OpenOptions.dwHeight>>1;i++)
		{
		    fwrite(ppbPlanes[2] + pdwStrides[2]*i, 1, m_OpenOptions.dwWidth>>1, fp);
		}

		fclose(fp);

	}


	#endif

	if(!(PRESENT_FLAG_NO_COPY_DATA & dwFlags))
	{
		if(FAILED(PrepareBuffer(m_dwPresentIndex,ppbPlanes,pdwStrides,dwFlags)))
		{
			//DP("[GFXPRESENTOVL]Present: preparebuf failed \n" );
			return E_FAIL;
		}
 	}
	return S_OK;
}

#ifdef USE_IPP_ROTATION
STDMETHODIMP CVRPresent::PrepareBufferIPP_YV12(LPBYTE frameBuffer, 	LONG lStride , LPBYTE *ppbPlanes,  DWORD *pdwStrides, DWORD* pdwBufStrides)
{

	IppCameraRotation rotation;
	IppCameraInterpolation interpolation;
	int rcpRatioX, rcpRatioY;
	const Ipp8u* pS[3];
	Ipp8u* pD[3];
	IppiSize srcSize, dstSize;
	RECT* pSrcSize =	&m_rSrcSize;
	pD[0] = frameBuffer;

#if  1//(_WIN32_WCE != 0x600)
	pD[2] = frameBuffer + pdwBufStrides[0];
	pD[1] = frameBuffer + pdwBufStrides[0]+ pdwBufStrides[1];
#else
	pD[1] = frameBuffer + pdwBufStrides[0];
	pD[2] = frameBuffer + pdwBufStrides[0]+ pdwBufStrides[1];
#endif	

	srcSize.width = WidthRect(pSrcSize);
	srcSize.height = HeightRect(pSrcSize);

	interpolation = ippCameraInterpBilinear;
	
	int srcStride[3]; 
	srcStride[0] = (int)pdwStrides[0];
	srcStride[1] = (int)pdwStrides[1];
	srcStride[2] = (int)pdwStrides[2];

	int dstStride[3]; 
	dstStride[0] = (int)lStride;
	dstStride[1] = (int)lStride>>1;
	dstStride[2] = (int)lStride>>1;

	
	pS[0] = ppbPlanes[0];
	pS[1] = ppbPlanes[1];
	pS[2] = ppbPlanes[2];

	if(m_lRotationMode==VP_ROTATION_0)
	{	// PORTRAIT
		rotation = ippCameraRotateDisable;
		dstSize.width = WidthRect(pSrcSize);
		dstSize.height = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
	}
	else if (m_lRotationMode==VP_ROTATION_90)
	{	// LANDSCAPE
		rotation = ippCameraRotate90L;
		dstSize.height = WidthRect(pSrcSize);
		dstSize.width = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
	}
	else if (m_lRotationMode==VP_ROTATION_180)
	{	// UPSIDE DOWN
		rotation = ippCameraRotate180;
		dstSize.width = WidthRect(pSrcSize);
		dstSize.height = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
	}
	else if (m_lRotationMode==VP_ROTATION_270)
	{	// LANDSCAPE, FLIP
		rotation = ippCameraRotate90R;
		dstSize.height = WidthRect(pSrcSize);
		dstSize.width = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
	}
	//DP("[VideoRender] Before IPP resize&rotate : srcWidth = %d, srcHeight = %d, srcStep = %d, dstWidth = %d, dstHeight = %d, dstStep = %d.\n", srcSize.width, srcSize.height, srcStep[0], dstSize.width, dstSize.height, dstStep[0]);
	ippiYCbCr420RszRot_8u_P3R(pS, srcStride, srcSize, pD,
			dstStride, dstSize, interpolation, rotation,
			rcpRatioX, rcpRatioY);
	return S_OK;
}
#endif

STDMETHODIMP CVRPresent::PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	LPBYTE pBuf;
	RECT rScaleSrc, rScaleDst;
	DWORD dwScaleFlags = 0;

	if(!m_bOpen)
		return E_FAIL;
	
	if(ppbPlanes[0]==0 || ppbPlanes[1]==0)
	{
		DP("[GFXPRESENTOVL]PresentBuffer: Invalid parameter == %d, %d ,%d \n",ppbPlanes[0], ppbPlanes[1], ppbPlanes[2]);
		return E_FAIL;
	}


	ZeroMemory(&rScaleDst, sizeof(rScaleDst));


	CProtect proc(&m_csDisplay);
	DWORD lStride = 0;

	int count =0;
	for(count =0;count < sizeof(m_ContextArray)/sizeof(ICivmVideoContext*);count++)
	{
		if(m_ContextArray[count])
		{
			m_ContextArray[count] -> Get(MC_PROPID_VP_FOURCC,&m_dstSurfFourCC,0);

			HRESULT hr = m_ContextArray[count]->Lock(&pBuf,&lStride );

			if(FAILED(hr))
			{
				continue;			
			}
			if(m_bSoftScale)
			{
				ZeroMemory(&rScaleSrc, sizeof(rScaleSrc));
				ZeroMemory(&rScaleDst, sizeof(rScaleDst));
				rScaleSrc.right = m_OpenOptions.dwWidth;
				rScaleSrc.bottom = m_OpenOptions.dwHeight;
				rScaleDst.right = m_SoftOpenOptions.dwWidth;
				rScaleDst.bottom = m_SoftOpenOptions.dwHeight;
				dwScaleFlags = m_bSoftScale;		
			}
			else
			{
				dwScaleFlags = 0;
			}

			DWORD srcWidth[3];
			DWORD srcHeight[3];
			memset(srcWidth, 0 ,sizeof(srcWidth));
			memset(srcHeight, 0 ,sizeof(srcHeight));
			srcWidth[0] = m_OpenOptions.dwWidth;

			if(m_dstSurfFourCC == MAKEFOURCC('N','V','1','2'))
				srcWidth[1] = srcWidth[2] = (m_OpenOptions.dwWidth);
			else
				srcWidth[1] = srcWidth[2] = (m_OpenOptions.dwWidth>>1);
			srcHeight[0] = m_OpenOptions.dwHeight;
			srcHeight[1] = srcHeight[2] = (m_OpenOptions.dwHeight>>1);
			

			HRESULT res = S_OK;
			if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
			{
				switch(m_dstSurfFourCC)
				{
				case MAKEFOURCC('Y','U','Y','2'):
				case MAKEFOURCC('Y','U','Y','V'):
					res = m_PrepareData.PrepareBufferYUY2(m_OpenOptions.dwFourCC,pBuf, lStride, ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode);
					if(FAILED(res))
						DP("!!!!!!!!Preapre failed err:0x%x \n",res);
					break;
				case MAKEFOURCC('Y','V','1','2'):
					{
						DWORD pdwBufStridesOrdered[3];
						pdwBufStridesOrdered[0] = lStride*srcHeight[0]; 
						pdwBufStridesOrdered[1] = (lStride>>1)*srcHeight[1]; 
						pdwBufStridesOrdered[2] = (lStride>>1)*srcHeight[2]; 
		 				
						//Swap U,V planes.
						if((m_overlayType == VP_OPEN_FLAG_SAMSUNG)||(m_overlayType == VP_OPEN_FLAG_ST)||(m_overlayType == VP_OPEN_FLAG_ANDROID_OPENCORE)
							|| (m_overlayType == VP_OPEN_FLAG_SAMSUNG6410) || (m_overlayType == VP_OPEN_FLAG_ANDROID))
						{
							LPBYTE buffTmp = ppbPlanes[1];
							ppbPlanes[1] = ppbPlanes[2];
							ppbPlanes[2] = buffTmp;
						}

					#ifdef USE_IPP_ROTATION
						res = PrepareBufferIPP_YV12(pBuf, lStride, ppbPlanes,pdwStrides, pdwBufStridesOrdered);
					#else

						res = m_PrepareData.PrepareBufferYV12(pBuf, lStride, 
							ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, 
							m_lRotationMode,dwScaleFlags,&rScaleDst,m_Deinterlaced_Mode);
						if(FAILED(res))
							DP("!!!!!!!!Preapre failed err:0x%x \n",res);

					#endif
					}
					break;
				case MAKEFOURCC('Y','U','1','2'):
					{
						LPBYTE ppbPlanesOrdered[3];
						DWORD pdwStridesOrdered[3];
						DWORD pdwWidthOrdered[3];
						DWORD pdwHeightOrdered[3];
						DWORD pdwBufStridesOrdered[3];

						ppbPlanesOrdered[0] = ppbPlanes[0];
						ppbPlanesOrdered[2] = ppbPlanes[1];
						ppbPlanesOrdered[1] = ppbPlanes[2];
						pdwStridesOrdered[0] = pdwStrides[0];
						pdwStridesOrdered[2] = pdwStrides[1];
						pdwStridesOrdered[1] = pdwStrides[2];
						pdwWidthOrdered[0] = srcWidth[0];
						pdwWidthOrdered[2] = srcWidth[1];
						pdwWidthOrdered[1] = srcWidth[2];
						pdwHeightOrdered[0] = srcHeight[0];
						pdwHeightOrdered[2] = srcHeight[1];
						pdwHeightOrdered[1] = srcHeight[2];
						pdwBufStridesOrdered[0] = lStride*pdwHeightOrdered[0]; 
						pdwBufStridesOrdered[1] = (lStride>>1)*pdwHeightOrdered[1]; 
						pdwBufStridesOrdered[2] = (lStride>>1)*pdwHeightOrdered[2]; 
						res =m_PrepareData.PrepareBufferYV12(pBuf, lStride, 
							ppbPlanesOrdered, pdwStridesOrdered, pdwWidthOrdered, 
							pdwHeightOrdered, dwFlags, m_lRotationMode, dwScaleFlags,
							&rScaleDst,VP_DEINTERLACE_None);
						if(FAILED(res))
							DP("!!!!!!!!Preapre failed err:0x%x \n",res);

					}
					break;
				case MAKEFOURCC('N','V','1','2'):
					{
						DWORD pdwBufStridesOrdered[3];
						pdwBufStridesOrdered[0] = lStride*srcHeight[0]; 
						res = m_PrepareData.PrepareBufferNV12(pBuf, lStride, ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, pdwBufStridesOrdered);
						if(FAILED(res))
							DP("!!!!!!!!Preapre failed err:0x%x \n",res);

					}
					break;
				case MAKEFOURCC('Y','U','2','4'):
					res = m_PrepareData.PrepareBufferYU24(pBuf, lStride, ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags);
					if(FAILED(res))
						DP("!!!!!!!!Preapre failed err:0x%x \n",res);
					break;
				case MAKEFOURCC('P','5','6','5'):
				case MAKEFOURCC('R','5','6','5'):
		 			if(dwScaleFlags)
					{
						res = m_PrepareData.PrepareBufferRGB565Scaled(pBuf, lStride,
							ppbPlanes, pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, 
							m_lRotationMode,m_Deinterlaced_Mode);
						if(FAILED(res))
							DP("!!!!!!!!Preapre failed err:0x%x \n",res);
					}
					else
					{
						
						res = m_PrepareData.PrepareBufferRGB565(pBuf,lStride, ppbPlanes, 
							pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode
							,m_Deinterlaced_Mode);
						if(FAILED(res))
							DP("!!!!!!!!Preapre failed err:0x%x \n",res);
					}
		 	        break;
			    case MAKEFOURCC('P','B','S','1'):
			        // The Priam back surface mode, just copy the handle of back surfcae
			        memcpy(pBuf, ppbPlanes, sizeof(void*));
			        break;
				case  MAKEFOURCC('X','R','G','B'):
					{
						if(surfaceFourCC != MAKEFOURCC('R','5','6','5'))
						{
							DP("PrepareBuffer Invalid input forcc format :0x%x\n", surfaceFourCC);
							break;
						}

						res = m_PrepareData.PrepareBufferXRGB(pBuf,lStride, ppbPlanes, 
							pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode);
						if(FAILED(res))
							DP("!!!!!!!!Preapre failed err:0x%x \n",res);

						break;
					}
				case MAKEFOURCC('R','8','8','8'):
					if(dwScaleFlags)
						res = m_PrepareData.PrepareBufferRGB888(pBuf, lStride, ppbPlanes,
						pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, m_lRotationMode);
					else
					{
						RECT rBuf;
						SetRect(&rBuf, 0,0,srcWidth[0],srcHeight[0]);
						res = m_PrepareData.PrepareBufferRGB888(pBuf, lStride, ppbPlanes, 
							pdwStrides, &rBuf, &rBuf, dwFlags, m_lRotationMode);
					
					}

					if(FAILED(res))
						DP("!!!!!!!!Preapre failed err:0x%x \n",res);
			        	break;
					
				//case MAKEFOURCC('U','Y','V','Y'):
				//		res = m_PrepareData.PrepareBufferYV12ToUYVY(pBuf, lStride, ppbPlanes, pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, m_lRotationMode);
				//	break;

				case MAKEFOURCC('A','R','G','B'):
					{
						DWORD srcStrideArr[3] = {m_OpenOptions.dwWidth,
							m_OpenOptions.dwWidth/2,m_OpenOptions.dwWidth/2};
						RECT rectSrc = {0,0,m_OpenOptions.dwWidth,m_OpenOptions.dwHeight};
						RECT rectDst = {0,0,m_OpenOptions.dwWidth,m_OpenOptions.dwHeight};
						if(m_Deinterlaced_Mode != VP_DEINTERLACE_WEAVE)
						{
							rectSrc.bottom /=2;
							srcStrideArr[0] *=2;
							srcStrideArr[1] *=2;
							srcStrideArr[2] *=2;
						}
						res = m_PrepareData.PrepareBufferYV12ToARGB(pBuf, lStride, ppbPlanes, srcStrideArr, 
							&rectSrc, &rectDst, 0, 0);
					}
					break;
				
				default:
					DP("Unknow format\n");
					break;
				}
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','U','Y','2'))
			{
				switch(m_dstSurfFourCC)
				{
				case MAKEFOURCC('Y','U','Y','2'):
				case MAKEFOURCC('Y','U','Y','V'):
					res = m_PrepareData.PrepareBufferYUY2(m_OpenOptions.dwFourCC,pBuf, lStride, ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags,m_lRotationMode);
					if(FAILED(res))
						DP("!!!!!!!!Preapre failed err:0x%x \n",res);
					break;
				default:
					break;
				}
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('X','R','G','B'))
			{
				memcpy(pBuf,ppbPlanes[0],srcWidth[0]*srcHeight[0]*4 );
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
			{
				if(dwScaleFlags)
				{
					res = m_PrepareData.PrepareBufferR565ToR565Scaled(pBuf, lStride, ppbPlanes, pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, m_lRotationMode);
					if(FAILED(res))
						DP("!!!!!!!!Preapre failed err:0x%x \n",res);
				}
				else
					memcpy(pBuf,ppbPlanes[0],srcWidth[0]*srcHeight[0]*2);
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','6'))
			{
			/*	if(m_dstSurfFourCC == MAKEFOURCC('R','5','6','5'))
				{
					RECT rSrc;
					SetRect(&rSrc, 0, 0, m_OpenOptions.dwWidth, m_OpenOptions.dwHeight);
					res = m_PrepareData.PrepareBufferYV16ToR565(pBuf,lStride, ppbPlanes,srcWidth,  &rSrc, &rSrc,0,0);
				}
				else
					DP("YV16 to 0x%x doesn't support. \n", surfaceFourCC);*/
			}

			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('N','V','1','2'))
			{
				if(m_dstSurfFourCC == MAKEFOURCC('N','V','1','2'))
				{
					if(lStride == srcWidth[0])
					{
						memcpy(pBuf, 							ppbPlanes[0], srcWidth[0]*srcHeight[0]);
						memcpy(pBuf + srcWidth[0]*srcHeight[0], 	ppbPlanes[1], srcWidth[0]*srcHeight[0]/2);
					}
					else
					{
						for(unsigned int i=0;i<srcHeight[0];i++)
							memcpy(pBuf + i*lStride,						ppbPlanes[0] + i*srcWidth[0], srcWidth[0]);

						for(unsigned int i=0;i<srcHeight[0];i++)
							memcpy(pBuf + i*lStride/2 + lStride*srcHeight[0],	ppbPlanes[1] + i*srcWidth[0]/2, srcWidth[0]/2);

					}
				}
			}

			m_ContextArray[count]->Unlock();
		}
	}
	return S_OK;
}

STDMETHODIMP CVRPresent::SetFPS(DWORD dwFPS)
{
	m_FPS = dwFPS;
	return S_OK;
}

STDMETHODIMP CVRPresent::GetBackBuffers(DWORD *pdwBackBuffers)
{
	if(!m_bOpen)
		return E_FAIL;

	*pdwBackBuffers = 1;
	return S_OK;
}

#endif
