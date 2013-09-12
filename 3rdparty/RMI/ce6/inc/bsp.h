/*****************************************************************************
Copyright 2003-2009 RMI Corporation. All rights reserved.

Any transfer or redistribution of the source code, with or without modification,
IS PROHIBITED, unless prior written consent was obtained. Any transfer or
redistribution of the binary code for use on the RMI Alchemy Family,
with or without modification, is permitted, provided that the following
condition is met:

Redistributions in binary form must reproduce the above copyright notice,

this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution:

THIS SOFTWARE IS PROVIDED BY RMI Corporation 'AS IS' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL RMI OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#ifndef __BSP_H__
#define __BSP_H__



#include <windows.h>
#include <ceddk.h>
#include <nkexport.h>
#include <nkintr.h>

#include "platform.h"
#include "args.h"
#include "oemioctl.h"
// #include "ioctl_cfg.h"


#define BSP_DEVICE_PREFIX       EBOOT_NAME


//
// values of scratch0 which denotes the state of the system
// Cold boot is anything else
//
#define HAL_COLD_BOOT           0
#define HAL_WARM_BOOT           0x5741524d      // "WARM" warm reboot with existing memory configurations
#define HAL_SUSPENDRESUME_BOOT  0x51EE50        // "51ee'p'" Sleep mode was induced

//
// The are several free SYSINTRs in the OAL section, see NKINTR.H
//
#define SYSINTR_IDLEWAKEUP SYSINTR_FIRMWARE

#if defined(AU1X00_IC)
 extern AU1X00_IC * const ic0;
 extern AU1X00_IC * const ic1;
#endif

extern AU1X00_SYS * const Sys;

//
// HAL related variables.
//

extern DWORD bootupType;


//
// Two pointers must be setup for Perf Counter
//
//extern BOOL (*pQueryPerformanceCounter)(LARGE_INTEGER *lpliPerformanceCount);
//extern BOOL (*pQueryPerformanceFrequency)(LARGE_INTEGER *lpliPerformanceFreq);
extern volatile ULONG ILT_InUse;
extern volatile ULONG ILT_TicksPerIrq;
extern volatile ULONG ILT_TicksLeftB4Irq;
extern volatile ULONG ILT_StartOfIsrTime;
extern volatile ULONG ILT_EndOfIsrTime;
extern volatile ULONG ILT_SPC;
extern volatile USHORT ILT_NumInterrupts;

// Gone for Talisker
extern volatile DWORD DiffMSec;
extern DWORD   TickerPeriodmS;
extern PDWORD  pTickerPeriodmS;

//
// Timer structs
//
typedef struct {
    DWORD   SysIntr;
    ULONG   StartTime;
    ULONG   Duration;
} TIMER, *PTIMER;

#define OS_TIMER_COUNT   3

//
// Kernel Exports
//
extern void
SC_Sleep(
    DWORD cMilliseconds
    );

//
// RTC and Interval Timer
//

VOID
InitClock(
    VOID
    );

BOOLEAN
OEMInitRealTime(
    IN LPSYSTEMTIME SuggestedTime
    );


//
// Parallel port
//

int
OEMParallelPortInit(
    VOID
    );

//
// Initialisation
//

extern VOID
HalTickerInit(VOID);

//
//
//  HAL functions for handling interrupts
//
extern VOID
OEMInterruptInit(VOID);

extern
ULONG
OEMInterruptConnect(
    IN INTERFACE_TYPE InterfaceType,
    IN ULONG BusNumber,
    IN ULONG BusInterrupt,
    IN ULONG InterruptMode
    );

extern
VOID
OEMInterruptDisconnect(
    ULONG Sysintr
    );

extern BOOL
OEMInterruptClearEdge(
      IN DWORD SysIntr
      );


extern VOID
OEMInterruptDone(IN DWORD SysIntr);


//
// HAL functions for handling timer interrupts.
//
extern
ULONG
OEMInterruptConnectTimer(
    VOID
    );

extern
VOID
OEMInterruptDisconnectTimer(
    IN ULONG SysIntr
    );

extern
BOOLEAN
OEMInterruptStartTimer(
    IN ULONG SysIntr,
    IN ULONG PeriodHns
    );

extern
BOOLEAN
OEMInterruptStopTimer(
    IN ULONG SysIntr
    );

extern
ULONGLONG
OEMInterruptQueryTimer(
    IN ULONG SysIntr
    );

extern
ULONG
OEMInterruptQuerySysIntr(
    IN ULONG HwIntr
    );

ULONG
OEMInterruptQueryEvents (IN DWORD SysIntr);

extern ULONG
OEMGetCpuFrequency(VOID);

extern ULONG
OEMGetAuxFrequency(VOID);

extern ULONG
OEMGetSBUSFrequency(VOID);

extern ULONG
OEMGetPBUSFrequency(VOID);


extern ULONG
DISABLE_INTERRUPTS(VOID);

extern ULONG
RESTORE_INTERRUPTS(ULONG);


extern DWORD
PerfCountSinceTick(
    VOID
    );


/*
 * CP0 accessor prototypes in cp0.s
 */
