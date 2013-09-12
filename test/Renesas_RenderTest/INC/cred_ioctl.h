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
#ifndef _CRED_IOCTL_H

#define _CRED_IOCTL_H

#ifdef __cplusplus
extern "C" {
#endif

#define CREDSVC_HANDLE_NAME 			L"CRD0:"

#define IOCTL_CREDSVC_BASE 			0

#define IOCTL_CREDSVC(x) 				((x) + IOCTL_CREDSVC_BASE)

#define IOCTL_CREDSVC_REFRESH 		IOCTL_CREDSVC(1)
#define IOCTL_CREDSVC_WRITE			IOCTL_CREDSVC(2)
#define IOCTL_CREDSVC_READ			IOCTL_CREDSVC(3)
#define IOCTL_CREDSVC_UPDATE			IOCTL_CREDSVC(4)
#define IOCTL_CREDSVC_DELETE 			IOCTL_CREDSVC(5)
#define IOCTL_CREDSVC_LIST_PROVIDERS	IOCTL_CREDSVC(6)
#define IOCTL_CREDSVC_DELETE_ALL		IOCTL_CREDSVC(7)
#define IOCTL_CREDSVC_LIST_CREDS		IOCTL_CREDSVC(8)

typedef struct {
	PCRED 		pCred;
	DWORD		dwFlags;
}I_CRED_WRITE, *PI_CRED_WRITE;

typedef struct {
 	PWCHAR		wszTarget;
	DWORD 		dwTargetLen;
 	DWORD		dwType;
 	DWORD		dwFlags;
	PCRED		pCred;
	PDWORD		pdwCredSize;
}I_CRED_READ, *PI_CRED_READ;

typedef struct {
	PWCHAR		wszTarget;
	DWORD 		dwTargetLen;
 	DWORD 		dwType;
	PCRED		pCred;
	DWORD		dwFlags;
}I_CRED_UPDATE, *PI_CRED_UPDATE;

typedef struct {
	PWCHAR		wszTarget;
	DWORD 		dwTargetLen;
	DWORD		dwType;
	DWORD		dwFlags;
}I_CRED_DELETE, *PI_CRED_DELETE;

#ifdef __cplusplus
}
#endif

#endif 

