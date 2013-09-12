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

extern HANDLE
GPIO_Init(
    VOID);

extern VOID
GPIO_Close(
    HANDLE);

extern BOOL
GPIO_GetPinFunction(
    HANDLE Handle,
    PULONG PinFunc);


extern BOOL
GPIO_ClearPinFunction(
    HANDLE Handle,
    ULONG PinStates);

extern BOOL
GPIO_SetPinFunction(
    HANDLE Handle,
    ULONG PinFunc);




extern BOOL
GPIO_GetTristate(
    HANDLE Handle,
    PULONG PinTristates);

extern BOOL
GPIO_SetAsOutput(
    HANDLE Handle,
    ULONG PinsToSet);

extern BOOL
GPIO_SetAsInput(
    HANDLE Handle,
    ULONG PinsToSet);




extern BOOL
GPIO_ClearPinOutputState(
    HANDLE Handle,
    ULONG PinsToClear);

extern BOOL
GPIO_SetPinOutputState(
    HANDLE Handle,
    ULONG PinsToSet);

extern BOOL
GPIO_GetState(
    HANDLE Handle,
    PULONG PinStates);

extern BOOL
GPIO_SetClockSourceControl(
    HANDLE Handle,
    ULONG   AndMask,
    ULONG   OrMask);

extern BOOL
GPIO_SetFrequencyControl1(
    HANDLE Handle,
    ULONG   AndMask,
    ULONG   OrMask);
extern BOOL
GPIO_SetFrequencyControl0(
    HANDLE Handle,
    ULONG   AndMask,
    ULONG   OrMask);


extern BOOL
GPIO2_GetPinState(
    HANDLE Handle,
    PULONG PinState
 );

extern BOOL
GPIO2_GetDirection(
    HANDLE Handle,
    PULONG Direction
 );

extern BOOL
GPIO2_SetDirection(
    HANDLE Handle,
    ULONG   AndMask,
    ULONG   OrMask
);

extern BOOL
GPIO2_GetDatatOutput(
    HANDLE Handle,
    PULONG DataOutput
);


extern BOOL
GPIO2_SetDataOutput(
    HANDLE Handle,
    ULONG  State
);


