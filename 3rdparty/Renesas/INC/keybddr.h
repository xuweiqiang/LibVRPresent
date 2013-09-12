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

keybddr.h

Abstract:

Interface to the Windows CE keyboard driver.

Notes:


--*/

//      @doc    EXTERNAL DRIVERS KEYBD_DRIVER
//      @module keybddr.h | Interface to the Windows CE keyboard driver.

#ifndef __KEYBDDR_H__
#define __KEYBDDR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <types.h>
#include <keybd.h>



/*	KEYBOARD DRIVER INTERFACE */

/*	VirtualKey argument layout.

	33222222 22221111 11111100 00000000
	10987654 32109876 54321098 76543210
		  RS II  EEEE          VVVVVVVV

	S = Silent, no key click sound
	R = Repeat Silent, no key click on an autorepeat
	E = Event Code
	I = Id of Keyboard
	V = Virtual key code
*/

//	More flags that aren't needed in key state information.
#define KEYBD_DEVICE_SILENT			0x01000000
#define KEYBD_DEVICE_SILENT_REPEAT	0x02000000

//	Keyboard identifier.
#define KEYBD_DEVICE_ID_MASK		0x00C00000
#define KEYBD_DEVICE_ID0			0x00000000
#define KEYBD_DEVICE_ID1			0x00400000
#define KEYBD_DEVICE_ID2			0x00800000
#define KEYBD_DEVICE_ID3			0x00C00000

//	Non-virtual-key events which must be signaled to the user input system.
//	This field is 0 for both virtual key press and virtual key release events.
#define KEYBD_VKEY_EVENT_MASK		0x000f0000
#define KEYBD_DEVICE_CONNECT		0x00010000
#define KEYBD_DEVICE_DISCONNECT		0x00020000
#define KEYBD_DEVICE_CONTRAST_LESS	0x00030000
#define KEYBD_DEVICE_CONTRAST_MORE	0x00040000



//	@doc    EXTERNAL DRIVERS KEYBD_DRIVER
/*	@func   void |
			KeybdEventCallback |
			This is the signature of the callback function of <f KeybdDriverInitialize>.

@parm	UINT32			| VirtualKey	| The virtual key/event code.
@parm   KEY_STATE_FLAGS	| KeyEvent		| The KeyStateDownFlag set or cleared.

@xref
	<tab><f KeybdDriverInitialize><nl>
	<tab><t KEY_STATE_FLAGS>

@comm The keyboard driver does not implement this function.  This is the
signature of the function which is passed in to the driver by the user
input system when it calls <f KeybdDriverInitialize>.  The driver should
remember it and call it whenever there is a keyboard event.

@comm The KeyStateDownFlag in the KeyEvent parameter should be set for a
key down event and clear for a key up event.  All other bits must be 0.

@comm The VirtualKey argument has more than just the virtual key
information.  See keybddr.h for a description of the layout of the
information in this argument.

@comm When a keyboard is connected or disconnected, an event must be
signalled with the appropriate connect or disconnect event code set in the
VirtualKey argument.

*/

typedef BOOL    (*PFN_KEYBD_EVENT_CALLBACK)(
					UINT32			VirtualKey,
					KEY_STATE_FLAGS	KeyEvent
					);

void	
KeybdDriverInitialize(
	PFN_KEYBD_EVENT_CALLBACK	pfnKeybdEventCallback
	);

typedef void	(*PFN_KEYBD_DRIVER_INITIALIZE)(
					PFN_KEYBD_EVENT_CALLBACK	pfnKeybdEventCallback
					);

typedef BOOL    (*PFN_KEYBD_EVENT_CALLBACK_EX)(
					UINT32			VirtualKey,
					UINT32			ScanCode,
					KEY_STATE_FLAGS	KeyEvent
					);

void	
KeybdDriverInitializeEx(
	PFN_KEYBD_EVENT_CALLBACK_EX	pfnKeybdEventCallbackEx
	);

typedef void	(*PFN_KEYBD_DRIVER_INITIALIZE_EX)(
					PFN_KEYBD_EVENT_CALLBACK_EX	pfnKeybdEventCallbackEx
					);




void
KeybdDriverPowerHandler(
	BOOL	bOff
	);

