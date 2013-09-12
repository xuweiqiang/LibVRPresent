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
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef _VRPRESENT_H_
#define _VRPRESENT_H_

#ifndef MAKE_FOURCC
#define MAKE_FOURCC(c1,c2,c3,c4) ((c4)<<24 | (c3)<<16 | (c2)<<8 | (c1))
#endif

#ifndef MAKEFOURCC
#define MAKEFOURCC(c1,c2,c3,c4) ((c4)<<24 | (c3)<<16 | (c2)<<8 | (c1))
#endif

#ifndef MC_PROPID_OFFSET
#define MC_PROPID_OFFSET(groupid)	(groupid<<8)
#endif

#ifndef EXPORT
#define EXPORT extern "C" _declspec(dllexport)
#endif


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
#ifndef TESTPROGRAM
// {9eccfcca-eb44-11d4-b365-0001022s4a87}
CI_DEFINE_GUID(IID_IMcVideoPresenter,
0x9eccfcca, 0xeb44, 0x11d4, 0xb3, 0x65, 0x00, 0x01, 0x02, 0x2a, 0x4a, 0x87);

// {022CF5CC-85A3-43aa-9471-675B6658A27B}
CI_DEFINE_GUID(IID_IMcVideoPresenterAdv,
0x22cf5cc, 0x85a3, 0x43aa, 0x94, 0x71, 0x67, 0x5b, 0x66, 0x58, 0xa2, 0x7b);

// {B1D629AA-D7AA-4efd-BA6B-7A54C899C5BF}
CI_DEFINE_GUID(IID_IMcVideoPresenterStream,
0xb1d629aa, 0xd7aa, 0x4efd, 0xba, 0x6b, 0x7a, 0x54, 0xc8, 0x99, 0xc5, 0xbf);
#endif
//Value range for COLOR CONTROL
#define MAX_COLORCONTROL_VAL	255
#define MIN_COLORCONTROL_VAL  0

//Value range for ALPA
#define MAX_ALPHA_VAL	255			//Show
#define MIN_ALPHA_VAL  0			//Hide

#define PRESENT_FLAG_NO_COPY_DATA	0x5000

enum
{
    VP_DMCP_CGMSA_Disabled = 0,
    VP_DMCP_CGMSA_CopyFreely,		
    VP_DMCP_CGMSA_CopyNoMore,
    VP_DMCP_CGMSA_CopyOneGeneration,
    VP_DMCP_CGMSA_CopyNever,
};

enum
{
    VP_DMCP_ACP_Level0 = 0,	 // disabled
    VP_DMCP_ACP_Level1,
    VP_DMCP_ACP_Level2,
    VP_DMCP_ACP_Level3,
};

enum
{
    VP_DMCP_HDCP_Level0 = 0,	// disabled
    VP_DMCP_HDCP_Level1,
};

typedef enum 
{
	VP_ZOOM_MODE_STRETCH		= 0x00000001,  /// stretching the source to fulfill the dst
	VP_ZOOM_MODE_LETTERBOX	= 0x00000010,  /// adding black border to keep the aspect ratio
	VP_ZOOM_MODE_CROPPING	= 0x00000100   /// cropping source to keep the aspect ratio
}VP_ZOOM_MODE;

typedef enum
{
	VP_ROTATION_0,
	VP_ROTATION_90,
	VP_ROTATION_180,
	VP_ROTATION_270,
}VP_ROTATION_MODE;

typedef enum
{
	VP_DEINTERLACE_WEAVE,
	VP_DEINTERLACE_BOB_SINGLE_FIELD,
	VP_DEINTERLACE_BOB_DOUBLE_FIELD
}VP_DEINTERLACE_MODE;

enum MC_PROPID_VP
{
	MC_PROPID_VP_TYPE = MC_PROPID_OFFSET(0),		//Surface type. Use VR_OPEN_SURF_TYPE.
	MC_PROPID_VP_ENABLE,							//Enable/Disable. Use BOOL(TRUE/FALSE).
	MC_PROPID_VP_RECTDST,							//Destination Rectangle. Use RECT.
	MC_PROPID_VP_RECTSRC,							//Source Rectangle. Use RECT. 			Notice:It takes effect only when Zoom mode is disable!!!

