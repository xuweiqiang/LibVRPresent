//=============================================================================
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO INTERVIDEO, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2004 - 2005  InterVideo Corporation.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef _WINSOCK2_H_
#define _WINSOCK2_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <process.h>
#include <errno.h>
#include "windows.h"

#define MAKEWORD(a, b)              ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define INVALID_SOCKET              (SOCKET)(~0)
#define SOCKET_ERROR                (-1)
//#define _int64                      long long
#define WSAStartup(x,y)             (0)
#define WSACleanup()                (0)
#define WSACreateEvent()            (1)
#define WSAEventSelect(x,y,z)       (0)
#define WSAEnumNetworkEvents(x,y,z) (0)
#define WSAGetLastError()           (errno)
inline int _WSAGetLastError()
{
	int nError = SOCKET_ERROR;
	switch(nError = errno)
	{
	case EWOULDBLOCK:
		return WSAEWOULDBLOCK;
	case EINPROGRESS:
		return WSAEINPROGRESS;
	case EALREADY:
		return WSAEALREADY;
	case ENOTSOCK:
		return WSAENOTSOCK;
	case EDESTADDRREQ:
		return WSAEDESTADDRREQ;
	case EMSGSIZE:
		return WSAEMSGSIZE;
	case EPROTOTYPE:
		return WSAEPROTOTYPE;
	case ENOPROTOOPT:
		return WSAENOPROTOOPT;
	case EPROTONOSUPPORT:
		return WSAEPROTONOSUPPORT;
	case ESOCKTNOSUPPORT:
		return WSAESOCKTNOSUPPORT;
	case EOPNOTSUPP:
		return WSAEOPNOTSUPP;
	case EPFNOSUPPORT:
		return WSAEPFNOSUPPORT;
	case EAFNOSUPPORT:
		return WSAEAFNOSUPPORT;
	case EADDRINUSE:
		return WSAEADDRINUSE;
	case EADDRNOTAVAIL:
		return WSAEADDRNOTAVAIL;
	case ENETDOWN:
		return WSAENETDOWN;
	case ENETUNREACH:
		return WSAENETUNREACH;
	case ENETRESET:
		return WSAENETRESET;
	case ECONNABORTED:
		return WSAECONNABORTED;
	case ECONNRESET:
		return WSAECONNRESET;
	case ENOBUFS:
		return WSAENOBUFS;
	case EISCONN:
		return WSAEISCONN;
	case ENOTCONN:
		return WSAENOTCONN;
	case ESHUTDOWN:
		return WSAESHUTDOWN;
	case ETOOMANYREFS:
		return WSAETOOMANYREFS;
	case ETIMEDOUT:
		return WSAETIMEDOUT;
	case ECONNREFUSED:
		return WSAECONNREFUSED;
	case ELOOP:
		return WSAELOOP;
	case ENAMETOOLONG:
		return WSAENAMETOOLONG;
	case EHOSTDOWN:
		return WSAEHOSTDOWN;
	case EHOSTUNREACH:
		return WSAEHOSTUNREACH;
	case ENOTEMPTY:
		return WSAENOTEMPTY;
// 	case EPROCLIM:
// 		return WSAEPROCLIM;
	case EUSERS:
		return WSAEUSERS;
	case EDQUOT:
		return WSAEDQUOT;
	case ESTALE:
		return WSAESTALE;
	case EREMOTE:
		return WSAEREMOTE;
	}
	return nError;
}

#define WSAEWOULDBLOCK              EWOULDBLOCK
#define closesocket(s)				close(s)

#ifdef INADDR_ANY
#undef INADDR_ANY
#define INADDR_ANY                  (unsigned long)(0)
#endif

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct hostent  HOSTENT, *PHOSTENT, *LPHOSTENT;
typedef unsigned int    GROUP;
//typedef long            INT_PTR, *PINT_PTR;
//typedef int*            LPINT;
typedef int             WSADATA, WSAEVENT, WSANETWORKEVENTS;

#endif //_WINSOCK2_H_
