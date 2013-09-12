
#include <string.h>

#include "GfxPresentAlphaBlending.h"

#ifndef _USE_ALPHABLENDING_WMMX
#define ALPHA_SHIFTBIT		30
#else
#define ALPHA_SHIFTBIT		16
#endif
#define MULLONG_RSHIFT30(x, y)      (unsigned long)(((unsigned long long)(x)*(unsigned long long)(y))>>30)

#ifdef _USE_ALPHABLENDING_WMMX
extern "C" void DoAlphaBlending1ByteSub_WMMX(unsigned char *, unsigned char *, unsigned long *);
#endif

CAlphaBlending::CAlphaBlending()
{
	memset(&m_tAlphaBlendingSetting, 0, sizeof(TAlphaBlendingSetting));
#ifndef _USE_ALPHABLENDING_WMMX
	m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoAlphaBlending1Byte_C;
#else
	m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoAlphaBlending1Byte_WMMX;
#endif
}

int CAlphaBlending::CheckAlphaBlendingSettings
(
	unsigned long u32Image1Width, unsigned long u32Image1Height,
	unsigned long u32Image2Width, unsigned long u32Image2Height,
	long s32PosLeftTopX, long s32PosLeftTopY,
	unsigned char u8BackgroundY, unsigned char u8BackgroundU, unsigned char u8BackgroundV,
	double f64Alpha
)
{
	int  retVal = 0;

	if (s32PosLeftTopX != m_tAlphaBlendingSetting.s32PosLeftTopX ||
		s32PosLeftTopY != m_tAlphaBlendingSetting.s32PosLeftTopY)
	{
		retVal |= ALPHABLENDING_POSITION_CHANGED;
	}
	if (f64Alpha != m_tAlphaBlendingSetting.f64Alpha)
	{
		retVal |= ALPHABLENDING_ALPHA_CHANGED;
	}
	if (u32Image1Width != m_tAlphaBlendingSetting.u32Image1Width ||
		u32Image1Height != m_tAlphaBlendingSetting.u32Image1Height)
	{
		retVal |= ALPHABLENDING_IMAGE1SIZE_CHANGED;
	}
	if (u32Image2Width != m_tAlphaBlendingSetting.u32Image2Width ||
		u32Image2Height != m_tAlphaBlendingSetting.u32Image2Height)
	{
		retVal |= ALPHABLENDING_IMAGE2SIZE_CHANGED;
	}
	if (u8BackgroundY != m_tAlphaBlendingSetting.u8BackgroundY ||
		u8BackgroundU != m_tAlphaBlendingSetting.u8BackgroundU ||
		u8BackgroundV != m_tAlphaBlendingSetting.u8BackgroundV)
	{
		retVal |= ALPHABLENDING_BACKGROUNDCOLOR_CHANGED;
	}

	return retVal;
}

int CAlphaBlending::SetAlphaBlendingPos(long s32PosLeftTopX, long s32PosLeftTopY)
{
	unsigned long u32Image1Width = m_tAlphaBlendingSetting.u32Image1Width;
	unsigned long u32Image1Height = m_tAlphaBlendingSetting.u32Image1Height;
	unsigned long u32Image2Width = m_tAlphaBlendingSetting.u32Image2Width;
	unsigned long u32Image2Height = m_tAlphaBlendingSetting.u32Image2Height;

	m_tAlphaBlendingSetting.s32PosLeftTopX  = s32PosLeftTopX;
	m_tAlphaBlendingSetting.s32PosLeftTopY  = s32PosLeftTopY;

	if (s32PosLeftTopX >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth = ((long)u32Image1Width-s32PosLeftTopX)<(long)u32Image2Width ?
			(long)u32Image1Width-s32PosLeftTopX : (long)u32Image2Width;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth = ((long)u32Image2Width+s32PosLeftTopX);
	}

	if (s32PosLeftTopY >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight = ((long)u32Image1Height-s32PosLeftTopY)<(long)u32Image2Height ?
			((long)u32Image1Height-s32PosLeftTopY) : (long)u32Image2Height;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight = ((long)u32Image2Height+s32PosLeftTopY);
	}
	return 0;
}

