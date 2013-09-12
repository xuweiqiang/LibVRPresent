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
//+-------------------------------------------------------------------------
//  Microsoft Windows CE
//
//
//  File:       ntlmssp.h
//
//  Contents:   NTLM User Management Routines
//
//
//----------------------------------------------------------------------------

#ifndef __NTLMSSP_H__
#define __NTLMSSP_H__

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------------------
// COMMON ERROR CODES
//
//	ERROR_SUCCESS - operation success
// 	ERROR_INTERNAL_ERROR - non-specific error
//	ERROR_INVALID_PARAMETER - arguments are invalid
// 	ERROR_OUTOFMEMORY - no more memory
//	TRUE - operation success
//	FALSE - operation failed

// ------------------------------------------------
// DESCRIPTION
// 	Set the user information
// ARGS
// 	IN pszUser - username in unicode format
//	IN pszPassword - password in unicode format

BOOL NTLMSetUserInfo(
IN LPTSTR pszUser,
IN LPTSTR pszPassword
);

// ------------------------------------------------
// DESCRIPTION
// 	Enumerate users
// ARGS
//	Function args similar in usage to RegEnumKeyEx
// 	IN index - start from zero, increment until returns FALSE
//	IN OUT pszUser - buff to hold username in unicode, can be NULL
//	IN OUT pcchUser - number of characters in username

BOOL NTLMEnumUser(
IN DWORD index,
OUT LPTSTR pszUser,
IN OUT LPDWORD pcchUser 
);


// ------------------------------------------------
// DESCRIPTION
// 	Delete the user
// ARGS
// 	IN pszUser - username in unicode format

BOOL NTLMDeleteUser(
IN LPTSTR pszUser
);


// ------------------------------------------------
// DESCRIPTION
// 	Add a new ntlm group
// ARGS
// 	IN groupName - name of group, case insensitive
// SETLASTERROR CODES
//	ERROR_ALREADY_EXISTS - group already exists

BOOL NTLMAddGroup(IN WCHAR *groupName);


// ------------------------------------------------
// DESCRIPTION
// 	Remove existing ntlm group
// ARGS
// 	IN groupName - name of group, case insensitive
// SETLASTERROR CODES
// 	ERROR_NOT_FOUND - group not found

BOOL NTLMRemoveGroup(IN WCHAR *groupName);


// ------------------------------------------------
// DESCRIPTION
//	Enumerate groups 
// ARGS
//	IN index - start with zero and increment until ERROR_NO_MORE_DATA
//	IN OUT groupName - buffer to hold the group name, can be NULL to figure out alloc size
//	IN OUT dwSizeInChars - size of buffer in characters, on output required/actual size
// SETLASTERROR CODES
// 	ERROR_NO_MORE_DATA - reached end of enumeration
//	ERROR_MORE_DATA - buffer not big enough to hold all information

BOOL NTLMEnumGroups( IN DWORD index, IN OUT WCHAR *groupName, IN OUT DWORD *dwSizeInChars);


// ------------------------------------------------
// DESCRIPTION
//	Add an existing user to an existing group
// ARGS
//	IN groupName - name of group, case insensitive
//	IN userName - name of user, case insensitive
// SETLASTERROR CODES
// 	ERROR_NOT_FOUND - group not found

BOOL NTLMAddUserToGroup(IN WCHAR *groupName, IN WCHAR *userName);


// ------------------------------------------------
// DESCRIPTION
// 	Remove user from a group
// ARGS
//	IN groupName - name of group, case insensitive
//	IN userName - name of user, case insensitive
// SETLASTERROR CODES
//	ERROR_NOT_FOUND - user/group not found

BOOL NTLMRemoveUserFromGroup(IN WCHAR *groupName, IN WCHAR *userName);


// ------------------------------------------------
// DESCRIPTION
//	Get list of users belonging to group
// ARGS
//	IN groupName - group name
//	IN userList - buffer to hold the user list as a multi-string, can be NULL to figure out actual size
//	IN OUT dwSizeInChars - size of buffer in characters, on output required/actual size
// SETLASTERROR CODES
// 	ERROR_NOT_FOUND -group not found 
//	ERROR_MORE_DATA - buffer not big enough to hold all information

BOOL NTLMGetUserList(IN WCHAR *groupName, IN OUT WCHAR *userList, IN OUT DWORD *pdwccLen);


// ------------------------------------------------
// DESCRIPTION
//	Get list of groups to which user belongs
// ARGS
//	IN userName - group name
//	IN groupList - buffer to hold the group list as a multi-string, can be NULL to figure out actual size
//	IN OUT dwSizeInChars - size of buffer in characters, on output required/actual size
// SETLASTERROR CODES
//	ERROR_MORE_DATA - buffer not big enough to hold all information

BOOL NTLMGetGroupList(IN WCHAR *userName, IN OUT WCHAR *groupList, IN OUT DWORD *pdwccLen);


#ifdef __cplusplus
}
#endif

#endif

