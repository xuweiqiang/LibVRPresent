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
#ifndef _IPPJP_H_
#define _IPPJP_H_

#include "ippdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*================== Intel IPP JPEG structure definiation ======================*/

/* ippJPEGENHuffTable */
typedef void IppiEncodeHuffmanSpec;

/* ippJPEGDEHuffTable */
typedef void IppiDecodeHuffmanSpec;

/* EncodeHuffmanState */
typedef void IppiEncodeHuffmanState;

/* DecodeHuffmanState */
typedef void IppiDecodeHuffmanState;

/*====================== Intel IPP JPEG API definiation ========================*/

/* 
//
//  Helper function  group
//
*/

/* 
//  Copy and pading function set
*/

/* Pading pixels for MCU in the boundary */
IPPAPI(IppStatus, ippiCopyExpand_8u_C3, (const Ipp8u *pSrc, 
                                        int srcStep,
                                        IppiSize srcSize,
                                        Ipp8u *pDst,
                                        int dstStep,
                                        IppiSize dstSize))

/* 
//  Color conversion function set
*/

/* CC: Convert MCU BGR to YCbCr with Level-Shift, the output data is interleave */
IPPAPI(IppStatus, ippiBGRToYCbCr444LS_MCU_8u16s_C3P3R,  (const Ipp8u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))
IPPAPI(IppStatus, ippiBGRToYCbCr422LS_MCU_8u16s_C3P3R,  (const Ipp8u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))
IPPAPI(IppStatus, ippiBGRToYCbCr411LS_MCU_8u16s_C3P3R,  (const Ipp8u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))

/* CC Convert MCU BGR555 to YCbCr with Level-Shift, the output data is interleave */
IPPAPI(IppStatus, ippiBGR555ToYCbCr444LS_MCU_16u16s_C3P3R,  (const Ipp16u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))
IPPAPI(IppStatus, ippiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R,  (const Ipp16u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))
IPPAPI(IppStatus, ippiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R,  (const Ipp16u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))

/* CC Convert MCU BGR565 to YCbCr with Level-Shift, the output data is interleave */
IPPAPI(IppStatus, ippiBGR565ToYCbCr444LS_MCU_16u16s_C3P3R,  (const Ipp16u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))
IPPAPI(IppStatus, ippiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R,  (const Ipp16u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))
IPPAPI(IppStatus, ippiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R,  (const Ipp16u * pSrcBGR, 
                                                        int srcStep, 
                                                        Ipp16s * pDstMCU[3]))

/* CC: Convert MCU YCbCr to BGR with Level-Shift, the input data is interleave */
IPPAPI(IppStatus, ippiYCbCr444ToBGRLS_MCU_16s8u_P3C3R,  (const Ipp16s * pSrcMCU[3],
                                                        Ipp8u * pDstBGR, 
                                                        int dstStep))
IPPAPI(IppStatus, ippiYCbCr422ToBGRLS_MCU_16s8u_P3C3R, (const Ipp16s * pSrcMCU[3],
                                                        Ipp8u * pDstBGR, 
                                                        int dstStep)) 
IPPAPI(IppStatus, ippiYCbCr411ToBGRLS_MCU_16s8u_P3C3R,(const Ipp16s * pSrcMCU[3],
                                                        Ipp8u * pDstBGR, 
                                                        int dstStep))


/* CC Convert MCU YCbCr to BGR555 with Level-Shift, the input data is interleave */
IPPAPI(IppStatus, ippiYCbCr444ToBGR555LS_MCU_16s16u_P3C3R,  (const Ipp16s * pSrcMCU[3],
                                                        Ipp16u * pDstBGR, 
                                                        int dstStep))
IPPAPI(IppStatus, ippiYCbCr422ToBGR555LS_MCU_16s16u_P3C3R, (const Ipp16s * pSrcMCU[3],
                                                        Ipp16u * pDstBGR, 
                                                        int dstStep)) 
