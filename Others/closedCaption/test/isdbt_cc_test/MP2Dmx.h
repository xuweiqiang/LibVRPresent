//=============================================================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

//
// Open action items:
// Handling conditional access.
// Integrating program stream demux.
//

#ifndef _MP2Dmx_H_
#define _MP2Dmx_H_

#include <windows.h>

#ifndef E_PROP_ID_UNSUPPORTED
#define E_PROP_ID_UNSUPPORTED     ((HRESULT)0x80070490L)
#endif

#ifndef COOKIE
#define COOKIE LPVOID
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void *MP2Dmx_HANDLE;

// this is passed back as status flags to the listener.

enum MP2DMX_TSFLAGS
{
	MP2DMX_TSFLAGS_NONE,
	MP2DMX_TSFLAGS_DISCONTINUOUS_PACKET = (1<<0),	// the continuity counter is not correct with previous packet.
	MP2DMX_TSFLAGS_DIRTY_PACKET = (1<<1), // no transport_error_indicator, but find error in the header
	
	//
	MP2DMX_TSFLAGS_TRANSPORT_PRIORITY = (1<<5),
	MP2DMX_TSFLAGS_PAYLOAD_UNIT_START_INDICATOR = (1<<6),
	MP2DMX_TSFLAGS_TRANSPORT_ERROR = (1<<7),

	//
	MP2DMX_TSFLAGS_CONTINUITY_COUNTER = (1<<8),	//4 bits
	MP2DMX_TSFLAGS_ADAPTION_FIELD_CONTROL = (1<<(8+4)),
	MP2DMX_TSFLAGS_PAYLOAD_BIT = (1<<(8+4)),
	MP2DMX_TSFLAGS_ADAPTATION_FIELD_BIT = (1<<(8+5)),
	MP2DMX_TSFLAGS_TRANSPORT_SCRAMBLING_CONTROL = (1<<(8+6)),	//2 bits

	//first 8 bits in adaptation_field
	MP2DMX_TSFLAGS_ADAPTATION_FIELD_EXTENSION_FLAG = (1<<16),
	MP2DMX_TSFLAGS_TRANSPORT_PRIVATE_DATA_FLAG = (1<<17),
	MP2DMX_TSFLAGS_SPLICING_POINT_FLAG = (1<<18),
	MP2DMX_TSFLAGS_OPCR_FLAG = (1<<19),
	MP2DMX_TSFLAGS_PCR_FLAG = (1<<20),
	MP2DMX_TSFLAGS_ELEMENTARY_STREAM_PRIORITY_INDICATOR = (1<<21),
	MP2DMX_TSFLAGS_RANDOM_ACCESS_INDICATOR = (1<<22),
	MP2DMX_TSFLAGS_DISCONTINUITY_INDICATOR = (1<<23),

	//PES flag
	MP2DMX_PES_DATA_ALIGNMENT_INDICATOR = (1<<24),	
	
	MP2DMX_PES_ESCR_PRESENT = (1<<25),
	MP2DMX_PES_DTS_PRESENT = (1<<26),
	MP2DMX_PES_PTS_PRESENT = (1<<27),
};


typedef HRESULT
(*PFN_MP2DMX_LISTENER)(
    IN PVOID pvContext,
	IN DWORD dwID,				// replicate the matching stream id or program id
	IN DWORD dwStatusFlags,		// status flags (MP2DMX_TSFLAGS for transport streams; MP2DMX_PSFLAGS for program streams)
	IN PBYTE pHeader,			// header corresponding to data (may match pData if type is TSPACKET or PSPACKET)
	IN PBYTE pOutputData,		// "payload"
	IN DWORD dwSize,
	IN PVOID pExtraData,
	IN DWORD dwExtraDataSize
	);

struct MP2Dmx_ExtraData
{	
	ULONGLONG ullPts;
	ULONGLONG ullDts;
	ULONGLONG ullEscr;
	DWORD	  dwPacketLength;
	DWORD	  dwPacketHeaderLength;
	DWORD Reserve[16];
};

enum MP2DMX_OPENFLAGS
{
	MP2DMX_OPENFLAGS_NONE,
	MP2DMX_OPENFLAGS_PSSTREAM	= (1<<0),						// program stream - not supported yet
	MP2DMX_OPENFLAGS_TSSTREAM	= (1<<1),						// transport stream
	MP2DMX_OPENFLAGS_STRIDE		= (1<<3),						// Sets stride, e.g. for error correcting codes (not implemented)
};

struct MP2Dmx_OpenOptions
{
	DWORD dwOpenFlags;
    PVOID pvInputDataContext;
	DWORD dwPacketOffset;										/// generally 0 (only valid if MP2DMX_OPENFLAGS_STRIDE set)
	DWORD dwPacketLength;										/// generally 188 (only valid if MP2DMX_OPENFLAGS_STRIDE set)
	DWORD dwPacketStride;										/// generally 188 (only valid if MP2DMX_OPENFLAGS_STRIDE set)
	DWORD Reserve[16];
};

enum MP2DMX_PIDFLAGS
{
	MP2DMX_PIDFLAGS_NONE,
	MP2DMX_PIDFLAGS_DELIVER_TSPACKET = (1<<0),				// entire packet
	MP2DMX_PIDFLAGS_DELIVER_TSPAYLOAD = (1<<1),				// payload of packet
	MP2DMX_PIDFLAGS_DELIVER_TSELEMENTARY = (1<<2),			// elementary stream (if PES)
	MP2DMX_PIDFLAGS_DELIVER_TSSECTIONS = (1<<3),	 		// section info (PSI)
	MP2DMX_PIDFLAGS_DELIVER_TSPCR = (1<<4),					// PCR only (8 byte ULONGLONG)
	MP2DMX_PIDFLAGS_DELIVER_TSTYPE = (1<<5),				// TYPE information (4 byte DWORD) (not implemented yet)
	MP2DMX_PIDFLAGS_DELIVER_COPY = (1<<16),					// copy on delivery (not implemented yet)
};

