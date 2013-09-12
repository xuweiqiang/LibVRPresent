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
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include "General_StreamInput.h"

#define COMPRESSED_FRAME_BUFFER_SIZE  /*2 * 1024 */64* 1024
//#define COMPRESSED_FRAME_BUFFER_SIZE  32 * 1024 * 1024

/***************************************************************************
// Name:             StreamInitBuffer
// Description:      Initialize the input bitstream buffer for decoder
//
// Input Arguments:
//      *pBsBuffer  Pointer to decoder input bitstream start buffer
//      *pBsCurByte Pointer to decoder input bitstream current buffer
//      bsByteLen   Pointer to decoder input bitstream buffer
//
// Output Arguments:
//      *pBsBuffer  Pointer to updated decoder bitstream start buffer
//      *pBsCurByte Pointer to decoder input bitstream current buffer
//      bsByteLen   Bitstream buffer length
//
// Returns:
//     0		--		No Error
*****************************************************************************/
int StreamInitBuffer(unsigned char **ppBsBuffer, unsigned char **ppBsCurByte, int *bsByteLen)
{
	/*
	// Initialize IviBitstream
	// at least big enough to store 2 frame data for less reload
	// It can also be used to re-initialize
	*/
	if(*ppBsBuffer == NULL)
	{
		*ppBsBuffer = (unsigned char *)calloc(COMPRESSED_FRAME_BUFFER_SIZE+4, sizeof(unsigned char));		//+ 4 For keep EOS in some special case
		if (*ppBsBuffer == NULL) {
			return -1;
		}
	}

	/*
	//no read data at beginning
	//set current pointer to the beginning of an (empty) buffer
	*/
	*ppBsCurByte = *ppBsBuffer;
	*bsByteLen = 0;

	return 0;
}


/***************************************************************************
// Name:             StreamReloadBuffer
// Description:      Reload the input bitstream buffer from file base for decoder
//
// Input Arguments:
//      pBsBuffer  Pointer to decoder input bitstream start buffer
//      *pBsCurByte Pointer to decoder input bitstream current buffer
//      bsByteLen   Pointer to decoder input bitstream buffer length
//		stream      Pointer to the input file stream buffer
//
// Output Arguments:
//      *pBsCurByte Pointer to decoder input bitstream current buffer
//      bsByteLen   Pointer to decoder input bitstream buffer length
//
// Returns:
//		0		--		No Error
//		-1		--		EOF
*****************************************************************************/
int StreamReloadBuffer(unsigned char *pBsBuffer, unsigned char **ppBsCurByte, int *bsByteLen, FILE *stream)
{
	int offset;
	int	remain_data;

	offset = *ppBsCurByte - pBsBuffer;
	remain_data = *bsByteLen - offset;
	if ( remain_data < 0) {
//		return -1;
		remain_data = 0;
	}

 	if ( remain_data > 0 && remain_data < COMPRESSED_FRAME_BUFFER_SIZE ) {
		memcpy( pBsBuffer, *ppBsCurByte, remain_data );
	}
	*bsByteLen = remain_data + fread( pBsBuffer + remain_data, 1,
		COMPRESSED_FRAME_BUFFER_SIZE - remain_data, stream );

	*ppBsCurByte = pBsBuffer;

	if( *bsByteLen == remain_data)
		return -2;

	if ( *bsByteLen < COMPRESSED_FRAME_BUFFER_SIZE ) {
		//fseek(stream, 0, SEEK_SET);
		//END of FILE
		*(pBsBuffer + *bsByteLen    ) = 0x00;
		*(pBsBuffer + *bsByteLen + 1) = 0x00;
		*(pBsBuffer + *bsByteLen + 2) = 0x00;
		*(pBsBuffer + *bsByteLen + 3) = 0x01;	//Add EOS tag into buffer

		(*bsByteLen) += 4;

		return -1;
	}

	return 0;
}

/***************************************************************************
// Name:             StreamFreeBuffer
// Description:      Free input bitstream buffer for video decoder
//
// Input Arguments:
//      pBsBuffer  Pointer to decoder input bitstream start buffer
//
// Output Arguments:
//      pBsBuffer  Free the pointer to decoder input bitstream start buffer
//
// Returns:
//     0		--		No Error
*****************************************************************************/
int StreamFreeBuffer(unsigned char *pBsBuffer)
{
	if (pBsBuffer) {
		free(pBsBuffer);
		pBsBuffer = NULL;
	}

	return 0;
}