int CAlphaBlending::SetAlphaBlendingAlpha(double f64Alpha)
{
	int  retVal = 0;

	m_tAlphaBlendingSetting.f64Alpha = f64Alpha>0? f64Alpha : -f64Alpha;

	if (m_tAlphaBlendingSetting.f64Alpha > 1.00f)
	{
		m_tAlphaBlendingSetting.f64Alpha = 1.00f;
		retVal = -1;
	}
	m_tAlphaBlendingSetting.u32Alpha1 = (unsigned long)(m_tAlphaBlendingSetting.f64Alpha*(1<<ALPHA_SHIFTBIT));
	m_tAlphaBlendingSetting.u32Alpha2 = (unsigned long)((1-m_tAlphaBlendingSetting.f64Alpha)*(1<<ALPHA_SHIFTBIT));
#ifdef _USE_ALPHABLENDING_WMMX
	if (m_tAlphaBlendingSetting.u32Alpha1 > 0xffff)
	{
		m_tAlphaBlendingSetting.u32Alpha1 = 0xffff;
	}
	if (m_tAlphaBlendingSetting.u32Alpha2 > 0xffff)
	{
		m_tAlphaBlendingSetting.u32Alpha2 = 0xffff;
	}
#endif
	if (m_tAlphaBlendingSetting.u32Alpha1 == 0)
	{
		// means we only need to copy the pixels of the image2 to the image1 except pixels in background color
		m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoCoverFirstImage1Byte_C;
	}
	else if (m_tAlphaBlendingSetting.u32Alpha2 == 0)
	{
		// means we do not need to do alpha blending
		m_pfnDoAlphaBlending1Byte = NULL;
	}
	else
	{
#ifndef _USE_ALPHABLENDING_WMMX
		m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoAlphaBlending1Byte_C;
#else
		m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoAlphaBlending1Byte_WMMX;
#endif
	}

	return retVal;
}

int CAlphaBlending::SetAlphaBlendingImage1Size(unsigned long u32Image1Width, unsigned long u32Image1Height)
{
	int  retVal = 0;

	m_tAlphaBlendingSetting.u32Image1Width  = u32Image1Width&~1;
	m_tAlphaBlendingSetting.u32Image1Height = u32Image1Height&~1;

	if (m_tAlphaBlendingSetting.s32PosLeftTopX >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth =
			((long)m_tAlphaBlendingSetting.u32Image1Width-m_tAlphaBlendingSetting.s32PosLeftTopX)<(long)m_tAlphaBlendingSetting.u32Image2Width ?
			(long)m_tAlphaBlendingSetting.u32Image1Width-m_tAlphaBlendingSetting.s32PosLeftTopX : (long)m_tAlphaBlendingSetting.u32Image2Width;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth =
			((long)m_tAlphaBlendingSetting.u32Image2Width+m_tAlphaBlendingSetting.s32PosLeftTopX);
	}

	if (m_tAlphaBlendingSetting.s32PosLeftTopY >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight =
			((long)m_tAlphaBlendingSetting.u32Image1Height-m_tAlphaBlendingSetting.s32PosLeftTopY)<(long)m_tAlphaBlendingSetting.u32Image2Height ?
			((long)m_tAlphaBlendingSetting.u32Image1Height-m_tAlphaBlendingSetting.s32PosLeftTopY) : (long)m_tAlphaBlendingSetting.u32Image2Height;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight =
			((long)m_tAlphaBlendingSetting.u32Image2Height+m_tAlphaBlendingSetting.s32PosLeftTopY);
	}

	if ((u32Image1Width&1) == 1 || (u32Image1Height&1) == 1)
	{
		retVal = -1;
	}

	return retVal;
}

