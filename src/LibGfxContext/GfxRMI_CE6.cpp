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

#include "../../inc/GfxBuildDef.h"


#ifdef RMI_CE6
#include <windows.h>
#include "GfxRMI_CE6.h"
#include "dp.h"

CGfxRMICE6::CGfxRMICE6()
{
	m_dwBackBuffers = NUM_BUFFERS;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectDstBak,sizeof(m_rectDstBak));

	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));
	m_hwnd = 0;
	m_dwOldWidth = m_dwOldHeight = 0;
	m_Oldhwnd = 0;
	m_bOpen = FALSE;
}

CGfxRMICE6::~CGfxRMICE6()
{
	Close();
}

STDMETHODIMP CGfxRMICE6::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	HRESULT ret = E_FAIL;
	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen) return E_UNEXPECTED;
		
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));

	SetRect(&m_rectScn,0,0,OS_GetScreenWidth(),OS_GetScreenHeight());

    memset(&m_ovlIoctl,0 ,sizeof(m_ovlIoctl));

  	m_ovlIoctl.bufctrl = 0;
	m_ovlIoctl.flags = 0;
	m_ovlIoctl.reserved0 = 0;
	m_ovlIoctl.reserved1 = 0;
	m_ovlIoctl.winctrl0 = 0;
	m_ovlIoctl.winctrl1 = 0; // Display driver will fill this pipe and priority
	m_ovlIoctl.winctrl2 = WINCTRL2_CKMODE(0);
	m_ovlIoctl.winctrl1 = WINCTRL1_FORM(AU_RGB_565) | WINCTRL1_PO(0) | WINCTRL1_PRI(3);
	m_ovlIoctl.ndx = OVERLAY_INDEX;

	OS_CreateOverlay(&m_ovlIoctl);

	///////////////////////
	//	Open mempool driver
	///////////////////////
	m_hMEM = mem_open_driver();
	if (NULL == m_hMEM)
	{
		RETAILMSG(1, (TEXT("Failed to open mempool driver\r\n")));
	}

  	DWORD dwYUVSize =  m_dwWidth*m_dwHeight* 3/2;
	DWORD  dwRGBSize =  OS_GetScreenWidth()* OS_GetScreenHeight()* 2;


  	for( int i=0; i<NUM_BUFFERS; i++ )
	{
		memYUV[i] = mem_alloc(m_hMEM, dwYUVSize, REGION_ITE, 0);
		memRGB[i] = mem_alloc(m_hMEM, dwRGBSize, REGION_LCD, 0);

		memset(memYUV[i]->pVirtual, dwYUVSize, 0x00);
		memset(memRGB[i]->pVirtual, dwRGBSize, 0x00);
	}


    memset(&m_ovlUpdateIoctl, 0 ,sizeof(m_ovlUpdateIoctl));
    m_ovlUpdateIoctl.ndx = OVERLAY_INDEX;
    m_ovlUpdateIoctl.flags = LCD_UPDATE_IN_VBLANK;

	/////////////////////////////////////////////////////////////////////////////
	// Open hand to UserActivity.  We'll touch this each fram so screen stays on. 
	/////////////////////////////////////////////////////////////////////////////
	m_hPowerManagementEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,TEXT
	    ("PowerManager/ActivityTimer/UserActivity"));

	///////////////////
	//	Open ITE driver
	///////////////////
	m_hITE = maeite_open_driver();
    if(!m_hITE)   return E_FAIL;

	////////////////
	// Configure ITE
	////////////////
	// Coefficients
	memset(&m_mbe_req, 0 ,sizeof(m_mbe_req));
	maeite_set_default_csc(&m_mbe_req);
	maeite_set_default_scf(&m_mbe_req, ITE_MODE_PROGRESSIVE);
	
	// Scaler ratios
	m_mbe_req.scfhsr = maeite_calc_sr(m_dwWidth, m_rectDst.right - m_rectDst.left);
	m_mbe_req.scfvsr = maeite_calc_sr((m_dwHeight), m_rectDst.bottom - m_rectDst.top);
	// Destination registers
	m_mbe_req.dstcfg |= MAEBE_DSTCFG_OF_N(DSTCFG_OF_16BPP);
	m_mbe_req.dstcfg |= (1 << 10);
	m_mbe_req.dstheight = m_rectDst.bottom - m_rectDst.top;
	m_mbe_req.dststr = (m_rectDst.right - m_rectDst.left) *2;
	m_mbe_req.magic = MAEBE_MAGIC;


  	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxRMICE6::Close()
{
	if(!m_bOpen) return E_UNEXPECTED;

	OS_ShowOverlay(m_ovlIoctl.ndx, FALSE);
	OS_DestroyOverlay(m_ovlIoctl);

	for( int i=0; i<NUM_BUFFERS; i++ )
	{
		mem_free(m_hMEM, memYUV[i]);
		mem_free(m_hMEM, memRGB[i]);
	}

	///////////////////////////
	// Close the mempool driver
	///////////////////////////
	if (NULL != m_hMEM)
	{
		mem_close_driver(m_hMEM);
	}

  	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxRMICE6::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	return Update();
}

