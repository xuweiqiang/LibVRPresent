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

#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <windows.h>
#include <yuv2rgb_c.h>

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
        ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
        ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

#include "McCom.h"
#include "GfxPresent.h"



inline int WidthRect(const RECT *r)
{
	return r->right - r->left;
}

inline int HeightRect(const RECT *r)
{
	return r->bottom - r->top;
}



#define MAX_FOURCC_NUMBER	30
enum
{
	HARDWARE_VIDEO_EFFECT,
	SOFTWARE_VIDEO_EFFECT,
};
typedef struct
{
	RECT m_rSrcSize;	
	RECT m_rSrcClip;	
	RECT m_rDst;
	BOOL bOpen;
}VP_HANDLE;

class CVRPresentFramebuffer : 
	public IMcVideoPresenter ,
	public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CVRPresentFramebuffer();
	virtual ~CVRPresentFramebuffer();
	STDMETHODIMP Open(VP_OpenOptions *pOptions, DWORD dwOptionSize);
	STDMETHODIMP Close();
	STDMETHODIMP  CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam);
	STDMETHODIMP  DestroySurface(VR_SURFACE_TYPE type);
	STDMETHODIMP Reset();
	STDMETHODIMP SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk);
	STDMETHODIMP SetVBI(VP_VIDEOBUFFER_INFO *pVBI);
	STDMETHODIMP SetFPS(DWORD dwFPS);
	STDMETHODIMP GetBackBuffers(DWORD *pdwBackBuffers);
	STDMETHODIMP Restore();
	STDMETHODIMP GetStatus(DWORD *pdwStatus);
	STDMETHODIMP GetCaps(VP_CAPS *pCaps);

	STDMETHODIMP Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStride, DWORD dwFlags);
	STDMETHODIMP PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags);
	STDMETHODIMP Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP PresentBuffer(LONG lNum, DWORD dwFlags);

protected:
	STDMETHODIMP FBCleanRect(RECT *r);
	STDMETHODIMP Present_Internal(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP PrepareBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP PresentBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags);
	HRESULT SetSrcSize(DWORD dwWidth, DWORD dwHeight);
	HRESULT UpdateDisplayParameters();
	HRESULT ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin);
	HRESULT InitColorControl();
	HRESULT RestoreColorControl();

	CRITICAL_SECTION m_csDisplay; 

	LONG m_lZoomMode;
	LONG m_lInterpolationMode;
	LONG m_lRotationMode;
	LONG m_lDeblockMode;
	LONG m_lResampleMode;
	LONG m_lRenderEnabled;
	LONG m_alCC[10];
	BOOL m_bZoomEnable;

	VP_VIDEOBUFFER_INFO m_VBI;

	//
	// Video Port information
	//
	RECT m_rDst;		// destination rectangle to store the image, relative to video window/port.
	RECT m_rSrcSize;	// source size (also used as source clipping region) 
	RECT m_rScreen;		// size of currently active screen
	DWORD m_dwStatus;
	DWORD m_dwPresentIndex;	// the current frame to be used by Present

	unsigned int    m_FPS;
	VP_OpenOptions m_OpenOptions;
	LONG m_overlayType;

	int m_fb;
	unsigned char * m_pBuff;
	int m_iBufSize;
	struct fb_var_screeninfo m_fbvinfo;
	struct fb_fix_screeninfo m_fbfinfo;
	
	DWORD surfaceFourCC;
	static const DWORD const_SurfFourCC[];
	static const DWORD const_InputFourCC[];

	BOOL m_bOpen;
	BOOL m_bvpenable;	//video enable or not
};


const DWORD CVRPresentFramebuffer::const_SurfFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('Y','U','Y','2'),
	MAKEFOURCC('R','5','6','5'),
	MAKEFOURCC('R','8','8','8'),
	0
};

