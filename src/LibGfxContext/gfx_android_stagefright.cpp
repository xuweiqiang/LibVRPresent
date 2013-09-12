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

#ifdef GFX_ANDROID_STAGEFRIGHT
#include <stdio.h>
#include <windows.h>

#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

#include <fcntl.h>
#include <dlfcn.h>
#include <ISurface.h>
#include <MemoryHeapBase.h>
#include <SoftwareRenderer.h>
#include <VideoRenderer.h>

#include <AndroidRuntime.h>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
  ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
  ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)
using namespace android;

typedef int PixelFormat_t;


#if 0
#include <media/stagefright/OMXClient.h>
#include <OMXRenderer.h>


class CMyStageFrightOMXRenderer 
{
public:
	CMyStageFrightOMXRenderer(void)
	{
	m_omxClient.connect();
	}
	CMyStageFrightOMXRenderer(void)
	{
	m_omxClient.disconnect();
	}

	void Init(const sp<ISurface>& surface)
  {
	m_surface = surface;
  }
	bool SetVideoFormat(int videoWidth, int videoHeight, int dispWidth, int dispHeight, PixelFormat_t format)
	{
	m_renderer = m_omxClient.interface()->createRenderer(m_surface, "OMX.VisualOn.Video.Sink",
                        (OMX_COLOR_FORMATTYPE)format,
                        videoWidth, videoHeight,
                        dispWidth, dispHeight);
	return (m_renderer != NULL) ? true : false;
	}
	void Start(){return;}
	void Stop(){return;}
	void SendData(unsigned char * data, unsigned int dataBytes)
	{
  OMX_BUFFERHEADERTYPE omxBufHdr;
  memset(&omxBufHdr, 0, sizeof(OMX_BUFFERHEADERTYPE));
  omxBufHdr.pBuffer = data;
  omxBufHdr.nFilledLen = dataBytes;
  DP("SendData - omxBufHdr %p, data %p, len %d", &omxBufHdr, omxBufHdr.pBuffer, omxBufHdr.nFilledLen);

  IOMXRenderer * renderer = m_renderer.get();
  DP("SendData - renderer %p", renderer);
  renderer->render((IOMX::buffer_id)&omxBufHdr);
  }


private:
	OMXClient m_omxClient;
	sp<ISurface> m_surface;
	sp<IOMXRenderer> m_renderer;
};

#endif

class CMyStageFrightLocalRenderer 
{
public:
  CMyStageFrightLocalRenderer(void);
  ~CMyStageFrightLocalRenderer(void){
    if(m_renderer)
      delete m_renderer;
    return ;}

  void Init(const sp<ISurface>& surface);
  bool SetVideoFormat(int videoWidth, int videoHeight, int dispWidth, int dispHeight, PixelFormat_t format);
  
  void Start(){return;}
  void Stop(){return;}
  void SendData(unsigned char * data, unsigned int dataBytes);
  
private:
  VideoRenderer * m_renderer;
  sp<ISurface> m_surface;
  void * m_libHandle;
};


CMyStageFrightLocalRenderer::CMyStageFrightLocalRenderer(void)
{
  m_renderer = NULL;
}

void CMyStageFrightLocalRenderer::Init(const sp<ISurface>& surface)
{
  m_surface = surface;
}

bool CMyStageFrightLocalRenderer::SetVideoFormat(int videoWidth, int videoHeight, int dispWidth, int dispHeight, PixelFormat_t format)
{
  m_libHandle = dlopen("/system/lib/libstagefrighthw.so", RTLD_NOW);
  if (m_libHandle) 
  {
    DP("m_libHandle find !\n");
    typedef VideoRenderer *(*CreateRendererFunc)(
    const sp<ISurface> &surface,
    const char *componentName,
    OMX_COLOR_FORMATTYPE colorFormat,
    size_t displayWidth, size_t displayHeight,
    size_t decodedWidth, size_t decodedHeight);

    CreateRendererFunc func =
    (CreateRendererFunc)dlsym(
    m_libHandle,
    "_Z14createRendererRKN7android2spINS_8ISurfaceEEEPKc20"
    "OMX_COLOR_FORMATTYPEjjjj");

    if (func) 
    {

      m_renderer =
      (*func)(m_surface, "AVCDecoder", (OMX_COLOR_FORMATTYPE)format,
      videoWidth, videoHeight,
      dispWidth, dispHeight);
      if (m_renderer != NULL)
      {
        DP("HW renderer created\n");
      }
    }
    else
    {
      DP("Load CreateRendererFunc failed!\n");
    }
  }
  else
  {
    DP("m_libHandle not find !\n");
  }
  if (m_renderer == NULL)
  {
    m_renderer = new SoftwareRenderer((OMX_COLOR_FORMATTYPE)format, m_surface, dispWidth, dispHeight, videoWidth, videoHeight);
    if (m_renderer != NULL)
    {
      DP("SW renderer created\n");
    }
  }

  return (m_renderer != NULL) ? true : false;
}


void CMyStageFrightLocalRenderer::SendData(unsigned char * data, unsigned int dataBytes)
{
  m_renderer->render(data, dataBytes, NULL);
}

class CGfxAndroidStageFright : public IMcGfxOverlay, public CMcUnknown
{
public:
  DECLARE_IUNKNOWN

  CGfxAndroidStageFright();
  virtual ~CGfxAndroidStageFright();
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
  STDMETHODIMP UIFlip(void*)
  {
    return S_OK;
  }

