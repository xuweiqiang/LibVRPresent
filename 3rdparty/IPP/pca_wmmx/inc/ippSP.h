/******************************************************************************
// INTEL CONFIDENTIAL
// Copyright 2000 ~ 2005 Intel Corporation All Rights Reserved. 
// The source code contained or described herein and all documents related to 
// the source code (¡°Material¡±) are owned by Intel Corporation or its 
// suppliers or licensors. Title to the Material remains with Intel Corporation
// or its suppliers and licensors. The Material contains trade secrets and 
// proprietary and confidential information of Intel or its suppliers and 
// licensors. The Material is protected by worldwide copyright and trade secret 
// laws and treaty provisions. No part of the Material may be used, copied, 
// reproduced, modified, published, uploaded, posted, transmitted, distributed, 
// or disclosed in any way without Intel¡¯s prior express written permission.
//
// No license under any patent, copyright, trade secret or other intellectual 
// property right is granted to or conferred upon you by disclosure or delivery 
// of the Materials, either expressly, by implication, inducement, estoppel or 
// otherwise. Any license under such intellectual property rights must be 
// express and approved by Intel in writing.
******************************************************************************/

#ifndef _IPPSP_H_
#define _IPPSP_H_

#include "ippdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IPP_MAX_EXP 308

#ifndef _CALLBACK
#define _CALLBACK __STDCALL
#endif

void* _CALLBACK ippsMalloc(int size);
void  _CALLBACK ippsFree(void *pSrcBuf);


/* =============== Structure Definition for Sample Generation ============== */

typedef struct {
	Ipp32s	cosBase;
	Ipp32s	sinBase;
	Ipp32s	cosCurr;
	Ipp32s	sinCurr;
} IppToneState_16s;

typedef struct {
    Ipp64s	mag;
    Ipp64s	step;
    Ipp64s	step1;
    Ipp64s	step2;
    Ipp64s	st12;
    Ipp64s	st21;
    Ipp64s	last;
 } IppTriangleState_16s;

typedef struct {
	Ipp32s  high; 
	Ipp32s	low;
	Ipp32s	m_seed[2];                       
	Ipp32s	d_seed[3];                       
	Ipp32s	carry;
	Ipp32s  quickSeed;
	Ipp32s  current;
} IppRandUniformState_16s;

typedef struct {
	Ipp32s   useExtra;
	Ipp32s	seed2;
	Ipp32s	carry;
	Ipp32s	seed1[3];
	Ipp16s   mean;
	Ipp16s	stdDev;
	Ipp16s   extraValue;
	Ipp32s   quickSeed;
} IppRandGaussState_16s;


/* =============== Structure Definition for 2^N variant length FFT ============== */

typedef void IppsFFTSpec_C_16sc;
typedef void IppsFFTSpec_C_32sc;
typedef void IppsFFTSpec_R_16s32s;
typedef void IppsFFTSpec_R_32s;

/* =========================== General Functions =========================== */


