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

natedit.h

Abstract:

@doc NATEDIT

    This file defines the structures, macros, and functions used by editor modules
    for the Network Address Translation (NAT) component.  

Notes: 

@topic  Network Address Translation (NAT) protocol editor modules |

    The NAT driver is responsible for translating address and port information for
    TCP and UDP traffic to enable scenarios such as Internet Connection Sharing.  Certain
    kinds of protocol traffic, however, cannot be translated by the NAT without further information.
    Examples of protocols of this type are:
    
    o  Protocols that involve unsolicited connection requests (or UDP data channels) from
       the public network.
       
    o  Protocols that embed network address or port information in the data stream.
   
    Examples of protocols which have such characteristics are FTP and PPTP.  These 
    2 protocols are handled by the Windows CE NAT driver internally. To handle other
    protocols which have such requirements, external "editor" modules are 
    required.
    
    When the NAT driver loads at system initialization, it will read the registry
    value HKEY_LOCAL_MACHINE\\Comm\\IPNat\\EditorList, which is a MULTI_SZ value containing a list of
    editor DLLs.  Each DLL in this list will be loaded, and the exported EditorInitialize()
    function will be called.  From within this routine (or at another time), an 
    editor should call the supplied <f RegisterEditor> function provided by the NAT
    driver to register itself.

--*/

#ifndef _NATEDIT_H_
#define _NATEDIT_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// @topic MISCELLANEOUS DECLARATIONS   | Miscellaneous definitions used by NAT editors
//

#define IP_NAT_VERSION          1

// @struct IPRcvBuf | IP receive buffer chain
// 
// This structure represents a chain of buffers corresponding to an IP packet.
// Fields in this structure should not be directly modified by NAT editors.  Instead, 
// editors should call the <f EditSession> helper function to modify IP packet data.

struct	IPRcvBuf {
	struct	IPRcvBuf	*ipr_next;				// @field Next buffer descriptor in chain.
	uint				ipr_reserved1;			// @field Reserved
	uchar				*ipr_buffer;			// @field Pointer to buffer.
	uint				ipr_size;				// @field Buffer size.
	uint				ipr_reserved2;          // @field Reserved

}; /* IPRcvBuf */

typedef struct IPRcvBuf IPRcvBuf, *PIPRCVBUF;

//
// IP header protocol-field constants
//

#define NAT_PROTOCOL_ICMP       0x01
#define NAT_PROTOCOL_IGMP       0x02
#define NAT_PROTOCOL_TCP        0x06
#define NAT_PROTOCOL_UDP        0x11
#define NAT_PROTOCOL_PPTP       0x2F

// @enum IP_NAT_DIRECTION | Specifies whether editor is interested in inbound or outbound sessions
typedef enum {
    NatInboundDirection = 0,  // @emem Specifies inbound sessions (i.e. from public -> private)
    NatOutboundDirection,     // @emem Specifies outbound sessions (i.e. from private -> public)
    NatMaximumDirection
} IP_NAT_DIRECTION, *PIP_NAT_DIRECTION;


//
// @topic Editor functions | The following routines are provided by NAT editors when they
// call the <f RegisterEditor> function supplied by the NAT driver (some of the functions
// are optional).  After the <f RegisterEditor> function has been called, subsequent events will 
// trigger the NAT driver to call back the editor, supplying the editor context and other
// information.

//
// @func NTSTATUS | CreateHandler | Editor function called by the NAT driver when a mapping 
//       is created associated with a registered editor.
//
// @comm   For synchronization reasons, <f CreateHandler> and <f DeleteHandler>
//         CANNOT invoke any helper functions other than <f QueryInfoSession>.
//
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_CREATE_HANDLER)(
    IN PVOID EditorContext,          // @parm Context supplied by editor in <f RegisterEditor>
    IN ULONG PrivateAddress,         // @parm Private address for mapping
    IN USHORT PrivatePort,           // @parm Private port for mapping
    IN ULONG PublicAddress,          // @parm Public address for mapping
    IN USHORT PublicPort,            // @parm Public port for mapping
    IN ULONG RemoteAddress,          // @parm Remote host address for mapping
    IN USHORT RemotePort,            // @parm Remote host port for mapping
    OUT PVOID* EditorSessionContextp // @parm [OPTIONAL] If specified, context to pass to subsequent callback routines associated with this session mapping (e.g. <f DeleteHandler>)
    );