struct MP2Dmx_AddListenerOptions
{
	DWORD dwFlags;									// MP2DMX_PIDFLAGS
	DWORD dwID;
	PFN_MP2DMX_LISTENER pListener;
	PVOID pvListenerContext;
	DWORD Reserve[16];
};

enum MP2DMX_STOPFLAGS
{
	MP2DMX_STOPFLAGS_NONE,			
	MP2DMX_STOPFLAGS_FLUSHINPUT = (1<<0),	// flushes input buffers
	MP2DMX_STOPFLAGS_RESETLISTENERS = (1<<1),	//resets listener state
	MP2DMX_STOPFLAGS_ALL = (~0)
};

//
// Creates a new demux object with a reference count
// of 1 and stores a pointer to it in ppDemux.
//
// This object needs to be freed by MP2Dmx_Release().
//
HRESULT
MP2Dmx_Create(
    OUT MP2Dmx_HANDLE *phdmx
    );

//
// Decrements reference count of the demux object. 
// If the reference count goes to 0, frees up all
// resources used by the demux.
//
// pDemux - Demux object created with MP2Dmx_Create()
//
ULONG
MP2Dmx_Release(
    IN MP2Dmx_HANDLE hDmx
    );

//
// Adds a reference count to the demux.
//
// pDemux - Demux object created with MP2Dmx_Create()
//
ULONG
MP2Dmx_AddRef(
    IN MP2Dmx_HANDLE hDmx
    );

//
// Initializes the demux with the OpenOptions.
// This is required after Creation of the demux 
// object, before any demuxing occurs.
// 
HRESULT
MP2Dmx_Open(
    IN MP2Dmx_HANDLE hDmx,
	IN  MP2Dmx_OpenOptions *pOptions,
	IN  DWORD dwSize
	);

//
// Deallocates any resources held by the demux.
// This frees runtime resources used by the demux
// 
HRESULT
MP2Dmx_Close(
    IN MP2Dmx_HANDLE hDmx
	);

//
// Halts current demultiplexing, releases all
// input buffers held within the demux.
// 
HRESULT
MP2Dmx_Stop(
    IN MP2Dmx_HANDLE hDmx,
	IN DWORD dwStopFlags
	);


//
// Adds a listener.
// This function returns a cookie which is used
// to remove thelistener.
// 
HRESULT
MP2Dmx_AddListener(
    IN MP2Dmx_HANDLE hDmx,
	IN MP2Dmx_AddListenerOptions *pOptions,
	IN DWORD dwSize,						// option size
	OUT COOKIE *pListenerCookie
	);

//
// Removes a listener.
// 
HRESULT
MP2Dmx_RemoveListener(
    IN MP2Dmx_HANDLE hDmx,
	IN COOKIE ListenerCookie
	);


//
// Demultiplexes an arbitrary piece of data. 
// The data, if fractional packet, will be
// put into an internally allocated buffer.
// If no such buffer exists, and the mode
// is unblocking, an error E_WOULD_BLOCK
// will be returned.  Otherwise, the demux 
// will wait for the buffer to be released.
//
// pDemux - demux object created with MP2Dmx_Create()
//
// The InputDataCookie variable is
// called with by pfnReleaseInputDataCallback 
// or pfnAddRefInputDataCallback if the 
// buffer be able to be released or addrefed.
//
HRESULT
MP2Dmx_ProcessInputData(
    IN MP2Dmx_HANDLE hDmx,
	IN PBYTE pInputData,
	IN DWORD dwSize	
    );

//
// Gets a property from the demux. 
// This function returns E_PROP_ID_UNSUPPORTED
// if the property ID is unsupported for the Get function.  
// This returns E_FAIL if the property ID is supported
// but either the cbPropData and pPropData are unmatched or
// the Get fails.
//
// QuerySupported can be implemented by a "Get" with cbPropData and pPropData
// set to 0 and testing whether the return value is E_PROP_ID_UNSUPPORTED
// or E_FAIL.
//
// There is no guid propset.  This should be taken care of by
// routing in the application or system framework layer.
//
HRESULT
MP2Dmx_Get(
    IN MP2Dmx_HANDLE hDmx,
	IN DWORD dwPropID,
	OUT LPVOID pPropData,
	IN DWORD cbPropData,
	OUT DWORD *pcbReturned
	);


//
// Sets a property in the demux. 
// This function returns E_PROP_ID_UNSUPPORTED
// if the property ID is unsupported for the Set function.  
// This returns E_FAIL if the property ID is supported
// but either the cbPropData and pPropData are unmatched or
// the Set fails.
//
// QuerySupported can be implemented by a "Set" with cbPropData and pPropData
// set to 0 and testing whether the return value is E_PROP_ID_UNSUPPORTED
// or E_FAIL.
//
// There is no guid propset.  This should be taken care of by
// routing in the application or system framework layer.
//
HRESULT
MP2Dmx_Set(
    IN MP2Dmx_HANDLE hDmx,
	IN DWORD dwPropID,
	IN LPVOID pPropData,
	IN DWORD cbPropData
	);


#ifdef __cplusplus
}
#endif

#endif // _MP2Dmx_H_