	//Video Effect
	MC_PROPID_VP_COLORSYS_CAP = MC_PROPID_OFFSET(1),	//Capabity of color control. 	Use DDCOLORCONTROL.
	MC_PROPID_VP_BRIGHTNESS ,								//Brightness. 	Range: MIN_COLORCONTROL_VAL-MAX_COLORCONTROL_VAL   
	MC_PROPID_VP_CONTRAST,								//Contrast. 	Range: MIN_COLORCONTROL_VAL-MAX_COLORCONTROL_VAL  
	MC_PROPID_VP_HUE,										//Hue. 	Range: MIN_COLORCONTROL_VAL-MAX_COLORCONTROL_VAL  
	MC_PROPID_VP_SATURATION,								//Satuation. 	Range: MIN_COLORCONTROL_VAL-MAX_COLORCONTROL_VAL  
	MC_PROPID_VP_GAMMA,									//Gamma. 	Range: MIN_COLORCONTROL_VAL-MAX_COLORCONTROL_VAL  
	MC_PROPID_VP_COLORENABLE,							//Color Enable. 	Range: MIN_COLORCONTROL_VAL-MAX_COLORCONTROL_VAL  
	MC_PROPID_VP_SHARPNESS,								//Sharpness. 	Range: MIN_COLORCONTROL_VAL-MAX_COLORCONTROL_VAL  
	
	//ColorKey
	MC_PROPID_VP_COLORKEYDST = MC_PROPID_OFFSET(2),	//Destination color key. Use DWORD.
	MC_PROPID_VP_COLORKEYSRC,							//Source color key . 	 Use 	YUV =0x00yyuuvv, RGB =0x00rrggbb;

	MC_PROPID_VP_DUMM1 =  MC_PROPID_OFFSET(3),	
	MC_PROPID_VP_ZOOM_ENABLE,	//Enable/Disable Zoom mode.  Use BOOL.VP_ZOOM only works after enalbing Zoom mode.
	MC_PROPID_VP_ZOOM,									//Zoom mode.  Use VP_ZOOM_MODE.
	MC_PROPID_VP_ROTATION,								//Rotation mode. Use VP_ROTATION_MODE
	MC_PROPID_VP_RECT_SCREEN,							//Screen Rectangle, Use RECT.
	MC_PROPID_VP_WND,									//Window Handle. Use  HWND.
	MC_PROPID_VP_ALPHA,									//Alpha Value. Use DWORD.
	MC_PROPID_VP_DUMM2 ,
	MC_PROPID_VP_VIDEO_MEMORY, 							//VideoMemory Adress. Use VP_VIDEO_MEMORY_INFO
	MC_PROPID_VP_TITIANII_HND, 							//TITIANII handle. Use VR_HANDLE
	MC_PROPID_VP_ANDROID_ISURFACE,							//Hardware surface. Depending on specific platform.
	MC_PROPID_VP_CLEAR,									//Clear screen. 

	
	//Not supported currently
	MC_PROPID_VP_INTERPOLATION = MC_PROPID_OFFSET(4),	//Not used
	MC_PROPID_VP_DEBLOCK,								//Not used
	MC_PROPID_VP_RESAMPLE,								//Not used
	MC_PROPID_VP_DEINTERLACE_MODE ,						//Not used

	//DMCP
	MC_PROPID_VP_DMCP_ACP = MC_PROPID_OFFSET(5),		//ACP configuration. Use DWORD.
	MC_PROPID_VP_DMCP_CGMS,								//CGMSA configuration. Use DWORD.
	MC_PROPID_VP_DMCP_HDCP,								//HDCP configuration . Use DWORD
	
	MC_PROPID_VP_SURFACE,
};

struct VP_OpenOptions
{
	DWORD	dwWidth;
	DWORD	dwHeight;
	DWORD	dwBuffers;	
	DWORD	dwFourCC;	 
	DWORD	dwSurfType;	
	DWORD	dwFlags;	
	LPVOID	hWnd;	
	DWORD	dwAspectX;	
	DWORD	dwAspectY;
};

