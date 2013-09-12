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
#ifndef _NMPIOCTL_H_
#define _NMPIOCTL_H_

#ifdef __cplusplus
extern "C"	{
#endif

#define NMP_IOCTL_CreatePipe					10
#define NMP_IOCTL_ConnectNamedPipe				11
#define NMP_IOCTL_DisconnectNamedPipe			12
#define NMP_IOCTL_SetNamedPipeHandleState		13
#define NMP_IOCTL_GetNamedPipeInfo				14
#define NMP_IOCTL_PeekNamedPipe					15
#define NMP_IOCTL_TransactNamedPipe				16
#define NMP_IOCTL_CreateNamedPipe				17
#define NMP_IOCTL_GetNamedPipeHandleState		18
#define NMP_IOCTL_CallNamedPipe					19
#define NMP_IOCTL_WaitNamedPipe					20
#define NMP_IOCTL_ReadNamedPipe					21
#define NMP_IOCTL_WriteNamedPipe				22
#define NMP_IOCTL_CloseNamedPipe				23
#define NMP_IOCTL_QuerySessionKeyOfBinding      24

// length of session key retrieved in FSCTL_PIPE_GET_USER_SES_KEY IOCTL
#define NMP_SESSION_KEY_LEN                     16

typedef union 
{
	struct ConnectDisconnectOrClose
	{
		HANDLE					hNamedPipe;
	} apiConnectDisconnectOrClose;

	struct SetNamedPipeHandleState
	{
		HANDLE		hNamedPipe;
		DWORD		dwMode;
		DWORD		dwMaxCollectionCount;
		DWORD		dwCollectDataTimeout;
	} apiSetNamedPipeHandleState;

	struct GetNamedPipeInfo
	{
		HANDLE		hNamedPipe;
		DWORD		dwFlags;
		DWORD		dwOutBufSize;
		DWORD		dwInBufSize;
		DWORD		dwMaxInstances;
	} apiGetNamedPipeInfo;

	struct TransactNamedPipe
	{
		HANDLE		hNamedPipe;
		LPVOID		lpInBuf;
		DWORD		dwInBufSize;
		LPVOID		lpOutBuf;
		DWORD		dwOutBufSize;
		DWORD		dwBytesRead;
	} apiTransactNamedPipe;

	struct CreateNamedPipe
	{
		LPCWSTR					lpName;
		DWORD					dwOpenMode;
		DWORD					dwPipeMode;
		DWORD					dwMaxInstances;
		DWORD					dwOutBufSize;
		DWORD					dwInBufSize;
		DWORD					dwDefTimeout;
		HANDLE					hNamedPipe;		//out param
	} apiCreateNamedPipe;

	struct GetNamedPipeHandleState
	{
		HANDLE		hNamedPipe;
		DWORD		dwState;
		DWORD		dwCurInstances;
		DWORD		dwMaxCollectionCount;
		DWORD		dwCollectDataTimeout;
		LPWSTR		dwUserName;
		DWORD		nMaxUserNameSize;
	} apiGetNamedPipeHandleState;

	struct CallNamedPipe
	{
		LPWSTR		lpPipeName;
		LPVOID		lpInBuf;
		DWORD		dwInBufSize;
		LPVOID		lpOutBuf;
		DWORD		dwOutBufSize;
		DWORD		dwBytesRead;
		DWORD		dwTimeout;
	} apiCallNamedPipe;
	
	struct WaitNamedPipe
	{
		LPWSTR		lpPipeName;
		DWORD		dwTimeout;
	} apiWaitNamedPipe;
	
	struct ReadWriteNamedPipe
	{
		HANDLE		hNamedPipe;
		LPVOID		lpBuffer;
		DWORD		dwBufSize;
		DWORD		dwBytesOut;
	} apiReadWriteNamedPipe;

	struct QuerySessionKeyForBinding
	{
		LPWSTR		lpPipeName;
		BYTE        bSessionKey[NMP_SESSION_KEY_LEN];
	} apiQuerySessionKeyForBinding;

} NAMEDPIPEAPICALL;



#define NAMED_PIPE_IOCTL(index)			CTL_CODE(FILE_DEVICE_NAMED_PIPE, (index), METHOD_BUFFERED, FILE_ANY_ACCESS)


//the desktop has similarly named ioctls...so use indices from 32 to avoid any potential conflict
#define FSCTL_PIPE_GET_HANDLE_STATE			NAMED_PIPE_IOCTL(32)
#define FSCTL_PIPE_SET_HANDLE_STATE			NAMED_PIPE_IOCTL(33)
#define FSCTL_PIPE_GET_NAMED_PIPE_INFO		NAMED_PIPE_IOCTL(34)
#define FSCTL_PIPE_TRANSACT					NAMED_PIPE_IOCTL(35)
#define FSCTL_PIPE_READ						NAMED_PIPE_IOCTL(36)
#define FSCTL_PIPE_WRITE					NAMED_PIPE_IOCTL(37)
#define FSCTL_PIPE_GET_USER_SES_KEY         NAMED_PIPE_IOCTL(38)


//
// Transaction - QueryInformationNamedPipe, Level 1, output data format
//
#pragma pack(1)

typedef struct NAMED_PIPE_INFORMATION_1 {
    USHORT OutputBufferSize;
    USHORT InputBufferSize;
    UCHAR MaximumInstances;
    UCHAR CurrentInstances;
    UCHAR PipeNameLength;
    UCHAR PipeName[1];
} NAMED_PIPE_INFORMATION_1;

typedef NAMED_PIPE_INFORMATION_1 UNALIGNED *PNAMED_PIPE_INFORMATION_1;

#pragma pack()


//
// Define SMB pipe handle state bits used by Query/SetNamedPipeHandleState
//
// These number are the bit location of the fields in the handle state.
//

#define PIPE_COMPLETION_MODE_BITS   15
#define PIPE_PIPE_END_BITS          14
#define PIPE_PIPE_TYPE_BITS         10
#define PIPE_READ_MODE_BITS          8
#define PIPE_MAXIMUM_INSTANCES_BITS  0

#ifdef __cplusplus
}
#endif

#endif
