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

mwindbas.h

Abstract:

private portion of windbase.h

Notes: 


--*/
   
#ifndef _MACRO_WINDBASE_H_
#define _MACRO_WINDBASE_H_

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifndef WINCEMACRO
#error WINCEMACRO not defined __FILE__
#endif

#ifdef WIN32_FS_CALL
// SDK functions
#define CeOidGetInfoEx2       	+++ Do Not Use +++
#define CeOidGetInfoEx2_Trap       WIN32_FS_CALL(BOOL,   12, (PCEGUID pguid, CEOID oid, CEOIDINFOEX *oidInfo))

#define CeFindFirstDatabaseEx	+++ Do Not Use +++
#define CeFindFirstDatabaseEx_Trap WIN32_FS_CALL(HANDLE, 13, (PCEGUID pguid, DWORD dwClassID))

#define CeCreateDatabaseEx2         +++ Do Not Use +++
#define CeCreateDatabaseEx2_Trap   WIN32_FS_CALL(CEOID,  14, (PCEGUID pguid, CEDBASEINFOEX *pInfo))

#define CeSetDatabaseInfoEx2         +++ Do Not Use +++
#define CeSetDatabaseInfoEx2_Trap  WIN32_FS_CALL(BOOL,   15, (PCEGUID pguid, CEOID oidDbase, CEDBASEINFOEX *pNewInfo))

#define CeOpenDatabaseEx2         +++ Do Not Use +++
#define CeOpenDatabaseEx2_Trap     WIN32_FS_CALL(HANDLE, 16, (PCEGUID pguid, PCEOID poid, LPWSTR lpszName, SORTORDERSPECEX* pSort, DWORD dwFlags, CENOTIFYREQUEST *pRequest))

#define CeDeleteDatabaseEx         +++ Do Not Use +++
#define CeDeleteDatabaseEx_Trap    WIN32_FS_CALL(BOOL,   28, (PCEGUID pguid, CEOID oid))

#define CeMountDBVol               +++ Do Not Use +++
#define CeMountDBVol_Trap          WIN32_FS_CALL(BOOL,   58, (PCEGUID pguid, LPWSTR lpszVol, DWORD dwFlags))

#define CeUnmountDBVol             +++ Do Not Use +++
#define CeUnmountDBVol_Trap        WIN32_FS_CALL(BOOL,   60, (PCEGUID pguid))

#define CeFlushDBVol               +++ Do Not Use +++
#define CeFlushDBVol_Trap          WIN32_FS_CALL(BOOL,   61, (PCEGUID pguid))

#define CeEnumDBVolumes            +++ Do Not Use +++
#define CeEnumDBVolumes_Trap       WIN32_FS_CALL(BOOL,   59, (PCEGUID pguid, LPWSTR lpBuf, DWORD dwSize))

#define GetStoreInformation         +++ Do Not Use +++
#define GetStoreInformation_Trap   WIN32_FS_CALL(BOOL,   32, (LPSTORE_INFORMATION lpsi))

#define CeFreeNotification         +++ Do Not Use +++
#define CeFreeNotification_Trap    WIN32_FS_CALL(BOOL,   62, (PCENOTIFYREQUEST pRequest, PCENOTIFICATION pNotify))

#define CeFindNextDatabaseEx         +++ Do Not Use +++
#define CeFindNextDatabaseEx_Trap    WIN32_DBFIND_CALL(CEOID, 2, (HANDLE hEnum, PCEGUID pguid))

#define CeSeekDatabaseEx         +++ Do Not Use +++
#define CeSeekDatabaseEx_Trap           WIN32_DBFILE_CALL(CEOID, 2, (HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, WORD wNumVals, LPDWORD lpdwIndex))

#define CeDeleteRecord           +++ Do Not Use +++
#define CeDeleteRecord_Trap             WIN32_DBFILE_CALL(BOOL,  3, (HANDLE hDatabase, CEOID oidRecord))

#define CeReadRecordPropsEx         +++ Do Not Use +++
#define CeReadRecordPropsEx_Trap        WIN32_DBFILE_CALL(CEOID, 4, (HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID, CEPROPID *rgPropID, LPBYTE *lplpBuffer, LPDWORD lpcbBuffer, HANDLE hHeap))

#define CeWriteRecordProps          +++ Do Not Use +++
#define CeWriteRecordProps_Trap         WIN32_DBFILE_CALL(CEOID, 5, (HANDLE hDbase, CEOID oidRecord, WORD cPropID, CEPROPVAL *rgPropVal))

