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
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define IPSEC_API_DH_GROUP_1 1
#define IPSEC_API_DH_GROUP_2 2
#define IPSEC_API_DH_GROUP_2048 4

#define IPSEC_API_DEFAULT_DH_GROUP (IPSEC_API_DH_GROUP_2|IPSEC_API_DH_GROUP_1)

#define IPSEC_API_ALL_DH IPSEC_API_DH_GROUP_1 | IPSEC_API_DH_GROUP_2 | IPSEC_API_DH_GROUP_2048

#define IPSEC_API_CONF_ALGO_3_DES 1
#define IPSEC_API_CONF_ALGO_DES 2
#define IPSEC_API_CONF_ALGO_NONE 4

#define IPSEC_API_DEFAULT_CONF_ALGO  (IPSEC_API_CONF_ALGO_3_DES|IPSEC_API_CONF_ALGO_DES)

#define IPSEC_API_ALL_QM_CONF_ALGO (IPSEC_API_CONF_ALGO_3_DES | IPSEC_API_CONF_ALGO_DES | IPSEC_API_CONF_ALGO_NONE)
#define IPSEC_API_ALL_MM_CONF_ALGO (IPSEC_API_CONF_ALGO_3_DES | IPSEC_API_CONF_ALGO_DES)

#define IPSEC_API_AUTH_ALGO_MD5 1
#define IPSEC_API_AUTH_ALGO_SHA1 2
#define IPSEC_API_AUTH_ALGO_NONE 4

#define IPSEC_API_DEFAULT_AUTH_ALGO  (IPSEC_API_AUTH_ALGO_SHA1|IPSEC_API_AUTH_ALGO_MD5)

#define IPSEC_API_ALL_QM_AUTH_ALGO (IPSEC_API_AUTH_ALGO_MD5 | IPSEC_API_AUTH_ALGO_SHA1 | IPSEC_API_AUTH_ALGO_NONE)
#define IPSEC_API_ALL_MM_AUTH_ALGO (IPSEC_API_AUTH_ALGO_MD5 | IPSEC_API_AUTH_ALGO_SHA1)

#define IPSEC_API_PSK_AUTH 1
#define IPSEC_API_CERT_AUTH 2

#define IPSEC_API_DEFAULT_AUTH  	IPSEC_API_CERT_AUTH

#define IPSEC_API_PROTOCOL_TCP 6
#define IPSEC_API_PROTOCOL_UDP 17

#define IPSEC_API_DEFAULT_PROTOCOL 0

#define IPSEC_API_IN_ACTION_CLEAR 0x1
#define IPSEC_API_IN_ACTION_SECURE 0x2

#define IPSEC_API_OUT_ACTION_CLEAR 0x1
#define IPSEC_API_OUT_ACTION_SOFT 0x2
#define IPSEC_API_OUT_ACTION_SECURE 0x3

#define IPSEC_API_DEFAULT_IN_ACTION IPSEC_API_IN_ACTION_CLEAR
#define IPSEC_API_DEFAULT_OUT_ACTION IPSEC_API_OUT_ACTION_SOFT

#define IPSEC_API_PARAM_LOG_LEVEL 0x1
#define IPSEC_API_PARAM_LOG_METHOD 0x2

#define IPSEC_API_ALL_PARAM IPSEC_API_PARAM_LOG_LEVEL | IPSEC_API_PARAM_LOG_METHOD

#define IPSEC_API_LOG_LEVEL_NONE 0
#define IPSEC_API_LOG_LEVEL_ERROR 1
#define IPSEC_API_LOG_LEVEL_WARNING 2
#define IPSEC_API_LOG_LEVEL_DIAG 3
#define IPSEC_API_LOG_LEVEL_PSS 4
#define IPSEC_API_LOG_LEVEL_TRACE 5
#define IPSEC_API_LOG_LEVEL_VERBOSE 6
#define IPSEC_API_LOG_LEVEL_AUDIT 7

#define IPSEC_API_LOG_PRIVATE_BASE 20

#define IPSEC_API_LOG_METHOD_DEBUGOUT 0x1
#define IPSEC_API_LOG_METHOD_CELOG 0x2

// Make sure you are in sync with flags in int_ipsec_api.h
#define IPSEC_API_NO_ICMP_EXEMPT 0x40
#define IPSEC_API_NO_SPECIALSERVERS_EXEMPT 0x80
#define IPSEC_API_APPLY_TO_ALL_SRC_IP 0x1000

#define IPSEC_API_ALL_VALID_EXTERNAL_FLAGS IPSEC_API_NO_ICMP_EXEMPT | \
								IPSEC_API_NO_SPECIALSERVERS_EXEMPT | \
								IPSEC_API_APPLY_TO_ALL_SRC_IP 								

typedef struct {
	DWORD inAction;
	DWORD outAction;
	DWORD auth;
	PBYTE authInfo;
	DWORD authInfoSize;
	DWORD mmHashMask;
	DWORD mmEncMask;
	DWORD qmHashMask;
	DWORD qmEncMask;
	DWORD dhMask;
	DWORD protocol;
	DWORD srcIP;
} IPSEC_API_MODE_INFO, *PIPSEC_API_MODE_INFO;

typedef struct {
	DWORD id;
	PVOID val;
	DWORD valSize;
} IPSEC_API_PARAM, *PIPSEC_API_PARAM;

HRESULT
StartIPSec(
void
);

HRESULT
StopIPSec(
void
);

HRESULT
IsIPSecRunning(
void
);

HRESULT
SetIPSecMode(
	IPSEC_API_MODE_INFO modeInfo,
	DWORD flags
);

HRESULT
ResetIPSecMode(
	DWORD srcIP,
	DWORD flags
);

HRESULT
SetIPSecConfig(
	IPSEC_API_PARAM param,
	DWORD flags
);

#ifdef __cplusplus
}
#endif