/* Addition */
IPPAPI(IppStatus, ippsAddC_16s_I,
	(Ipp16s val, Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsAddC_16s_ISfs,	
	(Ipp16s val, Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsAddC_16s,	
	(const Ipp16s * pSrc, Ipp16s val, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsAddC_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s val, Ipp16s * pDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsAdd_16s_I,	
	(const Ipp16s * pSrc, Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsAdd_16s_ISfs,	
	(const Ipp16s * pSrc, Ipp16s * pSrcDst, int len , int scaleFactor))

IPPAPI(IppStatus, ippsAdd_16s,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsAdd_16s_Sfs,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, Ipp16s * pDst, int len, 
	int scaleFactor))

IPPAPI(IppStatus, ippsAdd_32s,
    (const Ipp32s * pSrc1, const Ipp32s * pSrc2, 
	Ipp32s * pDst, int len))

IPPAPI(IppStatus, ippsAdd_32s_Sfs,
    (const Ipp32s * pSrc1, const Ipp32s * pSrc2, 
	Ipp32s * pDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsAdd_32sc,
    (const Ipp32sc * pSrc1, const Ipp32sc * pSrc2, 
	Ipp32sc * pDst, int len))

IPPAPI(IppStatus, ippsAdd_32sc_Sfs,
    (const Ipp32sc * pSrc1, const Ipp32sc * pSrc2, 
	Ipp32sc * pDst, int len, int scaleFactor))


/* Subtraction */	   
IPPAPI(IppStatus, ippsSubC_16s_I,
	(Ipp16s val, Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsSubC_16s_ISfs,
	(Ipp16s val, Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsSubC_16s,	
	(const Ipp16s * pSrc, Ipp16s val, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsSubC_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s val, Ipp16s * pDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsSub_16s_I,
	(const Ipp16s * pSrc, Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsSub_16s_ISfs,	
	(const Ipp16s * pSrc, Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsSub_16s,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsSub_16s_Sfs,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, Ipp16s * pDst, int len, 
	int scaleFactor))

IPPAPI(IppStatus, ippsSubCRev_16s_I,	
	(Ipp16s val, Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsSubCRev_16s_ISfs,	
	(Ipp16s val, Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsSub_32s,
    (const Ipp32s * pSrc1, const Ipp32s * pSrc2, Ipp32s * pDst, int len))

IPPAPI(IppStatus, ippsSub_32s_Sfs,
    (const Ipp32s * pSrc1, const Ipp32s * pSrc2, Ipp32s * pDst, int len, 
    int scaleFactor))

IPPAPI(IppStatus, ippsSub_32sc,
    (const Ipp32sc * pSrc1, const Ipp32sc * pSrc2, Ipp32sc * pDst, int len))

IPPAPI(IppStatus, ippsSub_32sc_Sfs,
    (const Ipp32sc * pSrc1, const Ipp32sc * pSrc2, Ipp32sc * pDst, int len, 
    int scaleFactor))

/* Multiplication */	   
IPPAPI(IppStatus, ippsMulC_16s_I,
	(Ipp16s val, Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsMulC_16s_ISfs,
	(Ipp16s val, Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsMulC_16s,	
	(const Ipp16s * pSrc, Ipp16s val, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsMulC_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s val, Ipp16s * pDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsMul_16s_I,
	(const Ipp16s * pSrc, Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsMul_16s_ISfs,	
	(const Ipp16s * pSrc, Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsMul_16s,
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsMul_16s_Sfs,
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, Ipp16s * pDst, int len, 
	int scaleFactor))

IPPAPI(IppStatus, ippsMul_32s,
    (const Ipp32s * pSrc1, const Ipp32s * pSrc2, Ipp32s * pDst, int len))

IPPAPI(IppStatus, ippsMul_32s_Sfs,
    (const Ipp32s * pSrc1, const Ipp32s * pSrc2, Ipp32s * pDst, int len, 
    int scaleFactor))

IPPAPI(IppStatus, ippsMul_32sc,
    (const Ipp32sc * pSrc1, const Ipp32sc * pSrc2, Ipp32sc * pDst, int len))

IPPAPI(IppStatus, ippsMul_32sc_Sfs,
    (const Ipp32sc * pSrc1, const Ipp32sc * pSrc2, Ipp32sc * pDst, int len, 
    int scaleFactor))

IPPAPI(IppStatus, ippsMul_32s32sc_ISfs, 
    (const Ipp32s *pSrc, Ipp32sc *pSrcDst,int len, int scaleFactor))

IPPAPI(IppStatus, ippsMul_32s32sc_Sfs, 
    (const Ipp32s *pSrc1, const Ipp32sc *pSrc2, Ipp32sc* Dst, int len, 
    int scalefactor))

/* Absolution */	
IPPAPI(IppStatus, ippsAbs_16s_I,
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsAbs_16s,
	(const Ipp16s * pSrc, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsAbs_32s, 
    (const Ipp32s* pSrc,Ipp32s* pDst, int len))

IPPAPI(IppStatus, ippsAbs_32s_I, 
    (Ipp32s* pSrcDst, int len))

IPPAPI(IppStatus, ippsAbs_32sc32s,
    (const  Ipp32sc  *pSrc,Ipp32s  *pDst,int  len))

IPPAPI(IppStatus, ippsAbs_32sc32s_Sfs, 
    (const Ipp32sc *pSrc, Ipp32s *pDst, int len, int scaleFactor))


/* Square */
IPPAPI(IppStatus, ippsSqr_16s_I,	
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsSqr_16s_ISfs,	
	(Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsSqr_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsSqr_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, int scaleFactor))

/* Square root */
IPPAPI(IppStatus, ippsSqrt_16s_I,
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsSqrt_16s_ISfs,
	(Ipp16s * pSrcDst, int len , int scaleFactor))

IPPAPI(IppStatus, ippsSqrt_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsSqrt_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, int scaleFactor))

/* Exponent */
IPPAPI(IppStatus, ippsExp_16s_I,
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsExp_16s_ISfs,	
	(Ipp16s * pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsExp_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsExp_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, int scaleFactor))


/* Logarithm */
IPPAPI(IppStatus, ippsLn_16s_I,	
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsLn_16s_ISfs,	
	(Ipp16s * pSrcDst, int len , int scaleFactor))

IPPAPI(IppStatus, ippsLn_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len))

IPPAPI(IppStatus, ippsLn_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsLn_32s, 
    (const Ipp32s *pSrc, Ipp32s *pDst, int len))

IPPAPI(IppStatus, ippsLn_32s_I, 
    (Ipp32s *pSrcDst, int len))

IPPAPI(IppStatus, ippsLn_32s_Sfs, 
    (const Ipp32s *pSrc, Ipp32s *pDst, int len, int scaleFactor))

IPPAPI(IppStatus, ippsLn_32s_ISfs, 
    (Ipp32s *pSrcDst, int len, int scaleFactor))

IPPAPI(IppStatus, ipps10Log10_32s, 
    (const Ipp32s *pSrc, Ipp32s *pDst, int len))

IPPAPI(IppStatus, ipps10Log10_32s_I, 
    (Ipp32s *pSrcDst, int len))

IPPAPI(IppStatus, ipps10Log10_32s_Sfs, 
    (const Ipp32s *pSrc, Ipp32s *pDst, int len, int scaleFactor))

IPPAPI(IppStatus, ipps10Log10_32s_ISfs, 
    (Ipp32s *pSrcDst, int len, int scaleFactor))

	
/* Dot product */
IPPAPI(IppStatus, ippsDotProd_16s,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp16s * pResult))

IPPAPI(IppStatus, ippsDotProd_16s_Sfs,
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp16s * pResult, 
	int scaleFactor))

/* Threshold */
IPPAPI(IppStatus, ippsThreshold_GT_16s_I,	
	(Ipp16s * pSrcDst, int len, Ipp16s threshold))

IPPAPI(IppStatus, ippsThreshold_GT_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, Ipp16s threshold))

IPPAPI(IppStatus, ippsThreshold_GTVal_16s_I,	
	(Ipp16s * pSrcDst, int len, Ipp16s threshold, Ipp16s val))

IPPAPI(IppStatus, ippsThreshold_GTVal_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, Ipp16s threshold, Ipp16s val))

IPPAPI(IppStatus, ippsThreshold_LT_16s_I,
	(Ipp16s * pSrcDst, int len, Ipp16s threshold))

IPPAPI(IppStatus, ippsThreshold_LT_16s,
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, Ipp16s threshold))

IPPAPI(IppStatus, ippsThreshold_LTVal_16s_I,
	(Ipp16s * pSrcDst, int len, Ipp16s threshold, Ipp16s val))

IPPAPI(IppStatus, ippsThreshold_LTVal_16s,
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, Ipp16s threshold, Ipp16s val))

IPPAPI(IppStatus, ippsThreshold_LTValGTVal_16s_I,
	(Ipp16s * pSrcDst, int len, Ipp16s gtThreshold, Ipp16s gtVal, 
	Ipp16s ltThreshold, Ipp16s ltVal))

IPPAPI(IppStatus, ippsThreshold_LTValGTVal_16s,
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, Ipp16s gtThreshold, Ipp16s gtVal, 
	Ipp16s ltThreshold, Ipp16s ltVal))

IPPAPI(IppStatus, ippsThreshold_LT_32s, 
    (const Ipp32s  *pSrc, Ipp32s* Dst, int len, Ipp32s threshold))

IPPAPI(IppStatus, ippsThreshold_LT_32s_I, 
    (Ipp32s  *pSrcDst, int len, Ipp32s threshold))


/* Normalization */
IPPAPI(IppStatus, ippsNormalize_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, Ipp16s offset, int normFactor))

IPPAPI(IppStatus, ippsNormalize_16s_Sfs,
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, Ipp16s offset, int normFactor, 
	int scaleFactor))

/* Copy */
IPPAPI(IppStatus, ippsCopy_16s,
	(const Ipp16s * pSrc, Ipp16s * pDst, int len))

/* Set */
IPPAPI(IppStatus, ippsSet_16s,
	(Ipp16s val, Ipp16s * pDst, int len))

/* Zero */
IPPAPI(IppStatus, ippsZero_16s,
	(Ipp16s * pDst, int len))

/* Max */
IPPAPI(IppStatus, ippsMax_16s,
	(const Ipp16s * pSrc, int len, Ipp16s * pResult))

/* Min */
IPPAPI(IppStatus, ippsMin_16s,
	(const Ipp16s * pSrc, int len, Ipp16s * pResult))

/* Mean	*/
IPPAPI(IppStatus, ippsMean_16s,
	(const Ipp16s * pSrc, int len, Ipp16s * pResult))

/* Standard deviation */
IPPAPI(IppStatus, ippsStdDev_16s,	
	(const Ipp16s * pSrc, int len, Ipp16s * pResult))

IPPAPI(IppStatus, ippsStdDev_16s_Sfs,	
	(const Ipp16s * pSrc, int len, Ipp16s * pResult, int scaleFactor))

/* Norm	*/
IPPAPI(IppStatus, ippsNormDiff_L1_16s32s,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp32s * pResult))

IPPAPI(IppStatus, ippsNormDiff_L1_16s32s_Sfs,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp32s * pResult,
	int scaleFactor))

IPPAPI(IppStatus, ippsNormDiff_L2_16s32s,
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp32s * pResult))

IPPAPI(IppStatus, ippsNormDiff_L2_16s32s_Sfs,
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp32s * pResult, 
	int scaleFactor))

