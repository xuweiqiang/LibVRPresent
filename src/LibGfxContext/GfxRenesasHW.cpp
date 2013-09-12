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

extern void VO_init_hw(VIDEO_CONTEXT *video_info);
extern void VO_deinit_hw(void);
extern void VO_set_video_startpos_hw(unsigned long x , unsigned long y);
extern void VO_set_video_mode_change_hw(unsigned long in_w, unsigned long in_h , unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , unsigned long out_mem_w , unsigned long buff_num);
extern void VO_set_video_parameter_hw(unsigned long in_w, unsigned long in_h , unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , unsigned long out_mem_w , unsigned long buff_num);
extern bool VO_write_hw(unsigned char *y , unsigned char *c , double dts);
extern bool VO_read_hw(void);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

class CGfxRenesasHW: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxRenesasHW();
	virtual ~CGfxRenesasHW();
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
      if(!pSurf )
        return E_FAIL;

      m_phycalHnd = *(GfxRENESAS_HND*)pSurf;
      VO_write_hw(m_phycalHnd.pYBuff,m_phycalHnd.pUVBuff,0);

#if (_WIN32_WCE == 0x500)
	if(_hVPU_display ==NULL)
	{
		m_threadRunning = true;
		_hVPU_display = CreateThread(NULL , 0 , DEC_VPU_display_thread , this,0 , 0);
	}
#else
      VO_read_hw();
#endif
		return S_OK;
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

	BOOL m_threadRunning;

private:

	BOOL m_bOpen;
	BOOL				m_bShow;
	RECT				m_rectSrc, m_rectDst, m_rectScn, m_rectDst_showbak;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	int				m_dwBackBuffers;
	GfxRENESAS_HND m_phycalHnd;
	   int m_srcIdx;
	HANDLE _hVPU_display;
 };

static DWORD WINAPI DEC_VPU_display_thread(PVOID pArg)
{
	CGfxRenesasHW* pThis = (CGfxRenesasHW*)pArg;
	while(pThis->m_threadRunning)
	{
		if(VO_read_hw() == true){
 		}
		Sleep(VO_THREAD_SYNC_TIME);
	}
	return 0;
}

CGfxRenesasHW::CGfxRenesasHW(): CMcUnknown(0,0)
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
	_hVPU_display = NULL;
	m_threadRunning = false;
	m_bShow = TRUE;
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
}

CGfxRenesasHW::~CGfxRenesasHW()
{
	Close();
}


STDMETHODIMP CGfxRenesasHW::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int ret  = 0;
	if(m_bOpen || (dwBuffers==0) || (pdwFourCC == NULL))
		return E_UNEXPECTED;

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	_hVPU_display = NULL;
 	for(int i=0;m_dwFourCC = pdwFourCC[i];i++)
	{
		if(m_dwFourCC == MAKEFOURCC('N','V','1','2'))
			break;
	}

	if(m_dwFourCC == NULL)
		goto FAIL;

		
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

    

	VO_init_hw(NULL);

	SetRect(&m_rectSrc, 0 ,0, m_dwWidth,m_dwHeight);
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	m_bOpen = TRUE;
	m_dwBackBuffers = 1;
	return S_OK;
FAIL:
	
	return E_FAIL;
}
STDMETHODIMP CGfxRenesasHW::Close()
{
	if(!m_bOpen)
		return E_UNEXPECTED;

	m_threadRunning = false;
	VO_deinit_hw();

	m_bOpen = FALSE;
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
   return S_OK;
}
STDMETHODIMP CGfxRenesasHW::Unlock(LONG num)
{
  return S_OK;
}

STDMETHODIMP CGfxRenesasHW::Flip(LONG num, DWORD dwFlags)
{
	if(num >= m_dwBackBuffers)
		return E_FAIL;

	return S_OK;

}
STDMETHODIMP CGfxRenesasHW::Show(BOOL bShow)
{
	HRESULT ret = E_FAIL;
	if((bool)m_bShow==(bool)bShow)
		return S_OK;
	if(!m_bShow)
	{
		ret = SetDstRect(&m_rectDst_showbak);
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
STDMETHODIMP CGfxRenesasHW::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxRenesasHW::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxRenesasHW::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;

	return S_OK;
}

STDMETHODIMP CGfxRenesasHW::SetDstRect(const RECT *pRect)
{
	if (!m_bShow)
	{
		m_rectDst_showbak = *pRect;
		return S_OK;
	}
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
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

	VO_set_video_parameter_hw(m_dwWidth, m_dwHeight,m_dwWidth, dst_width , dst_height, 800, 1);
	VO_set_video_startpos_hw(m_rectDst.left, m_rectDst.top);
	
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::GetScnRect(RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}
STDMETHODIMP CGfxRenesasHW::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::GetDDCaps(LONG*pCaps)
{
	if(pCaps==0)
		return E_POINTER;

	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{

	return S_OK;
}
STDMETHODIMP CGfxRenesasHW::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxRenesasHW::IsOpen()
{
	return m_bOpen? S_OK : S_FALSE;
}

IMcGfxOverlay *CreateOverlayRenesasHW()
{
	return new CGfxRenesasHW();
}
#endif
