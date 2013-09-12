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
//------------------------------------------------------------------------------
// 
//      Bluetooth OS abstraction layer
// 
// 
// Module Name:
// 
//      bt_os.h
// 
// Abstract:
// 
//      This file defines compatibility layer between Windows CE and Windows NT
// 
// 
//------------------------------------------------------------------------------
#if ! defined (__bt_os_H__)
#define __bt_os_H__			1

#if ! defined (UNDER_CE)
#define GetCurrentPermissions() 0xffffffff
#define GetOwnerProcess() (HANDLE *)GetCurrentProcessId()
#define SetProcPermissions(c) 0xffffffff
#define MapPtrToProcess(c, h) (c)

#define HKEY_BASE				HKEY_CURRENT_USER
#else
#define HKEY_BASE				HKEY_LOCAL_MACHINE
#endif

#if ! defined (MAX_COMPUTERNAME_LENGTH)
#define MAX_COMPUTERNAME_LENGTH			31
#endif

#if ! defined (CP_UTF8)
#define CP_UTF8	CP_ACP
#endif

#if ! defined (GetComputerName)
#if ! defined (UNDER_CE)
#error This code only supports Windows CE!
#endif

inline BOOL GetComputerName (
    LPWSTR lpBuffer,
    LPDWORD nSize
    ) {
	TCHAR szBuffer[_MAX_PATH];
	DWORD dwType = REG_SZ;
	DWORD dwSize = sizeof(szBuffer);
	HKEY hKey = NULL;
	if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, TEXT("Ident"), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return FALSE;

	if ((RegQueryValueEx (hKey, TEXT("name"), 0, &dwType, (LPBYTE)szBuffer, &dwSize) != ERROR_SUCCESS) ||
		(dwType != REG_SZ) || (dwSize > sizeof(szBuffer))) {
		RegCloseKey (hKey);
		return FALSE;
	}
	RegCloseKey (hKey);

	dwSize = wcslen (szBuffer) + 1;

	if (dwSize > *nSize) {
	    *nSize = dwSize;
	    SetLastError (ERROR_INSUFFICIENT_BUFFER);
	    return FALSE;
	}

    *nSize = dwSize;
	memcpy (lpBuffer, szBuffer, dwSize * sizeof(TCHAR));
	return TRUE;
}

#endif

#if defined (UNDER_CE) && defined (SDK_BUILD)
#include <pkfuncs.h>

#define ERROR_TIMEOUT                    1460L
#define ERROR_NO_SYSTEM_RESOURCES        1450L
#define ERROR_RESOURCE_NOT_AVAILABLE     5006L
#define ERROR_INVALID_OPERATION          4317L
#endif

#endif			//	END OS COMPATIBILITY LAYER
