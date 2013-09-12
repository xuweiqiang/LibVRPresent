//=============================================================================
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO INTERVIDEO, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2004 - 2005  InterVideo Corporation.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#include <windows.h>

#ifndef _USE_ARM_YUV2RGB
#if defined (_WIN32_WCE) || defined (__linux__)
#include <ippIP.h>
#else
#include "ipp_px.h"
//#include "ippi.h"
#include "ippcc.h"
#endif // _WIN32_WCE
#endif //_USE_ARM_YUV2RGB

#ifdef __SYMBIAN32__  // RGB444
#undef  RGB16_PACK
#define RGB16_PACK(r,g,b)   (((r>>4)&0x0F00) | (g>>8)&0x00F0 | (b>>12))
#endif

#define RGB16_PACK(r,g,b)	((r&0xF800) | ((g>>5)&0x07E0) | (b>>11))						

// this should be the fastest clip algorithm
#define CLIP_65535(a)	if((unsigned)a>=0x00010000) {if(a<0) a=0; else a=0xffff;}
// this is an alternate clip algorithm
//#define CLIP_65535(a)	if(a<0) a=0; else if(a>65535) a = 65535;
// this is for testing
//#define CLIP_65535(a)

#if 0
// this algorithm is faster if most of the pixels require clipping
#define RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b)\
	{\
	r  = luma+temp_r;\
	CLIP_65535(r);\
	b =  luma+temp_b;\
	CLIP_65535(b);\
	g  = luma+temp_g;\
	CLIP_65535(g);\
	}
#else
// this algorithm is faster if few of the pixels require clipping
#define RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b)\
	{\
	r  = luma+temp_r;\
	b =  luma+temp_b;\
	g  = luma+temp_g;\
	if((unsigned)(r|g|b)>=0x00010000)\
		{\
		CLIP_65535(g);\
		CLIP_65535(b);\
		CLIP_65535(r);\
		}\
	}
#endif

#define GCG (20279)
#define GCB (-73247)
#define GCR (-62743)
#define GC00 (299) //256+32+8+2+1
#define GC01 (-55) //32+16+4+2+1
#define GC02 (-137) //128+8+1
#define GC11 (543) //512+32-1
#define GC22 (461) //256+128+64+8+4+1
#define USE_CONSTANT
#ifdef USE_CONSTANT
#define gcg GCG
#define gcb GCB
#define gcr GCR
#define gc00 GC00
#define gc01 GC01
#define gc02 GC02
#define gc11 GC11
#define gc22 GC22
#define GAMMAFUNC(x) (x*gc00)
#else
#define GAMMAFUNC(x) (x*((gcgamma*x>>8)+gc00))
#endif

