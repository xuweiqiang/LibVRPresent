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

ppwinreg.h

Abstract:

Private portion of winreg.h

Notes: 


--*/
    
   
    
#ifndef _PRIV_WINREG_H_
#define _PRIV_WINREG_H_

#ifdef __cplusplus
extern "C"  {
#endif

BOOL RegCopyFile(LPCWSTR lpszFile);
BOOL RegRestoreFile(LPCWSTR lpszFile);
LONG RegSaveKey(HKEY hKey, LPCWSTR lpszFile,
                LPSECURITY_ATTRIBUTES lpSecurityAttributes);
LONG RegReplaceKey(HKEY hKey, LPCWSTR lpszSubKey, LPCWSTR lpszNewFile,
                   LPCWSTR lpszOldFile);

// User profile OAK functions
BOOL SetCurrentUser(LPCWSTR lpszUserName, LPBYTE lpbUserData, DWORD dwDataSize,
                    BOOL bCreateIfNew);
BOOL SetUserData(LPBYTE lpbUserData, DWORD dwDataSize);
BOOL GetUserDirectory(LPWSTR lpszBuffer, LPDWORD lpdwSize);

// GetUserInformation flags -- used by GetUserNameEx and GetUserDirectory
#define USERINFO_NAME       1
#define USERINFO_DIRECTORY  2


#define REG_WRITE_BYTES_START 0x00000001
#define REG_WRITE_BYTES_PROBE 0x80000000
#define REG_READ_BYTES_START  0x00000001


// Hive cleanup flags, used with IOCTL_HAL_GET_HIVE_CLEAN_FLAG
#define HIVECLEANFLAG_SYSTEM  1
#define HIVECLEANFLAG_USERS   2


// Settings for [HKLM\init\BootVars]  "RegistryFlags"
#define REGISTRY_FLAG_FLUSHONCLOSE        0x00000001
#define REGISTRY_FLAG_NOBACKGROUNDFLUSH   0x00000002  // cannot be used with FLUSHONCLOSE


// Used with IOCTL_HAL_GET_HIVE_RAM_REGION and IOCTL_HAL_SAVE_HIVE_RAM_REGION
typedef struct HiveRAMRegion {
    LPVOID lpStart;             // Physical address of the start of the region,
                                //   as would be passed to VirtualCopy
    DWORD  dwLength;            // Length of the region, in bytes
    DWORD  dwVirtCopyFlags;     // Flags to pass as fdwProtect parameter to VirtualCopy
} HiveRAMRegion;

typedef struct HiveRAMInfo {
    WORD   wVersion;            // Version of this structure; set to 1
    WORD   wFlags;              // Supported flags:
                                //   HIVE_REGION_COMBINED: System + User in same hive,
                                //                         single-user only (required)
    HiveRAMRegion SystemRegion; // Region to use for system hive
} HiveRAMInfo;

#define HIVE_REGION_COMBINED  1


//------------------------------------------------------------------------------
// Registry Security
//------------------------------------------------------------------------------

// Used with IOCTL_HAL_GETREGSECUREKEYS

typedef struct RegSecureKey {
    WORD   wRoots;  // Bitmask of root hkeys this name is protected under
    WORD   wLen;    // Length of protected name
    LPWSTR pName;   // Protected name
} RegSecureKey;

typedef struct RegSecureKeyList {
    DWORD  dwNumKeys;     // Number of keys in the list
    RegSecureKey *pList;  // List of keys
} RegSecureKeyList;

// Definitions of the bits for each of the registry roots
#define REGSEC_ROOTMASK(hkRoot)  ((WORD)(1 << ((DWORD)(hkRoot) - (DWORD)HKEY_CLASSES_ROOT)))

// Shorthand definitions for clearer code
#define REGSEC_HKCR  REGSEC_ROOTMASK(HKEY_CLASSES_ROOT)     // 0001
#define REGSEC_HKCU  REGSEC_ROOTMASK(HKEY_CURRENT_USER)     // 0002
#define REGSEC_HKLM  REGSEC_ROOTMASK(HKEY_LOCAL_MACHINE)    // 0004
#define REGSEC_HKUS  REGSEC_ROOTMASK(HKEY_USERS)            // 0008

typedef DWORD (*PREADREGDATA) (DWORD dwFlags, LPBYTE pBuf, DWORD len);
typedef BOOL  (*PWRITEREGDATA) (DWORD dwFlags, LPBYTE pBuf, DWORD len);
typedef BOOL  (*PREGISTRYOPERATION)(DWORD dwOp);
typedef BOOL  (*PREADGENERICDATA)(WORD wId, DWORD dwIndex, DWORD dwFlags, LPBYTE pBuf, DWORD dwLength, DWORD *pdwBytesReturned);
typedef BOOL  (*PWRITEGENERICDATA)(WORD dwId, DWORD dwIndex, DWORD dwFlags, LPBYTE pBuf, DWORD dwLength);


#define REGOP_START_FLUSH       1
#define REGOP_END_FLUSH         2
#define REGOP_START_RESTORE     3
#define REGOP_END_RESTORE       4
#define REGOP_DO_IMPORT         5
#define REGOP_START_WRITE		6 // Starting a sequence of WriteGenericData
#define REGOP_END_WRITE			7 // Ending a sequence of WriteGenericData
#define REGOP_START_READ		8 // Starting a sequence of WriteGenericData 
#define REGOP_END_READ			9 // Starting a sequence of ReadGenericData

#define OEM_DATATYPE_PASSWORD	0

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif //_PRIV_WINREG_H_
    
