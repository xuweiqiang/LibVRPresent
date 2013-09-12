//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// -----------------------------------------------------------------------------
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//  
// -----------------------------------------------------------------------------


#ifndef __DDGPE_H__
#define __DDGPE_H__

#include <gpe.h>
#include "ddrawi.h"


#define SETROPBIT(array,rop) array[(rop>>21)&0x07]|=(1<<((rop>>16)&0x1f))

//#define IN_DISPLAY          (!(IN_VBLANK))
#define WAIT_FOR_NOT_BUSY	{ while(IS_BUSY) {} }
#define WAIT_FOR_VBLANK		{ while( IN_VBLANK ); while( !IN_VBLANK ); } // wait for VBlank to end then wait for VBlank



// some predefined FourCC types
#define FOURCC_YUYV422  MAKEFOURCC('Y', 'U', 'Y', 'V') // 16bpp
#define FOURCC_UYVY422  MAKEFOURCC('U', 'Y', 'V', 'Y') // 16bpp
#define FOURCC_YUY2422  MAKEFOURCC('Y', 'U', 'Y', '2') // 16bpp

enum EDDGPEPixelFormat
{
	ddgpePixelFormat_1bpp = 0,
	ddgpePixelFormat_2bpp,
	ddgpePixelFormat_4bpp,
	ddgpePixelFormat_8bpp,

	ddgpePixelFormat_565,
	ddgpePixelFormat_5551,
	ddgpePixelFormat_4444,
	ddgpePixelFormat_5550,
	ddgpePixelFormat_8880,
	ddgpePixelFormat_8888,
	
	ddgpePixelFormat_YUYV422,
	ddgpePixelFormat_UYVY422,
        ddgpePixelFormat_YUY2422,

	// some generic types
	// Use dwPixelFormatData in conjunction with this info
	ddgpePixelFormat_15bppGeneric,
	ddgpePixelFormat_16bppGeneric,
	ddgpePixelFormat_24bppGeneric,
	ddgpePixelFormat_32bppGeneric,

	ddgpePixelFormat_UnknownFormat,
	// you can assign custom formats starting with this value
	ddgpePixelFormat_CustomFormat = 0x1000
};

#define INSTANTIATE_DDGPE \
const EGPEFormat EDDGPEPixelFormatToEGPEFormat[] =     { /*works for any "defined" value*/ \
                        gpe1Bpp,\
                        gpe2Bpp,\
                        gpe4Bpp,\
			gpe8Bpp,\
			gpe16Bpp,\
			gpe16Bpp,\
			gpe16Bpp,\
			gpe16Bpp,\
			gpe24Bpp,\
			gpe32Bpp,\
			gpe16Bpp,\
			gpe16Bpp,\
                        gpe16Bpp,\
			gpe16Bpp, /*no 15-bit gpe identifier*/\
			gpe16Bpp,\
			gpe24Bpp,\
			gpe32Bpp,\
			gpeUndefined\
};\
/* Defaults only... does not always return the proper mapping */\
const EDDGPEPixelFormat EGPEFormatToEDDGPEPixelFormat[] =     {\
													ddgpePixelFormat_1bpp,\
													ddgpePixelFormat_2bpp,\
													ddgpePixelFormat_4bpp,\
													ddgpePixelFormat_8bpp,\
													ddgpePixelFormat_565,\
													ddgpePixelFormat_8880,\
													ddgpePixelFormat_8888,\
													ddgpePixelFormat_UnknownFormat,\
													ddgpePixelFormat_UnknownFormat,\
													ddgpePixelFormat_UnknownFormat\
												};

extern const EGPEFormat EDDGPEPixelFormatToEGPEFormat[];
extern const EDDGPEPixelFormat EGPEFormatToEDDGPEPixelFormat[];

#define DDGPEStandardHeader		\
	DWORD	dwSize;				\
	DWORD	dwDriverSignature;	\
	DWORD	dwVersion;			\
	/**/						\
	DWORD	dwReserved0;		\
	DWORD	dwReserved1;		\
	DWORD	dwReserved2;		\
	DWORD	dwReserved3


#define GPEMODEEX_VERSION0			(0)
#define GPEMODEEX_CURRENTVERSION	(GPEMODEEX_VERSION0)
struct GPEModeEx
{
	// old-style mode info
	// - must be first in struct!
	GPEMode				modeInfo;

