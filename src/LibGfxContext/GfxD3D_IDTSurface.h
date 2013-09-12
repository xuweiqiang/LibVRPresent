// IDTSurface.h: interface for the IDTSurface class.

// Code by Karel Donk
// Contact me at karel@miraesoft.com for more information, or visit my homepage at
// http://www.miraesoft.com/karel/ from more codesamples.
// May be modified and used freely

#pragma once

#include "GfxD3D_idtpixelformat.h"

class GfxD3D_IDTSurface  
{
public:
	HRESULT BltFast(LPDIRECT3DSURFACE9 Dest, int x, int y, RECT srcRect);
	//HRESULT CreateOffScreenSurface(LPCTSTR FileName, D3DFORMAT PixelFormat);
	LPDIRECT3DSURFACE9 GetD3DSurface();
	HRESULT Initialize(LPDIRECT3DDEVICE9 pD3DDevice);
	HRESULT UnInitiallize();

	HRESULT CreateSurface(UINT Width, UINT Height, D3DFORMAT PixelFormat);
	HRESULT DestroySurface();

	GfxD3D_IDTSurface();
	virtual ~GfxD3D_IDTSurface();
	HRESULT Lock(void** pBuf,LONG *pStride);
	HRESULT UnLock();
	HRESULT BltStretch(LPDIRECT3DSURFACE9 Dest,RECT srcRect, RECT dstRect);

protected:
	GfxD3D_IDTPixelFormat m_PixelFormat;
	LPDIRECT3DDEVICE9 m_D3DDevice;
	LPDIRECT3DSURFACE9 m_D3DSurface;

	UINT m_Width;
	UINT m_Height;
	D3DFORMAT m_Format;
	CRITICAL_SECTION m_csLockUnlock; 

};

typedef GfxD3D_IDTSurface DT_SURFACE, *LPDT_SURFACE;
