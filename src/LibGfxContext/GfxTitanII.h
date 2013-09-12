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

#ifndef _GFXTITANII_H_
#define BSM_HOST

#include "GfxOverlay.h"
#include "..\..\3rdparty\TitanII\inc\RendererSDK.h"
#include "..\..\3rdparty\TitanII\inc\BHSurfMgr.h"
#include "../../inc/McCom.h"

typedef VR_FRAME_SURFACE *LPVR_FRAME_SURFACE;

typedef VR_RESULT	(*CVR_CreateVideoRender_fxn)(VR_HANDLE *pHandle,  VR_HWND hWnd);
typedef VR_RESULT	(*CVR_DeleteVideoRender_fxn)(VR_HANDLE handle);
typedef VR_RESULT	(*CVR_SetVideoFormat_fxn)(VR_HANDLE handle, VR_SURF_FORMATID format_id, int width,	int height,   VR_SRCRECT* pSrcRect,	 int* LumPitch,   int* ChromPitch);
typedef VR_RESULT	(*CVR_SetDisplayMode_fxn)(VR_HANDLE handle,  VR_DISP_MODE_DESC mode);
typedef VR_RESULT	(*CVR_SetDstRect_fxn)(VR_HANDLE handle, VR_DSTRECT  rect);
typedef VR_RESULT	(*CVR_Render_fxn)(VR_HANDLE handle,   VR_FRAME_SURFACE* pSurf,  VR_REN_SYNC_MODE WaitMode);
typedef VR_RESULT	(*CVR_WaitComplete_fxn)(VR_HANDLE handle);
typedef VR_RESULT	(*CVR_CreateSurface_fxn)(VR_HANDLE handle,VR_FRAME_SURFACE** ppSurf,	int bPhysical);
typedef VR_RESULT	(*CVR_DestroySurface_fxn)(VR_HANDLE handle,	VR_FRAME_SURFACE* pSurf);
typedef VR_RESULT	(*CVR_LockSurface_fxn)(VR_HANDLE handle, VR_FRAME_SURFACE* pSurf);
typedef VR_RESULT	(*CVR_UnlockSurface_fxn)(VR_HANDLE handle,VR_FRAME_SURFACE* pSurf);

typedef VR_RESULT (*CVR_SetDeinterlaceMode_fxn)(VR_HANDLE handle,  VR_DEINTERLACE_MODE mode);	
typedef VR_RESULT (*CVR_SetRenderLayer_fxn)(VR_HANDLE handle, VR_RENDER_LAYER layer,  double fAlpha);

typedef unsigned long (*CVR_GetLastError_fxn)(  VR_HANDLE handle);


typedef SX_UINT32 (*BSM_Init_fxn)(VR_HANDLE handle);
typedef void (*BSM_DeInit_fxn)();
typedef void (*BSM_OutputSurf_fxn)(SXMemSurfDesc* surf, SX_REN_SYNC_MODE WaitMode);
typedef VR_FRAME_SURFACE* (*CvtToRendererSurf_fxn)(SXMemSurfDesc* surf);

typedef SX_UINT32 (*BSM_LockSurf_fxn)(SXMemSurfDesc* surf);
typedef SX_UINT32 (*BSM_UnlockSurf_fxn)(SXMemSurfDesc* surf);

	
class CGfxTitanII : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxTitanII();
	virtual ~CGfxTitanII();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);		
	STDMETHODIMP Close();
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount);
	STDMETHODIMP Unlock(LONG num);
	STDMETHODIMP Flip(LONG num, DWORD dwFlags);
	STDMETHODIMP Show(BOOL bShow);
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetSrcRect(const RECT *pRect);
	STDMETHODIMP SetDstRect(const RECT *pRect);
	STDMETHODIMP SetScnRect(const RECT *pRect);
	STDMETHODIMP GetScnRect(RECT *pRect);

	STDMETHODIMP SetWindowHandle(HWND hwnd);
	STDMETHODIMP GetWindowHandle(HWND& hwnd);
	STDMETHODIMP SetAlpha(DWORD dwAlpha);
	STDMETHODIMP SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey);
	STDMETHODIMP SetDstColorKey(BOOL bEnable, DWORD dwDstCkey);
	STDMETHODIMP GetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP GetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP SetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP SetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP GetDDCaps(LONG*pCaps);
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize);
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP IsOpen();
	STDMETHODIMP GetVRHandle(LPVOID pHandle);
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		m_pVr = reinterpret_cast<VR_HANDLE*>(pHandle);
		if(!m_pVr)
			return E_FAIL;
		return S_OK;
	}
	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf, int mode);
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode);
	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode);
	STDMETHODIMP SetVSYNC(DWORD dwMode);
	STDMETHODIMP GetVSYNC(DWORD* pdwMode);  
	STDMETHODIMP SetDisplayMode(int disp_mode);