	DDGPEStandardHeader;

	EDDGPEPixelFormat	ePixelFormat;			// pixel format of mode
	DWORD				dwPixelFourCC;			// pixel FourCC code
	DWORD				dwPixelFormatData;		// any user data associated with pixel format

    LONG				lPitch;                 // pitch (in bytes) of mode
    DWORD				dwFlags;                // flags
    DWORD				dwRBitMask;             // red bit mask
    DWORD				dwGBitMask;             // green bit mask
    DWORD				dwBBitMask;             // blue bit mask
    DWORD				dwAlphaBitMask;         // alpha bit mask
};


#define DDGPEBLTPARMS_VERSION0			(0)
#define DDGPEBLTPARMS_CURRENTVERSION	(DDGPEBLTPARMS_VERSION0)
/* extend with private driver data */
typedef struct DDGPEBltParms
{
	// old-style blt parms
	// - must be first in struct!
	GPEBltParms			gpeBltParms;

	// for expansion
	DDGPEStandardHeader;

} DDGPEBltParms;


#define DDGPEALLOCSURFACEDATA_VERSION0			(0)
#define DDGPEALLOCSURFACEDATA_CURRENTVERSION	(DDGPEALLOCSURFACEDATA_VERSION0)
/* extend with private driver data */
typedef struct DDGPEAllocSurfaceData
{
	DDGPEStandardHeader;

	DWORD				dwCreateFlags;
		
	int					nWidth;
	int					nHeight;
	int					nStride;
	int					nSurfaceFlags;
	unsigned char *		pBits;
	EGPEFormat			gpeFormat;
	EDDGPEPixelFormat	ddgpePixelFormat;

} DDGPEAllocSurfaceData;


#define DDGPEDRIVERDATA_VERSION0			(0)
#define DDGPEDRIVERDATA_CURRENTVERSION		(DDGPEDRIVERDATA_VERSION0)
/* extend with private driver data */
typedef struct DDGPEDriverData
{
	DDGPEStandardHeader;

	DWORD			dwDriverFlags;
} DDGPEDriverData;






class DDGPESurf;

class DDGPE : public GPE
{
protected:
	GPEModeEx*		m_pModeEx;

public:
					DDGPE();
   virtual			~DDGPE();

public:

	// Surface memory allocation
	// this function must remain in this position
	virtual SCODE	AllocSurface
						(GPESurf *					*ppSurf,
						int							width,
						int							height,
						EGPEFormat					format,
						int							surfaceFlags )=0;
	virtual SCODE	AllocSurface
						(DDGPESurf *				*ppSurf,
						int							width,
						int							height,
						EGPEFormat					format,
						EDDGPEPixelFormat			pixelFormat,
						int							surfaceFlags );
	virtual SCODE	AllocSurface
						(DDGPESurf *				*ppSurf,
						DDGPEAllocSurfaceData*		pddgpeAllocSurfaceData );

	virtual SCODE	AllocVideoSurface
						(DDGPESurf*					*ppSurf,
						int							width,
						int							height,
						EGPEFormat					format,
						EDDGPEPixelFormat			pixelFormat,
						unsigned long *				pOffsetInVideoMemory );
	virtual SCODE	AllocVideoSurface
						(LPDDRAWI_DDRAWSURFACE_GBL	lpDDSurface,
						int							width,
						int							height,
						EGPEFormat					format,
						EDDGPEPixelFormat			pixelFormat,
						unsigned long *				pOffsetInVideoMemory );
	virtual SCODE	AllocVideoSurface
						(DDGPESurf*					*ppSurf,
						DDGPEAllocSurfaceData*		pddgpeAllocSurfaceData,
						unsigned long *				pOffsetInVideoMemory );
	virtual SCODE	AllocVideoSurface
						(LPDDRAWI_DDRAWSURFACE_GBL	lpDDSurface,
						DDGPEAllocSurfaceData*		pddgpeAllocSurfaceData,
						unsigned long *				pOffsetInVideoMemory );

