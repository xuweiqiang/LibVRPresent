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
/*****************************************************************************
*
*
*
*  @module	Cxport.h | 
*
*  @comm	Common Transport Functions used by the communications stacks
*
*
*/

#ifndef _CXPORT_H_
#define _CXPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

//**	cxport.h - Common transport environment include file.
//
//	This file defines the structures and external declarations needed
//	to use the common transport environment.
//

//
// Typedefs used in this file
//
#ifndef CTE_TYPEDEFS_DEFINED
#define CTE_TYPEDEFS_DEFINED  1

typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;

#endif // CTE_TYPEDEFS_DEFINED

#include "winbase.h"

#define STRUCT_OF(type, address, field) CONTAINING_RECORD(address, type, field)

extern int CXPortDumpInfo (DWORD Option);
extern int CTEInitialize(void);

#define CTESystemUpTime		GetTickCount

//* Request completion routine definition.
typedef void	(*CTEReqCmpltRtn)(void *, uint , uint);

//* Routine to notify CTE that you're unloading.
extern	void	CTEUnload(uchar *);

//* Definition of a load/unload notification procedure handler.
typedef void	(*CTENotifyRtn)(uchar *);

//* Routine to set load and unload notification handlers.
extern	ulong	CTESetLoadNotifyProc(CTENotifyRtn Handler);
extern	ulong	CTESetUnloadNotifyProc(CTENotifyRtn Handler);


#define DISPATCH_LEVEL	0

typedef ULARGE_INTEGER CTEULargeInt;

extern ulong CTEEnlargedUnsignedDivide (CTEULargeInt Dividend, ulong Divisor, ulong *Remainder);

#define DEFINE_LOCK_STRUCTURE(x)	CRITICAL_SECTION	x;

#define DEFINE_LOCK_HANDLE(x)		CTELockHandle				x = 0
#pragma warning(disable:4189)

#define LOCK_STRUCTURE_SIZE sizeof(CRITICAL_SECTION)

#define EXTERNAL_LOCK(x)	extern CRITICAL_SECTION x;
						
#define CTEFreeLock(x,y)	LeaveCriticalSection(x)
#ifdef UNDER_CE
#define PEGFreeLock(x)		LeaveCriticalSection(x)
#else
#define PEGFreeLock(x)	
#endif

#define CTEGetLock(x,y)		EnterCriticalSection(x)
#ifdef UNDER_CE
#define PEGGetLock(x)		EnterCriticalSection(x)
#else
#define PEGGetLock(x)	
#endif

#define CTEInitLock(x)		InitializeCriticalSection(x)


#define CTEDeleteLock(x)		DeleteCriticalSection(x)


#define CTELock				CRITICAL_SECTION


typedef DWORD CTELockHandle;


#define CTEGetLockAtDPC(l, h) CTEGetLock((l),(h))

#define CTEFreeLockFromDPC(l, h) CTEFreeLock((l), (h))


typedef void (*CTEEventRtn)(struct CTEEvent *, void *);

#define CTE_EVENT	0
#define CTE_TIMER	1
typedef enum {
	Event_Idle,
	Event_Wait,
	Event_Fired,
} CTEEventState;

struct CTEEvent {
	struct CTEEvent *ce_next;
	CTEEventRtn ce_event;		
	void		*ce_param;		
	DWORD		t_tickcount;	
	DWORD		type;			
	CTEEventState ce_state;		
	LPTSTR		ce_name;		
}; /* CTEEvent */
typedef struct CTEEvent CTEEvent;

extern void CTEInitEvent(CTEEvent *, CTEEventRtn);

extern int CTEScheduleEvent(CTEEvent *, void *);

extern int CTECancelEvent(CTEEvent *);


typedef struct CTEEvent CTETimer;

extern void CTEInitTimer(CTETimer *);

extern void * CTEStartTimer(CTETimer *, unsigned long, CTEEventRtn, void *);

extern int CTEStopTimer(CTETimer *);

extern CTEEventState CTEGetTimerState(CTETimer *);

struct CTEBlockStruc {
	uint		cbs_status;
	HANDLE		cbs_event;
}; /* CTEBlockStruc */

typedef struct CTEBlockStruc CTEBlockStruc;

#define CTEInitBlockStruc(Event)										\
			do {														\
			(Event)->cbs_status = 0;									\
			(Event)->cbs_event = CreateEvent(NULL, FALSE, FALSE, NULL); \
			} while (0)


//#define CTEBlock(Event) WaitForSingleObject((Event)->cbs_event, INFINITE), (Event)->cbs_status
__inline uint CTEBlock(CTEBlockStruc * Event) {
    WaitForSingleObject(Event->cbs_event, INFINITE);
    return Event->cbs_status;
}


