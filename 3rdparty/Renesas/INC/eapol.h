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


Module Name:
    eapol.h

Abstract:
    EAPOL interface definitions

--*/

#pragma once

#include <eap.h>
#include <cxport.h>
#include <wincrypt.h>
#include <wzcsapi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MD5_DIGEST_LEN      16

#define MASTER_SESSION_KEY_LENGTH   32
#define MASTER_SESSION_IV_LENGTH    32

typedef struct  _MASTER_SESSION_KEYS {
    BYTE    bPeerEncryptionKey[MASTER_SESSION_KEY_LENGTH];
    BYTE    bAuthenticatorEncryptionKey[MASTER_SESSION_KEY_LENGTH];
    BYTE    bPeerAuthenticationKey[MASTER_SESSION_KEY_LENGTH];
    BYTE    bAuthenticatorAuthenticationKey[MASTER_SESSION_KEY_LENGTH];
    BYTE    bPeerIV[MASTER_SESSION_IV_LENGTH];
    BYTE    bAuthenticatorIV[MASTER_SESSION_IV_LENGTH];
} MASTER_SESSION_KEYS, *PMASTER_SESSION_KEYS;


#define MAX_SESSION_KEY_LENGTH  32

#define csz_CLIENT_EAP_ENCRYPTION   "client EAP encryption"

typedef struct  _SESSION_KEYS {
    DWORD   dwKeyLength;
    BYTE    bSendKey[MAX_SESSION_KEY_LENGTH];
    BYTE    bReceiveKey[MAX_SESSION_KEY_LENGTH];
} SESSION_KEYS, *PSESSION_KEYS;

// secured session keys
typedef struct _SEC_SESSION_KEYS {
    DATA_BLOB   dblobSendKey;
    DATA_BLOB   dblobReceiveKey;
} SEC_SESSION_KEYS, *PSEC_SESSION_KEYS;

//
//	EAPOL Registry configuration key and value names
//
#define EAPOL_REGISTRY_KEY							        TEXT("Comm\\EAPOL")
#define EAPOL_REGISTRY_VALUENAME_MAX_START			        TEXT("MaxStart")
#define EAPOL_REGISTRY_VALUENAME_START_PERIOD		        TEXT("StartPeriodSeconds")
#define EAPOL_REGISTRY_VALUENAME_START_DELAY_MS             TEXT("StartDelayMilliseconds")
#define EAPOL_REGISTRY_VALUENAME_AUTH_PERIOD		        TEXT("AuthPeriodSeconds")
#define EAPOL_REGISTRY_VALUENAME_HELD_PERIOD		        TEXT("HeldPeriodSeconds")
#define EAPOL_REGISTRY_VALUENAME_CONFIRM_IDENTITY	        TEXT("ConfirmIdentityOnNewConnection")
#define EAPOL_REGISTRY_VALUENAME_WPA_PERIOD                 TEXT("WPAPeriodSeconds")
#define EAPOL_REGISTRY_VALUENAME_WPA_MAX_4_WAY_FAILURES     TEXT("WPAMax4WayHandshakeFailures")
#define EAPOL_REGISTRY_VALUENAME_WPA2_MAX_4_WAY_FAILURES     TEXT("WPA2Max4WayHandshakeFailures")

//	Per-SSID connection information is stored in the
//	registry key:
//
//	[HKLM\Comm\EAPOL\Config\<SSID>]
//		"Enable8021x"=dword
//		"DisplayMessage"="Request Identity Display Message"
//		"EapTypeId"=dword
//		\<EapTypeId>
//			ConnectionData=binary
//
#define EAPOL_REGISTRY_KEY_CONFIG					TEXT("Comm\\EAPOL\\Config")
#define EAPOL_REGISTRY_VALUENAME_ENABLE_8021X		TEXT("Enable8021x")
#define EAPOL_REGISTRY_VALUENAME_DISPLAY_MESSAGE	TEXT("DisplayMessage")
#define EAPOL_REGISTRY_VALUENAME_IDENTITY			TEXT("Identity")
#define EAPOL_REGISTRY_VALUENAME_PASSWORD			TEXT("Password")
#define EAPOL_REGISTRY_VALUENAME_LAST_AUTH_SUCCESS	TEXT("LastAuthSuccessful")
#define EAPOL_REGISTRY_VALUENAME_EAPTYPE			TEXT("EapTypeId")
#define EAPOL_REGISTRY_VALUENAME_CONNECTIONDATA		TEXT("ConnectionData")
#define EAPOL_REGISTRY_VALUENAME_USERDATA			TEXT("UserData")

