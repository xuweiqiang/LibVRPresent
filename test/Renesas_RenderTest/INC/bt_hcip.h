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
//------------------------------------------------------------------------------
// 
//      Bluetooth HCI Transport declarations
// 
// 
// Module Name:
// 
//      bt_hcip.h
// 
// Abstract:
// 
//      This file defines HCI transport interfaces
// 
// 
//------------------------------------------------------------------------------
#if ! defined (__bt_hcip_H__)
#define __bt_hcip_H__		1

#include <bt_buffer.h>

#if defined (__cplusplus)
extern "C" {
#endif

enum HCI_TYPE {
	COMMAND_PACKET  = 1,
	DATA_PACKET_ACL = 2,
	DATA_PACKET_SCO = 3,
	EVENT_PACKET    = 4,
	ETYPE_FINISH    = 5
};

enum HCI_EVENT {
	DEVICE_UP,
	DEVICE_DOWN,
	DEVICE_ERROR
};

typedef int (*HCI_TransportCallback) (HCI_EVENT eEvent, void *pEvent);

#define HCI_INTERFACE_VERSION_1_1			0x00010001

#define HCI_HARDWARE_VERSION_V_1_0_A		0
#define HCI_HARDWARE_VERSION_V_1_0_B		1
#define HCI_HARDWARE_VERSION_V_1_1			2

#define HCI_DEFAULT_WRITE_TIMEOUT			15000

#define HCI_DEFAULT_DRIFT					(60*1000)	// One minute

#define HCI_FLAGS_NORESET					0x00000001
#define HCI_FLAGS_NOLOCALNAME				0x00000002
#define HCI_FLAGS_NOROLESWITCH				0x00000004
#define HCI_FLAGS_AUTOCONFIGURE				0x80000000

//
//	Note:
//		iMaxSize parameters MUST fit the biggest HCI command
//			(HCI_RemoteNameRequest as of this writing is 257 bytes,
//			see also bt_ddi.h::HCI_MAX_COMMAND_LENGTH)
//		iReadBufferHeader and iWriteBufferHeader MUSt be 4-bytes aligned
//		HCI_ReadPacket MUST return 4-bytes aligned cStart in its buffer
//		HCI_ReadPacket and HCI_WritePacket WILL change cStart and cEnd of the
//		buffer, but WILL NOT change any other data.
//
//		iScoWriteNumPackets and iScoWriteMaxPacketSize:
//		Currently, only bthusb sets those to non-zero.
//
typedef struct __hci_parameters {
	unsigned int	uiSize;					// structure size
	unsigned int	fInterfaceVersion;		// interface version

	int				iMaxSizeRead;			// Max size of read packet (not including headers and trailers)
	int				iMaxSizeWrite;			// Max size of write packet (not including headers and trailers)
	int				iReadBufferHeader;		// Hint : read may contain up to as much transport header bytes
	int				iReadBufferTrailer;		// Hint : read may contain up to as much transport trailer bytes
	int				iWriteBufferHeader;		// Number of bytes to preallocate for transport header
	int				iWriteBufferTrailer;	// Number of bytes to preallocate for transport trailer

	int				uiFlags;				// Flags
	int				fHardwareVersion;		// Bluetooth spec version of hardware
	unsigned int	uiResetDelay;			// milliseconds : delay next command after reset by this much
	unsigned int	uiWriteTimeout;			// milliseconds : abort if card does not respond in this time (use the default above)
	unsigned int	uiDriftFactor;			// milliseconds : inquiry data is good for this long

	int				iScoWriteLowNumPackets;	// Minimum number of outstanding SCO write packets in backlog (0 - use default)
	int				iScoWriteNumPackets;	// Maximum number of outstanding SCO write packets in backlog (0 - use default)
	int				iScoWritePacketSize;	// Size of each SCO packet (0 - use default, -1 - SCO not supported)
	int				iScoSampleSize;			// Sample size of SCO packet (8 or 16 bits)

} HCI_PARAMETERS;

// These can be called before connection is open
int HCI_ReadHciParameters (HCI_PARAMETERS *pParms);

int HCI_SetCallback (HCI_TransportCallback pfCallback);
int HCI_StartHardware (void);
int HCI_StopHardware (void);

// These are connection setup functions
int HCI_OpenConnection (void);
int HCI_ReadPacket (HCI_TYPE *peType, BD_BUFFER *pInBuffer);
int HCI_WritePacket (HCI_TYPE eType, BD_BUFFER *pOutBuffer);
void HCI_CloseConnection (void);


#if defined (__cplusplus)
};			// __cplusplus
#endif

#endif				/* __bt_hcip_H__ */