int CAlphaBlending::SetAlphaBlendingImage2Size(unsigned long u32Image2Width, unsigned long u32Image2Height)
{
	int  retVal = 0;

	m_tAlphaBlendingSetting.u32Image2Width  = u32Image2Width&~1;
	m_tAlphaBlendingSetting.u32Image2Height = u32Image2Height&~1;

	if (m_tAlphaBlendingSetting.s32PosLeftTopX >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth =
			((long)m_tAlphaBlendingSetting.u32Image1Width-m_tAlphaBlendingSetting.s32PosLeftTopX)<(long)m_tAlphaBlendingSetting.u32Image2Width ?
			(long)m_tAlphaBlendingSetting.u32Image1Width-m_tAlphaBlendingSetting.s32PosLeftTopX : (long)m_tAlphaBlendingSetting.u32Image2Width;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth =
			((long)m_tAlphaBlendingSetting.u32Image2Width+m_tAlphaBlendingSetting.s32PosLeftTopX);
	}

	if (m_tAlphaBlendingSetting.s32PosLeftTopY >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight =
			((long)m_tAlphaBlendingSetting.u32Image1Height-m_tAlphaBlendingSetting.s32PosLeftTopY)<(long)m_tAlphaBlendingSetting.u32Image2Height ?
			((long)m_tAlphaBlendingSetting.u32Image1Height-m_tAlphaBlendingSetting.s32PosLeftTopY) : (long)m_tAlphaBlendingSetting.u32Image2Height;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight =
			((long)m_tAlphaBlendingSetting.u32Image2Height+m_tAlphaBlendingSetting.s32PosLeftTopY);
	}

	if ((u32Image2Width&1) == 1 || (u32Image2Height&1) == 1)
	{
		retVal = -1;
	}

	return retVal;
}

int CAlphaBlending::SetAlphaBlendingBackgroundColor(unsigned char u8BackgroundY, unsigned char u8BackgroundU, unsigned char u8BackgroundV)
{
	int  retVal = 0;

	m_tAlphaBlendingSetting.u8BackgroundY   = u8BackgroundY;
	m_tAlphaBlendingSetting.u8BackgroundU   = u8BackgroundU;
	m_tAlphaBlendingSetting.u8BackgroundV   = u8BackgroundV;

	return retVal;
}