	// Surface creation
	virtual SCODE	WrapSurface
						(LPDDRAWI_DDRAWSURFACE_GBL	lpDDSurface,
						int							width,
						int							height,
						EGPEFormat					format,
						EDDGPEPixelFormat			pixelFormat,
						int							stride,
						int							surfaceFlags);
	virtual SCODE	WrapSurface
						(DDGPESurf*					*ppSurf,
						int							width,
						int							height,
						EGPEFormat					format,
						EDDGPEPixelFormat			pixelFormat,
						unsigned char *				pBits,
						int							stride );
	virtual SCODE	WrapSurface
						(LPDDRAWI_DDRAWSURFACE_GBL	lpDDSurface,
						int							width,
						int							height,
						EGPEFormat					format,
						EDDGPEPixelFormat			pixelFormat,
						unsigned char *				pBits,
						int							stride );
	virtual SCODE	WrapSurface
						(DDGPESurf*					*ppSurf,
						DDGPEAllocSurfaceData*		pddgpeAllocSurfaceData );
	virtual SCODE	WrapSurface
						(LPDDRAWI_DDRAWSURFACE_GBL	lpDDSurface,
						DDGPEAllocSurfaceData*		pddgpeAllocSurfaceData );

	// Blit operations
	virtual SCODE	BltPrepare
						(GPEBltParms*		pBltParms)=0;	// this function must remain in this position
	virtual SCODE	BltPrepare
						(DDGPEBltParms*		pddgpeBltParms);
	virtual SCODE	PerformBlt
						(DDGPEBltParms*		pddgpeBltParms);
	virtual SCODE	BltExpanded
						(DDGPESurf*			pDst,
						DDGPESurf*			pSrc,
						DDGPESurf*			pPattern,
						DDGPESurf*			pMask,
						CLIPOBJ*			pco,
						XLATEOBJ*			pxlo,
						CONST RECT *		prclDst,
						CONST RECT *		prclSrc,
						ULONG				solidColor,
						ULONG				bltFlags,
						ROP4				rop4
						);
	virtual SCODE	BltExpanded
						(DDGPESurf*			pDst,
						DDGPESurf*			pSrc,
						DDGPESurf*			pPattern,
						CONST RECT *		prclDst,
						CONST RECT *		prclSrc,
						ULONG				solidColor,
						ULONG				bltFlags,
						ULONG				rop4);


	// Surface flipping
	virtual void	SetVisibleSurface
						(GPESurf *			pSurf,
						BOOL				bWaitForVBlank = FALSE);
	virtual void	SetVisibleSurface
						(GPESurf *			pSurf,
						DWORD				dwData,
						BOOL				bWaitForVBlank = FALSE);

	virtual SCODE	DetectMode
						(DWORD*				dwModeID,
						DWORD				dwWidth,
						DWORD				dwHeight,
						EGPEFormat			format,
						EDDGPEPixelFormat	pixelFormat,
						DDPIXELFORMAT*		pDDPF = NULL			// in: Explicit pixel format or current mode
						);
	virtual SCODE	DetectPixelFormat	
						(DWORD				dwCaps,					// in:  DDSCAPS_xxx flags
						DDPIXELFORMAT*		pDDPF,					// in:  Explicit pixel format or current mode
						EGPEFormat*			pFormat,				// out: pixel format
						EDDGPEPixelFormat*	pPixelFormat			// out: ddgpe pixel format
						);

	virtual DWORD	GetPhysicalModeId	(void) { return m_dwPhysicalModeID; }	// Get current hardware mode (mode of ddraw primary)

	virtual int		InDisplay(void);

// override these to mark as supported
public:
	virtual SCODE	GetModeInfoEx
						(GPEModeEx *		pModeEx,
						int					modeNo );
	// the driver should override this function to provide DDGPE with the ability
	// to access the mode of the hardware without changing the GDI primary surface mode
	virtual	SCODE	SetMode
						(int				modeNo,
						HPALETTE *			pPalette,
						BOOL				bChangeGDISurfaceMode);

	virtual DDGPEDriverData*		GetDriverData(void) { return m_pDriverData; }
	virtual void					SetDriverData(DDGPEDriverData* pDriverData) { m_pDriverData = pDriverData; }

protected:

	DWORD	m_dwPhysicalModeID; // driver must set this whenever it changes the HW mode
								// (may differ from GDI mode)

public:
	// a place for drivers to store their data
	DDGPEDriverData*			m_pDriverData;
	GUID						m_guidDriverData;
};