STDMETHODIMP CGfxRMICE6::SetDstRect(const RECT *pRect)
{
  RECT pInput = *pRect;
	pInput.left &= (~(4-1));
	pInput.top &= (~(4-1));
	pInput.right &= (~(4-1));
	pInput.bottom &= (~(4-1));

	if(EqualRect(&pInput,&m_rectDst))
		return S_OK;

	if(pInput.left == 0 && pInput.top == 0 && pInput.right == 0 &&pInput.bottom == 0 )
		return E_INVALIDARG;

	if(!m_bShow)
	{
		m_rectDstBak = pInput;
		return S_OK;
	}

	
	m_rectDst = pInput;

	DP("setdst %d,%d,%d,%d \n",m_rectDst.left, m_rectDst.top,m_rectDst.right ,m_rectDst.bottom);
	DWORD				outputWidth  = m_rectDst.right - m_rectDst.left;
	DWORD				outputHeight = m_rectDst.bottom - m_rectDst.top;

/*

	m_ovlIoctl.winctrl0 |= (WINCTRL0_OX(m_rectDst.left) | WINCTRL0_OY(m_rectDst.top)); // set the overlay origins -- WINCTRL0_O_MASK
	m_ovlIoctl.winctrl1 |= (WINCTRL1_SZX(outputWidth - 1) | WINCTRL1_SZY(outputHeight - 1)); // set the overlay size -- WINCTRL1_SZ_MASK
	m_ovlIoctl.winctrl2 |= (WINCTRL2_BX(outputWidth*2)); // set the stride (BX) -- WINCTRL2_BX_MASK
	OS_SetOverlayConfig(m_ovlIoctl);
	m_mbe_req.scfhsr = maeite_calc_sr(m_dwWidth, outputWidth);
	m_mbe_req.scfvsr = maeite_calc_sr((m_dwHeight), outputHeight);
	m_mbe_req.dstheight = m_rectDst.bottom - m_rectDst.top;
	m_mbe_req.dststr = (m_rectDst.right - m_rectDst.left) *2;
*/
	CIMSizeChange(m_rectDst.left, m_rectDst.top, m_dwWidth, m_dwHeight, outputWidth, outputHeight, &m_mbe_req);		

	Flip(0,0);
	return S_OK;
}

STDMETHODIMP CGfxRMICE6::SetScnRect(const RECT *pRect)
{
	if(pRect==0)
	{
		m_bScnClip = FALSE;
		return S_OK;
	}
	if(EqualRect(pRect,&m_rectScn))// && m_bScnClip)
		return S_OK;
	m_rectScn = *pRect;
	m_bScnClip = TRUE;
	return Update();
}


STDMETHODIMP CGfxRMICE6::GetScnRect(RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::GetWindowHandle(HWND& hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	if(m_bSrcCkey!=bEnable)
	{
		m_bSrcCkey = bEnable;
		if(bEnable)
			m_dwSrcCkey = dwSrcCkey;
		return Update();
	}
	if(bEnable && m_dwSrcCkey!=dwSrcCkey)
	{
		m_dwSrcCkey = dwSrcCkey;
		return Update();
	}
	return S_OK;
}

STDMETHODIMP CGfxRMICE6::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
  if(num >= m_dwBackBuffers)
    return E_FAIL;

  	if(ppBuf)
	  *ppBuf = memYUV[num]->pVirtual;
	
	if(pStride)
			*pStride = m_dwWidth;

	return S_OK;
}