int CAlphaBlending::SetAlphaBlendingYUV
(
	unsigned long u32Image1Width, unsigned long u32Image1Height,
	unsigned long u32Image2Width, unsigned long u32Image2Height,
	long s32PosLeftTopX, long s32PosLeftTopY,
	unsigned char u8BackgroundY, unsigned char u8BackgroundU, unsigned char u8BackgroundV,
	double f64Alpha
)
{
	int  retVal = 0;

	m_tAlphaBlendingSetting.u32Image1Width  = u32Image1Width&~1;
	m_tAlphaBlendingSetting.u32Image1Height = u32Image1Height&~1;
	m_tAlphaBlendingSetting.u32Image2Width  = u32Image2Width&~1;
	m_tAlphaBlendingSetting.u32Image2Height = u32Image2Height&~1;
	m_tAlphaBlendingSetting.s32PosLeftTopX  = s32PosLeftTopX;
	m_tAlphaBlendingSetting.s32PosLeftTopY  = s32PosLeftTopY;
	m_tAlphaBlendingSetting.u8BackgroundY   = u8BackgroundY;
	m_tAlphaBlendingSetting.u8BackgroundU   = u8BackgroundU;
	m_tAlphaBlendingSetting.u8BackgroundV   = u8BackgroundV;
	m_tAlphaBlendingSetting.f64Alpha        = f64Alpha>0? 1-f64Alpha : 1+f64Alpha;

	if (m_tAlphaBlendingSetting.s32PosLeftTopX >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth =
			((long)m_tAlphaBlendingSetting.u32Image1Width-m_tAlphaBlendingSetting.s32PosLeftTopX)<(long)m_tAlphaBlendingSetting.u32Image2Width ?
			(long)m_tAlphaBlendingSetting.u32Image1Width-m_tAlphaBlendingSetting.s32PosLeftTopX : (long)m_tAlphaBlendingSetting.u32Image2Width;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleWidth =
			((long)m_tAlphaBlendingSetting.u32Image2Width+m_tAlphaBlendingSetting.s32PosLeftTopX);
	}

	if (m_tAlphaBlendingSetting.s32PosLeftTopY >= 0)
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight =
			((long)m_tAlphaBlendingSetting.u32Image1Height-m_tAlphaBlendingSetting.s32PosLeftTopY)<(long)m_tAlphaBlendingSetting.u32Image2Height ?
			((long)m_tAlphaBlendingSetting.u32Image1Height-m_tAlphaBlendingSetting.s32PosLeftTopY) : (long)m_tAlphaBlendingSetting.u32Image2Height;
	}
	else
	{
		m_tAlphaBlendingSetting.s32Image2VisibleHeight =
			((long)m_tAlphaBlendingSetting.u32Image2Height+m_tAlphaBlendingSetting.s32PosLeftTopY);
	}

	if (m_tAlphaBlendingSetting.f64Alpha > 1.00f)
	{
		m_tAlphaBlendingSetting.f64Alpha = 1.00f;
		retVal = -1;
	}
	m_tAlphaBlendingSetting.u32Alpha1 = (unsigned long)(m_tAlphaBlendingSetting.f64Alpha*(1<<ALPHA_SHIFTBIT));
	m_tAlphaBlendingSetting.u32Alpha2 = (unsigned long)((1-m_tAlphaBlendingSetting.f64Alpha)*(1<<ALPHA_SHIFTBIT));
#ifdef _USE_ALPHABLENDING_WMMX
	if (m_tAlphaBlendingSetting.u32Alpha1 > 0xffff)
	{
		m_tAlphaBlendingSetting.u32Alpha1 = 0xffff;
	}
	if (m_tAlphaBlendingSetting.u32Alpha2 > 0xffff)
	{
		m_tAlphaBlendingSetting.u32Alpha2 = 0xffff;
	}
#endif
	if (m_tAlphaBlendingSetting.u32Alpha1 == 0)
	{
		// means we only need to copy the pixels of the image2 to the image1 except pixels in background color
		m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoCoverFirstImage1Byte_C;
	}
	else if (m_tAlphaBlendingSetting.u32Alpha2 == 0)
	{
		// means we do not need to do alpha blending
		m_pfnDoAlphaBlending1Byte = NULL;
	}
	else
	{
#ifndef _USE_ALPHABLENDING_WMMX
		m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoAlphaBlending1Byte_C;
#else
		m_pfnDoAlphaBlending1Byte = &CAlphaBlending::DoAlphaBlending1Byte_WMMX;
#endif
	}

	if ((u32Image1Width&1) == 1 || (u32Image2Width&1) == 1 ||
		(u32Image1Height&1) == 1 || (u32Image2Height&1) == 1)
	{
		retVal = -1;
	}

	return retVal;
}