IPPAPI(IppStatus, ippsNormDiff_Inf_16s32s,
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp32s * pResult))

IPPAPI(IppStatus, ippsNormDiff_Inf_16s32s_Sfs,	
	(const Ipp16s * pSrc1, const Ipp16s * pSrc2, int len, Ipp32s * pResult, 
	int scaleFactor))

IPPAPI(IppStatus, ippsNorm_L1_16s32s,
	(const Ipp16s * pSrc, int len, Ipp32s * pResult))

IPPAPI(IppStatus, ippsNorm_L1_16s32s_Sfs,	
	(const Ipp16s * pSrc, int len, Ipp32s * pResult, int scaleFactor))

IPPAPI(IppStatus, ippsNorm_L2_16s32s,
	(const Ipp16s * pSrc, int len, Ipp32s * pResult))

IPPAPI(IppStatus, ippsNorm_L2_16s32s_Sfs,
	(const Ipp16s * pSrc, int len, Ipp32s * pResult, int scaleFactor))

IPPAPI(IppStatus, ippsNorm_Inf_16s32s,	
	(const Ipp16s * pSrc, int len, Ipp32s * pResult))

IPPAPI(IppStatus, ippsNorm_Inf_16s32s_Sfs,
	(const Ipp16s * pSrc, int len, Ipp32s * pResult, int scaleFactor))

