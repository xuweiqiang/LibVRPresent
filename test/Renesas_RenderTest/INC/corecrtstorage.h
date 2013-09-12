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
#ifndef __CORE_CRT_STORAGE_H_INCLUDED__
#define __CORE_CRT_STORAGE_H_INCLUDED__

#include <windows.h>


#ifdef __cplusplus
extern "C" {
#endif

#define CVTBUFSIZE (309+40) /* # of digits in max. dp value + extra */

#define CRTF_IMM_DISABLED	0x0001
#define CRTF_IMM_ENABLED	0x0002

typedef struct {
    PVOID   CurrentExcept;
    PVOID   ExceptContext;
    PVOID   TargetEntry;
    PVOID   CatchExceptRN;
    DWORD   ExceptState;
#if !defined(_M_IX86)
    PVOID   FrameInfoChain;
#endif
#if defined(_M_ARM)
    PVOID   CatchChain;
#endif
    PVOID   CurrExceptRethrow;
    int     _ProcessingThrow; /* for uncaught_exception */
} crtEHData;

typedef struct crtGlobTag_t {
    DWORD     dwFlags;
    char      fpcvtbuf[CVTBUFSIZE];
    long      rand;
    char      *nexttoken;
    DWORD     dwHimcDefault;
    DWORD     dwHwndDefaultIme;
    crtEHData EHData;
    wchar_t   *wnexttoken;
} crtGlob_t;

crtGlob_t*	GetCRTStorage(void);
crtGlob_t*	GetCRTStorageEx(BOOL bCreateIfNecessary);

DWORD		GetCRTFlags(void);
void		SetCRTFlag(DWORD dwFlag);
void		ClearCRTFlag(DWORD dwFlag);

#ifdef __cplusplus
}
#endif

#endif