int CAlphaBlending::DoAlphaBlendingYUV
(
	unsigned char* pu8Image1Y, unsigned char* pu8Image1U, unsigned char* pu8Image1V,
	unsigned char* pu8Image2Y, unsigned char* pu8Image2U, unsigned char* pu8Image2V
)
{
#if 1//def GFX_ANDROID_SURFACE
	unsigned int width = m_tAlphaBlendingSetting.u32Image2Width ;
	unsigned int height = m_tAlphaBlendingSetting.u32Image2Height ;

	float alpha;
	unsigned char* dstY;
	unsigned char* dstU;
	unsigned char* dstV;

	dstY = &(pu8Image1Y[m_tAlphaBlendingSetting.s32PosLeftTopY*
		m_tAlphaBlendingSetting.u32Image1Width	+ m_tAlphaBlendingSetting.s32PosLeftTopX]);
	dstU = &(pu8Image1U[m_tAlphaBlendingSetting.s32PosLeftTopY/2*
		m_tAlphaBlendingSetting.u32Image1Width/2 + m_tAlphaBlendingSetting.s32PosLeftTopX/2]);
	dstV = &(pu8Image1V[m_tAlphaBlendingSetting.s32PosLeftTopY/2*
		m_tAlphaBlendingSetting.u32Image1Width/2 + m_tAlphaBlendingSetting.s32PosLeftTopX/2]);
  
	for(unsigned int height_idx = 0;height_idx<height;height_idx++)
	{
	for(unsigned int width_idx = 0;width_idx <width;width_idx++)
	{
		if(!((pu8Image2Y[height_idx*width + width_idx] == m_tAlphaBlendingSetting.u8BackgroundY)
		&&(pu8Image2U[height_idx/2*width/2 + width_idx/2] == m_tAlphaBlendingSetting.u8BackgroundU)
		&&(pu8Image2V[height_idx/2*width/2 + width_idx/2] == m_tAlphaBlendingSetting.u8BackgroundV)))
		{
			alpha = (float)m_tAlphaBlendingSetting.f64Alpha; 

			if(alpha <0.001f)
			{
				dstY[height_idx*width + width_idx] = pu8Image2Y[height_idx*width + width_idx];
				dstU[height_idx/2*width/2 + width_idx/2] =  pu8Image2U[height_idx/2*width/2 + width_idx/2];
				dstV[height_idx/2*width/2 + width_idx/2] =  pu8Image2V[height_idx/2*width/2 + width_idx/2];
			}
			else
			{
				dstY[height_idx*width + width_idx] = (unsigned char)(dstY[height_idx*width + width_idx]*alpha + 
					pu8Image2Y[height_idx*width + width_idx]*(1.0f -alpha));
				dstU[height_idx/2*width/2 + width_idx/2] = (unsigned char)(dstU[height_idx/2*width/2 + width_idx/2]*alpha + 
					pu8Image2U[height_idx/2*width/2 + width_idx/2]*(1.0f -alpha));
				dstV[height_idx/2*width/2 + width_idx/2] = (unsigned char)(dstV[height_idx/2*width/2 + width_idx/2]*alpha + 
					pu8Image2V[height_idx/2*width/2 + width_idx/2]*(1.0f -alpha));
			}

		}
	}   
	}

#else

	if (m_tAlphaBlendingSetting.s32Image2VisibleWidth <= 0 ||
		m_tAlphaBlendingSetting.s32Image2VisibleHeight <= 0)
	{
		return -1;
	}

	if (m_pfnDoAlphaBlending1Byte == 0)
	{
		return 0;
	}

	if (pu8Image1Y != 0 && pu8Image2Y != 0)
	{
		(this->*m_pfnDoAlphaBlending1Byte)(pu8Image1Y, pu8Image2Y, m_tAlphaBlendingSetting.u8BackgroundY);
	}
	m_tAlphaBlendingSetting.u32Image1Width >>= 1;
	m_tAlphaBlendingSetting.u32Image1Height >>= 1;
	m_tAlphaBlendingSetting.u32Image2Width >>= 1;
	m_tAlphaBlendingSetting.u32Image2Height >>= 1;
	m_tAlphaBlendingSetting.s32Image2VisibleWidth >>= 1;
	m_tAlphaBlendingSetting.s32Image2VisibleHeight >>= 1;
	m_tAlphaBlendingSetting.s32PosLeftTopX >>= 1;
	m_tAlphaBlendingSetting.s32PosLeftTopY >>= 1;
	if (pu8Image1U != 0 && pu8Image2U != 0)
	{
		(this->*m_pfnDoAlphaBlending1Byte)(pu8Image1U, pu8Image2U, m_tAlphaBlendingSetting.u8BackgroundU);
	}
	if (pu8Image1V != 0 && pu8Image2V != 0)
	{
		(this->*m_pfnDoAlphaBlending1Byte)(pu8Image1V, pu8Image2V, m_tAlphaBlendingSetting.u8BackgroundV);
	}
	m_tAlphaBlendingSetting.u32Image1Width <<= 1;
	m_tAlphaBlendingSetting.u32Image1Height <<= 1;
	m_tAlphaBlendingSetting.u32Image2Width <<= 1;
	m_tAlphaBlendingSetting.u32Image2Height <<= 1;
	m_tAlphaBlendingSetting.s32Image2VisibleWidth <<= 1;
	m_tAlphaBlendingSetting.s32Image2VisibleHeight <<= 1;
	m_tAlphaBlendingSetting.s32PosLeftTopX <<= 1;
	m_tAlphaBlendingSetting.s32PosLeftTopY <<= 1;
#endif
	return 0;
}

