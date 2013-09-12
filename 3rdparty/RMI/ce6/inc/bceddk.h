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

#ifndef __BCEDDK_H__
#define __BCEDDK_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <bldver.h>

//
// Public Header Files, these are provided by MS and give us the basics.
//

#include <windows.h>
#include <nkintr.h>

//
// ntcompat.h in turn includes ceddk.h
// Note: Conflicts on attempt to include both, wdm.h and ceddk.h
//  Therefore use of wdm.h precludes use of ceddk.h and vice versa.
//

#include <ntcompat.h>

#include "oemioctl.h"

//

//
// Memory Management
//
// The following Mm routines are defined in wdm.h only: need to define here
//
//      MmGetMdlVirtualAddress
//      MmGetMdlByteCount
//      MmGetMdlByteOffset
//      MmGetMdlStartVa
//      MmGetMdlBaseVa
//
// The following Mm routines are defined in wdm.h and ceddk.h: don't define again
//
//      MmMapIoSpace
//      MmUnmapIoSpace
//      PAGE_ALIGN
//      ADDRESS_AND_SIZE_TO_SPAN_PAGES
//      BYTE_OFFSET
//      BYTES_TO_PAGES
//      ROUND_TO_PAGES
//
VOID
MmFreeMdl(
    IN PMDL MemoryDescriptorList
    );


#define PHYSICAL_ADDRESS_CONST(_Low, _High) \
    {(ULONG)(_Low), (LONG)(_High)}

#define PHYSICAL_ADDRESS_MAXIMUM \
    PHYSICAL_ADDRESS_CONST(-1,-1)

#ifdef INTR_AU1X00_IC
	//
	// Interrupt Support
	//
	typedef ULONG INTR_MODE;

	#define INTR_MODE_EDGE              0x00000001
	#define INTR_MODE_LEVEL             0x00000002
	#define INTR_MODE_POSITIVE_LOGIC    0x00000004
	#define INTR_MODE_NEGATIVE_LOGIC    0x00000008
	#define INTR_MODE_POWERDOWN_WAKEUP  0x00000010
	#define INTR_MODE_HIGH_PRIORITY     0x00000020
	#define INTR_MODE_NOIDLE_WAKEUP     0x00000040
	#define INTR_MODE_LOW_PRIORITY      0x00000000  // Implicit from High priority
	#define INTR_MODE_UNMASK			0x00000080 // unmask thus enable
#endif

#if defined(INTR_AU13XX_GPINT)
	//
	// Interrupt Support
	//
	#include <gpintr.h>
#endif

//
// The following routines are defined as part of the standard Windows CE API
//
//      InterruptInitialize
//      InterruptDone
//      InterruptDisable
//

ULONG
InterruptConnect(
    IN INTERFACE_TYPE InterfaceType,
    IN ULONG BusNumber,
    IN ULONG BusInterrupt,
    IN ULONG InterruptMode
    );

VOID
InterruptDisconnect(
    IN ULONG Sysintr
    );

HANDLE
InterruptConfigure(
    IN LPCTSTR szRegActivePath,
    IN DWORD hwIntr,
    IN OUT DWORD* pdwSysintr
    );

VOID
InterruptUnconfigure(
    HANDLE hConfig
    );

//
// Timer Interrupt Support
//

ULONG
InterruptConnectTimer(
    VOID
    );

VOID
InterruptDisconnectTimer(
    IN ULONG SysIntr
    );

BOOLEAN
InterruptStartTimer(
    IN ULONG SysIntr,
    IN ULONG PeriodHns
    );

BOOLEAN
InterruptStopTimer(
    IN ULONG SysIntr
    );

ULONGLONG
InterruptQueryTimer(
    IN ULONG SysIntr
    );

ULONG
InterruptQuerySysintr(
    ULONG HwIntr
);

ULONG
InterruptQueryEvents(ULONG SysIntr);


extern
ULONG GetCPUCoreSpeed(VOID);

extern
ULONG GetPBUSSpeed(VOID);


//
// Adapter Objects, these are platform dependent
//

struct _ADAPTER_OBJECT;

typedef struct _ADAPTER_OBJECT ADAPTER_OBJECT, *PADAPTER_OBJECT;

typedef PVOID PDMA_CHANNEL_OBJECT;

//
// Hardware abstraction
//

//
// The following Hal routines are defined in wdm.h and ceddk.h: don't define again
//
//      HalTranslateBusAddress
//      HalGetBusDataByOffset
//      HalGetBusData
//      HalSetBusDataByOffset
//      HalSetBusData
//

PVOID
HalCreateBlockMapping(
    IN PHYSICAL_ADDRESS physAddr,
	IN ULONG size,
	IN ULONG cca
	);

int
HalAddWiredTLB(
	IN ULONG EntryHi,
	IN ULONG EntryLo0,
	IN ULONG EntryLo1,
	IN ULONG PageMask);

extern
BOOL
AtomicRMW_UCHAR(
    ULONG PhysAddr,
    UCHAR AndMask,
    UCHAR OrMask,
    PUCHAR pValue
    );