#define DEFAULT_INPUT_FOURCC	MAKEFOURCC('R','8','8','8')
const DWORD CVRPresentFramebuffer::const_InputFourCC[] =
{
	MAKEFOURCC('R','8','8','8'),
	0
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('Y','U','Y','2'),
	MAKEFOURCC('R','5','6','5'),
	MAKEFOURCC('R','8','8','8'),
	0
};

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter")
#define HARDWARE_VIDEO_EFFECT_INIT_VALUE	128
#define SOFTWARE_VIDEO_EFFECT_INIT_VALUE	128

CVRPresentFramebuffer::CVRPresentFramebuffer() :
	m_lInterpolationMode(0),
	m_lRotationMode(VP_ROTATION_0),
	m_lDeblockMode(0),
	m_lResampleMode(0),
	m_lRenderEnabled(TRUE),
	m_bZoomEnable(TRUE),
	m_dwStatus(NULL),
	m_dwPresentIndex(0),
	m_overlayType(0),
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_bOpen(FALSE),
	m_pBuff(NULL),
	m_fb(0),
	m_iBufSize(0),
	m_bvpenable(TRUE)
{
	ZeroMemory(&m_rSrcSize,sizeof(m_rSrcSize));
	ZeroMemory(&m_rSrcSize,sizeof(m_rSrcSize));
	ZeroMemory(&m_rDst,sizeof(m_rDst));
	ZeroMemory(&m_rScreen,sizeof(m_rScreen));
	ZeroMemory(&m_alCC,sizeof(m_alCC));
	ZeroMemory(&m_VBI,sizeof(m_VBI));
	ZeroMemory(&m_OpenOptions,sizeof(m_OpenOptions));
	ZeroMemory(&m_fbvinfo,sizeof(m_fbvinfo));

	InitializeCriticalSection(&m_csDisplay);
}