typedef void	(*PFN_KEYBD_DRIVER_POWER_HANDLER)(
					BOOL	bOff
					);

BOOL	
KeybdDriverGetInfo(
	INT		iKeybdId,
	INT		iIndex,
	LPVOID  lpOutput
	);

typedef BOOL	(*PFN_KEYBD_DRIVER_GET_INFO)(
					INT		iKeybdId,
					INT		iIndex,
					LPVOID  lpOutput
					);

BOOL
KeybdDriverSetMode(
	INT		iKeybdId,
	INT		iIndex,
	LPVOID	lpInput
	);

typedef  BOOL   (*PFN_KEYBD_DRIVER_SET_MODE)(
					INT		iKeybdId,
					INT		iIndex,
					LPVOID	lpInput
					);

BOOL
KeybdDriverInitStates(
	INT				iKeybdId,
	KEY_STATE       KeyState,
	void            *pKeybdDriverState
	);

typedef BOOL  (*PFN_KEYBD_DRIVER_INIT_STATES)(
					INT				iKeybdId,
					KEY_STATE       KeyState,
					void            *pKeybdDriverState
					);

UINT32
KeybdDriverVKeyToUnicode(
	UINT32          VKey,
	KEY_STATE_FLAGS KeyEventFlags,
	KEY_STATE       KeyState,
	void            *KeybdDriverState,
	UINT32          cBufferSize,
	UINT32          *pcCharacters,
	KEY_STATE_FLAGS *pShiftStateBuffer,
	UINT32          *pCharacterBuffer
	);

typedef UINT32  (*PFN_KEYBD_DRIVER_VKEY_TO_UNICODE)(
					UINT32          VKey,
					KEY_STATE_FLAGS KeyEventFlags,
					KEY_STATE       KeyState,
					void            *KeybdDriverState,
					UINT32          cBufferSize,
					UINT32          *pcCharacters,
					KEY_STATE_FLAGS *pShiftStateBuffer,
					UINT32          *pCharacterBuffer
					);


UINT32
KeybdDriverMapVirtualKey(
	UINT32	uCode,
	UINT32	uMapType
	);

typedef UINT32 (*PFN_KEYBD_DRIVER_MAP_VIRTUAL_KEY)(
					UINT32	uCode,
					UINT32	uMapType
					);



#define KEYBOARD_DRIVER_KEY            __TEXT("HARDWARE\\DEVICEMAP\\KEYBOARD")


int
LayoutMgrGetKeyboardType(
	int	nTypeFlag
	);

typedef int (*PFN_LAYOUT_MGR_GET_KEYBOARD_TYPE)(
					int	nTypeFlag
					);


BOOL
LayoutMgrGetKeyboardLayoutName(
	WCHAR	*pNameBuf
	);

typedef BOOL (*PFN_LAYOUT_MGR_GET_KEYBOARD_LAYOUT_NAME)(
					WCHAR	*pNameBuf
					);

HKL
LayoutMgrGetKeyboardLayout(
	DWORD idThread
	);

typedef HKL (*PFN_LAYOUT_MGR_GET_KEYBOARD_LAYOUT)(
					DWORD idThread
					);

UINT
LayoutMgrGetKeyboardLayoutList(
	int     nBuf,
	HKL FAR *lpList
	);

typedef UINT (*PFN_LAYOUT_MGR_GET_KEYBOARD_LAYOUT_LIST)(
                            	int     nBuf,
                            	HKL FAR *lpList
                            	);

HKL
LayoutMgrLoadKeyboardLayout(
	LPCWSTR pwszKLID,
	UINT    Flags
	);

typedef HKL (*PFN_LAYOUT_MGR_LOAD_KEYBOARD_LAYOUT)(
                                	LPCWSTR pwszKLID,
                                	UINT    Flags
                                	);

HKL
LayoutMgrActivateKeyboardLayout(
	HKL  hkl,
	UINT Flags
	);

typedef HKL (*PFN_LAYOUT_MGR_ACTIVATE_KEYBOARD_LAYOUT)(
                                	HKL  hkl,
                                	UINT Flags
                                	);

#ifdef __cplusplus
}
#endif


#endif