#ifndef _USE_ALPHABLENDING_WMMX
void CAlphaBlending::DoAlphaBlending1Byte_C(unsigned char* pu8Image1Buff, unsigned char* pu8Image2Buff, unsigned char u8BackgroundColor)
{
	long  i, j;
	long  s32VisibleWidth = m_tAlphaBlendingSetting.s32Image2VisibleWidth;
	long  s32VisibleHeight = m_tAlphaBlendingSetting.s32Image2VisibleHeight;
	long  s32PosLeftTopX = m_tAlphaBlendingSetting.s32PosLeftTopX;
	long  s32PosLeftTopY = m_tAlphaBlendingSetting.s32PosLeftTopY;
	unsigned char*  pu8Image1Temp = pu8Image1Buff;
	unsigned char*  pu8Image2Temp = pu8Image2Buff;
	unsigned long  u32Alpha1 = m_tAlphaBlendingSetting.u32Alpha1;
	unsigned long  u32Alpha2 = m_tAlphaBlendingSetting.u32Alpha2;

	if (s32PosLeftTopY > 0)
	{
		pu8Image1Temp += m_tAlphaBlendingSetting.u32Image1Width*s32PosLeftTopY;
	}
	else
	{
		pu8Image2Temp += m_tAlphaBlendingSetting.u32Image2Width*(-s32PosLeftTopY);
	}
	for (i = 0; i < s32VisibleHeight; i++)
	{
		unsigned char*  pu8Image1;
		unsigned char*  pu8Image2;

		pu8Image1 = s32PosLeftTopX<0 ? pu8Image1Temp : pu8Image1Temp+s32PosLeftTopX;
		pu8Image2 = s32PosLeftTopX<0 ? pu8Image2Temp-s32PosLeftTopX : pu8Image2Temp;
		for (j = 0; j < s32VisibleWidth; j++)
		{
			if (*pu8Image2 != u8BackgroundColor)
			{
				unsigned long u32Temp;
				u32Temp = MULLONG_RSHIFT30(*pu8Image1, u32Alpha1)+MULLONG_RSHIFT30(*pu8Image2, u32Alpha2);
				*pu8Image1 = u32Temp>255 ? 255 : (unsigned char)u32Temp;
			}
			pu8Image1++;
			pu8Image2++;
		}

		pu8Image1Temp += m_tAlphaBlendingSetting.u32Image1Width;
		pu8Image2Temp += m_tAlphaBlendingSetting.u32Image2Width;
	}

	return;
}
#else
void CAlphaBlending::DoAlphaBlending1Byte_WMMX(unsigned char* pu8Image1Buff, unsigned char* pu8Image2Buff, unsigned char u8BackgroundColor)
{
	long  s32VisibleWidth = m_tAlphaBlendingSetting.s32Image2VisibleWidth;
	long  s32VisibleHeight = m_tAlphaBlendingSetting.s32Image2VisibleHeight;
	long  s32PosLeftTopX = m_tAlphaBlendingSetting.s32PosLeftTopX;
	long  s32PosLeftTopY = m_tAlphaBlendingSetting.s32PosLeftTopY;
	unsigned char*  pu8Image1Temp = pu8Image1Buff;
	unsigned char*  pu8Image2Temp = pu8Image2Buff;
	unsigned long  u32Alpha1 = m_tAlphaBlendingSetting.u32Alpha1;
	unsigned long  u32Alpha2 = m_tAlphaBlendingSetting.u32Alpha2;
	unsigned long  u32VisibleWidthIn16 = (unsigned long)s32VisibleWidth>>4;
	unsigned long  u32VisibleWidthRemain = (unsigned long)s32VisibleWidth&15;

	unsigned long  u32Param[8] =
	{
		u32Alpha1, u32Alpha2,
		s32VisibleHeight,
		(unsigned long)u8BackgroundColor,
		u32VisibleWidthIn16,
		u32VisibleWidthRemain,
		m_tAlphaBlendingSetting.u32Image1Width, m_tAlphaBlendingSetting.u32Image2Width
	};

	if (s32PosLeftTopY > 0)
	{
		pu8Image1Temp += m_tAlphaBlendingSetting.u32Image1Width*s32PosLeftTopY;
	}
	else
	{
		pu8Image2Temp += m_tAlphaBlendingSetting.u32Image2Width*(-s32PosLeftTopY);
	}
	pu8Image1Temp = s32PosLeftTopX<0 ? pu8Image1Temp : pu8Image1Temp+s32PosLeftTopX;
	pu8Image2Temp = s32PosLeftTopX<0 ? pu8Image2Temp-s32PosLeftTopX : pu8Image2Temp;

	DoAlphaBlending1ByteSub_WMMX(pu8Image1Temp, pu8Image2Temp, u32Param);

	return;
}
#endif // ~_USE_ALPHABLENDING_WMMX

