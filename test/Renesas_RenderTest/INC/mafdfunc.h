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

mafdfunc.h

Abstract:  

Notes: 


--*/
  
#ifndef _MAFDFUNC_H_
#define _MAFDFUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(WINCEMACRO) && !defined(WINCEAFDMACRO)
#error WINCEMACRO not defined __FILE__
#endif

#define 	AFDSocket IMPLICIT_DECL (SOCKHAND, SH_COMM, 2,		\
  					(DWORD AddressFamily, 						\
  					 DWORD SocketType, 							\
  					 DWORD Protocol,							\
  					 DWORD CatId,								\
  					 GUID *pProvId))
  
#define		AFDControl IMPLICIT_DECL (DWORD, SH_COMM, 3,    	\
  					(DWORD Protocol,                        	\
  					 DWORD Action,								\
  					 LPVOID InputBuffer,						\
  					 LPDWORD InputBufferLength,					\
  					 LPVOID OutputBuffer,						\
  					 LPDWORD OutputBufferLength))
  
#define		AFDEnumProtocolsW IMPLICIT_DECL (DWORD, SH_COMM, 4, \
  					 (LPDWORD   lpiProtocols, 					\
  					  LPVOID	lpProtocolBuffer,				\
  					  LPDWORD	lpdwBufferLength))
  
#define		AFDGetHostentByAttr IMPLICIT_DECL (DWORD, SH_COMM, 7,\
  					(LPSOCK_THREAD 			pThread,			\
  					 LPWSTR					Name,				\
  					 LPBYTE					Address,			\
  					 LPVOID					pOptions))	
  
#define		AFDAddIPHostent IMPLICIT_DECL (DWORD, SH_COMM, 8,	\
  					(LPWSTR		 			Hostname,			\
  					 char					**IpAddr,				\
  					 LPWSTR					Aliases,			\
  					 LPFILETIME				lpFileTime))	
  
#define		RasIOControl IMPLICIT_DECL (DWORD, SH_COMM, 9,		\
  					(LPVOID 				hRasConn,			\
  					 DWORD 					dwCode,				\
  					 PBYTE 					pBufIn,				\
  					 DWORD 					dwLenIn, 			\
  					 PBYTE 				    pBufOut, 			\
  					 DWORD 					dwLenOut,			\
  					 PDWORD					pdwActualOut))
  
#define		AFDSelect IMPLICIT_DECL (DWORD, SH_COMM, 10,		\
  					(UINT					ReadCount,			\
  					 LPSOCK_LIST			ReadList,			\
  					 UINT					WriteCount,			\
  					 LPSOCK_LIST			WriteList,			\
  					 UINT					ExceptCount,		\
  					 LPSOCK_LIST			ExceptList,			\
  					 const struct timeval	*timeout,			\
  					 CRITICAL_SECTION		*pDllCS))

#define		AFDAddInterface IMPLICIT_DECL (DWORD, SH_COMM, 19,	\
  					(PTSTR					pAdapter,			\
					 VOID					*Nte,				\
					 DWORD					CONTEXT,			\
					 int					Flags,				\
					 uint					IpAddr,				\
					 uint					SubnetMask,			\
					 int					cDns,				\
					 uint					*pDns,				\
					 int					cWins,				\
					 uint					*pWins))

#define		RasDial IMPLICIT_DECL (DWORD, SH_COMM, 5,			\
					(LPRASDIALEXTENSIONS 	dialExtensions,		\
					 LPTSTR		    		phoneBookPath,		\
					 LPRASDIALPARAMS    	rasDialParam,		\
					 DWORD		    		NotifierType,		\
					 LPVOID		    		notifier,			\
					 LPHRASCONN	    		pRasConn))	

#define		RasHangup IMPLICIT_DECL (DWORD, SH_COMM, 6,			\
					(HRASCONN				Session))	

#define		RasEnumEntries IMPLICIT_DECL (DWORD, SH_COMM, 11,	\
					(LPWSTR					Reserved,			\
					 LPWSTR					lpszPhoneBookPath,	\
					 LPRASENTRYNAME			lprasentryname,		\
					 LPDWORD				lpcb,				\
					 LPDWORD				lpcEntries))
					 				
#define		RasGetEntryDialParams IMPLICIT_DECL (DWORD, SH_COMM, 12,\
					(LPWSTR					lpszPhoneBook,		\
					 LPRASDIALPARAMS		lpRasDialParams,	\
					 LPBOOL					lpfPassword))
					 				
#define		RasSetEntryDialParams IMPLICIT_DECL (DWORD, SH_COMM, 13,\
					(LPWSTR					lpszPhoneBook,		\
					 LPRASDIALPARAMS		lpRasDialParams,	\
					 BOOL					fRemovePassword))

#define		RasGetEntryProperties IMPLICIT_DECL (DWORD, SH_COMM, 14,\
					(LPWSTR					lpszPhoneBook,		\
					 LPWSTR					szEntry,			\
					 LPBYTE					lpbEntry,			\
					 LPDWORD				lpdwEntrySize,		\
					 LPBYTE					lpb,				\
					 LPDWORD				lpdwSize))
					 				
#define		RasSetEntryProperties IMPLICIT_DECL (DWORD, SH_COMM, 15,\
					(LPWSTR					lpszPhoneBook,		\
					 LPWSTR					szEntry,			\
					 LPBYTE					lpbEntry,			\
					 DWORD					dwEntrySize,		\
					 LPBYTE					lpb,				\
					 DWORD					dwSize))

#define		RasValidateEntryName IMPLICIT_DECL (DWORD, SH_COMM, 16, \
					(LPWSTR					lpszPhonebook,		\
					 LPWSTR					lpszEntry))

