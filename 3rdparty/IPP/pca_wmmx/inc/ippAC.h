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
 
#ifndef _IPPAC_H_
#define _IPPAC_H_

#include "ippdefs.h"    

#ifdef __cplusplus
extern "C" {
#endif

/**************************** include information for MP3 decoder ********************************/
/***** Data Structures and Macro ***********************************************/
/* Macro  */
#define IPP_MP3_GRANULE_LEN         576
#define IPP_MP3_SF_BUF_LEN          40  /* scalefactor buffer length */
#define IPP_MP3_V_BUF_LEN           512 /* V data buffer length */

/* scalefactor band table length for 3 sampling frequencies for long/short block  */
#define IPP_MP3_SFB_TABLE_LONG_LEN	138 
#define IPP_MP3_SFB_TABLE_SHORT_LEN	84
#define IPP_MP3_MBP_TABLE_LEN 12

typedef const Ipp16s IppMP3ScaleFactorBandTableLong[IPP_MP3_SFB_TABLE_LONG_LEN]; /* 138 elements */
typedef const Ipp16s IppMP3ScaleFactorBandTableShort[IPP_MP3_SFB_TABLE_SHORT_LEN]; /* 84 elements */
typedef const Ipp16s IppMP3MixedBlockPartitionTable[IPP_MP3_MBP_TABLE_LEN];

/* default scalefactor band table for long/short block,
// _ipp_pMP3SfbTableLong,  _ipp_pMP3SfbTableShort are built in tables
*/
#define IPP_MP3_DEFAULT_SFB_TABLE_LONG   	( (Ipp16s *) _ipp_pMP3SfbTableLong )
#define IPP_MP3_DEFAULT_SFB_TABLE_SHORT  	( (Ipp16s *) _ipp_pMP3SfbTableShort )



/* Data Structures */
/* MPEG -1, -2 BC header, 32 bits. See ISO/IEC 11172-3, sect 2.4.1.3, 2.4.2.3, 2.4.2.4 */
typedef struct {
    int id;                     /* ID 1: MPEG-1, 0: MPEG-2 */
    int layer;                  /* layer index 0x3: Layer I  
                                //             0x2: Layer II
                                //             0x1: Layer III */
    int protectionBit;          /* CRC flag 0: CRC on, 1: CRC off */
    int bitRate;                /* bit rate index */
    int samplingFreq;           /* sampling frequency index */
    int paddingBit;             /* padding flag 0: no padding, 1 padding  */
    int privateBit;             /* private_bit, no use  */
    int mode;                   /* mono/stereo select information */
    int modeExt;                /* extension to mode */
    int copyright;              /* copyright or not, 0: no, 1: yes  */
    int originalCopy;           /* original bitstream or copy, 0: copy, 1: original */
    int emphasis;               /* flag indicates the type of de-emphasis that shall be used */
    int CRCWord;                /* CRC-check word */

} IppMP3FrameHeader;


/* MP3 side informatin structure , for each granule. Other info main_data_begin, 
// private_bits, scfsi are not included here.
// please refer to reference ISO/IEC 11172-3:1993, 2.4.1.7, 2.4.2.7. ISO/IEC 
// 13818-3:1998, 2.4.1.7 ).
*/
typedef struct {
    int  part23Len;             /* the number of bits for scale factors and Huffman data */ 
    int  bigVals;               /* the half number of Huffman data whose maximum 
                                // amplitudes are greater than 1 
                                */
    int  globGain;              /* the quantizer step size information */
    int  sfCompress;            /* information to select the number of bits used for 
                                // the transmission of the scale factors 
                                */
    int  winSwitch;             /* flag signals that the block uses an other than
                                //   normal window 
                                */
    int  blockType;             /* flag indicates the window type */
    int  mixedBlock;            /* flag 0: non mixed block, 1: mixed block */
    int  pTableSelect[3];       /* Huffman table index for the 3 regions in big-values field */
    int  pSubBlkGain[3];        /* gain offset from the global gain for one subblock */
    int  reg0Cnt;               /* the number of scale factor bands at the boundary 
                                // of the first region of the big-values field 
                                */

    int  reg1Cnt;               /* similar to reg0Cnt, but of the second region */
    int  preFlag;               /* flag of high frequency amplification */
    int  sfScale;               /* scale to the scale factors */
    int  cnt1TabSel;            /* Huffman table index for the count1 region of quadruples */
} IppMP3SideInfo;

/***** MP3 Functions *****/

/* Frame Unpacking */
IPPAPI(IppStatus, ippsUnpackFrameHeader_MP3, 
       (Ipp8u **ppBitStream, IppMP3FrameHeader *pFrameHeader) )

IPPAPI(IppStatus, ippsUnpackSideInfo_MP3, 
        (Ipp8u **ppBitStream, IppMP3SideInfo *pDstSideInfo, int *pDstMainDataBegin, 
         int *pDstPrivateBits, int *pDstScfsi, IppMP3FrameHeader *pFrameHeader) )

IPPAPI(IppStatus, ippsUnpackScaleFactors_MP3_1u8s, 
       (Ipp8u **ppBitStream, int *pOffset, Ipp8s *pDstScaleFactor, 
        IppMP3SideInfo *pSideInfo, int *pScfsi,   
        IppMP3FrameHeader *pFrameHeader, int granule, int channel) )

/* Huffman Decoding */
IPPAPI(IppStatus, ippsHuffmanDecode_MP3_1u32s, 
       (Ipp8u **ppBitStream, int *pOffset, Ipp32s *pDstIs, int *pDstNonZeroBound, 
        IppMP3SideInfo *pSideInfo, IppMP3FrameHeader *pFrameHeader, int hufSize) )
IPPAPI(IppStatus, ippsHuffmanDecodeSfb_MP3_1u32s,
	   (Ipp8u **ppBitStream, int *pOffset, Ipp32s *pDstIs, int *pDstNonZeroBound, 
		IppMP3SideInfo *pSideInfo, IppMP3FrameHeader *pFrameHeader, int hufSize,
		IppMP3ScaleFactorBandTableLong pSfbTableLong) ) 

IPPAPI(IppStatus, ippsHuffmanDecodeSfbMbp_MP3_1u32s,
	   (Ipp8u **ppBitStream, int *pOffset, Ipp32s *pDstIs, int *pDstNonZeroBound, 
	    IppMP3SideInfo *pSideInfo, IppMP3FrameHeader *pFrameHeader, int hufSize,
		IppMP3ScaleFactorBandTableLong pSfbTableLong,
		IppMP3ScaleFactorBandTableShort pSfbTableShort,
		IppMP3MixedBlockPartitionTable pMbpTable))

/* Requantization */
IPPAPI(IppStatus, ippsReQuantize_MP3_32s_I, 
       (Ipp32s *pSrcDstIsXr, int *pNonZeroBound, Ipp8s *pScaleFactor, 
        IppMP3SideInfo *pSideInfo, IppMP3FrameHeader *pFrameHeader, Ipp32s *pBuffer) )

IPPAPI(IppStatus, ippsReQuantizeSfb_MP3_32s_I,
	   (Ipp32s *pSrcDstIsXr, int *pNonZeroBound, Ipp8s *pScaleFactor, 
		IppMP3SideInfo *pSideInfo, IppMP3FrameHeader *pFrameHeader, Ipp32s *pBuffer, 
 		IppMP3ScaleFactorBandTableLong pSfbTableLong, IppMP3ScaleFactorBandTableShort pSfbTableShort) ) 

/* Hybrid Filtering */
IPPAPI(IppStatus, ippsMDCTInv_MP3_32s, 
       (Ipp32s *pSrcXr, Ipp32s *pDstY, Ipp32s *pSrcDstOverlapAdd, 
        int nonZeroBound, int *pPrevNumOfImdct, int blockType, int mixedBlock) )

/* Polyphase Filtering */
IPPAPI(IppStatus, ippsSynthPQMF_MP3_32s16s, 
       (Ipp32s *pSrcY, Ipp16s *pDstAudioOut, Ipp32s *pVBuffer, int *pVPosition, int mode) )

	   
/**************************** include information for MP3 encoder ********************************/
#define IPP_MP3_X_BUF_LEN				512
#define IPP_MP3_CHANNEL_NUM				2
#define IPP_MP3_PSY_BAND_LONG_NUM		63
#define IPP_MP3_PSY_BAND_SHORT_NUM		42
#define FIRST_6_CW						6

/***** Data Structures ***********************************************/
typedef struct {

	// partition threshold of previous two granules for long blocks
	Ipp64s	   pPrevMaskedThresholdLong[2][63];
	// partition threshold of previous short block 
	Ipp64s	   pPrevMaskedThresholdShort[IPP_MP3_PSY_BAND_SHORT_NUM];
	// first 6 FFT coefficients and magnitudes of two previous long blocks
	Ipp32sc	   pPrevFFT[2][FIRST_6_CW];
	Ipp32s	   pPrevFFTMag[2][6];

	// PE for the first granule in the next frame
	int		   nextPerceptualEntropy;	
	// block type for the first granule in the next frame
	int		   nextBlockType;									
	// Mask to Signal Ratio for long block for the next frame
	Ipp32s     pNextMSRLong[IPP_MP3_SF_BUF_LEN];   
	// Mask to Signal Ratio for short block
	Ipp32s     pNextMSRShort[IPP_MP3_SF_BUF_LEN]; 
} IppMP3EncPsychoAcousticModel2State;

typedef struct {
	Ipp32s pPsyMSR[36];
	Ipp32s PE;
} IppMP3PsychoacousticModelTwoAnalysis;


typedef struct {
    int BitsRemaining;		/* bits currently remaining in the reservoir */
	int MaxBits;			/* maximum possible reservoir size, in bits, 
 							determined as follows:  min(7680-avg_frame_len,
							2^9*8), where  avg_frame_len is the average frame 
							length, in bits, including padding bits and
							excluding side information bits */ 
} IppMP3BitReservoir;

/***** Audio Coding Functions ************************************************/

/***** MP3 Encoder Functions *****/
/* Bit reservior initialization function */
IPPAPI(IppStatus, ippsBitReservoirInit_MP3, (IppMP3BitReservoir *pDstBitResv, 
											IppMP3FrameHeader *pFrameHeader))

/* Frame Packing */
IPPAPI(IppStatus, ippsPackFrameHeader_MP3,
	    (IppMP3FrameHeader *pSrcFrameHeader, Ipp8u **ppBitStream) )

IPPAPI(IppStatus, ippsPackSideInfo_MP3, (IppMP3SideInfo *pSrcSideInfo, 
		Ipp8u **ppBitStream, int mainDataBegin, int privateBits, int *pSrcScfsi, 
		IppMP3FrameHeader *pFrameHeader) )

IPPAPI(IppStatus, ippsPackScaleFactors_MP3_8s1u, (Ipp8s *pSrcScaleFactor, 
	   Ipp8u **ppBitStream, int *pOffset, IppMP3FrameHeader *pFrameHeader,	
	   IppMP3SideInfo *pSideInfo,  int *pScfsi, int granule, int channel))

/* Huffman Encoding */

IPPAPI(IppStatus, ippsHuffmanEncode_MP3_32s1u, (Ipp32s *pSrcIx,Ipp8u **ppBitStream, 
				int *pOffset, IppMP3FrameHeader *pFrameHeader, IppMP3SideInfo *pSideInfo, 
				int count1Len, int hufSize))

/* Psychoacoustic Model 2 */
IPPAPI(IppStatus, ippsPsychoacousticModelTwo_MP3_16s, 
	   (const Ipp16s *pSrcPcm, IppMP3PsychoacousticModelTwoAnalysis 
	    *pDstPsychoacousticModelOutput, int *pDstIsSfbBound, 
		IppMP3SideInfo *pDstSideInfo, IppMP3FrameHeader *pFrameHeader, 
		IppMP3EncPsychoAcousticModel2State *pPsychoacousticModelState,
		int pcmMode, Ipp32s *pWorkBuffer))

/* stereo encoder */
IPPAPI(IppStatus, ippsJointStereoEncode_MP3_32s_I, (Ipp32s *pSrcDstXrL, 
	   Ipp32s *pSrcDstXrR, Ipp8s *pDstScaleFactorR, IppMP3FrameHeader *pFrameHeader, 
	   IppMP3SideInfo *pSideInfo, int *pIsSfbBound))


/* Quantizer */
IPPAPI(IppStatus, ippsQuantize_MP3_32s_I, (Ipp32s *pSrcDstXrIx,  
		Ipp8s *pDstScaleFactor, int *pDstScfsi, int *pDstCount1Len, 
		int *pDstHufSize, IppMP3FrameHeader *pFrameHeader, 
		IppMP3SideInfo *pSideInfo, 
		IppMP3PsychoacousticModelTwoAnalysis *pPsychoacousticModelOutput, 
		IppMP3EncPsychoAcousticModel2State *pPsychoacousticModelState,
		IppMP3BitReservoir *pResv, int meanBits,  int *pIsSfbBound, 
		Ipp32s *pWorkBuffer))



/* Analysis Filterbank*/
IPPAPI(IppStatus, ippsAnalysisPQMF_MP3_16s32s, (const Ipp16s *pSrcPcm, Ipp32s *pDstXs, 
	    int pcmMode))
IPPAPI(IppStatus, ippsMDCTFwd_MP3_32s,(const Ipp32s *pSrcXs, Ipp32s *pDstXr, int blockType, 
				int mixedBlock, IppMP3FrameHeader *pFrameHeader, Ipp32s *pOverlapBuf))

/**************************** include information for AAC decoder ********************************/
/***** Data Structures and Macro ***********************************************/
/* micro */

#define IPP_AAC_FRAME_LEN		1024		/* frame length */
#define IPP_AAC_SF_LEN			120		/* scalefactor buffer length */
#define IPP_AAC_TNS_COEF_LEN		60		/* TNS coefficients buffer length */
#define IPP_AAC_SF_MAX				60		/* maximum number of sfb in one window */
#define IPP_AAC_GROUP_NUM_MAX		8		/* maximum groups number for one frame */
#define IPP_AAC_TNS_FILT_MAX		8		/* maximum TNS filters number for one frame */
#define IPP_AAC_PRED_SFB_MAX		41		/* maximum prediction scalefactor bands number for one frame */

#define IPP_AAC_ELT_NUM			16		/* maximum elements number for one program */
#define	IPP_AAC_LFE_ELT_NUM		(1<<2)		/* maximum Low Frequency Enhance elements number for one program */ 
#define	IPP_AAC_DATA_ELT_NUM		(1<<3)		/* maximum data elements number for one program */ 
#define	IPP_AAC_COMMENTS_LEN		(1<<8)		/* maximum length of the comments field in bytes */ 

/* Data Structures */
/* MPEG-2 AAC ADTS frame header See ISO/IEC 13838-7, Table 6.5 */
typedef struct {
	/* ADTS fixed header */
    int id;                     /* ID 1*/
    int layer;                  /* layer index 0x3: Layer I  
                                //             0x2: Layer II
                                //             0x1: Layer III */
    int protectionBit;          /* CRC flag 0: CRC on, 1: CRC off */
    int profile;                /* profile: 0:MP, 1:LP, 2:SSR */
    int samplingRateIndex;      /* sampling frequency index */
    int privateBit;             /* private_bit, no use  */
    int chConfig;               /* channel configuration */
    int originalCopy;           /* original bitstream or copy, 0: copy, 1: original */
    int home;
	int emphasis;				/* not used by ISO/IEC 13818-7, but used by 14490-3 */
	
	/* ADTS variable header */
	int cpRightIdBit;			/* copyright id bit */
	int cpRightIdStart;			/* copyright id start */
	int frameLen;				/* frame length in bytes */
	int ADTSBufFullness;		/* buffer fullness */
	int numRawBlock;			/* number of raw data blocks in the frame */
	
	/* ADTS CRC error check, 16bits */
	int CRCWord;                /* CRC-check word */
} IppAACADTSFrameHeader;

							
typedef struct {
    Ipp32u	ADIFId;								/* 32-bit, "ADIF" ASCII code */
    int	    copyIdPres;							/* copy id flag: 0: off, 1: on */ 
    int	    originalCopy;						/* original bitstream or copy, 0: copy, 1: original */
    int	    home;
    int	    bitstreamType;						/* bitstream flag: 0: constant rate bitstream, 1: varible rate bitstream */
    int     bitRate;							/* bit rate. if 0, unkown bit rate */
    int	    numPrgCfgElt;						/* number of program config elements */
    int	    pADIFBufFullness[IPP_AAC_ELT_NUM];	/* buffer fullness */
	Ipp8u   pCopyId[9];							/* 72-bit copy id */
} IppAACADIFHeader;


typedef struct {
	int		eltInsTag;						/* element instanse tag */
	int		profile;						/* profile index. 0: main profile, 1: Low Complexity profile,
											//				  2: Scaleable Sampling Rate profile, 3: reserved
											*/
	int		samplingRateIndex;				/* sampling rate index */				
	int		numFrontElt;					/* number of front elements */
	int		numSideElt;						/* number of side elements  */
	int		numBackElt;						/* number of back elements  */
	int		numLfeElt;						/* number of LFE elements   */
	int		numDataElt;						/* number of data elements   */	
	int		numValidCcElt;					/* number of coupling channel elements */
	int		monoMixdownPres;				/* mono mixdown flag. 0: off, 1: on */
	int		monoMixdownEltNum;				/* number of mono mixdown elements */
	
	int		stereoMixdownPres;				/* stereo mixdown flag. 0: off, 1: on */
	int		stereoMixdownEltNum;			/* number of stereo mixdown elements */
	
	int		matrixMixdownIdxPres;			/* matrix mixdown index flag. 0: off, 1: on */
	int		matrixMixdownIdx;				/* index of the surround mixdown coefficients */
	int		pseudoSurroundEnable;			/* pseudo surround flag. 0: off, 1: on */
	
	int		pFrontIsCpe[IPP_AAC_ELT_NUM];	/* channel pair flag for front elements */
	int		pFrontTagSel[IPP_AAC_ELT_NUM];	/* instance tag for front elements */

	int		pSideIsCpe[IPP_AAC_ELT_NUM];    /* channel pair flag for side elements */
	int		pSideTagSel[IPP_AAC_ELT_NUM];	/* instance tag for side elements */

	int		pBackIsCpe[IPP_AAC_ELT_NUM];    /* channel pair flag for back elements */
	int		pBackTagSel[IPP_AAC_ELT_NUM];	/* instance tag for back elements */

	int		pLfeTagSel[IPP_AAC_LFE_ELT_NUM];/* channel pair flag for LFE elements */
	int		pDataTagSel[IPP_AAC_DATA_ELT_NUM];/* instance tag for data elements */
	
	int		pCceIsIndSw[IPP_AAC_ELT_NUM];	/* independent flag for coupling channel elements */
	int		pCceTagSel[IPP_AAC_ELT_NUM];	/* instance tag for coupling channel elements */

	int		numComBytes;					/* number of comment field bytes */
	Ipp8s	pComFieldData[IPP_AAC_COMMENTS_LEN]; /* the buffer of comment field */
} IppAACPrgCfgElt;


typedef struct {
	/* unpacked from the bitstream */
	int		icsReservedBit;	
	int		winSequence;						/* window sequence flag */
	int		winShape;							/* window shape flag, 0: sine window, 1: KBD window */
	int		maxSfb;								/* maximum effective scalefactor bands */ 
	int		sfGrouping;							/* scalefactor grouping flag */
	int		predDataPres;						/* prediction data present flag for one fraem, 0: prediction off, 1: prediction on */
	int		predReset;							/* prediction reset flag, 0: reset off, 1: reset on */
	int		predResetGroupNum;					/* prediction reset group number */
	Ipp8u	pPredUsed[IPP_AAC_PRED_SFB_MAX+3];	/* prediction flag buffer for each scalefactor band: 0: off, 1: on
												// buffer length 44 bytes, 4-byte align
												*/
	/* decoded from the above info */
	int		numWinGrp;							/* number of window_groups */
	int		pWinGrpLen[IPP_AAC_GROUP_NUM_MAX];	/* buffer for number of windows in each group */
} IppAACIcsInfo;

typedef struct {
	int		commonWin;							/* common window flag, 0: off, 1: on */
	int		msMaskPres;	
	Ipp8u	ppMsMask[IPP_AAC_GROUP_NUM_MAX][IPP_AAC_SF_MAX];/* MS stereo mask present flag */ 	
} IppAACChanPairElt;

typedef struct {
	int		tag;
	int		id;			/* element id */
	
	int		samplingRateIndex;  
	int		predSfbMax;		/* maximum prediction scalefactor bands */
	int		preWinShape;	/* previous block window shape */
	
	int		winLen;		/* 128: if short window, 1024: others */
	int		numWin;		/* 1 for long block, 8 for short block */
	int		numSwb;		/* decided by sampling rate and block type */
	
	/* unpacking from the bitstream */
	int		globGain;				/* global gain */
	int		pulseDataPres;			/* pulse data present flag, 0: off, 1: on */
	int		tnsDataPres;			/* TNS data present flag, 0: off, 1: on */
	int		gainContrDataPres;		/* gain control data present flag, 0: off, 1: on */
	
	/* Ics Info pointer */
	IppAACIcsInfo *pIcsInfo;		/* pointer to IppAACIcsInfo structure */

	/* channel pair element pointer */
	IppAACChanPairElt *pChanPairElt;	/* pointer to IppAACChanPairElt structure */

	/* section data */
	Ipp8u pSectCb[IPP_AAC_SF_LEN];		/* section code book buffer */
	Ipp8u pSectEnd[IPP_AAC_SF_LEN];		/* at which scalefactor band each section ends */ 
    int pMaxSect[IPP_AAC_GROUP_NUM_MAX];/* maximum section number for each group */

	/* TNS data */
	int pTnsNumFilt[IPP_AAC_GROUP_NUM_MAX];		/* TNS number filter buffer */
	int pTnsFiltCoefRes[IPP_AAC_GROUP_NUM_MAX]; /* TNS coefficients resolution flag */
	int pTnsRegionLen[IPP_AAC_TNS_FILT_MAX];	/* TNS filter length */
	int pTnsFiltOrder[IPP_AAC_TNS_FILT_MAX];	/* TNS filter order */
	int pTnsDirection[IPP_AAC_TNS_FILT_MAX];	/* TNS filter direction flag */
}IppAACChanInfo;

/***** AAC Functions *****/
IPPAPI(IppStatus, ippsUnpackADIFHeader_AAC, 
	   (Ipp8u **ppBitStream, IppAACADIFHeader *pADIFHeader, IppAACPrgCfgElt *pPrgCfgElt, int prgCfgEltMax))

IPPAPI(IppStatus, ippsUnpackADTSFrameHeader_AAC, 
	   (Ipp8u **ppBitStream, IppAACADTSFrameHeader *pADTSFrameHeader))

IPPAPI(IppStatus, ippsDecodePrgCfgElt_AAC, 
	   (Ipp8u **ppBitStream, int *pOffset, IppAACPrgCfgElt *pPrgCfgElt))

IPPAPI(IppStatus, ippsDecodeChanPairElt_AAC, 
	(Ipp8u **ppBitStream, int *pOffset,	IppAACIcsInfo *pIcsInfo, IppAACChanPairElt *pChanPairElt, int predSfbMax) )

IPPAPI(IppStatus, ippsNoiselessDecoder_LC_AAC,
	(Ipp8u **ppBitStream, int *pOffset, int commonWin, IppAACChanInfo *pChanInfo, 
	 Ipp16s *pDstScalefactor, Ipp32s *pDstQuantizedSpectralCoef, Ipp8u *pDstSfbCb, Ipp8s *pDstTnsFiltCoef) )

IPPAPI(IppStatus, ippsDecodeDatStrElt_AAC,
	   ( Ipp8u **ppBitStream, int *pOffset, int *pDataTag, int *pDataCnt, Ipp8u *pDstDataElt))

IPPAPI(IppStatus, ippsDecodeFillElt_AAC, 
	   ( Ipp8u **ppBitStream, int *pOffset, int *pFillCnt, Ipp8u *pDstFillElt))

IPPAPI(IppStatus, ippsQuantInv_AAC_32s_I, 
	(Ipp32s *pSrcDstSpectralCoef, const Ipp16s *pScalefactor, int numWinGrp, 
	 const int *pWinGrpLen, int maxSfb, const Ipp8u *pSfbCb, int samplingRateIndex, int winLen) )

IPPAPI(IppStatus, ippsDecodeMsStereo_AAC_32s_I, 
	(Ipp32s *pSrcDstL, Ipp32s *pSrcDstR, int msMaskPres,
	const Ipp8u (*ppMsUsed)[IPP_AAC_SF_MAX], Ipp8u *pSfbCb,
	int numWinGrp, const int *pWinGrpLen, int maxSfb, int samplingRateIndex,
	int winLen) )

IPPAPI(IppStatus, ippsDecodeIsStereo_AAC_32s, 
	 (const Ipp32s *pSrcL, Ipp32s *pDstR, const Ipp16s *pScalefactor, const Ipp8u *pSfbCb,
	  int numWinGrp, const int *pWinGrpLen, int maxSfb, int samplingRateIndex, int winLen) )

IPPAPI(IppStatus, ippsDeinterleaveSpectrum_AAC_32s, 
	(const Ipp32s *pSrc, Ipp32s *pDst, int numWinGrp, 
	 const int *pWinGrpLen, int maxSfb, int samplingRateIndex, int winLen) )


IPPAPI(IppStatus, ippsDecodeTNS_AAC_32s_I, 
	(Ipp32s *pSrcDstSpectralCoefs, const int *pTnsNumFilt,	
	const int *pTnsRegionLen, const int *pTnsFiltOrder,	 
	const int *pTnsFiltCoefRes,	 const Ipp8s *pTnsFiltCoef,	
	const int *pTnsDirection, int maxSfb, int profile, 
	int samplingRateIndex, int winLen))


IPPAPI(IppStatus, ippsMDCTInv_AAC_32s16s,
	(Ipp32s *pSrcSpectralCoefs, Ipp16s *pDstPcmAudioOut, 
	Ipp32s *pSrcDstOverlapAddBuf, int winSequence, int winShape, 
	int prevWinShape, int pcmMode))

/*********************** AAC Scaleable Object ******************************/
#define IPP_AAC_CHAN_NUM	2
#define APP_AAC_MAX_NUM_OBJECT 16
#define IPP_AAC_WIN_MAX	8
#define IPP_AAC_MAX_LTP_SFB 40
#define IPP_LTP_BLOCK_LEN 1024*3
typedef struct {
	int tnsDataPresent;
	int pTnsNumFilt[IPP_AAC_GROUP_NUM_MAX];		/* Number of TNS filter */
	int pTnsFiltCoefRes[IPP_AAC_GROUP_NUM_MAX];	/* TNS coefficient resolution */
	int pTnsRegionLen[IPP_AAC_TNS_FILT_MAX];	/* TNS filter length */
	int pTnsFiltOrder[IPP_AAC_TNS_FILT_MAX];	/* TNS filter order */
	int pTnsDirection[IPP_AAC_TNS_FILT_MAX];	/* TNS filter direction flag */
	int pTnsCoefCompress[IPP_AAC_GROUP_NUM_MAX];
	Ipp8s pTnsFiltCoef[IPP_AAC_TNS_COEF_LEN];	/* TNS filter coefficients */
} IppAACTnsInfo ;

typedef struct{
	int ltpDataPresent;	/*if ltp is used */
	int ltpLag;				/*the optimal delay from 0 to 2047 */
	Ipp16s ltpCoef;		/*indicate the  LTP coefficient */
	int pLtpLongUsed[IPP_AAC_MAX_LTP_SFB+1]; /* if long block use ltp*/
	int pLtpShortUsed[IPP_AAC_WIN_MAX];/*if short block use ltp */
	int pLtpShortLagPresent[IPP_AAC_WIN_MAX]; /*if short lag is transmitted */
	int pLtpShortLag[IPP_AAC_WIN_MAX]; /*relative delay for short window */
}IppAACLtpInfo;

typedef struct {
	int windowSequence;
	int windowShape;
	int maxSfb;
	int sfGrouping;
	int numWinGrp;
	int pWinGrpLen[IPP_AAC_GROUP_NUM_MAX];
	int msMode;				/* MS stereo flag: 0 - none, 
							1 - different for every sfb, 2 - all */
	Ipp8u (*ppMsMask)[IPP_AAC_SF_MAX]; /* if MS is used in one Sfb */
	IppAACTnsInfo pTnsInfo[IPP_AAC_CHAN_NUM];
	IppAACLtpInfo pLtpInfo[IPP_AAC_CHAN_NUM];
} IppAACMainHeader;

typedef struct{
	int msMode;
	int maxSfb;	/* number of scale factor band for extension layer*/
	Ipp8u (*ppMsMask)[IPP_AAC_SF_MAX]; /* if MS is used in one Sfb */
	IppAACTnsInfo pTnsInfo[IPP_AAC_CHAN_NUM]; 		/* TNS structure for Stereo */
	int pDiffControlLr[IPP_AAC_CHAN_NUM][IPP_AAC_PRED_SFB_MAX]; /*FSS information for stereo*/
}IppAACExtHeader;

IPPAPI(IppStatus, ippsDecodeMainHeader_AAC,(Ipp8u **ppBitStream, int *pOffset, \
		IppAACMainHeader *pAACMainHeader, int channelNum, int monoStereoFlag))

IPPAPI(IppStatus, ippsDecodeExtensionHeader_AAC,(Ipp8u **ppBitStream, int *pOffset, \
		IppAACExtHeader *pAACExtHeader, int monoStereoFlag, int thisLayerStereo, \
		int monoLayerFlag, int preStereoMaxSfb, int hightstMonoMaxSfb, int winSequence))
		
IPPAPI(IppStatus, ippsDecodePNS_AAC_32s,(Ipp32s *pSrcDstSpec, int *pSrcDstLtpFlag, \
		Ipp8u *pSfbCb, Ipp16s *pScaleFactor, int maxSfb, int numWinGrp, \
		int *pWinGrpLen, int samplingFreqIndex, int winLen,	int *pRandomSeed))

IPPAPI(IppStatus, ippsDecodeMsPNS_AAC_32s,(Ipp32s *pSrcDstSpec, int *pSrcDstLtpFlag, \
		Ipp8u *pSfbCb, Ipp16s *pScaleFactor, int maxSfb, int numWinGrp, \
		int *pWinGrpLen, int samplingFreqIndex, int winLen,	int *pRandomSeed,\
		int channel, Ipp8u *pMsUsed, int *pNoiseState))

IPPAPI(IppStatus, ippsLongTermReconstruct_AAC_32s,(Ipp32s *pSrcEstSpec, Ipp32s *pSrcDstSpec, \
	   int *pLtpFlag, int winSequence, int samplingFreqIndex))
		
IPPAPI(IppStatus, ippsMDCTFwd_AAC_32s,(Ipp32s *pSrc, Ipp32s *pDst,Ipp32s *pOverlapAdd,\
		int winSequence, int winShape, int preWinShape, Ipp32s *pWindowedBuf))
		
IPPAPI(IppStatus, ippsEncodeTNS_AAC_32s_I,(Ipp32s *pSrcDstSpectralCoefs, 
	    const int *pTnsNumFilt,	const int *pTnsRegionLen,	 
		const int *pTnsFiltOrder, const int *pTnsFiltCoefRes, 
		const Ipp8s *pTnsFiltCoef, const int *pTnsDirection, int maxSfb,					 
		int profile, int samplingRateIndex,	int winLen))
		
IPPAPI(IppStatus, ippsLongTermPredict_AAC_32s,(Ipp32s *pSrcTimeSignal, Ipp32s *pDstEstTimeSignal, \
		IppAACLtpInfo *pAACLtpInfo, int winSequence))

IPPAPI(IppStatus, ippsNoiselessDecode_AAC,(Ipp8u **ppBitStream, int *pOffset,
	    IppAACMainHeader *pAACMainHeader, Ipp16s *pDstScalefactor, 
		Ipp32s *pDstQuantizedSpectralCoef, Ipp8u *pDstSfbCb, Ipp8s *pDstTnsFiltCoef,
		IppAACChanInfo *pChanInfo, int winSequence, int maxSfb, int commonWin,
		int scaleFlag, int audioObjectType))

IPPAPI(IppStatus, ippsLtpUpdate_AAC_32s,(Ipp32s *pSpecVal, Ipp32s *pLtpSaveBuf, int winSequence, \
	   int winShape, int preWinShape, Ipp32s *pWorkBuf))

IPPAPI(IppStatus, ippsDecodeChanPairElt_MPEG4_AAC,
	   (Ipp8u **ppBitStream, int *pOffset, IppAACIcsInfo *pIcsInfo, IppAACChanPairElt *pChanPairElt,
	   IppAACMainHeader *pAACMainHeader, int predSfbMax, int audioObjectType))

#ifdef __cplusplus
}
#endif

#endif /* end of #define _IPPAC_H_ */

/* EOF */

