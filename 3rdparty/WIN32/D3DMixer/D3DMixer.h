//=============================================================================
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
// 	Copyright (c) 2009  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef _D3DMIXER_H_
#define _D3DMIXER_H_

#ifndef CI_DEFINE_GUID

#ifdef __GNUG__  /* gcc/g++ compiler (Linux, Symbian ARMI) does the equivalent of _declspec(selectany) */
#define ATTRIBUTE_WEAK     __attribute__ ((weak))
#define DECLSPEC_SELECTANY
#elif defined(__ADS__)
#define ATTRIBUTE_WEAK  
#define DECLSPEC_SELECTANY 
#else
#define ATTRIBUTE_WEAK
#if !defined(DECLSPEC_SELECTANY)
#define DECLSPEC_SELECTANY _declspec(selectany)
#endif
#endif

	#define CI_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
		 EXTERN_C DECLSPEC_SELECTANY const GUID FAR name ATTRIBUTE_WEAK \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
                
#endif


// {8A1AC51C-4DCB-42bf-BBC7-268873D901E0}
CI_DEFINE_GUID(IID_IMcVideoStream,
0x8a1ac51c, 0x4dcb, 0x42bf, 0xbb, 0xc7, 0x26, 0x88, 0x73, 0xd9, 0x1, 0xe0);

// {30460145-35B8-43c2-BEBC-67627789F774}
CI_DEFINE_GUID(IID_IMcVideoStreamTest,
0x30460145, 0x35b8, 0x43c2, 0xbe, 0xbc, 0x67, 0x62, 0x77, 0x89, 0xf7, 0x74);

// {C672F9E9-F05B-4239-86C3-D4F0B8EBF8D8}
CI_DEFINE_GUID(IID_IMcVideoMixer,
0xc672f9e9, 0xf05b, 0x4239, 0x86, 0xc3, 0xd4, 0xf0, 0xb8, 0xeb, 0xf8, 0xd8);

// {380B9D1F-A590-46c2-AD24-5D5E63E4494A}
CI_DEFINE_GUID(IID_IMcVideoMixerExt,
0x380b9d1f, 0xa590, 0x46c2, 0xad, 0x24, 0x5d, 0x5e, 0x63, 0xe4, 0x49, 0x4a);

// {207B95A7-FB5D-4daa-9FDA-B838347FA7A6}
CI_DEFINE_GUID(MC_PROPSET_VIDEOMIXER,
0x207b95a7, 0xfb5d, 0x4daa, 0x9f, 0xda, 0xb8, 0x38, 0x34, 0x7f, 0xa7, 0xa6);

// {571000A5-BA60-4b43-8805-2645D28476B3}
CI_DEFINE_GUID(MC_PROPSET_VIDEOSTREAM,
0x571000a5, 0xba60, 0x4b43, 0x88, 0x5, 0x26, 0x45, 0xd2, 0x84, 0x76, 0xb3);

struct VideoMixer_OpenParams
{
	HWND hWnd;							// window handle
	DWORD dwQuality;					// ignored, 1-100
	DWORD dwRenderMode;					// ignored for now
	FLOAT fRenderFrequency;	
};

enum
{
	VIDEOMIXER_RENDERMODE_MANUAL,		// default
	VIDEOMIXER_RENDERMODE_ONUPDATE,		// wait for stream update
	VIDEOMIXER_RENDERMODE_ONVBLANK,		// wait for vertical blank
	VIDEOMIXER_RENDERMODE_ONFREQUENCY,	// use internally generated frequency fFrequency;
};

enum
{
	VIDEOMIXER_PROPID_BACKGROUNDCOLOR = 1,
	VIDEOSTREAM_PROPID_WINDOWSIZE,			// SET: resize request
};

struct VideoMixer_AdvancedParams;		/* This parameter is confidential and is supplied under Cidana NDA */

struct VideoStream_OpenParams
{
	DWORD dwVideoWidth;
	DWORD dwVideoHeight;
};

