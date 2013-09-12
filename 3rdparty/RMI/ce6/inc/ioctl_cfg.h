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
//
//  Header: ioctl_cfg.h
//
//  This file contains constant used in info OEMIoControl calls
//
#ifndef __IOCTL_CFG_H
#define __IOCTL_CFG_H

#if __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
//
//  Define: IOCTL_PLATFORM_TYPE/OEM
//
//  Defines the platform type and OEM string.
//

#if defined(SOC_AU1200)
#ifdef BUILD_PB1200
	#define IOCTL_PLATFORM_TYPE                 L"PbAu1200"
	#define IOCTL_PLATFORM_OEM                  L"RMI Alchemy Au12xx"
#else
	#define IOCTL_PLATFORM_TYPE                 L"DbAu1200"
	#define IOCTL_PLATFORM_OEM                  L"RMI Alchemy Au12xx"
#endif
#elif defined(SOC_AU13XX)
	#define IOCTL_PLATFORM_TYPE                 L"DbAu13xx"
	#define IOCTL_PLATFORM_OEM                  L"RMI Alchemy Au13xx"
#endif

//------------------------------------------------------------------------------
//
//  Define: IOCTL_PROCESSOR_VENDOR/NAME/CORE
//
//  Defines the processor information
//

#define IOCTL_PROCESSOR_VENDOR              L"RMI"
#if defined(SOC_AU1200)
#define IOCTL_PROCESSOR_NAME                L"Au12xx"
#elif defined(SOC_AU13XX)
#define IOCTL_PROCESSOR_NAME                L"Au13xx"
#endif
#define IOCTL_PROCESSOR_CORE                L"MIPS32"

//------------------------------------------------------------------------------
//
//  Define: IOCTL_PROCESSOR_INSTRUCTION_SET
//
//  Defines the processor instruction set information
//

#define IOCTL_PROCESSOR_INSTRUCTION_SET     0
#define IOCTL_PROCESSOR_CLOCK_SPEED         0

//------------------------------------------------------------------------------

#if __cplusplus
}
#endif

#endif
