#include "../../inc/GfxBuildDef.h"

#ifdef ST
#include "windows.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "GfxOverlay.h"
#include "../../3rdparty/ST/stglib.h"
//#include "../../3rdparty/ST/sga_types.h"
#pragma comment(lib, "../../3rdparty/ST/sgalib.lib")


#define DEFAULT_BACK_BUFFER	2
typedef struct
{
	PVOID pBuf;
	int iBufSize;
}STBUFFER_S;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

class CGfxST: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxST();
	virtual ~CGfxST();
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
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}


private:

	STHDC m_hdc;
	HWND m_hwnd;	
	BOOL m_bOpen;
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	int				m_dwBackBuffers;
	STHBITMAP* m_hbitYUV_Block;
	STBUFFER_S* m_pbuff;

};

CGfxST::CGfxST(): CMcUnknown(0,0)
{
	m_hwnd = 0;
	m_bOpen = FALSE;
	memset(&m_rectSrc, 0 ,sizeof(m_rectSrc));
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	memset(&m_rectScn, 0 ,sizeof(m_rectScn));

	m_dwWidth = 0;
	m_dwHeight = 0;
	m_dwFourCC = 0;
	m_dwBackBuffers = 0;
	m_hbitYUV_Block = NULL;
	m_pbuff = NULL;

	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = 0;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ST_DEMO));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = _T("STWND_CLASS");
	RegisterClass(&wc);

	m_hwnd = CreateWindow(_T("STWND_CLASS"), _T("Video"), WS_OVERLAPPED/* used to WS_VISIBLE.But it makes device crash sometimes*/,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	m_hdc = STGetDC(m_hwnd);

}

CGfxST::~CGfxST()
{
	if(m_hdc)
	{
		STReleaseDC(m_hdc);
		m_hdc = NULL;
	}
	if(m_hwnd)
	{
		//It causes hang up. Don't destroy it.
		//DestroyWindow(m_hwnd);
		m_hwnd=NULL;
	}

}


STDMETHODIMP CGfxST::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	int ret  = 0;
	if(m_bOpen || (dwBuffers==0) || (pdwFourCC == NULL))
		return E_UNEXPECTED;

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	m_dwBackBuffers = DEFAULT_BACK_BUFFER;

	m_hbitYUV_Block = (STHBITMAP*)malloc(sizeof(STHBITMAP)*m_dwBackBuffers);
	if(m_hbitYUV_Block == NULL)
		return E_FAIL;

	t_sgaColorFormat sformat = {0,0};

	for(int i=0;m_dwFourCC = pdwFourCC[i];i++)
	{
		switch(m_dwFourCC)
		{
			case MAKEFOURCC('Y','V','1','2'):
				sformat = SGA_DIS_FMT_YUV420;
				break;
			case MAKEFOURCC('R','5','6','5'):
				sformat = SGA_DIS_FMT_RGB565;
				break;
			case MAKEFOURCC('Y','U','Y','2'):
				sformat = SGA_DIS_FMT_YUV422;
				break;
			default:
				break;
		}
		if(sformat.colorDepth != 0)
			break;
	}

	if(m_dwFourCC == NULL)
		return E_FAIL;
	
	ret = STCreateVideoBuffers(m_hdc, m_dwWidth, m_dwHeight, sformat, &m_dwBackBuffers, m_hbitYUV_Block);
	if(ret == FALSE)
		return E_FAIL;

	m_pbuff = (STBUFFER_S*)malloc(sizeof(STBUFFER_S)* m_dwBackBuffers);
	if(m_pbuff == NULL)
		return E_FAIL;

	for(int i=0;i<m_dwBackBuffers;i++)
	{
		ret = STGetVideoBufferAddress(m_hdc, m_hbitYUV_Block[i], &(m_pbuff[i].pBuf));
		ret &= STGetVideoBufferSize(m_hdc, m_hbitYUV_Block[i], &(m_pbuff[i].iBufSize));
		if(ret == FALSE)
			goto FAIL;
	}

	STSetScreenOffset(m_hdc, 0, 0);

	SetRect(&m_rectSrc, 0 ,0, m_dwWidth,m_dwHeight);
	memset(&m_rectDst, 0 ,sizeof(m_rectDst));
	m_bOpen = TRUE;
	return S_OK;
FAIL:

	if(m_pbuff)
	{
		free(m_pbuff);
		m_pbuff = NULL;
	}
	
	if(m_hbitYUV_Block)
	{
		free(m_hbitYUV_Block);
		m_hbitYUV_Block = NULL;
	}

	
	return E_FAIL;
}
STDMETHODIMP CGfxST::Close()
{
	if(!m_bOpen)
		return E_UNEXPECTED;

	
	if(m_pbuff)
	{
		free(m_pbuff);
		m_pbuff = NULL;
	}
	
	if(m_hbitYUV_Block)
	{
		free(m_hbitYUV_Block);
		m_hbitYUV_Block = NULL;
	}

	if(m_hdc)
	{
		STDestroyVideoBuffers(m_hdc);
		STDestroyVideoBuffers(m_hdc);
	}

	m_bOpen = FALSE;
	return S_OK;
}
STDMETHODIMP CGfxST::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(num >= m_dwBackBuffers)
		return E_FAIL;
	if(ppBuf)
		*ppBuf = m_pbuff[num].pBuf;
	if(pStride)
		*pStride = m_dwWidth;
	if(pPixCount)
	{
		if(m_dwFourCC == MAKEFOURCC('Y','V','1','2'))
			*pPixCount = 1;
	}
	return S_OK;
}
STDMETHODIMP CGfxST::Unlock(LONG num)
{
	return S_OK;
}
STDMETHODIMP CGfxST::Flip(LONG num, DWORD dwFlags)
{
	if(num >= m_dwBackBuffers)
		return E_FAIL;


	STRenderVideoBuffer(m_hdc, m_hbitYUV_Block[num], 0, 0, true);

	return S_OK;

}
STDMETHODIMP CGfxST::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxST::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxST::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_FAIL;
}
STDMETHODIMP CGfxST::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	for(int i=0;i<m_dwBackBuffers;i++)
		STSetCropWindow(m_hbitYUV_Block[i],m_rectSrc.left,m_rectSrc.top, m_rectSrc.right - m_rectSrc.left, m_rectSrc.bottom - m_rectSrc.top);

	return S_OK;
}

STDMETHODIMP CGfxST::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
	m_rectDst = *pRect;

	DWORD width = m_rectDst.right - m_rectDst.left;
	DWORD height = m_rectDst.bottom - m_rectDst.top;
	//	STSetScreenOffset(m_hdc, m_rectDst.left, m_rectDst.top);


	for(int i=0;i<m_dwBackBuffers;i++)
	{
		STSetVideoBufferWindow(m_hdc, m_hbitYUV_Block[i], m_rectDst.left, m_rectDst.top, width , height);
	}

	return S_OK;
}
STDMETHODIMP CGfxST::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxST::GetScnRect(RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxST::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}
STDMETHODIMP CGfxST::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}
STDMETHODIMP CGfxST::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxST::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxST::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxST::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxST::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxST::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxST::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxST::GetDDCaps(LONG*pCaps)
{
	if(pCaps==0)
		return E_POINTER;
	*pCaps = GFXOVERLAY_CAPS_CROPPING_SUPPORT;

	return S_OK;
}
STDMETHODIMP CGfxST::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{

	return S_OK;
}
STDMETHODIMP CGfxST::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxST::IsOpen()
{
	return m_bOpen? S_OK : S_FALSE;
}

IMcGfxOverlay *CreateOverlayST()
{
	return new CGfxST();
}
#endif
