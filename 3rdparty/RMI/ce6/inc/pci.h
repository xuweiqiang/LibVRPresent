/*++
Copyright (c) 2001  BSQUARE Corporation.  All rights reserved.

Module:

    pci.h

Abstract:

    PCI bus handling support.

Author:

    Doug Hughes, June-2001

Revision(s):

--*/

#ifndef __PCI_H__
#define __PCI_H__

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif


#define PCI_MEM_BASE	           (PCI_MEM_PHYSADDR+KSEG1_OFFSET)
#define PCI_CONFIG_BASE	           (PCI_CONFIG_PHYSADDR+KSEG1_OFFSET)

#define PCI_MEM_CONFIG_BASE        (PCI_CONFIG_PHYSADDR+PCI_MEM_CONFIG_OFFSET+KSEG1_OFFSET)
#define PCI_CARDSLOT_CONFIG_BASE   (PCI_CONFIG_PHYSADDR+PCI_CARDSLOT_CONFIG_OFFSET+KSEG1_OFFSET)

#define PCI_IO_BASE                (PCI_IO_PHYSADDR+KSEG1_OFFSET)

#define IO_ACCESS_ENABLE           (1 << 0)
#define MEMORY_ACCESS_ENABLE       (1 << 1)
#define MASTER_ENABLE              (1 << 2)
#define SPECIAL_CYCLE_RECOGNITION  (1 << 3)
#define MEM_WRITE_ENABLE           (1 << 4)
#define VGA_PALATE_SNOOP_ENABLE    (1 << 5)
#define PARITY_ERROR_RESPONSE      (1 << 6)
#define WAIT_CYCLE_ENABLE          (1 << 7)
#define SYSTEM_ERROR_ENABLE        (1 << 8)
#define FAST_BTOB_ENABLE           (1 << 9)

//
// Host controller.
//

#define HOST_VENDOR_ID          0x1033
#define HOST_DEVICE_ID          0
#define HOST_DEVICE_NUMBER      0
#define HOST_FUNCTION_NUMBER    0


//
// Maximum PCI parameters.
//

#define MAX_SUPPORTED_PCI_BUS   1


typedef struct _TYPE0_PCI_ADDRESS {
    union {
        struct {
            ULONG Reserved:2;               // 0x00
            ULONG Register:6;               // 0x04
            ULONG Function:3;               // 0x08
            ULONG Device:21;                // 0x0C
        } bits;

        ULONG AsULONG;
    } u;
} TYPE0_PCI_ADDRESS, *PTYPE0_PCI_ADDRESS;



//
// PCI type 1 configuration mechanism.
//

typedef struct _TYPE1_PCI_ADDRESS {
    union {
        struct {
            ULONG Reserved:2;               // 0x00
            ULONG Register:6;               // 0x04
            ULONG Function:3;               // 0x08
            ULONG Device:5;                 // 0x0C
            ULONG Bus:8;                    // 0x10
            ULONG Reserved2:7;              // 0x14
            ULONG ConfigurationAccess:1;    // 0x18
        } bits;

        ULONG AsULONG;
    } u;
} TYPE1_PCI_ADDRESS, *PTYPE1_PCI_ADDRESS;




//
// Function definitions.  Note: XXXReadBusData and XXXWriteBusData are not
// serialized in terms of access to the PCI bus.  These routines are useful
// for accessing the PCI configuration space during kernel init (single
// threaded operation at that point).  XXXGetBusDataByOffset and
// XXXSetBusDataByOffset are serialized.
//
VOID
PCISetExtension(
    IN ULONG Address
    );

ULONG
PCIReadBusData(
    IN ULONG BusNumber,
    IN PCI_SLOT_NUMBER SlotNumber,
    IN PVOID Buffer,
    IN ULONG Offset,
    IN ULONG Length
    );

ULONG
PCIWriteBusData(
    IN ULONG BusNumber,
    IN PCI_SLOT_NUMBER SlotNumber,
    IN PVOID Buffer,
    IN ULONG Offset,
    IN ULONG Length
    );

ULONG
PCIGetBusDataByOffset(
    IN ULONG BusNumber,
    IN ULONG SlotNumber,
    IN PVOID Buffer,
    IN ULONG Offset,
    IN ULONG Length
    );

ULONG
PCISetBusDataByOffset(
    IN ULONG BusNumber,
    IN ULONG SlotNumber,
    IN PVOID Buffer,
    IN ULONG Offset,
    IN ULONG Length
    );

////////////
UCHAR
ReadPciConfigUchar(
    IN UCHAR CycleType,
    IN UCHAR BusNumber,
    IN UCHAR DeviceNumber,
    IN UCHAR FunctionNumber,
    IN UCHAR RegisterNumber
    );

USHORT
ReadPciConfigUshort(
    IN UCHAR CycleType,
    IN UCHAR BusNumber,
    IN UCHAR DeviceNumber,
    IN UCHAR FunctionNumber,
    IN UCHAR RegisterNumber
    );

ULONG
ReadPciConfigUlong(
    IN UCHAR CycleType,
    IN UCHAR BusNumber,
    IN UCHAR DeviceNumber,
    IN UCHAR FunctionNumber,
    IN UCHAR RegisterNumber
    );

VOID
WritePciConfigUchar(
    IN UCHAR CycleType,
    IN UCHAR BusNumber,
    IN UCHAR DeviceNumber,
    IN UCHAR FunctionNumber,
    IN UCHAR RegisterNumber,
    IN UCHAR Data
    );

VOID
WritePciConfigUshort(
    IN UCHAR CycleType,
    IN UCHAR BusNumber,
    IN UCHAR DeviceNumber,
    IN UCHAR FunctionNumber,
    IN UCHAR RegisterNumber,
    IN USHORT Data
    );

VOID
WritePciConfigUlong(
    IN UCHAR CycleType,
    IN UCHAR BusNumber,
    IN UCHAR DeviceNumber,
    IN UCHAR FunctionNumber,
    IN UCHAR RegisterNumber,
    IN ULONG Data
    );

VOID
PCIWriteIOByte(
    IN PUCHAR Address,
    IN WORD Value
    );

WORD
PCIReadIOByte(
    IN PUCHAR Address
    );

VOID
PCIWriteIOWord(
    IN PUCHAR Address,
    IN WORD Value
    );

USHORT
PCIReadIOWord(
    IN PUCHAR Address
    );

VOID
PCIWriteIOLong(
    IN PUCHAR Address,
    IN ULONG Value
    );

ULONG
PCIReadIOLong(
    IN PUCHAR Address
    );


extern
VOID
PciConfigure(
    VOID
    );


#ifdef __cplusplus
}
#endif

#endif __PCI_H__
