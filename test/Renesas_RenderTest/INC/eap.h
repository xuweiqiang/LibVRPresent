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
    eap.h

Abstract:
    EAP support dll definitions

--*/

#pragma once

#include <raseapif.h>
#include <cxport.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RAS_EAP_REGISTRY_LOCATION				TEXT("Comm\\EAP")

/////////////////////////////////////////////////////////////////////
//	Eap extension management definitions, structures, and functions
/////////////////////////////////////////////////////////////////////

#define RAS_EAP_EXTENSIONS_REGISTRY_LOCATION	TEXT("Comm\\EAP\\Extension")

typedef struct _EAP_EXTENSION_INFO
{
	// Information describing the extension loaded from the registry
	DWORD				dwExtensionType;
	WCHAR				wszExtensionDllPath[MAX_PATH];
	WCHAR				wszInteractiveUIPath[MAX_PATH];
	WCHAR				wszIdentityPath[MAX_PATH];
	WCHAR				wszConfigUIPath[MAX_PATH];
	WCHAR				wszFriendlyName[MAX_PATH];
	BOOL				bRequireConfigUI;
	BOOL				bStandaloneSupported;
	BOOL				bMPPEEncryptionSupported;
	BOOL				bInvokeUsernameDialog;
	BOOL				bInvokePasswordDialog;
	WCHAR				wszConfigCLSID[MAX_PATH];
} EAP_EXTENSION_INFO, *PEAP_EXTENSION_INFO;
//
// Structure used to hold information about loaded EAP extensions
//
typedef struct _EAP_EXTENSION 
{
	struct _EAP_EXTENSION	*pNext;

	//
	//	Each Session that has this extension type open maintains
	//	a reference to it.
	//
	int					refcnt;

	// Information describing the extension loaded from the registry
	EAP_EXTENSION_INFO	info;
	BYTE				*DefaultConfigData;

    // Handle to loaded extension DLL
    HINSTANCE			hExtensionDll;
    
    // Information returned by calling extension's RasEapGetInfo function
    PPP_EAP_INFO		PPPEapInfo;

} EAP_EXTENSION, *PEAP_EXTENSION;

extern PEAP_EXTENSION
eapExtensionOpen(
	DWORD			dwExtensionType);

extern VOID
eapExtensionClose(
	PEAP_EXTENSION	pExtension);

extern DWORD
eapUtilGetProcAddresses(
	IN	PWCHAR		wszDllName,
	OUT	HINSTANCE	*phDll,
	...);

extern RAS_AUTH_ATTRIBUTE *
EapUtilAuthAttributeGetVendorSpecific(
    IN  RAS_AUTH_ATTRIBUTE *    pUserAttributes,
    IN  DWORD                   dwVendorId,
    IN  BYTE                    dwVendorType);

extern DWORD
EapUtilAuthAttributeInsertVSA(
    IN  DWORD                   iAttribute,
    OUT RAS_AUTH_ATTRIBUTE *    pAttributes,
    IN  DWORD                   dwVendorId,
    IN  BYTE                    bVendorType,
	__in_bcount(cbValue) IN  PBYTE                   pValue,
	IN  BYTE                    cbValue);

extern RAS_AUTH_ATTRIBUTE *
EapUtilAuthAttributeArrayAlloc(
	DWORD nAttrs);

extern void
EapUtilAuthAttributeArrayFree(
	RAS_AUTH_ATTRIBUTE *pAttributes);

#define MAX_MPPE_KEY_LENGTH	32

extern DWORD
EapUtilExtractMPPEKey(
	IN	OUT	PBYTE				pMPPEKey,
		OUT PDWORD				pcbMPPEKey,
    IN  RAS_AUTH_ATTRIBUTE *    pUserAttributes,
    IN  DWORD                   dwVendorId,
    IN  BYTE                    dwVendorType);

extern DWORD
eapExtensionInitializeModule();

extern DWORD
eapExtensionShutdownModule();

extern DWORD
EapEnumExtensions(
	IN	DWORD				cbExtensionInfo,
	OUT	PEAP_EXTENSION_INFO	pExtensionInfo,
	OUT	PDWORD				pdwNumExtensions,
	OUT PDWORD				pcbExtensionInfoNeeded);

