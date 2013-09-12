#include "../../inc/GfxBuildDef.h"

#ifdef ST_LINUX
#include "windows.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "GfxOverlay.h"
#include "../../3rdparty/ST_Linux/noDataCopy/stglib.h"

#define	ASSERT(x)	{ int ret;\
			ret = x;\
			if(ret != 1) { \
				DP("Function at line %d failed\n",__LINE__); \
				return; \
			} \
		}


#define DEFAULT_BACK_BUFFER	8
typedef struct
{
	PVOID pBuf;
	int iBufSize;
}STBUFFER_S;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

class CGfxSTLnx: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxSTLnx(bool bExtBuff);
	virtual ~CGfxSTLnx();
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

	STDMETHODIMP SetWindowHandle(Window  hwnd)	{return E_NOTIMPL;}
	STDMETHODIMP GetWindowHandle(Window & hwnd)	{return E_NOTIMPL;}
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
		return E_FAIL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
       STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
		if(!pSurf || (m_bExternalBuff == false) )
		return E_FAIL;

		m_phycalHnd = *(GfxST_HND*)pSurf;
		m_hdc = m_phycalHnd.hdc;
		return STSetFlipDelay(m_hdc, delayval);
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
	STDMETHODIMP UIFlip(LPVOID pHandle){return E_NOTIMPL;}
	STDMETHODIMP SetFLipDelay(int val)
	{
	    delayval = val;
	    if(m_hdc)
	    {
	      return STSetFlipDelay(m_hdc, delayval);
	    }
    return S_OK;
	}

private:

	STHDC m_hdc;
	HWND m_hwnd;	
	BOOL m_bOpen;
	RECT				m_rectSrc, m_rectDst, m_rectScn, m_rectDst_showbak;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	int				m_dwBackBuffers;
	STHBITMAP* m_hbitYUV_Block;
    void** m_virBufAddr;
    unsigned long* m_phyBufAddr;
    
	GfxST_HND m_phycalHnd;
    bool m_bExternalBuff;
    t_sgaColorFormat m_sformat; 
    bool m_bShow;
    int delayval;
    sga_uint16 m_colorKey;
    sga_uint16 m_alpha;
};

CGfxSTLnx::CGfxSTLnx(bool bExtBuff): CMcUnknown(0,0)
{
	m_hwnd = 0;
	m_bOpen = false;
	memset(&m_rectSrc, 0 ,sizeof(m_rectSrc));
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
//	memset(&m_rectScn, 0 ,sizeof(m_rectScn));

	SetRect(&m_rectScn,0,0,800,480);
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_dwFourCC = 0;
	m_dwBackBuffers = 0;
	m_hbitYUV_Block = NULL;
	memset(&m_phycalHnd, 0 ,sizeof(m_phycalHnd));

    m_bExternalBuff = bExtBuff;
	//mainTest();
}

CGfxSTLnx::~CGfxSTLnx()
{
  Close();
	if(m_hwnd)
	{
		//It causes hang up. Don't destroy it.
		//DestroyWindow(m_hwnd);
		m_hwnd=NULL;
	}

}