//
// EAPOL packet types
//
typedef enum _EAPOL_PACKET_TYPE 
{
    EAPOL_EAP_Packet = 0,              
    EAPOL_Start,            
    EAPOL_Logoff,          
    EAPOL_Key            
} EAPOL_PACKET_TYPE;

//
//	EAPOL PDU format
//

typedef struct _EAPOL_PDU
{
	BYTE	ProtocolVersion;
#define			EAPOL_PROTOCOL_VERSION			1
	BYTE	PacketType;
	BYTE	PacketBodyLength[2];

	//
	//	Following the header is the PacketBody,
	//	which is PacketBodyLength bytes long.
	//
} EAPOL_PDU, *PEAPOL_PDU;


//
//  Structure: EAPOL_PACKET
//  Should be exactly the same as EAPOL_PDU.
//  EAPOL_PACKET is used by EAPOL SSN.
//

typedef struct _EAPOL_PACKET 
{
    BYTE        ProtocolVersion;
    BYTE        PacketType;
    BYTE        PacketBodyLength[2];
    BYTE        PacketBody[1];
} EAPOL_PACKET, *PEAPOL_PACKET;


#define EAPOL_PDU_HEADER_SIZE	sizeof(EAPOL_PDU)

#define REPLAY_COUNTER_LENGTH	8
#define KEY_SIGNATURE_LENGTH	16
#define KEY_IV_LENGTH			16

typedef struct _EAPOL_KEY_DESCRIPTOR 
{
    BYTE        DescriptorType;
    BYTE        KeyLength[2];							// Key length in octets, MSB first
    BYTE        ReplayCounter[REPLAY_COUNTER_LENGTH];
    BYTE        Key_IV[KEY_IV_LENGTH];
    BYTE        KeyIndex;
    BYTE        KeySignature[KEY_SIGNATURE_LENGTH];
    BYTE        Key[1];
} EAPOL_KEY_DESC, *PEAPOL_KEY_DESC;

//
// Format of EAPOL_KEY_DESC.Key field when
//    DescriptorType == EAPOL_KEY_DESCRIPTOR_TYPE_PER_STA
//
typedef struct _EAPOL_KEY_MATERIAL
{
    BYTE        KeyMaterialLength[2];
    BYTE        KeyMaterial[1];
} EAPOL_KEY_MATERIAL, *PEAPOL_KEY_MATERIAL;


//
//	Type identifiers for tuples sent in a Request/Ident message
//
typedef enum
{
	TT_NetworkId,
	TT_NASId,
	TT_PortId,
	TT_Count
} TupleType_t;


//
//	States for the EAPOL State Machine
//
typedef enum
{
	EOLState_Logoff,			// no user logged on
	EOLState_Disconnected,		// user logged on, media disconnected
	EOLState_Connecting,		// user logged on, media connected, sent Start, waiting for Request/Identity
	EOLState_Acquired,			// user logged on, media connected, received Request/Identity, sent Response/Identity
	EOLState_Authenticating,	// user logged on, media connected, received Request/Auth, sent Response/Auth
	EOLState_Authenticated,		// user logged on, media connected, received Success OR timedout
	EOLState_WPAAuthenticating, // user logged on, media connected, in WPA 4-way-handshake prior to confirming PMK for PTK.
	EOLState_Held,				// user logged on, media connected, received Fail
} EOLSessionState;

typedef enum
{
	EOLAuthState_Unchanged,     // no change
	EOLAuthState_EAP_Completed, // EAP has completed successfully, 4 way handshake to follow
	EOLAuthState_Authenticated, // all necessary authentication (EAP, 4-way) completed successfully
	EOLAuthState_Failed,        // authentication failed
} EOLAuthStateChange;

