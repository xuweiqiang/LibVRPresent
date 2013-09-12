//=============================================================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2009  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------
//
// This file is furnished as part of the Cidana Codec SDK. 
// Usage of this file, code, concepts, and/or algorithms is prohibited
// except under the terms of the Software Licensing Agreement with Cidana.
//

#ifndef _CI_H264BPDEC_H_
#define _CI_H264BPDEC_H_

#ifdef __cplusplus
extern "C" {
#endif

// {98D6F60E-8FFD-41e4-A770-4534588E9037}
CI_DEFINE_GUID(IID_H264BPDEC_C, 
0x98d6f60e, 0x8ffd, 0x41e4, 0xa7, 0x70, 0x45, 0x34, 0x58, 0x8e, 0x90, 0x37);

typedef struct
{
	CI_VIDEC_OPENOPTIONS_COMMON
	CI_U32 u32DoQuickOut;
} CI_H264BPDEC_OPENOPTIONS;

typedef struct  
{
	CI_VIDEC_DECODEOPTIONS_COMMON
} CI_H264BPDEC_DECODEOPTIONS;

typedef struct  
{
	CI_U32 u32Size;
} CI_H264BPDEC_GETFRAMEOPTIONS;

typedef struct
{	// Basic information
	CI_U32 u32Size;
	CI_U32 u32Cookie;			// unique signature used to release frame
	CI_U32 u32FrameCounter;		// used to indicate presence of dropped frames.
	CI_U32 u32FrameType;		// frame storage type
	CI_U32 u32ErrorStatus;		// error level
	CI_U32 u32ChromaFormat;
	CI_U32 u32FrameRate;
	CI_F64 f64BitRate;
	CI_DECODE_PTS Pts;			// Time Stamp of this frame

	CI_U8 *apu8Frame[3];			// pointer to each buffer
	CI_U32 au32Width[3];			// width
	CI_U32 au32Height[3];			// height
	CI_U32 au32Stride[3];			// stride of each buffer

	// TBD
	CI_U32 u32AspectRatioX;
	CI_U32 u32AspectRatioY;
	CI_U32 u32ProgressiveSequence;
	CI_U32 u32TopFieldFirst;
	CI_U32 u32ProgressiveFrame;

	// Special information
	CI_U32 u32POC;
	CI_S32 s32CPBRemovalDelay;
	CI_S32 s32BufferingPeriodSEI;
	CI_S32 s32FixedFrameRate;

	CI_U32 u32HorizontalSize;
	CI_U32 u32VerticalSize;
	CI_U32 au32FrameCenterHorizontalOffset[3];
	CI_U32 au32FrameCenterVerticalOffset[3];
	CI_S32 s32WarnFlag;
} CI_H264BPDEC_FRAME;

typedef struct  
{
	CI_VIDEC_STREAMINFO_COMMON
	CI_U32 u32Profile;
} CI_H264BPDEC_STREAMINFO;

typedef struct 
{
	CI_VIDEC_BUFFERINFO_COMMON
}CI_H264BPDEC_BUFFERINFO;

CI_RESULT CI_H264BPDEC_Create(
	OUT CI_VOID **ppDecoder,
	IN CONST CI_VOID *pLicense);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_Create)(
	OUT CI_VOID **ppDecoder,
	IN CONST CI_VOID *pLicense);

CI_RESULT CI_H264BPDEC_Open(
	IN OUT CI_VOID *pDecoder, 
	IN CONST CI_H264BPDEC_OPENOPTIONS *pOptions);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_Open)(
	IN OUT CI_VOID *pDecoder, 
	IN CONST CI_H264BPDEC_OPENOPTIONS *pOptions);

CI_RESULT CI_H264BPDEC_Close(
	IN OUT CI_VOID *pDecoder);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_Close)(
	IN OUT CI_VOID *pDecoder);

CI_U32 CI_H264BPDEC_Release(
	IN OUT CI_VOID *pDecoder);

typedef CI_U32 (* LPFN_CI_H264BPDEC_Release)(
	IN OUT CI_VOID *pDecoder);

CI_U32 CI_H264BPDEC_AddRef(
	IN OUT CI_VOID *pDecoder);

typedef CI_U32 (* LPFN_CI_H264BPDEC_AddRef)(
	IN OUT CI_VOID *pDecoder);

CI_RESULT CI_H264BPDEC_DecodeBuffer(
	IN OUT CI_VOID *pDecoder, 
	IN CONST CI_H264BPDEC_DECODEOPTIONS *pOptions,
	IN CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U32 *pu32FramesDecoded,
	OUT CI_U32 *pu32NumberOfSkippedFrames,
	OUT CI_U32 *pu32Status);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_DecodeBuffer)(
	IN OUT CI_VOID *pDecoder, 
	IN CONST CI_H264BPDEC_DECODEOPTIONS *pOptions,
	IN CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U32 *pu32FramesDecoded,
	OUT CI_U32 *pu32NumberOfSkippedFrames,
	OUT CI_U32 *pu32Status);

