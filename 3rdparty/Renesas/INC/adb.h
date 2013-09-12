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
//  File:       adb.h
//
//  Contents:   Account Database Prototypes and Definitions
//
//----------------------------------------------------------------------------
#ifndef __ADB_H__
#define __ADB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef DWORD ACCTID;
typedef DWORD PRIVILEGE, *PPRIVILEGE;
#define ISEXTPRIVILEGE(x) ((x) & 0x80000000)

// values for Account Flags
#define ACCT_FLAG_GROUP             0x1
#define ACCT_FLAG_TOPLEVEL          0x2
#define ACCT_FLAG_BUILTIN           0x4

#define ACCT_FLAG_MASK  (ACCT_FLAG_GROUP | ACCT_FLAG_TOPLEVEL | ACCT_FLAG_BUILTIN)

// well known accounts. Can't create accounts with these names
//
#define ACCT_NAME_CREATOR_OWNER     L"Owner"
#define ACCT_NAME_CREATOR_GROUP      L"Group"

 /*  
Description
    Create a new account of the specified type.
Parameters
    pszName
    [in] account name. Must be a valid non-NULL name.
    dwAcctFlags
    [in] Bit mask of one or more of the following account flags:
    ACCT_FLAG_GROUP - group account
    ACCT_FLAG_TOPLEVEL - account cannot be added to a group
    ACCT_FLAG_BUILTIN - account cannot be deleted
    Reserved
    [in] must be zero

Notes:
    caller must be trusted
*/
LONG 
ADBCreateAccount(
    IN LPCWSTR pszName,
    IN DWORD dwAcctFlags,
    IN DWORD Reserved);

 /*
Description
    Delete the named account.
Parameters
pszName
[in] account name. Must be the name of an existing account to be deleted.

Notes:
    caller must be trusted
    associated registry data, if any is also deleted
*/
LONG 
ADBDeleteAccount(
    IN LPCWSTR pszName);

#define ADB_DWORD        0x0000
#define ADB_STRING       0x10000
#define ADB_MSTRING      0x20000
#define ADB_BINARY       0x30000
#define ADBTYPE(propertyId) ((0xf0000 & (propertyId)) >> 16)

#define ADBPROP_ACCTID              (ADB_DWORD | 0x5)
#define ADBPROP_ACCTFLAGS           (ADB_DWORD | 0x6)
#define ADBPROP_PRIVILEGES          (ADB_DWORD |0x7)
#define ADBPROP_EXTPRIVILEGES       (ADB_BINARY | 0x8)
#define ADBPROP_REG_PATH            (ADB_STRING |0x9)
#define ADBPROP_GROUPS              (ADB_MSTRING | 0xa)
#define ADBPROP_GROUPMEMBERS        (ADB_MSTRING |0xb)
#define ADBPROP_NTLM_CREDS          (ADB_BINARY | 0xc)

typedef struct _ADB_BLOB {
    DWORD propertyId;
    VOID    *pData;
    DWORD   cbData;
} ADB_BLOB, *PADB_BLOB;

/*
Description
    Read an account property.
Parameters
    pszName
    [in] name of an existing account to be queried.
    propertyId
    [in] property identifier. See table for valid property identifiers. 
    PropertyId	Description	 Read/Write	Type
    ADBPROP_ACCTID	Account Identifier	R	DWORD
    ADBPROP_FLAGS	Account Flags	R	DWORD
    ADBPROP_PRIVILEGES	Basic Privileges	R/W	DWORD
    ADBPROP_EXTPRIVILEGES	Extended Privileges	R/W	Binary
    ADBPROP_REG_PATH	 Registry Path	R	String
    ADBPROP_GROUPS	Groups 	R	Multi String
    ADBPROP_GROUP_MEMBERS	Members	R	Multi String
    ADBPROP_NTLM_CREDS	NTLM password credentials	R/W	Binary
    pcbPropertyValue
    [in/out] size in bytes of property value buffer on input. Filled with actual size of propertyvalue on output
    pValue
    [out] property value. This buffer must be aligned appropriately for the expected type.
Notes:
    certain properties may require the caller to be trusted
*/
LONG
ADBGetAccountProperty(
    IN PCWSTR pszName,
    IN DWORD propertyId,
    IN OUT PDWORD pcbPropertyValue,
    OUT PVOID pValue);