typedef VOID	(*EOL_SEND_PACKET_CB)
							(IN	PVOID              Context,
						 	 IN	PBYTE		       pPacket, OPTIONAL
							 IN	DWORD              cbPacket,
							 IN	EOLAuthStateChange EolState,
							 IN DWORD		       dwAuthResultCode);

typedef DWORD	(*EOL_SET_WEP_KEY_CB)
							(IN	PVOID          Context,
						 	 IN	PBYTE		   pWEPKey,
							 IN	DWORD          cbWEPKey,
							 IN BYTE		   bKeyIndex);

typedef DWORD	(*EOL_SET_KEY_CB)
							(IN	PVOID          Context,
							 IN DWORD          KeyIndex,
							 IN PBYTE          KeyRSC,
						 	 IN	PBYTE		   pKey,
							 IN	DWORD          cbKey);

typedef BOOL    (*EOL_SET_OID)
                            (IN PVOID          Context,
                             IN DWORD          OID,
                         	 IN PBYTE	       pbData,
                         	 IN DWORD          dwDataLength);


typedef BOOL    (*EOL_QUERY_OID)
                            (IN  PVOID         Context,
                             IN  DWORD         OID,
                         	 OUT PBYTE	       pbData,
                         	 OUT PDWORD        pdwDataLength,
                         	 OUT PBOOL         pbNativewifi);


typedef BOOL    (*EOL_QUERY_MACS)
                            (IN  PVOID         Context,
                             OUT PBYTE         pSrc,
                             OUT PBYTE         pDst);                         	 


typedef BOOL    (*EOL_SCAN_IE)
                            (IN  LPBYTE        lpIEs, 
                             IN  ULONG         nIEsLen, 
                             OUT LPBYTE        *ppbSSNIE);

typedef DWORD   (*EOL_PARSE_SSN_IE)
                            (IN  LPBYTE              lpSSNIE, 
                             IN  ULONG               nSSNIELen, 
                             IN  PWZC_WLAN_CONFIG    pWzcCfg);


////////////////////////////////////////////////////////////////////////////////
//  EAPOL SSN support.
////////////////////////////////////////////////////////////////////////////////

#define SIZE_NONCE                  32
#define SIZE_ETHERNET_TYPE          2

//
// Structure:   EAPOL_KEY
// This structure holds a buffer used for storing the PTK
//

typedef struct _EAPOL_KEYS
{
    DWORD                   cbTemporalKey;
    BYTE                    pbMICKey[16];
    BYTE                    pbEncryptionKey[16];
    BYTE                    pbTemporalKey[MAX_SESSION_KEY_LENGTH];
} EAPOL_KEYS, *PEAPOL_KEYS;

#define MAX_SSNIE_LENGTH            255

typedef struct _SSN_DATA 
{
    BYTE                    pbMICKey[16];
    BYTE                    pbEncryptionKey[16];
    EAPOL_KEYS              pbTempPTK;
    DWORD                   dwSizeOfSSNIE;
    BYTE                    pbSSNIE[MAX_SSNIE_LENGTH];
    ULONGLONG               ullLastSSNReplayCounter;
    USHORT                  usKeyDescription;
} SSN_DATA, *PSSN_DATA;


#define EAPOL_KEY_DESC_VERSION          0x0000
#define SSN_KEY_DESC_VERSION_WEP        0x0001
#define SSN_KEY_DESC_VERSION_TKIP       0x0001
#define SSN_KEY_DESC_VERSION_AES        0x0002

#define SSN_KEY_TYPE_PAIRWISE           0x0008
#define SSN_KEY_TYPE_GROUP              0x0000      // bit 3 = 0 ==> Group key

