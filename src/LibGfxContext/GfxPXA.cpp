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

// Things to do: (achung)
// 1) Check directdraw surface lost through resolution change - done
// 2) Check multimon support (multiple monitors) - done
// 3) Check overlay failure (creation) - done
// 4) Implement interlacing support
#include "../../inc/GfxBuildDef.h"

#ifdef PXA
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

typedef struct
{
    unsigned long OverlayHeight;
    unsigned long OverlayWidth;
    unsigned long X_Position;
    unsigned long Y_Position;
    unsigned long Format;
    unsigned long DegradeBaseFrame;
    unsigned long CH1;
    unsigned long CH2_Y;
    unsigned long CH3_Cb;
    unsigned long CH4_Cr;
    unsigned long OverlayBPP;
    unsigned long TmpBPP;
    unsigned long ch1_size;
    unsigned long ch2_size;
    unsigned long ch3_size;
    unsigned long ch4_size;
} XLLP_OVERLAY_T, *P_XLLP_OVERLAY_T;

#define ESCAPECODEBASE          100000
#define DISPLAYPAGE             (ESCAPECODEBASE + 1)
#define GETPALETTERAMPHYSICAL   (ESCAPECODEBASE + 2)
#define VERTICALBLANKINTERRUPT  (ESCAPECODEBASE + 3)
#define OS_SCREENACCESS         (ESCAPECODEBASE + 4)
#define SCROLL                  (ESCAPECODEBASE + 5)
#define OVERLAY2_ENABLE         (ESCAPECODEBASE + 6)
#define OVERLAY2_DISABLE        (ESCAPECODEBASE + 7)
#define OVERLAY1_ENABLE         (ESCAPECODEBASE + 8)
#define OVERLAY1_DISABLE        (ESCAPECODEBASE + 9)
#define GET_OVERLAY1_ADDRESS    (ESCAPECODEBASE + 10)
#define GET_OVERLAY2_ADDRESS    (ESCAPECODEBASE + 11)
#define SET_OVERLAY2_K_VALUE    (ESCAPECODEBASE + 12)


#define FORMAT_RGB          0x0
#define FORMAT_PACKED_444   0x1
#define FORMAT_PLANAR_444   0x2
#define FORMAT_PLANAR_422   0x3
#define FORMAT_PLANAR_420   0x4


#define O_BPP_4             0x2
#define O_BPP_8             0x3
#define O_BPP_16            0x4
#define O_BPP_18            0x5
#define O_BPP_18_PACKED     0x6
#define O_BPP_19            0x7
#define O_BPP_19_PACKED     0x8
#define O_BPP_24            0x9
#define O_BPP_25            0xA


class CGfxPXA : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxPXA();
	virtual ~CGfxPXA();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);		
	STDMETHODIMP Close();
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount);
	STDMETHODIMP Unlock(LONG num);
	STDMETHODIMP Flip(LONG num, DWORD dwFlags);
	STDMETHODIMP Show(BOOL bShow);
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags);
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags);
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
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP GetDDCaps(LONG*pCaps)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP IsOpen();
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf)
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


protected:
	STDMETHODIMP Update();
	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow;
	HWND				m_hwnd;
	BOOL m_bOpen; 	

	HDC m_hdc;
	XLLP_OVERLAY_T m_pXllpOverlay2;
	unsigned long m_CH2_Y;
	unsigned long m_CH2_Cb;
	unsigned long m_CH2_Cr;
	unsigned long m_CH2_Y_size;
	unsigned long m_CH2_Cb_size;
	unsigned long m_CH2_Cr_size;
	unsigned long m_BuffSize;
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	0
};

CGfxPXA::CGfxPXA() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	m_hwnd = 0;
	m_bOpen = FALSE;
	m_dwFourCC = 0;

}

CGfxPXA::~CGfxPXA()
{
	Close();
}

