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

eapcfg.h

Abstract:  

    Contains UI functions for the following EAP extensions:
        EAPTLS
    
Functions:


Notes: 
    

--*/
#ifndef _EAPCFG_H_
#define _EAPCFG_H_
// @CESYSGEN IF CE_MODULES_NETUI


#ifdef __cplusplus
extern "C" {
#endif

#define PPP_EAP_TLS                  13
#define PPP_EAP_PEAP		    25		// EAP type
#define PPP_EAP_MSCHAPv2		26		// EAP type


#define MAX_HASH_SIZE       20      // Certificate hash size

typedef struct _EAPTLS_HASH
{
    DWORD   cbHash;                 // Number of bytes in the hash
    BYTE    pbHash[MAX_HASH_SIZE];  // The hash of a certificate

} EAPTLS_HASH;

typedef struct _EAPTLS_CONN_PROPERTIES_V1
{
    DWORD       dwVersion;              // Version will be 1 for this release
    DWORD       dwSize;                 // Number of bytes in this structure
    DWORD       fFlags;                 // See EAPTLS_CONN_FLAG_*
//#ifdef UNDER_CE
    EAPTLS_HASH UserHash;	// hash for the user certificate (also in USER_PROPERTIES)
//#endif    
    DWORD       dwNumHashes;            // Number of root cert hash structures in the list
    BYTE        bData[1];               // Data - contains an array of
                                        // EAPTLS_HASH structures followed by
                                        // a string specifying server name
}EAPTLS_CONN_PROPERTIES_V1;

// Values of the EAPTLS_CONN_PROPERTIES->fFlags field

// Use a certificate on this machine
#define     EAPTLS_CONN_FLAG_REGISTRY           0x00000001
// Do not validate server cert
#define     EAPTLS_CONN_FLAG_NO_VALIDATE_CERT   0x00000002
// Do not Validate server name
#define     EAPTLS_CONN_FLAG_NO_VALIDATE_NAME   0x00000004
// Do not use client certificate authentication
#define     EAPTLS_CONN_FLAG_NO_CLIENT_CERT   0x00000008

#define     EAPTLS_CONN_PROPS_V1                   1

//
// This structure is passed in to RasEapInvokeInteractiveUI as  pUIContextData
//  
typedef struct _EAPEXTUI_INPUT
{
    DWORD dwVersion;        // should be 1
    DWORD dwSize;           // size of this struct
    DWORD fFlags;
    DWORD dwCode;   // see EAPEXTUI_CODE_ below
    DWORD dwStatus;
    PVOID   param;
} EAPEXTUI_INPUT;

//  Values for dwCode
#define EAPEXTUI_CODE_SERVERAUTH_ERROR      1
#define EAPEXTUI_CODE_CLIENTAUTH_ERROR      2

// Values for dwFlags
#define EAPEXTUI_FLAG_ERROR                   0x00000001     // error notification (dwStatus is error code)
#define EAPEXTUI_FLAG_WARNING              0x00000002     // warning notification
#define EAPEXTUI_FLAG_RESPONSE             0x00000004     // response needed

//
// This structure is returned from RasEapInvokeInteractiveUI as pDataFromInteractiveUI
//

typedef struct _EAPEXTUI_OUTPUT
{
    DWORD dwVersion;
    DWORD dwSize;
    DWORD fFlags;
    DWORD dwValue;
} EAPEXTUI_OUTPUT;


#ifdef __cplusplus
}
#endif
// @CESYSGEN ENDIF
#endif //_EAPCFG_H_


