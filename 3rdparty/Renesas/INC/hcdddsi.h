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
    hcdddsi.h
    
Abstract:  
    This file provides the definitions for the USB HCD MDD->PDD 
    interface.
Notes: 

    @doc DRIVERS           
    
    @topic   Creating an HCI driver for Windows CE    |
    
             The Windows CE Universal Host Controller Driver (HCD) module consists of
             two portions - a platform independant portion (MDD) which is provided with
             the OS in public\\common\\oak\\drivers\\usb\\hcd\\uhcd, and a platform 
             dependant portion (PDD), which must be written by the OEM.  
             
             The bulk of the processing occurs in the MDD, with the PDD providing 
             the necessary glue to interface the hardware to a particular platform.  
             Since the HCI spec is very detailed as to how the hardware should behave, 
             there is very little that needs to be done in the PDD, primarily locating 
             the card at init, and providing the MDD with pointers to the hardware and 
             shared memory area.
             
             To configure the HCI driver to be loaded by device.exe during system
             startup, the standard driver registry settings should be added to the
             platform.reg file, for example
             

             ; USB - HCI driver <nl>
             [HKEY_LOCAL_MACHINE\\Drivers\\BuiltIn\\HCI] <nl>
                "Prefix"="HCD"   <nl>
                "Dll"="uhci.dll" <nl>
                "Index"=dword:1  <nl>
                "Order"=dword:1  <nl>
             
             This will cause device.exe to load uhci.dll, and call the initialization
             routine.
--*/
#ifndef   __HCDDDSI_H__
#define   __HCDDDSI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Debug defs */
#ifdef DEBUG
#define ZONE_HCD               DEBUGZONE(0)
#define ZONE_INIT               DEBUGZONE(1)
#define ZONE_REGISTERS          DEBUGZONE(2)
#define ZONE_HUB                DEBUGZONE(3)
#define ZONE_ATTACH             DEBUGZONE(4)
#define ZONE_DESCRIPTORS        DEBUGZONE(5)
#define ZONE_FUNCTION           DEBUGZONE(6)
#define ZONE_PIPE               DEBUGZONE(7)
#define ZONE_TRANSFER           DEBUGZONE(8)
#define ZONE_QH                 DEBUGZONE(9)
#define ZONE_TD                 DEBUGZONE(10)
#define ZONE_CPHYSMEM           DEBUGZONE(11)
#define ZONE_VERBOSE            DEBUGZONE(12)
#define ZONE_WARNING            DEBUGZONE(13)
#define ZONE_ERROR              DEBUGZONE(14)
#define ZONE_UNUSED             DEBUGZONE(15)

#define ZONE_DEVICE             (ZONE_HUB || ZONE_FUNCTION)
#endif

/* @topic  HCD MDD functions |
 *         The following functions are exported by the HCD MDD code.  They must be 
 *         called at various points in the PDD code to allow the MDD to update state.
 */

/* @func  LPVOID | HcdMdd_CreateMemoryObject | Create the HCI shared memory area.
 * 
 * @comm  This function is called by the HCD PDD to initialize the shared memory area used
 *        by the HCI driver.  This area is used to allocate DMA buffers for client driver
 *        transfers.
 *
 *        There are two options available - if a platform has an area of memory set aside that
 *        can be used by HCI (e.g. specified in the config.bib file), it may pass the virtual
 *        and physical address pointers in to this function. This is the recommended method.
 *        Alternatively, the PDD may pass NULL in for the physical and virtual address pointers,
 *        in which case the <f HcdMdd_CreateMemoryObject> function will attempt to allocate a
 *        suitable memory buffer from system RAM. 
 *
 *        The HCI shared memory area is broken into two separate areas - a high priority pool
 *        for isochronous client buffers, and a lower priority pool for other transfers.
 *        Refer to the section on client driver transfers for a description of how client
 *        driver buffer management works.
 *
 *        The size of the shared memory area is dependant on the perceived usage of USB within the
 *        platform.  The HCI controller uses 4K page boundaries for internal data structures,
 *        so the size should be a multiple of 4K, and must be at least 8K, for the control
 *        structures and memory needed for device configuration.  Additionally, client drivers
 *        may need to utilize this area for their transfers, so it is recommended that additional
 *        memory be provided.  The maximum DMA memory used for a client transfer is 8K, so OEMs should
 *        try to determine what a reasonable number of outstanding client transfers may be.
 *
 *        A recommended value for general purpose USB operation is 38K, with 20K reserved 
 *        for high priority transfers.  Platforms which anticipate less demand on the USB subsystem
 *        can safely reduce this to 28K (20K high priority).  Reducing the size of the high priority
 *        buffer may cause isoch transfers to fail if memory cannot be allocated, whereas reducing
 *        the size of the non high priority buffer will reduce performance on bulk transfers (but
 *        won't cause failures, since bulk transfers can block to wait for memory to become available).
 *
 *        If possible, OEMs should consider placing the HCD shared memory area in high 
 *        speed memory, particularly to support streaming (isochronous) data transfers.
 *
 * @rdesc Returns pointer to HCD memory object, or NULL if failure occurs.
 */
