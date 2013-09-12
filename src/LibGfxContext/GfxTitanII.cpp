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

#ifdef TITAN_II
#include <windows.h>
#include "GfxTitanII.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

//#pragma comment(lib, "..\\..\\3rdparty\\TitanII\\lib\\BHVidRenderer.lib")
//#pragma comment(lib, "..\\..\\3rdparty\\TitanII\\lib\\BHSurfMgr.lib")

#define ZERO_RECT_WIDTH 	4
#define ZERO_RECT_HEIGHT 	4

#define ALIGNX	16
#define ALIGNY	16

 #define ROOF(v, align)  (((v-1)|((align)-1))+1)
 
CGfxTitanII::CGfxTitanII()
{
	m_pVr = 0;
	m_pBackBuffer = 0;
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));
	m_hwnd = 0;
	m_dwOldWidth = m_dwOldHeight = 0;
	m_Oldhwnd = 0;
	m_sx = 1;
	m_sy = 1;
	m_bUseDSP = TRUE; // use the DSP

	InitializeCriticalSection(&m_csDisplay);
	
	m_deinterlace_mode = VP_DEINTERLACE_WEAVE;
	m_internal_hnd = false;
	m_bOpen = false;
	m_hRendererDll = NULL;
	m_hSurfMgrDll = NULL;
	m_sync = SX_ASYNC_WAITVSYNC;
	memset(&m_dispmode, 0 ,sizeof(m_dispmode));
}

CGfxTitanII::~CGfxTitanII()
{
	Close();
	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CGfxTitanII::GetVRHandle(LPVOID pHandle)
{
	if(pHandle == NULL)
		return E_FAIL;
	*reinterpret_cast<VR_HANDLE*>(pHandle) = m_pVr;  
	return S_OK;
}

STDMETHODIMP CGfxTitanII::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)	return E_FAIL;
	m_bOpen = true;
	m_sync = SX_ASYNC_WAITVSYNC;
	LoadRendererDll();
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_dispmode.smode = VR_SCR_NORMAL;
	m_dispmode.dmode = VR_DISP_FULL;
	m_dispmode.rmode = VR_REN_NORMAL;	
	
	if(!m_pVr)
	{
		if(m_CVR_CreateVideoRender(&m_pVr, NULL) != VR_OK)
			return E_FAIL;
		m_internal_hnd  = true;
	}
	else
		m_internal_hnd  = false;

	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));

	memset(&m_BSMBackBuffer, 0 ,sizeof(m_BSMBackBuffer));;
	if(m_bUseDSP)
	{	// hack because of poor DSP memory allocation
		if(dwWidth*dwHeight >= 704*480)
			m_dwMaxBuffers = min(4,dwBuffers); //dwBuffers;
		else
			m_dwMaxBuffers = min(8,dwBuffers); //dwBuffers;
	}
	else
		m_dwMaxBuffers = dwBuffers;
    if(*pdwFourCC == MAKEFOURCC('P', 'B', 'S', '1'))
    {
        m_bBSMMode = TRUE;
    }
    else
    {
        m_bBSMMode = FALSE;
    }

	m_bShow = FALSE;
	m_bScnClip = TRUE;
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	SetRect(&m_rectDst, 0,0,ZERO_RECT_WIDTH,ZERO_RECT_HEIGHT);
	//m_rectDst = m_rectScn;
	return CreateSurfaces();
}

STDMETHODIMP CGfxTitanII::Close()
{
	if(!m_bOpen)	return E_FAIL;
	m_bOpen = false;

	ReleaseSurfaces();
	if(m_internal_hnd  && m_pVr)
	{
		m_CVR_DeleteVideoRender(m_pVr);
		m_pVr = NULL;
	}
	UnloadRendererDll();

	return S_OK;
}

STDMETHODIMP CGfxTitanII::SetDisplayMode(int disp_mode)
{

	return E_NOTIMPL;
/*
	if(disp_mode == VP_ZOOM_MODE_STRETCH)
		m_dispmode.dmode = VR_DISP_FULL;
	else if(disp_mode == VP_ZOOM_MODE_LETTERBOX)
		m_dispmode.dmode = VR_DISP_NORMAL;
	else
		return E_NOTIMPL;

	return Update();
*/
}