#define CeGetDBInformationByHandle         +++ Do Not Use +++
#define CeGetDBInformationByHandle_Trap WIN32_DBFILE_CALL(BOOL,  6, (HANDLE hDatabase, LPBY_HANDLE_DB_INFORMATION lpDBInfo))

// OAK functions  
#define CeGetReplChangeMask         +++ Do Not Use +++
#define CeGetReplChangeMask_Trap     WIN32_FS_CALL(BOOL, 33, (LPDWORD lpmask))

#define CeSetReplChangeMask         +++ Do Not Use +++
#define CeSetReplChangeMask_Trap     WIN32_FS_CALL(BOOL, 34, (DWORD mask))

#define CeGetReplChangeBitsEx         +++ Do Not Use +++
#define CeGetReplChangeBitsEx_Trap   WIN32_FS_CALL(BOOL, 35, (PCEGUID pguid, CEOID oid, LPDWORD lpbits, DWORD dwFlags))

#define CeSetReplChangeBitsEx         +++ Do Not Use +++
#define CeSetReplChangeBitsEx_Trap   WIN32_FS_CALL(BOOL, 51, (PCEGUID pguid, CEOID oid, DWORD mask))

#define CeClearReplChangeBitsEx         +++ Do Not Use +++
#define CeClearReplChangeBitsEx_Trap WIN32_FS_CALL(BOOL, 36, (PCEGUID pguid, CEOID oid, DWORD mask))

#define CeGetReplOtherBitsEx            +++ Do Not Use +++
#define CeGetReplOtherBitsEx_Trap    WIN32_FS_CALL(BOOL, 37, (PCEGUID pguid, CEOID oid, LPDWORD lpbits))

#define CeSetReplOtherBitsEx            +++ Do Not Use +++
#define CeSetReplOtherBitsEx_Trap    WIN32_FS_CALL(BOOL, 38, (PCEGUID pguid, CEOID oid, DWORD bits))

#define ReplOpenSync                    +++ Do Not Use +++
#define ReplOpenSync_Trap            WIN32_FS_CALL(BOOL, 72, (CEGUID* pguid, LPBYTE lpSyncState, DWORD dwSize, DWORD dwSyncFlags, HREPL* phRepl))

#define ReplCheckpoint                  +++ Do Not Use +++
#define ReplCheckpoint_Trap          WIN32_FS_CALL(BOOL, 73, (HREPL hRepl, HANDLE hChangeEvent))

#define ReplCloseSync                   +++ Do Not Use +++
#define ReplCloseSync_Trap           WIN32_FS_CALL(BOOL, 74, (HREPL hRepl))

#define ReplGetSyncState                +++ Do Not Use +++
#define ReplGetSyncState_Trap        WIN32_FS_CALL(BOOL, 75, (HREPL hRepl, LPBYTE lpSyncState, DWORD* lpdwSize, BOOL fFailIfUnchanged))

#define ReplChangeSyncSettings          +++ Do Not Use +++
#define ReplChangeSyncSettings_Trap  WIN32_FS_CALL(BOOL, 76, (HREPL hRepl, DWORD dwSyncFlags, DWORD* pOldFlags))

#define ReplFindNextChange              +++ Do Not Use +++
#define ReplFindNextChange_Trap      WIN32_FS_CALL(BOOL, 77, (HREPL hRepl, REPLCHANGEINFO* pReplInfo, BOOL fRestart))

#define ReplGetOidStatus                +++ Do Not Use +++
#define ReplGetOidStatus_Trap        WIN32_FS_CALL(BOOL, 78, (HREPL hRepl, CEOID oid, REPLCHANGEINFO* pReplInfo))

#define CeChangeDatabaseLCID            +++ Do Not Use +++
#define CeChangeDatabaseLCID_Trap WIN32_FS_CALL(VOID, 56, (PCEGUID pguid, DWORD LCID))

#define CeRegisterFileSystemNotification         +++ Do Not Use +++
#define CeRegisterFileSystemNotification_Trap WIN32_FS_CALL(BOOL, 10, (HWND hWnd))

#define CeRegisterReplNotification               +++ Do Not Use +++
#define CeRegisterReplNotification_Trap WIN32_FS_CALL(BOOL, 11, (CENOTIFYREQUEST *pRequest))

#define RegisterAFSEx                            +++ Do Not Use +++
#define RegisterAFSEx_Trap WIN32_FS_CALL (BOOL, 45, (int index, HANDLE h, DWORD dw, DWORD version, DWORD flags))

#define DeregisterAFS                            +++ Do Not Use +++
#define DeregisterAFS_Trap WIN32_FS_CALL (BOOL, 46, (int index))