void CAlphaBlending::DoCoverFirstImage1Byte_C(unsigned char* pu8Image1Buff, unsigned char* pu8Image2Buff, unsigned char u8BackgroundColor)
{
	long  i, j;
	long  s32VisibleWidth = m_tAlphaBlendingSetting.s32Image2VisibleWidth;
	long  s32VisibleHeight = m_tAlphaBlendingSetting.s32Image2VisibleHeight;
	long  s32PosLeftTopX = m_tAlphaBlendingSetting.s32PosLeftTopX;
	long  s32PosLeftTopY = m_tAlphaBlendingSetting.s32PosLeftTopY;
	unsigned char*  pu8Image1Temp = pu8Image1Buff;
	unsigned char*  pu8Image2Temp = pu8Image2Buff;
//	unsigned long  u32Alpha1 = m_tAlphaBlendingSetting.u32Alpha1;
//	unsigned long  u32Alpha2 = m_tAlphaBlendingSetting.u32Alpha2;

	if (s32PosLeftTopY > 0)
	{
		pu8Image1Temp += m_tAlphaBlendingSetting.u32Image1Width*s32PosLeftTopY;
	}
	else
	{
		pu8Image2Temp += m_tAlphaBlendingSetting.u32Image2Width*(-s32PosLeftTopY);
	}
	for (i = 0; i < s32VisibleHeight; i++)
	{
		unsigned char*  pu8Image1;
		unsigned char*  pu8Image2;

		pu8Image1 = s32PosLeftTopX<0 ? pu8Image1Temp : pu8Image1Temp+s32PosLeftTopX;
		pu8Image2 = s32PosLeftTopX<0 ? pu8Image2Temp-s32PosLeftTopX : pu8Image2Temp;
		for (j = 0; j < s32VisibleWidth; j++)
		{
			if (*pu8Image2 != u8BackgroundColor)
			{
				*pu8Image1 = *pu8Image2;
			}
			pu8Image1++;
			pu8Image2++;
		}

		pu8Image1Temp += m_tAlphaBlendingSetting.u32Image1Width;
		pu8Image2Temp += m_tAlphaBlendingSetting.u32Image2Width;
	}

	return;
}