#define SSN_KEY_DESC_VERSION_BITS       0x0007
#define SSN_KEY_DESC_KEY_TYPE_BIT       0x0008
#define SSN_KEY_DESC_KEY_INDEX_BITS     0x0030
#define SSN_KEY_DESC_KEY_USAGE_BIT      0x0040
#define SSN_KEY_DESC_ACK_BIT            0x0080
#define SSN_KEY_DESC_MIC_BIT            0x0100
#define SSN_KEY_DESC_SECURE_BIT         0x0200
#define SSN_KEY_DESC_ERROR_BIT          0x0400
#define SSN_KEY_DESC_REQUEST_BIT        0x0800
#define SSN_KEY_DESC_RESERVED_BITS      0xF000

#define SSN_KEY_DESC_INDEX_BITS_OFFSET  4

typedef struct _EAPOL_KEY_SSN_DESCRIPTOR
{
    BYTE                    DescriptorType;
    BYTE                    KeyInfo[2];
    BYTE                    KeyLength[2];
    BYTE                    ReplayCounter[8];
    BYTE                    KeyNonce[SIZE_NONCE];
    BYTE                    Key_IV[16];
    BYTE                    Key_RSC[8];
    BYTE                    Key_ID[8];    
    BYTE                    KeyMIC[16];
    BYTE                    KeyMaterialLength[2];
    BYTE                    KeyMaterial[1];
} EAPOL_KEY_SSN_DESC, *PEAPOL_KEY_SSN_DESC;


//
//  If bSupportSSN is set to TRUE the rest of the entries must be valid.
//  Setting bSupportSSN indicates to EAPOL to handle SSN transactions.
//

typedef struct _EAPOL_SSN_SUPPORT
{
    BOOL                bSupportSSN;            
    EOL_SCAN_IE         ScanIE;                 // Callback function to scan the given IE for SSN IE.   
    EOL_PARSE_SSN_IE    ParseSSNIE;             // Callback function to parse a given SSN IE.
    EOL_SET_OID         SetOid;                 // Callback to set OID.
    EOL_QUERY_OID       QueryOid;               // Callback to query OID.
    EOL_QUERY_MACS      QueryMacs;              // Callback to get SRC and DST (BSSID) MAC addresses.
    DWORD               dwEncryptionStatus;
    DWORD               dwAuthenticationMode;

}   EAPOL_SSN_SUPPORT, *PEAPOL_SSN_SUPPORT;



#define SIZE_MAC_ADDR               6

#define EAPOL_KEY_DESC_RC4          1
#define EAPOL_KEY_DESC_WPA2         2
#define EAPOL_KEY_DESC_WPA          254
#define EAPOL_KEY_DESC_RSN          EAPOL_KEY_DESC_WPA2
#define EAPOL_KEY_DESC_SSN          EAPOL_KEY_DESC_WPA

#define MAX_KEY_DESC                EAPOL_KEY_DESC_SSN

