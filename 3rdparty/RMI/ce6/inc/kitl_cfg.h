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

#ifndef __KITL_CFG_H
#define __KITL_CFG_H

#include <oal.h>
#include <platform.h>

#ifdef LAN91C111_PHYS_ADDR
#include "lan91c111.h"
#endif

#ifdef LAN9210_PHYS_ADDR
#include "lan9210.h"
#endif

//------------------------------------------------------------------------------


#ifdef LAN91C111_PHYS_ADDR
OAL_KITL_ETH_DRIVER g_kitlEthLan91c = {
    Lan91c111_Init,
    NULL,
    NULL,
    Lan91c111_SendFrame,
    Lan91c111_GetFrame,
    Lan91c111_EnableInts,
    Lan91c111_DisableInts,
    NULL,
    NULL,
    Lan91c111_ApplyPacketFilter,
    Lan91c111_ApplyMulticastList
};

OAL_KITL_DEVICE g_kitlDevices[] = {
    {
        L"SMSC Lan91c ", Internal, LAN91C111_PHYS_ADDR, LAN91C111_PHYS_ADDR, OAL_KITL_TYPE_ETH,
        &g_kitlEthLan91c
    }, {
        NULL, 0, 0, 0, 0, NULL
    }
};
#endif //LAN91C111_PHYS_ADDR


#ifdef LAN9210_PHYS_ADDR
OAL_KITL_ETH_DRIVER g_kitlEthLan9210 = OAL_ETHDRV_LAN911x;

OAL_KITL_DEVICE g_kitlDevices[] = {
    {
        L"SMSC Lan9210 ", Internal, LAN9210_PHYS_ADDR, LAN9210_PHYS_ADDR, OAL_KITL_TYPE_ETH,
        &g_kitlEthLan9210
    }, {
        NULL, 0, 0, 0, 0, NULL
    }
};
#endif


//------------------------------------------------------------------------------

#endif