struct VP_SURFACE_OpenOptions
{
	DWORD	dwWidth;
	DWORD	dwHeight;
	DWORD	dwFourCC;
	DWORD	dwTranColor;
};

typedef enum
{
	VP_OPEN_FLAG_AUTO			= 0,
	VP_OPEN_FLAG_HARDWARE			= (1<<0),
	VP_OPEN_FLAG_DDRAW				= (1<<1),
	VP_OPEN_FLAG_GX					= (1<<2),
	VP_OPEN_FLAG_GDI				= (1<<3),
	VP_OPEN_FLAG_D3D				= (1<<4),
	VP_OPEN_FLAG_TITIAN_II		= (1<<5),
	VP_OPEN_FLAG_RMI			= (1<<6),
	VP_OPEN_FLAG_SDL			= (1<<7),
	VP_OPEN_FLAG_SAMSUNG	= (1<<8),
	VP_OPEN_FLAG_PXA	= (1<<9),
	VP_OPEN_FLAG_ANDROID			= (1<<10),
	VP_OPEN_FLAG_FB			= (1<<11),
	VP_OPEN_FLAG_ST			= (1<<12),
	VP_OPEN_FLAG_DDRAW_OSD	= (1<<13),
	VP_OPEN_FLAG_ANDROID_SF			= (1<<14),
}VR_OPEN_SURF_TYPE;
/*
There are two ways to use VR_OPEN_SURF_TYPE.
1. Use VP_OPEN_FLAG_AUTO. VideoPresenter will select hardware surface automatically.
	On windows platform, priority sequence is D3D>DDRAW>GDI.
	On the others platform, priority sequence is Hardware render>DDRAW>GDI>FB. 
	It returns failed even if all platforms aren't supported.
2. Specify a or more surface types, such as VP_OPEN_FLAG_ST|VP_OPEN_FLAG_DDRAW. 
Then ST_Hardware render surface will be tried first and Ddraw surface later. It returns 
failed if both sufaces aren't supported.  VideoPresenter will not be responsible for providing a 
workable surface under this mode.

*/

typedef enum
{
	VP_OPEN_FLAG_DDRAW_AUTO				= 0,
	VP_OPEN_FLAG_DDRAW_OVERLAY			= (1<<0),
	VP_OPEN_FLAG_DDRAW_BLIT				= (1<<1),
	VP_OPEN_FLAG_OSD						= (1<<2),
}VR_OPEN_SURF_TYPE_DETIAL;


typedef enum
{
	VP_SURF_LAYERTOP = 0,	//It's the hightest surface. Please use it while you want to set all params to all surfaces.
	VP_SURF_LAYER1= 1,
	VP_SURF_LAYER2= 2,
	VP_SURF_LAYER3= 3,
	VP_SURF_LAYERBOTTOM= 4
}VR_SURFACE_TYPE;


#define VP_OPEN_FLAG_MASK (VP_OPEN_FLAG_HARDWARE | VP_OPEN_FLAG_DDRAW | VP_OPEN_FLAG_GX | VP_OPEN_FLAG_GDI | VP_OPEN_FLAG_D3D | 	\
VP_OPEN_FLAG_TITIAN_II | VP_OPEN_FLAG_SDL | VP_OPEN_FLAG_SAMSUNG | VP_OPEN_FLAG_PXA | VP_OPEN_FLAG_ANDROID | VP_OPEN_FLAG_FB		\
| VP_OPEN_FLAG_ST | VP_OPEN_FLAG_DDRAW_OSD | VP_OPEN_FLAG_ANDROID_SF)

//VP_CAPS.dwCaps
enum
{
	VP_CAP_PRESENT_BOB				= (1<<0),
	VP_CAP_PRESENT_DEINTERLACE		= (1<<1),
	VP_CAP_PREPARE_BOB				= (1<<16),
	VP_CAP_PREPARE_DEINTERLACE		= (1<<17),
};

enum 
{
	VP_PRESENT_FIELD_TOPFIRST		= (1<<0),
	VP_PRESENT_FIELD_BOTTOMFIRST	= (1<<1),
	VP_PRESENT_FIELD_TOP_ONLY	= (1<<2),
	VP_PRESENT_FIELD_BOTTOM_ONLY	= (1<<3),
	VP_PREPARE_FIELD_TOPFIRST		= (1<<16),
	VP_PREPARE_FIELD_BOTTOMFIRST	= (1<<17)
};

