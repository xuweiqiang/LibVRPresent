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


#ifdef SAMSUNG
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"
#include "Post.h"

#define	  PST_DSP_MODE					1
#define	  PST_DSP_REDATA_MODE			2
#define	  PST_BOTH_MODE				3
#define	  PST_FULLSCN_MODE			4

class CGfxSAMSUNG : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxSAMSUNG();
	virtual ~CGfxSAMSUNG();
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
    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf, int mode)
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
	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow;
	HWND				m_hwnd;
	BOOL m_bOpen; 	
	HANDLE m_cam_hnd;
	BYTE *m_pInputBuff,*m_pOutputBuff;
	POSTINFO m_open_para;
	unsigned char m_uAlpha; //0-0xf
	WIN1SIZE m_sWin1Size;

};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	0
};

CGfxSAMSUNG::CGfxSAMSUNG() : 
	CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	ZeroMemory(&m_open_para, sizeof(m_open_para));
	ZeroMemory(&m_sWin1Size, sizeof(m_sWin1Size));
	m_hwnd = 0;
	m_bOpen = FALSE;
	m_pInputBuff = m_pOutputBuff = NULL;
	m_dwFourCC = 0;
	m_cam_hnd = 0;
	m_uAlpha = 0xf;
}

CGfxSAMSUNG::~CGfxSAMSUNG()
{
	Close();
}

STDMETHODIMP CGfxSAMSUNG::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	UINT32 CamIFState = 0;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)
	{
		DP("[GFXSAMSUNG]Invalid call \n");
		return E_FAIL;
	}
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
	
	DP("[GFXSAMSUNG]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);

	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	if(pDst == NULL)
	{
		m_rectDst.right = 8;
		m_rectDst.bottom = 8;
	}
	else
	{
		m_rectDst.left = pDst->left;
		m_rectDst.top = pDst->top;
		m_rectDst.right = pDst->right;
		m_rectDst.bottom = pDst->bottom;
	}
		
//Create SAMSUNG Surface Start-------------------------------------------------------------
	ZeroMemory(&m_open_para, sizeof(m_open_para));
	m_open_para.nOrgSrcWidth = m_dwWidth;
	m_open_para.nOrgSrcHeight = m_dwHeight;
	m_open_para.nSrcWidth = m_dwWidth;
	m_open_para.nSrcHeight = m_dwHeight;
	m_open_para.nDestWidth = m_rectDst.right - m_rectDst.left;
	m_open_para.nDestHeight = m_rectDst.bottom - m_rectDst.top;
	m_open_para.nSrcStartX = 0;
	m_open_para.nSrcStartY = 0;
	m_open_para.nInputFormat =0;	//YUV:420
#ifndef 	SAMSUNG_2416
	m_open_para.nPositionX = m_rectDst.left;
	m_open_para.nPositionY =m_rectDst.top;
#else
	m_sWin1Size.StartPointX = m_rectDst.left;
	m_sWin1Size.StartPointY = m_rectDst.top;
	m_sWin1Size.Width    = m_open_para.nDestWidth;
	m_sWin1Size.Height   = m_open_para.nDestHeight;
#endif
	m_open_para.nControlstyle = PST_DSP_MODE;

	m_cam_hnd =  CreateFile(TEXT("PST1:"),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
	if(m_cam_hnd ==INVALID_HANDLE_VALUE)
	{
		DP("[GFXSAMSUNG]Open PST1 Fail\n");
		return E_FAIL;
	}

#ifdef SAMSUNG_2416
	if( !ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_CLOSE, 0, NULL, 0, NULL) )
	{
		RETAILMSG(1,(L"$$$$$$ close win1 Failed ! $$$$$$$$$$$$$$$$$$\n"));
	}
#endif


	if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_STOP,   //停止转
			NULL, 0, NULL, 0, NULL, NULL))
	{
		DP("PST CamIF stop failed\n");
	}
	
	if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_GETHWSTATE,
			NULL, 0, &CamIFState, sizeof(DWORD), NULL, NULL))
	{
		DP("PST Get CamIF State false!\r\n");
	//	return E_FAIL;
	}

	if(CamIFState == CAMIF_IDLE_STATE)
	{

#ifdef SAMSUNG_2416
			m_uAlpha = 0xf;
			if(!ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_SETALPHA, sizeof(m_uAlpha),(LPCSTR)&m_uAlpha,0,NULL))
		   {
			   RETAILMSG(1,(L"$$$$$$ SetAlpha Failed ! $$$$$$$$$$$$$$$$$$\n"));
		   }

			if(!ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_SETSIZE, sizeof(m_sWin1Size),(LPCSTR)&m_sWin1Size,0,NULL))
		   {
				RETAILMSG(1,(L"$$$$$$ SetWin1Size Failed ! $$$$$$$$$$$$$$$$$$\n"));	
		   }
#endif

		if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_INIT,
			&m_open_para, sizeof(m_open_para), NULL, 0, NULL, NULL))
		{
			DP("PST IOCTROL FAIL!\r\n");
			return E_FAIL;
		}
	}
	else
	{
		DP("PST CamIF is not idle!\r\n");
		return E_FAIL;
	}

