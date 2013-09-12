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
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

certui.h

Abstract:  

Functions:


Notes: 
    The netui library cannot be loaded before GWE has initialized (since it
    initializes common controls on DLL entry).  So, check that the WMGR API
    set has been registered before loading netui.dll.

--*/

#ifndef _CERTUI_H_
#define _CERTUI_H_
// @CESYSGEN IF CE_MODULES_NETUI

#include <windev.h>
#include <wincrypt.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NETUI_PICKCERT
{
	int iSelected;	// [IN, OUT] the index of the selected cert
	int nCerts;		// [IN] number of certificates in ppCert array
	DWORD dwFlags;		// [IN] options
	PCCERT_CONTEXT *ppCerts	; // [IN] array of certs to display
} NETUI_PICKCERT, *PNETUI_PICKCERT;

typedef struct _NETUI_SECURITY_INFO
{
	BOOL	fSecure;			// Is this a secure connection.
	DWORD	dwCertStatus;		//	The status of the certificate
	DWORD	dwProtocol;			//	Protocol that this connection was made with (PCT, SSL2, SSL3, etc)
	ALG_ID	aiCipher;			//	Cipher that this connection as made with
	DWORD	dwCipherStrength;	//	Strength (in bits) that this connection was made with;
	ALG_ID	aiHash;				//	Hash that this connection as made with
	DWORD	dwHashStrength;		//	Strength (in bits) that this connection was made with;
	ALG_ID	aiExch;				//	Key Exchange type that this connection as made with
	DWORD dwExchStrength;		//	Strength (in bits) that this connection was made with;
} NETUI_SECURITY_INFO , *PNETUI_SECURITY_INFO;

BOOL PickCertificate (HWND hParent, PNETUI_PICKCERT pPickCert);
BOOL ImportCertificate(HWND hParent, HCERTSTORE hStore);
typedef BOOL (WINAPI *PFNIMPORTCERTIFICATE)(IN HWND hParent,
											IN HCERTSTORE hStore);

BOOL ShowCertificate(HWND hParent, PCCERT_CONTEXT pCert);
typedef BOOL (WINAPI *PFNSHOWCERTIFICATE)(IN HWND hParent,
										  IN PCCERT_CONTEXT pCert);

BOOL ShowCertificateEx(HWND hParent, PCCERT_CONTEXT pCert, PNETUI_SECURITY_INFO pNUISecInfo);
typedef BOOL (WINAPI *PFNSHOWCERTIFICATEEX)(IN HWND hParent,
											IN PCCERT_CONTEXT pCert,
											PNETUI_SECURITY_INFO pNUISecInfo);
#ifdef __cplusplus
}
#endif
// @CESYSGEN ENDIF
#endif //_CERTUI_H_

