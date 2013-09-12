#ifndef __IPPIP_H__
#define __IPPIP_H__
#ifdef __cplusplus 
extern "C" { 
#endif 
	
#if !defined( IPPAPI ) 
	#define   IPPAPI( type,name,arg )        extern type name arg; 
#endif 

typedef unsigned char   Ipp8u;

typedef struct { 
	int width; 
	int height; 
} IppiSize;	
/* ///////////////////////////////////////////////////////////////////////////// 
 //        The following enumerator defines a status of IPP operations 
 //                     negative value means error 
 */ 
typedef enum { 
	/* errors */ 
	ippStsNotSupportedModeErr   = -9999,  /* The requested mode is currently not supported  */ 
	ippStsCpuNotSupportedErr    = -9998,  /* The target cpu is not supported */ 
	
	ippStsNotPosDefErr           = -179,  /* Not positive-definite matrix */ 
	ippStsEphemeralKeyErr        = -178, /* ECC: Bad ephemeral key   */ 
	ippStsMessageErr	         = -177, /* ECC: Bad message digest  */ 
	ippStsShareKeyErr            = -176, /* ECC: Invalid share key   */ 
	ippStsIvalidPublicKey        = -175, /* ECC: Invalid public key  */ 
	ippStsIvalidPrivateKey       = -174, /* ECC: Invalid private key */ 
	ippStsOutOfECErr             = -173, /* ECC: Point out of EC     */ 
	ippStsECCInvalidFlagErr      = -172, /* ECC: Invalid Flag        */ 
	
	ippStsMP3FrameHeaderErr      = -171,  /* Error in fields IppMP3FrameHeader structure */ 
	ippStsMP3SideInfoErr         = -170,  /* Error in fields IppMP3SideInfo structure */ 
	
	ippStsBlockStepErr           = -169,  /* Step for Block less than 8 */ 
	ippStsMBStepErr              = -168,  /* Step for MB less than 16 */ 
	
	ippStsAacPrgNumErr           = -167,  /* AAC: Invalid number of elements for one program   */         
	ippStsAacSectCbErr           = -166,  /* AAC: Invalid section codebook                     */       
	ippStsAacSfValErr            = -164,  /* AAC: Invalid scalefactor value                    */       
	ippStsAacCoefValErr          = -163,  /* AAC: Invalid quantized coefficient value          */      
	ippStsAacMaxSfbErr           = -162,  /* AAC: Invalid coefficient index  */                          
	ippStsAacPredSfbErr          = -161,  /* AAC: Invalid predicted coefficient index  */                
	ippStsAacPlsDataErr          = -160,  /* AAC: Invalid pulse data attributes  */                      
	ippStsAacGainCtrErr          = -159,  /* AAC: Gain control not supported  */                         
	ippStsAacSectErr             = -158,  /* AAC: Invalid number of sections  */                         
	ippStsAacTnsNumFiltErr       = -157,  /* AAC: Invalid number of TNS filters  */                      
	ippStsAacTnsLenErr           = -156,  /* AAC: Invalid TNS region length  */                          
	ippStsAacTnsOrderErr         = -155,  /* AAC: Invalid order of TNS filter  */                        
	ippStsAacTnsCoefResErr       = -154,  /* AAC: Invalid bit-resolution for TNS filter coefficients  */ 
	ippStsAacTnsCoefErr          = -153,  /* AAC: Invalid TNS filter coefficients  */                    
	ippStsAacTnsDirectErr        = -152,  /* AAC: Invalid TNS filter direction  */                       
	ippStsAacTnsProfileErr       = -151,  /* AAC: Invalid TNS profile  */                                
	ippStsAacErr                 = -150,  /* AAC: Internal error  */                                     
	ippStsAacBitOffsetErr        = -149,  /* AAC: Invalid current bit offset in bitstream  */            
	ippStsAacAdtsSyncWordErr     = -148,  /* AAC: Invalid ADTS syncword  */                            
	ippStsAacSmplRateIdxErr      = -147,  /* AAC: Invalid sample rate index  */                         
	ippStsAacWinLenErr           = -146,  /* AAC: Invalid window length (not short or long)  */          
	ippStsAacWinGrpErr           = -145,  /* AAC: Invalid number of groups for current window length  */ 
	ippStsAacWinSeqErr           = -144,  /* AAC: Invalid window sequence range  */                      
	ippStsAacComWinErr           = -143,  /* AAC: Invalid common window flag  */                         
	ippStsAacStereoMaskErr       = -142,  /* AAC: Invalid stereo mask  */                                
	ippStsAacChanErr             = -141,  /* AAC: Invalid channel number  */                             
	ippStsAacMonoStereoErr       = -140,  /* AAC: Invalid mono-stereo flag  */                           
	ippStsAacStereoLayerErr      = -139,  /* AAC: Invalid this Stereo Layer flag  */                    
	ippStsAacMonoLayerErr        = -138,  /* AAC: Invalid this Mono Layer flag  */                       
	ippStsAacScalableErr         = -137,  /* AAC: Invalid scalable object flag  */                       
	ippStsAacObjTypeErr          = -136,  /* AAC: Invalid audio object type  */                          
	ippStsAacWinShapeErr         = -135,  /* AAC: Invalid window shape  */                               
	ippStsAacPcmModeErr          = -134,  /* AAC: Invalid PCM output interleaving indicator  */          
	ippStsVLCUsrTblHeaderErr          = -133,  /* VLC: Invalid header inside table */ 
	ippStsVLCUsrTblUnsupportedFmtErr  = -132,  /* VLC: Unsupported table format */ 
	ippStsVLCUsrTblEscAlgTypeErr      = -131,  /* VLC: Unsupported Ecs-algorithm */ 
	ippStsVLCUsrTblEscCodeLengthErr   = -130,  /* VLC: Incorrect Esc-code length inside table header */ 
	ippStsVLCUsrTblCodeLengthErr      = -129,  /* VLC: Unsupported code length inside table */ 
	ippStsVLCInternalTblErr           = -128,  /* VLC: Invalid internal table */ 
	ippStsVLCInputDataErr             = -127,  /* VLC: Invalid input data */ 
	ippStsVLCAACEscCodeLengthErr      = -126,  /* VLC: Invalid AAC-Esc code length */ 
	ippStsNoiseRangeErr         = -125,  /* Noise value for Wiener Filter is out range. */ 
	ippStsUnderRunErr           = -124,  /* Data under run error */ 
	ippStsPaddingErr            = -123,  /* Detected padding error shows the possible data corruption */ 
	ippStsCFBSizeErr            = -122,  /* Wrong value for crypto CFB block size */ 
	ippStsPaddingSchemeErr      = -121,  /* Invalid padding scheme  */ 
	ippStsInvalidCryptoKeyErr   = -120,  /* A compromised key causes suspansion of requested cryptographic operation  */ 
	ippStsLengthErr             = -119,  /* Wrong value of string length */ 
	ippStsBadModulusErr         = -118,  /* Bad modulus caused a module inversion failure */ 
	ippStsLPCCalcErr            = -117,  /* Linear prediction could not be evaluated */ 
	ippStsRCCalcErr             = -116,  /* Reflection coefficients could not be computed */ 
	ippStsIncorrectLSPErr       = -115,  /* Incorrect Linear Spectral Pair values */ 
	ippStsNoRootFoundErr        = -114,  /* No roots are found for equation */ 
	ippStsJPEG2KBadPassNumber   = -113,  /* Pass number exceeds allowed limits [0,nOfPasses-1] */ 
	ippStsJPEG2KDamagedCodeBlock= -112,  /* Codeblock for decoding is damaged */ 
	ippStsH263CBPYCodeErr       = -111,  /* Illegal Huffman code during CBPY stream processing */ 
	ippStsH263MCBPCInterCodeErr = -110,  /* Illegal Huffman code during MCBPC Inter stream processing */ 
	ippStsH263MCBPCIntraCodeErr = -109,  /* Illegal Huffman code during MCBPC Intra stream processing */ 
	ippStsNotEvenStepErr        = -108,  /* Step value is not pixel multiple */ 
	ippStsHistoNofLevelsErr     = -107,  /* Number of levels for histogram is less than 2 */ 
	ippStsLUTNofLevelsErr       = -106,  /* Number of levels for LUT is less than 2 */ 
	ippStsMP4BitOffsetErr       = -105,  /* Incorrect bit offset value */ 
	ippStsMP4QPErr              = -104,  /* Incorrect quantization parameter */ 
	ippStsMP4BlockIdxErr        = -103,  /* Incorrect block index */ 
	ippStsMP4BlockTypeErr       = -102,  /* Incorrect block type */ 
	ippStsMP4MVCodeErr          = -101,  /* Illegal Huffman code during MV stream processing */ 
	ippStsMP4VLCCodeErr         = -100,  /* Illegal Huffman code during VLC stream processing */ 
	ippStsMP4DCCodeErr          = -99,   /* Illegal code during DC stream processing */ 
	ippStsMP4FcodeErr           = -98,   /* Incorrect fcode value */ 
	ippStsMP4AlignErr           = -97,   /* Incorrect buffer alignment            */ 
	ippStsMP4TempDiffErr        = -96,   /* Incorrect temporal difference         */ 
	ippStsMP4BlockSizeErr       = -95,   /* Incorrect size of block or macroblock */ 
	ippStsMP4ZeroBABErr         = -94,   /* All BAB values are zero             */ 
	ippStsMP4PredDirErr         = -93,   /* Incorrect prediction direction        */ 
	ippStsMP4BitsPerPixelErr    = -92,   /* Incorrect number of bits per pixel    */ 
	ippStsMP4VideoCompModeErr   = -91,   /* Incorrect video component mode        */ 
	ippStsMP4LinearModeErr      = -90,   /* Incorrect DC linear mode */ 
	ippStsH263PredModeErr       = -83,   /* Prediction Mode value error                                       */ 
	ippStsH263BlockStepErr      = -82,   /* Step value is less than 8                                         */ 
	ippStsH263MBStepErr         = -81,   /* Step value is less than 16                                        */ 
	ippStsH263FrameWidthErr     = -80,   /* Frame width is less then 8                                        */ 
	ippStsH263FrameHeightErr    = -79,   /* Frame height is less than or equal to zero                        */ 
	ippStsH263ExpandPelsErr     = -78,   /* Expand pixels number is less than 8                               */ 
	ippStsH263PlaneStepErr      = -77,   /* Step value is less than the plane width                           */ 
	ippStsH263QuantErr          = -76,   /* Quantizer value is less than or equal to zero, or greater than 31 */ 
	ippStsH263MVCodeErr         = -75,   /* Illegal Huffman code during MV stream processing                  */ 
	ippStsH263VLCCodeErr        = -74,   /* Illegal Huffman code during VLC stream processing                 */ 
	ippStsH263DCCodeErr         = -73,   /* Illegal code during DC stream processing                          */ 
	ippStsH263ZigzagLenErr      = -72,   /* Zigzag compact length is more than 64                             */ 
	ippStsFBankFreqErr          = -71,   /* Incorrect value of the filter bank frequency parameter */ 
	ippStsFBankFlagErr          = -70,   /* Incorrect value of the filter bank parameter           */ 
	ippStsFBankErr              = -69,   /* Filter bank is not correctly initialized"              */ 
	ippStsNegOccErr             = -67,   /* Negative occupation count                      */ 
	ippStsCdbkFlagErr           = -66,   /* Incorrect value of the codebook flag parameter */ 
	ippStsSVDCnvgErr            = -65,   /* No convergence of SVD algorithm"               */ 
	ippStsJPEGHuffTableErr      = -64,   /* JPEG Huffman table is destroyed        */ 
	ippStsJPEGDCTRangeErr       = -63,   /* JPEG DCT coefficient is out of the range */ 
	ippStsJPEGOutOfBufErr       = -62,   /* Attempt to access out of the buffer    */ 
	ippStsDrawTextErr           = -61,   /* System error in the draw text operation */ 
	ippStsChannelOrderErr       = -60,   /* Wrong order of the destination channels */ 
	ippStsZeroMaskValuesErr     = -59,   /* All values of the mask are zero */ 
	ippStsQuadErr               = -58,   /* The quadrangle is nonconvex or degenerates into triangle, line or point */ 
	ippStsRectErr               = -57,   /* Size of the rectangle region is less than or equal to 1 */ 
	ippStsCoeffErr              = -56,   /* Unallowable values of the transformation coefficients   */ 
	ippStsNoiseValErr           = -55,   /* Bad value of noise amplitude for dithering"             */ 
	ippStsDitherLevelsErr       = -54,   /* Number of dithering levels is out of range"             */ 
	ippStsNumChannelsErr        = -53,   /* Bad or unsupported number of channels                   */ 
	ippStsCOIErr                = -52,   /* COI is out of range */ 
	ippStsDivisorErr            = -51,   /* Divisor is equal to zero, function is aborted */ 
	ippStsAlphaTypeErr          = -50,   /* Illegal type of image compositing operation                           */ 
	ippStsGammaRangeErr         = -49,   /* Gamma range bounds is less than or equal to zero                      */ 
	ippStsGrayCoefSumErr        = -48,   /* Sum of the conversion coefficients must be less than or equal to 1    */ 
	ippStsChannelErr            = -47,   /* Illegal channel number                                                */ 
	ippStsToneMagnErr           = -46,   /* Tone magnitude is less than or equal to zero                          */ 
	ippStsToneFreqErr           = -45,   /* Tone frequency is negative, or greater than or equal to 0.5           */ 
	ippStsTonePhaseErr          = -44,   /* Tone phase is negative, or greater than or equal to 2*PI              */ 
	ippStsTrnglMagnErr          = -43,   /* Triangle magnitude is less than or equal to zero                      */ 
	ippStsTrnglFreqErr          = -42,   /* Triangle frequency is negative, or greater than or equal to 0.5       */ 
	ippStsTrnglPhaseErr         = -41,   /* Triangle phase is negative, or greater than or equal to 2*PI          */ 
	ippStsTrnglAsymErr          = -40,   /* Triangle asymmetry is less than -PI, or greater than or equal to PI   */ 
	ippStsHugeWinErr            = -39,   /* Kaiser window is too huge                                             */ 
	ippStsJaehneErr             = -38,   /* Magnitude value is negative                                           */ 
	ippStsStrideErr             = -37,   /* Stride value is less than the row length */ 
	ippStsEpsValErr             = -36,   /* Negative epsilon value error"            */ 
	ippStsWtOffsetErr           = -35,   /* Invalid offset value of wavelet filter                                       */ 
	ippStsAnchorErr             = -34,   /* Anchor point is outside the mask                                             */ 
	ippStsMaskSizeErr           = -33,   /* Invalid mask size                                                           */ 
	ippStsShiftErr              = -32,   /* Shift value is less than zero                                                */ 
	ippStsSampleFactorErr       = -31,   /* Sampling factor is less than or equal to zero                                */ 
	ippStsSamplePhaseErr        = -30,   /* Phase value is out of range: 0 <= phase < factor                             */ 
	ippStsFIRMRFactorErr        = -29,   /* MR FIR sampling factor is less than or equal to zero                         */ 
	ippStsFIRMRPhaseErr         = -28,   /* MR FIR sampling phase is negative, or greater than or equal to the sampling factor */ 
	ippStsRelFreqErr            = -27,   /* Relative frequency value is out of range                                     */ 
	ippStsFIRLenErr             = -26,   /* Length of a FIR filter is less than or equal to zero                         */ 
	ippStsIIROrderErr           = -25,   /* Order of an IIR filter is less than or equal to zero                         */ 
	ippStsDlyLineIndexErr       = -24,   /* Invalid value of the delay line sample index */ 
	ippStsResizeFactorErr       = -23,   /* Resize factor(s) is less than or equal to zero */ 
	ippStsInterpolationErr      = -22,   /* Invalid interpolation mode */ 
	ippStsMirrorFlipErr         = -21,   /* Invalid flip mode                                         */ 
	ippStsMoment00ZeroErr       = -20,   /* Moment value M(0,0) is too small to continue calculations */ 
	ippStsThreshNegLevelErr     = -19,   /* Negative value of the level in the threshold operation    */ 
	ippStsThresholdErr          = -18,   /* Invalid threshold bounds */ 
	ippStsContextMatchErr       = -17,   /* Context parameter doesn't match the operation */ 
	ippStsFftFlagErr            = -16,   /* Invalid value of the FFT flag parameter */ 
	ippStsFftOrderErr           = -15,   /* Invalid value of the FFT order parameter */ 
	ippStsStepErr               = -14,   /* Step value is less than or equal to zero */ 
	ippStsScaleRangeErr         = -13,   /* Scale bounds are out of the range */ 
	ippStsDataTypeErr           = -12,   /* Bad or unsupported data type */ 
	ippStsOutOfRangeErr         = -11,   /* Argument is out of range or point is outside the image */ 
	ippStsDivByZeroErr          = -10,   /* An attempt to divide by zero */ 
	ippStsMemAllocErr           = -9,    /* Not enough memory allocated for the operation */ 
	ippStsNullPtrErr            = -8,    /* Null pointer error */ 
	ippStsRangeErr              = -7,    /* Bad values of bounds: the lower bound is greater than the upper bound */ 
	ippStsSizeErr               = -6,    /* Wrong value of data size */ 
	ippStsBadArgErr             = -5,    /* Function arg/param is bad */ 
	ippStsNoMemErr              = -4,    /* Not enough memory for the operation */ 
	ippStsSAReservedErr3        = -3,    /*  */ 
	ippStsErr                   = -2,    /* Unknown/unspecified error */ 
	ippStsSAReservedErr1        = -1,    /*  */ 
	/*  */ 
	/* no errors */                     /*  */ 
	ippStsNoErr                 =   0,   /* No error, it's OK */ 
	/*  */ 
	/* warnings */                      /*  */ 
	ippStsNoOperation       =   1,       /* No operation has been executed */ 
	ippStsMisalignedBuf     =   2,       /* Misaligned pointer in operation in which it must be aligned */ 
	ippStsSqrtNegArg        =   3,       /* Negative value(s) of the argument in the function Sqrt */ 
	ippStsInvZero           =   4,       /* INF result. Zero value was met by InvThresh with zero level */ 
	ippStsEvenMedianMaskSize=   5,       /* Even size of the Median Filter mask was replaced by the odd one */ 
	ippStsDivByZero         =   6,       /* Zero value(s) of the divisor in the function Div */ 
	ippStsLnZeroArg         =   7,       /* Zero value(s) of the argument in the function Ln     */ 
	ippStsLnNegArg          =   8,       /* Negative value(s) of the argument in the function Ln */ 
	ippStsNanArg            =   9,       /* Not a Number argument value warning                  */ 
	ippStsJPEGMarker        =   10,      /* JPEG marker was met in the bitstream                 */ 
	ippStsResFloor          =   11,      /* All result values are floored                        */ 
	ippStsOverflow          =   12,      /* Overflow occurred in the operation                   */ 
	ippStsLSFLow            =   13,      /* Quantized LP syntethis filter stability check is applied at the low boundary of [0,pi] */ 
	ippStsLSFHigh           =   14,      /* Quantized LP syntethis filter stability check is applied at the high boundary of [0,pi] */ 
	ippStsLSFLowAndHigh     =   15,      /* Quantized LP syntethis filter stability check is applied at both boundaries of [0,pi] */ 
	ippStsZeroOcc           =   16,      /* Zero occupation count */ 
	ippStsUnderflow         =   17,      /* Underflow occurred in the operation */ 
	ippStsSingularity       =   18,      /* Singularity occurred in the operation                                       */ 
	ippStsDomain            =   19,      /* Argument is out of the function domain                                      */ 
	ippStsNonIntelCpu       =   20,      /* The target cpu is not Genuine Intel                                         */ 
	ippStsCpuMismatch       =   21,      /* The library for given cpu cannot be set                                     */ 
	ippStsNoIppFunctionFound =  22,      /* Application does not contain IPP functions calls                            */ 
	ippStsDllNotFoundBestUsed = 23,      /* The newest version of IPP dll's not found by dispatcher                     */ 
	ippStsNoOperationInDll  =   24,      /* The function does nothing in the dynamic version of the library             */ 
	ippStsInsufficientEntropy=  25,      /* Insufficient entropy in the random seed and stimulus bit string caused the prime/key generation to fail */ 
	ippStsOvermuchStrings   =   26,      /* Number of destination strings is more than expected                         */ 
	ippStsOverlongString    =   27,      /* Length of one of the destination strings is more than expected              */ 
	ippStsAffineQuadChanged =   28,      /* 4th vertex of destination quad is not equal to customer's one               */                                
	ippStsWrongIntersectROI =   29,      /* Wrong ROI that has no intersection with the source or destination ROI. No operation */ 
	ippStsWrongIntersectQuad =  30,      /* Wrong quadrangle that has no intersection with the source or destination ROI. No operation */ 
	ippStsSmallerCodebook   =   31,      /* Size of created codebook is less than cdbkSize argument */ 
	ippStsAEMeetLimit		=	32,      /* Camera: The maximum/minimum AEC and AGC is used, but the captured image is still too dark/bright */ 
	ippStsAWBMeetLimit		=	33,      /* Camera: The maximum/minimum Gains is used, but the captured image is not balanced enough */ 
	ippStsAWBFailed			=	34       /* Camera: AWB failed. For WGA method, this means that all green pixels are dark; 
										  for WPD method, this means that not enough white point is found */ 
} IppStatus;   	
/*============ Intel IPP Camera Interface structure defination =============*/ 
typedef enum  
{ 
	ippCameraInterpNearest  	 = 0,    /* for nearest interpolation */ 
	ippCameraInterpBilinear      = 1,    /* for bilinear interpolation */ 
	ippCameraInterpMedian		 = 2,    /* for median interpolation */ 
	ippCameraInterpNearLinear    = 3    /* for NearLinear interpolation */ 
}IppCameraInterpolation; 

typedef enum 
{ 
	ippCameraRotateDisable       = 0, 
	ippCameraRotate90L           = 1,     
	ippCameraRotate90R           = 2, 
	ippCameraRotate180           = 3, 
	ippCameraFlipHorizontal		 = 4, 
	ippCameraFlipVertical		 = 5 
}IppCameraRotation; 

typedef enum 
{ 
	ippCameraCscYCbCr422ToRGB565 = 0, 
	ippCameraCscYCbCr422ToRGB555 = 1, 
	ippCameraCscYCbCr422ToRGB444 = 2, 
	ippCameraCscYCbCr422ToRGB888 = 3, 
	ippCameraCscYCbCr420ToRGB565 = 4, 
	ippCameraCscYCbCr420ToRGB555 = 5, 
	ippCameraCscYCbCr420ToRGB444 = 6, 
	ippCameraCscYCbCr420ToRGB888 = 7, 
	/* CFA format to YUV422/420 format */ 
	ippCameraCscRGGBToYCbCr422P  = 8, 
	ippCameraCscRGGBToYCbCr420   = 9, 	
	ippCameraCscYCbCr422ToRGB666 = 16, 
	ippCameraCscYCbCr420ToRGB666 = 17 
}IppCameraCsc;	

IPPAPI(IppStatus, ippiYCbCr420RszCscRotRGB_8u_P3C3R, 
		   (const Ipp8u *pSrc[3], int srcStep[3], IppiSize srcSize, void *pDst,  
			int dstStep, IppiSize dstSize, IppCameraCsc	colorConversion,  
			IppCameraInterpolation interpolation, IppCameraRotation rotation, 
			int rcpRatiox, int rcpRatioy)) 
	
#ifdef __cplusplus 
} 
#endif 	
#endif /* __IPPIP_H__ */