/*
Description
    Write one or more account properties. See ADBGetAccountProperty for a list of possible property Ids. 
    Only those that are R/W may be set. No change is made to the account if there as an error writing any property.
Parameters
    pszName
    [in] name of an existing account to be updated.
    cProperties
    [in] number of properties to be set
    rgProperties
    [in] array of ADB_BLOB structures, each describing one property Id and the corresponding value.
Notes:
    Most properties require the caller to be trusted.
     
*/
LONG
ADBSetAccountProperties(
    IN PCWSTR pszName,
    IN DWORD cProperties,
    IN ADB_BLOB *rgProperties    
);

/*
    This structure is used with ADBGetAccountSecurityInfo, to get Groups and Privileges for all groups that the account is
    directly or indirectly a member of.
*/
typedef struct _ADB_SECURITY_INFO
{
    DWORD version;	// must be 1
    DWORD accountFlags;	// account flags
    // the following are effective groups and privileges
    // obtained by expanding the groups that the account belongs to
    DWORD cAllGroups;
    PWSTR pmszAllGroups;
    PRIVILEGE allBasicPrivileges;
    DWORD cAllExtPrivileges;
    PRIVILEGE *pAllExtPrivileges;
} ADB_SECURITY_INFO;

/*
Description
    Get Groups and Privileges for all groups that the account is directly or indirectly a member of. 
Parameters
    pszName
    [in] name of account to be queried.
    pSecurityInfo
    [out] pointer to a buffer to receive the response. The function returns a ADB_SECURITY_INFO struct in this buffer, followed by data referenced by the structure.
    pcbBuf
    [in/out]  size of pSecurityInfo buffer on input. Returns the size used (or required) .
Notes:
// Do we need this function or is it enough to obtain
// this information through GetAccountProperty() ?

*/
LONG ADBGetAccountSecurityInfo(
    IN PCWSTR pszName,
    IN OUT PDWORD pcbBuf,
    OUT ADB_SECURITY_INFO *pSecurityInfo
    );

/*
Description
    Add an account to a group. 
Parameters
    pszName
    [in] name of account to be added to group.
    pszGroupName
    [in] name of Group.
*/
LONG ADBAddAccountToGroup(
    IN PCWSTR pszName,
    IN PCWSTR pszGroupName);

/*
Description
    Remove an account from a group. 
Parameters
    pszName
    [in] name of account to be removed from group.
    pszGroupName
    [in] name of Group.
*/
LONG ADBRemoveAccountFromGroup(
    IN PCWSTR pszName,
    IN PCWSTR pszGroupName);

/*
Description
    Enumerate all accounts in database. If there are accounts added or removed while the enumeration is in progress, 
    the changes may not be reported.
Parameters
    dwFlags
    [in] must be zero.
    pszName
    [in] name of previously enumerated name. Set to NULL when starting enumeration.
    pchNextName
    [in/out] length of pszNextName buffer on input. Returns the number of characters in the name (including trailing NULL).
    pszNextName
    [out] next enumerated name.
*/
LONG ADBEnumAccounts(
    IN DWORD dwFlags,
    IN PCWSTR pszPrevName,
    IN OUT PDWORD pchNextName,
    OUT PWSTR pszNextName);

// function pointer typedefs for above functions, in case they are dynamically loaded
typedef LONG (* PFNADBCreateAccount)(IN LPCWSTR pszName, IN DWORD dwAcctFlags, IN DWORD Reserved);
typedef LONG (* PFNADBDeleteAccount)(IN LPCWSTR pszName);
typedef LONG (* PFNADBGetAccountProperty)(IN PCWSTR pszName, IN DWORD propertyId, IN OUT PDWORD pcbPropertyValue, OUT PVOID pValue);
typedef LONG (* PFNADBSetAccountProperties)(IN PCWSTR pszName, IN DWORD cProperties, IN ADB_BLOB * rgProperties);
typedef LONG (* PFNADBGetAccountSecurityInfo)(IN PCWSTR pszName, IN OUT PDWORD pcbBuf, OUT ADB_SECURITY_INFO * pSecurityInfo);
typedef LONG (* PFNADBAddAccountToGroup)(IN PCWSTR pszName, IN PCWSTR pszGroupName);
typedef LONG (* PFNADBRemoveAccountFromGroup)(IN PCWSTR pszName, IN PCWSTR pszGroupName);
typedef LONG (* PFNADBEnumAccounts)(IN DWORD dwFlags, IN PCWSTR pszPrevName, IN OUT PDWORD pchNextName, OUT PWSTR pszNextName);


#ifdef __cplusplus
}
#endif

#endif // __ADB_H__


