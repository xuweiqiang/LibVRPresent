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

credmgr.h

Abstract:  
	Credential management functions

Functions:


Notes: 
    These functions are exposed by secur32.dll. They are likely to change in future versions of the OS.

--*/
#ifndef _CREDMGR_H
#define _CREDMGR_H_

// Bring in the new credMan header file
#include <cred.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Values of the Credential Flags field.
//

// Deprecated
// Caller should set credential blob to NULL instead
#define CRED_FLAGS_PROMPT_NOW           0x0002

// Deprecated
#define CRED_FLAGS_USERNAME_TARGET      0x0004
// Deprecated
#define CRED_FLAGS_PERSIST_CRED_BLOB    0x0008
// Deprecated
#define CRED_FLAGS_VALID_FLAGS          0x000E  // Mask of all valid flags

//
// Values of the Credential Type field.
//

// Defined in cred.h
// #define CRED_TYPE_GENERIC               1

// Defined in cred.h
//#define CRED_TYPE_DOMAIN_PASSWORD       2

// Redefined
//#define CRED_TYPE_DOMAIN_CERTIFICATE    3
#define CRED_TYPE_DOMAIN_CERTIFICATE    CRED_TYPE_CERTIFICATE

// Redefined
//#define CRED_TYPE_DOMAIN_VISIBLE_PASSWORD 4
#define CRED_TYPE_DOMAIN_VISIBLE_PASSWORD CRED_TYPE_PLAINTEXT_PASSWORD

// Deprecated
#define CRED_TYPE_MAXIMUM               7       // Maximum supported cred type

// Removed
//#define CRED_SUBTYPE_NTLM               1

// Removed
//#define CRED_SUBTYPE_KERBEROS       2

// Removed
//#define CRED_SUBTYPE_NTLM_V1         3

// Removed
//#define CRED_TYPE_DOMAIN_PASSWORD_NTLM_V1        (CRED_TYPE_DOMAIN_PASSWORD | (CRED_SUBTYPE_NTLM_V1 << 16))

// Redefined
//#define CRED_TYPE_DOMAIN_PASSWORD_NTLM              (CRED_TYPE_DOMAIN_PASSWORD | (CRED_SUBTYPE_NTLM << 16))
#define CRED_TYPE_DOMAIN_PASSWORD_NTLM             CRED_TYPE_NTLM

// Redefined
//#define CRED_TYPE_DOMAIN_PASSWORD_KERBEROS      (CRED_TYPE_DOMAIN_PASSWORD | (CRED_SUBTYPE_KERBEROS << 16))
#define CRED_TYPE_DOMAIN_PASSWORD_KERBEROS      CRED_TYPE_KERBEROS

//
// Maximum size of the CredBlob field (in bytes)
//

// Redefined
//#define CRED_MAX_CREDENTIAL_BLOB_SIZE 512
#define CRED_MAX_CREDENTIAL_BLOB_SIZE CRED_MAX_BLOB_SIZE

//
// Values of the Credential Persist field
//

#define CRED_PERSIST_NONE               0
#define CRED_PERSIST_SESSION            0

// Redefined
// #define CRED_PERSIST_LOCAL_MACHINE      2
#define CRED_PERSIST_LOCAL_MACHINE      CRED_FLAG_PERSIST

// Redefined
// #define CRED_PERSIST_ENTERPRISE         3
#define CRED_PERSIST_ENTERPRISE         CRED_FLAG_PERSIST

typedef struct _CECREDENTIAL {
	DWORD Flags;
	DWORD Type;
	FILETIME LastWritten;
	DWORD CredentialBlobSize;
	LPBYTE CredentialBlob;
	DWORD Persist;
	LPWSTR UserName;
} CECREDENTIAL, *PCECREDENTIAL;

BOOL WINAPI
CeCredRead( 
	IN PCWSTR pszTarget, 
	IN DWORD dwCredType,
	IN DWORD dwFlags,
	OUT PCECREDENTIAL *ppCred);

typedef BOOL (WINAPI *PFNCECREDREAD)( IN PCWSTR pszTarget, IN DWORD dwCredType,	IN DWORD dwFlags, OUT PCECREDENTIAL *ppCred);

BOOL
WINAPI 
CeCredWrite(
	IN PCWSTR pszTarget,
	IN PCECREDENTIAL pCred);

typedef BOOL (WINAPI *PFNCECREDWRITE)(IN PCWSTR pszTarget, IN PCECREDENTIAL pCred);

_inline void
CeCredFree(PVOID pvBuffer){
	LocalFree(pvBuffer);
}

typedef BOOL (WINAPI *PFNCECREDFREE)(PVOID pvBuffer);


#ifdef __cplusplus
}
#endif

#endif // _CREDMGR_H