//
//	The EOL_SESSION structure contains the state of
//	an EAPOL connection.
//
typedef struct _EOL_SESSION
{
	struct _EOL_SESSION	*pNext;

	int					refcnt;

	//
	//	Lock to synchronize multiple threads accessing the
	//	session structure.
	//
	CRITICAL_SECTION	Lock;

	BOOL				fAuthenticator;

	EOLSessionState		State;

	//
	//	TRUE if our media state is connected,
	//	FALSE if our media is disconnected.
	//
	BOOL				bMediaConnected;

	//
	//	The SSID with which we are currently
	//	associated if bMediaConnected is TRUE.
	//
	PWCHAR				pwszSSID;

	//
	//	EAP type in use for the session.
	//
	DWORD				bEapTypeId;

	//
	//	Timer used by authenticator to resend packets
	//	when no response is received.
	//
	CTETimer			Timer;
	BOOL				bTimerRunning;

	//
	//	startCount eeps track of the number of times
	//	that a EAPOL_Start message has been
	//	sent without receiving any Request/Identity
	//	response.  If startCount reaches maxStart
	//	we assume that the peer does not do
	//	EAPOL and so we enter the Authenticated
	//	state.
	//
	//	startPeriod is the time in seconds that
	//	we wait before resending an EAPOL_Start
	//	message.
	//
	DWORD				startCount;
	DWORD				maxStart;
#define						DEFAULT_MAX_START				3
	DWORD				startPeriodSeconds;
#define						DEFAULT_START_PERIOD_SECONDS	3

	// Delay sending the very first EAPOL_Start message if
	// startDelayMilliseconds is >0. This is used on 802.11 networks
	// where the AP will automatically initiate 802.1X authentication
	// as soon as the client associates, without the need for an explicit
	// EAPOL_Start message to be sent by the client.
	//
	DWORD				startDelayMilliseconds;
#define						DEFAULT_START_DELAY_MS 500

	//
	//	authPeriod is the number of seconds we
	//	wait for a message from the server in
	//	the ACQUIRED or AUTHENTICATING states.
	//
	DWORD				authPeriodSeconds;
#define						DEFAULT_AUTH_PERIOD_SECONDS	30

	//
	//	heldPeriod is the number of seconds we
	//	wait and ignore all messages after a
	//	few failed authentications, to hinder
	//	brute force attacks
	//
	DWORD				heldPeriodSeconds;
#define						DEFAULT_HELD_PERIOD_SECONDS	60

	//
	//  If this is TRUE, then when a connection
	//  that is not currently
	//  authenticated receives a Request/Identity,
	//  the user will be prompted to confirm/enter
	//  identity intformation even if previous
	//  identity information has been saved 
	//
	//  If FALSE, the saved information will be used
	//  when possible to avoid prompting the user.
	//  Note that this precludes the possibility of
	//  the user being prompted to change identity
	//  so long as the old identity successfully
	//  authenticates.
	//
	//  In either case, if there is no saved identity
	//  information the user will be prompted for it.
	//
	BOOL				bConfirmIdentityOnNewConnection;

	//
	//	Context is passed back to the caller
	//	in callback functions.
	//
	PVOID				Context;

	//
	//	Maximum size of pSendPacket
	//
	DWORD				cbSendPacket;

	//
	//	Space reserved for external protocol use in
	//	the send packet.  That is, when the SendPacketCB
	//	is called (with the packet pointer equal to pSendPacket),
	//	the callback handler function can subtract cbSendHeaderReserved
	//	from the pointer and use the space to prefix its protocol
	//	header information.
	//
	DWORD				cbSendHeaderReserved;

	//
	//	Pointer to memory allocated to hold packets generated
	//	by EAPOL itself, e.g. EAPOL_Start PDUs.  Note that
	//	cbSendHeaderReserved bytes of space will be reserved
	//	in front of this EAPOL_PDU for use by the next protocol
	//	layer.
	//
	PEAPOL_PDU			pTxPDU;

	EOL_SEND_PACKET_CB	SendPacketCB;

	EOL_SET_WEP_KEY_CB	SetWEPKeyCB;

	EOL_SET_KEY_CB		SetKeyCB;

	EOL_SET_OID         pf_SetOID;

	EOL_QUERY_OID       pf_QueryOID;

	EOL_QUERY_MACS      pf_QueryMac;

	EOL_SCAN_IE         pf_ScanIE;

	EOL_PARSE_SSN_IE    pf_ParseSSNIE;

	//
	//	Replay counter from most recent EAPOL_KEY message, or
	//	zero if none received.
	//
	BYTE				LastReplayCounter[REPLAY_COUNTER_LENGTH];

	//
	//	MPPE encryption keys saved from the EAP extension
	//
	BYTE				MPPESendKey[MAX_MPPE_KEY_LENGTH];
	DWORD				cbMPPESendKey;
	BYTE				MPPERecvKey[MAX_MPPE_KEY_LENGTH];
	DWORD				cbMPPERecvKey;

	//
	//	Authenticatee: Tuple values saved from the last Request/Ident message
	//  Authenticator: Tuple values to send in Request/Ident message
	//
	PCHAR				TupleValues[TT_Count];

	PEAP_SESSION		pEapSession;

	//
	//	Set to TRUE if the user cancels a credentials dialog,
	//	indicating that they are not interested in ever logging
	//	in to this session.
	//
	BOOL				bUserCancelled;

	//
	//	Set to TRUE to indicate that the most recent authentication
	//	attempt was successful.
	//
	BOOL				bAuthenticationSuccessful;

	SESSION_KEYS		SessionKeys;


	/////////////////////////
	//  RSN (WPA2) data
	/////////////////////////
	PVOID               pRSNSession;

    ////////////////////////////////////////////////////////////////////////////
    //  SSN support
    ////////////////////////////////////////////////////////////////////////////

    BOOL                bSSNSupported;

    SSN_DATA            SSNData;

    //
    //  Version of EAPOL supported
    //
    
    BYTE                bProtocolVersion;   

    //
    // Mac Addr of peer (switch port access point)
    //
    
    BYTE                bSrcMacAddr[SIZE_MAC_ADDR];  

    //
    //  Mac Addr of peer (switch port or access point)
    //
    
    BYTE                bDestMacAddr[SIZE_MAC_ADDR]; 

    //
    //  Friendly name of the interface on which this port is opened
    //  We don't use this...
    //
    
    WCHAR               *pwszFriendlyName;


    //
    //  Last replay counter. Used to guard against security attacks
    //
    
    ULONGLONG           ullLastReplayCounter; 


    //
    //  This must be EOL_SESSION.Context.
    //
    
    PVOID               hPort;    


    //
    //  Encryption Status and Authentication Mode for 4 way handshake.
    //

    DWORD               dwEncryptionStatus;
    DWORD               dwAuthenticationMode;


    //
    //  Time out when in 4-way-handshake.
    //

#define DEFAULT_WPA_PERIOD_SECONDS          5    

    DWORD               WPAPeriodSeconds;

    BOOL                bWPAMediaSenseAutodetect;


#define DEFAULT_WPA_MAX_4_WAY_FAILURES      16

    DWORD               dwWPAMax4WayFailures;
    DWORD               dwNoOf4WayFailures;

    //
    // Whether or not we have received the pairwise key.
    //
    
    BOOL                fTransmitKeyReceived;

    //
    // RAS_EAP_FLAG_xxx settings to be passed to EAP
    //
    DWORD               EapFlags;

} EOL_SESSION, *PEOL_SESSION;