extern DWORD
EapInvokeConfigUI(
	IN		DWORD			dwEapTypeId,
	IN		HWND			hWndParent,
	IN		DWORD			dwFlags,
	IN		PBYTE			pConnectionDataIn,
	IN		DWORD			dwSizeOfConnectionDataIn,
	OUT		PBYTE			*ppConnectionDataOut,
	OUT		PDWORD			pdwSizeOfConnectionDataOut);

extern VOID
EapFreeConnectionData(
	IN		PBYTE			pConnectionData);

extern DWORD
EapInvokeInteractiveUI(
	IN	    DWORD        dwEapTypeId,
	IN      HWND         hwndParent,       // handle to parent window
	IN      PBYTE        pUIContextData,   // pointer to context data
	IN      DWORD        dwSizeofUIContextData,  // size of context data
	OUT     PBYTE        *ppDataFromInteractiveUI, // pointer to data returned from UI
	OUT     PDWORD       pdwSizeOfDataFromInteractiveUI  // size of data returned from UI.
);

////////////////////////////////////////////////
// EAP message types and functions
////////////////////////////////////////////////

#define EAPTYPE_Identity         1
#define EAPTYPE_Notification     2
#define EAPTYPE_Nak              3

typedef struct _DECODED_EAP_PACKET
{
	PPP_EAP_PACKET	*pEncodedPacket;
	DWORD			cbEncodedPacket;

	BYTE			Code;
	BYTE			Identifier;

	// Request and response only fields (not present in Success/Failure packets)
	BYTE			Type;
	PBYTE			pTypeData;
	DWORD			cbTypeData;
} DECODED_EAP_PACKET, *PDECODED_EAP_PACKET;

BOOL
eapDecodePacket(
	IN	PBYTE				pReceiveBuf,
	IN	DWORD				cbReceiveBuf,
	OUT	PDECODED_EAP_PACKET	pDecodedPacket);

////////////////////////////////////////////////
// EAP session types and functions
////////////////////////////////////////////////


typedef enum
{
	EAPState_Initial,			// authenticatee: no communication with peer yet
								// authenticator: Request/Ident sent, no response yet
	EAPState_Acquired,			// authenticatee: Response/Ident sent
								// authenticator: Response/Ident received, Request/Auth sent
	EAPState_Authenticating,	// authenticatee: Response/Auth sent
								// authenticator: Response/Auth received, AuthenticateCB called
	EAPState_Authenticated,		// authenticatee: Success received
								// authenticator: Success sent
	EAPState_Failed,			// authenticatee: Failure received
								// authenticator: Failure sent
	EAPState_Error,				// Serious error occurred, game over
} EAPSessionState;

typedef VOID	(*EAP_GET_IDENTITY_CB)
							(IN	PVOID Context,
							 IN	PBYTE pTypeData,
							 IN DWORD cbTypeData,
							 OUT PBYTE pIdentity,
							 IN OUT PDWORD cbIdentity);

typedef VOID	(*EAP_NOTIFICATION_CB)
							(IN	PVOID Context,
							 IN	PBYTE pTypeData,
							 IN	DWORD cbTypeData);

typedef VOID	(*EAP_SEND_PACKET_CB)
							(IN	PVOID          Context,
						 	 IN	PPP_EAP_PACKET *pPacket, OPTIONAL
							 IN	DWORD          cbPacket,
							 IN BOOL		   bDone,
							 IN DWORD		   dwAuthResultCode);

typedef VOID	(*EAP_AUTHENTICATE_CB)
					 (IN	PVOID				Context,
					  IN	RAS_AUTH_ATTRIBUTE	*pUserAttributes);

typedef VOID	(*EAP_SAVE_CONNECTION_DATA_CB)
					 (IN	PVOID         Context,
					  IN	PBYTE		  pConnectionData,
					  IN	DWORD		  dwSizeOfConnectionData);

typedef VOID	(*EAP_SAVE_USER_DATA_CB)
					 (IN	PVOID         Context,
					  IN	PBYTE		  pUserData,
					  IN	DWORD		  dwSizeOfUserData);

typedef DWORD	(*EAP_INVOKE_UI_CB)
					 (IN	PVOID         Context,
					  IN    BYTE          dwEapTypeId,
					  IN    PBYTE         pUIContextData,
					  IN    DWORD         dwSizeofUIContextData);

typedef VOID	(*EAP_SAVE_PASSWORD_CB)
					 (IN	PVOID         Context,
					  IN    PWSTR         pwszPassword,
					  IN    BOOL          fSavePassword);

