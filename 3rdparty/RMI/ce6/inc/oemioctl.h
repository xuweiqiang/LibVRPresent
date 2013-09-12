/*****************************************************************************
Copyright 2003-2008 RMI Corporation(RMI). All rights reserved.

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

#ifndef __OEMIOCTL_H__
#define __OEMIOCTL_H__

#include <winioctl.h>
#include <windef.h>

//
// System ID
//
#define IOCTL_OEM_GET_SYSTEMID \
    CTL_CODE(FILE_DEVICE_HAL, 52000, METHOD_NEITHER, FILE_ANY_ACCESS)

//
// GET_SYSTEMID read structure
//
typedef struct _SYSTEMID {
	ULONG	prid;			// CP0 Register 15, sel 0
	ULONG   brdid;			// Specific to Board -- Optional
	BYTE	guid[32];		// Programmed in non-volitle memory, otp, flash etc
	ULONG	reserved0;		// reserved data
	ULONG	reserved1;		// reserved data
} SYSTEMID, *PSYSTEMID;


//
// GPIO Config Request
//
#define IOCTL_OEM_GPINTR_REQ \
    CTL_CODE(FILE_DEVICE_HAL, 52001, METHOD_NEITHER, FILE_ANY_ACCESS)

//
// GPINTR_REQ structure
//
typedef struct _GPINTRREQ {
	ULONG	gpintr;			// GPINTR pin ( 0 - 127 ). See Au1x00.h GPINRT
    ULONG   op;				// Read State, Output State
	ULONG   cfg;			// GPINTR Config
	ULONG	reserved0;		// reserved data
} GPINTRREQ, *PGPINTRREQ;

#define GPINTR_REQ_READSTATE		0x01
#define GPINTR_REQ_OUTPUT_0			0x02
#define GPINTR_REQ_OUTPUT_1			0x04
#define GPINTR_REQ_GET_CFG			0x08
#define GPINTR_REQ_SET_CFG			0x10
#define GPINTR_REQ_GET_RSTVAL		0x20

#if defined(SOC_AU13XX)
//
// Handler prototype. Definition in gpintr.c
//
BOOL OALIoCtlOemGpintrReq(
    UINT32 code, VOID* pInpBuffer, UINT32 inpSize, VOID* pOutBuffer,
    UINT32 outSize, UINT32 *pOutSize );
#endif


#if 0
			#define IOCTL_HAL_INTR_CONNECT \
			    CTL_CODE(FILE_DEVICE_HAL, 52002, METHOD_NEITHER, FILE_ANY_ACCESS)

			#define IOCTL_HAL_INTR_DISCONNECT \
			    CTL_CODE(FILE_DEVICE_HAL, 52003, METHOD_NEITHER, FILE_ANY_ACCESS)


			typedef struct _INTR_CONNECT_IOCTL_PARAM {
			    ULONG      InterfaceType;
			    ULONG      BusNumber;
			    ULONG      BusInterrupt;
			    ULONG      InterruptMode;
			    ULONG      Sysintr;
			} INTR_CONNECT_IOCTL_PARAM, *PINTR_CONNECT_IOCTL_PARAM;
#endif

typedef struct _INTR_START_TIMER_IOCTL_PARAM {
    ULONG SysIntr;
    ULONG PeriodHns;
} INTR_START_TIMER_IOCTL_PARAM, *PINTR_START_TIMER_IOCTL_PARAM;

#define IOCTL_HAL_QUERY_CORE_SPEED \
    CTL_CODE(FILE_DEVICE_HAL, 52004, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_QUERY_PBUS_SPEED \
    CTL_CODE(FILE_DEVICE_HAL, 52005, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_ADD_WIRED_TLB \
    CTL_CODE(FILE_DEVICE_HAL, 52006, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_INTR_CONNECT_TIMER \
    CTL_CODE(FILE_DEVICE_HAL, 2050, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_INTR_DISCONNECT_TIMER \
    CTL_CODE(FILE_DEVICE_HAL, 2051, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_INTR_START_TIMER \
    CTL_CODE(FILE_DEVICE_HAL, 2052, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_INTR_STOP_TIMER \
    CTL_CODE(FILE_DEVICE_HAL, 2053, METHOD_NEITHER, FILE_ANY_ACCESS)

typedef struct _ADD_WIRED_TLB_IOCTL_PARAM
{
	IN ULONG EntryHi;
	IN ULONG EntryLo0;
	IN ULONG EntryLo1;
	IN ULONG PageMask;
} ADD_WIRED_TLB_IOCTL_PARAM;


//
// VSSREQ structure
//
typedef struct _VSSREQ {
	ULONG	block;			// Index for VSS block to control (See au1x00.h)
    BOOL    enable; 		// TRUE, FALSE
	ULONG   reserved0;		// reserved
	ULONG	reserved1;		// reserved
} VSSREQ, *PVSSREQ;

#define IOCTL_HAL_VSS \
    CTL_CODE(FILE_DEVICE_HAL, 52007, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_MARKEV \
    CTL_CODE(FILE_DEVICE_HAL, 52008, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_EVENT_BUFFER_SIZE 1024*1024
#define IOCTL_HAL_READEV \
    CTL_CODE(FILE_DEVICE_HAL, 52009, METHOD_NEITHER, FILE_ANY_ACCESS)




//
// Peripheral Info
//
#define IOCTL_OEM_GET_PERIPHERAL_INFO \
    CTL_CODE(FILE_DEVICE_HAL, 52010, METHOD_NEITHER, FILE_ANY_ACCESS)


enum {
	GET_PERIPHERAL_INFO_3D_GP_ID = 0,
	GET_PERIPHERAL_INFO_3D_PP_ID,
	GET_PERIPHERAL_INFO_OTG_ID,
	GET_PERIPHERAL_INFO_OTG_CONFIG1,
	GET_PERIPHERAL_INFO_OTG_CONFIG2,
	GET_PERIPHERAL_INFO_OTG_CONFIG3,
	GET_PERIPHERAL_INFO_OTG_CONFIG4,
	GET_PERIPHERAL_INFO_CP0_CONFIG1
};


#endif __OEMIOCTL_H__
