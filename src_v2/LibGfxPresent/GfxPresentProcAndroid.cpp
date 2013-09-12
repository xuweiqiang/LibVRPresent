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

#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/CProtect.h"
#include "tchar.h"
#include "GfxPresentProcAndroid.h"
#include "GfxPresentStreamAndroid.h"


#define VIDEO_PRESENTER_NAME TEXT("Video Presenter2.0")

CVRProcAndroid ::CVRProcAndroid () :
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_Init(false)
{
	InitializeCriticalSection(&m_csDisplay);
   m_Init = false;
}

CVRProcAndroid ::~CVRProcAndroid ()
{
	DeleteCriticalSection(&m_csDisplay);
}	


//-------------------------------------------------------------------
// InitializeWindow
//
// Creates the application window.
//-------------------------------------------------------------------

STDMETHODIMP CVRProcAndroid ::Open(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize)
{
  HRESULT hr = S_OK;
  if(pOpenParams==NULL )	return E_INVALIDARG;
  if(m_Init)
  return E_UNEXPECTED;

  memset(&m_openParams, 0,sizeof(m_openParams));
  memcpy(&m_openParams, pOpenParams, dwOpenSize);
  memset(m_pStmArrar, 0,sizeof(m_pStmArrar));

  m_Init = true;
  return hr;
}

STDMETHODIMP CVRProcAndroid ::Close()
{
  if(!m_Init)
  return E_UNEXPECTED;

  memset(m_pStmArrar, 0,sizeof(m_pStmArrar));

  m_Init = false;
  return S_OK;
}


STDMETHODIMP CVRProcAndroid ::Render(DWORD dwRenderFlags)
{
  return S_OK;
}


STDMETHODIMP CVRProcAndroid ::Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
  return E_PROP_ID_UNSUPPORTED;
}


STDMETHODIMP CVRProcAndroid ::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
  return E_PROP_ID_UNSUPPORTED;
}


STDMETHODIMP CVRProcAndroid ::CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize ,OUT IMcVideoPresenterStream **ppStream)
{
  HRESULT hr = S_OK;
  CProtect proc(&m_csDisplay);
  if(!pOpenParams || !ppStream )  return E_INVALIDARG;
  if(!m_Init)	return VR_FAIL_UNOPEN;

  if(pOpenParams->dwWidth == 0 ||pOpenParams->dwHeight== 0 || pOpenParams->dwFourCC == 0)
    return E_INVALIDARG;

  UINT avalable_idx = 0;
  for(avalable_idx=0;avalable_idx<sizeof(m_pStmArrar)/sizeof(SSTREAM_INFO) ;avalable_idx++)
  {
    if(!m_pStmArrar[avalable_idx].valid)	break;
  }
  if(avalable_idx>= sizeof(m_pStmArrar)/sizeof(SSTREAM_INFO))	return E_UNEXPECTED;

  memcpy(&m_pStmArrar[avalable_idx].param, pOpenParams, sizeof(VP_OPEN_STREAM_PARAMS));
DP("chromeformat:0x%x \n", pOpenParams->dwFourCC);
	if(isSW_YV12(pOpenParams->dwFourCC))
		m_pStmArrar[avalable_idx].param.dwFourCC = MAKE_FOURCC('Y','V','1','2');
	else if(isSW_NV12(pOpenParams->dwFourCC))
		m_pStmArrar[avalable_idx].param.dwFourCC = MAKE_FOURCC('N','V','1','2');
	else if(isSW_RGB565(pOpenParams->dwFourCC))
		m_pStmArrar[avalable_idx].param.dwFourCC = MAKE_FOURCC('R','5','6','5');

  
  m_pStmArrar[avalable_idx].pStream = new CVRStmAndroid(&m_pStmArrar[avalable_idx].param);
  if(!m_pStmArrar[avalable_idx].pStream)	return E_FAIL;

  m_pStmArrar[avalable_idx].valid = true;

  *ppStream = static_cast<IMcVideoPresenterStream*>(m_pStmArrar[avalable_idx].pStream);

  return S_OK;
}


STDMETHODIMP CVRProcAndroid ::DestroyStream(IN IMcVideoPresenterStream *ppStream)
{
  CProtect proc(&m_csDisplay);
  if(!ppStream)	return E_FAIL;
  if(!m_Init)	return VR_FAIL_UNOPEN;

  for(unsigned int i=0;i<sizeof(m_pStmArrar)/sizeof(SSTREAM_INFO);i++)
  {
    if(m_pStmArrar[i].pStream == ppStream)
    {
      m_pStmArrar[i].pStream->Release();
      ZeroMemory(&m_pStmArrar[i], sizeof(SSTREAM_INFO));
      return S_OK;
    }
  }
  return E_FAIL;
}


