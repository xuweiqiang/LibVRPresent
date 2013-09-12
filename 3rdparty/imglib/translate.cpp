#include "ci_codec_type.h"


#define C_SHIFT 7

#define CLAMP(x,low,high) ((x)<low ? low : (x)>high ? high : (x))

#define ASSEMBLE565(r,g,b) ((r)<<11 | (g)<<5 | (b))

#define SAT(val, bot, top) (val<bot?bot:(val>top?top:val))

const double ColorMatrix[12] = 
{
	1.1644f, 0.0000f, 1.5960f,-222.9216f, 
	1.1644f,-0.3918f,-0.8130f, 135.5753f, 
	1.1644f, 2.0172f, 0.0000f,-276.8359f, 
};

//u32Mode: 3: CI_IMAGEPROC_CSC_YCBCR420_TO_RGB565
//u32InKey: 0<<24 | y<<16 | u<<8 | v<<0
//u32OutKey: rgb565 & 0xFFFF

CI_RESULT CI_TranslateColorKey(IN CI_U32 u32Mode, IN CI_U32 u32InKey, OUT CI_U32 *pu32OutKey)
{
	if(0 == pu32OutKey)
		return CI_EINVALIDARG;

	switch(u32Mode)
	{
	case 3:
		{
			CI_U32 y, u, v, r, g, b;
			double f0, f2, f6, f9, f5;
			CI_S32 s0, s2, s6, s9, s5;
			double *pCoef = (double *)&ColorMatrix[0];
			y = (u32InKey>>16) & 0xFF;
			u = (u32InKey>>8) & 0xFF;
			v = (u32InKey>>0) & 0xFF;
			f0 = (pCoef[0] * (y-16) * (double)(1<<C_SHIFT))+0.5;
			f2 = (pCoef[2] * (v-128) * (double)(1<<(C_SHIFT-1)))+0.5;
			f5 = (pCoef[5] * (u-128) * (double)(1<<(C_SHIFT-1)))+0.5;
			f6 = (pCoef[6] * (v-128) * (double)(1<<(C_SHIFT-1)))+0.5;
			f9 = (pCoef[9] * (u-128) * (double)(1<<(C_SHIFT-1)))+0.5;

			s0 = (int)f0;
			s0 = CLAMP(s0,0,65535);

			s2 = (int)f2;
			s2 = CLAMP(s2,-32768,32767);

			s5 = (int)f5;
			s5 = CLAMP(s5,-32768,32767);

			s6 = (int)f6;
			s6 = CLAMP(s6,-32768,32767);

			s9 = (int)f9;
			s9 = CLAMP(s9,-32768,32767);

			r = (s0+s2)>>(C_SHIFT+8-5);
			r = SAT(r, 0, 31);
			g = (s0+((s5+s6)<<1))>>(C_SHIFT+8-6);
			g = SAT(g, 0, 63);
			b = (s0+s9)>>(C_SHIFT+8-5);
			b = SAT(b, 0, 31);

			*pu32OutKey = (ASSEMBLE565(r, g, b)) & 0xFFFF;

		}
		break;
	default:
		return CI_EFAIL;
	}

	return CI_SOK;
}