LPVOID  HcdMdd_CreateMemoryObject(
                                   DWORD cbSize,            // @parm [IN] - Total size of HCD shared memory area.
                                   DWORD cbHighPrioritySize,// @parm [IN] - Bytes reserved for high priority (isoch/interrupt) transfers
                                   PUCHAR pVirtAddr,        // @parm [IN] - Virtual address of memory area (may be NULL).
                                   PUCHAR pPhysAddr);       // @parm [IN] - Physical address of memory area (may be NULL).

/* @func  BOOL | HcdMdd_DestroyMemoryObject | Clean up an HCI memory object.
 *
 * @comm  This function should be called when an HCI controller is removed, or if an error
 *        occurs during initialization, to allow the MDD to clean up internal data structures.
 *
 * @rdesc Returns TRUE if successful, FALSE if error occurs.
 */
BOOL    HcdMdd_DestroyMemoryObject(
                                    LPVOID lpvMemoryObject); // @parm [IN] - Pointer to HCD memory object.

/* @func  LPVOID | HcdMdd_CreateHcdObject | Create and initialize HCI driver object.
 * 
 * @comm  This function is called by the PDD during initialization to initialize an HCI
 *        driver object. As part of this processing, the USB Driver (USBD.DLL) is loaded,
 *        and the HCI interrupt is hooked into the system.  Then, the HCI hardware is
 *        placed into OPERATIONAL state, and list processing begins (device attach events
 *        are serviced, transfers initiated, etc).
 *        
 *        There can only be one HCI host controller active in the system.
 *
 * @rdesc Returns pointer to HCI driver object, or NULL if failure occurs.
 */
LPVOID  HcdMdd_CreateHcdObject(
                                 LPVOID lpvHcdPddObject, // @parm [IN] - Pointer to PDD specific data for this instance.
                                 LPVOID lpvMemoryObject,  // @parm [IN] - Memory object created by <f HcdMdd_CreateMemoryObject>.
                                 LPCWSTR szRegKey,        // @parm [IN] - Registry key where HCI configuration settings are stored.
                                 PUCHAR ioPortBase,       // @parm [IN] - Pointer to HCI controller registers.
                                 DWORD dwSysIntr);        // @parm [IN] - Logical value for HCI interrupt (SYSINTR_xx).

/* @func  BOOL | HcdMdd_DestroyHcdObject | Clean up an HCI driver object.
 *
 * @comm  This function should be called when an HCI controller is removed, or if an error
 *        occurs during initialization, to allow the MDD to clean up internal data structures.
 * @rdesc Returns TRUE if successful, FALSE if error occurs.
 */
BOOL    HcdMdd_DestroyHcdObject(
                                  LPVOID lpvHcdObject); // @parm [IN] - Pointer to HCI driver object.

/* @func  BOOL | HcdMdd_PowerUp | HCD MDD power up handler.
 *
 * @comm  This function should be called by the PDD as part of the processing during
 *        <f HcdPdd_PowerUp>.  It will cause the MDD to restore state and reinitialize
 *        the HCI hardware.  All client drivers that were loaded at the time of power
 *        down will be unloaded.  Once this is completed, the HCI hardware will be placed
 *        into the OPERATIONAL state, and drivers for devices which are still attached to
 *        the USB will be loaded.
 * @rdesc Return TRUE if successful, FALSE if error occurs.
 */