struct VP_CAPS
{
	DWORD dwSize;
	DWORD dwCaps;
};

/*! Video stream attributes.
 */
struct VP_VIDEOBUFFER_INFO
{
	RECT	rSrc;			/// source rectangle
	RECT	rDst;			/// destination rectangle
	RECT	rBnd;			/// bounding rectangle (of buffer). 
	DWORD	dwPlanes;
	DWORD	dwWidth[4];
	DWORD	dwHeight[4];
	DWORD	dwStride[4];
	DWORD	dwFourCC;
	DWORD	dwAspectX;
	DWORD	dwAspectY;
};

enum
{
	VPI_DISPLAYFLAG_TOPFIELDFIRST = (1<<0),
	VPI_DISPLAYFLAG_BOTTOMFIELDFIRST = (1<<1),
};

struct EMC_VIDEOPRESENTATION_INFO
{
	DWORD	dwDisplayFlag;
};

struct VP_VIDEO_MEMORY_INFO
{
	LPBYTE pBuf;
	LONG lStride;
};

enum STREAM_TYPE
{
	eMAINSTREAM = 0x1000,
	eSUBSTREAMS
};

struct VP_OPEN_STREAM_PARAMS
{
	STREAM_TYPE type;
	DWORD	dwWidth;
	DWORD	dwHeight;
	DWORD	dwInputFourCC;
	DWORD	dwTranColor;
	HWND	hWnd;
	DWORD	dwAspectX;	
	DWORD	dwAspectY;
};

struct VP_OpenOptionsAdv
{
	HWND hWnd;							
	DWORD dwQuality;					
	DWORD dwRenderMode;				
	FLOAT fRenderFrequency;	
};