#ifdef SAMSUNG_2416
	m_pInputBuff = (BYTE*)malloc(m_dwWidth* m_dwHeight *4 );
	m_pOutputBuff = (BYTE*)malloc(m_open_para.nDestWidth* m_open_para.nDestHeight*4);
	if((m_pInputBuff == NULL) || (m_pOutputBuff  == NULL))
		return E_FAIL;
	if( !ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_OPEN, 0, NULL, 0, NULL) )
   {
        RETAILMSG(1,(L"$$$$$$ open win1  Failed ! $$$$$$$$$$$$$$$$$$\n"));
   }

#else

	#ifdef SAMSUNG_2450_LEADER_INTL
		m_pInputBuff = (BYTE*)malloc(m_dwWidth* m_dwHeight *4 );
		m_pOutputBuff = (BYTE*)malloc(480*272*4);
		if((m_pInputBuff == NULL) || (m_pOutputBuff  == NULL))
			return E_FAIL;
		if( !ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_OPEN, 0, NULL, 0, NULL) )
	   {
	        RETAILMSG(1,(L"$$$$$$ open win1  Failed ! $$$$$$$$$$$$$$$$$$\n"));
	   }

	#else
		if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_GETINPUTBUFFER,  //get input buffer
				NULL, 0, &m_pInputBuff,sizeof(m_pInputBuff), NULL, NULL))
		{
			DP("get input buffer FAIL!");
			return E_FAIL;
		}

		if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_GETOUTPUTBUFFER,  //get OUTput buffer
				NULL, 0, &m_pOutputBuff,sizeof(m_pOutputBuff), NULL, NULL))
		{
			DP("get output buffer FAIL!");
			return E_FAIL;
		}
	#endif
#endif
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	m_dwFourCC = MAKEFOURCC('Y','V','1','2');
	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::Close()
{

#ifdef SAMSUNG_2416
	if( !ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_CLOSE, 0, NULL, 0, NULL) )
	{
		RETAILMSG(1,(L"$$$$$$ close win1 Failed ! $$$$$$$$$$$$$$$$$$\n"));
	}
#endif

	if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_STOP,   //停止转
			NULL, 0, NULL, 0, NULL, NULL))
	{
		DP("PST IOCTROL FAIL!");
	}

	if(m_pInputBuff)
	{
		free(m_pInputBuff);
		m_pInputBuff = NULL;
	}

	if(m_pOutputBuff)
	{
		free(m_pOutputBuff);
		m_pOutputBuff = NULL;
	}

	CloseHandle(m_cam_hnd);
	m_cam_hnd = NULL;
	
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::SetSrcRect(const RECT *pRect)
{
	if(!pRect)
		return E_FAIL;

	RECT rSrc;
	SetRect(&rSrc, 0, 0,m_dwWidth, m_dwHeight);
	if(!memcmp(&rSrc, pRect, sizeof(rSrc)))
		return S_OK;
	
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG::SetDstRect(const RECT *pRect)
{
	DWORD width = pRect->right - pRect->left;
	DWORD height = pRect->bottom - pRect->top;


#ifdef SAMSUNG_2450
	RECT zeroRect = {0,0,0,0};

	DP("[GFXSAMSUNG]Set destRect (%d,%d, %d, %d) \n", pRect->left, pRect->top, pRect->right, pRect->bottom);
	if(EqualRect(pRect,&m_rectDst) )
		return S_OK;

	if( EqualRect(pRect,&zeroRect))
	{
		m_rectDst.left = m_rectDst.top = 0;
		m_rectDst.right  = m_rectDst.bottom = 8;
	}
	else
		m_rectDst = *pRect;

	IntersectRect(&m_rectDst,&m_rectDst,&m_rectScn);

	DWORD alignx = 8;
	DWORD aligny = 8;
	m_rectDst.left = (m_rectDst.left|(alignx-1)) + 1 - alignx;
	m_rectDst.right = (m_rectDst.right|(alignx-1)) + 1 - alignx;
	m_rectDst.top = (m_rectDst.top|(aligny-1)) + 1 - aligny;
	m_rectDst.bottom = (m_rectDst.bottom|(aligny-1)) + 1 - aligny;

//#ifdef SAMSUNG_2416
//	if( !ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_CLOSE, 0, NULL, 0, NULL) )
//	{
//		RETAILMSG(1,(L"$$$$$$ close win1 Failed ! $$$$$$$$$$$$$$$$$$\n"));
//	}
//#endif

	if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_STOP,   //停止转
			NULL, 0, NULL, 0, NULL, NULL))
	{
		RETAILMSG(1,(TEXT("PST IOCTROL FAIL!")));
	}

	m_open_para.nDestWidth = m_rectDst.right - m_rectDst.left;
	m_open_para.nDestHeight = m_rectDst.bottom - m_rectDst.top;
//#ifndef SAMSUNG_2416
	m_open_para.nPositionX = m_rectDst.left;
	m_open_para.nPositionY =m_rectDst.top;
