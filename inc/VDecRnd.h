// VDecRnd.h : main header file for the VDecRnd DLL
//

#pragma once

#include "GfxPresent.h"

#ifndef EXPORT
#define EXPORT extern "C" _declspec(dllexport)
#endif

#ifndef CI_DEFINE_GUID

#ifdef __GNUG__  /* gcc/g++ compiler (Linux, Symbian ARMI) does the equivalent of _declspec(selectany) */
#define ATTRIBUTE_WEAK     __attribute__ ((weak))
#define DECLSPEC_SELECTANY
#elif defined(__ADS__)
#define ATTRIBUTE_WEAK  
#define DECLSPEC_SELECTANY 
#else
#define ATTRIBUTE_WEAK
#if !defined(DECLSPEC_SELECTANY)
#define DECLSPEC_SELECTANY _declspec(selectany)
#endif
#endif
	#define CI_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
		 EXTERN_C DECLSPEC_SELECTANY const GUID FAR name ATTRIBUTE_WEAK \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }                
#endif


CI_DEFINE_GUID(IID_IMcVDecRnd_Decoder,
0x6a6de540, 0xa540, 0x43b7, 0x94, 0x50, 0x3e, 0x7d, 0x24, 0xf0, 0x0, 0xe6);

// CVDecRndApp
// See VDecRnd.cpp for the implementation of this class
//
typedef enum
{
	MPEG2,
	H264
}VDECRND_DECTYPE;

typedef struct 
{
	long flags;
}DOpenOptions;

typedef struct 
{
	long flags;
}DResetOptions;

typedef struct 
{
	long flags;
}DDecodeOptions;

typedef struct 
{
	long flags;
}DGetFrameOptions;
typedef struct 
{
	long flags;
}DFrame;

DECLARE_INTERFACE_(IMcVDecRnd_Decoder,IUnknown)
{
	STDMETHOD(VDec_Open)(DOpenOptions* pOptions, VDECRND_DECTYPE type) PURE;
	STDMETHOD(VDec_Close)()PURE;
	STDMETHOD(VDec_Reset)(DResetOptions* pOptions)PURE;
	STDMETHOD(VDec_DecodeFrame)(DDecodeOptions* pOptions, unsigned char** StreamBuffer,int BufLen)PURE;
	STDMETHOD(VDec_GetFrame)(DGetFrameOptions* pOptions, DFrame* pFrame)PURE;
	STDMETHOD(VDec_ReleaseFrame)(DFrame* pFrame)PURE;
};

extern "C" _declspec(dllexport) IMcVDecRnd_Decoder *CreateVDecRnd();

typedef  IMcVDecRnd_Decoder*  (*fpCreateVDecRnd)();
