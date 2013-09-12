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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

crypt_hw_offload

Abstract:  

    Defines the crypto hw offload interface for windows ce.
    
Notes: 


--*/
#ifdef __CRYPT_HW_OFFLOAD__
#else
#define __CRYPT_HW_OFFLOAD__
typedef DWORD ALGID;
typedef PVOID HOFFLOAD;
typedef PVOID HOFFLOADKEY;
typedef PVOID HOFFLOADHASH;

/* Block Cipher algorithms of interest
      CALG_DES
      CALG_3DES
      CALG_AES_128
      CALG_AES_192
      CALG_AES_256
*/
/* Hash algorithms of interest
       CALG_MD5
       CALG_SHA
       CALG_SHA_256
*/

typedef
HOFFLOADKEY (* PFN_INITBLOCKCIPHERKEY)(HOFFLOAD hOffload, ALGID algid, PBYTE key, DWORD cbKey);

typedef
BOOL (* PFN_BLOCKCIPHER)(HOFFLOADKEY hKey, PBYTE pbBlock, BOOL fEncrypt);

typedef
BOOL (* PFN_BLOCKCIPHERCBC)(HOFFLOADKEY hKey, PBYTE pbBlock, DWORD cBlocks, PBYTE pbIV,  BOOL fEncrypt);

typedef
void (* PFN_FREEBLOCKCIPHERKEY)(HOFFLOADKEY hKey);

typedef
HOFFLOADHASH (* PFN_INITHASH)(HOFFLOAD hOffload, ALGID algid);

typedef
BOOL (*PFN_UPDATEHASH)(HOFFLOADHASH hHash, const BYTE *pbData, DWORD cbData);

typedef
BOOL (*PFN_FINALIZEHASH)(HOFFLOADHASH hHash, PBYTE pbHashVal, DWORD cbHashVal);
typedef
BOOL (*PFN_MOD_EXP)( IN HOFFLOAD pOffloadInfo, IN BYTE *pbBase, IN BYTE *pbExpo, IN DWORD cbExpo, IN BYTE *pbModulus, IN DWORD cbModulus, OUT BYTE *pbResult, IN VOID *pReserved, IN DWORD dwFlags);

// hash algorithm support
#define CO_SUPPORTS_MD5_128         0x0001
#define CO_SUPPORTS_SHA_160         0x0002
#define CO_SUPPORTS_SHA_256         0x0004

// block cipher support
#define CO_SUPPORTS_DES             0x0001
#define CO_SUPPORTS_3DES            0x0002     
#define CO_SUPPORTS_AES_128         0x0010
#define CO_SUPPORTS_AES_192         0x0020
#define CO_SUPPORTS_AES_256         0x0040

// CBC mode multi-block cipher support
#define CO_SUPPORTS_DES_CBC         0x0004
#define CO_SUPPORTS_AES_CBC         0x0080

// Other items supported.
#define CO_SUPPORTS_MODEXP          0x001

// Initialize offload interface and determine its level of supports
//
typedef struct {
    DWORD cbSize;   // size of this table
    DWORD CiphersSupported;     // 
    DWORD HashesSupported;
    DWORD OtherSupported;
    PFN_INITBLOCKCIPHERKEY InitBlockCipherKey;
    PFN_BLOCKCIPHER BlockCipher;
    PFN_BLOCKCIPHERCBC BlockCipherCBC;
    PFN_FREEBLOCKCIPHERKEY FreeBlockCipherKey;
    PFN_INITHASH InitHash;
    PFN_UPDATEHASH UpdateHash;
    PFN_FINALIZEHASH FinalizeHash;
    PFN_MOD_EXP ModExp;
} CRYPTO_OFFLOAD_TABLE, *PCRYPTO_OFFLOAD_TABLE;

HOFFLOAD
InitCryptoInterface(PCRYPTO_OFFLOAD_TABLE pOffloadTable);

void
FreeCryptoInterface(HOFFLOAD hCryptoInterface);

// Based in HKLM
#define REG_HWO_ROOT L"Comm\\Security\\crypt_hw_offload"
#define HWO_INIT  L"InitCryptoInterface"
#define HWO_DEINIT L"FreeCryptoInterface"

#endif // __CRYPT_HW_OFFLOAD__