STDMETHODIMP CGfxRMICE6::Unlock(LONG num)
{
  if(num >= m_dwBackBuffers)
    return E_FAIL;

	return S_OK;
}


STDMETHODIMP CGfxRMICE6::Flip(LONG num, DWORD dwFlags)
{
  if(num >= m_dwBackBuffers)
    return E_FAIL;

  // setup source registers
  maeite_setup_src_regs(&m_mbe_req, FOURCC_IYUV, (UINT32) memYUV[num]->pPhysical, 
  	m_dwWidth, m_dwHeight);

  m_mbe_req.dstaddr = (uint32) memRGB[num]->pPhysical;
  maeite_submit_frame(m_hITE, &m_mbe_req, 0, 0);

  m_ovlUpdateIoctl.phys = memRGB[num]->pPhysical;
  OS_SetNextOverlayBuffer(m_ovlUpdateIoctl);

  // Keep the LCD going.
  SetEvent(m_hPowerManagementEvent);

  OS_ShowOverlay(m_ovlUpdateIoctl.ndx, TRUE);
  return S_OK;
}

STDMETHODIMP CGfxRMICE6::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	if(!bShow)
	{
		m_rectDstBak = m_rectDst;
		RECT zero;
		SetRect(&zero,0,0,4,4);
		SetDstRect(&zero);
		m_bShow = bShow;
	}
	else
	{
		m_bShow = bShow;
		SetDstRect(&m_rectDstBak );
	}

		
	return S_OK;
}

STDMETHODIMP CGfxRMICE6::Update()
{
	return S_OK;
}

STDMETHODIMP CGfxRMICE6::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxRMICE6::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;

	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = MAKEFOURCC('Y','V','1','2');
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxRMICE6::IsOpen()
{
	return E_FAIL;
}

void CGfxRMICE6::CIMSizeChange(DWORD dwX, DWORD dwY, 
	DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwDstWidth, DWORD dwDstHeight, 
	mae_be_request_t	*	mbe_req)
{
	#define BPP 16 // This is set to 16 to remove the 16 lines of VBI data from each field

	OVERLAY_IOCTL ovlIoctl;

	OS_GetOverlayConfig(&ovlIoctl);
	mbe_req->scfvsr = maeite_calc_sr((dwSrcHeight), dwDstHeight);
	mbe_req->scfhsr = maeite_calc_sr(dwSrcWidth, dwDstWidth);
	
	mbe_req->dstheight = dwDstHeight;
	mbe_req->dststr = dwDstWidth * (BPP/8);

	///////////////////////////
	// Initialize an overlay
	///////////////////////////
	ovlIoctl.bufctrl = 0;
	ovlIoctl.flags = 0;
	ovlIoctl.reserved0 = 0;
	ovlIoctl.reserved1 = 0;
	ovlIoctl.winctrl0 = 0;
	ovlIoctl.winctrl1 = 0; // Display driver will fill this pipe and priority
	ovlIoctl.winctrl2 = WINCTRL2_CKMODE(0);
	// by NETL JC, Follow registries should be matched with Display.reg otherwise there is interlace noise in 16bpp mode
	ovlIoctl.winctrl1 = WINCTRL1_FORM(AU_RGB_565) | WINCTRL1_PO(0) | WINCTRL1_PRI(3);
	ovlIoctl.ndx = OVERLAY_INDEX;

	//////////////////////
	// Configure overlay
	//////////////////////
	ovlIoctl.winctrl0 |= (WINCTRL0_OX(dwX) | WINCTRL0_OY(dwY)); // set the overlay origins -- WINCTRL0_O_MASK
	ovlIoctl.winctrl1 |= (WINCTRL1_SZX(dwDstWidth - 1) | WINCTRL1_SZY(dwDstHeight - 1)); // set the overlay size -- WINCTRL1_SZ_MASK
	ovlIoctl.winctrl2 |= (WINCTRL2_BX(2* dwDstWidth)); // set the stride (BX) -- WINCTRL2_BX_MASK

	OS_SetOverlayConfig(ovlIoctl);
}

IMcGfxOverlay *CreateOverlayRMI_CE6()
{
	return new CGfxRMICE6;
}
#endif