/* Upsample */
IPPAPI(IppStatus, ippsUpSample_16s,	
	(const Ipp16s * pSrc, int srcLen, int * pSrcDstPhase, Ipp16s * pDst, 
	int sampleFactor))

IPPAPI(IppStatus, ippsUpSampleSize,	
	(int srcLen, int sampleFactor, int phase, int * pDstLen))

/* Downsample */
IPPAPI(IppStatus, ippsDownSample_16s,
	(const Ipp16s * pSrc, int srcLen, int * pSrcDstPhase, Ipp16s * pDst, 
	int sampleFactor))

IPPAPI(IppStatus, ippsDownSampleSize,
	(int srcLen, int sampleFactor, int phase, int * pDstLen))

/* Correlation */
IPPAPI(IppStatus, ippsAutoCorr_16s,	
	(const Ipp16s * pSrc, int srcLen, Ipp16s * pDst, int dstLen))

IPPAPI(IppStatus, ippsAutoCorr_16s_Sfs,	
	(const Ipp16s * pSrc, int srcLen, Ipp16s * pDst, int dstLen, 
	int scaleFactor))

IPPAPI(IppStatus, ippsAutoCorr_NormA_16s,
	(const Ipp16s * pSrc, int srcLen, Ipp16s * pDst, int dstLen))

