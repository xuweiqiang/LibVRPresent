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

    pcmfuncs.h

Abstract:  

    This file implements typedefs, macros, data and functions to support
    run-time dynamic linking of the PCMCIA card services functions.
    This is provided as a sample to platform writers and is
    expected to be able to be used without modification on most (if not
    all) hardware platforms.

Functions:

    InitCardServicesTable()

Notes:


--*/

#ifndef _PCMFUNCS_H_
#define _PCMFUNCS_H_

#ifdef __cplusplus
extern "C" {
#endif


//
// Card Services Function Prototypes
//
typedef CARD_CLIENT_HANDLE (WINAPI *PFN_CardRegisterClient)(CLIENT_CALLBACK CallBackFn, PCARD_REGISTER_PARMS pParms);
typedef STATUS (WINAPI *PFN_CardDeregisterClient)(CARD_CLIENT_HANDLE hCardClient);
typedef STATUS (WINAPI *PFN_CardGetFirstTuple)(PCARD_TUPLE_PARMS pGetTupleParms);
typedef STATUS (WINAPI *PFN_CardGetNextTuple)(PCARD_TUPLE_PARMS pGetTupleParms);
typedef STATUS (WINAPI *PFN_CardGetTupleData)(PCARD_DATA_PARMS pGetTupleData);
typedef STATUS (WINAPI *PFN_CardGetParsedTuple)(CARD_SOCKET_HANDLE hSocket, UINT8 uDesiredTuple, PVOID pBuf, PUINT32 pnItems);
typedef STATUS (WINAPI *PFN_CardRequestExclusive)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSocket);
typedef STATUS (WINAPI *PFN_CardReleaseExclusive)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSocket);
typedef STATUS (WINAPI *PFN_CardRequestSocketMask)(CARD_CLIENT_HANDLE hCardClient,
                             CARD_SOCKET_HANDLE hSock, UINT16 fEventMask);
typedef STATUS (WINAPI *PFN_CardReleaseSocketMask)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSock);
typedef STATUS (WINAPI *PFN_CardGetEventMask)(CARD_CLIENT_HANDLE hCardClient, PCARD_EVENT_MASK_PARMS pMaskParms);
typedef STATUS (WINAPI *PFN_CardSetEventMask)(CARD_CLIENT_HANDLE hCardClient, PCARD_EVENT_MASK_PARMS pMaskParms);
typedef STATUS (WINAPI *PFN_CardResetFunction)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSock);
typedef CARD_WINDOW_HANDLE (WINAPI *PFN_CardRequestWindow)(CARD_CLIENT_HANDLE hCardClient, PCARD_WINDOW_PARMS pCardWinParms);
typedef STATUS (WINAPI *PFN_CardReleaseWindow)(CARD_WINDOW_HANDLE hCardWin);   
typedef STATUS (WINAPI *PFN_CardModifyWindow)(CARD_WINDOW_HANDLE hCardWindow, UINT16 fAttributes, UINT8 fAccessSpeed);
typedef PVOID  (WINAPI *PFN_CardMapWindow)(CARD_WINDOW_HANDLE hCardWindow, UINT32 uCardAddress, UINT32 uSize, PUINT32 pGranularity);
typedef STATUS (WINAPI *PFN_CardGetStatus)(PCARD_STATUS pStatus);
typedef STATUS (WINAPI *PFN_CardRequestIRQ)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSocket,
                      CARD_ISR ISRFunction, UINT32 uISRContextData);
typedef STATUS (WINAPI *PFN_CardReleaseIRQ)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSocket);
typedef STATUS (WINAPI *PFN_CardRequestConfiguration)(CARD_CLIENT_HANDLE hCardClient, PCARD_CONFIG_INFO pParms);
typedef STATUS (WINAPI *PFN_CardModifyConfiguration)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSock, PUINT16 fAttributes);
typedef STATUS (WINAPI *PFN_CardReleaseConfiguration)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSock);
typedef STATUS (WINAPI *PFN_CardAccessConfigurationRegister)(CARD_CLIENT_HANDLE hCardClient,
                                       CARD_SOCKET_HANDLE hSock,UINT8 rw_flag,
                                       UINT8 offset,UINT8 *pValue);
typedef STATUS (WINAPI *PFN_CardPowerOn)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSock);
typedef STATUS (WINAPI *PFN_CardPowerOff)(CARD_CLIENT_HANDLE hCardClient, CARD_SOCKET_HANDLE hSock);

