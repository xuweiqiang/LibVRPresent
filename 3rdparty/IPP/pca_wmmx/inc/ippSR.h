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

#ifndef __IPPSR_H__
#define __IPPSR_H__

#include "ippdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========= Enums for parameter definitions =============================== */

typedef enum {
    IPP_CDBK_FULL=1, 
    IPP_CDBK_KMEANS_LONG=2, 
    IPP_CDBK_KMEANS_NUM=4
} Ipp_Cdbk_Hint;

typedef enum {
    IPP_FBANK_MELWGT=1, 
    IPP_FBANK_FREQWGT=2, 
    IPP_POWER_SPECTRUM=4
} IppMelMode;

typedef struct DCTLifterState_16s IppsDCTLifterState_16s;
typedef struct FBankState_32s IppsFBankState_32s;
typedef struct CdbkState_16s IppsCdbkState_16s;

/* =============== Structure Definition for Acoustic Echo Cancellation ===== */

typedef struct AECCtrlState_Def IppAECCtrlState;

typedef struct AECNLMSParam_Def {
    Ipp16s *pMicrophone;     /* pointer to mic samples */
    Ipp16s *pLoudspeaker;    /* pointer to speaker samples */
    Ipp16s *pError;          /* pointer to error samples */
    Ipp32s *pAFInputPSD;     /* pointer to filter input PSD */
    Ipp32sc **ppAFCoefs;     /* pointer to filter segment array */
    Ipp32s muQ31;            /* fixed step size (Q31 value in (0,1)) */
    Ipp32s AECOutGainQ30;    /* AEC output gain (Q30 value in [0,1]) */
    Ipp32s speakerGainQ30;   /* loudspeaker gain (Q30 value in [0,1]) */
    int numSegments;         /* number of segments of filter tail */
    int numFFTBins;          /* number of FFT bins (FFTSize / 2 + 1) */
    int numSamples;          /* mic, error, and loudspeaker frame size */
    int sampleRate;          /* sample rate */
} IppAECNLMSParam;

typedef	struct IppAECScaled32s_Def	{
	Ipp32s	val;
	Ipp32s	sf;
} IppAECScaled32s;

/* =============== Structure Definition for Noise Reduction ================ */
typedef struct MCRAState_Def IppMCRAState;


/* ===================Feature Compression / Vector Quantization ============ */

/* Codebook GetSize/Init */
IPPAPI(IppStatus, ippsCdbkGetSize_16s, 
    (int width, int step, int height, int cdbkSize, Ipp_Cdbk_Hint hint, 
    int* pSize))

IPPAPI(IppStatus, ippsCdbkInit_L2_16s, 
    (IppsCdbkState_16s* pCdbk, const Ipp16s* pSrc, int width, int step, 
    int height, int cdbkSize, Ipp_Cdbk_Hint hint))

/* Split VQ */
IPPAPI(IppStatus, ippsSplitVQ_16s16s, 
    (const Ipp16s* pSrc, int srcStep, Ipp16s* pDst, int dstStep, int height, 
    const IppsCdbkState_16s** pCdbks, int nStream))

/* Form VQ */
IPPAPI(IppStatus, ippsFormVectorVQ_16s16s, 
    (const Ipp16s* pSrc, int srcStep, Ipp16s* pDst, int dstStep, int height, 
    const IppsCdbkState_16s** pCdbks, int nStream))

/* ========================= Feature Extraction ============================ */

/* Offset Compensation */
IPPAPI(IppStatus, ippsCompensateOffsetQ15_16s, 
    (const Ipp16s*   pSrc, Ipp16s* pDst, int len, 
    Ipp16s* pSrcDstPrevInputSample, Ipp16s prevOutputSample, Ipp16s valQ15))

IPPAPI(IppStatus, ippsCompensateOffsetQ15_16s_I, 
    (Ipp16s*   pSrcDst, int len, Ipp16s* pSrcDstPrevInputSample, 
    Ipp16s prevOutputSample, Ipp16s valQ15))

/* Mel Frequency Cepstral Coefficients (MFCC) GetSize/Init */
IPPAPI(IppStatus, ippsMelFBankGetSize_32s, 
    (int winSize, int nFilter, IppMelMode mode, int* pSize))

IPPAPI(IppStatus, ippsMelFBankInit_32s, 
    (IppsFBankState_32s* pFBank, int* pFFTLen, int winSize, Ipp32s sampFreq, 
    Ipp32s lowFreq, Ipp32s highFreq, int nFilter, Ipp32s melMulQ15, 
    Ipp32s melDivQ15, IppMelMode mode))

/* Calculate MFCC feature */
IPPAPI(IppStatus, ippsEvalFBank_32s_Sfs, 
    (const Ipp32s* pSrc, Ipp32s*   pDst, const IppsFBankState_32s* pFBank, 
    int scaleFactor))

/* DCTLifter GetSize/Init */
IPPAPI(IppStatus, ippsDCTLifterGetSize_MulC0_16s, 
    (int lenDCT, int lenCeps, int* pSize))

