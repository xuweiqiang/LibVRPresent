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
#ifndef _CRED_PROV_H

#define _CRED_PROV_H

#ifdef __cplusplus
extern "C" {
#endif

// Registry keys

#define REG_CRED_PRI_TYPE_PROV_ROOT 	L"comm\\security\\credman\\types\\primitive"
#define REG_CRED_VIR_TYPE_PROV_ROOT 	L"comm\\security\\credman\\types\\virtual"
#define REG_CRED_PRI_TYPE_FLAGS 		L"flags"
#define REG_CRED_PRI_TYPE_DLL			L"dll"
#define REG_CRED_VIR_TYPE_IDS			L"ids"

// Enums

typedef enum {
	PROV_TYPE_INVALID = 0,
	PROV_TYPE_PRIMITIVE,
	PROV_TYPE_VIRTUAL
}PROV_TYPE;

// Type Flags

#define CRED_TYPE_FLAG_UNIQUE_USER			0x00000001
#define CRED_TYPE_FLAG_NO_BLOB_HANDLING		0x00000002
#define CRED_TYPE_FLAG_NO_USER_MATCHING		0x00000004
#define CRED_TYPE_FLAG_NO_TARGET_MATCHING	0x00000008
#define CRED_TYPE_FLAG_TRUSTED 				0x00000010

// Functions

#define HANDLE_BLOB_FUNC 	L"HandleBlob"
#define MATCH_USER_FUNC	L"MatchUser"
#define MATCH_TARGET_FUNC	L"MatchTarget"
#define LOAD_FUNC 			L"Load"
#define UNLOAD_FUNC 		L"UnLoad"

typedef
DWORD
(*F_HandleBlob)(
	IN		DWORD			dwType,
	IN		PWCHAR			szTarget,
	IN		PWCHAR			szUser,
	IN 		PBYTE			pInBlob,
	IN 		DWORD			dwInSize,
	IN OUT	PBYTE			pOutBlob,
	IN OUT	PDWORD			pdwOutSize,
	IN		DWORD			dwFlag
);

typedef
DWORD
(*F_MatchTarget)(
	IN		DWORD			dwType,
	IN		PWSTR			wszSrc,
	IN		PWSTR			wszDest,
	IN		DWORD			dwFlag,
	OUT		PBOOL 			pbMatch
);

typedef
DWORD
(*F_MatchUser)(
	IN		DWORD			dwType,
	IN		PWSTR			wszSrc,
	IN		PWSTR			wszDest,
	IN		DWORD			dwFlag,
	OUT 	PBOOL 			pbMatch
);	

typedef
DWORD
(*F_Load)(
	IN 		DWORD 			dwType,
	IN 		DWORD 			dwFlags
);

typedef
DWORD
(*F_UnLoad)(
	IN 		DWORD 			dwType,
	IN 		DWORD 			dwFlags
);

// Function Table

typedef struct {
	F_HandleBlob 		pfHandleBlob;
	F_MatchTarget 	pfMatchTarget;
	F_MatchUser		pfMatchUser;
	F_Load 			pfLoad;
	F_UnLoad 		pfUnLoad;
	
}ProvFuncTable;

// Functions
DWORD
HandleBlob(
	IN		DWORD			dwType,
	IN		PWCHAR			szTarget,
	IN		PWCHAR			szUser,
	IN 		PBYTE			pInBlob,
	IN 		DWORD			dwInSize,
	IN OUT	PBYTE			pOutBlob,
	IN OUT	PDWORD			pdwOutSize,
	IN		DWORD			dwFlag
);

DWORD
MatchTarget(
	IN		DWORD			dwType,
	IN		PWSTR			wszSrc,
	IN		PWSTR			wszDest,
	IN		DWORD			dwFlag,
	OUT 	PBOOL			pbMatch
);

DWORD
MatchUser(
	IN		DWORD			dwType,
	IN		PWSTR			wszSrc,
	IN		PWSTR			wszDest,
	IN		DWORD			dwFlag,
	OUT 	PBOOL 			pbMatch
);

DWORD
Load(
	IN 		DWORD 			dwType,
	IN 		DWORD 			dwFlags
);

DWORD
UnLoad(
	IN 		DWORD 			dwType,
	IN 		DWORD 			dwFlags
);

#ifdef __cplusplus
}
#endif

#endif