STDMETHODIMP CGfxPXA::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	POINT pt;
	UINT32 CamIFState = 0;
	int res = 0;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)
	{
		DP("[GFXSAMSUNG]Invalid call \n");
		return E_FAIL;
	}
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));

	if(pDst && (pDst->right - pDst->left != 0) && (pDst->bottom - pDst->top != 0))
	{
		m_rectDst = *pDst;
	}
	else
	{
		m_rectDst.right = 16;
		m_rectDst.bottom = 16;
	}
	
	m_dwWidth = m_rectDst.right - m_rectDst.left;
	m_dwHeight = m_rectDst.bottom - m_rectDst.top;

	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
	
	DP("[GFXPXA]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);
	pt.x = m_rectDst.left;
	pt.y = m_rectDst.top;

//Create PXA260 Surface Start-------------------------------------------------------------
	m_hdc = GetDC(NULL);

	res = ::ExtEscape(m_hdc,OVERLAY2_DISABLE,0,NULL,0,NULL);//disable

	ZeroMemory(&m_pXllpOverlay2 ,sizeof(m_pXllpOverlay2));
    m_pXllpOverlay2.X_Position = m_rectDst.left;
    m_pXllpOverlay2.Y_Position = m_rectDst.top;
    m_pXllpOverlay2.OverlayWidth = m_dwWidth;
    m_pXllpOverlay2.OverlayHeight = m_dwHeight;


    m_pXllpOverlay2.Format = FORMAT_PLANAR_420;//FORMAT_RGB;
    //pXllpOverlay->DegradeBaseFrame = false;

   res =  ExtEscape(m_hdc,GET_OVERLAY2_ADDRESS,0,NULL,sizeof(XLLP_OVERLAY_T),(char *)&m_pXllpOverlay2);//get address

    m_BuffSize = m_dwWidth * m_dwHeight;

    m_CH2_Y = m_pXllpOverlay2.CH2_Y;
    m_CH2_Cb = m_pXllpOverlay2.CH3_Cb;
    m_CH2_Cr = m_pXllpOverlay2.CH4_Cr;
    m_CH2_Y_size = m_pXllpOverlay2.ch2_size;
    m_CH2_Cb_size = m_pXllpOverlay2.ch3_size;
    m_CH2_Cr_size = m_pXllpOverlay2.ch4_size;

    memset((void *)m_CH2_Y, 0x0, m_BuffSize);
    memset((void *)m_CH2_Cb, 0x80, m_BuffSize/2);
    memset((void *)m_CH2_Cr, 0x80, m_BuffSize/2);

    m_pXllpOverlay2.DegradeBaseFrame = FALSE;
    m_pXllpOverlay2.OverlayBPP = O_BPP_16;
    m_pXllpOverlay2.ch2_size = m_CH2_Y_size;
    m_pXllpOverlay2.ch3_size = m_CH2_Cb_size;
    m_pXllpOverlay2.ch4_size = m_CH2_Cr_size;

#if 0
    res = ExtEscape(m_hdc,OVERLAY2_ENABLE,0,NULL,sizeof(XLLP_OVERLAY_T),(char *)&m_pXllpOverlay2);//enable
 
    if ( res <= 0)
    {
        DP("[GFXPXA]OverLay::OverLay2_Enable failed\n");
        return E_FAIL;
    }
#endif

	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	Show(FALSE);
	m_dwFourCC = MAKEFOURCC('Y','V','1','2');
	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxPXA::Close()
{
	int res = ::ExtEscape(m_hdc,OVERLAY2_DISABLE,0,NULL,0,NULL);//disable

	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxPXA::SetSrcRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxPXA::SetDstRect(const RECT *pRect)
{
	RECT rd;
	DP("[GFXPXA]Set destRect (%d,%d, %d, %d) \n", pRect->left, pRect->top, pRect->right, pRect->bottom);
	if(EqualRect(pRect,&m_rectDst) )
		return S_OK;

	DWORD width = pRect->right - pRect->left;
	DWORD height = pRect->bottom - pRect->top;

	if((width == 0) || (height ==0 ))
		return E_FAIL;

	if((width != m_dwWidth) || (height != m_dwHeight))
		return E_UNEXPECTED;

	rd = *pRect;
	IntersectRect(&rd,&rd,&m_rectScn);

	m_rectDst = rd;

	m_pXllpOverlay2.X_Position = rd.left;
	m_pXllpOverlay2.Y_Position = rd.top;

	int res = ::ExtEscape(m_hdc,OVERLAY2_DISABLE,0,NULL,0,NULL);//disable

	res =  ExtEscape(m_hdc,GET_OVERLAY2_ADDRESS,0,NULL,sizeof(XLLP_OVERLAY_T),(char *)&m_pXllpOverlay2);//get address

 	res = ExtEscape(m_hdc,OVERLAY2_ENABLE,0,NULL,sizeof(XLLP_OVERLAY_T),(char *)&m_pXllpOverlay2);//enable
 

	return S_OK;
}

STDMETHODIMP CGfxPXA::SetScnRect(const RECT *pRect)
{
	return S_OK;	
}

STDMETHODIMP CGfxPXA::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxPXA::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxPXA::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxPXA::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}

STDMETHODIMP CGfxPXA::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxPXA::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxPXA::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxPXA::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxPXA::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxPXA::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxPXA::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
		*ppBuf = (BYTE*)m_CH2_Y;
	if(pStride)
		*pStride = m_dwWidth;
	return S_OK;
}

STDMETHODIMP CGfxPXA::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxPXA::Flip(LONG num, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxPXA::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;

	if(bShow)
		ExtEscape(m_hdc,OVERLAY2_ENABLE,0,NULL,sizeof(XLLP_OVERLAY_T),(char *)&m_pXllpOverlay2);
	else
		ExtEscape(m_hdc,OVERLAY2_DISABLE,0,NULL,0,NULL);

	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxPXA::Update()
{
	return S_OK;
}

STDMETHODIMP CGfxPXA::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxPXA::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxPXA::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if((pInfo==0) ||(pInfo->dwSize!=sizeof(*pInfo)))
		return E_FAIL;

	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0; 
	return S_OK;
}

STDMETHODIMP CGfxPXA::IsOpen()
{
	return m_bOpen;
}

IMcGfxOverlay *CreateOverlayPXA()
{
	return new CGfxPXA();
}
#endif