  STDMETHODIMP GetDDCaps(LPVOID *pCaps)
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
  STDMETHODIMP SetWindowHandle(Window hwnd)
  {
    return E_NOTIMPL;
  }
  STDMETHODIMP GetWindowHandle(Window& hwnd)
  {
    return E_NOTIMPL;
  }

  STDMETHODIMP SetHWRenderSurf(LPVOID pSurf);
  STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
  {
    return E_NOTIMPL;
  }

  STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
  {
    return E_NOTIMPL;
  }
  STDMETHODIMP SetVRHandle(LPVOID pHandle)
  {
    return E_NOTIMPL;
  }
  STDMETHODIMP GetDDCaps(LONG* pCaps)
  {
    return E_NOTIMPL;
  }
  STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
  STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
  STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}

protected:
  RECT				m_rectSrc, m_rectDst;
  DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
  const DWORD			*m_pdwFourCCList;
  DWORD				m_dwBackBuffers;
  BOOL				m_bShow, m_bLastShow;
  HWND				m_hwnd;
  BOOL m_bOpen; 	

  LPBYTE m_pBuff;
  ISurface* m_isurface;
  CMyStageFrightLocalRenderer m_stageFrightRender;
};

static const DWORD const_dwFourCC[] =
{
  MAKEFOURCC('Y','V','1','2'),
  0
};

CGfxAndroidStageFright::CGfxAndroidStageFright() : CMcUnknown(0,0)
{
  m_bOpen = false;
}

CGfxAndroidStageFright::~CGfxAndroidStageFright()
{
  Close();
}

STDMETHODIMP CGfxAndroidStageFright::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
  int idx = 0;
  while(pdwFourCC && pdwFourCC[idx])
  {
    if(pdwFourCC[idx] ==  MAKEFOURCC('Y','V','1','2'))
    {
      m_dwFourCC = pdwFourCC[idx];
      break;
    }
    idx ++;
  }
  if((pdwFourCC == 0 )|| (pdwFourCC[idx]== 0))
    return E_FAIL;


  m_dwWidth = dwWidth;
  m_dwHeight = dwHeight;
  DP("Open %ld,%ld, 0x%x \n", m_dwWidth, m_dwHeight, m_dwFourCC);
    if(m_dwFourCC == MAKEFOURCC('Y','V','1','2'))
    {
      m_pBuff = (LPBYTE)malloc(m_dwWidth*m_dwHeight*3/2);
      if(!m_pBuff)  return E_FAIL;
      memset(m_pBuff,0x00,m_dwWidth*m_dwHeight);
      memset(m_pBuff + m_dwWidth*m_dwHeight,0x80,m_dwWidth*m_dwHeight/2);
    }
  m_bOpen = true;
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::Close()
{
  m_bOpen = false;
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::SetSrcRect(const RECT *pRect)
{
  return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidStageFright::SetDstRect(const RECT *pRect)
{
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::SetScnRect(const RECT *pRect)
{
  return S_OK;	
}

STDMETHODIMP CGfxAndroidStageFright::GetScnRect(RECT *pRect)
{
  return E_FAIL;
}

STDMETHODIMP CGfxAndroidStageFright::SetWindowHandle(HWND hwnd)
{
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::GetWindowHandle(HWND& hwnd)
{
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::SetAlpha(DWORD dwAlpha)
{
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
  return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidStageFright::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
  return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidStageFright::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
  return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidStageFright::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
  return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidStageFright::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
  if(m_bOpen == false)
    return E_FAIL;

	if(ppBuf)
		*ppBuf = static_cast<BYTE*>(m_pBuff);
	
	if(pStride){
			*pStride = m_dwWidth;
	}
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::Unlock(LONG num)
{
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::Flip(LONG num, DWORD dwFlags)
{
  if(m_bOpen == false)
    return E_FAIL;

	m_stageFrightRender.SendData(m_pBuff, m_dwWidth*m_dwHeight*3/2);
  	return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::Show(BOOL bShow)
{
  if(m_bShow==bShow)
    return S_OK;

  m_bShow = bShow;
  return S_OK;
}

STDMETHODIMP CGfxAndroidStageFright::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
  return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidStageFright::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
  return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidStageFright::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxAndroidStageFright::SetHWRenderSurf(LPVOID pSurf)
{
  if(pSurf == NULL)
    return E_INVALIDARG;

  if(m_bOpen == false)
    return E_FAIL;

  VR_ANDROID_ISUR_PARAM params = *(static_cast<VR_ANDROID_ISUR_PARAM *>(pSurf));
  m_isurface = (ISurface*)(params.pAndroidSFBuf);
  if(m_isurface == NULL)
  {
    return E_FAIL;
  }

  sp<ISurface> mSurface = m_isurface;

  m_stageFrightRender.Init(mSurface);
	DP("Renderer Init ok\n");

    PixelFormat_t pixel_format = OMX_COLOR_FormatYUV420Planar;
	bool isRendererInited = m_stageFrightRender.SetVideoFormat(m_dwWidth, m_dwHeight, m_dwWidth, m_dwHeight, pixel_format/*OMX_COLOR_FormatYUV420Planar*/);
	if (isRendererInited)
	{
		DP("Renderer set video format OK - video size %ld x %ld\n", m_dwWidth, m_dwHeight);
	}
	else
	{
		DP("Renderer set video format failed - video size %ld x %ld\n",m_dwWidth, m_dwHeight);
    return E_FAIL;
	}

	
  return S_OK;
}
STDMETHODIMP CGfxAndroidStageFright::IsOpen()
{
  return m_bOpen;
}

IMcGfxOverlay *CreateAndroidStageFright()
{
  return new CGfxAndroidStageFright();
}
#endif