STDMETHODIMP CGfxTitanII::SetHWRenderSurf(LPVOID pSurf, int mode)
{
	if(pSurf == NULL)
		return E_FAIL;

	EnterCriticalSection(&m_csDisplay);
	m_BSMBackBuffer = *(SXMemSurfDesc*)pSurf;
	LeaveCriticalSection(&m_csDisplay);
	return S_OK;	

}

static void clearoutsiderect(unsigned char *pb, int width, int height, int stride, int val, int xl, int xr, int yt, int yb)
{
	int j;

	for(j=0;j<yt;j++)
	{
		memset(pb,val,xl+width+xr);
		pb += stride;
	}
	for(;j<yt+height;j++)
	{
		memset(pb,val,xl);
		memset(pb+xl+width,val,xr);
		pb += stride;
	}
	for(;j<yt+height+yb;j++)
	{
		memset(pb,val,xl+width+xr);
		pb += stride;
	}
}

STDMETHODIMP CGfxTitanII::CreateSurfaces()
{
	HRESULT hr;
	int i;
	DWORD height,width,buffers,hpad,wpad;

	if(m_dwWidth==0 || m_dwHeight==0 /*|| m_hwnd==0*/)
		return E_FAIL;
	if(m_pVr && m_dwWidth==m_dwOldWidth && m_dwHeight==m_dwOldHeight && m_hwnd==m_Oldhwnd)
		return S_OK;

	if(m_bBSMMode == TRUE)
	{
		m_BSM_Init(m_pVr);
	}

	m_dwOldWidth = m_dwWidth;
	m_dwOldHeight = m_dwHeight;

	m_Oldhwnd = m_hwnd;

	//VR_LAYER_OVERLAY:VR_LAYER_PRIMARY
#if defined(SIRF_NOTEBOOK	) || defined(RTK_TITAN_II)
	hr = m_CVR_SetRenderLayer(m_pVr, VR_LAYER_PRIMARY,0.0f);
#else
	hr = m_CVR_SetRenderLayer(m_pVr, VR_LAYER_OVERLAY,0.0f);
#endif
	//[optional] it will change settings in renderer. 
	//TITAN_BITMAP_ORDER: this is the set the bitmap data order 
	//inputted. 
	//pInBuf: the pointer of input data block  
	//data_size: the size of input data block 
	//pOutBuf: the pointer of output data block. 
	//CVR_EscapeCtrl(m_pVr, TITAN_BITMAP_ORDER, pInBuf, data_size, 	pOutBuf); 
	  
	//[optional] it will set the display mode 
	//mode: the display mode to be set 
	hr = m_CVR_SetDisplayMode(m_pVr, m_dispmode); 
  
	//if(m_dwHeight<480)
	//	m_sy = 2;		// double the height!
	//else
		m_sy = 1;
	//if(m_bUseDSP)
	//{	// DSP renderer
	//	alignx = 16;	// 16 is required (spec doesn't specify)
	//	aligny = 4;
	//	hr = CVR_EscapeCtrl(m_pVr,TITAN_DSP_RENDERER,0,0,0);
	//	if(hr!=VR_OK)
	//	{
	//		// do something here.
	//	}
	//}
	//else
	{
//		alignx = 2; /* actually 2, but we prefer this for our averaging ops */
//		aligny = 2;

	}
	buffers = m_dwMaxBuffers+1;
	wpad = ~(m_dwWidth-1) & (ALIGNX-1);
	width = m_dwWidth+wpad;
	height = m_dwHeight*m_sy;
	hpad = ~(height-1) & (ALIGNY-1);
	height = height+hpad;
	//width = width + (width&0xf);
	//height = height + (height&0xf);
	//SetDisplayMode(1);


	m_iYPitch = width;
	m_iUVPitch = m_iYPitch>>1;
	//hr = CVR_SetVideoFormat(m_pVr, 0, width, height, 0, &m_iYPitch, &m_iUVPitch);
		//Set the Video parameters. 
	//VR_FORMAT_I420: set the input data format as I420  
	//width: width of source frame 
	//height: height of source frame 
	//rect: the source rect 
	//LumPitch: pitch of lum 
	//ChromPitch: pitch of chrom 

	width = ROOF(m_dwWidth, ALIGNX);

	if(m_bBSMMode == TRUE)
		height = ROOF(m_dwHeight, ALIGNY);
	else
		height = m_dwHeight;

	VR_SRCRECT rect;
	rect.left= 0;
	rect.right = m_dwWidth;
	rect.top = 0;
	rect.bottom= m_dwHeight;

	hr = m_CVR_SetVideoFormat(m_pVr, VR_FORMAT_I420, width, height, &rect,  &m_iYPitch, &m_iUVPitch);

	SetDeinterlaceMode(m_deinterlace_mode);

    if(!m_bBSMMode)
    {
	    m_pBackBuffer = new LPVR_FRAME_SURFACE[buffers];
	    ZeroMemory(m_pBackBuffer,sizeof(m_pBackBuffer[0])*buffers);

	    for(i=0;i<(signed)buffers;i++)
	    {
		    hr = m_CVR_CreateSurface(m_pVr,&m_pBackBuffer[i],1);
		    if(hr!=VR_OK)
			    break;
	    }
	    if(i==0)
	    {
		    delete[] m_pBackBuffer;
		    m_pBackBuffer = 0;
		    //CVR_DeleteVideoRender(m_pVr);
		    //m_pVr = 0;
		    return E_FAIL;
	    }

	    if(i>1)
	    {		// make sure we have at least one surface available for background scratch, otherwise hang.
		    m_CVR_DestroySurface(m_pVr,m_pBackBuffer[--i]);
		    ZeroMemory(&m_pBackBuffer[i],sizeof(m_pBackBuffer[i]));
	    }

	    m_dwBackBuffers = i;

	    // clear out buffers
	    unsigned char *pb;
	    LONG lstride;
	    int xl,xr,yt,yb,ht;

	    yt = hpad>>1&~1;
	    yb = hpad - yt;
	    xl = 0; // (wpad>>1)&~3;
	    xr = wpad -xl;
	    ht = m_dwHeight*m_sy;

	    for(i=0;i<(signed)m_dwBackBuffers;i++)
	    {
		    if(SUCCEEDED(Lock(i, (LPVOID *)&pb, &lstride, 0)))
		    {
			    clearoutsiderect(pb,m_dwWidth,ht,lstride,0,xl,xr,yt,yb);
			    clearoutsiderect(pb+height*width,m_dwWidth>>1,ht>>1,lstride>>1,128,xl>>1,xr>>1,yt>>1,yb>>1);
			    clearoutsiderect(pb+height*width+(height*width>>2),m_dwWidth>>1,ht>>1,lstride>>1,128,xl>>1,xr>>1,yt>>1,yb>>1);
			    Unlock(i);
		    }
	    }
    }
	// how many back buffers able to work under CVR back buffer mode?
	// Ryan 08-10-14
    else
    {
	    m_dwBackBuffers = MAX_BSM_BACK_BUFFER;
    }
	
	return S_OK;
}