IPPAPI(IppStatus, ippsAutoCorr_NormA_16s_Sfs,
	(const Ipp16s * pSrc, int srcLen, Ipp16s * pDst, int dstLen, 
	int scaleFactor))

IPPAPI(IppStatus, ippsAutoCorr_NormB_16s,
	(const Ipp16s * pSrc, int srcLen, Ipp16s * pDst, int dstLen))

IPPAPI(IppStatus, ippsAutoCorr_NormB_16s_Sfs,
	(const Ipp16s * pSrc, int srcLen, Ipp16s * pDst, int dstLen, 
	int scaleFactor))

IPPAPI(IppStatus, ippsCrossCorr_16s,
	(const Ipp16s * pSrc1, int src1Len, const Ipp16s * pSrc2, int src2Len,
	Ipp16s * pDst, int dstLen, int lowLag))

IPPAPI(IppStatus, ippsCrossCorr_16s_Sfs,
	(const Ipp16s * pSrc1, int src1Len, const Ipp16s * pSrc2, int src2Len,
	Ipp16s * pDst, int dstLen, int lowLag, int scaleFactor))

/* =========================== Filter Functions ============================ */
	
/* FIR single rate */
IPPAPI(IppStatus, ippsFIR_Direct_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int sampLen, const Ipp16s * pTapsQ15, 
	int tapsLen, Ipp16s * pDelayLine, int * pDelayLineIndex))

IPPAPI(IppStatus, ippsFIR_Direct_16s_I,	
	(Ipp16s * pSrcDst, int sampLen, const Ipp16s * pTapsQ15, int tapsLen, 
	Ipp16s * pDelayLine, int * pDelayLineIndex))

IPPAPI(IppStatus, ippsFIROne_Direct_16s,
	(Ipp16s val, Ipp16s * pResult, const Ipp16s * pTapsQ15, int tapsLen, 
	Ipp16s * pDelayLine, int * pDelayLineIndex))

IPPAPI(IppStatus, ippsFIROne_Direct_16s_I,	
	(Ipp16s * pValResult, const Ipp16s * pTapsQ15, int tapsLen, 
	Ipp16s * pDelayLine, int * pDelayLineIndex))


IPPAPI(IppStatus, ippsFIR_Direct_16s_Sfs,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int sampLen, const Ipp16s * pTapsQ15, 
	int tapsLen, Ipp16s * pDelayLine, int * pDelayLineIndex, int scaleFactor))

IPPAPI(IppStatus, ippsFIR_Direct_16s_ISfs,	
	(Ipp16s * pSrcDst, int sampLen, const Ipp16s * pTapsQ15, int tapsLen, 
	Ipp16s * pDelayLine, int * pDelayLineIndex, int scaleFactor))

IPPAPI(IppStatus, ippsFIROne_Direct_16s_Sfs,
	(Ipp16s val, Ipp16s * pResult, const Ipp16s * pTapsQ15, int tapsLen, 
	Ipp16s * pDelayLine, int * pDelayLineIndex, int scaleFactor))

IPPAPI(IppStatus, ippsFIROne_Direct_16s_ISfs,	
	(Ipp16s * pValResult, const Ipp16s * pTapsQ15, int tapsLen, 
	Ipp16s * pDelayLine, int * pDelayLineIndex, int scaleFactor))

/* FIR Multirate */
IPPAPI(IppStatus, ippsFIRMR32s_Direct_16s_Sfs,
	(const Ipp16s*	pSrc, Ipp16s* pDst, int numIters, const Ipp32s*	pTaps,
	int tapsLen, int tapsFactor, int upFactor, int upPhase, int downFactor,
	int downPhase, Ipp16s* pDlyLine, int scaleFactor))

