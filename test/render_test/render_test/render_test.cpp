// render_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>

#include <ddraw.h>
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")


IDirectDraw4		*m_pDDraw;
IDirectDrawSurface4 *m_pPrimary, *m_pOverlay, **m_pBackBuffer;
DWORD m_dwBackBuffers= 0;

HRESULT WINAPI EnumSurfaceCallback(LPDIRECTDRAWSURFACE4 pSurf, LPDDSURFACEDESC2 pDesc, LPVOID lpContext)
{
	m_pBackBuffer[m_dwBackBuffers++] = pSurf;
	return DDENUMRET_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{

	if(FAILED(DirectDrawCreate(0,(LPDIRECTDRAW*)&m_pDDraw,0)))
		return E_FAIL;

	if(FAILED(m_pDDraw->SetCooperativeLevel(0,DDSCL_NORMAL)))
		return E_FAIL;


	DDSURFACEDESC2 ddsd;
	/* create primary */
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if(FAILED(m_pDDraw->CreateSurface(&ddsd,&m_pPrimary,0)))
	{
		return E_FAIL;
	}


		//Init parameters
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY;
	ddsd.dwFlags |= DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps |= DDSCAPS_FLIP  ;
	#ifdef DDSCAPS_COMPLEX
	ddsd.ddsCaps.dwCaps |= DDSCAPS_COMPLEX;
	#endif

	ddsd.dwFlags |= DDSD_BACKBUFFERCOUNT;

	ddsd.dwWidth = 320;
	ddsd.dwHeight = 240;

	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
	ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','U','Y','V');

	ddsd.dwBackBufferCount = 2;
	HRESULT hr = m_pDDraw->CreateSurface(&ddsd,&m_pOverlay,0);
	if(FAILED(hr))
		return -1;

	IDirectDrawSurface4 *pSurf;

	m_pBackBuffer = new LPDIRECTDRAWSURFACE4[2];
	m_dwBackBuffers = 0;
	/* get back buffers */
	for(pSurf=m_pOverlay;m_dwBackBuffers<(unsigned)2;)
	{
		if(FAILED(pSurf->EnumAttachedSurfaces(0,EnumSurfaceCallback)))
			break;
		pSurf = m_pBackBuffer[m_dwBackBuffers-1];
	}



	DDOVERLAYFX			m_OFX;
	ZeroMemory(&m_OFX,sizeof(m_OFX));
	m_OFX.dwSize = sizeof(m_OFX);

	RECT rs,rd;
	SetRect(&rs, 0,0,320,240);
	SetRect(&rd, 0,0,320,240);
	hr = m_pOverlay->UpdateOverlay(&rs, m_pPrimary, &rd, DDOVER_SHOW, &m_OFX);
	if(FAILED(hr))	
		return E_FAIL;


	DWORD dwFlags = 0;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
#ifdef DDLOCK_WAIT
	dwFlags |= DDLOCK_WAIT;
#endif

	DWORD surface_id =0;

	DWORD ap_idle_st = GetIdleTime();
	DWORD ap_tick_st = GetTickCount();
	DWORD start_tick = GetTickCount();
	DWORD frameCount = 0;

	while(GetTickCount() - ap_tick_st <10000)	//Test 10 second.
	{
		start_tick = GetTickCount();
		pSurf = m_pBackBuffer[surface_id];
		pSurf->Lock(0, &ddsd, dwFlags, 0);
		pSurf->Unlock(0);


		m_pOverlay->Flip(pSurf,0);

		if(surface_id<1)
			surface_id++;
		else
			surface_id = 0;

		int sleep_tick = 40 - (GetTickCount() - start_tick);
		if(sleep_tick>0)
			Sleep(sleep_tick);
		frameCount++;
	}

	DWORD ap_idle_ed = GetIdleTime();
	DWORD ap_tick_ed = GetTickCount();

	printf("Player duration %dms  CPU usage %2.2f%%  frm_rate :%f  \n", ap_tick_ed-ap_tick_st, 
		(1.00-(((float)ap_idle_ed-(float)ap_idle_st)/((float)ap_tick_ed-(float)ap_tick_st)))*100,(float)((float)frameCount/(float)((float)((float)ap_tick_ed-(float)ap_tick_st)/1000)));

	delete pBuf;
	m_pOverlay->Release();
	m_pPrimary->Release();
	m_pDDraw->Release();

	TCHAR logBuf[MAX_PATH];
		_stprintf(logBuf, _T("Player duration %dms  CPU usage %2.2f%%  frm_rate :%f  \n"),ap_tick_ed-ap_tick_st, 
		(1.00-(((float)ap_idle_ed-(float)ap_idle_st)/((float)ap_tick_ed-(float)ap_tick_st)))*100,(float)((float)frameCount/(float)((float)((float)ap_tick_ed-(float)ap_tick_st)/1000)));
	MessageBox(0,logBuf, _T("INFO"), MB_OK);

	return 0;
}