struct VideoStream_RenderParams
{
	FLOAT fZOrder;
	DWORD dwAlphaTechnique;
	FLOAT fAlpha;
	FLOAT fSrcLeft, fSrcTop, fSrcRight, fSrcBottom;
	FLOAT fDstLeft, fDstTop, fDstRight, fDstBottom;
	FLOAT fDeltaBrightness, fDeltaContrast, fDeltaHue, fDeltaSaturation, fDeltaGamma;
	DWORD dwStretchTechnique;
	FLOAT fStretchSlope, fStretchCenter;
	DWORD dwDeinterlaceTechnique;
};

struct VideoStream_AdvancedParams;		/* This parameter is confidential and is supplied under Cidana NDA */

enum
{
	VIDEOSTREAM_ALPHA_TECHNIQUE_NONE,						// equivalent to fAlpha = 1.0
	VIDEOSTREAM_ALPHA_TECHNIQUE_CONSTANT,					// use fAlpha
	VIDEOSTREAM_ALPHA_TECHNIQUE_SRC,						// use source alpha.  Video always has alpha of 1.
};

enum
{
	VIDEOSTREAM_STRETCH_TECHNIQUE_NONE,					//default
	VIDEOSTREAM_STRETCH_TECHNIQUE_LINEAR,
	VIDEOSTREAM_STRETCH_TECHNIQUE_CUBIC,
};

enum
{
	VIDEOSTREAM_DEINTERLACE_TECHNIQUE_WEAVE,
	VIDEOSTREAM_DEINTERLACE_TECHNIQUE_BOB,
	VIDEOSTREAM_DEINTERLACE_TECHNIQUE_MEDIAN,
};

enum
{
	VIDEOSTREAM_RENDERFLAG_ODD			= 0,		//default
	VIDEOSTREAM_RENDERFLAG_WEAVE		= 0,		//default
	VIDEOSTREAM_RENDERFLAG_INTERLACED	= (1<<0),
	VIDEOSTREAM_RENDERFLAG_EVEN			= (1<<1),
};

enum
{
	VIDEOSTREAM_PROPID_RENDERPARAMS = 1,
	VIDEOSTREAM_PROPID_ZORDER,
	VIDEOSTREAM_PROPID_BACKGROUNDCOLOR,		// background color (shouldn't matter)
};

DECLARE_INTERFACE_(IMcVideoStream, IUnknown)
{
	STDMETHOD(Open)(IN VideoStream_OpenParams *pOpenParams,
					 IN DWORD dwOpenSize) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(Render)(DWORD dwRenderFlags) PURE;
	STDMETHOD(Get)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned) PURE;
	STDMETHOD(Set)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData) PURE;
	STDMETHOD(LockSurface)(DWORD dwIndex, LPBYTE *ppData, DWORD *pdwStride) PURE;
	STDMETHOD(UnlockSurface)(DWORD dwIndex) PURE;
	STDMETHOD(GetD3DTexture)(DWORD dwIndex, LPVOID *ppTexture) PURE;
};

DECLARE_INTERFACE_(IMcVideoStreamTest, IUnknown)
{
	STDMETHOD(CreateYV12TestPattern)(DWORD dwPattern) PURE;
	STDMETHOD(LoadTextureFromFile)(DWORD dwIndex, TCHAR *tszPath) PURE;
};

typedef IMcVideoStream *LPMCVIDEOSTREAM;

DECLARE_INTERFACE_(IMcVideoMixer, IUnknown)
{
	STDMETHOD(Open)(IN VideoMixer_OpenParams *pOpenParams,
					 IN DWORD dwOpenSize) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(Render)(DWORD dwRenderFlags) PURE;
	STDMETHOD(Get)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned) PURE;
	STDMETHOD(Set)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData) PURE;
	STDMETHOD(CreateVideoStream)(OUT IMcVideoStream **ppStream) PURE;
};

IMcVideoMixer *CreateD3DVideoMixer();

#endif //_D3DMIXER_H_