CVRPresentFramebuffer::~CVRPresentFramebuffer()
{
	Close();
	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CVRPresentFramebuffer::Open(VP_OpenOptions *pOptions, DWORD dwOptionSize)
{
	//printf("CVRPresentFramebuffer::Open\n");//fflush(stdout);
	
	if (pOptions == NULL || sizeof(m_OpenOptions) != dwOptionSize)
		return E_FAIL;
	if(pOptions != &m_OpenOptions)
	{
		memset(&m_OpenOptions,0,sizeof(m_OpenOptions));
	}
	memcpy(&m_OpenOptions,pOptions,sizeof(m_OpenOptions));
	
	m_fb = open("/dev/fb0", O_RDWR);
	if(m_fb < 0)
	{
		printf("framebuffer open error\n");
		return E_FAIL;
	}
	
	int ret = -1;
	ret = ioctl(m_fb, FBIOGET_VSCREENINFO, &m_fbvinfo);
	if(ret)
	{
		printf("ioctl(..., FBIOGET_VSCREENINFO, ...) on framebuffer error %x\n", ret);
		return E_FAIL;
	}
	m_iBufSize = m_fbvinfo.xres * m_fbvinfo.bits_per_pixel * m_fbvinfo.yres / 8;
	
	ret = ioctl(m_fb, FBIOGET_FSCREENINFO, &m_fbfinfo);
	if(ret)
	{
		printf("ioctl(..., FBIOGET_FSCREENINFO, ...) on framebuffer error %x\n", ret);
		return E_FAIL;
	}
/*
	printf("m_fbvinfo.bits_per_pixel=%d, m_fbvinfo.xres=%d, m_fbvinfo.yres=%d, m_iBufSize=%d\n", m_fbvinfo.bits_per_pixel, m_fbvinfo.xres, m_fbvinfo.yres, m_iBufSize);
	printf("m_OpenOptions.dwWidth %d, m_OpenOptions.dwHeight %d\n", m_OpenOptions.dwWidth, m_OpenOptions.dwHeight);
*/

	m_pBuff = (unsigned char *)mmap(0, m_iBufSize, PROT_WRITE, MAP_SHARED, m_fb, 0);
	if((void*)-1 == m_pBuff)
	{
		printf("mmap framebuffer error\n");
		return E_FAIL;
	}
	if (yuv2rgb_c_init(1,m_fbvinfo.bits_per_pixel,0,0,1<<16,1<<16) != 0)
	{
		printf("yuv2rgb_c_init error\n");
		return E_FAIL;
	}
	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::Close()
{
	if(!m_bOpen)
		return S_OK;
	
	FBCleanRect(&m_rDst);

	if (m_fb && m_pBuff)
	{
		int ret = -1;
		if(-1 == munmap(m_pBuff, m_iBufSize))
		{
			printf("munmap error\n");
		}
		m_pBuff = NULL;
		m_iBufSize = 0;
	}
	
	if (m_fb)
	{
		close(m_fb);
		m_fb = 0;
	}
	
	yuv2rgb_c_uninit();
	
	m_bOpen = FALSE;
	
	return S_OK;
}

STDMETHODIMP  CVRPresentFramebuffer::CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam)
{
	if(!m_bOpen)
		return E_FAIL;

	if(pparam == NULL)
		return E_FAIL;		
			
	return S_OK;

}

STDMETHODIMP  CVRPresentFramebuffer::DestroySurface(VR_SURFACE_TYPE type)
{
	if(!m_bOpen)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::Reset()
{
	HRESULT res = S_OK;
	res = Close();
	if(FAILED(res))
		return E_FAIL;
	
	res = Open(&m_OpenOptions,sizeof(m_OpenOptions));
	if(FAILED(res))
		return E_FAIL;

	return S_OK;
}

HRESULT CVRPresentFramebuffer::InitColorControl()
{
	return S_OK;
}

HRESULT CVRPresentFramebuffer::RestoreColorControl()
{
	LONG value;

	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentFramebuffer::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{

	switch (dwPropID)
	{
	case MC_PROPID_VP_ZOOM_ENABLE:
		return ReturnComLong(m_bZoomEnable,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ZOOM:
		return ReturnComLong(m_lZoomMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ROTATION:
		return ReturnComLong(m_lRotationMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_INTERPOLATION:
		return ReturnComLong(m_lInterpolationMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_DEBLOCK:
		return ReturnComLong(m_lDeblockMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_RESAMPLE:
		return ReturnComLong(m_lResampleMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ENABLE:
		return ReturnComLong(m_lRenderEnabled,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_TYPE:
		return ReturnComLong(m_overlayType,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_RECTDST:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rDst;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_RECTSRC:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rSrcSize;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_RECT_SCREEN:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rScreen;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			return S_OK;
		}
	case MC_PROPID_VP_COLORSYS_CAP:
		{
			return S_OK;
		}
	case MC_PROPID_VP_WND:
		{
			return S_OK;
		}
	}
	return E_PROP_ID_UNSUPPORTED;
}

static void printrectinfo(RECT r)
{
	printf("^^^^^^^^^^^^^^^^^^^^^^r=%d,%d,%d,%d\n", r.top, r.left, r.right-r.left, r.bottom-r.top);	
	printf("^^^^^^^^^^^^^^^^^^^^^^r=%d,%d,%d,%d\n", r.top, r.left, r.right, r.bottom);	

}

static BOOL comparerect(RECT *r1, RECT *r2)
{
	if(memcmp(r1, r2, sizeof(RECT)))
		return FALSE;
	else
		return TRUE;
}

STDMETHODIMP CVRPresentFramebuffer::FBCleanRect(RECT *r)
{
	if(WidthRect(r)<=0 || HeightRect(r)<=0 ||!m_pBuff )
		return E_FAIL;
	// TODO: multiple with 2? may be changed according to bitsperpixel
	int i;
	unsigned char* p = m_pBuff+m_fbfinfo.line_length*(r->top>0?r->top-1:0)+r->left*2;

	for(i=0;i<HeightRect(r);++i)
	{
		memset(p, 0, WidthRect(r)*2);
		p += m_fbfinfo.line_length;
	}
	
/*
	printf("FBCleanRect m_pBuff %x, m_fbfinfo.line_length %d\n", m_pBuff, m_fbfinfo.line_length);
	printrectinfo(*r);
*/

	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	if(!m_bOpen)
		return E_FAIL;

	BOOL bRefresh = FALSE;

	switch (dwPropID)
	{
	case MC_PROPID_VP_ZOOM_ENABLE:
		{
			LONG lZoomEnable = *(BOOL *)pPropData;
			m_bZoomEnable = lZoomEnable ;
			return S_OK;
		}
	case MC_PROPID_VP_ZOOM:
		{
			LONG lZoomMode = *(LONG *)pPropData;
			return  S_FALSE;
		}
	case MC_PROPID_VP_ROTATION:
		{
			LONG lRotationMode = *(LONG *)pPropData;
			printf("lRotationMode %d\n", lRotationMode);
			return S_FALSE;
		}
	case MC_PROPID_VP_RECTDST:
		{
			RECT screenrect = {0, 0, m_fbvinfo.xres, m_fbvinfo.yres};
			RECT resrect;
			RECT *dstrect = (RECT*)pPropData;
			printrectinfo(*dstrect);
			
			//printf("WidthRect(dstrect) %d, HeightRect(dstrect) %d\n", WidthRect(dstrect), HeightRect(dstrect));
			
			if(WidthRect(dstrect)<=0 
				|| HeightRect(dstrect)<=0 
				||(IntersectRect(&resrect, &screenrect, dstrect) && !comparerect(&resrect, dstrect))
				)
			{
				printf("CVRPresentFramebuffer::Set(MC_PROPID_VP_RECTDST) rect param not correct, babe\n");
				printrectinfo(resrect);
				return S_FALSE;
			}
			FBCleanRect(&m_rDst);
			memcpy(&m_rDst, dstrect, sizeof(m_rDst));
			return S_OK;
		}
	case MC_PROPID_VP_RECTDST_INTERNAL:
		{
			return S_FALSE;
		}
	case MC_PROPID_VP_RECTSRC:
		{
			RECT *pr = reinterpret_cast<RECT *>(pPropData);
			return S_FALSE;
		}
	case MC_PROPID_VP_ENABLE:
		{
			LONG lEnable = *(LONG *)pPropData;
			printf("lEnable %d\n", lEnable);
			m_bvpenable = lEnable;
			return S_FALSE;
		}
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			return S_FALSE;
		}

	case MC_PROPID_VP_RESAMPLE:
		{
			LONG lResampleMode = *(LONG *)pPropData;
			return S_FALSE;
		}
	case MC_PROPID_VP_INTERPOLATION:
		{
			LONG lInterpolationMode = *(LONG *)pPropData;

			m_lInterpolationMode = lInterpolationMode;
			return S_OK;
		}
	case MC_PROPID_VP_DEBLOCK:
		{
			LONG lDeblockMode = *(LONG *)pPropData;

			m_lDeblockMode = lDeblockMode;
			return S_OK;
		}
	case MC_PROPID_VP_COLORKEYDST:
		{
			DWORD dwCkeyDst = *(DWORD *)pPropData;
			return E_FAIL;
		}
	case MC_PROPID_VP_COLORKEYSRC:
		{
			DWORD dwCkeySrc = *(DWORD *)pPropData;
			return E_FAIL;
		}
	case MC_PROPID_VP_WND:
		{
			return E_FAIL;
		}
	case MC_PROPID_VP_ALPHA:
		{
			LONG lAlpha = *(LONG *)pPropData;
			return E_FAIL;
		}
	}
	return E_PROP_ID_UNSUPPORTED;
}

STDMETHODIMP CVRPresentFramebuffer::GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
	DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
	DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	return S_OK;
}

HRESULT CVRPresentFramebuffer::SetSrcSize(DWORD dwWidth, DWORD dwHeight)
{
	RECT r;

	SetRect(&r, 0, 0, dwWidth, dwHeight);
	if(EqualRect(&r,&m_rSrcSize))
		return S_OK;
	m_rSrcSize = r;

	return S_OK;
}

HRESULT CVRPresentFramebuffer::Restore()
{
	if(!m_bOpen)
		return E_FAIL;

	return S_OK;
}

HRESULT CVRPresentFramebuffer::UpdateDisplayParameters()
{
	HRESULT hr = S_OK;
	return hr;
}

STDMETHODIMP CVRPresentFramebuffer::PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return Present(VP_SURF_LAYERTOP,ppbPlanes, pdwStrides,  dwFlags);
}

STDMETHODIMP CVRPresentFramebuffer::PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentFramebuffer::PresentBuffer(LONG lNum, DWORD dwFlags)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentFramebuffer::Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen || !m_pBuff || !m_fb)
		return E_FAIL;

	if(m_fbfinfo.line_length/2 < m_OpenOptions.dwWidth)
	{
		printf("screen size is too small to fit this clip\n");
		return E_FAIL;
	}
	
	int dstStride[1];
	unsigned char * dst[1];
	
	dstStride [0] = m_fbvinfo.xres * m_fbvinfo.bits_per_pixel / 8;
	
	//dst[0] = m_pBuff+2048*(m_fbvinfo.yres-240)-640;
	dst[0] = m_pBuff+m_fbfinfo.line_length*(m_rDst.top>0?m_rDst.top-1:0)+m_rDst.left*2;
	//printf("Present m_pBuff %x, m_fbfinfo.line_length %d\n", m_pBuff, m_fbfinfo.line_length);
	
	if(!m_bvpenable)
		return S_OK;
	
	if (dstStride[0] >= pdwStrides[0])
	{
		if (m_fbvinfo.bits_per_pixel == 24)
		{
			yuv2rgb_c_24_bgr(ppbPlanes,(int *)pdwStrides,0,m_OpenOptions.dwHeight,dst,dstStride,m_OpenOptions.dwWidth);
		}
		else if (m_fbvinfo.bits_per_pixel == 32)
		{
			yuv2rgb_c_32(ppbPlanes,(int *)pdwStrides,0,m_OpenOptions.dwHeight,dst,dstStride,m_OpenOptions.dwWidth);
		}
		else if (m_fbvinfo.bits_per_pixel == 16)
		{
			yuv2rgb_c_16(ppbPlanes,(int *)pdwStrides,0,m_OpenOptions.dwHeight,dst,dstStride,m_OpenOptions.dwWidth);
		}
	}
	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::Present_Internal(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	HRESULT hr = E_FAIL;

	if(!m_bOpen)
		return E_FAIL;

	return hr;
}

HRESULT CVRPresentFramebuffer::ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin)
{
	LONG lSrcNum = lSrcMax - lSrcMin;
	LONG lDstNum = lDstMax - lDstMin;

	if(lSrcVal>lSrcMax)
		lSrcVal = lDstMax;
	else if(lSrcVal < lSrcMin)
		lSrcVal = lDstMin;
	else		
		lSrcVal = lDstMin + (lSrcVal-lSrcMin)*lDstNum/lSrcNum;
	return S_OK;
}


STDMETHODIMP CVRPresentFramebuffer::PrepareBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentFramebuffer::PresentBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::SetVBI(VP_VIDEOBUFFER_INFO *pVBI)
{
	if(!m_bOpen)
		return E_FAIL;

	HRESULT hr;

	memcpy(&m_VBI, pVBI, sizeof(m_VBI));


	m_OpenOptions.dwWidth = m_VBI.dwWidth[0];
	m_OpenOptions.dwHeight = m_VBI.dwHeight[0];
	m_OpenOptions.dwAspectX = m_VBI.dwAspectX;
	m_OpenOptions.dwAspectY = m_VBI.dwAspectY;
	
	return Restore();
}

STDMETHODIMP CVRPresentFramebuffer::SetFPS(DWORD dwFPS)
{
	m_FPS = dwFPS;
	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::GetBackBuffers(DWORD *pdwBackBuffers)
{
	if(!m_bOpen)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::GetStatus(DWORD *pdwStatus)
{
	if(pdwStatus==0)
		return E_POINTER;
	*pdwStatus = m_dwStatus;
	return S_OK;
}

STDMETHODIMP CVRPresentFramebuffer::GetCaps(VP_CAPS *pCaps)
{
	if(!m_bOpen)
		return E_FAIL;

	return S_OK;
}

IMcVideoPresenter *CreateVideoPresenter()
{
	return new CVRPresentFramebuffer();
}