//
// @func NTSTATUS | DeleteHandler | Called by the NAT driver when a mapping associated with editor is removed
// @comm   For synchronization reasons, <f CreateHandler> and <f DeleteHandler>
//         CANNOT invoke any helper functions other than <f QueryInfoSession>.
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_DELETE_HANDLER)(
    IN PVOID InterfaceHandle,     // @parm Handle to Interface
    IN PVOID SessionHandle,       // @parm Handle to session (e.g. for calling <f QueryInfoSession>)
    IN PVOID EditorContext,       // @parm Context supplied by editor in <f RegisterEditor>
    IN PVOID EditorSessionContext // @parm Context supplied by editor in the <f CreateHandler> routine
    );


//
// @func NTSTATUS | DataHandler | Called by the NAT driver for each packet received on session controlled by an editor
// @comm  This function is optional, and may not need to be implemented for all protocol editors.
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_DATA_HANDLER)(
    IN PVOID InterfaceHandle,     // @parm Handle to interface
    IN PVOID SessionHandle,       // @parm Handle to session 
    IN PVOID DataHandle,          // @parm Per packet data context (e.g. for calling <f EditSession>)
    IN PVOID EditorContext,       // @parm Context supplied by editor in <f RegisterEditor>
    IN PVOID EditorSessionContext,// @parm Context supplied by editor in the <f CreateHandler> routine
    IN PIPRCVBUF RecvBuffer,      // @parm Received packet data
    IN ULONG DataOffset           // @parm Offset of the protocol data in <p RecvBuffer>
    );

//
// @topic NAT helper functions | The following functions are supplied by the NAT driver to 
// assist editors in packet mapping and translation.   Pointers to the functions are returned in the call
// to <f RegisterEditor>.
//

//
// @func NTSTATUS | CreateTicket | Create a NAT ticket for an inbound session
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_CREATE_TICKET)(
    IN PVOID InterfaceHandle,       // @parm Handle to interface
    IN UCHAR Protocol,              // @parm IP header protocol field
    IN ULONG PrivateAddress,        // @parm Private IP address to translate session to
    IN USHORT PrivatePort,          // @parm Private port to translate session to
    IN ULONG RemoteAddress,         // @parm [OPTIONAL] If specified, create mapping only on sessions from this address
    IN USHORT RemotePort,           // @parm [OPTIONAL] If specified, create mapping only on sessions from this port
    OUT PULONG PublicAddress,       // @parm Filled in with public address that will be used in mapping
    OUT PUSHORT PublicPort          // @parm Filled in with port that will be used in mapping
    );


//
// @func NTSTATUS | DeleteTicket | Called to delete a NAT ticket
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_DELETE_TICKET)(
    IN PVOID InterfaceHandle,       // @parm Handle to interface
    IN ULONG PublicAddress,         // @parm Address of the ticket's public endpoint
    IN UCHAR Protocol,              // @parm NAT_PROTOCOL_TCP or NAT_PROTOCOL_UDP
    IN USHORT PublicPort,           // @parm Port of the ticket's public endpoint
    IN ULONG RemoteAddress,         // @parm [OPTIONAL] If specified, remote address for ticket
    IN USHORT RemotePort            // @parm [OPTIONAL] If specified, remote port for ticket
    );

//
// @func NTSTATUS | EditorDeregister | Called to deregister a NAT editor
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_DEREGISTER)(
    IN PVOID EditorHandle  // @parm Handle to the editor, supplied by editor in call to <f RegisterEditor> 
    );

//
// @func NTSTATUS | DisassociateSession | Called to disassociate an editor from a specific session.
// @comm  Calling this function will cause the NAT driver to stop calling the editor data
//        handler routines for the session, however the mapping will be preserved, and
//        standard NAT translation will continue for packets on the session.  Useful for 
//        protocols which require a specific packet to be edited early on in a session, but
//        not subsequent packets.
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_DISSOCIATE_SESSION)(
    IN PVOID EditorHandle,    // @parm Handle to the editor, supplied by editor in call to <f RegisterEditor> 
    IN PVOID SessionHandle    // @parm Handle to session, supplied by NAT driver in the <f CreateHandler>     
    );


//
// @func NTSTATUS | EditSession | This routine is invoked by an editor to replace one range of bytes 
//        in a packet with another range of bytes.
// @comm  The routine makes the necessary adjustments to TCP sequence numbers
//        if the edition alters the size of a TCP segment.  It is called from within the editor
//        <f DataHandler> routine.
//
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef NTSTATUS
(*PNAT_EDITOR_EDIT_SESSION)(
    IN PVOID DataHandle,     // @parm Per packet context passed to 'DataHandler'
    IN PIPRCVBUF RecvBuffer, // @parm The 'RecvBuffer' argument passed to 'DataHandler'
    IN ULONG OldDataOffset,  // @parm Offset into <p RecvBuffer> of the range to be replaced
    IN ULONG OldDataLength,  // @parm Length of range to be replaced
    IN PUCHAR NewData,       // @parm Pointer to bytes to serve as a replacement for <p OldData>
    IN ULONG NewDataLength   // @parm Number of bytes in replacement range
    );