#define CP0(REG) \
extern ULONG Cp0Rd ## REG (VOID); \
extern VOID Cp0Wr ## REG (ULONG);

CP0(Index)
CP0(Random)
CP0(EntryLo0)
CP0(EntryLo1)
CP0(Context)
CP0(PageMask)
CP0(Wired)
CP0(BadVAddr)
CP0(Count)
CP0(EntryHi)
CP0(Compare)
CP0(Status)
CP0(Cause)
CP0(EPC)
CP0(PRId)
CP0(Config)
CP0(Config0)
//CP0(Config1)
CP0(LLAddr)
CP0(WatchLo)
//CP0(IWatchLo)
CP0(WatchHi)
//CP0(IWatchHi)
CP0(Scratch)
CP0(Debug)
CP0(DEPC)
CP0(PerfCnt)
//CP0(PerfCtrl)
CP0(DTag)
//CP0(DData)
CP0(ITag)
//CP0(IData)
CP0(ErrorEPC)
CP0(DESave)
#undef CP0


void CPUEnterIdleState(void);
void OEMCPUPowerOff(void);
void EnableSystemToEnterSleepState(void);

extern VOID PostInit(VOID);

extern BOOL
SC_AtomicRMW_ULONG(
    PULONG   ptr,
    ULONG    AndMask,
    ULONG    OrMask,
    PULONG   pValue
);

extern BOOL
SC_AtomicRMW_USHORT(
    PSHORT   ptr,
    USHORT AndMask,
    USHORT OrMask,
    PUSHORT  pValue
);

extern BOOL
SC_AtomicRMW_UCHAR(
    PUCHAR   ptr,
    UCHAR AndMask,
    UCHAR OrMask,
    PUCHAR  pValue
);


extern BOOL
MacLayer_SetPacketFilter(
	PDWORD pdwRequestedFilter
);

extern BOOL
MacLayer_SetMulticastList(
	PUCHAR  pucMulticastAddressList,
	DWORD  dwNoOfAddresses
);

VOID SetDebugEnabledState(BOOL State);
BOOL GetDebugEnabledState(VOID);

extern BOOL OALIoCtlHalInitRegistry();


//
// KITL funcs
//

extern unsigned long
CreateDeviceName(
    USHORT wMAC[3],
    unsigned char *Name
);

extern BOOL
GetMacAddress(
    USHORT wMAC[3]
);

extern BOOL
KitlInitSerial(
    PKITLTRANSPORT pKitl
    );
extern LPBYTE
KitlSerialDecode(
    LPBYTE Buf,
    PUSHORT Size
);

extern void
InitKitlService(void);

extern BOOL
KitlInitEthernet(
    PKITLTRANSPORT pKitl
    );

extern BOOL
KitlInitSerial(
    PKITLTRANSPORT pKitl
    );



#endif __BSP_H__