BOOL    HcdMdd_PowerUp(
                        LPVOID lpvHcdObject);   // @parm [IN] - Pointer to HCI driver object.

/* @func  BOOL | HcdMdd_PowerDown | HCD MDD power down handler.
 *
 * @comm  This function should be called by the PDD as part of the processing during
 *        <f HcdPdd_PowerDown>.  It will cause the MDD to save state and prepare the
 *        HCI controller for power down.  The HCI controller will be placed into
 *        reset (SOF generation disabled).
 * @rdesc Return TRUE if successful, FALSE if error occurs.
 */
BOOL    HcdMdd_PowerDown(
                          LPVOID lpvHcdObject);   // @parm [IN] - Pointer to HCI driver object.

/* @func  BOOL | HcdMdd_SetCapability  | HCD MDD Set Capability.
 *
 * @comm  This function should be called by the PDD as part of the initialization.
 *        For backward compable, mdd assume all the Capability bit is zero if it is
 *        not called. It currently only support HCD_SUSPEND_RESUME bit that tell mdd this
 *        device can do suspend and resume according to the HCI spec.
 * @rdesc Return TRUE if successful, FALSE if error occurs.
 */
#define HCD_SUSPEND_RESUME 1
#define HCD_ROOT_HUB_INTERRUPT (1<<1)
#define HCD_SUSPEND_ON_REQUEST (1<<2)
DWORD   HcdMdd_SetCapability (LPVOID lpvHcdObject,// @parm [IN] - Pointer to HCI driver object.
                                DWORD );   // @parm [IN] - Bitmap for Capabliity
/* @topic  HCD PDD functions |
 *         The following functions must be provided by the HCD PDD code.
 *
 */

/* @func  BOOL | HcdPdd_DllMain | DLL Entry point for PDD portion of HCI driver.
 *
 * @comm  This function is provided to allow the PDD to perform any processing at
 *        DLL entry time.  Generally, this function doesn't do much and most initialization
 *        should be performed in the <f HcdPdd_Init> routine.
 *
 * @rdesc Return TRUE if successful, FALSE if error occurs.
 */
BOOL    HcdPdd_DllMain(
                        HANDLE hinstDLL,    // @parm [IN] - Handle to DLL instance.
                        DWORD  dwReason,    // @parm [IN] - Reason code. 
                        LPVOID lpvReserved);// @parm [IN] - System parameter.

/* @func  DWORD | HcdPdd_Init | Initialize HCI platform specific HW and structures.
 *
 * @comm  PDD Entry point - called at system init to detect and configure HCI card.
 *        As part of the processing for this function, the <f HcdMdd_CreateHcdObject>
 *        function should be called to initialize the MDD and HCI hardware.
 *
 * @rdesc Return pointer to PDD specific data structure, or NULL if error.
 */
DWORD   HcdPdd_Init(
                     DWORD dwContext);  // @parm [IN] - Pointer to a string indicating our device registry key.

/* @func  BOOL | HcdPdd_CheckConfigPower | Check power for requested configuration.
 *
 * @comm  When a device is attached to a root hub port, HCI selects a configuration based
 *        on the power requirements of the system, by calling this function for each
 *        configuration and then selecting the configuration with the highest power
 *        requirements. If no configurations can be supported, the device is not configured.
 *
 *        This allows the platform to perform additional processing if necessary (e.g.
 *        checking whether on AC or batteries, possibly sampling current system power
 *        status, etc). 
 *
 * @rdesc Return TRUE if configuration can be supported, FALSE if not.
 */
BOOL    HcdPdd_CheckConfigPower(
                                 UCHAR bPort,         // @parm [IN] - Root hub port # (1-N).
                                 DWORD dwConfigPower, // @parm [IN] - Milliamps required by configuration.
                                 DWORD dwTotalPower); // @parm [IN] - Power drawn by other devices on this port (ma).

/* @func  void | HcdPdd_PowerUp | HCD power up handler.
 *
 * @comm  This function is called during the system power on sequence.  
 *        In this function, the <f HcdMdd_PowerUp> function should be called to allow
 *        the MDD to restore state information.  The HCI memory area (as passed to  
 *        <f HcdMdd_CreateHcdObject>) must be preserved across a suspend/resume cycle.  
 *        Note that no system calls may be made from within this function.
 */
