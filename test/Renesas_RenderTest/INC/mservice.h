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

mservice.h

Abstract:  

Notes: 


--*/

#if ! defined (__mservice_H__)
#define __mservice_H__		1

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifndef WINCEMACRO
#error WINCEMACRO not defined __FILE__
#endif

#define WIN32_SERVICE_CALL(type, api, args) (*(type (*) args)IMPLICIT_CALL(SH_SERVICES, api))

// external exports from services
#define ActivateService            WIN32_SERVICE_CALL(HANDLE, 2, (LPCWSTR lpszDevKey, DWORD dwClientInfo))
#define RegisterService            WIN32_SERVICE_CALL(HANDLE, 3, (LPCWSTR lpszType, DWORD dwIndex, LPCWSTR lpszLib, DWORD dwInfo))
#define DeregisterService          WIN32_SERVICE_CALL(BOOL,4,(HANDLE hDevice))
// #define CloseAllServiceHandles     WIN32_SERVICE_CALL(void,5,(HPROCESS proc)) // not supported
#define CreateServiceHandle        WIN32_SERVICE_CALL(HANDLE,6,(LPCWSTR lpNew, DWORD dwAccess, DWORD dwShareMode, HPROCESS hProc))
#define GetServiceByIndex          WIN32_SERVICE_CALL(BOOL,7,(DWORD dwIndex, LPWIN32_FIND_DATA lpFindFileData))
#define ServiceIoControl           WIN32_SERVICE_CALL(BOOL,8,(HANDLE hService, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped))
#define ServiceAddPort             WIN32_SERVICE_CALL(BOOL,9,(HANDLE hService, SOCKADDR *pSockAddr, int cbSockAddr, int iProtocol, WCHAR *szRegWritePath))
#define ServiceUnbindPorts         WIN32_SERVICE_CALL(BOOL,10,(HANDLE hService))
#define EnumServices               WIN32_SERVICE_CALL(BOOL,11,(PBYTE pBuffer, DWORD *pdwServiceEntries, DWORD *pdwBufferLen))
#define GetServiceHandle           WIN32_SERVICE_CALL(HANDLE,12,(LPWSTR szPrefix, LPWSTR szDllName, DWORD *pdwDllBuf))
#define ServiceClosePort           WIN32_SERVICE_CALL(BOOL,13,(HANDLE hService, SOCKADDR *pSockAddr, int cbSockAddr, int iProtocol, BOOL fRemoveFromRegistry))

#ifdef __cplusplus
}
#endif

#endif	/* __mservice_H__ */

