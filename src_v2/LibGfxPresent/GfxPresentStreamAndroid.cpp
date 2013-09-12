//===================================== ========================================
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

#include "GfxPresentStreamAndroid.h"
#include "../../inc/dp.h"


CVRStmAndroid::CVRStmAndroid(VP_OPEN_STREAM_PARAMS *pOpenParams):
m_bEnable(false),
m_deinterlace(VP_DEINTERLACE_WEAVE)
{
  if(!pOpenParams)    
  {
    DP("CVRStmAndroid::CVRStmAndroid invalid pOpenParams!!!");
    return ;
  }
  memcpy(&m_open_param, pOpenParams, sizeof(m_open_param));
  m_bOpen = false;
  return;
}

CVRStmAndroid::~CVRStmAndroid()
{
  return;
}

STDMETHODIMP CVRStmAndroid::Open()
{
  HRESULT hr;
  if(m_bOpen)	return E_UNEXPECTED;

  	m_pStream = CreateAndroidSF();
    if(!m_pStream)
    {
      DP("failed! File:%s L:%d \n",__FILE__, __LINE__);
      return E_FAIL;
    }

  DWORD foucc_arr[2] = {m_open_param.dwFourCC,0};
    
  if(FAILED(m_pStream->Open(m_open_param.dwWidth, m_open_param.dwHeight, 0,(const DWORD *)foucc_arr)))
  {
      foucc_arr[0] = MAKEFOURCC('R','5','6','5');
    if(FAILED(m_pStream->Open(m_open_param.dwWidth, m_open_param.dwHeight, 0,(const DWORD *)foucc_arr)))
    {
          DP("failed! File:%s L:%d \n",__FILE__, __LINE__);
        return E_FAIL;
    }
  }

  SetRect(&m_rDst, 0, 0,m_open_param.dwWidth, m_open_param.dwHeight );    
  if(FAILED(m_pStream->SetDstRect(&m_rDst)))
  {
      DP("failed! File:%s L:%d \n",__FILE__, __LINE__);
    return E_FAIL;
  }

  memset(&m_stream_info, 0,sizeof(m_stream_info));
  m_stream_info.dwSize = sizeof(m_stream_info);
  if(FAILED(m_pStream->GetSurfaceInfo(&m_stream_info)))
  {
      DP("failed! File:%s L:%d \n",__FILE__, __LINE__);
    return E_FAIL;
  }
  
  m_deinterlace  = VP_DEINTERLACE_WEAVE;
  m_bOpen = true;
  return S_OK;
};

STDMETHODIMP CVRStmAndroid::Close()
{
  if(!m_bOpen)	return E_UNEXPECTED;

  if(!m_pStream)
  {
    m_pStream->Close();
    m_pStream = NULL;
  }
  m_bOpen = false;
  return S_OK;
};

STDMETHODIMP CVRStmAndroid::Get(IN REFGUID guidPropSet, IN DWORD dwPropID,
IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
  if(pPropData==0)
   return E_POINTER;

  if(!m_bOpen)	return E_UNEXPECTED;

  HRESULT hr= E_NOTIMPL;
  switch(dwPropID)
  {
    case MC_PROPID_VP_ENABLE:
        *reinterpret_cast<bool *>(pPropData) = m_bEnable;
        if(pcbReturned)
        	*pcbReturned = sizeof(RECT);
        hr = S_OK;
        break;
    case MC_PROPID_VP_DEINTERLACE_MODE:
      *reinterpret_cast<VP_DEINTERLACE_MODE*>(pPropData) = m_deinterlace ;
          hr = S_OK;
          break;
    default:
      break;
  }
  return hr;
}

