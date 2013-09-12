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

    netlog.h

Abstract:

    retail data logger to allow CE to capture packets.
    Calls out to a helper DLL.

Environment:

    
--*/

// All work is done in the handler API.

#ifndef __NETLOG_H
#define  __NETLOG_H

#ifdef __cplusplus 
extern "C" { 
#endif 

#include <windows.h>
#include <ndis.h>
#include <cxport.h>

// Try to load the logging DLL.
// Invoked at startup and given an IOCTL.
BOOL LoadRetailLogger();

// Try to load the logging DLL.
// Invoked if given an IOCTL.
BOOL UnloadRetailLogger();


typedef
VOID (*LP_LOG_NDIS_SEND)(
    OUT PNDIS_STATUS            Status,
    IN  NDIS_HANDLE             NdisBindingHandle,
    IN  PNDIS_PACKET            Packet
    );

typedef
VOID (*LP_LOG_NDIS_SEND_PACKETS)(
    IN  NDIS_HANDLE             NdisBindingHandle,
    IN  PPNDIS_PACKET           PacketArray,
    IN  UINT                    NumberOfPackets
    );



// For the IndicateReceivePackets Case
typedef VOID 
(*LP_LOG_MINIPORT_INDICATE_RECEIVE_PACKETS)(
    IN  PNDIS_MINIPORT_BLOCK    Miniport,
    IN  PPNDIS_PACKET           PacketArray,
    IN  UINT                    NumberOfPackets,
    IN  NDIS_PHYSICAL_MEDIUM    Medium
                                           );

// For IndicateReceive
typedef VOID 
(*LP_LOG_MINIPORT_INDICATE_RECEIVE)(
    IN NDIS_STATUS              Status,
    IN  NDIS_HANDLE             MacReceiveContext,
    IN  PCHAR                   Address,
    IN  PVOID                   HeaderBuffer,
    IN  UINT                    HeaderBufferSize,
    IN  PVOID                   LookaheadBuffer,
    IN  UINT                    LookaheadBufferSize,
    IN  UINT                    PacketSize,
    IN  NDIS_PHYSICAL_MEDIUM    Medium
    );

typedef VOID 
(*LP_LOG_NDIS_TRANSFER_DATA)(
    OUT PNDIS_PACKET            Packet,
    OUT PUINT                   BytesTransferred,
    IN  NDIS_HANDLE             MiniportAdapterContext,
    IN  NDIS_HANDLE             MiniportReceiveContext,
    IN  UINT                    ByteOffset,
    IN  UINT                    BytesToTransfer
    );

typedef VOID
(*LP_LOG_EVENT)(
	IN  PSTR  szEvent);

// Declared in cxport.
// Do not use directly use the accessor inline functions instead.



// When we're builing   CXPORT  we don't need a declspec.
// When we're building anything else we need it.
#ifdef BUILD_CXPORT
#define  NL_DEC_SPEC  
#else
#define  NL_DEC_SPEC  __declspec(dllimport)
#endif 



NL_DEC_SPEC extern LP_LOG_NDIS_SEND g_pLogNdisSend;
NL_DEC_SPEC extern LP_LOG_NDIS_SEND_PACKETS g_pLogNdisSendPackets;
NL_DEC_SPEC extern LP_LOG_NDIS_TRANSFER_DATA g_pLogNdisTransferData;

NL_DEC_SPEC extern LP_LOG_MINIPORT_INDICATE_RECEIVE  g_pLogMiniportIndicateReceive;
NL_DEC_SPEC extern LP_LOG_MINIPORT_INDICATE_RECEIVE_PACKETS g_pLogMiniportIndicateReceivePackets;

NL_DEC_SPEC extern LP_LOG_EVENT g_pLogEvent;


#undef NL_DEC_SPEC
//////////////////////////////////////////////////////
//
// Here are the functions to be used in NDIS and PPP
//
//////////////////////////////////////////////////////

inline void  LogNdisSend(
    OUT PNDIS_STATUS            Status,
    IN  NDIS_HANDLE             NdisBindingHandle,
    IN  PNDIS_PACKET            Packet
    )
{
    if (g_pLogNdisSend)
    {
        g_pLogNdisSend(Status,NdisBindingHandle,Packet);
    }
    return ;
}

inline VOID LogNdisSendPackets(
    IN  NDIS_HANDLE             NdisBindingHandle,
    IN  PPNDIS_PACKET           PacketArray,
    IN  UINT                    NumberOfPackets
    )
{
    if (g_pLogNdisSendPackets)
    {
        g_pLogNdisSendPackets(NdisBindingHandle,PacketArray,NumberOfPackets);
    }
    return ;

}

inline void  LogNdisTransferData(
    OUT PNDIS_PACKET            Packet,
    OUT PUINT                   BytesTransferred,
    IN  NDIS_HANDLE             MiniportAdapterContext,
    IN  NDIS_HANDLE             MiniportReceiveContext,
    IN  UINT                    ByteOffset,
    IN  UINT                    BytesToTransfer
    )

{
    if (g_pLogNdisTransferData)
    {
        g_pLogNdisTransferData(Packet,BytesTransferred,MiniportAdapterContext,MiniportReceiveContext,ByteOffset,BytesToTransfer);
    }
    return;
}
inline void  LogMiniportIndicateReceive(
    IN  NDIS_STATUS              Status,
    IN  NDIS_HANDLE             MacReceiveContext,
    IN  PCHAR                   Address,
    IN  PVOID                   HeaderBuffer,
    IN  UINT                    HeaderBufferSize,
    IN  PVOID                   LookaheadBuffer,
    IN  UINT                    LookaheadBufferSize,
    IN  UINT                    PacketSize,
    IN  NDIS_PHYSICAL_MEDIUM    Medium
    )
{
    if (g_pLogMiniportIndicateReceive)
    {
        g_pLogMiniportIndicateReceive(
                Status,
                MacReceiveContext,
                Address,
                HeaderBuffer,
                HeaderBufferSize,
                LookaheadBuffer,
                LookaheadBufferSize,
                PacketSize,
                Medium );

    }
    return ;
}

inline void  LogMiniportIndicateReceivePackets(
    IN  PNDIS_MINIPORT_BLOCK    Miniport,
    IN  PPNDIS_PACKET           PacketArray,
    IN  UINT                    NumberOfPackets,
    IN  NDIS_PHYSICAL_MEDIUM    Medium)
{
    if (g_pLogMiniportIndicateReceivePackets)
    {
        g_pLogMiniportIndicateReceivePackets (
                Miniport,PacketArray,NumberOfPackets,Medium);

    }
    return;
}

inline void  LogEvent(
    IN  PSTR szEvent)
{
    if (g_pLogEvent)
        g_pLogEvent(szEvent);
}

//
//  Define our IOCTL Interface.
//



enum NETLOG_IOCTL 
{
    // Dump all stats to debug out.
    IOCTL_NETLOG_LOAD = CXPORT_IOCTL_NETLOG_BASE ,
    IOCTL_NETLOG_UNLOAD,
	IOCTL_CXLOG_GET_MODULE_INFO,
	IOCTL_CXLOG_SET_FILTER
};

#ifdef __cplusplus 
}
#endif 

#endif // ifdef __NETLOG_H