#define IND_CardRegisterClient               0
#define IND_CardDeregisterClient             1
#define IND_CardGetFirstTuple                2
#define IND_CardGetNextTuple                 3 
#define IND_CardGetTupleData                 4
#define IND_CardGetParsedTuple               5
#define IND_CardRequestExclusive             6
#define IND_CardReleaseExclusive             7
#define IND_CardRequestSocketMask            8
#define IND_CardReleaseSocketMask            9
#define IND_CardGetEventMask                10
#define IND_CardSetEventMask                11
#define IND_CardResetFunction               12
#define IND_CardRequestWindow               13
#define IND_CardReleaseWindow               14
#define IND_CardModifyWindow                15
#define IND_CardMapWindow                   16
#define IND_CardGetStatus                   17
#define IND_CardRequestIRQ                  18
#define IND_CardReleaseIRQ                  19
#define IND_CardRequestConfiguration        20
#define IND_CardReleaseConfiguration        21
#define IND_CardAccessConfigurationRegister 22
#define IND_CardPowerOn                     23
#define IND_CardPowerOff                    24
#define IND_CardModifyConfiguration         25

#define MAX_CARDSERVICES_FUNCTIONS			26

#define DECLARE_CARDSERVICES_TABLE	DWORD	CardServiceFunctions[MAX_CARDSERVICES_FUNCTIONS]
extern DWORD CardServiceFunctions[];

#define CallCardServices(name) (*((PFN_##name)(CardServiceFunctions[IND_##name])))

_inline HINSTANCE
InitCardServicesTable()
{
    HINSTANCE hInst;
    int i;

    if (NULL != (hInst = LoadLibrary(TEXT("pcmcia.dll")))) {

	CardServiceFunctions[IND_CardRegisterClient] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardRegisterClient"));

	CardServiceFunctions[IND_CardDeregisterClient] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardDeregisterClient"));
	
	CardServiceFunctions[IND_CardGetFirstTuple] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardGetFirstTuple"));
	
	CardServiceFunctions[IND_CardGetNextTuple] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardGetNextTuple"));
	
	CardServiceFunctions[IND_CardGetTupleData] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardGetTupleData"));
	
	CardServiceFunctions[IND_CardGetParsedTuple] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardGetParsedTuple"));
	
	CardServiceFunctions[IND_CardRequestExclusive] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardRequestExclusive"));
	
	CardServiceFunctions[IND_CardReleaseExclusive] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardReleaseExclusive"));
	
	CardServiceFunctions[IND_CardRequestSocketMask] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardRequestSocketMask"));
	
	CardServiceFunctions[IND_CardReleaseSocketMask] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardReleaseSocketMask"));
	
	CardServiceFunctions[IND_CardGetEventMask] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardGetEventMask"));
	
	CardServiceFunctions[IND_CardSetEventMask] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardSetEventMask"));
	
	CardServiceFunctions[IND_CardResetFunction] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardResetFunction"));
	
	CardServiceFunctions[IND_CardRequestWindow] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardRequestWindow"));
	
	CardServiceFunctions[IND_CardReleaseWindow] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardReleaseWindow"));
	
	CardServiceFunctions[IND_CardModifyWindow] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardModifyWindow"));
	
	CardServiceFunctions[IND_CardMapWindow] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardMapWindow"));
	
	CardServiceFunctions[IND_CardGetStatus] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardGetStatus"));
	
        CardServiceFunctions[IND_CardRequestIRQ] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardRequestIRQ"));

	CardServiceFunctions[IND_CardReleaseIRQ] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardReleaseIRQ"));
	
	CardServiceFunctions[IND_CardRequestConfiguration] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardRequestConfiguration"));
	
	CardServiceFunctions[IND_CardReleaseConfiguration] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardReleaseConfiguration"));
	
	CardServiceFunctions[IND_CardAccessConfigurationRegister] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardAccessConfigurationRegister"));
	
        CardServiceFunctions[IND_CardPowerOn] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardPowerOn"));

        CardServiceFunctions[IND_CardPowerOff] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardPowerOff"));

        CardServiceFunctions[IND_CardModifyConfiguration] =
            (DWORD)GetProcAddressW(hInst, TEXT("CardModifyConfiguration"));

#ifdef TARGET_NT
        pf_ntecards_inb = (PFN_ntecards_inb) GetProcAddressW(hInst, TEXT("ntecards_inb"));
        pf_ntecards_outb = (PFN_ntecards_outb) GetProcAddressW(hInst, TEXT("ntecards_outb"));
#endif

	// Check for errors.
	for (i=0; i < MAX_CARDSERVICES_FUNCTIONS; i++) {
            if (CardServiceFunctions[i] == (DWORD)NULL) {
                FreeLibrary(hInst);
                return NULL;
            }
	}
    }
    return hInst;
}

#ifdef __cplusplus
}
#endif

#endif // _PCMFUNCS_H_