STDMETHODIMP CVRStmAndroid::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, 
IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
  HRESULT hr = E_NOTIMPL;
  if(!m_bOpen)	return E_UNEXPECTED;
  if(dwPropID!=MC_PROPID_VP_CLEAR && !pPropData)	return E_INVALIDARG;

  switch (dwPropID)
  {
  	case MC_PROPID_VP_RECTDST:
      DP(" %s MC_PROPID_VP_RECTDST\n",__FUNCTION__);
      m_rDst =*reinterpret_cast<RECT *>(pPropData);
      hr = m_pStream->SetDstRect(&m_rDst);
      break;
      
    case MC_PROPID_VP_ENABLE:
      DP(" %s MC_PROPID_VP_ENABLE\n",__FUNCTION__);
      m_bEnable = *(reinterpret_cast<bool*>(pPropData));
      hr = m_pStream->Show(m_bEnable);
      break;

	case MC_PROPID_VP_ANDROID_ISURFACE:
      DP(" %s MC_PROPID_VP_ANDROID_ISURFACE \n",__FUNCTION__);
		hr = m_pStream->SetHWRenderSurf(pPropData);
      break;        
      
    case MC_PROPID_VP_CLEAR:
      if(m_open_param.dwFourCC == MAKEFOURCC('Y','V','1','2'))
      {
        //Malloc Buffer & Feed black screen data				
        LPBYTE pbuf = NULL;
        pbuf = (LPBYTE)malloc(m_open_param.dwWidth*m_open_param.dwHeight*3/2);
        if(pbuf == NULL)
        	return E_FAIL;
        char y,u,v;
        if(!pPropData)
        {
        	y = (char)0x00;					u = (char)0x80;					v = (char)0x80;	
        }
        else
        {
        	DWORD yuv = *reinterpret_cast<DWORD*>(pPropData);
        	y = (char)((yuv >>16)&0xff);
        	u = (char)((yuv >>8)&0xff);
        	v = (char)((yuv )&0xff);
        }
        memset(pbuf, y,m_open_param.dwWidth*m_open_param.dwHeight);
        memset(pbuf+m_open_param.dwWidth*m_open_param.dwHeight, u,m_open_param.dwWidth*m_open_param.dwHeight/4);
        memset(pbuf+m_open_param.dwWidth*m_open_param.dwHeight*5/4, v,m_open_param.dwWidth*m_open_param.dwHeight/4);

        //Present
        DWORD strides[3];
        strides[0]= m_open_param.dwWidth; 
        strides[1] = strides[2] = m_open_param.dwWidth/2;
        LPBYTE bufStrides[3];
        bufStrides[0] = pbuf; 
        bufStrides[1] = pbuf+m_open_param.dwWidth*m_open_param.dwHeight;
        bufStrides[2] = pbuf+m_open_param.dwWidth*m_open_param.dwHeight*5/4;
        Present(bufStrides, strides,NULL);
        //FreeBuffer
        free(pbuf);
        hr = S_OK;
      }
      else if(m_open_param.dwFourCC == MAKEFOURCC('R','5','6','5'))
      {
        //Malloc Buffer & Feed black screen data				
          LPBYTE pbuf = NULL;
          long bufferSize = m_open_param.dwWidth*m_open_param.dwHeight*2;
          pbuf = (LPBYTE)malloc(bufferSize);
          if(pbuf == NULL)
          	return E_FAIL;

          char rgb;
          if(!pPropData)
          	rgb = (char)0xff;
          else
          	rgb = *reinterpret_cast<char*>(pPropData);

          memset(pbuf, rgb,bufferSize);

          //Present
          DWORD strides[3];
          strides[0] = strides[1] = strides[2] = m_open_param.dwWidth; 
          LPBYTE bufStrides[3];
          bufStrides[0] = bufStrides[1] = bufStrides[2] = pbuf; 
          Present(bufStrides, strides,NULL);

          free(pbuf);
          hr = S_OK;
      }
      break;
      
    case MC_PROPID_VP_DEINTERLACE_MODE:
      m_deinterlace  = *(reinterpret_cast<VP_DEINTERLACE_MODE*>(pPropData));
      hr = S_OK;
      break;

    default:
      break;
  }
  return hr;
}

STDMETHODIMP CVRStmAndroid::LockSurface(LPBYTE *ppData, DWORD *pdwStride)
{
  if(!m_bOpen)	return E_UNEXPECTED;
  return (m_pStream->Lock(0,(LPVOID *)ppData,(LONG*)pdwStride,0));
}

STDMETHODIMP CVRStmAndroid::UnlockSurface()
{
  if(!m_bOpen)	return E_UNEXPECTED;

  return (m_pStream->Unlock(0));
}

STDMETHODIMP CVRStmAndroid::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
  LPBYTE pBuf = NULL;
  DWORD stride = 0;
  HRESULT hr  = S_OK;

  if(!m_bOpen)	return E_UNEXPECTED;

  hr = LockSurface(&pBuf , &stride);
  if(FAILED(hr))	return hr;

  DWORD srcWidth[3];
  DWORD srcHeight[3];
  srcWidth[0] = m_open_param.dwWidth;
  srcWidth[1] = srcWidth[2] = m_open_param.dwWidth>>1;

  srcHeight[0] = m_open_param.dwHeight;
  srcHeight[1] = srcHeight[2] = m_open_param.dwHeight>>1;

  if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
  {
    if(m_stream_info.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
    {
        hr  = m_PrepareData.PrepareBufferYV12(pBuf, stride, 
			ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, 0,0,0,m_deinterlace);
    }

  else if(m_stream_info.dwFourCC == MAKE_FOURCC('R','5','6','5'))
    {
        hr  = m_PrepareData.PrepareBufferRGB565(pBuf, stride, ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, 0);
    }

  }

  hr = UnlockSurface();
  if(FAILED(hr))	return hr;

  m_pStream->Flip(0,0);
  return S_OK;
}

STDMETHODIMP CVRStmAndroid::StartAnimation(VR_ANIMATION_PARAMS* pParms)
{
  return E_NOTIMPL;
}

STDMETHODIMP CVRStmAndroid::StopAnimation()
{
  return E_NOTIMPL;
}


