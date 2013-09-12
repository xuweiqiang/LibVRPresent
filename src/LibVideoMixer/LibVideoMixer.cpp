
// LibVideoMixer.cpp : Defines the initialization routines for the DLL.
//
#include "../../inc/GfxBuildDef.h"

#include <stdio.h>
#include <windows.h>
#include "LibVideoMixer.h"

#define DEFAULT_FOURCC MAKEFOURCC('Y','V','1','2')
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
 ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif
#define DEFAULT_ALPHA_VAL	1.0f
static void copybuf(unsigned char *dst, unsigned char *src, int d_stride, int s_stride, int width, int height)
{
	int i;
	for(i=0;i<height;i++)
	{
		memcpy(dst,src,width);
		dst += d_stride;
		src += s_stride;
	}
}

static void PrepareBufferYUY2(LPBYTE pBuf, LONG lStride, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwWidth, DWORD dwHeight)
{
	DWORD i,j;
	LPBYTE py,pu,pv,pyuv;

	for(i=0;i<dwHeight;i++)
	{
		pyuv = pBuf + i*lStride;
		py = ppbPlanes[0] + i*pdwStrides[0];
		pu = ppbPlanes[1] + (i>>1)*pdwStrides[1];
		pv = ppbPlanes[2] + (i>>1)*pdwStrides[2];
		for(j=0;j<dwWidth;j+=2)
		{
			*pyuv++ = *py++;
			*pyuv++ = *pu++;
			*pyuv++ = *py++;
			*pyuv++ = *pv++;
		}
	}
}


static void PrepareBufferYV12(LPBYTE pBuf, LONG lStride, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwWidth, DWORD dwHeight)
{
	copybuf(pBuf,ppbPlanes[0],lStride,pdwStrides[0],dwWidth,dwHeight);
	pBuf += lStride*dwHeight;

	copybuf(pBuf,ppbPlanes[2],lStride>>1,pdwStrides[2],dwWidth>>1,dwHeight>>1);
	pBuf += (lStride>>1)*(dwHeight>>1);
	copybuf(pBuf,ppbPlanes[1],lStride>>1,pdwStrides[1],dwWidth>>1,dwHeight>>1);

}

CLibVideoMixerApp::CLibVideoMixerApp():
	m_pMainBuf(NULL),
	m_bOpen(FALSE),
	m_bytes_per_pixcel(0),
	m_pVRPresent(0),
	m_pAlphaBlend(0),
	m_pCCResizeBuf(NULL)
{
	memset(&m_openOptions,0,sizeof(m_openOptions));
	memset(&m_hSurface, 0,sizeof(m_hSurface));
	InitializeCriticalSection(&m_csPresent);
	m_mainSurf_FourCC = MAKEFOURCC('Y','V','1','2');
}

CLibVideoMixerApp::~CLibVideoMixerApp()
{
	DeleteCriticalSection(&m_csPresent);
}