/* IIR direct form */
IPPAPI(IppStatus, ippsIIR_Direct_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, const Ipp16s * pTaps, 
	int order, Ipp32s * pDelayLine))

IPPAPI(IppStatus, ippsIIR_Direct_16s_I,	
	(Ipp16s * pSrcDst, int len, const Ipp16s * pTaps, int order, 
	Ipp32s * pDelayLine))

IPPAPI(IppStatus, ippsIIROne_Direct_16s,
	(Ipp16s val, Ipp16s * pResult, const Ipp16s * pTaps, int order, 
	Ipp32s * pDelayLine))

IPPAPI(IppStatus, ippsIIROne_Direct_16s_I,
	(Ipp16s * pValResult, const Ipp16s * pTaps, int order, Ipp32s * pDelayLine))

/* IIR biquad */
IPPAPI(IppStatus, ippsIIR_BiQuadDirect_16s,	
	(const Ipp16s * pSrc, Ipp16s * pDst, int len, const Ipp16s * pTaps, 
	int numBiquad, Ipp32s * pDelayLine))

IPPAPI(IppStatus, ippsIIR_BiQuadDirect_16s_I,
	(Ipp16s * pSrcDst, int len, const Ipp16s * pTaps, int numBiquad, 
	Ipp32s * pDelayLine))

IPPAPI(IppStatus, ippsIIROne_BiQuadDirect_16s,
	(Ipp16s val, Ipp16s * pResult, const Ipp16s * pTaps, int numBiquad, 
	Ipp32s * pDelayLine))

IPPAPI(IppStatus, ippsIIROne_BiQuadDirect_16s_I,
	(Ipp16s * pValResult, const Ipp16s * pTaps, int numBiquad, 
	Ipp32s * pDelayLine))

/* LMS FIR */
IPPAPI(IppStatus, ippsFIRLMSOne_Direct_16s,	
	(Ipp16s val, Ipp16s valDesire, Ipp16s * pResult, Ipp32s * pTapsQ15, 
	int tapsLen, Ipp32s stepQ15, Ipp16s * pDelayLine, int * pDelayLineIndex))

IPPAPI(IppStatus, ippsFIRLMSOne_Direct_16s_I,	
	(Ipp16s * pValResult, Ipp16s valDesire, Ipp32s * pTapsQ15, int tapsLen, 
	Ipp32s stepQ15, Ipp16s * pDelayLine, int * pDelayLineIndex))
		
/* ========================= Convolution Functions ========================= */

/* 1D convolution */
IPPAPI(IppStatus, ippsConv_16s,	
	(const Ipp16s * pSrc1, int src1Len, const Ipp16s * pSrc2, int src2Len, 
	Ipp16s * pDst))

IPPAPI(IppStatus, ippsConv_16s_Sfs,	
	(const Ipp16s * pSrc1, int src1Len, const Ipp16s * pSrc2, int src2Len,
	Ipp16s * pDst, int scaleFactor))

/* ========================== Transform Functions ========================== */

