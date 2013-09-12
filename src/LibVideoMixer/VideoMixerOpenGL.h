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
// 	Copyright (c) 2010  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <../../inc/McCom.h>
#include "VideoMixerOpenGLGLee.h"
#include <../../3rdparty/freeglut_lib/GL/glut.h>
#include <../../3rdparty/freeglut_lib/GL/freeglut.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/DMCP.h"
#include "../../inc/dp.h"
#include "../../inc/GfxVideoWindow.h"
#include "VideoMixerOpenGLTM.h"
#pragma warning(disable: 4244)	// disable truncation from LONG_PTR to LONG


class COpenGLVideoContext : 
	public CMcUnknown,
	public ICivmVideoContext
{
public:

	COpenGLVideoContext(HWND hWndParent, HWND hWndNotify, DWORD dwNotifyID, RECT *prPos);
	
	virtual ~COpenGLVideoContext();
	
	DECLARE_IUNKNOWN;

	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid==IID_ICivmVideoContext)
			return GetInterface(static_cast<ICivmVideoContext*>(this),ppvObj);
		return CMcUnknown::NonDelegatingQueryInterface(riid,ppvObj);
	}
	STDMETHOD(GetPos2D)(RECT *prPos);
	
	STDMETHOD(SetPos2D)(RECT *prPos);
	
	STDMETHOD(Show)(BOOL bShow);
	
	STDMETHOD(BringToTop2D)();
	
	STDMETHOD_(HWND,GetHwnd)()
	{
		return 0;
	}
	STDMETHOD(Connect)(REFIID riid, LPUNKNOWN pUnk);
	
	STDMETHOD(DisConnect)(REFIID riid, LPUNKNOWN pUnk) ;
	
	STDMETHOD(PrepareFrame)(void); 
	STDMETHOD(Get)(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData) ;
	STDMETHOD(Set)(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData) ;
	STDMETHOD(Lock)( LPBYTE *ppData, DWORD *pdwStride) ;
	STDMETHOD(Unlock)();

	STDMETHOD(GetActualPos2D)(RECT* pDst);
	STDMETHOD(StartShakeEffect)();
	STDMETHOD(StopShakeEffect)();

protected:
	LRESULT Notify(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(m_dwNotifyID && m_hWndNotify)
			::PostMessage(m_hWndNotify, m_dwNotifyID, message, (LPARAM)static_cast<ICivmVideoContext *>(this));
		return 0;
	}
	static LRESULT CALLBACK VideoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}

	HWND m_hWndNotify;
	//WNDPROC m_pVideoWndProcChain;
	DWORD m_dwNotifyID;
	BOOL m_bShow;
	RECT m_rLastPos;
	RECT m_rPos;
	int m_textureId;
	unsigned char* m_pBuff;
	VP_OpenOptions m_openOptions;
	bool m_contentUpdated;
	CRITICAL_SECTION m_csDisplay; 
	IMcVideoPresenter* m_pVideoPresenter;
	float m_zOrder;
	VP_ZOOM_MODE m_lZoomMode;
	IMcVideoWindow* m_pVideoWindow;
	CTextureManager	m_textureMgr;
	int m_bpp;
	bool m_bBringToTop;
	bool m_bPopUp;
	LARGE_INTEGER m_tSetposStart;
	LONG m_setposSpeed;// complete time. msec

	LARGE_INTEGER m_coutFreq;
};


class CVideoMixerOpenGL : 
	public CMcUnknown,
	public ICivmVideoMixer
{
public:
	CVideoMixerOpenGL();
	
	virtual ~CVideoMixerOpenGL();
	
	DECLARE_IUNKNOWN;
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid==IID_ICivmVideoMixer)
			return GetInterface(static_cast<ICivmVideoMixer*>(this),ppvObj);
		return CMcUnknown::NonDelegatingQueryInterface(riid,ppvObj);
	}
	
	STDMETHOD(Open)(HWND hWndParent, DWORD dwNotifyID, DWORD dwMixerType, RECT *prPort);
	STDMETHOD(Close)();
	STDMETHOD(Display)();
	STDMETHOD(CreateVideoContext)(ICivmVideoContext **ppVideoContext, DWORD dwVideoType, RECT *prPos);
	STDMETHOD(DestroyVideoContext)(ICivmVideoContext *ppVideoContext);
	STDMETHOD(GetViewPort)(RECT *prPort);
	STDMETHOD(SetViewPort)(RECT *prPort);
	STDMETHOD(Show)(BOOL bShow);
	STDMETHOD(Connect)(REFIID riid, LPUNKNOWN pUnk);
	STDMETHOD(DisConnect)(REFIID riid, LPUNKNOWN pUnk) ;
	STDMETHOD(Get)(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData);
	STDMETHOD(Set)(IN DWORD dwPropID, IN LPVOID pPropData, IN DWORD cbPropData);

	HWND m_hWndParent;
	CRITICAL_SECTION m_csDisplay; 
	HRESULT GetInitRect(RECT* pRect);
	RECT m_rViewPort;

protected:

#define MAX_CONTEXT_NUM	16	
	HRESULT SetViewPortPri(RECT* prPort);
	HRESULT SetContextDest(RECT* pDst);


	DWORD m_dwNotifyID;
	BOOL m_bShow;
	BOOL m_bCanDisp;
	COpenGLVideoContext* m_contextArrary[MAX_CONTEXT_NUM];
//	IMcVideoPresenter* m_pVideoPresenter;

	IDmcpDisplay *m_pDMCP;
	DWORD 	m_DMCP_ACP_flag;
	DWORD 	m_DMCP_CGMS_flag;
	DWORD 	m_DMCP_HDCP_flag;
	bool m_bOpen;
	HANDLE m_hThread;
};

