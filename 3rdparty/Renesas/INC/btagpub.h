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

#ifndef __BTAGPUB_H__
#define __BTAGPUB_H__

#include <windows.h>

#include <winsock2.h>
#include <ws2bth.h>


#define IOCTL_AG_OPEN_AUDIO         0x01
#define IOCTL_AG_CLOSE_AUDIO        0x02
#define IOCTL_AG_CLOSE_CONTROL      0x03
#define IOCTL_AG_SET_SPEAKER_VOL    0x04
#define IOCTL_AG_SET_MIC_VOL        0x05
#define IOCTL_AG_GET_SPEAKER_VOL    0x06
#define IOCTL_AG_GET_MIC_VOL        0x07
#define IOCTL_AG_GET_POWER_MODE     0x08
#define IOCTL_AG_SET_POWER_MODE     0x09
#define IOCTL_AG_OPEN_CONTROL       0x0A

#define MAX_SEND_BUF                256
#define MAX_PHONE_NUMBER            128
#define MAX_DISPLAY_NAME            128

#define RK_AUDIO_GATEWAY            _T("SOFTWARE\\Microsoft\\Bluetooth\\AudioGateway")
#define RK_AUDIO_GATEWAY_DEVICES    (RK_AUDIO_GATEWAY TEXT("\\Devices"))
#define RK_AG_SERVICE               _T("Services\\BTAGSVC")

#define NETWORK_FLAGS_DROP_ACTIVE   0x01
#define NETWORK_FLAGS_DROP_HOLD     0x02
#define NETWORK_FLAGS_DROP_ALL      0x03

#define NETWORK_FLAGS_STATE_ACTIVE      0x01
#define NETWORK_FLAGS_STATE_HOLD        0x02
#define NETWORK_FLAGS_STATE_OFFERING    0x04

#define AG_PHONE_EVENT_VOICE_RECOG		0x01
#define AG_PHONE_EVENT_SPEAKER_VOLUME	0x02
#define AG_PHONE_EVENT_MIC_VOLUME		0x03
#define AG_PHONE_EVENT_BT_CTRL			0x04
#define AG_PHONE_EVENT_BT_AUDIO			0x05


typedef void (*PFN_PhoneExtServiceCallback) (BOOL fHaveService);
typedef DWORD (*PFN_SendATCommand) (LPSTR szCommand, DWORD cbCommand);
typedef DWORD (*PFN_BthAGOnVoiceTag) (BOOL fOn);
   

DWORD BthAGNetworkInit(HINSTANCE hInstance);
void BthAGNetworkDeinit(void);
DWORD BthAGNetworkDialNumber(LPWSTR pwszNumber);
DWORD BthAGNetworkDropCall(DWORD dwFlags);
DWORD BthAGNetworkAnswerCall(void);
DWORD BthAGNetworkTransmitDTMF(LPWSTR pwszDTMF);
DWORD BthAGNetworkHoldCall(void);
DWORD BthAGNetworkUnholdCall(void);
DWORD BthAGNetworkSwapCall(void);
DWORD BthAGNetworkGetCallState(PDWORD pdwFlags);

DWORD BthAGPhoneExtInit(void);
void BthAGPhoneExtDeinit(void);
BOOL BthAGOverrideCallIn(BOOL fHandsfree);
BOOL BthAGOverrideCallOut(BOOL fHandsfree);
void BthAGPhoneExtEvent(DWORD dwEvent, DWORD dwParam, VOID* pvParam2);
BOOL BthAGGetNameByPhoneNumber(LPCSTR pszNumber, LPWSTR pwszName);
BOOL BthAGGetSpeedDial(unsigned short usIndex, LPWSTR pwszNumber);
BOOL BthAGGetLastDialed(LPWSTR pwszNumber);
DWORD BthAGSetServiceCallback(PFN_PhoneExtServiceCallback pfn);




#endif // __BTAGPUB_H__