#define CTESignal(Event,Status)								\
			do {											\
				(Event)->cbs_status = Status;				\
				SetEvent((Event)->cbs_event);				\
			} while (0)

#define CTEDeinitBlockStruc(Event)										\
			do {														\
			CloseHandle((Event)->cbs_event); \
			} while (0)



extern LPVOID CTEAllocMem(DWORD size);

extern void CTEFreeMem(LPVOID ptr);

#define CTERefillMem()

#define CTEMemCopy(d,s,l)	memcpy((d),(s),(l))

#define CTEMemSet(d, v, l)	memset((d), (v), (l))

#define CTEMemCmp(d, s, l)	memcmp((d), (s), (l))

#ifdef DEBUG

#define CTEStructAssert(s, t) if ((s)->t##_sig != t##_signature) {\
				ASSERT(0); \
				}

#define CTEAssert(c)	if (!(c)) {\
				ASSERT(0); \
				}
#define CTEPrint(String)   DEBUGMSG(1, (TEXT(String)))
#define CTEPrintNum(Num)   DEBUGMSG(1, (TEXT("%d"), Num))
#define CTEPrintCRLF()	   DEBUGMSG(1, (TEXT("\r\n")))

#else	// DEBUG
#define DEBUGSTRING(v,s)
#define CTECheckMem(s)
#define CTEStructAssert(s,t )
#define CTEAssert(c)
#define CTEPrint(s)

#endif	// DEBUG


/*NOINC*/
#define RESULT_ZERO		1
#define RESULT_POSITIVE 0
#define RESULT_NEGATIVE 2

typedef enum CTE_INTERLOCKED_RESULT {
	ResultZero = RESULT_ZERO,
	ResultPositive = RESULT_POSITIVE,
	ResultNegative = RESULT_NEGATIVE
} CTEInterlockedResult;

// Adds an increment to a ULong. The increment may be negative. Returns the
// original value of the addend.
//
__inline unsigned long
CTEInterlockedAddUlong(unsigned long *pAddend, int Increment, CTELock *pLock) {
	unsigned long i;

	CTEGetLock(pLock, 0);
	i = *pAddend; *pAddend += Increment;
	CTEFreeLock(pLock, 0);
	return i;
}

#define CTEInterlockedDecrementLong(AddendPtr) \
            InterlockedDecrement(AddendPtr)

#define CTEInterlockedIncrementLong(AddendPtr) \
            InterlockedIncrement(AddendPtr)

BOOL GetRegDWORDValue (HKEY hKey, LPTSTR lpValueName, LPDWORD lpData);

BOOL SetRegDWORDValue (HKEY hKey, LPTSTR lpValueName, DWORD Data);

BOOL GetRegMultiSZValue (HKEY hKey, LPTSTR lpValueName, LPTSTR pData,
						 DWORD dwSize);

BOOL GetRegSZValue (HKEY hKey, LPTSTR lpValueName, LPTSTR pData, DWORD dwSize);

BOOL GetRegBinaryValue (HKEY hKey, LPTSTR lpValueName, LPBYTE pData, LPDWORD pdwSize);


BOOL
SetRegMultiSZValue (HKEY hKey, LPTSTR lpValueName, PTSTR pData);

BOOL
SetRegSZValue (HKEY hKey, LPTSTR lpValueName, LPTSTR szData);

BOOL
SetRegBinaryValue (HKEY hKey, LPTSTR lpValueName, LPBYTE pData, DWORD dwSize);

#define COMM_REG_KEY	TEXT("Comm\\")

DWORD
CXUtilGetProcAddresses(
	IN		PTCHAR	   tszDllName,
	IN	OUT	HMODULE	  *phDll,
	...);

DWORD
CXUtilTryGetProcAddresses(
	IN		PTCHAR	   tszDllName,
	IN	OUT	HMODULE	  *phDll,
	...);

#define CXREG_ALLOC_MEMORY (1<<0)

DWORD
CxRegReadValues(
	IN	HKEY	 hKey,
	IN	TCHAR	*tszSubKeyName,	OPTIONAL
	...);

DWORD
CxRegWriteValues(
	IN	HKEY	 hKey,
	IN	TCHAR	*tszSubKeyName,	OPTIONAL
	...);

#ifdef UNDER_CE

typedef struct CTEFTimer {
	struct CTEFTimer *pNext;
	CTEEventRtn		pFn;
	FILETIME		Time;
	void			*pParam;
} CTEFTimer;

