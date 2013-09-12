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

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <windows.h>

#define NETWORK_EVENT_CALL_IN           0x00
#define NETWORK_EVENT_CALL_OUT          0x01
#define NETWORK_EVENT_CALL_CONNECT      0x02
#define NETWORK_EVENT_CALL_DISCONNECT   0x03
#define NETWORK_EVENT_CALL_REJECT       0x04
#define NETWORK_EVENT_CALL_INFO         0x05
#define NETWORK_EVENT_CALL_BUSY         0x06
#define NETWORK_EVENT_RING              0x07

#define NETWORK_EVENT_FAILED            0xff

typedef enum _NETWORK_CALL_TYPE {
    CALL_TYPE_ANSWER = 0x01,
    CALL_TYPE_DROP = 0x02,
    CALL_TYPE_DIAL = 0x03,
    CALL_TYPE_HOLD = 0x04,
    CALL_TYPE_SWAP = 0x05,
    CALL_TYPE_UNHOLD = 0x06,
} NETWORK_CALL_TYPE;

typedef struct _NetworkCallFailedInfo {
	USHORT usCallType;			// Type of call (see NETWORK_CALL_TYPE enum
	DWORD dwStatus;				// Status of the call
} NetworkCallFailedInfo;

void BthAGOnNetworkEvent(DWORD dwEvent, LPVOID lpvParam, DWORD cbParam);


#endif // __NETWORK_H__