STDMETHODIMP CGfxTitanII::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	return Update();
}

STDMETHODIMP CGfxTitanII::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
	m_rectDst = *pRect;
	HRESULT res = Update(&m_rectDst);
	if(FAILED(res))
		return res;
	if(!(m_rectDst.left == 0 && m_rectDst.top == 0 && m_rectDst.right ==0 && m_rectDst.bottom == 0))
		Flip(0,0);
	return S_OK;
}

STDMETHODIMP CGfxTitanII::SetScnRect(const RECT *pRect)
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

STDMETHODIMP CGfxTitanII::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}


STDMETHODIMP CGfxTitanII::SetWindowHandle(HWND hwnd)
{
#if 0
	if(m_hwnd!=hwnd)
	{
		m_hwnd = hwnd;
		Update();
	}
#endif
	return S_OK;
}

STDMETHODIMP CGfxTitanII::GetWindowHandle(HWND& hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
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

STDMETHODIMP CGfxTitanII::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

void *CGfxTitanII::GetSurface(LONG num)
{	
	if(m_bBSMMode)
		return &m_BSMBackBuffer;
	
	if(num>=0 && num<(signed)m_dwBackBuffers)
    {
	    return m_pBackBuffer[num];
    }
	return 0;
}

STDMETHODIMP CGfxTitanII::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	void *pSurf;

	EnterCriticalSection(&m_csDisplay);
	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
    if(!m_bBSMMode)
    {
	    VR_RESULT hr = m_CVR_LockSurface(m_pVr, (VR_FRAME_SURFACE*)pSurf);
	    if(hr!=VR_OK)
		    return E_FAIL;
	    if(ppBuf)
		    *ppBuf = ((VR_FRAME_SURFACE*)pSurf)->pFrameBuf;
	    if(pStride)
		    *pStride = ((VR_FRAME_SURFACE*)pSurf)->Y_stride;
	    if(pPixCount)
		    *pPixCount = 12;
    }
    else
    {
	    *ppBuf = pSurf;
    }
	return S_OK;
}

