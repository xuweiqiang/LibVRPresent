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

#ifndef __IVI_IMAGELIB_H__
#define __IVI_IMAGELIB_H__

typedef enum
{
	CIRotationDisable	= 0,	// no rotation
	CIRotation90L		= 1,	// rotating by 90 degree anticlockwise
	CIRotation90R		= 2,	// rotating by 90 degree clockwise
	CIRotation180		= 3,	// rotating by 180
	CIRotationFlipHorz  = 4,
	CIRotationFlipVert  = 5,
}CIRotType;

typedef enum
{
	CIInterpNearest     = 0,
	CIInterpBilinear    = 1,
	CIInterpMedian      = 2,
	CIInterpNearLinear  = 3,
}CIInterpolation;

typedef enum
{
	CIYCbCr420 = 0,
	CIYCbCr422 = 1,
	CIYCbCr444 = 2,
	CIYCbCrNV12 = 3,
	CIRGB565 = 4,
	CIRGB888 = 5,
	CIARGB32 = 6,
	CIYUY2   = 7,
	CIUYVY   = 8,
	CINV12   = 9,
}CIImageFormat;

	//CICscYCbCr422ToRGB565 = 0,
	//CICscYCbCr422ToRGB555 = 1,
	//CICscYCbCr422ToRGB444 = 2,
	//CICscYCbCr422ToRGB888 = 3,
	//CICscYCbCr420ToRGB565 = 4,
	//CICscYCbCr420ToRGB555 = 5,
	//CICscYCbCr420ToRGB444 = 6,
	//CICscYCbCr420ToRGB888 = 7,
	//CICscYCbCr420ToAlphaRGB = 8,
#define CICscYCbCr420ToRGB565	 ((CIYCbCr420<<8)|CIRGB565)
#define CICscYCbCr420ToAlphaRGB	 ((CIYCbCr420<<8)|CIARGB32)
#define CICscYCbCr420ToYCbCr420	 ((CIYCbCr420<<8)|CIYCbCr420)
#define CICscRGB565ToAlphaRGB	 ((CIRGB565<<8)|CIARGB32)
#define CICscYCbCr444ToRGB565    ((CIYCbCr444<<8)|CIRGB565)
#define CICscYCbCr444ToRGB888    ((CIYCbCr444<<8)|CIRGB888)
#define CICscYCbCr444ToAlphaRGB  ((CIYCbCr444<<8)|CIARGB32)
#define CICscYCbCr420ToYUY2      ((CIYCbCr420<<8)|CIYUY2)
#define CICscYCbCr420ToUYVY      ((CIYCbCr420<<8)|CIUYVY)
#define CICscRGB565ToRGB565		 ((CIRGB565<<8)|CIRGB565)
#define CICscRGB888ToRGB565      ((CIRGB888<<8)|CIRGB565)
#define CICscYCbCr420ToRGB888    ((CIYCbCr420<<8)|CIRGB888)
#define CICscRGB888ToRGB888      ((CIRGB888<<8)|CIRGB888)
#define CICscYCbCr422ToRGB565    ((CIYCbCr422<<8)|CIRGB565)
#define CICscYCbCr420ToNV12		 ((CIYCbCr420<<8)|CINV12)

typedef struct {
	int width;
	int height;
} CISize;

typedef struct _tagImageOption
{
	int    iBrightness;
	int	   iContrast;
	int    iSaturation;
}CIImageOption;

typedef void* CIImagePrcHandle;

#ifdef __cplusplus
extern "C" {
#endif

/* /////////////////////////////////////////////////////////////////////////////
//                  Image Process interface Functions
///////////////////////////////////////////////////////////////////////////// */
int CI_ImgPrcInit(CIImagePrcHandle* hImgPrc, int convertion, CIRotType rotation, CIInterpolation interpolation, CISize *pSrcSize, CISize *pDstSize, unsigned char alpha);
int CI_ImgPrcOneFrame(CIImagePrcHandle hImgPrc, unsigned char *pSrc[3], int srcStride[3], unsigned char *pDst[3], int dstStride[3], CIInterpolation interpolation);
int CI_ImgPrcUninit(CIImagePrcHandle hImgPrc);

/* /////////////////////////////////////////////////////////////////////////////
//                  Color Conversion Functions
///////////////////////////////////////////////////////////////////////////// */

int CI_YCbCrToRGBCSC_Set(CIImageOption * Opt);

int CI_YCbCr_Set(CIImagePrcHandle hImgPrc, CIImageOption *Opt);


#ifdef __cplusplus
}
#endif

#endif //__IVI_IMAGELIB_H__