IPPAPI(IppStatus, ippiYCbCr411ToBGR555LS_MCU_16s16u_P3C3R,(const Ipp16s * pSrcMCU[3],
                                                        Ipp16u * pDstBGR, 
                                                        int dstStep))

/* CC Convert MCU YCbCr to BGR565 with Level-Shift, the input data is interleave */
IPPAPI(IppStatus, ippiYCbCr444ToBGR565LS_MCU_16s16u_P3C3R,  (const Ipp16s * pSrcMCU[3],
                                                        Ipp16u * pDstBGR, 
                                                        int dstStep))
IPPAPI(IppStatus, ippiYCbCr422ToBGR565LS_MCU_16s16u_P3C3R, (const Ipp16s * pSrcMCU[3],
                                                        Ipp16u * pDstBGR, 
                                                        int dstStep)) 
IPPAPI(IppStatus, ippiYCbCr411ToBGR565LS_MCU_16s16u_P3C3R,(const Ipp16s * pSrcMCU[3],
                                                        Ipp16u * pDstBGR, 
                                                        int dstStep))
/*
//
// JPEG codec function group
//
*/

/*
// DCT & Quantization function set
*/

/* Forward DCT with quantization */
IPPAPI(IppStatus, ippiDCTQuantFwdTableInit_JPEG_8u16u, (const Ipp8u *pQuantRawTable,
                                                        Ipp16u *pQuantFwdTable))

IPPAPI(IppStatus, ippiDCTQuantFwd_JPEG_16s, (const Ipp16s* pSrc, 
                                             Ipp16s *pDst, 
                                             const Ipp16u *pQuantFwdTable))

IPPAPI(IppStatus, ippiDCTQuantFwd_JPEG_16s_I, (Ipp16s* pSrcDst, 
                                               const Ipp16u *pQuantFwdTable))

/* Inverse DCT with dequantization */
IPPAPI(IppStatus, ippiDCTQuantInvTableInit_JPEG_8u16u, (const Ipp8u *pQuantRawTable,
                                                        Ipp16u *pQuantInvTable))

IPPAPI(IppStatus, ippiDCTQuantInv_JPEG_16s, (const Ipp16s* pSrc, 
                                             Ipp16s *pDst, 
                                             const Ipp16u *pQuantInvTable))

IPPAPI(IppStatus, ippiDCTQuantInv_JPEG_16s_I, (Ipp16s* pSrcDst, 
                                               const Ipp16u *pQuantInvTable))

/*
// Huffman coding function set
*/

/* Generate Huffman table for encoder */
IPPAPI(IppStatus, ippiEncodeHuffmanSpecGetBufSize_JPEG_8u, (int* pSize))

IPPAPI(IppStatus, ippiEncodeHuffmanSpecInit_JPEG_8u, (const Ipp8u *pHuffBits,
                                                    const Ipp8u *pHuffValue,
                                                    IppiEncodeHuffmanSpec *pHuffTable))

IPPAPI(IppStatus, ippiEncodeHuffmanStateGetBufSize_JPEG_8u, (int* pSize))

IPPAPI(IppStatus, ippiEncodeHuffmanStateInit_JPEG_8u,
                                 (IppiEncodeHuffmanState* pHuffState))

/* Encoder for baseline */
IPPAPI(IppStatus, ippiEncodeHuffman8x8_Direct_JPEG_16s1u_C1, ( const Ipp16s *pSrc, Ipp8u  *pDst,  
                                                    int   *pDstBitsLen, Ipp16s *pDCPred,
                                                    const IppiEncodeHuffmanSpec *pDCHuffTable,
                                                    const IppiEncodeHuffmanSpec *pACHuffTable))

/* Encoder for progressive */
IPPAPI(IppStatus, ippiEncodeHuffman8x8_DCFirst_JPEG_16s1u_C1,
                             (const Ipp16s                  *pSrc,
                              Ipp8u                         *pDst,
                              int                           dstBytesLen,
                              int                           *pDstCurrpos,
                              Ipp16s                        *pDCPred,
                              int                           al,
                              const IppiEncodeHuffmanSpec   *pDCHuffTable,
                              IppiEncodeHuffmanState        *pHuffState,
                              int                           bFlushState))