//
// EAP_SESSION_DESCRIPTOR contains constant information that tells
// the EAP engine various characteristics of the creator of an EAP
// session.
//
typedef struct
{
	//
	// Flag to cause the GetIdentityCB and the InvokeUICB
	// to be invoked synchronously. That is, if this flag
	// is FALSE then a thread will be spun and the callbacks
	// will occur asynchronously on that thread.
	//
	BOOL                        bDoSyncCallbacks;

	//
	//	[Optional] Callback to handle get identity requests
	//
	EAP_GET_IDENTITY_CB			GetIdentityCB;

	//
	//	Callback to handle notification requests
	//
	EAP_NOTIFICATION_CB			NotificationCB;

	//
	//	Callback to send packets and/or to indicate
	//  authentication process completion.
	//  If the fDone flag is TRUE, then the authentication
	//  process is over and dwAuthResultCode will be SUCCESS
	//  if authentication was successful.  Any non-zero value indicates failure.
	//
	//
	EAP_SEND_PACKET_CB			SendPacketCB;

	//
	//	[Optional, Server only]Callback to authenticate user credentials
	//
	EAP_AUTHENTICATE_CB			AuthenticateCB;

	//
	//	[Optional] Callbacks to save connection and user data respectively.
	//	These are called when RasEapMakeMessage returns and the
	//	authentication module has requested that the data be
	//	saved.
	//
	EAP_SAVE_CONNECTION_DATA_CB	SaveConnectionDataCB;

	EAP_SAVE_USER_DATA_CB		SaveUserDataCB;	

	//
	//  [Optional] Callback to intercept invocation of interactive
	//  UI requests by the extension.
	//
	EAP_INVOKE_UI_CB		    InvokeUICB;

	//
	//  [Optional] Callback to save an updated password that
	//  changed as a result of interactive UI.
	//
	EAP_SAVE_PASSWORD_CB		SavePasswordCB;	

} EAP_SESSION_DESCRIPTOR, *PEAP_SESSION_DESCRIPTOR;