//
// @struct   IP_NAT_SESSION_MAPPING_STATISTICS  | Session statistics
//
// This structure holds statistics for a single session-mapping. Returned from
// <f QueryInfoSession> call.
//

typedef struct _IP_NAT_SESSION_MAPPING_STATISTICS {
    ULONG64 BytesForward;    // @field  Number of bytes sent on the forward channel
    ULONG64 BytesReverse;    // @field  Number of bytes sent on the reverse channel
    ULONG64 PacketsForward;  // @field  Number of packets sent on the forward channel
    ULONG64 PacketsReverse;  // @field  Number of packets sent on the reverse channel
    ULONG64 RejectsForward;  // @field  Number of packets rejected on forward channel
    ULONG64 RejectsReverse;  // @field  Number of packets rejected on reverse channel
} IP_NAT_SESSION_MAPPING_STATISTICS, *PIP_NAT_SESSION_MAPPING_STATISTICS;


//
// @func NTSTATUS | QueryInfoSession | This routine is invoked by an editor to get session statistics
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef VOID
(*PNAT_EDITOR_QUERY_INFO_SESSION)(
    IN PVOID SessionHandle,
    OUT PULONG PrivateAddress,             // @parm [OPTIONAL] Receives the IP address of the session's private endpoint
    OUT PUSHORT PrivatePort,               // @parm [OPTIONAL] Receives the protocol port of the session's private endpoint
    OUT PULONG RemoteAddress,              // @parm [OPTIONAL] Receives the IP address of the session's remote endpoint
    OUT PUSHORT RemotePort,                // @parm [OPTIONAL] Receives the protocol port of the session's remote endpoint
    OUT PULONG PublicAddress,              // @parm [OPTIONAL] Receives the IP address of the session's public endpoint
    OUT PUSHORT PublicPort,                // @parm [OPTIONAL] Receives the protocol port of the session's public endpoint
    OUT PIP_NAT_SESSION_MAPPING_STATISTICS Statistics   // @parm [OPTIONAL] Receives statistics for the session
    );


//
// @func NTSTATUS | TimeoutSession | This routine is invoked by an editor to indicate that session should be timed out
// @comm  Marks the session to be cleaned up, and the mapping destroyed by the NAT driver.  Once
//        this occurs, no further translation will happen on the session.  The actual removal is done
//        asynchronously in the NAT driver, so may not happen immediately.
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
typedef VOID
(*PNAT_EDITOR_TIMEOUT_SESSION)(
    IN PVOID EditorHandle,   // @parm Handle to the editor, supplied by editor in call to <f RegisterEditor>
    IN PVOID SessionHandle   // @parm Handle to session, supplied by NAT driver in the <f CreateHandler> 
    );

//
// @struct  IP_NAT_REGISTER_EDITOR | Passed to NAT RegisterEditor function
//
// The editor uses this structure to register itself with the NAT,
// and to obtain entrypoints of helper-functions provided by the NAT.
//
// On input, 'EditorContext' should contain a value which the NAT will
// pass to the editor's provided functions to serve as identification.
//
// On output, 'EditorHandle' contains the handle which the editor should
// pass to the NAT's helper functions to identify itself.
// 

typedef struct _IP_NAT_REGISTER_EDITOR {
    IN ULONG Version;             // @field Set to IP_NAT_VERSION
    IN ULONG Flags;               // @field Set to IP_NAT_EDITOR_FLAGS_RESIZE if editor will need to potentially change the size of packets
    IN UCHAR Protocol;            // @field NAT_PROTOCOL_TCP or NAT_PROTOCOL_UDP
    IN USHORT Port;               // @field Specifies TCP/UDP port to edit
    IN IP_NAT_DIRECTION Direction;// @field Either NatInboundDirection or NatOutboundDirection
    IN PVOID EditorContext;       // @field Context to pass to editor routines
    IN PNAT_EDITOR_CREATE_HANDLER CreateHandler;    // @field [OPTIONAL] routine to call on mapping creation
    IN PNAT_EDITOR_DELETE_HANDLER DeleteHandler;    // @field [OPTIONAL] routine to call when mappings are deleted
    IN PNAT_EDITOR_DATA_HANDLER ForwardDataHandler; // @field [OPTIONAL] routine to call on each forward channel packet
    IN PNAT_EDITOR_DATA_HANDLER ReverseDataHandler; // @field [OPTIONAL] routine to call on each reverse channel packet
    OUT PVOID EditorHandle;        // @field Filled in with handle to identify editor to NAT module
    OUT PNAT_EDITOR_CREATE_TICKET CreateTicket; // @field Filled in with helper function address
    OUT PNAT_EDITOR_DELETE_TICKET DeleteTicket; // @field Filled in with helper function address
    OUT PNAT_EDITOR_DEREGISTER Deregister;      // @field Filled in with helper function address
    OUT PNAT_EDITOR_DISSOCIATE_SESSION DissociateSession;  // @field Filled in with helper function address
    OUT PNAT_EDITOR_EDIT_SESSION EditSession;        // @field Filled in with helper function address
    OUT PNAT_EDITOR_QUERY_INFO_SESSION QueryInfoSession; // @field Filled in with helper function address
    OUT PNAT_EDITOR_TIMEOUT_SESSION TimeoutSession;      // @field Filled in with helper function address
} IP_NAT_REGISTER_EDITOR, *PIP_NAT_REGISTER_EDITOR;