//
//	eapolsession.c
//
extern PEOL_SESSION
EolSessionCreate(
	IN		PVOID				Context,
	IN		BOOL				fAuthenticator,
	IN		DWORD				cbSendPacket,
	IN		DWORD				cbSendHeaderReserved,
	IN		EOL_SEND_PACKET_CB	SendPacketCB,
	IN		EOL_SET_WEP_KEY_CB	SetWEPKeyCB,
	IN		EOL_SET_KEY_CB	    SetKeyCB,
	IN      PEAPOL_SSN_SUPPORT  pSSN,
    IN      DWORD               EapFlags
	);


extern VOID
EolSessionDestroy(
	PEOL_SESSION pSession);

extern VOID
EolSessionUserLogon(
	PEOL_SESSION pSession);

extern VOID
EolSessionUserLogoff(
	PEOL_SESSION pSession);

extern DWORD
EolSessionMediaConnect(
	PEOL_SESSION pSession,
	PWSTR		 pwszSSID
	);

extern VOID
EolSessionMediaDisconnect(
	PEOL_SESSION pSession);

extern void
EolSessionSetMACAddresses(
	IN	OUT	PEOL_SESSION	pSession,
	IN      const BYTE     *pAPMAC,
	IN      const BYTE     *pStationMAC);

extern void
EolSessionSetStationRSNIE(
	IN	OUT	PEOL_SESSION	pSession,
	IN __in_bcount(cbAssociationRSNIE) const BYTE *pStationRSNIE, 
	IN                                 DWORD       cbStationRSNIE);

extern void
EolSessionSetExpectedPMKID(
	IN	OUT	PEOL_SESSION	pSession,
	IN      const BYTE     *pExpectedPMKID);

extern VOID
EolSessionMediaSpecific(
	PEOL_SESSION pSession,
	PVOID        pvBlob,
	DWORD        dwBlobLength);

extern VOID
EolSessionProcessRxPacket(
	IN	OUT	PEOL_SESSION pSession,
	IN		PBYTE		 pRxBuffer,
	IN		DWORD		 cbRxBuffer);

