#ifndef _ALPHABLENDING_H_
#define _ALPHABLENDING_H_

#define       ALPHABLENDING_POSITION_CHANGED               0x00000001
#define       ALPHABLENDING_ALPHA_CHANGED                  0x00000002
#define       ALPHABLENDING_IMAGE1SIZE_CHANGED             0x00000004
#define       ALPHABLENDING_IMAGE2SIZE_CHANGED             0x00000008
#define       ALPHABLENDING_BACKGROUNDCOLOR_CHANGED        0x00000010

class CAlphaBlending
{
public:
	CAlphaBlending();
	~CAlphaBlending() {}

	int CheckAlphaBlendingSettings
	(
		 unsigned long u32Image1Width, unsigned long u32Image1Height,
		 unsigned long u32Image2Width, unsigned long u32Image2Height,
		 long s32PosLeftTopX, long s32PosLeftTopY,
		 unsigned char u8BackgroundY, unsigned char u8BackgroundU, unsigned char u8BackgroundV,
		 double f64Alpha
	);

	int SetAlphaBlendingPos
	(
		long s32PosLeftTopX,
		long s32PosLeftTopY
	);

	int SetAlphaBlendingAlpha
	(
		double f64Alpha
	);

	int SetAlphaBlendingImage1Size
	(
		unsigned long u32Image1Width,
		unsigned long u32Image1Height
	);

	int SetAlphaBlendingImage2Size
	(
		unsigned long u32Image2Width,
		unsigned long u32Image2Height
	);

	int SetAlphaBlendingBackgroundColor
	(
		unsigned char u8BackgroundY,
		unsigned char u8BackgroundU,
		unsigned char u8BackgroundV
	);

	int SetAlphaBlendingYUV
	(
		unsigned long u32Image1Width, unsigned long u32Image1Height,
		unsigned long u32Image2Width, unsigned long u32Image2Height,
		long s32PosLeftTopX, long s32PosLeftTopY,
		unsigned char u8BackgroundY, unsigned char u8BackgroundU, unsigned char u8BackgroundV,
		double f64Alpha
	);

	int DoAlphaBlendingYUV
	(
		unsigned char* pu8Image1Y, unsigned char* pu8Image1U, unsigned char* pu8Image1V,
		unsigned char* pu8Image2Y, unsigned char* pu8Image2U, unsigned char* pu8Image2V
	);

private:
	typedef struct _tagTAlphaBlendingSetting
	{
		unsigned long  u32Image1Width;
		unsigned long  u32Image1Height;
		unsigned long  u32Image2Width;
		unsigned long  u32Image2Height;
		long  s32Image2VisibleWidth;
		long  s32Image2VisibleHeight;
		long  s32PosLeftTopX;
		long  s32PosLeftTopY;
		unsigned char   u8BackgroundY;
		unsigned char   u8BackgroundU;
		unsigned char   u8BackgroundV;
		double  f64Alpha;
		unsigned long  u32Alpha1;
		unsigned long  u32Alpha2;
	}TAlphaBlendingSetting;
	TAlphaBlendingSetting  m_tAlphaBlendingSetting;

#ifndef _USE_ALPHABLENDING_WMMX
	void DoAlphaBlending1Byte_C(unsigned char* pu8Image1Buff, unsigned char* pu8Image2Buff, unsigned char u8BackgroundColor);
#else
	void DoAlphaBlending1Byte_WMMX(unsigned char* pu8Image1Buff, unsigned char* pu8Image2Buff, unsigned char u8BackgroundColor);
#endif

	void DoCoverFirstImage1Byte_C(unsigned char* pu8Image1Buff, unsigned char* pu8Image2Buff, unsigned char u8BackgroundColor);

	typedef void (CAlphaBlending::*PFNTYPE)(unsigned char* pu8Image1Buff, unsigned char* pu8Image2Buff, unsigned char u8BackgroundColor);
	PFNTYPE  m_pfnDoAlphaBlending1Byte;
};

#endif  //~_ALPHABLENDING_H_