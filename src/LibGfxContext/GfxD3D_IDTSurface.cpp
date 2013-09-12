// GfxD3D_IDTSurface.cpp: implementation of the GfxD3D_IDTSurface class.

// Code by Karel Donk
// Contact me at karel@miraesoft.com for more information, or visit my homepage at
// http://www.miraesoft.com/karel/ from more codesamples.
// May be modified and used freely
#include "../../inc/GfxBuildDef.h"

#ifdef GFX_D3D

#include "GfxD3D_IDTSurface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GfxD3D_IDTSurface::GfxD3D_IDTSurface()
{
	m_D3DDevice = NULL;
	m_D3DSurface = NULL;

	m_Width = 0;
	m_Height = 0;
	m_Format = D3DFMT_UNKNOWN;
	InitializeCriticalSection(&m_csLockUnlock);

}

GfxD3D_IDTSurface::~GfxD3D_IDTSurface()
{
	if (m_D3DSurface != NULL) 
	{
		m_D3DSurface->Release();
		m_D3DSurface = NULL;
	}

	m_Width = 0;
	m_Height = 0;
	m_Format = D3DFMT_UNKNOWN;
	DeleteCriticalSection(&m_csLockUnlock);

}

HRESULT GfxD3D_IDTSurface::DestroySurface()
{
	if (m_D3DDevice == NULL) return DT_NOT_INITIALIZED;
	if (m_D3DSurface == NULL) return DT_NOT_INITIALIZED;

	if (m_D3DSurface != NULL) 
	{
		m_D3DSurface->Release();
		m_D3DSurface = NULL;
	}

	m_Width = 0;
	m_Height = 0;
	m_Format = D3DFMT_UNKNOWN;

	return DT_OK;
}

HRESULT GfxD3D_IDTSurface::CreateSurface(UINT Width, UINT Height, D3DFORMAT PixelFormat)
{
	if (m_D3DDevice == NULL) return DT_NOT_INITIALIZED;

	if (m_D3DDevice->CreateOffscreenPlainSurface(Width, Height, PixelFormat, D3DPOOL_DEFAULT, &m_D3DSurface, NULL) != D3D_OK) return DT_ERROR;

	m_Width = Width;
	m_Height = Height;
	m_Format = PixelFormat;

	return DT_OK;
}

// Should be called to set the D3DDevice before doing anything with the surface
HRESULT GfxD3D_IDTSurface::Initialize(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == NULL) return DT_NOT_INITIALIZED;

	m_D3DDevice = pD3DDevice;

	return DT_OK;
}

HRESULT GfxD3D_IDTSurface::UnInitiallize()
{
	return DT_OK;
}

LPDIRECT3DSURFACE9 GfxD3D_IDTSurface::GetD3DSurface()
{
	return m_D3DSurface;
}

// And now for the beaf!
HRESULT GfxD3D_IDTSurface::BltStretch(LPDIRECT3DSURFACE9 Dest,RECT srcRect, RECT dstRect)
{
	if (m_D3DDevice == NULL) return DT_NOT_INITIALIZED;
	if (m_D3DSurface == NULL) return DT_NOT_INITIALIZED;
	if (Dest == NULL) return DT_ERROR;

	int ret = m_D3DDevice->StretchRect(m_D3DSurface,&srcRect,Dest, &dstRect,D3DTEXF_LINEAR );
	if (ret != D3D_OK) return ret;

	return DT_OK;
}