/************************			Interface 2.5	*****************************************************************/
DECLARE_INTERFACE_(IMcVideoPresenter,IUnknown)
{
	//For MainSurface Command
	STDMETHOD(Open)(VP_OpenOptions *pOptions, DWORD dwOptionSize) PURE;
	/*
	Open video presenter according to the open options.

	Parameters:
		dwWidth: surface width.(MUST)
		dwHeight: surface height.(MUST)
		//Default value will be set if following parameters are ignored.
		dwBuffers: back buffer number.
		dwFourCC; fourCC mode of input data. 
		dwSurfType: internal surface type.  Use VR_OPEN_SURF_TYPE.
		dwFlags: detailed inernal surface type. Use VR_OPEN_SURF_TYPE_DETIAL.This flag is only available while set VP_OPEN_FLAG_DDRAW
		hWnd: Handle of video window. 
		dwAspectX:Aspect ratio of original picture.	
		dwAspectY:Aspect ratio of original picture

		dwOptionSize: size of VP_OpenOptions.

	Return code:
		S_OK on success.
		E_INVALIDARG if argument is invalid.
		E_INVALIDSTATE if renderer is already opened.
		E_FAIL if it fails to be initialized.
	*/

	STDMETHOD(Close)() PURE;
	/*
	Close video presenter.

	Parameters:
		NULL.
	Return code:
		S_OK on success.
		E_FAIL if the video presenter cannot be closed.
	*/

	STDMETHOD(Reset)() PURE;		
	/*
	Reset whole video presenter.

	Parameters:
		NULL.
	Return code:
		S_OK on success.
		E_FAIL if the video presenter cannot be closed.
	*/
	STDMETHOD(GetStatus)(DWORD *pdwStatus) PURE;
	/*
	Gets status info from the presenter.  This fills the caller allocated structure pdwStatus.

	Parameters:
		pdwStatus: Status structure.
		
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	
	STDMETHOD(GetCaps)(VP_CAPS *pCaps) PURE;
	/*
	Gets capability info from the presenter.  This fills the caller allocated structure pdwStatus.

	Parameters:
		pCaps: Capability structure.
		
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	STDMETHOD(Restore)() PURE;	//Restore the Gfxsurf.
	/*
	Reset video presenter surfaces only.

	Parameters:
		NULL.
		
	Return code:
		S_OK on success.
		E_FAIL if the video presenter cannot be closed.
	*/

	STDMETHOD(SetInterface)(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk) PURE;
	/*
	Import a COM instance in.

	Parameters:
		dwRoute: identifier of runtime app.
		riid: COM interface.
		pUnk: COM instance pointer.
		
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	STDMETHOD(SetFPS)(DWORD dwFPS) PURE;
	/*
	Set render framerate

	Parameters:
		dwFPS: frame rate.
		
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	STDMETHOD(GetBackBuffers)(DWORD *pdwBackBuffers) PURE;
	/*
	Gets backbuffer numberfrom the presenter.  This fills the caller allocated structure pdwBackBuffers.

	Parameters:
		pdwBackBuffers: backbuffer number.
		
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(Get)(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned) PURE;
	/*
	Returns the internal render property.

	Parameters:
		guidPropSet: GUID of caller.
		dwPropID: Command. Use MC_PROPID_VP;
		pInstanceData: Not used.
		cbInstanceData: Not used.
		pPropData: Input parameter pointer.
		pInstanceData: Not used.
		pcbReturned: return value.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	STDMETHOD(Set)(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData) PURE;
	/*
	Sets the internal presenter property.

	Parameters:
		guidPropSet: GUID of caller.
		dwPropID: property ID which matches identically the Get();
		pInstanceData: Not used.
		cbInstanceData: Not used.
		pPropData: Input parameter pointer.
		pInstanceData: Not used.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(Present)(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags) PURE;
	/*
	Prepare & Present a frame.

	Parameters:
		ppbPlanes: 
			ppbPlanes[0]:Y plane data/ R plane data.
			ppbPlanes[1]:U plane data/ G plane data.
			ppbPlanes[2]:V plane data/ B plane data.
		pdwStrides:
			pdwStrides[0]: stride of plane0.
			pdwStrides[1]: stride of plane1.
			pdwStrides[2]: stride of plane2.
		dwFlags: Not used.	
			
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	STDMETHOD(PrepareBuffer)(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags) PURE;
	/*
	Prepare a frame. It's not rendered until PresentBuffer() is called.

	Parameters:
		lNum: buffer index.
		ppbPlanes:
			ppbPlanes[0]:Y plane data/ R plane data.
			ppbPlanes[1]:U plane data/ G plane data.
			ppbPlanes[2]:V plane data/ B plane data.
		pdwStrides:
			pdwStrides[0]: stride of plane0.
			pdwStrides[1]: stride of plane1.
			pdwStrides[2]: stride of plane2.
		dwFlags: Not used.	
			
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(PresentBuffer)(LONG lNum, DWORD dwFlags) PURE;
	/*
	Present a frame. The data is feed by PrepareBuffer().

	Parameters:
		lNum: buffer index.
		dwFlags: Not used.	
			
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	STDMETHOD(SetVBI)(VP_VIDEOBUFFER_INFO *pVBI) PURE;
	/*
	Set informations from decoder. This API shall be called if video resolution changes.

	Parameters:
		pVBI: input params.	
			
	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	
	/*************************For OSD Surface Command ***********************************/
	STDMETHOD(CreateSurface)(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam) PURE;
	STDMETHOD(DestroySurface)(VR_SURFACE_TYPE type) PURE;
	STDMETHOD(GetSurface)(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned) PURE;
	STDMETHOD(SetSurface)(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData) PURE;
	STDMETHOD(Present)(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags) PURE;
	STDMETHOD(PrepareBuffer)(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags) PURE;
	STDMETHOD(PresentBuffer)(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags) PURE;

#ifdef TITAN_II
	STDMETHOD(PresentRect)(LPBYTE *ppbPlanes, DWORD *pdwStrides,DWORD dwFlags ,RECT* pSrcRgn, RECT* pDstRgn) PURE;
#endif
};


/************************			Interface 3.0	*****************************************************************/