STDMETHODIMP CGfxSTLnx::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
const DWORD *pdwFourCC, RECT* pDst)
{
	int ret  = 0;
	if(m_bOpen || (dwBuffers==0) || (pdwFourCC == NULL))
		return E_UNEXPECTED;

    if(dwWidth > 2048 ||dwHeight >2048)
      return E_INVALIDARG;

    memset(&m_sformat, 0,sizeof(m_sformat));
    m_bShow =true;
    memset(&m_rectDst_showbak, 0,sizeof(m_rectDst_showbak));
    delayval =0;
	for(int i=0;m_dwFourCC = pdwFourCC[i];i++)
	{
		switch(m_dwFourCC)
		{
			case MAKEFOURCC('Y','V','1','2'):
				m_sformat = SGA_DIS_FMT_YUV420;
				break;
			case MAKEFOURCC('R','5','6','5'):
				m_sformat = SGA_DIS_FMT_RGB565;
				break;
			//case MAKEFOURCC('Y','U','Y','2'):
			//	m_sformat = SGA_DIS_FMT_YUV422;
			//	break;
			default:
				break;
		}
		if(m_sformat.colorDepth != 0)
			break;
	}

	if(m_dwFourCC == NULL)
		return E_FAIL;
  DP("GfxST_LINUX openformat :%p ,(%d,%d )\n",m_dwFourCC,dwWidth,dwHeight);

	memset(&m_phycalHnd, 0 ,sizeof(m_phycalHnd));

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

    if(m_bExternalBuff)
    {
        	m_dwBackBuffers = 8;
    		m_hdc = NULL;
    }
    else
    {
        	m_dwBackBuffers = 2;
        	void *address;
        	m_hdc = STCreateDC((HWND)1, 3, &address);
        	if(!m_hdc)
        	{
        		DP("Failed to STCreateDC \n");
        		return E_FAIL;
        	}


         long 	bufferSize = 0; 
		if(m_dwFourCC == MAKEFOURCC('Y','V','1','2'))
              bufferSize = m_dwWidth * m_dwHeight* 3 / 2;
		else if(m_dwFourCC == MAKEFOURCC('R','5','6','5'))
              bufferSize = m_dwWidth * m_dwHeight *2;

        	m_hbitYUV_Block = (STHBITMAP*)malloc(sizeof(STHBITMAP)*m_dwBackBuffers);
        	if(m_hbitYUV_Block == NULL)
        		return E_FAIL;

        m_virBufAddr = (void**)malloc(sizeof(void*)*m_dwBackBuffers);
        m_phyBufAddr  = (unsigned long*)malloc(sizeof(unsigned long)*m_dwBackBuffers);
        
     for(int i =0;i< m_dwBackBuffers;i++ )
    {
		m_hbitYUV_Block[i] = STCreateDMABuffer(m_hdc, bufferSize);

		if(m_hbitYUV_Block[i] == 0)
		  {
          DP("%s failed l:%d \n",__FUNCTION__, __LINE__);
          goto FAIL;
		  }
		  if(STGetDMABufferAddress(m_hdc, m_hbitYUV_Block[i], &m_virBufAddr[i], &m_phyBufAddr[i]) != 1)
		  {
          DP("%s failed l:%d \n",__FUNCTION__, __LINE__);
          goto FAIL;
		  }

 //     DP("m_virBufAddr[i]=%p , m_phyBufAddr[i] = %p \n",m_virBufAddr[i], m_phyBufAddr[i]);
    }


    }

	SetRect(&m_rectSrc, 0 ,0, m_dwWidth,m_dwHeight);
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));

    m_colorKey = 0;
    m_alpha = 0;
	m_bOpen = TRUE;

	return S_OK;
FAIL:


	if(m_hbitYUV_Block)
	{
		free(m_hbitYUV_Block);
		m_hbitYUV_Block = NULL;
	}

    if(!m_bExternalBuff)
  {
      if(m_hbitYUV_Block)
      {
        for(int i=0; i<m_dwBackBuffers; i++)
        {
          if(m_hbitYUV_Block[i])
            STDestroyDMABuffer(m_hdc, m_hbitYUV_Block[i]);
        }
      }

    STDestroyVideoBuffers(m_hdc);
    STReleaseDC(m_hdc);
  }

	return E_FAIL;
}

