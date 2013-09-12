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

#ifndef _GFXRMI_CE6_H_

#include "GfxOverlay.h"
#include "../../inc/McCom.h"

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define SOC_AU13XX
//#include "camera.h"
//#include "maelcd.h"
#include "lcd_ioctl.h"
#include "mem_ioctl.h"
#include "maeioctl.h"
#include "au1x00.h"

#include "os_api.h"

#define OVERLAY_INDEX MAE_PLANE

class CGfxRMICE6 : public IMcGfxOverlay, public CMcUnknown
{
#define NUM_BUFFERS 2

public:
	DECLARE_IUNKNOWN

	CGfxRMICE6();
	virtual ~CGfxRMICE6();
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

	void CIMSizeChange(DWORD dwX, DWORD dwY, 
		DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwDstWidth, DWORD dwDstHeight, 
		mae_be_request_t	*	mbe_req);

	RECT				m_rectSrc, m_rectDst, m_rectScn;
	RECT				m_rectDstBak;
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


	OVERLAY_IOCTL		m_ovlIoctl;
	OVERLAY_UPDATE_IOCTL	m_ovlUpdateIoctl;

	HANDLE				m_hMEM ;
    HANDLE					m_hPowerManagementEvent;
    HANDLE					m_hITE;
    PMEM_IOCTL			memYUV[NUM_BUFFERS];
    PMEM_IOCTL			memRGB[NUM_BUFFERS];
   	mae_be_request_t		m_mbe_req;

    BOOL m_bOpen;
};

#endif // _GFXTITANII_H_
