/*
	WARNING	WARNING WARNING WARNINGWARNING	WARNING WARNING WARNING
	WARNING	WARNING WARNING WARNINGWARNING	WARNING WARNING WARNING
	WARNING	WARNING WARNING WARNINGWARNING	WARNING WARNING WARNING
	WARNING	WARNING WARNING WARNINGWARNING	WARNING WARNING WARNING
	WARNING	WARNING WARNING WARNINGWARNING	WARNING WARNING WARNING
	WARNING	WARNING WARNING WARNINGWARNING	WARNING WARNING WARNING
	WARNING	WARNING WARNING WARNINGWARNING	WARNING WARNING WARNING


	include this file before MS ECC routines


*/

//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Module Name:    ECC.H

Abstract:       Error Correcting Codes (ECC) module for FLASH Media Drivers 
  
-----------------------------------------------------------------------------*/

#ifndef _ECC_H_
#define _ECC_H_ 

#include <windows.h>


/*
 *	Lets not limit the amount of ECC data produced. Just keep the 3 bytes of ECC
 *  for every 256bytes of data.
 *
 *
 *	set max ecc buf size to support 8k block. 8192 / 256) * 3 =	96
 */
#define DATA_BLOCK_LEN          256
#define MAX_ECC_BUFF_LEN        96                // 3 bytes per 256 bytes of data

#define UNCORRECTABLE_ERROR     1
#define CORRECTABLE_ERROR       11


#ifdef __cplusplus
extern "C" {
#endif 

//---------------------------- Structure Definitions -----------------------------



//------------------------------- Public Interface ------------------------------

BOOL ECC_ComputeECC(LPBYTE pData, DWORD dwDataBuffLen, LPBYTE pECC, DWORD dwECCBuffLen); 
BOOL ECC_IsDataValid(LPBYTE pData, DWORD dwDataBuffLen, LPBYTE pExistingECC, DWORD dwECCBuffLen); 
BOOL ECC_CorrectData(LPBYTE pData, DWORD dwDataBuffLen, LPBYTE pExistingECC, DWORD dwECCBuffLen);
BOOL ECC_IsWritten (LPBYTE pECC, DWORD dwECCBuffLen);


//------------------------------ Helper Functions ------------------------------ 
UCHAR CountNumberOfOnes(UCHAR num);

#ifdef __cplusplus
}
#endif

#endif _ECC_H_