STDMETHODIMP CGfxSTLnx::Close()
{
  if(!m_bOpen)
    return E_UNEXPECTED;

    if(m_bExternalBuff == false)
    {
      for(int i=0; i<m_dwBackBuffers; i++)
        STDestroyDMABuffer(m_hdc, m_hbitYUV_Block[i]);


    	if(m_hbitYUV_Block)
    	{
    		free(m_hbitYUV_Block);
    		m_hbitYUV_Block = NULL;
    	}

    	STDestroyVideoBuffers(m_hdc);

		STReleaseDC(m_hdc);
		m_hdc = NULL;
    }

	m_bOpen = FALSE;
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
  if(!m_bOpen)
    return E_UNEXPECTED;

    if(m_bExternalBuff)
    {
         DP("%s failed l:%d \n",__FUNCTION__, __LINE__);
        return E_FAIL;
    }
      
	if(num >= m_dwBackBuffers)
		return E_FAIL;
		
	if(ppBuf)
		*ppBuf = m_virBufAddr[num];
	if(pStride)
	{
		if(m_dwFourCC == MAKEFOURCC('Y','V','1','2'))
		*pStride = m_dwWidth;
		else if(m_dwFourCC == MAKEFOURCC('R','5','6','5'))
		*pStride = m_dwWidth*2;
	}
	if(pPixCount)
	{
		if(m_dwFourCC == MAKEFOURCC('Y','V','1','2'))
			*pPixCount = 1;
		else if(m_dwFourCC == MAKEFOURCC('R','5','6','5'))
			*pPixCount = 2;
	}
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::Unlock(LONG num)
{
  if(!m_bOpen)
    return E_UNEXPECTED;

	return S_OK;
}
STDMETHODIMP CGfxSTLnx::Flip(LONG num, DWORD dwFlags)
{
  if(!m_bOpen)
    return E_UNEXPECTED;

	if(num >= m_dwBackBuffers){	DP("FLIP failed :%d \n",__LINE__);		return E_FAIL;	}
	if(m_hdc == NULL)		{	DP("FLIP failed :%d \n",__LINE__);		return E_FAIL;	}
/*
    unsigned short* pBuf = (unsigned short*)m_virBufAddr[num];
    for(int i=0;i<m_dwWidth*m_dwHeight/3;i++)
    {
      *pBuf = (0xf800);
      pBuf++;
    }
    for(int i=0;i<m_dwWidth*m_dwHeight/3;i++)
    {
      *pBuf = (0x07e0);
      pBuf++;
    }

    for(int i=0;i<m_dwWidth*m_dwHeight/3;i++)
    {
      *pBuf = (0x001f);
      pBuf++;
    }
      */
  int ret =0;

	STSetScreenOffset(m_hdc, 0, 0);
	STSetVideoBufferWindow(m_hdc, NULL, m_rectDst.left, m_rectDst.top, 
			m_rectDst.right - m_rectDst.left, m_rectDst.bottom - m_rectDst.top);

  if(m_bExternalBuff)
  {

      if(m_phycalHnd.m_bitmap == NULL)
        DP("m_phycalHnd.m_bitmap = NULL !! \n");
  
    //ret = STSetCropWindow(m_phycalHnd.m_bitmap,m_rectSrc.left,m_rectSrc.top, m_rectSrc.right - m_rectSrc.left, m_rectSrc.bottom - m_rectSrc.top);
	//if(ret == FALSE)	{	DP("FLIP failed :%d ret:%d \n",__LINE__,ret );		}

	//DP("STRenderVideoBuffer:%d (%d,%d ),%d,%d, str:%d\n",m_phycalHnd.yBuff,
	//	m_dwWidth, m_dwHeight, m_phycalHnd.uOffset,m_phycalHnd.vOffset, m_phycalHnd.yStride);

    if(m_dwFourCC == MAKEFOURCC('R','5','6','5'))
    {
      m_phycalHnd.uStride = m_phycalHnd.vStride = 0;
      m_phycalHnd.uOffset = m_dwWidth*m_dwHeight;
      m_phycalHnd.vOffset = m_dwWidth*m_dwHeight*5/4;
    }
	ret = STRenderVideoBuffer(m_hdc, NULL, m_phycalHnd.yBuff,
			m_dwWidth, m_dwHeight,0, m_phycalHnd.uOffset,m_phycalHnd.vOffset, 
			m_phycalHnd.yStride,m_phycalHnd.uStride,m_phycalHnd.vStride,m_sformat);
	if(ret == FALSE)	{	DP("FLIP failed :%d \n",__LINE__);		return E_FAIL;	}
  }
  else
  {
    if(m_dwFourCC == MAKEFOURCC('R','5','6','5'))
    {
      DP("L:%d \n",__LINE__);
     	ret = STRenderVideoBuffer(m_hdc,NULL,m_phyBufAddr[num],m_dwWidth, m_dwHeight,
     	0, m_dwWidth* m_dwHeight,m_dwWidth* m_dwHeight*5/4, m_dwWidth,0,0,m_sformat);
    }
  else
     	ret = STRenderVideoBuffer(m_hdc,NULL,m_phyBufAddr[num],m_dwWidth, m_dwHeight,0, m_dwWidth* m_dwHeight,
    		m_dwWidth* m_dwHeight*5/4, m_dwWidth,m_dwWidth/2,m_dwWidth/2,m_sformat);
  	if(ret == FALSE)	{	DP("FLIP failed :%d \n",__LINE__);		return E_FAIL;	}
  }
	return S_OK;

}
STDMETHODIMP CGfxSTLnx::Show(BOOL bShow)
{
  HRESULT ret = S_OK;
	if(m_bShow==(bool)bShow)
		return S_OK;

	if(!m_bShow)
	{
		m_rectDst = m_rectDst_showbak;
	}
	else
	{
		m_rectDst_showbak = m_rectDst;
		RECT zeroRect;
		memset(&zeroRect, 0 ,sizeof(zeroRect));
		ret = SetDstRect(&zeroRect);
		Flip(0,0);
	}
	if(SUCCEEDED(ret))	
		m_bShow = (bool)bShow;

	return S_OK;
}
STDMETHODIMP CGfxSTLnx::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxSTLnx::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxSTLnx::SetSrcRect(const RECT *pRect)
{
  if(!m_bOpen)
    return E_UNEXPECTED;

	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
/*
  if(!m_bExternalBuff){
	for(int i=0;i<m_dwBackBuffers;i++)
		STSetCropWindow(m_hbitYUV_Block?m_hbitYUV_Block[i]:NULL,m_rectSrc.left,m_rectSrc.top, m_rectSrc.right - m_rectSrc.left, m_rectSrc.bottom - m_rectSrc.top);
  }
*/

	return S_OK;
}

STDMETHODIMP CGfxSTLnx::SetDstRect(const RECT *pRect)
{
  RECT* pDst = NULL;
	if(!m_bShow)
	pDst = &m_rectDst_showbak;
  else
	pDst = &m_rectDst;

	if(EqualRect(pRect,pDst))
		return S_OK;

	*pDst = *pRect;
  pDst->left &= (~(2-1));
  pDst->right &= (~(2-1));
  pDst->top &= (~(2-1));
  pDst->bottom &= (~(2-1));
  
	DP("=============SetDstRect :%d,%d,%d,%d\n",pDst->left, pDst->top, pDst->right, pDst->bottom);

	return S_OK;
}
STDMETHODIMP CGfxSTLnx::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxSTLnx::SetAlpha(DWORD dwAlpha)
{
    m_alpha = float(dwAlpha)*2.55;

    if(m_hdc)
    {
        bool keyAndAlpha;
        if(dwAlpha)
          keyAndAlpha = true;
        else
          keyAndAlpha  = false;

        STColorKeyAlpha(m_hdc, keyAndAlpha, m_colorKey,  m_alpha); 
        return S_OK;
    }


	return E_FAIL;
}
STDMETHODIMP CGfxSTLnx::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
  m_colorKey= dwDstCkey;
  if(m_hdc)
  {
    STColorKeyAlpha(m_hdc, true, m_colorKey,  m_alpha); 
    return S_OK;
  }
  return E_FAIL;
}
STDMETHODIMP CGfxSTLnx::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxSTLnx::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}
STDMETHODIMP CGfxSTLnx::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{

	return S_OK;
}
STDMETHODIMP CGfxSTLnx::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(m_bOpen==0 || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0; 

	return S_OK;
}

STDMETHODIMP CGfxSTLnx::IsOpen()
{
	return m_bOpen? S_OK : S_FALSE;
}

IMcGfxOverlay *CreateOverlaySTLnx(bool bExtBuff)
{
  DP("%s : %d\n",__FUNCTION__, bExtBuff);
	return new CGfxSTLnx(bExtBuff);
}
#endif