#define GetSystemMemoryDivision                  +++ Do Not Use +++
#define GetSystemMemoryDivision_Trap WIN32_FS_CALL(BOOL, 39, (LPDWORD lpdwStorePages, LPDWORD lpdwRamPages, LPDWORD lpdwPageSize))

#define SetSystemMemoryDivision                  +++ Do Not Use +++
#define SetSystemMemoryDivision_Trap WIN32_FS_CALL(DWORD, 40, (DWORD dwStorePages))

#define RegisterAFSName                          +++ Do Not Use +++
#define RegisterAFSName_Trap WIN32_FS_CALL(int,52,(LPCWSTR pName))

#define DeregisterAFSName                        +++ Do Not Use +++
#define DeregisterAFSName_Trap WIN32_FS_CALL(BOOL,53,(int index))

#define DumpFileSystemHeap                       +++ Do Not Use +++
#define DumpFileSystemHeap_Trap WIN32_FS_CALL(void,57,(void))

#elif defined(COREDLL)

BOOL xxx_CeGetDBInformationByHandle();
#define CeGetDBInformationByHandle xxx_CeGetDBInformationByHandle
BOOL xxx_CeOidGetInfoEx2 (PCEGUID pguid, CEOID oid, CEOIDINFOEX *oidInfo);
#define CeOidGetInfoEx2 xxx_CeOidGetInfoEx2
HANDLE xxx_CeFindFirstDatabaseEx (PCEGUID pguid, DWORD dwClassID);
#define CeFindFirstDatabaseEx xxx_CeFindFirstDatabaseEx
CEOID xxx_CeCreateDatabaseEx2 (PCEGUID pguid, CEDBASEINFOEX *pInfo);
#define CeCreateDatabaseEx2 xxx_CeCreateDatabaseEx2
BOOL xxx_CeSetDatabaseInfoEx2 (PCEGUID pguid, CEOID oidDbase, CEDBASEINFOEX *pNewInfo);
#define CeSetDatabaseInfoEx2 xxx_CeSetDatabaseInfoEx2
HANDLE xxx_CeOpenDatabaseEx2 (PCEGUID pguid, PCEOID poid, LPWSTR lpszName, SORTORDERSPECEX* pSort, DWORD dwFlags, CENOTIFYREQUEST *pRequest);
#define CeOpenDatabaseEx2 xxx_CeOpenDatabaseEx2
BOOL xxx_CeDeleteDatabaseEx (PCEGUID pguid, CEOID oid);
#define CeDeleteDatabaseEx xxx_CeDeleteDatabaseEx
BOOL xxx_CeMountDBVol (PCEGUID pguid, LPWSTR lpszVol, DWORD dwFlags);
#define CeMountDBVol xxx_CeMountDBVol
BOOL xxx_CeUnmountDBVol (PCEGUID pguid);
#define CeUnmountDBVol xxx_CeUnmountDBVol
BOOL xxx_CeFlushDBVol (PCEGUID pguid);
#define CeFlushDBVol xxx_CeFlushDBVol
BOOL xxx_CeEnumDBVolumes (PCEGUID pguid, LPWSTR lpBuf, DWORD dwSize);
#define CeEnumDBVolumes xxx_CeEnumDBVolumes
BOOL xxx_GetStoreInformation (LPSTORE_INFORMATION lpsi);
#define GetStoreInformation xxx_GetStoreInformation
BOOL xxx_CeFreeNotification (PCENOTIFYREQUEST pRequest, PCENOTIFICATION pNotify);
#define CeFreeNotification xxx_CeFreeNotification
CEOID xxx_CeFindNextDatabaseEx(HANDLE hEnum, PCEGUID pguid);
#define CeFindNextDatabaseEx xxx_CeFindNextDatabaseEx
CEOID xxx_CeSeekDatabaseEx(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, WORD wNumVals, LPDWORD lpdwIndex);
#define CeSeekDatabaseEx xxx_CeSeekDatabaseEx
BOOL xxx_CeDeleteRecord(HANDLE hDatabase, CEOID oidRecord);
#define CeDeleteRecord xxx_CeDeleteRecord
CEOID xxx_CeReadRecordPropsEx(HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID, CEPROPID *rgPropID, LPBYTE *lplpBuffer, LPDWORD lpcbBuffer, HANDLE hHeap);
#define CeReadRecordPropsEx xxx_CeReadRecordPropsEx
CEOID xxx_CeWriteRecordProps(HANDLE hDbase, CEOID oidRecord, WORD cPropID, CEPROPVAL *rgPropVal);
#define CeWriteRecordProps xxx_CeWriteRecordProps