//inline void ComposePixelR(int x1, int x2, int z1, int z2, unsigned int &R0, unsigned int &R1)
#define ComposePixelR(x1, x2, z1, z2, R0, R1)\
{\
	temp_g = z1*gc01 + z2*gc02 + gcg;\
	temp_b = z1*gc11 + gcb;\
	temp_r = z2*gc22 + gcr;\
	luma = (x1&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = RGB16_PACK(r,g,b);\
	luma = (x2&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = RGB16_PACK(r,g,b)<<16 | R0;\
	luma = (x1>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = RGB16_PACK(r,g,b);\
	luma = (x2>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = RGB16_PACK(r,g,b)<<16 | R1;\
}

//inline void ComposePixel(int x1, int x2, int z1, int z2, unsigned int &R0, unsigned int &R1)
#define ComposePixel(x1, x2, z1, z2, R0, R1)\
{\
	temp_g = z1*gc01 + z2*gc02 + gcg;\
	temp_b = z1*gc11 + gcb;\
	temp_r = z2*gc22 + gcr;\
	luma = (x1&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = RGB16_PACK(r,g,b);\
	luma = (x1>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = RGB16_PACK(r,g,b)<<16 | R0;\
	luma = (x2&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = RGB16_PACK(r,g,b);\
	luma = (x2>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = RGB16_PACK(r,g,b)<<16 | R1;\
}

//inline void ComposePixelU(int x1, int x2, int z1, int z2, unsigned int &R0, unsigned int &R1)
#define ComposePixelU(x1, x2, z1, z2, R0, R1)\
{\
	temp_g = z1*gc01 + z2*gc02 + gcg;\
	temp_b = z1*gc11 + gcb;\
	temp_r = z2*gc22 + gcr;\
	luma = (x1&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = RGB16_PACK(r,g,b);\
	luma = (x1>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = R0<<16 | RGB16_PACK(r,g,b);\
	luma = (x2&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = RGB16_PACK(r,g,b);\
	luma = (x2>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = R1<<16 | RGB16_PACK(r,g,b);\
}

//inline void ComposePixelRU(int x1, int x2, int z1, int z2, unsigned int &R0, unsigned int &R1)
#define ComposePixelRU(x1, x2, z1, z2, R0, R1)\
{\
	temp_g = z1*gc01 + z2*gc02 + gcg;\
	temp_b = z1*gc11 + gcb;\
	temp_r = z2*gc22 + gcr;\
	luma = (x1&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = RGB16_PACK(r,g,b);\
	luma = (x2&0xff);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R0 = R0<<16 | RGB16_PACK(r,g,b);\
	luma = (x1>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = RGB16_PACK(r,g,b);\
	luma = (x2>>8);\
	luma = GAMMAFUNC(luma);\
	RGB_CLIP(r,g,b,luma,temp_r,temp_g,temp_b);\
	R1 = R1<<16 | RGB16_PACK(r,g,b);\
}

// add by henry gu, 2005.7.29
HRESULT epp_yuv2rgb565_rot_8u16u(unsigned char *in_y, unsigned char *in_u, unsigned char *in_v,
							 int stride_y, int stride_uv, unsigned short *pOut, int src_width, int src_height, int out_width, int out_height,
							 int scanline_increment, int scanpixel_increment, LONG *cc)
{
#ifndef _USE_ARM_YUV2RGB
#ifdef _WIN32_WCE
	IppCameraCsc colorConversion;
	IppCameraRotation rotation;
	IppCameraInterpolation interpolation;
	const Ipp8u* pSrc[3];
	int srcStep[3];
	void* pDst;
	int dstStep;
	IppiSize srcSize, dstSize;
	IppStatus status;
	int rcpRatioX, rcpRatioY;

	pDst = (void *)((unsigned long)pOut & (~0x7));

	srcSize.width = src_width;//out_width;
	srcSize.height = src_height;//out_height;
	dstSize.width = out_width;
	dstSize.height = out_height;

	dstStep = abs(scanline_increment);
	colorConversion = ippCameraCscYCbCr420ToRGB565;
	interpolation = ippCameraInterpNearest;
	if (scanline_increment>0 && scanpixel_increment>0)
	{
		// no rotate
		rotation = ippCameraRotateDisable;
	}
	else if (scanline_increment>0 && scanpixel_increment<0)
	{
		// 90 right
		rotation = ippCameraRotate90L;
	}
	else if (scanline_increment<0 && scanpixel_increment<0)
	{
		// 180
		rotation = ippCameraRotate180;
	}
	else if (scanline_increment<0 && scanpixel_increment>0)
	{
		// 90 left
		rotation = ippCameraRotate90R;
	}

	pSrc[0] = in_y;
	pSrc[1] = in_u;
	pSrc[2] = in_v;

	srcStep[0] = stride_y;
	srcStep[1] = stride_uv;
	srcStep[2] = stride_uv;

	rcpRatioX =	(int)(((double)((srcSize.width-1)<<16))/(dstSize.width-1));
	rcpRatioY =	(int)(((double)((srcSize.height-1)<<16))/(dstSize.height-1));

	status = ippiYCbCr420RszCscRotRGB_8u_P3C3R(pSrc, srcStep, srcSize, pDst,
		dstStep, dstSize, colorConversion, interpolation, rotation,
		rcpRatioX, rcpRatioY);

#else
	Ipp8u* pSrc[3];
	int srcStep[3];
	Ipp16u* pDst;
	int dstStep;
	IppiSize roiSize;
	IppStatus status;

	pSrc[0] = in_y;
	pSrc[1] = in_u;
	pSrc[2] = in_v;

	srcStep[0] = stride_y;
	srcStep[1] = stride_uv;
	srcStep[2] = stride_uv;

	pDst = pOut;
	dstStep = scanline_increment;

	roiSize.width = src_width;
	roiSize.height = src_height;
	//	YUV420ToBGR565_8u16u_P3C3R();
	status = ippiYUV420ToBGR565_8u16u_P3C3R(pSrc,  srcStep, pDst, dstStep, roiSize);
	//ippStsNoErr
#endif

#endif
	return S_OK;
}
// end by henry gu

// add by henry gu, 2005.7.29
HRESULT epp_yuv2rgb565_8u16u(unsigned char *in_y, unsigned char *in_u, unsigned char *in_v,
							 int stride_y, int stride_uv, unsigned short *pOut, int out_width, int out_height,
							 int scanline_increment, int scanpixel_increment, LONG *cc)
{
#ifndef _USE_ARM_YUV2RGB
	Ipp8u* pSrc[3];
	int srcStep[3];
	Ipp16u* pDst;
	int dstStep;
	IppiSize roiSize;
	IppStatus status;

	pSrc[0] = in_y;
	pSrc[1] = in_u;
	pSrc[2] = in_v;

	srcStep[0] = stride_y;
	srcStep[1] = stride_uv;
	srcStep[2] = stride_uv;

	pDst = pOut;
	dstStep = scanline_increment;

	roiSize.width = out_width;
	roiSize.height = out_height;

	//	YUV420ToBGR565_8u16u_P3C3R();
	status = ippiYUV420ToBGR565_8u16u_P3C3R(pSrc,  srcStep,
		pDst, dstStep, roiSize);
	//ippStsNoErr
#endif
	return S_OK;
}
// end by henry gu

HRESULT epp_yuv2rgb565_8u16u_right(unsigned char *in_y, unsigned char *in_u, unsigned char *in_v,
				int stride_y, int stride_uv, unsigned short *pOut, int out_width, int out_height,
				int scanline_increment, int scanpixel_increment, LONG *cc)
{
	int i, j;
	int r,g,b,luma,temp_g,temp_b,temp_r;
	unsigned int	*pRGBtop, *pRGBbottom;
	unsigned short	*pYtop, *pYbottom;
	unsigned int	*pRGBt,*pRGBb;
	unsigned char	*pU, *pV;
#ifndef USE_CONSTANT
	int gcg, gcb, gcr, gc00, gc01, gc02, gc11, gc22, gcgamma;

	if(cc)
	{
		gc00 = (GC00 * (cc[1]+256))>>8;
		gc01 = (GC01 * (cc[3]+256))>>8;
		gc02 = (GC02 * (cc[3]+256))>>8;
		gc11 = (GC11 * (cc[3]+256))>>8;
		gc22 = (GC22 * (cc[3]+256))>>8;
		gcg = GCG + (cc[0]<<7) + ((GC01+GC02-gc01-gc02)<<7);
		gcb = GCB + (cc[0]<<7) + ((GC11-gc11)<<7);
		gcr = GCR + (cc[0]<<7) + ((GC22-gc22)<<7);
		i = -cc[4]*gc00;
		gcgamma = (i+(i>>31&1))>>8;  // actually more like /255
		gc00 = (256+cc[4])*gc00>>8;
	}
	else
	{
		gc00 = GC00;
		gc01 = GC01;
		gc02 = GC02;
		gc11 = GC11;
		gc22 = GC22;
		gcg = GCG;
		gcb = GCB;
		gcr = GCR;
	}
#endif

	pYtop		= (unsigned short *)in_y;
	pYbottom	= (unsigned short *)&in_y[stride_y];
	pU			= (unsigned char *)in_u;
	pV			= (unsigned char *)in_v;
	pRGBtop		= (unsigned int *)pOut;
	if(abs(scanline_increment)<=4)
	{	// ROTATED
		pRGBbottom	= (unsigned int *)&pOut[scanpixel_increment];
		for(i=0;i<out_height;i+=2)
		{
			pRGBt = pRGBtop;
			pRGBb = pRGBbottom;
			if(scanpixel_increment<0)
			{
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixelR(pYtop[j],pYbottom[j],pU[j],pV[j],pRGBt[0],pRGBb[0]);
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
				}
			}
			else
			{
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixelRU(pYtop[j],pYbottom[j],pU[j],pV[j],pRGBt[0],pRGBb[0]);
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
				}
			}
			pYtop		+= stride_y;			// jump two lines
			pYbottom	+= stride_y;			// jump two lines
			pU			+= stride_uv;			// jump one line
			pV			+= stride_uv;			// jump one line
			pRGBtop		+= scanline_increment>>1;	// jump two pels (actually this is pRGBtop+=1)
			pRGBbottom	+= scanline_increment>>1;	// jump two pels
		}
	}
	else
	{	// NORMAL
		pRGBbottom	= (unsigned int *)&pOut[scanline_increment>>1];
		for(i=0;i<out_height;i+=2)
		{
			if(scanpixel_increment==1)
			{
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixel(pYtop[j],pYbottom[j],pU[j],pV[j],pRGBtop[j],pRGBbottom[j]);
				}
			}
			else if(scanpixel_increment>0)
			{
				pRGBt = pRGBtop;
				pRGBb = pRGBbottom;
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixel(pYtop[j],pYbottom[j],pU[j],pV[j],pRGBt[0],pRGBb[0]);
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
				}
			}
			else
			{
				pRGBt = pRGBtop;
				pRGBb = pRGBbottom;
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixelU(pYtop[j],pYbottom[j],pU[j],pV[j],pRGBt[0],pRGBb[0]);
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
				}
			}
			pYtop		+= stride_y;			// jump two lines
			pYbottom	+= stride_y;			// jump two lines
			pU			+= stride_uv;			// jump one line
			pV			+= stride_uv;			// jump one line
			pRGBtop		+= scanline_increment>>1;	// jump two lines
			pRGBbottom	+= scanline_increment>>1;	// jump two lines
		}
	}
	return S_OK;
}

HRESULT epp_yuv2rgb565_uv_8u16u(unsigned char *in_y, unsigned char *in_uv,
				int stride_y, int stride_uv, unsigned short *pOut, int out_width, int out_height,
				int scanline_increment, int scanpixel_increment, LONG *cc)
{
	int i, j;
	int r,g,b,luma,temp_g,temp_b,temp_r;
	unsigned int	*pRGBtop, *pRGBbottom;
	unsigned int	*pRGBt,*pRGBb;
	unsigned short	*pYtop, *pYbottom;
	unsigned char	*pUV,*pUVtemp;
#ifndef USE_CONSTANT
	int gcg, gcb, gcr, gc00, gc01, gc02, gc11, gc22, gcgamma;

	if(cc)
	{
		gc00 = (GC00 * (cc[1]+256))>>8;
		gc01 = (GC01 * (cc[3]+256))>>8;
		gc02 = (GC02 * (cc[3]+256))>>8;
		gc11 = (GC11 * (cc[3]+256))>>8;
		gc22 = (GC22 * (cc[3]+256))>>8;
		gcg = GCG + (cc[0]<<7) + ((GC01+GC02-gc01-gc02)<<7);
		gcb = GCB + (cc[0]<<7) + ((GC11-gc11)<<7);
		gcr = GCR + (cc[0]<<7) + ((GC22-gc22)<<7);
		i = -cc[4]*gc00;
		gcgamma = (i+(i>>31&1))>>8;
		gc00 = (256+cc[4])*gc00>>8;
	}
	else
	{
		gcg = GCG;
		gcb = GCB;
		gcr = GCR;
		gc00 = GC00;
		gc01 = GC01;
		gc02 = GC02;
		gc11 = GC11;
		gc22 = GC22;
	}
#endif
	pYtop		= (unsigned short *)in_y;
	pYbottom	= (unsigned short *)&in_y[stride_y];
	pUV			= (unsigned char *)in_uv;
	pRGBtop		= (unsigned int *)pOut;
	if(abs(scanline_increment)<=4)
	{	// ROTATED
		pRGBbottom	= (unsigned int *)&pOut[scanpixel_increment];
		for(i=0;i<out_height;i+=2)
		{
			pRGBt = pRGBtop;
			pRGBb = pRGBbottom;
			pUVtemp = pUV;
			if(scanpixel_increment<0)
			{
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixelR(pYtop[j],pYbottom[j],pUVtemp[0],pUVtemp[1],pRGBt[0],pRGBb[0]);
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
					pUVtemp += 2;
				}
			}
			else
			{
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixelRU(pYtop[j],pYbottom[j],pUVtemp[0],pUVtemp[1],pRGBt[0],pRGBb[0]);
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
					pUVtemp += 2;
				}
			}
			pYtop		+= stride_y;			// jump two lines
			pYbottom	+= stride_y;			// jump two lines
			pUV			+= stride_uv;			// jump one line
			pRGBtop		+= scanline_increment>>1;	// jump two pels (actually this is pRGBtop+=1)
			pRGBbottom	+= scanline_increment>>1;	// jump two pels
		}
	}
	else
	{	// NORMAL
		pRGBbottom	= (unsigned int *)&pOut[scanline_increment>>1];
		for(i=0;i<out_height;i+=2)
		{
			pUVtemp = pUV;
			if(scanpixel_increment==1)
			{
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixel(pYtop[j],pYbottom[j],pUVtemp[0],pUVtemp[1],pRGBtop[j],pRGBbottom[j]);
					pUVtemp += 2;
				}
			}
			else if(scanpixel_increment>0)
			{
				pRGBt = pRGBtop;
				pRGBb = pRGBbottom;
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixel(pYtop[j],pYbottom[j],pUVtemp[0],pUVtemp[1],pRGBt[0],pRGBb[0]);
					pUVtemp += 2;
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
				}
			}
			else
			{
				pRGBt = pRGBtop;
				pRGBb = pRGBbottom;
				for(j=0; j<out_width>>1; j++)	// do one RGB quad at a time
				{
					ComposePixelU(pYtop[j],pYbottom[j],pUVtemp[0],pUVtemp[1],pRGBt[0],pRGBb[0]);
					pUVtemp += 2;
					pRGBt += scanpixel_increment;
					pRGBb += scanpixel_increment;
				}
			}
			pYtop		+= stride_y;			// jump two lines
			pYbottom	+= stride_y;			// jump two lines
			pUV			+= stride_uv;			// jump one line
			pRGBtop		+= scanline_increment>>1;	// jump two lines
			pRGBbottom	+= scanline_increment>>1;	// jump two lines
		}
	}
	return S_OK;
}






