IPPAPI(IppStatus, ippiEncodeHuffman8x8_DCRefine_JPEG_16s1u_C1,
                             (const Ipp16s                  *pSrc,
                              Ipp8u                         *pDst,
                              int                           dstBytesLen,
                              int                           *pDstCurrpos,
                              int                           al,
                              IppiEncodeHuffmanState        *pHuffState,
                              int                           bFlushState))

IPPAPI(IppStatus, ippiEncodeHuffman8x8_ACFirst_JPEG_16s1u_C1,
                             (const Ipp16s                  *pSrc,
                              Ipp8u                         *pDst,
                              int                           dstBytesLen,
                              int                           *pDstCurrpos,
                              int                           ss,
                              int                           se,
                              int                           al,
                              const IppiEncodeHuffmanSpec   *pACHuffTable,
                              IppiEncodeHuffmanState        *pHuffState,
                              int                           bFlushState))

IPPAPI(IppStatus, ippiEncodeHuffman8x8_ACRefine_JPEG_16s1u_C1,
                             (const Ipp16s                  *pSrc,
                              Ipp8u                         *pDst,
                              int                           dstBytesLen,
                              int                           *pDstCurrpos,
                              int                           ss,
                              int                           se,
                              int                           al,
                              const IppiEncodeHuffmanSpec   *pACHuffTable,
                              IppiEncodeHuffmanState        *pHuffState,
                              int                           bFlushState))

/* Generate Huffman table for decoder */
IPPAPI(IppStatus, ippiDecodeHuffmanSpecGetBufSize_JPEG_8u, (int* pSize))

IPPAPI(IppStatus, ippiDecodeHuffmanSpecInit_JPEG_8u, ( const Ipp8u *pHuffBits,
                                                    const Ipp8u *pHuffValue,
                                                    IppiDecodeHuffmanSpec *pHuffTable))
                                                        
IPPAPI(IppStatus, ippiDecodeHuffmanStateGetBufSize_JPEG_8u, (int* pSize))

IPPAPI(IppStatus, ippiDecodeHuffmanStateInit_JPEG_8u,
                                     (IppiDecodeHuffmanState* pHuffState))

/* Decoder for baseline */
IPPAPI(IppStatus, ippiDecodeHuffman8x8_Direct_JPEG_1u16s_C1,(const Ipp8u *pSrc, int    *pSrcBitsLen,
                                                    Ipp16s      *pDst, Ipp16s *pDCPred,
                                                    int      *pMarker, Ipp32u *pPrefetchedBits,
                                                    int      *pNumValidPrefetchedBits,
                                                    const IppiDecodeHuffmanSpec *pDCHuffTable,
                                                    const IppiDecodeHuffmanSpec *pACHuffTable))

/* Decoder for baseline */
IPPAPI(IppStatus, ippiDecodeHuffman8x8_DCFirst_JPEG_1u16s_C1,
                             (const Ipp8u                   *pSrc,
                              int                           srcBytesLen,
                              int                           *pSrcCurrpos,
                              Ipp16s                        *pDst,
                              Ipp16s                        *pDCPred,
                              int                           *pMarker,
                              int                           al,
                              const IppiDecodeHuffmanSpec   *pDCHuffTable,
                              IppiDecodeHuffmanState        *pHuffState))

IPPAPI(IppStatus, ippiDecodeHuffman8x8_DCRefine_JPEG_1u16s_C1,
                             (const Ipp8u                   *pSrc,
                              int                           srcBytesLen,
                              int                           *pSrcCurrpos,
                              Ipp16s                        *pDst,
                              int                           *pMarker,
                              int                           al,
                              IppiDecodeHuffmanState        *pHuffState))