CI_RESULT CI_H264BPDEC_DecodeFrame(
	IN CI_VOID *pDecoder,
	IN CONST CI_H264BPDEC_DECODEOPTIONS *pOptions,
	OUT CI_U32 *pu32NumberOfDecodedFrames,
	OUT CI_U32 *pu32NumberOfSkippedFrames,
	OUT CI_U32 *pu32Status);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_DecodeFrame)(
	IN CI_VOID *pDecoder,
	IN CONST CI_H264BPDEC_DECODEOPTIONS *pOptions,
	OUT CI_U32 *pu32NumberOfDecodedFrames,
	OUT CI_U32 *pu32NumberOfSkippedFrames,
	OUT CI_U32 *pu32Status);

CI_RESULT CI_H264BPDEC_Reset(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32Flags);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_Reset)(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32Flags);

CI_RESULT CI_H264BPDEC_GetFrame(
	IN CI_VOID *pDecoder,
	IN CONST CI_H264BPDEC_GETFRAMEOPTIONS *pOptions,
	IN OUT CI_H264BPDEC_FRAME *pFrame);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_GetFrame)(
	IN CI_VOID *pDecoder,
	IN CONST CI_H264BPDEC_GETFRAMEOPTIONS *pOptions,
	IN OUT CI_H264BPDEC_FRAME *pFrame);

CI_U32 CI_H264BPDEC_AddRefFrame(
	IN CI_VOID *pDecoder, 
	IN CI_H264BPDEC_FRAME *pFrame);

typedef CI_U32 (* LPFN_CI_H264BPDEC_AddRefFrame)(
	IN CI_VOID *pDecoder, 
	IN CI_H264BPDEC_FRAME *pFrame);

CI_U32 CI_H264BPDEC_ReleaseFrame(
	IN CI_VOID *pDecoder, 
	IN CI_H264BPDEC_FRAME *pFrame);

typedef CI_U32 (* LPFN_CI_H264BPDEC_ReleaseFrame)(
	IN CI_VOID *pDecoder, 
	IN CI_H264BPDEC_FRAME *pFrame);

CI_RESULT CI_H264BPDEC_Get(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData,
	IN CI_U32 u32DataLen,
	OUT CI_U32 *pu32DataUsed);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_Get)(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData,
	IN CI_U32 u32DataLen,
	OUT CI_U32 *pu32DataUsed);

CI_RESULT CI_H264BPDEC_Set(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData,
	IN CI_U32 u32DataLen);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_Set)(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData,
	IN CI_U32 u32DataLen);

CI_RESULT CI_H264BPDEC_ParseInfo(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32SearchLevel,
	IN CI_S16 *ps16Suffix,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pdwConfidence,
	IN OUT CI_H264BPDEC_STREAMINFO *pStreamInfo);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_ParseInfo)(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32SearchLevel,
	IN CI_S16 *ps16Suffix,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pdwConfidence,
	IN OUT CI_H264BPDEC_STREAMINFO *pStreamInfo);

CI_RESULT CI_H264BPDEC_ParseAccessUnit(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pu32Status,
	OUT CI_U32 *pu32AUPos,
	OUT CI_U32 *pu32AULen,
	OUT CI_U64 *pu64AUTimeNum,
	OUT CI_U32 *pu32AUTimeDen);

typedef CI_RESULT (* LPFN_CI_H264BPDEC_ParseAccessUnit)(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pu32Status,
	OUT CI_U32 *pu32AUPos,
	OUT CI_U32 *pu32AULen,
	OUT CI_U64 *pu64AUTimeNum,
	OUT CI_U32 *pu32AUTimeDen);

typedef struct
{
	LPFN_CI_H264BPDEC_Create Create;
	LPFN_CI_H264BPDEC_Open Open;
	LPFN_CI_H264BPDEC_Close Close;
	LPFN_CI_H264BPDEC_Release Release;
	LPFN_CI_H264BPDEC_AddRef AddRef;
	LPFN_CI_H264BPDEC_DecodeBuffer DecodeBuffer;
	LPFN_CI_H264BPDEC_DecodeFrame DecodeFrame;
	LPFN_CI_H264BPDEC_Reset Reset;
	LPFN_CI_H264BPDEC_GetFrame GetFrame;
	LPFN_CI_H264BPDEC_ReleaseFrame ReleaseFrame;
	LPFN_CI_H264BPDEC_AddRefFrame AddRefFrame;
	LPFN_CI_H264BPDEC_Get Get;
	LPFN_CI_H264BPDEC_Set Set;
	LPFN_CI_H264BPDEC_ParseInfo ParseInfo;
} CI_H264BPDEC_FUNCTIONS;

#ifdef __cplusplus
}
#endif
#endif // _CI_H264BPDEC_H_