class DDGPESurf : public GPESurf
{
public:
					DDGPESurf();
					DDGPESurf(               // Create video-memory surface
						int					width,
						int					height,
						void *				pBits,            // virtual address of allocated bits
						int					stride,
						EGPEFormat			format);
					DDGPESurf(               // Create video-memory surface
						int					width,
						int					height,
						void *				pBits,            // virtual address of allocated bits
						int					stride,
						EGPEFormat			format,
						EDDGPEPixelFormat	pixelFormat);
					DDGPESurf(
						int					width,
						int					height,
						int					stride,
						EGPEFormat			format,
						EDDGPEPixelFormat	pixelFormat);
					DDGPESurf(
						int					width,		// Create system-memory surface and allocate storage
						int					height,
						EGPEFormat			format);
	virtual         ~DDGPESurf();

public:

	// Initialization
	virtual void	Init
						(int				width,
						int					height,
						void *				pBits,
						int					stride,
						EGPEFormat			format,
						EDDGPEPixelFormat	pixelFormat);

	//virtual LPDDRAWI_DDRAWSURFACE_LCL	GetDirectDrawSurface(void);
	//virtual void		SetDirectDrawSurface(LPDDRAWI_DDRAWSURFACE_LCL lpDDSurface);

	virtual void					SetDDGPESurf(LPDDRAWI_DDRAWSURFACE_GBL lpDDSurface);

	virtual DWORD					Bpp(void);
	// get the actual width of the surface's allocated memory
	virtual DWORD					AlignedWidth(void);

	virtual void					SetColorKey(DWORD dwColorKey);
	virtual DWORD					ColorKey(void);
	virtual void					SetColorKeyMask(DWORD dwColorKeyMask);
	virtual DWORD					ColorKeyMask(void);

	virtual void					SetColorKeyLow(DWORD dwColorKey);
	virtual DWORD					ColorKeyLow(void);
	virtual void					SetColorKeyHigh(DWORD dwColorKey);
	virtual DWORD					ColorKeyHigh(void);

	virtual EDDGPEPixelFormat		PixelFormat(void);

	virtual BOOL					HasAlpha(void);

	virtual DDGPEDriverData*		GetDriverData(void) { return m_pDriverData; }
	virtual void					SetDriverData(DDGPEDriverData* pDriverData) { m_pDriverData = pDriverData; }

public:

	static void						DeleteSurface(LPDDRAWI_DDRAWSURFACE_LCL lpDDSurface);
	static DDGPESurf*				GetDDGPESurf(LPDDRAWI_DDRAWSURFACE_LCL lpDDSurface);
	//static void						Swap(LPDDRAWI_DDRAWSURFACE_LCL s1, LPDDRAWI_DDRAWSURFACE_LCL s2);

protected:
		DWORD						m_dwColorKeyLowValue;		// color key for this surface
		DWORD						m_dwColorKeyHighValue;		// color key for this surface
		DWORD						m_dwColorKeyMask;			// color key mask for this surface
		EDDGPEPixelFormat			m_ePixelFormat;

public:

		LPDDRAWI_DDRAWSURFACE_GBL	lpDDSurface;
	
		// a place for drivers to store their data
		DDGPEDriverData*			m_pDriverData;
		GUID						m_guidDriverData;
};


// Surface Heap (use instead of Node2D if you're dealing with multiple surface types
class SurfaceHeap
{
	SurfaceHeap		*m_pNext;
	SurfaceHeap		*m_pPrev;
	ADDRESS			m_pStart;
	DWORD			m_nSize;
	DWORD			m_fUsed;
public:
					SurfaceHeap
					(
						DWORD size,
						ADDRESS base,
						SurfaceHeap *pNext = (SurfaceHeap *)NULL,
						SurfaceHeap *pPrev = (SurfaceHeap *)NULL
					);
					~SurfaceHeap();
	SurfaceHeap *	Alloc( DWORD size );
	ADDRESS			Address() { return m_pStart; } // offset
	void			Free();
	DWORD			Available();
	DWORD			Size();	// returns size of this & subsequent nodes
	DWORD			NodeSize(){ return m_nSize; }
};


#endif __DDGPE_H__

