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

socksv2.h

Abstract:  

This file implements the Windows CE PC Card Socket Services structures and
definitions.  This is provided as a sample to platform writers and is
expected to be able to be used without modification on most (if not
all) hardware platforms.

Functions:


Notes:


--*/

#ifndef __SOCKSV2_H__
#define __SOCKSV2_H__ 1
// Do not include cardserv.h
#ifndef __CARDSERV_H__
#define __CARDSERV_H__
#endif
#include <ceddk.h>
#ifdef __cplusplus
extern "C"
{
#endif
    typedef UINT32 STATUS;
    //
    // @doc DRIVERS
    //
    // PCCARD Socket feture structure
    //
    // Socket Service Handle is created when a card inserted into socket. With static socket number that assigned by Card Service,
    // it formed a unique access ID.
    //
#define IRQ_ROUTINGTABLE_SIZE 16 
    typedef struct _SS_SOCKET_INFO
    {
        DWORD   dwSocketCaps;               // Socket Capablity. Bitmaps of Follows.
#define SOCK_CAP_MEM_CARD       0x1
#define SOCK_CAP_IO_MEMORY_CARD 0x2
#define SOCK_CAP_SUPPORT_CARDBUS_CARD   0x4
#define SOCK_CAP_WAKEUP_BYCLIENT 0x8
#define SOCK_CAP_WAKEUP_BYEVENT 0x10
#define SOCK_CAP_FIXED_FUNC_IRQ 0x40
#define SOCK_CAP_KEEP_POWERED   0x80    // Keep PCMCIA bus powered while the system
#define SOCK_CAP_ONLY_SYSINTR   0x8000  // It indicate Socket only support SysIntr and bArrayIrqRouting gives SysIntr Value.
        DWORD   dwSocketIntrCaps;           // Socket Interrupt Capability Set follow bitmap.
        DWORD   dwSocketRptCaps;            // Socket status report capability
        DWORD   dwSocketIndCaps;            // Socket status indicate capability
#define SOCK_CAP_WP             0x1     // Write Protection 
#define SOCK_CAP_LOCKED         0x2     // Externally gernerated indicating the state of a mechanical or electrical card lock mechanism
#define SOCK_CAP_EJECT          0x4     // Externally gernrated indicating a request to eject a PC Card from the socket has been made
#define SOCK_CAP_INSERT         0x8     // Externally gernerated indicating a request to insert a PC Card from the socket has been made
#define SOCK_CAP_BVD1           0x10    // PC Card BVD1. When set, this indicates the battery is no longer servieable.
#define SOCK_CAP_BVD2           0x20    // PC Card BVD2 When Set this indicates the battery is weak.
#define SOCK_CAP_READY          0x40    // PC Card Ready sigal (or Function Interrupt).
#define SOCK_CAP_CD             0x80    // PC Card Detection, when set it is inserted completed otherwise it removed.
#define SOCK_CAP_PWRCYCLE       0x100   // PC Card IPower Cycle.
#define SOCK_CAP_CARDBUS_CARD   0x4000  // Card Bus Card inserted this is only for state..
#define SOCK_CAP_SUSPEND_RESUME 0x8000  // PC Suspend/Resume capable.
        DWORD   dwNumOfPowerEntry;          // Number of power entry this socket have.
        DWORD   dwNumOfWindows;             // Number of Window supported in this Socket.
        DWORD   dwBusType;                  // Bus Type of this device connected.
        DWORD   dwBusNumber;                // Bus number for this socket if this is Card Bus Card.
        BYTE    bArrayIrqRouting[IRQ_ROUTINGTABLE_SIZE]; // PCCARD Logical to Platform Physical IRQ or SysIntr Mapping.
    } SS_SOCKET_INFO, * PSS_SOCKET_INFO;


    //    STATUS (*SSInquireSocketInfo)(HANDLE hSocket, PSS_SOCKET_INFO pSocketInfo);
    //

    //**********************************************************************
    // Power entries
    //**********************************************************************

    //
    // PDCARD_POWER_ENTRY.fSupply
    //
#define PWR_SUPPLY_VPP2  0x20    // power entry describes Vpp2
#define PWR_SUPPLY_VPP1  0x40    // power entry describes Vpp1
#define PWR_SUPPLY_VCC   0x80    // power entry describes Vcc

    //
    // @struct PDCARD_POWER_ENTRY | Structure used to indicate the voltage levels that
    //                              a socket controller can produce.
    //
    typedef struct _SS_POWER_ENTRY
    {
        UINT8   uPowerLevel;          // @field Voltage in 0.1 volt increments
        UINT8   fSupply;              // @field Bit field indicating the voltage source (Vcc, Vpp1 or Vpp2)
    } SS_POWER_ENTRY, * PSS_POWER_ENTRY;
    //
    //    STATUS (*SSGetSocketPowerEnry)(HANDLE hSocket, PDWORD pdwNumofEntry, PSS_POWER_ENTRY pPowerEntry);


    //**********************************************************************
    // memory and I/O window information
    //**********************************************************************

    //
    // PDCARD_WINDOW_INFO.fWindowCaps
    //
#define WIN_CAP_COMMON    0x01 // common memory can be mapped into host space
#define WIN_CAP_ATTRIBUTE 0x02 // attribute memory can be mapped
#define WIN_CAP_IO        0x04 // I/O ports can be mapped into host I/O space
#define WIN_CAP_WAIT      0x80 // WAIT signal can extend access cycle

    //
    // PDCARD_WINDOW_INFO.fMemoryCaps
    //
#define MEM_CAP_PRG_BASE   0x0001    // programmable base address
#define MEM_CAP_PRG_SIZE   0x0002    // programmable base window size
#define MEM_CAP_ENABLE_WIN 0x0004    // Enable/disable window
#define MEM_CAP_8BIT       0x0008    // 8 bit data bus supported
#define MEM_CAP_16BIT      0x0010    // 16 bit data bus supported
#define MEM_CAP_BASE_ALIGN 0x0020    // base must align with multiple of size
#define MEM_CAP_OFF_ALIGN  0x0080    // card offset must align on size boundary
#define MEM_CAP_PAGING_HW  0x0100    // Paging hardware available
#define MEM_CAP_PAGING_SH  0x0200    // Paging hardware shared
#define MEM_CAP_PAGING_EN  0x0400    // Enable/disable paging
#define MEM_CAP_WRITE_PROT 0x0800    // window write protect supported

    //
    // PDCARD_WINDOW_INFO.fIOCaps
    //
#define IO_CAP_PRG_BASE   MEM_CAP_PRG_BASE
#define IO_CAP_PRG_SIZE   MEM_CAP_PRG_SIZE
#define IO_CAP_ENABLE_WIN MEM_CAP_ENABLE_WIN
#define IO_CAP_8BIT       MEM_CAP_8BIT
#define IO_CAP_16BIT      MEM_CAP_16BIT
#define IO_CAP_BASE_ALIGN MEM_CAP_BASE_ALIGN
#define IO_CAP_INPACK     0x0080    // INPACK# supported

    //
    // @struct PDCARD_WINDOW_INFO | Structure used by <f PDCardInquireWindow> to report 
    //          a memory or I/O window's characteristics and capabilities.
    //
    typedef struct _SS_WINDOW_INFO
    {
        HANDLE  hSocket;        // @ handle of socket
        UINT16  fWindowCaps;     // @field Bit encoded window capabilities
        UINT16  fMemoryCaps;     // @field Bit encoded memory window capabilities
        UINT16  fIOCaps;         // @field Bit encoded I/O window capabilities  
        UINT16  Reserved;   
        UINT32  uMemFirstByte;   // @field Physical address of first addressable byte
        UINT32  uMemLastByte;    // @field Physical address of last addressable byte
        UINT32  uMemMinSize;     // @field Minimum size of window
        UINT32  uMemMaxSize;     // @field Maximum size of window
        UINT32  uMemGranularity; // @field Required window granularity
        UINT32  uMemBase;        // @field Required window base address alignment 
        UINT32  uMemOffset;      // @field Required card offset alignment
        UINT32  uIOFirstByte;    // @field Physical address of first addressable byte
        UINT32  uIOLastByte;     // @field Physical address of last addressable byte
        UINT32  uIOMinSize;      // @field Minimum size of window
        UINT32  uIOMaxSize;      // @field Maximum size of window
        UINT32  uIOGranularity;  // @field Required window granularity
        UINT8   uAddressLines;   // @field Number of I/O lines decoded
        UINT8   fSlowest;        // @field Bit encoded slowest access speed
        UINT8   fFastest;        // @field Bit encoded fastest access speed
        UINT8   Reserved2;
    } SS_WINDOW_INFO, * PSS_WINDOW_INFO;

    //    STATUS (*SSCardInquireWindow)(UINT32 uWindow, PPDCARD_WINDOW_INFO pWindowInfo);

    //
    // PDCARD_WINDOW_STATE.fState
    //
#define WIN_STATE_MAPS_IO    0x01    // 1 = I/O, 0 = memory
#define WIN_STATE_ENABLED    0x02
#define WIN_STATE_16BIT      0x04
#define WIN_STATE_SHARED_IO  0x08
#define WIN_STATE_ATTRIBUTE  0x80    // memory window accesses attribute space
#define WIN_STATE_READONLY   0x40    // This windows is write protected.
#define WIN_STATE_SH4_AREA6  0x8000  // SH4 only flags, does not used anywhere else.
    //
    // @struct PDCARD_WINDOW_STATE | Structure used by <f PDCardGetWindow> and
    //      <f PDCardSetWindow> to report or change a memory or I/O window's state.
    //
    typedef struct _SS_WINDOW_STATE
    {
        HANDLE  hSocket;     // @field Socket handle which this window is mappped
        UINT16  fState;      // @field Bit encoded window state
        UINT8   fSpeed;      // @field Bit encoded access speed
        UINT8   Reserved;
        UINT32  uSize;       // @field Size of window
        UINT32  uBase;       // @field Physical address of beginning of window
        UINT32  uOffset;     // @field Offset in PC card memory
    } SS_WINDOW_STATE, * PSS_WINDOW_STATE;

    //    STATUS (*PDCardGetWindow)(UINT32 uWindow, PPDCARD_WINDOW_STATE pWindowState);
    //    STATUS (*PDCardSetWindow)(UINT32 uWindow, PPDCARD_WINDOW_STATE pWindowState);
    // (PSS_WINDOW_INFO.fSlowest)
    // (PSS_WINDOW_INFO.fFastest)
    // (PSS_WINDOW_STATE.fSpeed)
    //
#define WIN_SPEED_EXP_MASK  0x07    // Exponent mask
#define WIN_SPEED_EXP_1NS   0x00    // 1 nanosecond
#define WIN_SPEED_EXP_10NS  0x01
#define WIN_SPEED_EXP_100NS 0x02
#define WIN_SPEED_EXP_1US   0x03    // 1 microsecond
#define WIN_SPEED_EXP_10US  0x04
#define WIN_SPEED_EXP_100US 0x05
#define WIN_SPEED_EXP_1MS   0x06    // 1 millisecond
#define WIN_SPEED_EXP_10MS  0x07

#define WIN_SPEED_MANT_MASK 0x78    // Mantissa mask
#define WIN_SPEED_MANT_10   0x08    // 1.0
#define WIN_SPEED_MANT_12   0x10    // 1.2
#define WIN_SPEED_MANT_13   0x18    // 1.3
#define WIN_SPEED_MANT_15   0x20    // 1.5
#define WIN_SPEED_MANT_20   0x28    // 2.0
#define WIN_SPEED_MANT_25   0x30    // 2.5
#define WIN_SPEED_MANT_30   0x38    // 3.0
#define WIN_SPEED_MANT_35   0x40    // 3.5
#define WIN_SPEED_MANT_40   0x48    // 4.0
#define WIN_SPEED_MANT_45   0x50    // 4.5
#define WIN_SPEED_MANT_50   0x58    // 5.0
#define WIN_SPEED_MANT_55   0x60    // 5.5
#define WIN_SPEED_MANT_60   0x68    // 6.0
#define WIN_SPEED_MANT_70   0x70    // 7.0
#define WIN_SPEED_MANT_80   0x78    // 8.0

#define WIN_SPEED_USE_WAIT  0x80    // Use wait if available


    //**********************************************************************
    // Socket information
    //**********************************************************************

    //
    // @struct PDCARD_SOCKET_STATE | Structure used by <f PDCardGetSocket> and
    //      <f PDCardSetSocket> to retrieve or change the specified socket's characteristics.
    //
    typedef struct _SS_SOCKET_STATE
    {
        DWORD   dwEventMask;        // Indicate which PCARD event will gerenrate callback function. same definition of SS_SOCKET_INFO.dwSocketIntrCaps
        DWORD   dwEventChanged;     // Indicate which PCCARD event has happened. same definition of SS_SOCKET_INFO.dwSocketIntrCaps
        DWORD   dwEventStatus;      // Current Event Status of the Card. same definition of SS_SOCKET_INFO.dwSocketIntrCaps
        // If it is set. It will serve as set indicator.
#define SOCK_EVENT_WP       SOCK_CAP_WP             
#define SOCK_EVENT_LOCKED   SOCK_CAP_LOCKED   
#define SOCK_EVENT_EJECT    SOCK_CAP_EJECT     
#define SOCK_EVENT_INSERT   SOCK_CAP_INSERT   
#define SOCK_EVENT_BVD1     SOCK_CAP_BVD1       
#define SOCK_EVENT_BVD2     SOCK_CAP_BVD2       
#define SOCK_EVENT_READY    SOCK_CAP_READY     
#define SOCK_EVENT_CD       SOCK_CAP_CD           
#define SOCK_EVENT_PWRCYCLE SOCK_CAP_PWRCYCLE
#define SOCK_EVENT_CARDBUS_CARD     SOCK_CAP_CARDBUS_CARD
#define SOCK_EVENT_SUSPEND_RESUME   SOCK_CAP_SUSPEND_RESUME

        DWORD   dwInterfaceType;    // @field Memory-only or memory and I/O
#define CFG_IFACE_MEMORY    0x0001  // Put socket controller in memory only interface mode
#define CFG_IFACE_MEMORY_IO 0x0002  // Put socket controller in memory and I/O interface mode
#define CFG_IFACE_CARDBUS   0x0004  // CardBus Card Socket. 
        DWORD   dwInteruptStatus;
#define SOCK_INT_FUNC_IRQ_ROUTING   0x800000  // Enable Client Card Interrupt.
#define SOCK_INT_FUNC_IRQ_WAKE      0x400000  // Enable Client Interrupt Wakeup.
#define SOCK_INT_STATUS_CHG_WAKE    0x200000  // Enable Pccard status change wakeup.

        UINT8   fIREQRouting;         //  IrqRouting Info, if SOCK_CAP_FUNC_IRQ_ROUTING is set.
        UINT8   fVcc;                 // @field Vcc power entry index and status
        UINT8   uVpp1;                // @field Vpp1 power entry index
        UINT8   uVpp2;                // @field Vpp2 power entry index
    } SS_SOCKET_STATE, * PSS_SOCKET_STATE;
    //
    // for fVcc,uVpp1,uVpp2 mask.
#define SOCK_VCC_LEVEL_MASK    0x0F    // Mask for power entry index

    //
    // Memory access struct
    //
    typedef struct _SS_MEMORY_ACCESS
    {
        LPVOID  pCardMem;            // @field pointer into memory space
        DWORD   uOffset;             // @field offset into memory space
        DWORD   dwData;              // @field data to be written or result of read
        DWORD   fFlags;              // @field memory access type
    } SS_MEMORY_ACCESS, * PSS_MEMORY_ACCESS;

    typedef struct _SS_SOCKET_SERVICE
    {
        DWORD   dwVersion;  // Version of this structure.
        STATUS ( *SS_InquireSocket )( HANDLE hSocket,
                                      PSS_SOCKET_INFO pSocketInfo );
        STATUS ( *SS_GetSocket )( HANDLE hSocket,
                                  PSS_SOCKET_STATE pSocketState );
        STATUS ( *SS_SetSocket )( HANDLE hSocket,
                                  PSS_SOCKET_STATE pSocketState );
        STATUS ( *SS_ResetSocket )( HANDLE hSocket );

        STATUS ( *SS_InquireWindow )( UINT32 uWindow,
                                      PSS_WINDOW_INFO pWindowInfo );
        STATUS ( *SS_GetWindow )( UINT32 uWindow,
                                  PSS_WINDOW_STATE pWindowState );
        STATUS ( *SS_SetWindow )( UINT32 uWindow,
                                  PSS_WINDOW_STATE pWindowState );
        STATUS ( *SS_AccessMemory )( HANDLE hSocket,
                                     PSS_MEMORY_ACCESS pMemoryAccess );

        STATUS ( *SS_GetPowerEntry )( HANDLE hSocket,
                                      PDWORD pdwNumOfEnery,
                                      PSS_POWER_ENTRY pPowerEntry );

        STATUS ( *SS_TranslateBusAddr )( HANDLE hSocket,
                                         INTERFACE_TYPE  InterfaceType,
                                         ULONG BusNumber,
                                         PHYSICAL_ADDRESS BusAddress,
                                         PULONG AddressSpace,
                                         PPHYSICAL_ADDRESS TranslatedAddress );
        STATUS ( *SS_TranslateSystemAddr )( HANDLE hSocket,
                                            INTERFACE_TYPE  InterfaceType,
                                            ULONG BusNumber,
                                            PHYSICAL_ADDRESS SystemAddress,
                                            PPHYSICAL_ADDRESS TranslatedAddress );
    } SS_SOCKET_SERVICE, * PSS_SOCKET_SERVICE;

    // Function interface between PDD and MDD(PCCardService).
#define PCCARDSERVICE_KEY TEXT("PCCARDDLL") // Registry Key that point to CPCCard Service.
#define PCCARDHARDWARE_NAME_KEY TEXT("PCCARDNAME") 
    // Socket callback to PCCardService
#define CS_CALLBAKC_NAME TEXT("CS_SocketCallback")
    typedef void ( *PF_SS_CALLBACK )( HANDLE hSocket,
                                      UINT16 uSocket,
                                      PSS_SOCKET_STATE pSocketState ) ; 
    // Socket to Get or Release Socket Index Number that assinged by CPCCard.
#define CS_ADD_SOCKET TEXT("CS_AddSocket")
    typedef STATUS ( *PF_CS_ADD_SOCKET )( PUINT16 puSocket,
                                          DWORD dwSize,
                                          const SS_SOCKET_SERVICE* pSocketService,
                                          LPCTSTR lpcHardwareName ); 
#define CS_DELETE_SOCKET TEXT("CS_DeleteSocket")
    typedef STATUS ( *PF_CS_DELETE_SOCKET )( UINT16 uSocket );

    //
    //
    // @type STATUS | Set of return codes used by Socket Services
    //
#define CERR_SUCCESS              0x00
#define CERR_BAD_ADAPTER          0x01
#define CERR_BAD_ATTRIBUTE        0x02
#define CERR_BAD_BASE             0x03
#define CERR_BAD_EDC              0x04
#define CERR_BAD_IRQ              0x06
#define CERR_BAD_OFFSET           0x07
#define CERR_BAD_PAGE             0x08
#define CERR_READ_FAILURE         0x09
#define CERR_BAD_SIZE             0x0A
#define CERR_BAD_SOCKET           0x0B
#define CERR_BAD_TYPE             0x0D
#define CERR_BAD_VCC              0x0E
#define CERR_BAD_VPP              0x0F
#define CERR_BAD_WINDOW           0x11
#define CERR_WRITE_FAILURE        0x12
#define CERR_NO_CARD              0x14
#define CERR_UNSUPPORTED_SERVICE  0x15
#define CERR_UNSUPPORTED_MODE     0x16
#define CERR_BAD_SPEED            0x17
#define CERR_BUSY                 0x18
#define CERR_GENERAL_FAILURE      0x19
#define CERR_WRITE_PROTECTED      0x1A
#define CERR_BAD_ARG_LENGTH       0x1B
#define CERR_BAD_ARGS             0x1C
#define CERR_CONFIGURATION_LOCKED 0x1D
#define CERR_IN_USE               0x1E
#define CERR_NO_MORE_ITEMS        0x1F
#define CERR_OUT_OF_RESOURCE      0x20
#define CERR_BAD_HANDLE           0x21
#define CERR_BAD_VERSION          0x22

    //
    // PCMCIA memory access functions
    //
    // Standard PCMCIA controllers provide several memory windows to access the
    // the different areas of card memory.  Individual windows can access common
    // or attribute memory and the controller asserts the correct signals when a
    // particular window is used.
    // On some platforms, software must toggle all the PCMCIA signals, hence access
    // must be controlled via an API which can properly serialize conflicting
    // accesses.
    //
    //UINT8 PDCardReadAttrByte (PVOID pCardMem, UINT32 uOffset);
    //UINT8 PDCardReadCmnByte  (PVOID pCardMem, UINT32 uOffset);
    //UINT8 PDCardReadIOByte   (PVOID pCardMem, UINT32 uOffset);
    //VOID  PDCardWriteAttrByte(PVOID pCardMem, UINT32 uOffset, UINT8 uByte);
    //VOID  PDCardWriteCmnByte (PVOID pCardMem, UINT32 uOffset, UINT8 uByte);
    //VOID  PDCardWriteIOByte  (PVOID pCardMem, UINT32 uOffset, UINT8 uByte);

#ifdef __cplusplus
}
#endif

#endif

