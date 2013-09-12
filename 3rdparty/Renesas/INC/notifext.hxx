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

    notifext.hxx

Abstract:

    This module contains the function prototypes and constant, type and
    structure definitions for the implementation of user notification
    subsystem.

Notes: 


--*/
#if ! defined (__notifextHXX__)
#define __notifextHXX__		1

#if defined (DEBUG) || defined (_DEBUG)

#define ZONE_INIT               DEBUGZONE(0)        
#define ZONE_TRACE              DEBUGZONE(1)        

#define ZONE_VERBOSE            DEBUGZONE(9)
#define ZONE_DATABASE           DEBUGZONE(10)
#define ZONE_UI                 DEBUGZONE(11)
#define ZONE_NOTIFY             DEBUGZONE(12)
#define ZONE_MISC               DEBUGZONE(13)
#define ZONE_WARNING            DEBUGZONE(14)       
#define ZONE_ERROR              DEBUGZONE(15)       


#endif    // DEBUG

#define SMALL_MESSAGE				128
#define SMALL_HEADER				64
#define BIG_MESSAGE					512

#define SHELLAPI_BOOTING	0
#define SHELLAPI_READY		1
#define SHELLAPI_NONE		2

#define SHELLAPI_WAIT		4

#define M_WORD				'SVS '

#define WM_USER_ANNUNCIATOR 	(WM_USER + 1)
#define WM_USER_PACKET_DISPOSE	(WM_USER + 2)
#define WM_USER_PACKET_NEW		(WM_USER + 3)
#define WM_USER_ALARM			(WM_USER + 4)

	//
	// Must be 2**n or change mask below... and not less than sizeof(TCHAR)
	//
#define MAX_EXPANSION_ALIGNMENT		8
#define EXPANSION_ALIGNMENT_MASK	(~(MAX_EXPANSION_ALIGNMENT - 1))

union PersistentPacket {
	struct {
		DWORD			dwActionFlags;
		DWORD			dwPacketFlags;
		FILETIME		ftStart;
		FILETIME		ftStop;

		union { 
			unsigned int	uiAppNameOffset;
			WCHAR			*lpszAppName;
		};

		union {
			unsigned int	uiCmdLineOffset;
			WCHAR			*lpszCmdLine;
		};

		union {
			unsigned int	uiDialogTitleOffset;
			WCHAR			*lpszDialogTitle;
		};

		union {
			unsigned int	uiDialogTextOffset;
			WCHAR			*lpszDialogText;
		};

		union {
			unsigned int	uiSoundOffset;
			WCHAR			*lpszSound;
		};

		union {
			unsigned int	uiExpansionOffset;
			void			*pvExpansionData;
		};

		unsigned char header_end[1];
	};
	unsigned char packet_start[1];
};

#define PACKET_UI			0x00000001
#define PACKET_MMMASK		0x000000f0
#define PACKET_VIBR			0x00000010
#define PACKET_SOUND		0x00000020
#define PACKET_LED			0x00000040

struct NotifyPacket {
	NotifyPacket	*pNext;
	NotifyPacket	*pPrev;

	unsigned int	uiPacketType;

	CEOID			oidRecord;

	HICON			hIcon;
	HICON			hBigIcon;

	int				fHasIcon;
	HWND			hDlg;

	void			*pExtraUIData;

	PersistentPacket	sPacket;
};


// Holds per-dialog info for the user settings dialog.
// Pointed to by the first spare dword of the dialog's extra bytes.

typedef struct tagSettingsDialogData{
    // Ptr to the initial (and returned) settings structure.
    PCE_USER_NOTIFICATION UserNotification;
    TCHAR *MappedSoundBuffer;   // Ptr to mapped version of sound file.
	void  *pvExtension;			// Ptr to UI extension buffer
    HWND hParentWindow;         // Handle of the main dialog window
    HWND hLedWindow;            // Handle of the LED checkbox window
    HWND hVibrateWindow;        // Handle of the vibrate checkbox window
    HWND hDialogWindow;         // Handle of the dialog checkbox window
    HWND hSoundWindow;          // Handle of the sound checkbox window
    HWND hComboWindow;          // Handle of the sound combo window
    HWND hRepeatWindow;         // Handle of the repeat sound checkbox window
    HICON hIcon;                // Handle to icon displayed on dialog.
    BOOL SaveData;              // TRUE if user hits OK (not cancel)
} SettingsDialogData;

int  CeNotifyPublic_Initialize             (void);
int  CeNotifyPublic_InitializeUI           (HINSTANCE, HWND);
void CeNotifyPublic_Uninitialize           (void);
int  CeNotifyPublic_FilterEvent            (DWORD dwEvent);
int  CeNotifyPublic_FilterUserNotification (CE_USER_NOTIFICATION *pceun);
void CeNotifyPublic_DisposeOfPacket        (NotifyPacket *pPacket);
void CeNotifyPublic_NewPacket              (CEOID oidRecord);
int  CeNotifyPublic_PresentSettingsBox     (HWND hWnd, CE_USER_NOTIFICATION *pceun, TCHAR *lpszSoundBuffer, void *pExpansion);
void CeNotifyPublic_UserAlarmSignalled     (void);

LRESULT CALLBACK CeNotifyPublic_UIWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void CeNotifyPrivate_Lock   (void);
void CeNotifyPrivate_Unlock (void);
NotifyPacket *CeNotifyPrivate_TranslateOID (CEOID);
int CeNotifyPrivate_Reschedule             (CEOID, int iDelaySeconds);
void CeNotifyPrivate_DeleteNotification    (CEOID);
void CeNotifyPrivate_SetUserAlarm		   (FILETIME *ftWhen);
void CeNotifyPrivate_ClearUserAlarm		   (void);
void CeNotifyPrivate_FilterType			   (unsigned int uiFlag);

#endif
