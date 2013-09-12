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

#ifndef __IOCTL_H__
#define __IOCTL_H__

#include <winioctl.h>
#include <windef.h>
#include "bceddk.h"

//
// HAL/OAL IOCTLs used by bceddk.dll and other modules to communicate with the kernel.
//

#define IOCTL_HAL_QUERY_CORE_SPEED \
    CTL_CODE(FILE_DEVICE_HAL, 2065, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_ADD_WIRED_TLB \
    CTL_CODE(FILE_DEVICE_HAL, 2068, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_HAL_QUERY_PBUS_SPEED \
    CTL_CODE(FILE_DEVICE_HAL, 2069, METHOD_NEITHER, FILE_ANY_ACCESS)


//
// This is defined in <ceddk.h> and <wdm.h>
// Unfortunately these are partially overlapping and we can include only one of them.
// To not force the module to include a particular of these, and break the builds which
// do differently, we don't include one of these files here, but declare the types directly.
// Granted, this will cause problems if we include "ioctl.h" before including the other file,
// but headers are full of this type of contradictions anyway.
//

#if !defined(_CEDDK_) && !defined(_WDMDDK_)

typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    MaximumInterfaceType
} INTERFACE_TYPE, *PINTERFACE_TYPE;

//
// Define types of bus information.
//

typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos,
    EisaConfiguration,
    Pos,
    CbusConfiguration,
    PCIConfiguration,
    VMEConfiguration,
    NuBusConfiguration,
    PCMCIAConfiguration,
    MPIConfiguration,
    MPSAConfiguration,
    PNPISAConfiguration,
    MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;

#endif //#if !defined(_CEDDK_) && !defined(_WDMDDK_)


//
// Structure used for passing parameters from InterruptStartTimer
// to the HAL.
//

typedef struct _INTR_START_TIMER_IOCTL_PARAM {
    ULONG SysIntr;
    ULONG PeriodHns;
} INTR_START_TIMER_IOCTL_PARAM, *PINTR_START_TIMER_IOCTL_PARAM;




#endif __IOCTL_H__