#define		RasDeleteEntry IMPLICIT_DECL (DWORD, SH_COMM, 17,	\
					(LPWSTR					lpszPhonebook,		\
					 LPWSTR					lpszEntry))

#define		RasRenameEntry IMPLICIT_DECL (DWORD, SH_COMM, 18,	\
					(LPWSTR					lpszPhonebook,		\
					 LPWSTR					lpszOldEntry,		\
					 LPWSTR					lpszNewEntry))
					 				
#define		RasEnumConnections IMPLICIT_DECL (DWORD, SH_COMM, 20,	\
					(LPRASCONN				lprasconn,				\
					 LPDWORD				lpcb,					\
					 LPDWORD				lpcConnections))

#define		RasGetConnectStatus IMPLICIT_DECL (DWORD, SH_COMM, 21,	\
					(HRASCONN				rasconn,				\
					 LPRASCONNSTATUS		lprasconnstatus))

#define     RasGetEntryDevConfig IMPLICIT_DECL (DWORD, SH_COMM, 22, \
                    (LPCTSTR                szPhonebook,            \
                     LPCTSTR                szEntry,                \
                     LPDWORD                pdwDeviceID,            \
                     LPDWORD                pdwSize,                \
                     LPVARSTRING            pDeviceConfig))

#define     RasSetEntryDevConfig IMPLICIT_DECL (DWORD, SH_COMM, 23, \
                    (LPCTSTR                szPhonebook,            \
                     LPCTSTR                szEntry,                \
                     DWORD                  dwDeviceID,             \
                     LPVARSTRING            lpDeviceConfig))

#define		NETbios IMPLICIT_DECL (DWORD, SH_COMM, 24,		\
					(DWORD				x1,					\
					 DWORD				dwOpCode,			\
					 PVOID				pNCB,				\
					 DWORD				cBuf1,				\
					 PBYTE				pBuf1,				\
					 DWORD				cBuf2,				\
					 PDWORD				pBuf2))

#define PMInstallProvider IMPLICIT_DECL (DWORD, SH_COMM, 26,		\
					(LPGUID				lpProviderId,				\
					const WCHAR FAR		*lpszProviderDllPath,		\
					const LPWSAPROTOCOL_INFOW	lpProtocolInfoList,	\
					DWORD				dwNumberOfEntries,			\
					DWORD				Flags))

#define PMEnumProtocols IMPLICIT_DECL (DWORD, SH_COMM, 27,	\
					(LPINT				lpiProtocols,		\
					LPWSAPROTOCOL_INFOW	lpProtocolBuffer,	\
					PDWORD				lpdwBufferLength,	\
					PDWORD				lpdwFlags,			\
					LPINT				lpErrno))

#define PMFindProvider IMPLICIT_DECL  (DWORD, SH_COMM, 28,	\
					(int			af,						\
					int				type, 					\
					int				protocol,				\
					DWORD			dwCatalogEntryId,		\
					IN DWORD		dwFlags,				\
					OUT LPWSAPROTOCOL_INFOW lpProtocolInfo, \
					OUT PWSTR		psPath))

#define PMInstallNameSpace IMPLICIT_DECL (DWORD, SH_COMM, 29,	\
					(LPWSTR				lpszIdentifier,		\
					LPWSTR				lpszPathName,		\
					DWORD				dwNameSpace,		\
					DWORD				dwVersion,			\
					LPGUID				lpProviderId,		\
					PDWORD				pFlags))

#define PMEnumNameSpaceProviders IMPLICIT_DECL  (DWORD, SH_COMM, 30,	\
					(PDWORD					pcBuf,			\
					LPWSANAMESPACE_INFOW	pBuf, 			\
					PDWORD					pFlags,			\
					int						*pErr))


#define PMFindNameSpaces IMPLICIT_DECL  (DWORD, SH_COMM, 31,	\
					(WSAQUERYSETW		*pQuery,				\
					void				*pBuf,					\
					int					*pcBuf,					\
					int					*pErr))


#define PMAddrConvert IMPLICIT_DECL  (DWORD, SH_COMM, 32,	\
					(DWORD				Op,						\
					DWORD				AddrFamily, 			\
					SOCKADDR			*pSockAddr,				\
					DWORD				*pcSockAddr,			\
					LPWSAPROTOCOL_INFOW	pProtInfo,				\
					PWSTR				psAddr,					\
					DWORD				*pcAddr,				\
					VOID				*Unused))

#if defined (_WINDNS_INCLUDED_)
#define AFDDnsQuery_W IMPLICIT_DECL (DWORD, SH_COMM, 33,    \
                      (PCWSTR pszName,                      \
                      WORD wType,                           \
                      DWORD Options,                        \
                      PIP4_ARRAY aipServers,                \
                      PDNS_RECORD *ppQueryResults,          \
                      PVOID *pReserved))


#define AfdDNSRecordListFree IMPLICIT_DECL (void, SH_COMM, 34, \
                             (PDNS_RECORD pRecordList,         \
                             DNS_FREE_TYPE FreeType))

#define AFD_UpdateOrModifyRecords IMPLICIT_DECL (DWORD, SH_COMM, 35, \
                                  (PDNS_RECORD pAddRecords,         \
                                  PDNS_RECORD pDeleteRecords,       \
                                  DWORD Options,                    \
                                  HANDLE hContext,                  \
                                  PIP4_ARRAY pServerList,           \
                                  PVOID pReserved,                  \
                                  BOOL fReplace))

#endif // _WINDNS_INCLUDED_


#ifdef __cplusplus
}
#endif

#endif // __MAFDFUNC.H