/* Variant-length (2^order) 16-bit complex FFT */
IPPAPI(IppStatus, ippsFFTFwd_CToC_16sc_Sfs,
    (const Ipp16sc *pSrc, Ipp16sc *pDst, const IppsFFTSpec_C_16sc *pFFTSpec, 
    int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTInv_CToC_16sc_Sfs,
	(const Ipp16sc *pSrc, Ipp16sc *pDst, const IppsFFTSpec_C_16sc *pFFTSpec,
	 int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTInitAlloc_C_16sc,
    (IppsFFTSpec_C_16sc** pFFTSpec, int order, int flag, IppHintAlgorithm hint))

IPPAPI(IppStatus, ippsFFTGetBufSize_C_16sc,
	(const IppsFFTSpec_C_16sc *pFFTSpec, int *pSize))

IPPAPI(IppStatus, ippsFFTFree_C_16sc,
    (IppsFFTSpec_C_16sc* pFFTSpec))
    
    
/* Variant-length (2^order) 32-bit complex FFT */
IPPAPI(IppStatus, ippsFFTFwd_CToC_32sc_Sfs,
    (const Ipp32sc *pSrc, Ipp32sc *pDst, const IppsFFTSpec_C_32sc *pFFTSpec,
	int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTInv_CToC_32sc_Sfs,
    (const Ipp32sc *pSrc, Ipp32sc *pDst, const IppsFFTSpec_C_32sc *pFFTSpec,
	int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTInitAlloc_C_32sc,
    (IppsFFTSpec_C_32sc** pFFTSpec, int order, int flag, IppHintAlgorithm hint))

IPPAPI(IppStatus, ippsFFTGetBufSize_C_32sc,
	(const IppsFFTSpec_C_32sc *pFFTSpec, int *pSize))

IPPAPI(IppStatus, ippsFFTFree_C_32sc,
    (IppsFFTSpec_C_32sc* pFFTSpec))
    

/* Variant-length (2^order) real->complex FFT */
IPPAPI(IppStatus, ippsFFTFwd_RToCCS_16s32s_Sfs,
    (const Ipp16s *pSrc, Ipp32s *pDst, const IppsFFTSpec_R_16s32s *pFFTSpec,
	int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTInv_CCSToR_32s16s_Sfs,
    (const Ipp32s *pSrc, Ipp16s *pDst, const IppsFFTSpec_R_16s32s *pFFTSpec,
	int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTFwd_RToCCS_32s_Sfs,
    (const Ipp32s *pSrc, Ipp32s *pDst, const IppsFFTSpec_R_32s *pFFTSpec,
	int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTInv_CCSToR_32s_Sfs,
    (const Ipp32s *pSrc, Ipp32s *pDst, const IppsFFTSpec_R_32s *pFFTSpec,
	int scaleFactor, Ipp8u *pBuffer))

IPPAPI(IppStatus, ippsFFTInitAlloc_R_16s32s,
    (IppsFFTSpec_R_16s32s** pFFTSpec, int order, int flag, 
	IppHintAlgorithm hint))

IPPAPI(IppStatus, ippsFFTInitAlloc_R_32s,
    (IppsFFTSpec_R_32s** pFFTSpec, int order, int flag, 
	IppHintAlgorithm hint))

IPPAPI(IppStatus, ippsFFTGetBufSize_R_16s32s,
	(const IppsFFTSpec_R_16s32s *pFFTSpec, int *pSize))

IPPAPI(IppStatus, ippsFFTGetBufSize_R_32s,
	(const IppsFFTSpec_R_32s *pFFTSpec, int *pSize))

IPPAPI(IppStatus, ippsFFTFree_R_16s32s,
    (IppsFFTSpec_R_16s32s* pFFTSpec))

IPPAPI(IppStatus, ippsFFTFree_R_32s,
    (IppsFFTSpec_R_32s* pFFTSpec))    


/* ========================== Windowing Functions ========================== */

IPPAPI(IppStatus, ippsWinBartlett_16s_I,
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsWinHann_16s_I,
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsWinHamming_16s_I,	
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsWinBlackmanQ15_16s_I,
	(Ipp16s * pSrcDst, int len, Ipp32s alphaQ15))

IPPAPI(IppStatus, ippsWinBlackmanQ15_16s_ISfs,	
	(Ipp16s * pSrcDst, int len, Ipp32s alphaQ15, int scaleFactor))

IPPAPI(IppStatus, ippsWinBlackmanStd_16s_I,	
	(Ipp16s * pSrcDst, int len))

IPPAPI(IppStatus, ippsWinBlackmanOpt_16s_I,	
	(Ipp16s * pSrcDst, int len))


/* ====================== Sample Generation Functions ====================== */

/* Tone */
IPPAPI(IppStatus, ippsToneGetStateSizeQ15_16s,
	(int * pToneStateSize))

IPPAPI(IppStatus, ippsToneInitQ15_16s,	
	(IppToneState_16s * pToneState, Ipp16s magQ15, Ipp16s freqQ15, 
	Ipp32s phaseQ15))

IPPAPI(IppStatus, ippsToneQ15_16s,	
	(Ipp16s * pDst, int len, IppToneState_16s * pToneState))

IPPAPI(IppStatus, ippsToneQ15_Direct_16s,
	(Ipp16s * pDst, int len, Ipp16s magQ15, Ipp16s freqQ15, Ipp32s phaseQ15))

/* Triangle */
IPPAPI(IppStatus, ippsTriangleGetStateSizeQ15_16s,
	(int * pTriangleStateSize))

IPPAPI(IppStatus, ippsTriangleInitQ15_16s,
	(IppTriangleState_16s * pTriangleState, Ipp16s magQ15, Ipp16s freqQ15, 
	Ipp32s phaseQ15, Ipp32s asymQ15))

IPPAPI(IppStatus, ippsTriangleQ15_16s,	
	(Ipp16s * pDst, int len, IppTriangleState_16s * pTriangleState))

IPPAPI(IppStatus, ippsTriangleQ15_Direct_16s,	
	(Ipp16s * pDst, int len, Ipp16s magQ15, Ipp16s freqQ15, Ipp32s phaseQ15, 
	Ipp32s asymQ15))

/* Random */
IPPAPI(IppStatus, ippsRandUniformGetSize_16s,
	(int * pRandUniformStateSize))

IPPAPI(IppStatus, ippsRandUniformInit_16s,
	(IppRandUniformState_16s * pRandUniformState, 
	Ipp16s low, Ipp16s high, Ipp16s seed))

IPPAPI(IppStatus, ippsRandUniform_16s,
	(Ipp16s * pDst, int len, IppRandUniformState_16s * pRandUniformState))

IPPAPI(IppStatus, ippsRandGaussGetSize_16s,	
	(int * pRandGaussStateSize))

IPPAPI(IppStatus, ippsRandGaussInit_16s,
	(IppRandGaussState_16s * pRandGaussState, Ipp16s mean, 
	Ipp16s stdDev, Ipp32u seed))

IPPAPI(IppStatus, ippsRandGauss_16s,
	(Ipp16s * pDst, int len, IppRandGaussState_16s * pRandGaussState))

/* Weighted average */
IPPAPI(IppStatus, ippsAddWeightedQ31_32s, 
    (const Ipp32s *pSrcOld, const Ipp32s *pSrcNew, Ipp32s *pDst, 
    int len, Ipp32s weightQ31))

IPPAPI(IppStatus, ippsAddWeightedQ31_32s_I, 
    (const Ipp32s *pSrc, Ipp32s *pSrcDst, int len, Ipp32s weightQ31))

/* Magnitude squared */
IPPAPI(IppStatus, ippsMagSquared_32sc32s, 
    (Ipp32sc *pSrc, Ipp32s *pDst, int len))

IPPAPI(IppStatus, ippsMagSquared_32sc32s_Sfs, 
    (Ipp32sc *pSrc, Ipp32s *pDst, int len, int scaleFactor))

/* Summation */
IPPAPI(IppStatus, ippsSum_32s, 
    (const Ipp32s *pSrc, int len, Ipp32s *pDst))

IPPAPI(IppStatus, ippsSum_32s_Sfs, 
    (const Ipp32s *pSrc, int len, Ipp32s *pDst, int scaleFactor) )

/* Division */
IPPAPI(IppStatus, ippsDiv_32s_Sfs, 
    (const Ipp32s *pSrc1,const Ipp32s *pSrc2,Ipp32s *pDst,int len,
    int scalefactor))

IPPAPI(IppStatus, ippsDiv_32s_ISfs, 
    (const Ipp32s *pSrc,Ipp32s *pSrcDst,int len,int scalefactor))

IPPAPI(IppStatus, ippsDivQ15_32s, 
    (const Ipp32s *pSrc1,const Ipp32s *pSrc2,Ipp32s *pDst,int len))

IPPAPI(IppStatus, ippsDivQ15_32s_I, 
    (const Ipp32s *pSrc,Ipp32s *pSrcDst,int len))

/* Median */
IPPAPI(IppStatus, ippsFilterMedian_32s, 
    (const Ipp32s *pSrc, Ipp32s *pDst, int len, int maskSize))

IPPAPI(IppStatus, ippsFilterMedian_32s_I, 
    (Ipp32s *pSrcDst, int len, int maskSize))

#ifdef __cplusplus
}
#endif

#endif /* _IPPSP_H_ */ 
	
/* End of file */