IPPAPI(IppStatus, ippiDecodeHuffman8x8_ACFirst_JPEG_1u16s_C1,
                             (const Ipp8u                   *pSrc,
                              int                           srcBytesLen,
                              int                           *pSrcCurrpos,
                              Ipp16s                        *pDst,
                              int                           *pMarker,
                              int                           ss,
                              int                           se,
                              int                           al,
                              const IppiDecodeHuffmanSpec   *pACHuffTable,
                              IppiDecodeHuffmanState        *pHuffState))

IPPAPI(IppStatus, ippiDecodeHuffman8x8_ACRefine_JPEG_1u16s_C1,
                             (const Ipp8u                   *pSrc,
                              int                           srcBytesLen,
                              int                           *pSrcCurrpos,
                              Ipp16s                        *pDst,
                              int                           *pMarker,
                              int                           ss,
                              int                           se,
                              int                           al,
                              const IppiDecodeHuffmanSpec   *pACHuffTable,
                              IppiDecodeHuffmanState        *pHuffState))

/*==================== Intel IPP JPEG 2000 API definition ===================*/

/*========== 2D DWT functions =========*/
IPPAPI(IppStatus, ippiWTFwd_B53_JPEG2K_16s_C1IR, (Ipp16s  *pSrcDstTile,
                                                  int     step,
                                                  const   IppiRect *pTileRect,
                                                  Ipp8u   *pBuffer))

IPPAPI(IppStatus, ippiWTFwd_B53_JPEG2K_32s_C1IR, (Ipp32s  *pSrcDstTile,
                                                  int     step,
                                                  const   IppiRect *pTileRect,
                                                  Ipp8u   *pBuffer))

IPPAPI(IppStatus, ippiWTInv_B53_JPEG2K_16s_C1IR, (Ipp16s  *pSrcDstTile,
                                                  int     step,
                                                  const   IppiRect *pTileRect,
                                                  Ipp8u   *pBuffer))

IPPAPI(IppStatus, ippiWTInv_B53_JPEG2K_32s_C1IR, (Ipp32s  *pSrcDstTile,
                                                  int     step,
                                                  const   IppiRect *pTileRect,
                                                  Ipp8u   *pBuffer))

IPPAPI(IppStatus, ippiWTFwd_D97_JPEG2K_16s_C1IR, (Ipp16s   *pSrcDstTile,
                                                  int      step,
                                                  const    IppiRect *pTileRect,
                                                  Ipp8u    *pBuffer))

IPPAPI(IppStatus, ippiWTFwd_D97_JPEG2K_32s_C1IR, (Ipp32s   *pSrcDstTile,
                                                  int      step,
                                                  const    IppiRect *pTileRect,
                                                  Ipp8u    *pBuffer))

IPPAPI(IppStatus, ippiWTInv_D97_JPEG2K_16s_C1IR, (Ipp16s   *pSrcDstTile,
                                                  int      step,
                                                  const    IppiRect *pTileRect,
                                                  Ipp8u    *pBuffer))

IPPAPI(IppStatus, ippiWTInv_D97_JPEG2K_32s_C1IR, (Ipp32s   *pSrcDstTile,
                                                  int      step,
                                                  const    IppiRect *pTileRect,
                                                  Ipp8u    *pBuffer))

IPPAPI(IppStatus, ippiWTGetBufSize_B53_JPEG2K_16s_C1IR,(const IppiRect *pTileRect,
                                                                   int *pSize))

IPPAPI(IppStatus, ippiWTGetBufSize_B53_JPEG2K_32s_C1IR,(const IppiRect *pTileRect,
                                                                   int *pSize))

IPPAPI(IppStatus, ippiWTGetBufSize_D97_JPEG2K_16s_C1IR,(const IppiRect *pTileRect,
                                                                   int *pSize))

IPPAPI(IppStatus, ippiWTGetBufSize_D97_JPEG2K_32s_C1IR,(const IppiRect *pTileRect,
                                                                   int *pSize))


#ifdef __cplusplus
}
#endif

#endif /* _IPPJP_H_ */ 

/* End of file */