BOOL xxx_CeGetReplChangeMask (LPDWORD lpmask);
#define CeGetReplChangeMask xxx_CeGetReplChangeMask
BOOL xxx_CeSetReplChangeMask (DWORD mask);
#define CeSetReplChangeMask xxx_CeSetReplChangeMask
BOOL xxx_CeGetReplChangeBitsEx (PCEGUID pguid, CEOID oid, LPDWORD lpbits, DWORD dwFlags);
#define CeGetReplChangeBitsEx xxx_CeGetReplChangeBitsEx
BOOL xxx_CeSetReplChangeBitsEx (PCEGUID pguid, CEOID oid, DWORD mask);
#define CeSetReplChangeBitsEx xxx_CeSetReplChangeBitsEx
BOOL xxx_CeClearReplChangeBitsEx (PCEGUID pguid, CEOID oid, DWORD mask);
#define CeClearReplChangeBitsEx xxx_CeClearReplChangeBitsEx
BOOL xxx_CeGetReplOtherBitsEx (PCEGUID pguid, CEOID oid, LPDWORD lpbits);
#define CeGetReplOtherBitsEx xxx_CeGetReplOtherBitsEx
BOOL xxx_CeSetReplOtherBitsEx (PCEGUID pguid, CEOID oid, DWORD bits);
#define CeSetReplOtherBitsEx xxx_CeSetReplOtherBitsEx

BOOL xxx_ReplOpenSync(CEGUID* pguid, LPBYTE lpSyncState, DWORD dwSize, DWORD dwSyncFlags, HREPL* phRepl);
#define ReplOpenSync xxx_ReplOpenSync
BOOL xxx_ReplCheckpoint(HREPL hRepl, HANDLE hChangeEvent);
#define ReplCheckpoint xxx_ReplCheckpoint
BOOL xxx_ReplCloseSync(HREPL hRepl);
#define ReplCloseSync xxx_ReplCloseSync
BOOL xxx_ReplGetSyncState(HREPL hRepl, LPBYTE lpSyncState, DWORD* lpdwSize, BOOL fFailIfUnchanged);
#define ReplGetSyncState xxx_ReplGetSyncState
BOOL xxx_ReplChangeSyncSettings(HREPL hRepl, DWORD dwSyncFlags, DWORD* pOldFlags);
#define ReplChangeSyncSettings xxx_ReplChangeSyncSettings
BOOL xxx_ReplFindNextChange(HREPL hRepl, REPLCHANGEINFO* pReplInfo, BOOL fRestart);
#define ReplFindNextChange xxx_ReplFindNextChange
BOOL xxx_ReplGetOidStatus(HREPL hRepl, CEOID oid, REPLCHANGEINFO* pReplInfo);
#define ReplGetOidStatus xxx_ReplGetOidStatus

VOID xxx_CeChangeDatabaseLCID (PCEGUID pguid, DWORD LCID);
#define CeChangeDatabaseLCID xxx_CeChangeDatabaseLCID
BOOL xxx_CeRegisterFileSystemNotification (HWND hWnd);
#define CeRegisterFileSystemNotification xxx_CeRegisterFileSystemNotification
BOOL xxx_CeRegisterReplNotification (CENOTIFYREQUEST *pRequest);
#define CeRegisterReplNotification xxx_CeRegisterReplNotification
BOOL xxx_RegisterAFSEx (int index, HANDLE h, DWORD dw, DWORD version, DWORD flags);
#define RegisterAFSEx xxx_RegisterAFSEx
BOOL xxx_DeregisterAFS (int index);
#define DeregisterAFS xxx_DeregisterAFS
BOOL xxx_GetSystemMemoryDivision (LPDWORD lpdwStorePages, LPDWORD lpdwRamPages, LPDWORD lpdwPageSize);
#define GetSystemMemoryDivision xxx_GetSystemMemoryDivision
DWORD xxx_SetSystemMemoryDivision (DWORD dwStorePages);
#define SetSystemMemoryDivision xxx_SetSystemMemoryDivision
int xxx_RegisterAFSName (LPCWSTR pName);
#define RegisterAFSName xxx_RegisterAFSName
BOOL xxx_DeregisterAFSName (int index);
#define DeregisterAFSName xxx_DeregisterAFSName
void xxx_DumpFileSystemHeap (void);
#define DumpFileSystemHeap xxx_DumpFileSystemHeap
		 
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_MACRO_WINDBASE_H_