STDMETHODIMP CGfxTitanII::Unlock(LONG num)
{
	HRESULT hr = VR_OK;
    if(!m_bBSMMode) 
    {
	    void *pSurf;

    pSurf = GetSurface(num);
    if(pSurf==0)
	    return E_FAIL;

	    hr = m_CVR_UnlockSurface(m_pVr, (VR_FRAME_SURFACE*)pSurf);
    }

	LeaveCriticalSection(&m_csDisplay);
	return hr==VR_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CGfxTitanII::Flip(LONG num, DWORD dwFlags)
{
	void *pSurf;
	HRESULT hr = S_OK;
	//static DWORD tick = GetTickCount();
	//DP("Flip:%d \n", GetTickCount() - tick);
	//tick = GetTickCount();
	//return S_OK;

	EnterCriticalSection(&m_csDisplay);
	pSurf = GetSurface(num);
	if(pSurf==0)
	{
		LeaveCriticalSection(&m_csDisplay);
		return E_FAIL;
	}

    if(!m_bBSMMode)
    {
		VR_FRAME_SURFACE* pSurfFrame = (VR_FRAME_SURFACE*)pSurf; 
		if(pSurfFrame->width == 0)	
		{
			LeaveCriticalSection(&m_csDisplay);
			return S_OK;
		}

		m_CVR_WaitComplete(m_pVr);
	#ifdef ZHONGHONG
	    hr = m_CVR_Render(m_pVr, pSurfFrame,VR_SYNC_WAITVSYNC/*VR_ASYNC_WAITVSYNC*/);
	#else
	    hr = m_CVR_Render(m_pVr, pSurfFrame,VR_SYNC_WAITVSYNC/*VR_ASYNC_WAITVSYNC*/);
	#endif
    }
    else
    {
		SXMemSurfDesc* pSurfFrame = (SXMemSurfDesc*)pSurf; 
		if(pSurfFrame->width == 0)
		{
			LeaveCriticalSection(&m_csDisplay);
			return S_OK;
		}
		//SHOW/HIDE freqentyly may cause hang up here.BY GOLAN
		//m_CVR_WaitComplete(m_pVr);
	#ifdef ZHONGHONG
	    m_BSM_OutputSurf(pSurfFrame, SX_SYNC_WAITVSYNC);
	#else
	    m_BSM_OutputSurf(pSurfFrame, m_sync);
//IT'S by default.		m_BSM_OutputSurf(pSurfFrame, SX_ASYNC_WAITVSYNC);
	#endif
    }

	if(FAILED(hr))
		DP("[GFXTITIAN II]error: 0x%x\n",m_CVR_GetLastError(m_pVr));

	LeaveCriticalSection(&m_csDisplay);

	return hr==VR_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CGfxTitanII::Show(BOOL bShow)
{
	HRESULT ret = E_FAIL;
	if(((bool)(m_bShow))==((bool)(bShow)))
		return S_OK;

	if(bShow)
	{
	#if defined(SIRF_NOTEBOOK) || defined(RTK_TITAN_II)	
		m_CVR_SetRenderLayer(m_pVr, VR_LAYER_PRIMARY,1.0f);
	#else
		m_CVR_SetRenderLayer(m_pVr, VR_LAYER_OVERLAY,1.0f);
	#endif
	}
	else
		m_CVR_SetRenderLayer(m_pVr, VR_LAYER_OVERLAY,0.0f);

	m_bShow = (bool)bShow;

	Flip(0,0);


	return ret;
}

STDMETHODIMP CGfxTitanII::Update(RECT* pDst)
{
	HRESULT hr = S_OK;
	RECT rs, rd;

	SetRect(&rs,0,0,m_dwWidth,m_dwHeight);
	IntersectRect(&rs,&rs,&m_rectSrc);
	if(pDst)
		rd = *pDst;
	else
		rd = m_rectDst;
	// handle alignment
#if 0
	if(m_Caps.dwAlignSizeDest>1)
	{
		rd.right &= ~(m_Caps.dwAlignSizeDest-1);
		rd.left &= ~(m_Caps.dwAlignSizeDest-1);
	}
	if(m_bScnClip)
	{
		IntersectRect(&rd,&rd,&m_rectScn);
		if(!EqualRect(&rd,&m_rectDst))
		{				// translate the clipped destination rectangle to clipping for source rectangle
			RECT rn;	// normalization 
			int wd,ws,hd,hs;

			rn = rd;
			OffsetRect(&rn,-m_rectDst.left,-m_rectDst.top);
			wd = m_rectDst.right-m_rectDst.left;
			hd = m_rectDst.bottom-m_rectDst.top;
			ws = rs.right-rs.left;
			hs = rs.bottom-rs.top;
			if(wd<=0 || hd<=0)
				SetRect(&rs,0,0,0,0);
			else
			{
				rn.left = rn.left*ws/wd;
				rn.right = rn.right*ws/wd;
				rn.top = rn.top*hs/hd;
				rn.bottom = rn.bottom*hs/hd;
				OffsetRect(&rn,rs.left,rs.top);
				IntersectRect(&rs,&rs,&rn);
			}
		}
	}
#endif
	

	if(m_rectScn.right>0 && m_rectScn.bottom>0 && !IsRectEmpty(&rd))
	{
		m_rectBltDst.left = (float)rd.left/m_rectScn.right;
		m_rectBltDst.right = (float)rd.right/m_rectScn.right;
		m_rectBltDst.top = (float)rd.top/m_rectScn.bottom;
		m_rectBltDst.bottom = (float)rd.bottom/m_rectScn.bottom;
	}
	else
	{
		m_rectBltDst.left = (float)0;
		m_rectBltDst.right = (float)1.;
		m_rectBltDst.top = (float)0;
		m_rectBltDst.bottom = (float)1.;
	}

	if(m_rectBltDst.bottom > 1.0f)
		m_rectBltDst.bottom = 1.0f;

	if(m_rectBltDst.right > 1.0f)
		m_rectBltDst.right = 1.0f;

	hr = m_CVR_SetDstRect(m_pVr,m_rectBltDst);
	if(FAILED(hr))
		DP("[GFXTITIAN II]error: 0x%x\n",m_CVR_GetLastError(m_pVr));

	if((rd.right == ZERO_RECT_WIDTH) && (rd.bottom == ZERO_RECT_HEIGHT))
	{
		hr = Flip(0,0);
		if(FAILED(hr))
		{
			DP("[GFXTITIAN II]error: 0x%x\n",m_CVR_GetLastError(m_pVr));
			return hr;		
		}
	}

	DWORD width, height;

	rs.left &= ~(ALIGNX-1);
	rs.right &= ~(ALIGNX-1);
	rs.top &= ~(ALIGNY-1);
	rs.bottom &= ~(ALIGNY-1);

	width = ROOF(m_dwWidth, ALIGNX);
	if(m_bBSMMode == TRUE)
		height = ROOF(m_dwHeight, ALIGNY);
	else
		height  = m_dwHeight;

	VR_SRCRECT srcRect;
	memset(&srcRect, 0 ,sizeof(srcRect));
	srcRect.left = rs.left;
	srcRect.right = rs.right;
	srcRect.top = rs.top;
	srcRect.bottom = rs.bottom;

	hr = m_CVR_SetVideoFormat(m_pVr, VR_FORMAT_I420, width, height, &srcRect,  &m_iYPitch, &m_iUVPitch);
	if(FAILED(hr))
		DP("[GFXTITIAN II]error: 0x%x\n",m_CVR_GetLastError(m_pVr));
/*
	#if defined(SIRF_NOTEBOOK) || defined(RTK_TITAN_II)	
	hr = m_CVR_SetRenderLayer(m_pVr, VR_LAYER_PRIMARY,1.0f);
	#else
	hr = m_CVR_SetRenderLayer(m_pVr, VR_LAYER_OVERLAY,1.0f);
	#endif
	if(FAILED(hr))
		DP("[GFXTITIAN II]error: 0x%x\n",m_CVR_GetLastError(m_pVr));
*/

	/*
	hr = m_CVR_SetDisplayMode(m_pVr, m_dispmode); 
	if(FAILED(hr))
		DP("[GFXTITIAN II]error: 0x%x\n",m_CVR_GetLastError(m_pVr));
	*/
//	m_bUpdateBltDst = TRUE;
//	if(m_bLastShow==FALSE && m_bShow==FALSE)
//		return S_OK;
//	m_bLastShow = m_bShow;
	return SUCCEEDED(hr) ? S_OK : E_FAIL;
}

STDMETHODIMP CGfxTitanII::ReleaseSurfaces()
{
    HRESULT hr;

	if(m_bBSMMode == FALSE)
	{
		if(m_pBackBuffer)
		{
			int i;
		
			for(i=0;i<(signed)m_dwBackBuffers;i++)
			{
				hr = m_CVR_DestroySurface(m_pVr,m_pBackBuffer[i]);
			}
			delete[] m_pBackBuffer;
			m_pBackBuffer = 0;
		}

	}
	else
		m_BSM_DeInit();
	//if(m_pVr)
	//{
	//	CVR_DeleteVideoRender(m_pVr);
	//	m_pVr = 0;
	//}

	m_dwBackBuffers = 0;
    m_dwOldWidth = 0;
    m_dwOldHeight = 0;
	return S_OK;
}


STDMETHODIMP CGfxTitanII::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;

//	if(pCaps)
//		*pCaps = GFXOVERLAY_CAPS_ZOOM_SUPPORT;
//	return S_OK;
}

STDMETHODIMP CGfxTitanII::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if((m_pBackBuffer==0 && !m_bBSMMode) || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
    if(m_bBSMMode)
    {
        pInfo->dwFourCC = MAKEFOURCC('P','B','S','1');
    }
    else
    {
	    if(m_sy==2)
		    pInfo->dwFourCC = MAKEFOURCC('Y','U','2','4');
	    else
		    pInfo->dwFourCC = MAKEFOURCC('Y','V','1','2');
    }
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxTitanII::IsOpen()
{
	return m_pBackBuffer ? S_OK : S_FALSE;
}

STDMETHODIMP CGfxTitanII::SetDeinterlaceMode(DWORD dwMode)
{
	if(!m_pVr)
		return E_FAIL;

	VR_DEINTERLACE_MODE  mode;
	switch(dwMode)
	{
		case VP_DEINTERLACE_BOB_SINGLE_FIELD:
			mode = VR_DEINTERLACE_FIELD_EVEN;
			break;
		case VP_DEINTERLACE_BOB_DOUBLE_FIELD:
			mode = VR_DEINTERLACE_FIELD_ODD;
			break;
		case VP_DEINTERLACE_WEAVE:
			mode = VR_DEINTERLACE_PROGRESSIVE ;
			break;
		default:
			return E_FAIL;
	}
	DP("SetdeinterlaceMode to %d \n", dwMode);
	m_CVR_SetDeinterlaceMode(m_pVr, mode);
	m_deinterlace_mode = dwMode;
	return S_OK;
}

STDMETHODIMP CGfxTitanII::GetDeinterlaceMode(DWORD* pdwMode)
{
	if(!m_pVr || pdwMode == NULL)
		return E_FAIL;
	*pdwMode = m_deinterlace_mode;
	return S_OK;
}


#define LOAD_RENDERER_FXN(dll, fn) \
do {\
	m_##fn = (fn##_fxn)GetProcAddress(dll, L###fn);\
	if (!m_##fn)\
	{\
		RETAILMSG(1, (L"LoadRendererDll: function %s missed\r\n", L###fn));\
		return false;\
	}\
} while (0);



STDMETHODIMP CGfxTitanII::SetVSYNC(DWORD dwMode) 
{
	switch(dwMode)
	{
		case VP_VSYNC_MODE_ASYNC:
			DP("Set SYNC mode: VP_VSYNC_MODE_ASYNC \n");
			m_sync =SX_ASYNC_WAITVSYNC ;
			break;
		case VP_VSYNC_MODE_NOWAIT:
			DP("Set SYNC mode: SX_NOWAIT_VSYNC \n");
			m_sync =SX_NOWAIT_VSYNC;
			break;
		case VP_VSYNC_MODE_SYNC:
			DP("Set SYNC mode: VP_VSYNC_MODE_SYNC \n");
			m_sync =SX_SYNC_WAITVSYNC ;
			break;
		default:
			return E_INVALIDARG;
		
	}
	
	return S_OK;
}

STDMETHODIMP CGfxTitanII::GetVSYNC(DWORD* pdwMode) 
{
	if(!pdwMode)	return E_INVALIDARG;
	*pdwMode = m_sync ;
	return S_OK;
}

bool CGfxTitanII::LoadRendererDll()
{
	DP("Before m_hRendererDll:%x m_hSurfMgrDll :%x\n", m_hRendererDll,m_hSurfMgrDll );
	if (!m_hRendererDll)
	{
		m_hRendererDll = LoadLibrary(L"BHVidRenderer.dll");
		if (!m_hRendererDll)
		{
			RETAILMSG(1, (L"Load BHVidRenderer.dll Failed"));
			return false;
		}
		m_hSurfMgrDll = LoadLibrary(L"BHSurfMgr.dll");
		DP("m_hRendererDll:%x m_hSurfMgrDll :%x\n", m_hRendererDll,m_hSurfMgrDll );
		if (!m_hSurfMgrDll)
		{
			RETAILMSG(1, (L"Load BHSurfMgr.dll Failed"));
			FreeLibrary(m_hRendererDll);
			m_hRendererDll = NULL;
			return false;

		}
	}

	LOAD_RENDERER_FXN(m_hRendererDll, CVR_CreateVideoRender)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_DeleteVideoRender)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetVideoFormat)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetDisplayMode)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetDstRect)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_Render)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_WaitComplete)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_CreateSurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_DestroySurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_LockSurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_UnlockSurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetDeinterlaceMode)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetRenderLayer)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_Init)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_DeInit)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_OutputSurf)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, CvtToRendererSurf)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_LockSurf)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_UnlockSurf)

	return true;
	

}