#define IP_NAT_EDITOR_FLAGS_RESIZE      0x00000001

//
// @func NTSTATUS | RegisterEditor | Pointer to function called by NAT editors to register themselves with the NAT driver
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
//
typedef NTSTATUS 
(*PNAT_REGISTER_EDITOR)( 
    IN PIP_NAT_REGISTER_EDITOR RegisterContext   // @parm Protocol and function information
    );  

//
// @func NTSTATUS | EditorInitialize | Entry point for NAT editors 
// @comm This function must be exported by all NAT editors.  It will be called during system init, when
//       the NAT driver loads the editor list.
//        
// @rdesc Returns STATUS_SUCCESS if successful, non zero status code if not
// 
NTSTATUS
EditorInitialize(
    IN OUT PNAT_REGISTER_EDITOR RegisterEditor     // @parm Contains pointer to <f RegisterEditor> function within the NAT driver
    );





#ifdef UNDER_CE

//
//  The following LOG functions are called by NAT when a packet is dropped,
//    connection is made, or deleted.  They are all on the critical path in NAT and 
//    therefore some form of queuing (via a low priority thread) probably should be
//    implemented in the DLL to avoid performance loss.  The DLL is registered
//    by adding the following registry string:
//         [HKLM\Comm\IPNat]
//             FirewallLoggerDll="your.dll"
//
//    the following are the function names (signatures are below)
//             "LogDroppedPacket"
//             "LogConnectionCreation"
//             "LogConnectionDeletion"
//             "LogInit"
//


//
// @func PNat_LOG_DROPPED_PACKET -- function signature for log dll 
// @comm  this function is used by client dlls that wish to monitor dropped 
//        packets -- please note that this function is on the critical
//        path in NAT and therefore must return quickly to prevent performance
//        loss
//        
// @rdesc return type void
// 
typedef VOID (*PNAT_LOG_DROPPED_PACKET)( DWORD SourceAddr,  
                                          DWORD DestAddr, 
                                          USHORT SourcePort, 
                                          USHORT DestPort, 
                                          DWORD PacketSize,
                                          BYTE Protocol,
                                          DWORD Proto1, 
                                          DWORD Proto2,
                                          DWORD Proto3, 
                                          DWORD Proto4);

//
// @func PNAT_LOG_CONNECTION_DELETION -- function signature for log dll 
// @comm  this function is used by client dlls that wish to monitor connection deletion
//        -- please note that this function is on the critical
//        path in NAT and therefore must return quickly to prevent performance
//        loss
//        
// @rdesc return type void
// 
typedef VOID (*PNAT_LOG_CONNECTION_DELETION) (DWORD LocalAddress,
                                            DWORD RemoteAddress,
                                            USHORT LocalPort,
                                            USHORT RemotePort,
                                            UCHAR Protocol,
                                            BOOLEAN InboundConnection);

//
// @func PNAT_LOG_CONNECTION_CREATION -- function signature for log dll 
// @comm  this function is used by client dlls that wish to monitor connection creation
//        -- please note that this function is on the critical
//        path in NAT and therefore must return quickly to prevent performance
//        loss
//        
// @rdesc return type void
// 
typedef VOID  (*PNAT_LOG_CONNECTION_CREATION) (
                                        DWORD LocalAddress,
                                        DWORD RemoteAddress,
                                        USHORT LocalPort,
                                        USHORT RemotePort,
                                        UCHAR Protocol,
                                        BOOLEAN InboundConnection);

//
// @func PNAT_LOG_CONNECTION_CREATION -- function signature for log dll 
// @comm  this function is called once and can be used by used by 
//        a logging client to do initialization
//        
// @rdesc return type void
// 
typedef VOID  (*PNAT_LOG_INIT) ();

#endif



#ifdef __cplusplus
}
#endif

#endif // _NATEDIT_H_