IPPAPI(IppStatus, ippsDCTLifterInit_MulC0_16s, 
    (IppsDCTLifterState_16s* pDCTLifter, int lenDCT, const Ipp32s* pLifterQ15, 
    int lenCeps))

/* Perform DCT and Liftering */
IPPAPI(IppStatus, ippsDCTLifter_32s16s_Sfs, 
    (const Ipp32s* pSrc, Ipp16s* pDst, const IppsDCTLifterState_16s* pDCTLifter,
    int scaleFactor))

/* Sign Change Count*/
IPPAPI(IppStatus, ippsSignChangeRate_16s, 
    (const Ipp16s *pSrc, int len, Ipp32s *pDstResult))

/* =============== Noise Reduction ========================================= */

/* EMNS Filter Update */
IPPAPI(IppStatus, ippsFilterUpdateEMNS_32s, 
    (const Ipp32s *pSrcWienerCoefsQ31, const Ipp32s *pSrcPostSNRQ15, 
    Ipp32s *pDstFilterCoefsQ31, int len))

/* Wiener Filter Update */
IPPAPI(IppStatus, ippsFilterUpdateWiener_32s, 
    (const Ipp32s *pSrcPriorSNRQ15, Ipp32s *pDstFilterCoefsQ31, int len))

/* Noise Floor Estimation GetSize/Init */
IPPAPI(IppStatus, ippsGetSizeMCRA_32s, 
    (int nFFTSize, int *pDstSize))

IPPAPI(IppStatus, ippsInitMCRA_32s, 
    (int nSamplesPerSec, int nFFTSize, IppMCRAState *pDst))

    /* alternative init function:  not supported, use at your own peril */
IPPAPI(IppStatus, ippsAltInitMCRA_32s, 
    (int nSamplesPerSec, int nFFTSize, int nUpdateSamples, IppMCRAState *pDst))

/* Noise Floor Estimation */
IPPAPI(IppStatus, ippsUpdateNoisePSDMCRA_32s_I, 
    (const Ipp32s *pSrcNoisySpeech, IppMCRAState *pSrcDstState, 
    Ipp32s *pSrcDstNoisePSD))

/* =============== Acoustic Echo Cancellation ============================= */

/* Acoustic Echo Cancellation Filter */
IPPAPI(IppStatus, ippsFilterAECNLMS_32sc_Sfs, 
    (Ipp32sc **ppSrcSignalIn, Ipp32sc **ppSrcCoefs, Ipp32sc *pDstSignalOut, 
    int numSegments, int len, int scaleFactor))

/* Acoustic Echo Cancellation Filter Update */
IPPAPI(IppStatus, ippsCoefUpdateAECNLMS_32sc_I, 
    (const IppAECScaled32s *pSrcStepSize, const Ipp32sc **ppSrcFilterInput, 
    const Ipp32sc *pSrcError, Ipp32sc **ppSrcDstCoefsQ15, int numSegments, 
    int len, int scaleFactorCoef))

/* Step Size Update */
IPPAPI(IppStatus, ippsStepSizeUpdateAECNLMS_32s, 
    (Ipp32s *pSrcInputPSD, Ipp32s muQ31, IppAECScaled32s maxStepSize, 
    Ipp32s minInputPSD, IppAECScaled32s *pDstStepSize, int len, int scaleFactorPSD ))

/* AEC Controller GetSize/Init.*/
IPPAPI(IppStatus, ippsControllerGetSizeAEC_32s, 
    (int *pDstSize))

IPPAPI(IppStatus, ippsControllerInitAEC_32s, 
    (const IppAECNLMSParam *pSrcParams, IppAECCtrlState *pDstState))

/* AEC Controller .*/
IPPAPI(IppStatus, ippsControllerUpdateAEC_32s, 
    (const IppAECNLMSParam *pSrcParams, IppAECCtrlState *pSrcDstState, 
    Ipp32s *pDstMuQ31, Ipp32s *pDstAECOutGainQ30, Ipp32s *pDstSpeakerGainQ30))


/* =============== Voice Activity Detection =============================== */

/* Find Peaks */
IPPAPI(IppStatus, ippsFindPeaks_32s8u, 
    (const Ipp32s *pSrc, Ipp8u *pDstPeaks, int len, int searchSize, 
    int movingAvgSize))

/* Periodicity */
IPPAPI(IppStatus, ippsPeriodicityLSPE_16s, 
    (const Ipp16s *pSrc, int len, Ipp16s *periodicityQ15, int *period,
    int maxPeriod, int minPeriod))

IPPAPI(IppStatus, ippsPeriodicity_32s16s, 
    (const Ipp32s *pSrc, int len, Ipp16s *periodicityQ15, int *period,
    int maxPeriod, int minPeriod) )


#ifdef __cplusplus
}
#endif

#endif  /* #ifndef __IPPAUDIO_H__ */

/* EOF */