void *CTEStartFTimer(CTEFTimer *pTimer, FILETIME FT, CTEEventRtn pFn,
			  void *pParam);

int CTEStopFTimer(CTEFTimer *pTimer);

#endif

DWORD
CTEIOControl(
    DWORD dwCode,
    LPBYTE lpBufIn,
    DWORD dwLenIn,
    LPBYTE lpBufOut,
    DWORD dwLenOut,
    LPDWORD lpdwActualOut
    );

//
// For CTE_IOCTL_SET_IDLE_TIMER_RESET and CTE_IOCTL_CLEAR_IDLE_TIMER_RESET:
//      1. These 2 functions increment or decrement a counter as there may be multiple modules interested
//      in keeping the system from going to sleep. When the internal counter is not 0, then CTE
//      will periodically call SystemIdleTimerReset. CTE will ensure that the counter does not go negative.
//      
//      2. None of the buffer parameters is used, only dwCode is used.
//
#define CTE_IOCTL_SET_IDLE_TIMER_RESET      1   // Cause CTE to periodically call SystemIdleTimerReset
#define CTE_IOCTL_CLEAR_IDLE_TIMER_RESET    2   // Cause CTE to quit calling SystemIdleTimerReset

//
// For CTE_IOCTL_GET_STATISTICS_1
//      1. lpBufIn and dwLenIn are not used.
//      2. lpBufOut must point to a CTE_STATISTICS_1 structure
//
#define CTE_IOCTL_GET_STATISTICS_1          3   // Get current statistics_1 structure

typedef struct _CTE_STATISTICS_1 {
    DWORD cs1_cCxportClients;       // Count of users of CXPORT
    DWORD cs1_cWorkerThreads;       // Count of Worker Threads.
    DWORD cs1_cIdleWorkers;         // Count of Idle Threads.
    DWORD cs1_WorkerID;             // Running Worker ID
    DWORD cs1_WorkerThreadPriority; // Priority of worker threads.
    DWORD cs1_WorkerThreadQuantum;  // Quantum for worker threads.
    DWORD cs1_cMaxWorkerThreads;
    DWORD cs1_cMinWorkerThreads;
    DWORD cs1_cDieThreads;          // Number of threads that need to die off.
    DWORD cs1_cMaxEventList;        // Maximum number of events waiting to fire.
    DWORD cs1_cMaxWorkerList;       // Maximum number of events waiting to run..
    DWORD cs1_cEventList;           // Current number of events waiting to fire.
    DWORD cs1_cWorkerList;          // Current number of events waiting to run.
    DWORD cs1_IdleTimerResetCount;  // Internal counter to control calling SystemIdleTimerReset
} CTE_STATISTICS_1, *PCTE_STATISTICS_1;


#define CTE_IOCTL_DUMP_INFO                 4   // Cause CXPortDumpInfo to be called

#ifdef DEBUG
//
// For CTE_IOCTL_GET_MAX_FIRE_TIME
//      1. This function returns the number of milliseconds that the slowest timer function took to run.
//      2. lpBufIn and dwLenIn are not used.
//      3. lpBufOut must point to a DWORD
//
#define CTE_IOCTL_GET_MAX_FIRE_TIME         5

//
// For CTE_IOCTL_SET_MAX_FIRE_TIME
//      1. This function sets the maximum timer function run time. This allows you to catch the next slow
//          timer without having to wait for the "slowest" timer. It also allows you to adjust for varying
//          conditions such as additional debug zones being turned on.
//      2. lpBufOut and dwLenOut are not used.
//      3. lpBufIn must point to a DWORD which contains the new value to use for the 
//
#define CTE_IOCTL_SET_MAX_FIRE_TIME         6

#endif  // DEBUG

//
// For CTE_IOCTL_UNLOAD_CXPORT:
//      Cause CXPORT to clean up after all events have run. CTEIOControl will block until everything
//      has been cleaned up. The macro CTEFinish() can be used to issue this IOCTL
//      
//      None of the buffer parameters are used, only dwCode is used.
//
#define CTE_IOCTL_UNLOAD_CXPORT             7

// convenient macro to cleanup
#define CTEFinish() CTEIOControl(CTE_IOCTL_UNLOAD_CXPORT,NULL,0,NULL,0,NULL)

// Stuff added for new TcpIp port
#define CTEInitString(D, S) \
        ( (CTEAllocateString((D), strlen((S)) + 1) == TRUE) ?   \
          ((D)->Length = (D)->MaximumLength, _tcscpy((D)->Buffer, (S)), TRUE) : \
          FALSE)