void    HcdPdd_PowerUp(
                        DWORD dwDeviceContext); // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>

/* @func  void | HcdPdd_PowerDown | HCD power off handler.
 *
 * @comm  This function is called during the system power off sequence.
 *        In this function, the <f HcdMdd_PowerDown> function should be called to allow
 *        the MDD to save state information.  Then, any processing specific to the platform
 *        (e.g. removing power from the host controller hardware) can be done.  Note that
 *        no system calls may be made from within this function.
 */
void    HcdPdd_PowerDown(
                          DWORD dwDeviceContext);  // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>

/* @func  BOOL | HcdPdd_Deinit | HCD deinitialization routine.
 * 
 * @comm  This function is called if the HCI driver is unloaded.  As part of the processing
 *        for this function, <f HcdMdd_DestroyHcdObject> should be called to allow the MDD
 *        to clean up internal data structures.
 * @rdesc Return TRUE if deinit was successful, FALSE if error occurred.
 */
BOOL    HcdPdd_Deinit(
                       DWORD dwDeviceContext); // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>

/* @func  DWORD | HcdPdd_Open | HCD open routine.
 *
 * @comm  This is the standard stream interface driver open routine.  It is currently unused.
 * @rdesc For future compatibility, drivers should return 1.
 */
DWORD   HcdPdd_Open(
                     DWORD dwDeviceContext,  // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>
                     DWORD dwAccessCode,     // @parm - Unused
                     DWORD dwShareMode);     // @parm - Unused

/* @func  BOOL | HcdPdd_Close | HCD close routine.
 *
 * @comm  This is the standard stream interface driver close routine.  It is currently unused.
 * @rdesc For future compatibility, drivers should return TRUE.
 */
BOOL    HcdPdd_Close(
                      DWORD dwOpenContext);  // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>

/* @func  DWORD | HcdPdd_Read | HCD read routine.
 *
 * @comm  This is the standard stream interface driver read routine.  It is currently unused.
 * @rdesc For future compatibility, drivers should return -1.
 */
DWORD   HcdPdd_Read(
                     DWORD dwOpenContext,    // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>
                     LPVOID lpvBuffer, 
                     DWORD dwCount);

/* @func  DWORD | HcdPdd_Write | HCD write routine.
 *
 * @comm  This is the standard stream interface driver write routine.  It is currently unused.
 * @rdesc For future compatibility, drivers should return -1.
 */
DWORD   HcdPdd_Write(
                      DWORD dwOpenContext,    // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>
                      LPCVOID lpvSourceBytes,
                      DWORD dwNumberOfBytes);

/* @func  DWORD | HcdPdd_Seek | HCD seek routine.
 *
 * @comm  This is the standard stream interface driver seek routine.  It is currently unused.
 * @rdesc For future compatibility, drivers should return -1.
 */
DWORD   HcdPdd_Seek(
                     DWORD dwOpenContext,     // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>
                     LONG lAmount, 
                     DWORD dwType);

/* @func  BOOL | HcdPdd_IOControl | HCD ioctl routine.
 *
 * @comm  This is the standard stream interface driver ioctl routine.  It is currently unused.
 * @rdesc For future compatibility, drivers should return FALSE.
 */
BOOL    HcdPdd_IOControl(
                          DWORD dwOpenContext, // @parm [IN] - Pointer to device context returned from <f HcdPdd_Init>
                          DWORD dwCode, 
                          PBYTE pbBufIn,
                          DWORD dwLenIn, 
                          PBYTE pbBufOut, 
                          DWORD dwLenOut,
                          PDWORD pdwActualOut);


/* @func  void | HcdPdd_InitiatePowerUp.
 *
 * @comm  This function will reinitialize the HCD after a Resume event. It should be called from the USB IST after a
 *        resume has been detected.
 * 
 */
void    HcdPdd_InitiatePowerUp(DWORD dwDeviceContext);

// Define Common Registry Entry.
#define HCD_CAPABILITY_VALNAME (TEXT("HcdCapability"))
#define HCD_CAPABILITY_VALTYPE REG_DWORD
//Global Active context (valid only in HcdPdd_Init).
extern DWORD g_dwContext;


#ifdef __cplusplus
}
#endif

#endif