#define UNLOAD_RENDERER_FXN(fn)  m_##fn = NULL;

void CGfxTitanII::UnloadRendererDll()
{
	UNLOAD_RENDERER_FXN(CVR_CreateVideoRender)
	UNLOAD_RENDERER_FXN(CVR_DeleteVideoRender)
	UNLOAD_RENDERER_FXN(CVR_SetVideoFormat)
	UNLOAD_RENDERER_FXN(CVR_SetDisplayMode)
	UNLOAD_RENDERER_FXN(CVR_SetDstRect)
	UNLOAD_RENDERER_FXN(CVR_Render)
	UNLOAD_RENDERER_FXN(CVR_WaitComplete)
	UNLOAD_RENDERER_FXN(CVR_CreateSurface)
	UNLOAD_RENDERER_FXN(CVR_DestroySurface)
	UNLOAD_RENDERER_FXN(CVR_LockSurface)
	UNLOAD_RENDERER_FXN(CVR_UnlockSurface)
	UNLOAD_RENDERER_FXN(CVR_SetDeinterlaceMode)
	UNLOAD_RENDERER_FXN(CVR_SetRenderLayer)
	UNLOAD_RENDERER_FXN(BSM_Init)
	UNLOAD_RENDERER_FXN(BSM_DeInit)
	UNLOAD_RENDERER_FXN(BSM_OutputSurf)
	UNLOAD_RENDERER_FXN(CvtToRendererSurf)
	UNLOAD_RENDERER_FXN(BSM_LockSurf)
	UNLOAD_RENDERER_FXN(BSM_UnlockSurf)
	if (m_hRendererDll)
	{
		FreeLibrary(m_hRendererDll);
		m_hRendererDll = NULL;
		FreeLibrary(m_hSurfMgrDll);
		m_hSurfMgrDll = NULL;
	}

	
}

IMcGfxOverlay *CreateOverlayTitanII()
{
	return new CGfxTitanII;
}

#endif