// And now for the beaf!
HRESULT GfxD3D_IDTSurface::BltFast(LPDIRECT3DSURFACE9 Dest, int x, int y, RECT srcRect)
{
	if (m_D3DDevice == NULL) return DT_NOT_INITIALIZED;
	if (m_D3DSurface == NULL) return DT_NOT_INITIALIZED;
	if (Dest == NULL) return DT_ERROR;

	// Clipping
	if (srcRect.left < 0) srcRect.left = 0;
	if (srcRect.left > (int)(m_Width)) srcRect.left = (m_Width);
	if (srcRect.top < 0) srcRect.top = 0;
	if (srcRect.top > (int)(m_Height)) srcRect.top = (m_Height);

	if (srcRect.right < srcRect.left) srcRect.right = srcRect.left;
	if (srcRect.right > (int)(m_Width)) srcRect.right = (m_Width);
	if (srcRect.bottom < srcRect.top) srcRect.bottom = srcRect.top;
	if (srcRect.bottom > (int)(m_Height)) srcRect.bottom = (m_Height);

	UINT nHeight = srcRect.bottom-srcRect.top;
	UINT nWidth = srcRect.right-srcRect.left;

	D3DSURFACE_DESC sdesc;
	Dest->GetDesc(&sdesc);

	// Clipping
	if (x < 0)
	{	
		nWidth += x;
		x = 0;
	}
    if (x+nWidth > sdesc.Width)
	{
		nWidth = sdesc.Width-x;
	}
	if (y < 0)
	{
		nHeight += y;
		y = 0;
	}
	if (y+nHeight > sdesc.Height)
	{
		nHeight = sdesc.Height-y;
	}

	// Return if we don't have anything to blit
	if ((nHeight <= 0) || (nWidth <= 0)) return DT_OK;

	RECT destRect; destRect.left=x; destRect.top=y; destRect.right=x+nWidth; destRect.bottom=y+nHeight;

	// Get source and destination pixelformats
	// Optimization: Remove this and do it somewhere else where you can do it once since it makes things slower.
	DT_PIXELFMT spFmt, dpFmt;
	int ret = m_PixelFormat.GetDTPixelFormat(m_Format, &spFmt);
	if (ret != DT_OK) return ret;
	ret = m_PixelFormat.GetDTPixelFormat(sdesc.Format, &dpFmt);
	if (ret != DT_OK) return ret;

	D3DLOCKED_RECT slRect;
	D3DLOCKED_RECT dlRect;

	if (m_D3DSurface->LockRect(&slRect, &srcRect, D3DLOCK_READONLY) == D3D_OK)
	{
		if (Dest->LockRect(&dlRect, &destRect, 0) == D3D_OK)
		{
			BYTE* Source = (BYTE*)slRect.pBits;
			BYTE* Dest = (BYTE*)dlRect.pBits;

			for (UINT y1=0; y1<nHeight; y1++)
			{
				BYTE* Source2 = (BYTE*)Source;
				BYTE* Dest2 = (BYTE*)Dest;

				// If the surface formats would be the same, and we just want to do a SRCCOPY blit, we could just do the following to make it fast:
				//*(WORD*)Dest2 = *(WORD*)Source2;

				// But the following code illustrates breaking up a pixel from the source surface in ARGB components,
				// and then reassembling them on the destination surface, after color conversion if needed. With this 
				// code you can add blending effects such as translucency, multiply, overlay etc.
				// In an application with realtime animation, like a game, however you would not want to work with 
				// many different surface formats, because color conversion makes the blit slower. The best 
				// thing to do would be to use one surface format that best suites your needs.
				
				for (UINT x1=0; x1<nWidth; x1++)
				{
					if (spFmt.BitsPerPixel == 16)
					{
						WORD Sp = *(WORD*)Source2;
											
						DWORD alpha = ((Sp & spFmt.ABMask) >> spFmt.AShift);
						DWORD red = ((Sp & spFmt.RBMask) >> spFmt.RShift);
						DWORD green = ((Sp & spFmt.GBMask) >> spFmt.GShift);
						DWORD blue = ((Sp & spFmt.BBMask) >> spFmt.BShift);

						// Color conversion
						if (spFmt.Format != dpFmt.Format)
						{
							alpha = (DWORD) ((float)((float)alpha/(float)spFmt.AMaxVal) * (float)dpFmt.AMaxVal);
							red = (DWORD) ((float)((float)red/(float)spFmt.RMaxVal) * (float)dpFmt.RMaxVal);
							green = (DWORD) ((float)((float)green/(float)spFmt.GMaxVal) * (float)dpFmt.GMaxVal);
							blue = (DWORD) ((float)((float)blue/(float)spFmt.BMaxVal) * (float)dpFmt.BMaxVal);
						}

						if (dpFmt.BitsPerPixel == 16)
						{
  							WORD* Dp = (WORD*)Dest2;
							*Dp = (WORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
						else if (dpFmt.BitsPerPixel > 16)
						{
  							DWORD* Dp = (DWORD*)Dest2;
							*Dp = (DWORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
					}
					else if (spFmt.BitsPerPixel > 16)
					{
						DWORD Sp = *(DWORD*)Source2;
											
						DWORD alpha = ((Sp & spFmt.ABMask) >> spFmt.AShift);
						DWORD red = ((Sp & spFmt.RBMask) >> spFmt.RShift);
						DWORD green = ((Sp & spFmt.GBMask) >> spFmt.GShift);
						DWORD blue = ((Sp & spFmt.BBMask) >> spFmt.BShift);

						// Color conversion
						if (spFmt.Format != dpFmt.Format)
						{
							alpha = (DWORD) ((float)((float)alpha/(float)spFmt.AMaxVal) * (float)dpFmt.AMaxVal);
							red = (DWORD) ((float)((float)red/(float)spFmt.RMaxVal) * (float)dpFmt.RMaxVal);
							green = (DWORD) ((float)((float)green/(float)spFmt.GMaxVal) * (float)dpFmt.GMaxVal);
							blue = (DWORD) ((float)((float)blue/(float)spFmt.BMaxVal) * (float)dpFmt.BMaxVal);
						}
					
						if (dpFmt.BitsPerPixel == 16)
						{
  							WORD* Dp = (WORD*)Dest2;
							*Dp = (WORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
						else if (dpFmt.BitsPerPixel > 16)
						{
  							DWORD* Dp = (DWORD*)Dest2;
							*Dp = (DWORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
					}

					Dest2+=dpFmt.BitsPerPixel/8;
					Source2+=spFmt.BitsPerPixel/8;
				}

				Dest+=dlRect.Pitch;
				Source+=slRect.Pitch;
			}
		}
		else
		{
			m_D3DSurface->UnlockRect();

			return DT_ERROR;
		}

		Dest->UnlockRect();
	}
	else return DT_ERROR;

	m_D3DSurface->UnlockRect();

	return DT_OK;
}

// And now for the beaf!
HRESULT GfxD3D_IDTSurface::Lock(void** pBuf,LONG *pStride)
{
	if ((m_D3DDevice == NULL)||(m_D3DSurface == NULL)) 
		return DT_NOT_INITIALIZED;

	D3DLOCKED_RECT slRect;

	EnterCriticalSection(&m_csLockUnlock);
	if (m_D3DSurface->LockRect(&slRect, NULL, D3DLOCK_DISCARD) == D3D_OK)
	{
		*pBuf = (void*)slRect.pBits;
		*pStride = slRect.Pitch;
	}
	else 
	{
		LeaveCriticalSection(&m_csLockUnlock);
		return DT_ERROR;
	}

	return DT_OK;
}

HRESULT GfxD3D_IDTSurface::UnLock()
{
	if ((m_D3DDevice == NULL) ||(m_D3DSurface == NULL))
	{
		LeaveCriticalSection(&m_csLockUnlock);
		return DT_NOT_INITIALIZED;
	}

	m_D3DSurface->UnlockRect();
	LeaveCriticalSection(&m_csLockUnlock);
	return DT_OK;
}

#endif