protected:

	STDMETHODIMP CreateSurfaces();
	STDMETHODIMP ReleaseSurfaces();
	STDMETHODIMP Update(RECT* pDst = NULL);
	void *GetSurface(LONG num);
	bool LoadRendererDll();
	void UnloadRendererDll();

#define DECLEAR_RENDERER_FXN(fn) fn##_fxn m_##fn;

	DECLEAR_RENDERER_FXN(CVR_CreateVideoRender)
	DECLEAR_RENDERER_FXN(CVR_DeleteVideoRender)
	DECLEAR_RENDERER_FXN(CVR_SetVideoFormat)
	DECLEAR_RENDERER_FXN(CVR_SetDisplayMode)
	DECLEAR_RENDERER_FXN(CVR_SetDstRect)
	DECLEAR_RENDERER_FXN(CVR_Render)
	DECLEAR_RENDERER_FXN(CVR_WaitComplete)
	DECLEAR_RENDERER_FXN(CVR_CreateSurface)
	DECLEAR_RENDERER_FXN(CVR_DestroySurface)
	DECLEAR_RENDERER_FXN(CVR_LockSurface)
	DECLEAR_RENDERER_FXN(CVR_UnlockSurface)
	DECLEAR_RENDERER_FXN(CVR_SetDeinterlaceMode)
	DECLEAR_RENDERER_FXN(CVR_SetRenderLayer)
	DECLEAR_RENDERER_FXN(CVR_GetLastError)
	
	DECLEAR_RENDERER_FXN(BSM_Init)
	DECLEAR_RENDERER_FXN(BSM_DeInit)
	DECLEAR_RENDERER_FXN(BSM_OutputSurf)
	DECLEAR_RENDERER_FXN(CvtToRendererSurf)
	DECLEAR_RENDERER_FXN(BSM_LockSurf)
	DECLEAR_RENDERER_FXN(BSM_UnlockSurf)

	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	RECT				m_rectDst_showbak;
	
	VR_DSTRECT			m_rectBltDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD				m_dwBackBuffers;
	DWORD				m_dwMaxBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	BOOL				m_bUseDSP;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	VR_HANDLE			m_pVr;
    BOOL                m_bBSMMode;
	VR_FRAME_SURFACE	**m_pBackBuffer;
    enum { MAX_BSM_BACK_BUFFER = 1};
    SXMemSurfDesc       m_BSMBackBuffer;
	int					m_iYPitch, m_iUVPitch;
	int					m_sx,m_sy;
	HWND				m_hwnd;	// always 0 for now
	DWORD				m_dwSurfacePad;
	DWORD				m_dwOldWidth,m_dwOldHeight;
	HWND				m_Oldhwnd;
	DWORD 				m_deinterlace_mode;
	bool	m_internal_hnd;
	CRITICAL_SECTION m_csDisplay; 
	HMODULE			m_hRendererDll;
	HMODULE			m_hSurfMgrDll;					//surf mgr dll
	bool m_bOpen;
	SX_REN_SYNC_MODE m_sync;
	VR_DISP_MODE_DESC m_dispmode;

};

#endif // _GFXTITANII_H_
