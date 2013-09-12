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

ppnotify.h

Abstract:

Private portion of notify.h - Header for User Notifications

Notes: 


--*/
#ifndef _PRIV_NOTIFY_H_
#define _PRIV_NOTIFY_H_

#ifdef __cplusplus
extern "C"  {
#endif

BOOL CeEventHasOccurred (LONG lWhichEvent, TCHAR *pwszEndOfCommandLine);
DWORD CeGetNotificationThreadId (void);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* _PRIV_NOTIFY_H_ */