//
//	The EAP_SESSION structure contains the state of
//	an EAP authentication negotiations for a connection.
//
typedef struct _EAP_SESSION
{
	struct _EAP_SESSION	*pNext;

	int							refcnt;

	//
	//	Lock to synchronize multiple threads accessing the
	//	session structure.
	//
	CRITICAL_SECTION			Lock;

	//
	//	Data used to communicate with extensions
	//
	PPP_EAP_INPUT				PPPEapInput;
	PPP_EAP_OUTPUT				PPPEapOutput;

	//
	//	If we have selected a particular extension type and
	//	are using it, then pExtension points to it (with a
	//	reference taken).  Otherwise, pExtension is NULL.
	//
	PEAP_EXTENSION				pExtension;

	//
	//	The extension's RasEapBegin function is called to
	//	allocate a work buffer for the session.  On the
	//	authenticatee, this occurs when a Request/Auth
	//	is received.  On the authenticator, this will be
	//	called to generate the first Request/Auth.
	//
	//	pWorkBuffer is freed and set to null when RasEapEnd
	//	is called when pExtension transitions from non-NULL
	//	to NULL.
	//
	PVOID						pWorkBuffer;

	//
	//	Only want to call RasEapBegin for a given extension type.
	//	When we attach a new extension to the session, this
	//	flag is cleared.
	//
	BOOL						bRasEapBeginCalled;

	//
	//	Data opaque to EAP, passed to callback functions
	//
	PVOID						Context;

	//
	//	Maximum size of EAP packet we can send (note that this size
	//  does not include any protocol headers that may encapsulate
	//  the EAP packet).
	//
	DWORD				         cbSendPacket;

	//
	//	Space reserved for external protocol use in
	//	the send packet.  That is, when the SendPacketCB
	//	is called (with the packet pointer equal to pSendPacket),
	//	the callback handler function can subtract cbSendHeaderReserved
	//	from the pointer and use the space to prefix its protocol
	//	header information.
	//
	DWORD				          cbSendHeaderReserved;

	PEAP_SESSION_DESCRIPTOR       pDescriptor;

	//
	//	For an authenticatee this is the only EAP authentication
	//	protocol type that it will allow for this session.
	//
	//	An authenticator will try this type first, but if the
	//	client NAKs it will try other types that the authenticator
	//  supports.
	//
	BYTE				bEapTypeToBeUsed;
#define EAP_INVALID_TYPE	0

	//
	//	State of the connection with the peer
	//
	EAPSessionState		State;

	//
	//	Timer used by authenticator to resend packets
	//	when no response is received.
	//
	CTETimer			Timer;
	BOOL				bTimerRunning;

	//
	//	dwRetryTimeoutMs is the timer interval
	//	dwRetryMax is the max number of retries before giving up
	//	dwRetryCount counts the number of attempts to get a response
	//
	DWORD				dwRetryTimeoutMs;
#define	DEFAULT_RETRY_TIMEOUT_MS	5000
	DWORD				dwRetryMax;
#define DEFAULT_RETRY_MAX			10
	DWORD				dwRetryCount;

	//
	//	ID of the last request message sent to the peer
	//
	DWORD				dwLastRequestId;

	//
	//	ID of the last response message sent to the peer
	//
	DWORD				dwLastResponseId;
#define EAP_INVALID_ID	0xFFFFFFFF

	//
	//	Request ID to use next
	//
	BYTE				NextRequestId;

	//
	//	Last message sent to the peer
	//
	PPP_EAP_PACKET		*pSendPacket;

	//
	//	Whether to increment dwRetryCount on retransmissions of the packet.
	//  (When user input is involved, might want to allow infinite retries)
	//
	BOOL				bSendTimeoutInteractive;

	//
	//	Connection specific data currently stored in the registry
	//	This information is passed to RasEapGetIdentity call, so
	//	should be set prior to calling EapSessionGetIdentity using
	//	the call EapSessionSetConnectionData
	//
	PBYTE				pConnectionData;
	DWORD				cbConnectionData;

	//
	//	User-specific data currently stored for this user in the registry
	//	This information is passed to RasEapGetIdentity call, and is
	//	also updated by that call.
	//
	PBYTE				pUserData;
	DWORD				cbUserData;

	//
	//	String identifying the user requesting authentication. This string
	//	is passed to the authentication protocol in PPPEapInput.pwszIdentity
	//	during the call to RasEapBegin.
	//
	//	The string gets set in one of two ways:
	//		Authenticatee: Set during call to RasEapGetIdentity
	//		Authenticator: Set by client Response/Identity packet
	//
	PWCHAR				pwszIdentity;

	//
	//	Authenticatee: Password retrieved from user,
	//                 and flag indicating whether password should be saved
	//                 across sessions.
	//
	PWCHAR				pwszPassword;
	BOOL                bSavePassword;

	//
	//	Cxport event structure used to asynchronously execute UI
	//	tasks on the authenticatee.
	//
	CTEEvent			CTEWorkerEvent;
	BOOL				bWorkerEventQueued;

	//
	//	Copy of packet received that prompted the invocation of the interactive
	//	UI.
	//
	PDECODED_EAP_PACKET	pUIPacket;

	//
	//  Handle to Username/password dialog if it is currently active.
	//
	HWND                hDlgUsernamePassword;

} EAP_SESSION, *PEAP_SESSION;

#define EAP_SESSION_LOCK(pSession)		EnterCriticalSection(&(pSession)->Lock)
#define EAP_SESSION_UNLOCK(pSession)	LeaveCriticalSection(&(pSession)->Lock)

extern DWORD
eapSessionOpenExtension(
	PEAP_SESSION pSession,
	DWORD		 dwExtensionType);

extern DWORD
eapSessionBegin(
	PEAP_SESSION pSession);

extern VOID 
eapSessionInitializeModule();

extern VOID 
eapSessionShutdownModule();

extern BOOL
eapSessionAddRef(
	PEAP_SESSION pSessionToRef);

extern VOID
eapSessionDelRef(
	PEAP_SESSION pSession);

extern PEAP_SESSION
EapSessionCreate(
	IN PVOID			         Context,
	IN DWORD			         fFlags,
	IN BOOL				         fAuthenticator,
	IN BYTE				         bEapTypeToBeUsed,
	IN DWORD                     cbSendPacket,
	IN DWORD                     cbSendHeaderReserved,
	IN PEAP_SESSION_DESCRIPTOR   pDescriptor);

extern VOID
EapSessionDestroy(
	PEAP_SESSION pSession);