DECLARE_INTERFACE_(IMcVideoPresenterStream,IUnknown)
{
	STDMETHOD(Get)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned) PURE;
	/*
	Returns the stream property.

	Parameters:
		guidPropSet: GUID of caller.
		dwPropID: Command. Use MC_PROPID_VP;
		pInstanceData: Not used.
		cbInstanceData: Not used.
		pPropData: Input parameter pointer.
		pInstanceData: Not used.
		pcbReturned: return value.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(Set)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData) PURE;
	/*
	Sets the stream property.

	Parameters:
		guidPropSet: GUID of caller.
		dwPropID: property ID which matches identically the Get();
		pInstanceData: Not used.
		cbInstanceData: Not used.
		pPropData: Input parameter pointer.
		pInstanceData: Not used.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(LockSurface)( LPBYTE *ppData, DWORD *pdwStride) PURE;
	/*
	Lock a surface.

	Parameters:
		deIndex: surface id.
		ppData: a pointer to return start buffer address which is locked;
		pdwStride: stride of locked buffer.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(UnlockSurface)() PURE;
	/*
	UnLock a surface.

	Parameters:
		deIndex: surface id.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/
	STDMETHOD(Present)(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags) PURE;

};

DECLARE_INTERFACE_(IMcVideoPresenterAdv,IUnknown)
{
	//For MainSurface Command
	STDMETHOD(Open)(IN VP_OpenOptionsAdv *pOpenParams,
					 IN DWORD dwOpenSize) PURE;
	/*
	Open video presenter according to the open options.

	Parameters:
		pOpenParams: input params.
			hWnd: window handle.
			dwQuality: video quality. 1-100(DEFAULT,CLEAR)
			dwRenderMode:	Not used.
			fRenderFrequency: Not used.	
		dwOpenSize: size of VP_OpenOptionsAdv.
		
	Return code:
		S_OK on success.
		E_INVALIDARG if argument is invalid.
		E_INVALIDSTATE if renderer is already opened.
		E_FAIL if it fails to be initialized.
	*/

	STDMETHOD(Close)() PURE;
	/*
	Close video presenter.

	Parameters:
		NULL.
	Return code:
		S_OK on success.
		E_FAIL if the video presenter cannot be closed.
	*/
	STDMETHOD(Render)(DWORD dwRenderFlags) PURE;
	/*
	Render a frame, which is composed of all visable streams.

	Parameters:
		dwRenderFlags: Not used.
		
	Return code:
		S_OK on success.
		E_FAIL if the video presenter cannot be closed.
	*/

	STDMETHOD(Get)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned) PURE;
	/*
	Returns the internal render property.

	Parameters:
		guidPropSet: GUID of caller.
		dwPropID: Command. Use MC_PROPID_VP;
		pInstanceData: Not used.
		cbInstanceData: Not used.
		pPropData: Input parameter pointer.
		pInstanceData: Not used.
		pcbReturned: return value.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(Set)(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData) PURE;
	/*
	Sets the internal presenter property.

	Parameters:
		guidPropSet: GUID of caller.
		dwPropID: property ID which matches identically the Get();
		pInstanceData: Not used.
		cbInstanceData: Not used.
		pPropData: Input parameter pointer.
		pInstanceData: Not used.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(CreateStream)(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream) PURE;
	/*
	Create a handle for the stream instance.

	Parameters:
		ppStream: a pointer to receivce stream handle.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

	STDMETHOD(DestroyStream)(IN IMcVideoPresenterStream *ppStream) PURE;
	/*
	Releases a handle for the stream instance.

	Parameters:
		ppStream: a pointer to receivce stream handle.

	Return code:
		S_OK on success.
		E_FAIL if it fails.
	*/

};


EXPORT IMcVideoPresenter *CreateVideoPresenter();
EXPORT IMcVideoPresenterAdv*CreateVideoPresenterAdv();
/*
Creates a handle for the video render instance, which is returned in LPVOID*.
Parameters:
	riid: a render interface: IMcVideoPresenter,IMcVideoPresenterAdv. 

Return code:
	Non-ZERO on success.
	NULL if the render cannot be initialized.
*/

typedef  IMcVideoPresenter* (*fpCreateVideoPresenter)();
typedef  IMcVideoPresenterAdv* (*fpCreateVideoPresenterAdv)();

/*
Function pointer type, which is used to query entry function.
*/
#endif //_VRPRESENT_H_
