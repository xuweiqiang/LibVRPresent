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
#ifndef __ACLINKCONTORL_H__
#define __ACLINKCONTORL_H__

#ifdef __cplusplus
extern "C"{
#endif

// DEV_... constants are used as bit identifiers in a uchar, so must be <=7
#define DEV_TOUCH   1
#define DEV_AUDIO   2
#define DEV_BATTERY 3

// Name of mutex for accessing touch/audio registers
#define ACLINK_MUTEX_NAME   TEXT("ACLINK_CONTROL")  
#define AC97_SHARED_FILENAME    TEXT("AC97_SHARED_FILE")
#define COUNT_MAX       10000000
static HANDLE hACLinkControlMutex = NULL;

typedef struct
{
    int InitRefCnt; // reference count
} AC97_GLOBALS;

// mask definitions for the CodecAccessRegister
#define CLEAR(reg, bit_mask) (reg &= ~bit_mask)
#define TEST(reg, bit_mask)  (reg & bit_mask)


BOOL ReleaseAC97Lock(void);
BOOL GetAC97Lock(void);
BOOL UnConfigureAC97Control(void);
BOOL ConfigureAC97Control(void);
BOOL WriteAC97(UINT Offset, UINT16 Data, UINT8 DevId);
BOOL ReadAC97(UINT Offset, UINT16 *pData, UINT8 DevId);
BOOL WriteAC97Raw(UINT Offset, UINT16 Data, UINT8 DevId);
BOOL ReadAC97Raw(UINT Offset, UINT16 *pData, UINT8 DevId);
BOOL InitializeACLink(BOOL, UINT8);
BOOL DeInitializeACLink(BOOL, UINT8);
BOOL ColdResetAC97Control(void);
static BOOL AllocateACLinkResources(UINT8);
static BOOL DeAllocateACLinkResources(UINT8 DevId);
BOOL StartAC97Rx(void);
BOOL StartAC97Tx(void);
BOOL StopAC97Rx(void);
BOOL StopAC97Tx(void);

#ifdef __cplusplus
}
#endif

#endif //__ACLINKCONTORL_H__