extern VOID
EapSessionSendIdentityRequest(
	IN  OUT                        PEAP_SESSION   pSession,
	IN		__in_bcount(cbMessage) PBYTE	      pMessage,
	IN                             DWORD		  cbMessage);

extern DWORD
EapSessionGetIdentity(
	IN	OUT	PEAP_SESSION	pSession,
    IN		HWND			hWndParent,
	IN		PWSTR			pwszEntry,
	IN	OUT PWSTR			pwszIdentity,	
	IN	OUT	PWSTR			pwszPassword,
	IN		BOOL			bAlwaysInvokeUI,
	IN  OUT PBOOL           pbHavePassword,
	    OUT PBOOL           pbCredentialsChanged);

extern VOID
EapSessionProcessRxPacket(
	IN	OUT	PEAP_SESSION pSession,
	IN		PBYTE		 pRxBuffer,
	IN		DWORD		 cbRxBuffer);

extern VOID
EapSessionRxImpliedSuccessPacket(
	IN	OUT	PEAP_SESSION pSession);

extern VOID
EapSessionProcessAuthenticationResult(
	IN	OUT	PEAP_SESSION pSession,
	IN		DWORD		 dwAuthResultCode);

extern DWORD
EapSessionSetConnectionData(
	IN	OUT	PEAP_SESSION pSession,
	IN		PBYTE		 pConnectionData,
	IN		DWORD		 cbConnectionData);

extern DWORD
EapSessionSetUserData(
	IN	OUT	PEAP_SESSION pSession,
	IN		PBYTE		 pUserData,
	IN		DWORD		 cbUserData);

extern DWORD
EapSessionSetIdentity(
	IN	OUT	PEAP_SESSION pSession,
	IN		PWSTR		 pwszIdentity);

extern DWORD
EapSessionSetPassword(
	IN	OUT	PEAP_SESSION pSession,
	IN		PWSTR		 pwszPassword);

extern DWORD
EapSessionReset(
	IN	OUT	PEAP_SESSION pSession);



DWORD
EapSessionFreeMemory(
	IN	OUT	PEAP_SESSION pSession,
	IN      PVOID        pMem);

///////////////////////////////////////
//	Registry utilities functions
///////////////////////////////////////

//
// dwFlags values:
//
#define REG_ALLOC_MEMORY	(1<<0)

DWORD
RegReadValues(
	IN	HKEY	 hKey,
	IN	TCHAR	*tszSubKeyName,	OPTIONAL
	IN	PVOID   (*pfMemAlloc)(DWORD), OPTIONAL
	...);
//
//	Read in a number of registry values.
//
//	For each value to be read in, the following parameters must be passed:
//
//		TCHAR  *tszValueName
//		DWORD	valueType
//		DWORD	dwFlags
//		void   *pValue
//		DWORD	cbValue		// size of space pointed to by pValue
//
//	The list is terminated with a NULL tszValueName
//
//	Returns the count of the number of values that were read in successfully.
//

DWORD
RegWriteValues(
	IN	HKEY	 hKey,
	IN	TCHAR	*tszSubKeyName,	OPTIONAL
	...);
//
//	Write out a number of registry values.
//
//	For each value to be written, the following parameters must be passed:
//
//		TCHAR  *tszValueName
//		DWORD	valueType
//		DWORD	dwFlags
//		void   *pValue
//		DWORD	cbValue		// size of space pointed to by pValue
//
//	The list is terminated with a NULL tszValueName
//
//	Returns the count of the number of values that were written out successfully.
//

LONG
RegDeleteKeyValues(
	HKEY	hKey,
	PWSTR	tszSubKeyName OPTIONAL);
//
//  Delete all the values contained in the specified key.
//

LONG
RegDeleteKeyAndContents(
	IN	HKEY	 hKey,
	IN	TCHAR	*tszSubKeyName);
//
//  A registry key cannot be deleted unless is is empty.
//  This function empties the key and then deletes it.
//


//
//	Enumerate all the subkeys of a key and
//	call the specified function on each
//
DWORD
RegTraverseKey(
	HKEY	hRootKey,
	PWSTR	wszKeyName, OPTIONAL
	DWORD	(*pFunc)(HKEY hKey, PWSTR wszSubKeyName, PVOID pContext),
	PVOID	pContext
	);

#ifdef __cplusplus
}
#endif


