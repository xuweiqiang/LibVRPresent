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
//#include "../../inc/VideoMixer.h"

#define MAX_STREAM 8

extern STDMETHODIMP LoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);
extern STDMETHODIMP UnLoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter);

class CVRPresentOvlProcessorSW : 
	public IMcVideoPresenterAdv,
	public CMcUnknown
{
public:
	CVRPresentOvlProcessorSW ();
	virtual ~CVRPresentOvlProcessorSW ();

	//IUNKNOWN
	DECLARE_IUNKNOWN
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid == IID_IMcVideoPresenter)
		{
			LoadV1Library(m_pDll,&m_pPresent);
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

	STDMETHODIMP CreateCanvas();
	STDMETHODIMP CreateV1VR(struct VP_OpenOptionsAdv* pOpenParm);

private:
	STDMETHODIMP ColorConvertion_Resize(void* pInput,void* pOutput,
		DWORD input_width, DWORD input_height, DWORD input_format,
		DWORD output_width, DWORD output_height, DWORD output_format, 
		RECT* pCropRect, RECT* pDstRect, bool forTranspRent, LONG flipMode);
	
	STDMETHODIMP TransparentOverwrite(void* pBackground,void* pSrc,DWORD dwSrcTransColor, DWORD dwBackWidth, DWORD dwBackHeight,
		DWORD dwSrcWidth,DWORD dwSrcHeight, RECT* pSrcOffset, FLOAT alpha);
	
	STDMETHODIMP GetFourCCLength(DWORD fourCC, DWORD *plength);
	STDMETHODIMP AdjustCanvasSize(unsigned int w, unsigned int h, float* pAspwnd);
	int GetStreamNumber();

	IMcVideoPresenter* m_pPresent;
	bool m_Init;
	DWORD m_streamCount;
	CAlphaBlending* m_pAlphaBlend;
	long m_zoom_mode;
	HMODULE m_pDll;
	struct VP_OpenOptionsAdv m_openParams;
	struct VP_OpenOptionsAdv m_openParamsVR1;

	typedef struct
	{
		bool valid;
		LPBYTE pBuf;
		VP_OPEN_STREAM_PARAMS param;
		IMcVideoPresenterStream *pStream;
	}SSTREAM_INFO;
	SSTREAM_INFO m_pStmArrar[MAX_STREAM];

	GfxPrepareDataV2 m_PrepareData;
	RECT m_rDst;		// destination rectangle to store the image, relative to video window/port.
	RECT m_rSrc;	// source size (also used as source clipping region) 
	CRITICAL_SECTION m_csDisplay; 
	LPBYTE m_canvasBuf;
	LPBYTE m_canvasResizeBuf;
	DWORD m_canvasFourCC;
	RECT m_rWnd;
	bool m_bcanChanged;
	long m_cropwidth ;
	long m_cropheight ;
	bool m_VRopened;
	ICivmVideoMixer* m_pVideoMixer;
	VR_ANDROID_ISUR_PARAM m_android_param;
	#ifdef ANDROID_VR_JNI
	VR_ANDROID_SURF_PARAM m_android_surfparam;
    #endif
	LONG m_lFlipVideoMode;
};



