#include "../../inc/GfxBuildDef.h"

#ifdef RENESAS
#include "windows.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../3rdparty/Renesas/include/libvo.h"
#include "GfxOverlay.h"
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

class CGfxRenesas: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxRenesas();
	virtual ~CGfxRenesas();
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

private:

	BOOL m_bOpen;
	BOOL				m_bShow;
	RECT				m_rectSrc, m_rectDst, m_rectScn, m_rectDst_showbak;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	int				m_dwBackBuffers;
	unsigned char* m_pBuff;
   int m_srcIdx;
	CRITICAL_SECTION m_csDisplay; 
};

CGfxRenesas::CGfxRenesas(): CMcUnknown(0,0)
{
	m_bOpen = FALSE;
	memset(&m_rectSrc, 0 ,sizeof(m_rectSrc));
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	memset(&m_rectScn, 0 ,sizeof(m_rectScn));
	memset(&m_rectDst_showbak, 0 ,sizeof(m_rectDst_showbak));

	m_dwWidth = 0;
	m_dwHeight = 0;
	m_dwFourCC = 0;
	m_dwBackBuffers = 0;
	m_bShow = TRUE;
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	InitializeCriticalSection(&m_csDisplay);
}

CGfxRenesas::~CGfxRenesas()
{
	DeleteCriticalSection(&m_csDisplay);

	Close();
}


STDMETHODIMP CGfxRenesas::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int ret  = 0;
	if(m_bOpen || (dwBuffers==0) || (pdwFourCC == NULL))
		return E_UNEXPECTED;
		
#if 0// (_WIN32_WCE == 0x500)
    switch(dwWidth)
    {
      case 180:
        m_srcIdx = 0;
        break;
      case 240:
        m_srcIdx = 1;
        break;
      case 320:
        m_srcIdx = 2;
        break;
      case 400:
        m_srcIdx = 3;
        break;
      case 480:
        m_srcIdx = 4;
        break;
      case 640:
        m_srcIdx = 5;
        break;
      case 720:
        m_srcIdx = 6;
        break;
      default:
        DP("size:%d,%d aren't supported by renesas",dwWidth,dwHeight);
        return E_INVALIDARG;
    }
#endif
    
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
    m_pBuff= 0;
	for(int i=0;m_dwFourCC = pdwFourCC[i];i++)
	{
		if(m_dwFourCC == MAKEFOURCC('N','V','1','2'))
			break;
	}

	if(m_dwFourCC == NULL)
		goto FAIL;

  m_pBuff = (unsigned char*)malloc(m_dwWidth *m_dwHeight*3/2);
  if(!m_pBuff ) 
  {
    DP("[RENESAS] FAILED TO MALLOC BUFFER \n");
    return E_FAIL;
  }

	VO_init(NULL);

	SetRect(&m_rectSrc, 0 ,0, m_dwWidth,m_dwHeight);
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	m_bOpen = TRUE;
	m_dwBackBuffers = 1;
	return S_OK;
FAIL:
	
	return E_FAIL;
}
STDMETHODIMP CGfxRenesas::Close()
{
	if(!m_bOpen)
		return E_UNEXPECTED;

	VO_deinit();

  if(m_pBuff)
    free(m_pBuff );

	m_bOpen = FALSE;
	return S_OK;
}
STDMETHODIMP CGfxRenesas::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
		*ppBuf = m_pBuff;

	if(pStride)
		*pStride = m_dwWidth;

  return S_OK;
}
STDMETHODIMP CGfxRenesas::Unlock(LONG num)
{
  return S_OK;
}

STDMETHODIMP CGfxRenesas::Flip(LONG num, DWORD dwFlags)
{
	if(num >= m_dwBackBuffers)
		return E_FAIL;

	CProtect proc(&m_csDisplay);
 
	VO_write(m_pBuff,m_pBuff + m_dwWidth*m_dwHeight,0);
 	VO_read();

	return S_OK;

}
STDMETHODIMP CGfxRenesas::Show(BOOL bShow)
{
	HRESULT ret = E_FAIL;
	if((bool)m_bShow==(bool)bShow)
		return S_OK;
	if(!m_bShow)
	{
		m_rectDst = m_rectDst_showbak;
		ret = SetDstRect(&m_rectDst);
	}
	else
	{
		m_rectDst_showbak = m_rectDst;
		RECT zeroRect = {0, 0, 0, 0};
		ret = SetDstRect(&zeroRect);
	}
	if(SUCCEEDED(ret))	
		m_bShow = (bool)bShow;
	return ret;
}
STDMETHODIMP CGfxRenesas::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxRenesas::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxRenesas::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;

	return S_OK;
}

STDMETHODIMP CGfxRenesas::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;

	CProtect proc(&m_csDisplay);

	m_rectDst = *pRect;

	m_rectDst.left  &= ~(4-1);
	m_rectDst.top  &= ~(4-1);
	m_rectDst.right  &= ~(4-1);
	m_rectDst.bottom  &= ~(4-1);

#if (_WIN32_WCE == 0x500)


#ifdef FIXED_DST
    long oriWidth = m_rectDst.right - m_rectDst.left;
    long oriHeight = m_rectDst.bottom - m_rectDst.top;
     long preferWidth = 0;
     long preferHeight = 0;
 
 	if(oriHeight == 480 || oriWidth == 800)
 	{
 	preferWidth = 800;
 	preferHeight = 440;
 	}
 	else
 	{
 	preferWidth = 320;
 	preferHeight = 240;
 	}
 
 	m_rectDst.left -= (preferWidth - oriWidth)/2;
 	m_rectDst.right += (preferWidth - oriWidth)/2;
 	m_rectDst.top -= (preferHeight - oriHeight)/2;
 	m_rectDst.bottom += (preferHeight - oriHeight)/2;
#endif


	if(m_rectDst.left<0)
	{
	m_rectDst.right -= m_rectDst.left;
	m_rectDst.left = 0;
	}

	if(m_rectDst.top<0)
	{
	m_rectDst.bottom -= m_rectDst.top;
	m_rectDst.top = 0;
	}

	if(m_rectDst.right > 800)
	{
	m_rectDst.left = 0;
	m_rectDst.right = 800;
	}

	if(m_rectDst.bottom > 440)
	{
	m_rectDst.top = 20;
	m_rectDst.bottom  = 460;
	}

 
#endif

   	DWORD dst_width = m_rectDst.right - m_rectDst.left;
	DWORD dst_height = m_rectDst.bottom - m_rectDst.top;

	DP("renesas:%d,%d size:%d,%d \n",m_rectDst.left,m_rectDst.top,dst_width,dst_height);

	VO_set_video_startpos(m_rectDst.left, m_rectDst.top);
	VO_set_video_parameter(m_dwWidth, m_dwHeight,m_dwWidth, dst_width , dst_height, 800, 1);
	
	return S_OK;
}
STDMETHODIMP CGfxRenesas::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::GetScnRect(RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}
STDMETHODIMP CGfxRenesas::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesas::GetDDCaps(LONG*pCaps)
{
	if(pCaps==0)
		return E_POINTER;

	return S_OK;
}
STDMETHODIMP CGfxRenesas::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{

	return S_OK;
}
STDMETHODIMP CGfxRenesas::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxRenesas::IsOpen()
{
	return m_bOpen? S_OK : S_FALSE;
}

IMcGfxOverlay *CreateOverlayRenesas()
{
	return new CGfxRenesas();
}
#endif