//
// Allocates a data buffer for Length characters in an uninitialized
// NDIS_STRING. The allocated space must be freed by a call to CTEFreeString.
// Returns TRUE if it succeeds. FALSE otherwise.
//
#define CTEAllocateString(S, L) \
        ( (((S)->Buffer = CTEAllocMem(L)) != NULL) ?           \
        ((S)->Length = 0, (S)->MaximumLength = (L), TRUE) :      \
          FALSE)

//
// Routine to free the string buffer associated with an NDIS_STRING. The
// buffer must have been allocated by a previous call to CTEInitString.
// Returns nothing.
//
#define CTEFreeString(String) CTEFreeMem((String)->Buffer)


//
// Calculates and returns the length of an NDIS_STRING in bytes.
//
#define CTELengthString(String) ((String)->Length)


//
// Compares two NDIS_STRING variables for case-sensitive equality
// Returns TRUE if the strings are equivalent. FALSE otherwise.
//
#define CTEEqualString(S1, S2) \
            ((_tcscmp((S1)->Buffer, (S2)->Buffer) == 0) ? TRUE : FALSE)


//
//     Copies one NDIS_STRING to another. Behavior is undefined if the
//     destination is not long enough to hold the source. Returns nothing.
//
#define CTECopyString(D, S)  (void) _tcscpy((D)->Buffer, (S)->Buffer); \
            (D)->Length = (S)->Length


// Define IOCTL Ranges
// statman gets IOCTLS from 0 - 1000
// We get the ioctols from 1000-2000

#define CXPORT_IOCTL_STATMAN_BASE  0
#define CXPORT_IOCTL_NETLOG_BASE  1000


//
// Trace message logging support
//
#define CXLOG_MSGTYPE_FATAL	0
#define CXLOG_MSGTYPE_ERROR 1
#define CXLOG_MSGTYPE_WARN  2
#define CXLOG_MSGTYPE_INFO  3

struct CXLOG_MODULE_INFO
{
	PSTR   szModuleName;
	PSTR   *szZoneNames;
	DWORD  numZones;
	PDWORD pdwEnableMask;
	DWORD  dwSavedEnableMask;
};

#define CXLOG_MAX_MODULES 32
#define CXLOG_INVALID_MODULE_ID 0xFFFFFFFF

extern void  CxLogInitialize();
extern void  CxLogShutdown();

extern DWORD CxLogRegister(
				IN PSTR   szModuleName,
				IN PSTR   szZoneNames[],
				IN DWORD  numZones,
				IN PDWORD pdwEnableMask);

extern void CxLogDeregister(
				IN  DWORD dwModuleId);

extern void CxLogMsg(
	IN DWORD dwModuleIdAndMessageTypeAndZone,
	IN PSTR  szMsg,
	...);

BOOL
CxLogGetModuleInfo(
	IN  DWORD                     dwModuleId,
	OUT struct CXLOG_MODULE_INFO *pInfo);

BOOL
CxLogSetModuleFilter(
	IN DWORD dwModuleId,
	IN DWORD Type,
	IN DWORD ZoneMask);

#define CXFILTER(zone_mask) (g_CxLogEnableMask & (zone_mask))
#define CXMSGINFO(zone_mask) ((g_CxLogModuleId << 24 ) | (CXLOG_MSGTYPE_INFO << 16) | (zone_mask))

#define CXLOGINFO_0(zone_mask, STR) \
	((void)( CXFILTER(zone_mask) ? CxLogMsg(CXMSGINFO(zone_mask), STR),1:0 ))
#define CXLOGINFO_1(zone_mask, STR, ARG1) \
	((void)( CXFILTER(zone_mask) ? CxLogMsg(CXMSGINFO(zone_mask), STR, ARG1),1:0 ))
#define CXLOGINFO_2(zone_mask, STR, ARG1, ARG2) \
	((void)( CXFILTER(zone_mask) ? CxLogMsg(CXMSGINFO(zone_mask), STR, ARG1, ARG2),1:0 ))


struct CXLOG_SET_FILTER_REQUEST
{
	DWORD dwModuleId;
	DWORD Type;
	DWORD ZoneMask;
};

#define CXLOG_MAX_ZONE_NAME_LEN	16

struct CXLOG_GET_MODULE_INFO_RESPONSE
{
	char  szModuleName[16];
	DWORD dwModuleId;
	DWORD Type;
	DWORD ZoneMask;
	DWORD numZones;
	char  szZoneName[16][CXLOG_MAX_ZONE_NAME_LEN];
};

//
// FIPS Random number generator support
//
DWORD CTEGenRandom(DWORD cBytes, PUCHAR Buffer);

#ifdef __cplusplus
}
#endif


#endif // _CXPORT_H_
