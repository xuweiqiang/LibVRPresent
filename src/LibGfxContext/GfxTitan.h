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

#ifndef _GFXTITAN_H_

#include "GfxOverlay.h"
#include "TitanRenderSDK.h"
#include "../../inc/McCom.h"

typedef VR_FRAME_SURFACE *LPVR_FRAME_SURFACE;

class CGfxTitan : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxTitan();
	virtual ~CGfxTitan();
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
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetScnRect(RECT *pRect);
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  

protected:

	STDMETHODIMP CreateSurfaces();
	STDMETHODIMP ReleaseSurfaces();
	STDMETHODIMP Update();
	VR_FRAME_SURFACE *GetSurface(LONG num);
	STDMETHODIMP SetDisplayMode(int disp_mode);


	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	VR_RECT				m_rectBltSrc;
	VR_DSTRECT			m_rectBltDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD				m_dwBackBuffers;
	DWORD				m_dwMaxBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	BOOL				m_bUseDSP;
	BOOL				m_bUpdateBltDst;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	VR_HANDLE			m_pVr;
	VR_FRAME_SURFACE	**m_pBackBuffer;
	int					m_iYPitch, m_iUVPitch;
	int					m_sx,m_sy;
	HWND				m_hwnd;	// always 0 for now
	DWORD				m_dwSurfacePad;
	DWORD				m_dwOldWidth,m_dwOldHeight;
	HWND				m_Oldhwnd;
};

#endif // _GFXTITAN_H_