extern DWORD
EolSessionKeyMaterialSet(
	IN	OUT	PEOL_SESSION	pSession,
	IN      PSESSION_KEYS   pSessionKeys);

extern VOID
EolSessionSetStartDelay(
	IN  OUT PEOL_SESSION pSession,
    IN      DWORD        StartDelayMs);

extern DWORD
EolSessionGetSendKey(
	IN          PEOL_SESSION pSession,
        OUT     PBYTE        pSendKey,
    IN  OUT     PDWORD       pcbSendKey);

//
//	eapolfsm.c
//
VOID
eolFsmProcessTxIdentityEvent(
	IN	OUT	PEOL_SESSION pSession);

VOID
eolFsmProcessTxAuthEvent(
	IN	OUT	PEOL_SESSION pSession);

VOID
eolFsmProcessUserLogonEvent(
	IN	OUT	PEOL_SESSION pSession);

VOID
eolFsmProcessUserLogoffEvent(
	IN	OUT	PEOL_SESSION pSession);

VOID
eolFsmProcessMediaConnectEvent(
	IN	OUT	PEOL_SESSION pSession);

VOID
eolFsmProcessMediaDisconnectEvent(
	IN	OUT	PEOL_SESSION pSession);


//
//	eapolmsg.c
//

DWORD
eolTxStart(
	PEOL_SESSION pSession);

DWORD
eolTxLogoff(
	PEOL_SESSION pSession);

DWORD
eolTxEapResponse(
	IN	PEOL_SESSION		pSession);

//
//	Configuration functions
//

extern DWORD
EolEnumExtensions(
	IN	DWORD				cbExtensionInfo,
	OUT	PEAP_EXTENSION_INFO	pExtensionInfo,
	OUT	PDWORD				pdwNumExtensions,
	OUT PDWORD				pcbExtensionInfoNeeded);


extern DWORD
EolSS8021xEnableGet(
    IN	PWSTR        pwszSSID,
    OUT PBOOL        pbEnabled
    );

extern DWORD
EolSS8021xEnableSet(
    IN	PWSTR        pwszSSID,
    IN  BOOL        bEnabled
    );

extern DWORD
EolSSEapTypeGet(
    IN	PWSTR         pwszSSID,
    OUT	PDWORD        pdwEapTypeId
    );

extern DWORD
EolSSEapTypeGet(
    IN	PWSTR         pwszSSID,
    OUT	PDWORD        pdwEapTypeId
    );

extern DWORD
EolSSEapTypeSet(
    IN	PWSTR         pwszSSID,
    IN	DWORD         dwEapTypeId
    );

extern DWORD
EolSSConnectionDataGet(
    IN		PWCHAR        pwszSSID,
    IN		DWORD         dwEapTypeId,
		OUT	PBYTE         pbConnInfo,
    IN	OUT	PDWORD        pdwInfoSize
    );

extern DWORD
EolSSConnectionDataSet(
    IN	PWCHAR        pwszSSID,
    IN	DWORD         dwEapTypeId,
    IN	PBYTE         pbConnInfo,
    IN	DWORD         dwInfoSize
    );

extern DWORD
EolSSUserDataGet(
    IN		PWCHAR        pwszSSID,
    IN		DWORD         dwEapTypeId,
    OUT		PBYTE         pUserData,
    IN	OUT	PDWORD        pcbUserData);

extern DWORD
EolSSUserDataSet(
    IN	PWCHAR        pwszSSID,
    IN	DWORD         dwEapTypeId,
    IN	PBYTE         pUserData,
    IN	DWORD         cbUserData);

extern DWORD
EolSSInvokeConfigUI(
    IN	PWCHAR        pwszSSID,
    IN	DWORD         dwEapTypeId,
	IN	HWND		  hWndParent,
	IN	DWORD		  dwFlags,
    IN	PBYTE         pbConnInfo,
    IN	DWORD         dwInfoSize
    );

DWORD
EolSSDelete(
    IN	PWCHAR		pwszSSID);

#ifdef __cplusplus
}
#endif

