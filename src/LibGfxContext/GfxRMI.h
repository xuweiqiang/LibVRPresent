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

#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "..\..\3rdparty\RMI\mae_interface.h"
#include "..\..\3rdparty\RMI\maelcd.h"
#include "..\..\3rdparty\RMI\windev.h"

//typedef VR_FRAME_SURFACE *LPVR_FRAME_SURFACE;

typedef struct ContextTime_s 
{
  LONGLONG pts;
  LONGLONG stc;
} ContextTime_t;


typedef struct tagDebugBuffers
{
  OUT int   Status;
  IN  unsigned long Handle;
}DebugBuffers;


class CGfxRMI : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxRMI();
	virtual ~CGfxRMI();
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
	STDMETHODIMP GetDDCaps(LONG*pCaps)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize);
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP IsOpen();
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
	    return E_FAIL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
	    return E_FAIL;
	}
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}



protected:

	STDMETHODIMP Update();
	STDMETHODIMP mae_setdeinterlacemode(DWORD deinterlacemode);
	STDMETHODIMP mae_run(long ts);
	STDMETHODIMP mae_writetimestamp(LONGLONG pts, LONGLONG stc);
	STDMETHODIMP init_mae_driver();
	STDMETHODIMP close_mae_driver();
	STDMETHODIMP open_mae_driver();


	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	RECT				m_rectBltSrc;
	RECT			m_rectBltDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD				m_dwBackBuffers;
	DWORD				m_dwMaxBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	HWND				m_hwnd;	// always 0 for now
	DWORD				m_dwOldWidth,m_dwOldHeight;
	HWND				m_Oldhwnd;


	HANDLE m_filehandle;
	DebugBuffers  m_DB;
	unsigned long m_BytesReturned;
	int m_run; 
};

#endif // _GFXTITANII_H_