STDMETHODIMP CLibVideoMixerApp::GetFourCCLength(DWORD fourCC, DWORD *plength)
{
	if(plength == NULL)
		return E_FAIL;
	
	switch(fourCC)
	{
		case MAKEFOURCC('Y','V','1','2'):
		case MAKEFOURCC('N','V','1','2'):
		case MAKEFOURCC('Y','U','Y','2'):
			*plength = 1;
			break;
		case MAKEFOURCC('R','5','6','5'):
			*plength = 2;
			break;
		default:
			break;
	}
	
	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::Open(struct VM_OpenOptions* pOptions, DWORD dwOptionSize)
{
	if((m_bOpen==TRUE) || (pOptions==NULL)||dwOptionSize!=sizeof(struct VM_OpenOptions)||(pOptions->dwWidth == 0)||(pOptions->dwHeight == 0))
		return E_FAIL;
	memcpy(&m_openOptions, pOptions,sizeof(m_openOptions));

	if(m_openOptions.dwFourCC == 0)
		m_openOptions.dwFourCC = DEFAULT_FOURCC;

	GetFourCCLength(m_openOptions.dwFourCC, (DWORD*)&m_bytes_per_pixcel);
	if(m_bytes_per_pixcel == 0)
		return E_FAIL;

	m_pMainBuf = malloc(m_openOptions.dwHeight*m_openOptions.dwWidth*m_bytes_per_pixcel*3/2);
	if(m_pMainBuf ==NULL)
		return E_FAIL;

	m_pCCResizeBuf = malloc(m_openOptions.dwHeight*m_openOptions.dwWidth*m_bytes_per_pixcel*3/2);
	if(m_pCCResizeBuf ==NULL)
	{
		free(m_pMainBuf);
		m_pMainBuf = NULL;
		return E_FAIL;
	}

	m_pAlphaBlend = new CAlphaBlending();
	if(m_pAlphaBlend  == NULL)
		return E_FAIL;

	m_pAlphaBlend->SetAlphaBlendingYUV(m_openOptions.dwWidth, m_openOptions.dwHeight,m_openOptions.dwWidth, m_openOptions.dwHeight,
		0,0,0,0,0,DEFAULT_ALPHA_VAL);

	for(int i=MAX_SURFACE_NUM-1;i>=0;i--)
		m_hSurface[i].fAlpha = DEFAULT_ALPHA_VAL;

	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::Close()
{
	//Destroy main buf
	if(m_pMainBuf)
	{
		free(m_pMainBuf);
		m_pMainBuf = NULL;
	}
	//Destroy Surface buf
	for(int i=0;i<MAX_SURFACE_NUM;i++)
	{
		if(m_hSurface[i].bEnabled)
		{
			free(m_hSurface[i].pBuf);
		}
	}
	memset(&m_hSurface, 0 ,sizeof(m_hSurface));

	if(m_pCCResizeBuf)
	{
		free(m_pCCResizeBuf);
		m_pCCResizeBuf = NULL;
	}

	if(m_pAlphaBlend)
	{	
		delete m_pAlphaBlend;
		m_pAlphaBlend = NULL;
	}
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::Reset()
{
	return S_OK;
}
STDMETHODIMP CLibVideoMixerApp::Get(VM_SURFACE_LAYER_ID id, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	if((id>=MAX_SURFACE_NUM)||(!m_hSurface[id].bEnabled))
		return E_FAIL;

	return S_OK;
}
STDMETHODIMP CLibVideoMixerApp::Set(VM_SURFACE_LAYER_ID id, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	if((id>=MAX_SURFACE_NUM))
		return E_FAIL;
	switch (dwPropID)
	{
	case MC_PROPID_VP_RECTSRC:
		{
			RECT *pr = reinterpret_cast<RECT *>(pPropData);
			m_hSurface[id].dwWidth = pr->right - pr->left;
			m_hSurface[id].dwHeight = pr->bottom - pr->top;
		}
		break;
	case MC_PROPID_VP_RECTDST:
		{
			RECT* pr;
			pr = reinterpret_cast<RECT *>(pPropData);
			m_hSurface[id].rDst = *pr;
		}
		break;
	case MC_PROPID_VP_COLORKEYSRC:
		{
			DWORD* pr;
			pr = reinterpret_cast<DWORD*>(pPropData);
			m_hSurface[id].dwTranColor = *pr;
		}
		break;
	case MC_PROPID_VP_ALPHA:
		{
			LONG* pr;
			pr = reinterpret_cast<LONG*>(pPropData);
			m_hSurface[id].fAlpha = (float)*pr/100;
			if(m_hSurface[id].fAlpha >1.0f)
				m_hSurface[id].fAlpha =1.0f;
			m_pAlphaBlend->SetAlphaBlendingYUV(m_openOptions.dwWidth, m_openOptions.dwHeight,m_openOptions.dwWidth, m_openOptions.dwHeight,
				0,0,0,0,0,m_hSurface[id].fAlpha);
		}
		break;
	case MC_PROPID_VP_CLEAR:
		{
			LPBYTE pBuf = (LPBYTE)malloc(m_hSurface[id].dwWidth*m_hSurface[id].dwHeight*3/2);
			if(!pBuf)	return E_FAIL;
			LPBYTE pArray[3];
			pArray[0] = pBuf;
			pArray[1] = pBuf + m_hSurface[id].dwWidth*m_hSurface[id].dwHeight;
			pArray[2] = pBuf + m_hSurface[id].dwWidth*m_hSurface[id].dwHeight*5/4;

			memset(pArray[0],(m_hSurface[id].dwTranColor>>16)&0xff,m_hSurface[id].dwWidth*m_hSurface[id].dwHeight);
			memset(pArray[1],(m_hSurface[id].dwTranColor>>8)&0xff,m_hSurface[id].dwWidth*m_hSurface[id].dwHeight/4);
			memset(pArray[2],(m_hSurface[id].dwTranColor)&0xff,m_hSurface[id].dwWidth*m_hSurface[id].dwHeight/4);

			
			Prepare(id,pArray, &m_hSurface[id].dwWidth, 0);
			delete pBuf;
		}
		break;
	case MC_PROPID_VP_ENABLE:
		{
			BOOL* pr;
			pr = reinterpret_cast<BOOL*>(pPropData);
			if(!pr)	return E_INVALIDARG;
			m_hSurface[id].bEnabled = *pr;
		}
		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::CreateSurface(VM_SURFACE_LAYER_ID id, struct VM_SurfaceOpenOptions* pOptions)  
{
	if((m_bOpen==FALSE)||(pOptions == NULL))
		return E_FAIL;

	if((id>=MAX_SURFACE_NUM)||(m_hSurface[id].bEnabled) || (pOptions->dwInputFourCC == 0))
		return E_FAIL;

	m_hSurface[id].surface_id = id;
	m_hSurface[id].input_fourCC = pOptions->dwInputFourCC;
	m_hSurface[id].dwWidth = pOptions->dwWidth;
	m_hSurface[id].dwHeight = pOptions->dwHeight;
	m_hSurface[id].dwTranColor = pOptions->dwTranColor;
	m_hSurface[id].fAlpha = 1.0f;
	
	DWORD bytes_pixcel = 0;
	
	GetFourCCLength(pOptions->dwInputFourCC, &bytes_pixcel);
	if(bytes_pixcel  == 0)
		return E_FAIL;
	
	m_hSurface[id].pBuf= malloc(pOptions->dwHeight*pOptions->dwWidth*bytes_pixcel*3/2);
	if(m_hSurface[id].pBuf == NULL)
		return E_FAIL;
	
	m_hSurface[id].bEnabled = TRUE;
	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::DestroySurface(VM_SURFACE_LAYER_ID id)
{
	if(m_bOpen==FALSE)
		return E_FAIL;
	
	if(id < MAX_SURFACE_NUM)
	{
		if(m_hSurface[id].pBuf)
		{
			free(m_hSurface[id].pBuf);
			memset(&m_hSurface[id], 0,sizeof(VIDEOMIXER_HANDLE ));
		}
	}
	
	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk)
{
	if (riid == IID_IMcVideoPresenter)
	{
		EnterCriticalSection(&m_csPresent);
		m_pVRPresent = reinterpret_cast<IMcVideoPresenter*>(pUnk);
		LeaveCriticalSection(&m_csPresent);
		return S_OK;
	}
	return E_FAIL;

}

STDMETHODIMP CLibVideoMixerApp::Prepare(VM_SURFACE_LAYER_ID id, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(ppbPlanes == NULL || ppbPlanes[0]==0 || ppbPlanes[1]==0 || ppbPlanes[2]==0)
		return E_FAIL;

	if(pdwStrides == NULL)
		return E_FAIL;

	LPBYTE pBuf = (LPBYTE)m_hSurface[id].pBuf;

#if 0
	DWORD dwStrides[3];
	pdwStrides[0] = m_hSurface[id].dwWidth;
	pdwStrides[1] = m_hSurface[id].dwWidth>>1;
	pdwStrides[2] = m_hSurface[id].dwWidth>>1;
#endif

	switch(m_hSurface[id].input_fourCC)
	{
		case MAKEFOURCC('Y','U','Y','2'):
		case MAKEFOURCC('Y','U','Y','V'):
			PrepareBufferYUY2(pBuf, m_hSurface[id].dwWidth, ppbPlanes, pdwStrides, m_hSurface[id].dwWidth, m_hSurface[id].dwHeight);
			break;
		case MAKEFOURCC('Y','V','1','2'):
			PrepareBufferYV12(pBuf, m_hSurface[id].dwWidth, ppbPlanes, pdwStrides,  m_hSurface[id].dwWidth, m_hSurface[id].dwHeight);
			break;
		default:
			break;
	}

	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::ColorConvertion_Resize(void* pInput,
	void* pOutput,DWORD input_width, DWORD input_height, DWORD input_format,
	DWORD output_width, DWORD output_height, DWORD output_format, 
	RECT* pDst, DWORD dwColor)
{
	if((pInput == NULL)||(pOutput == NULL)||(pDst == NULL))
		return E_FAIL;
	
	CI_IMAGEPROC_SIZE sSrcSize,sDstSize;
	memset(&sSrcSize, 0,sizeof(sSrcSize));
	memset(&sDstSize, 0,sizeof(sDstSize));
	
	if(pDst->right > (LONG)output_width)
		pDst->right =  (LONG)output_width;
	if(pDst->bottom > (LONG)output_height)
		pDst->bottom =  (LONG)output_height;
	
	sSrcSize.s32Width = input_width;
	sSrcSize.s32Height = input_height;

	sDstSize.s32Width = pDst->right - pDst->left;
	sDstSize.s32Height = pDst->bottom - pDst->top;
			
	if((input_format == MAKEFOURCC('Y','V','1','2'))&&(output_format == MAKEFOURCC('Y','V','1','2')))
	{
		unsigned char *SrcBuf[3];
		SrcBuf[0] = (unsigned char *)pInput;
		SrcBuf[1] = (unsigned char *)pInput + input_width*input_height;
		SrcBuf[2] = (unsigned char *)pInput + input_width*input_height*5/4;

		int srcStride[3]; 
		srcStride[0] = (int)input_width;
		srcStride[1] = (int)input_width>>1;
		srcStride[2] = (int)input_width>>1;

		unsigned char *DstBuf[3];
		DWORD srcOffset = pDst->top * output_width + pDst->left;
		DstBuf[0] = (unsigned char *)pOutput + srcOffset;
		DstBuf[2] = (unsigned char *)pOutput + output_width*output_height + srcOffset/4 ;
		DstBuf[1] = (unsigned char *)pOutput + output_width*output_height*5/4 + srcOffset/4;

		int dstStride[3]; 
		dstStride[0] = (int)output_width;
		dstStride[1] = (int)output_width>>1;
		dstStride[2] = (int)output_width>>1;

		return (m_PrepareData.PrepareBufferYV12Scaled(SrcBuf,srcStride, sSrcSize, DstBuf, 
			dstStride, sDstSize, CI_IMAGEPROC_ROTATION_DISABLE,VP_FLIPVH_NO));
	}

	return E_FAIL;
}

STDMETHODIMP CLibVideoMixerApp::TransparentOverwrite(void* pBackground,void* pSrc,DWORD dwSrcTransColor, RECT* pDst, FLOAT alpha)
{
	if(pBackground== NULL || pSrc==NULL || pDst == NULL)
		return E_FAIL;
	unsigned char* background_buf[3];
	unsigned char* src_buf[3];
	DWORD offset = (pDst->top*m_openOptions.dwWidth + pDst->left)*m_bytes_per_pixcel; 
	DWORD offset2 = (pDst->top*m_openOptions.dwWidth/4 + pDst->left/2)*m_bytes_per_pixcel; 
	background_buf[0] = (unsigned char*)LPBYTE(pBackground)+ offset;
	background_buf[1] = (unsigned char*)LPBYTE(pBackground) + m_openOptions.dwWidth *m_openOptions.dwHeight +  offset2;
	background_buf[2] = (unsigned char*)LPBYTE(pBackground) + m_openOptions.dwWidth *m_openOptions.dwHeight*5/4 + offset2;
	
	src_buf[0] = (unsigned char*)LPBYTE(pSrc)+ offset ;
	src_buf[1] = (unsigned char*)LPBYTE(pSrc)+ m_openOptions.dwWidth *m_openOptions.dwHeight +  offset2;
	src_buf[2] = (unsigned char*)LPBYTE(pSrc)+ m_openOptions.dwWidth *m_openOptions.dwHeight *5/4+  offset2;

	m_pAlphaBlend->SetAlphaBlendingYUV(pDst->right - pDst->left, pDst->bottom- pDst->top,
		pDst->right - pDst->left, pDst->bottom- pDst->top,
		0,0,0,0,0,alpha);

	if(m_openOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
	{
		unsigned char y = 0,u = 0,v = 0;
		y =(unsigned char)((dwSrcTransColor&0xff0000)>>16);
		u =(unsigned char)((dwSrcTransColor&0xff00)>>8);
		v =(unsigned char)dwSrcTransColor&0xff;
		m_pAlphaBlend->SetAlphaBlendingBackgroundColor(y,u,v);
	}

	m_pAlphaBlend->DoAlphaBlendingYUV(background_buf[0], background_buf[1], background_buf[2],
		src_buf[0] , src_buf[1] , src_buf[2]);
	return S_OK;
}

STDMETHODIMP CLibVideoMixerApp::PresentBuffer(VM_SURFACE_LAYER_ID id, DWORD dwFlags)
{
	HRESULT res = S_OK;
	if(m_pVRPresent == NULL)
		return E_FAIL;
	//memset(m_pMainBuf,0,m_openOptions.dwHeight*m_openOptions.dwWidth*m_bytes_per_pixcel*3/2);


//Save memcpy on 1 surface
	int surface_num = 0;

	for(int i=MAX_SURFACE_NUM-1;i>=0;i--)
	{
		if(m_hSurface[i].bEnabled)
			surface_num++;
	}

	if(surface_num == 1)
	{
		for(int i=MAX_SURFACE_NUM-1;i>=0;i--)
		{
			if(m_hSurface[i].bEnabled && (m_openOptions.dwWidth == m_hSurface[i].dwWidth) && (m_openOptions.dwHeight == m_hSurface[i].dwHeight))
			{
				LPBYTE curr[3];
				curr[0] = (LPBYTE )m_hSurface[i].pBuf;
				curr[2] = LPBYTE(m_hSurface[i].pBuf) + m_hSurface[i].dwWidth*m_hSurface[i].dwHeight;
				curr[1] = LPBYTE(m_hSurface[i].pBuf) + m_hSurface[i].dwWidth*m_hSurface[i].dwHeight*5/4;

				DWORD strides[3];
				strides[0] = m_hSurface[i].dwWidth;
				strides[1] = strides[2] = m_hSurface[i].dwWidth>>1;

				m_pVRPresent->Present(curr,strides, 0);
				return S_OK;
			}

		}
	}

//Multi-Surface Condition
	DWORD iSurface = 0;
	for(int i=MAX_SURFACE_NUM-1;i>=0;i--)
	{
		if(m_hSurface[i].bEnabled)
		{
			iSurface++;
			
			if(iSurface  == 1)
			{
				res = ColorConvertion_Resize(m_hSurface[i].pBuf, m_pMainBuf, m_hSurface[i].dwWidth, m_hSurface[i].dwHeight, m_hSurface[i].input_fourCC, 
					m_openOptions.dwWidth, m_openOptions.dwHeight, m_mainSurf_FourCC ,&m_hSurface[i].rDst, m_hSurface[i].dwTranColor);
				if(FAILED(res))
					return E_FAIL;
			}
			else
			{
				if(m_hSurface[i].rDst.right - m_hSurface[i].rDst.left != (LONG)m_openOptions.dwWidth)
					memcpy(m_pCCResizeBuf,m_pMainBuf, m_openOptions.dwHeight*m_openOptions.dwWidth*m_bytes_per_pixcel*3/2);

				res = ColorConvertion_Resize(m_hSurface[i].pBuf, m_pCCResizeBuf, m_hSurface[i].dwWidth, m_hSurface[i].dwHeight, m_hSurface[i].input_fourCC, 
					m_openOptions.dwWidth, m_openOptions.dwHeight, m_mainSurf_FourCC ,&m_hSurface[i].rDst, m_hSurface[i].dwTranColor);
				if(FAILED(res))
					return E_FAIL;

				res = TransparentOverwrite(m_pMainBuf,m_pCCResizeBuf, m_hSurface[i].dwTranColor, &m_hSurface[i].rDst, m_hSurface[i].fAlpha);
				if(FAILED(res))
					return E_FAIL;
			}
		}
	}

	LPBYTE curr[3];
	curr[0] = (LPBYTE )m_pMainBuf;
	curr[1] = LPBYTE(m_pMainBuf) + m_openOptions.dwWidth*m_openOptions.dwHeight;
	curr[2] = LPBYTE(m_pMainBuf) + m_openOptions.dwWidth*m_openOptions.dwHeight*5/4;

	DWORD strides[3];
	strides[0] = m_openOptions.dwWidth;
	strides[1] = strides[2] = m_openOptions.dwWidth>>1;

	m_pVRPresent->Present(curr,strides, 0);
	return S_OK;
}


IMcVideoPresenterMixer *CreateFrameMixer()
{
	return new CLibVideoMixerApp(); 
}