extern
BOOL
AtomicRMW_USHORT(
    ULONG PhysAddr,
    USHORT AndMask,
    USHORT OrMask,
    PUSHORT pValue
    );
extern
BOOL
AtomicRMW_ULONG(
    ULONG PhysAddr,
    ULONG AndMask,
    ULONG OrMask,
    PULONG pValue
    );


// Logical devices used to Initialize a DMA/DDMA channel
typedef enum {
	DMA_UART0_TX,		DMA_UART0_RX,
	DMA_UART3_TX,		DMA_UART3_RX,
	DMA_AC97_TX,		DMA_AC97_RX,
	DMA_USBD_EP0_TX,	DMA_USBD_EP0_RX,
	DMA_USBD_EP1_TX,
	DMA_USBD_EP2_TX,
						DMA_USBD_EP3_RX,
						DMA_USBD_EP4_RX,
	DMA_I2S_TX,			DMA_I2S_RX,
	DMA_SD0_TX,			DMA_SD0_RX,
	DMA_SD1_TX,			DMA_SD1_RX,
	DMA_SPI_TX,			DMA_SPI_RX,
	DMA_IDE_TX,			DMA_IDE_RX,
#if defined(SOC_AU1200)
	DMA_CIM_FIFOA,		DMA_CIM_FIFOB,		DMA_CIM_FIFOC,
#elif defined(SOC_AU13XX)
	DMA_CIM_FRAMESYNC,
	DMA_NAND_TX,		DMA_NAND_RX,
#endif
	DMA_M2M_COPY,
	// The final entry, used for array sizing
	MAX_DMA_LOGICAL_DEVICES
} DMA_LOGICAL_DEVICE;

extern PDMA_CHANNEL_OBJECT HalAllocateDMAChannel();
extern VOID HalFreeDMAChannel(PDMA_CHANNEL_OBJECT ChannelObject);


extern BOOL HalInitDmaChannel(PDMA_CHANNEL_OBJECT DmaChannelObject,
                       DMA_LOGICAL_DEVICE  Device,
                       ULONG               BufferSize,
					   BOOL                InterruptEnable);

extern PVOID HalGetNextDMABuffer(PDMA_CHANNEL_OBJECT DmaChannelObject);

extern BOOL HalActivateDMABuffer(PDMA_CHANNEL_OBJECT DmaChannelObject,
                      PVOID pBuffer,
                      ULONG             Size);
extern BOOL HalStartDMA(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern BOOL HalStopDMA(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern ULONG HalCheckForDMAInterrupt(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern BOOL HalAckDMAInterrupt(PDMA_CHANNEL_OBJECT DmaChannelObject,ULONG Mask);
extern BOOL HalSetDMAForReceive(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern UCHAR HalGetDMAHwIntr(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern ULONG HalGetDMABufferRxSize(PDMA_CHANNEL_OBJECT DmaChannelObject,
                            PVOID pBuffer);
extern VOID HalWaitForDMA(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern BOOL HalDMAIsUnderflowed(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern BOOL HalReconfigureDMA(PDMA_CHANNEL_OBJECT DmaChannelObject,
                       DMA_LOGICAL_DEVICE  Device);

extern BOOL HalSetupMdlDMA(PDMA_CHANNEL_OBJECT DmaChannelObject,
                    PMDL                pMDLHead,
					BOOL                ReadTransfer);
extern BOOL HalStartMdlDMA(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern BOOL HalCompleteMdlDMA(PDMA_CHANNEL_OBJECT DmaChannelObject,PMDL pMDL);

//
// Entry points for platform specific modules.
//
VOID DmaEntry( VOID );
VOID BusEntry( VOID );
VOID OALStallExecutionEntry( VOID );

////////////////////////////////////////////////////////////////////////////////
// For CIM WEAVE mode
extern BOOL HalInitNoBufferDMAChannel(	PDMA_CHANNEL_OBJECT DmaChannelObject,
                       	  	   			DMA_LOGICAL_DEVICE  Device,
					   	  	   			BOOL                InterruptEnable);
extern BOOL HalSetupDMABuffer(	PDMA_CHANNEL_OBJECT DmaChannelObject,
					 			DMA_LOGICAL_DEVICE  Device,
					 			PVOID pMem,
					 			ULONG Phys,
				     			ULONG Size);
extern BOOL HalSetupVPPtrDMABuffer(	PDMA_CHANNEL_OBJECT DmaChannelObject,
					 				DMA_LOGICAL_DEVICE  Device,
					 				ULONG *pSrc,
					 				ULONG *pDest,
				     				ULONG Size);
extern BOOL HalFreeDMABuffer(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern ULONG HalGetDMAByteCount(PDMA_CHANNEL_OBJECT DmaChannelObject);
extern BOOL HalSetupVPPtrDMAStrideBuffer(	PDMA_CHANNEL_OBJECT DmaChannelObject,
					 				DMA_LOGICAL_DEVICE  Device,
					 				ULONG *pSrc,
									ULONG SrcStrideBlock,
					 				ULONG *pDest,
									ULONG DestStrideBlock,
				     				ULONG Size);

////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif __BCEDDK_H__