//#else
//	m_sWin1Size.StartPointX = m_rectDst.left;
//	m_sWin1Size.StartPointY = m_rectDst.top;
//	m_sWin1Size.Width    = m_open_para.nDestWidth;
//	m_sWin1Size.Height   = m_open_para.nDestHeight; //显示的宽度和高度必须是8的倍数,必须与sInMsg.nDestHeight的值一样。
//#endif

	if(EqualRect(&m_rectDst, &m_rectScn))
		m_open_para.nControlstyle = PST_FULLSCN_MODE;
	else
		m_open_para.nControlstyle = PST_DSP_MODE;

//#ifdef SAMSUNG_2416
//		if(!ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_SETSIZE, sizeof(m_sWin1Size),(LPCSTR)&m_sWin1Size,0,NULL))
//	   {
//			RETAILMSG(1,(L"$$$$$$ SetWin1Size Failed ! $$$$$$$$$$$$$$$$$$\n"));	
//	   }
//#endif

	if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_INIT,
		&m_open_para, sizeof(m_open_para), NULL, 0, NULL, NULL))
	{
		DP("PST IOCTROL FAIL!\r\n");
	}
	return S_OK;
#else

	if(EqualRect(pRect,&m_rectDst) || (m_bShow == FALSE))
	{
#ifdef SAMSUNG_2416
		memset(m_pInputBuff, 0x10 ,m_dwWidth* m_dwHeight);
		memset(m_pInputBuff + (m_dwWidth* m_dwHeight), 0x80 ,m_dwWidth* m_dwHeight/2);
		Flip(0, 0);
#endif
		return S_OK;
	}

	if((width == 0) || (height ==0 ))
	{
		SetAlpha(MIN_ALPHA_VAL);
		return S_OK;
	}

#ifdef SAMSUNG_2450_LEADER_INTL
	m_sWin1Size.StartPointX = pRect->left;
	m_sWin1Size.StartPointY = pRect->top;
	m_sWin1Size.Width    = width;
	m_sWin1Size.Height   = height; 

		if(!ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_SETSIZE, sizeof(m_sWin1Size),(LPCSTR)&m_sWin1Size,0,NULL))
	   {
			RETAILMSG(1,(L"$$$$$$ SetWin1Size Failed ! $$$$$$$$$$$$$$$$$$\n"));	
	   }

	m_open_para.nPositionX = pRect->left;
	m_open_para.nPositionY =pRect->top;
	m_open_para.nDestWidth =width;
   	m_open_para.nDestHeight=height;

	DWORD align = 4;
	m_open_para.nPositionX = (m_open_para.nPositionX |(align-1)) + 1 - align;
	m_open_para.nPositionY  = (m_open_para.nPositionY  |(align-1)) + 1 - align;
	m_open_para.nDestWidth = (m_open_para.nDestWidth|(align-1)) + 1 - align;
	m_open_para.nDestHeight= (m_open_para.nDestHeight|(align-1)) + 1 - align;

	if(!DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_INIT,
		&m_open_para, sizeof(m_open_para), NULL, 0, NULL, NULL))
	{
		DP("PST IOCTROL FAIL!\r\n");
	}

		return S_OK;
#endif
#endif
	return E_UNEXPECTED;

}

STDMETHODIMP CGfxSAMSUNG::SetScnRect(const RECT *pRect)
{
	return S_OK;	
}

STDMETHODIMP CGfxSAMSUNG::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::SetAlpha(DWORD dwAlpha)
{
	m_uAlpha = (unsigned char)dwAlpha*0xf/MAX_ALPHA_VAL;
	if(m_uAlpha > 0xf)
		m_uAlpha = 0xf;

	if(!ExtEscape( ::GetDC( NULL ), IOCTL_WIN1_SETALPHA, sizeof(m_uAlpha),(LPCSTR)&m_uAlpha,0,NULL))
	{
	   RETAILMSG(1,(L"$$$$$$ SetAlpha Failed ! $$$$$$$$$$$$$$$$$$\n"));
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
		*ppBuf = m_pInputBuff;
	if(pStride)
		*pStride = m_dwWidth;
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::Flip(LONG num, DWORD dwFlags)
{
	DWORD bDeviceIoControl = 0 ;
	bDeviceIoControl = DeviceIoControl(m_cam_hnd, (DWORD)IOCTL_POST_RUN,  //run the translation
				(BYTE *)m_pInputBuff,0, (BYTE *)m_pOutputBuff,0, NULL, NULL);
	if(!bDeviceIoControl)
	{
		DP("Post run FAIL!\r\n");
	}
	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxSAMSUNG::Update()
{
	if(m_bShow)
		SetAlpha(MAX_ALPHA_VAL);
	else
		SetAlpha(MIN_ALPHA_VAL);

	return S_OK;
}

STDMETHODIMP CGfxSAMSUNG::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSAMSUNG::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxSAMSUNG::IsOpen()
{
	return m_bOpen;
}

IMcGfxOverlay *CreateOverlaySAMSUNG()
{
	return new CGfxSAMSUNG();
}
#endif
