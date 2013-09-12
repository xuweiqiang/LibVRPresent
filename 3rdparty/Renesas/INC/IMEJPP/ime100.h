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

Module:

	Microsoft Pocket IME (pIME) Ver 2.0

File Name:

	ime100.h

Abstract:

	Header for Pocet IME.

Notes: 


--*/

#ifndef __IME100_H__
#define __IME100_H__
#ifdef __cplusplus
extern "C" {			/* Assume C declarations for C++ */
#endif /* __cplusplus */


#define	IMPLEMENT_LEARN_SC	1


#ifdef UNDER_CE
#include <imm.h>
#else //UNDER_CE
#include <_imm.h>
#endif//UNDER_CE


#define Unref(a)		UNREFERENCED_PARAMETER(a)
#define DbgAssert(a)

//defines
#define MAXCOMPSIZE             128
#ifdef UNDER_CE
#define cCandPageSizeMax		5
#else //UNDER_CE
#define cCandPageSizeMax		9
#endif//UNDER_CE
#define cbCompMax               128			//max characters for composition string
#define cCompWndMax				128
#define cbErrorMax				64
#define cchBufMax				256

#define cClsiInc				8
#define cwchStrInc				16
#define cbCandInc				512
#define cbCompInc				512

#define cStatusButtons			5

#define szUICLASSNAME			TEXT("IME100UI")
#ifdef  UNDER_CE
#define szIME100Retail			TEXT("imejpp.dll")
#else   // UNDER_CE
#define szIME100Retail			TEXT("ime100.ime")
#endif  // UNDER_CE
#define szIME100Debug			TEXT("debug100.ime")

/* Init or Clear Structure Flag */
#define CLR_RESULT 1
#define CLR_UNDET  2
#define CLR_RESULT_AND_UNDET  3

/* WndExtra of child UI windows */
#define FIGWL_MOUSE         4
#define FIGWL_SVRWND        8
#define FIGWL_FONT         12
#define FIGWL_COMPSTARTSTR 16
#define FIGWL_COMPSTARTNUM 20
#define FIGWL_STATUSBMP    24
#define FIGWL_CLOSEBMP     28
#define FIGWL_PUSHSTATUS   32
#define FIGWL_CHILDWND     36
#define UIEXTRASIZE        40

/* The flags of FIGWL_MOUSE */
#define FIM_CAPUTURED 0x01
#define FIM_MOVED     0x02

/* Change Mode index */
#define TO_CMODE_ALPHANUMERIC  0x0001
#define TO_CMODE_KATAKANA      0x0002
#define TO_CMODE_HIRAGANA      0x0003
#define TO_CMODE_FULLSHAPE     0x0008
#define TO_CMODE_ROMAN         0x0010
#define TO_CMODE_CHARCODE      0x0020
#define TO_CMODE_TOOLBAR       0x0100
#define TO_CMODE_DIRECT        0x0200

/* ID of guideline table */
#define MYGL_NODICTIONARY     0
#define MYGL_TYPINGERROR      1

/* KANA KEY STACK MODE of Function SetKanaKeyOnOff */
#define KANAKEY_INIT          0
#define KANAKEY_IMEONOFF      1
#define KANAKEY_CLOSE		  2
#define KANAKEY_OPEN		  3

/* special messages */
#define WM_UI_UPDATE      (WM_USER+500)
#define WM_UI_HIDE        (WM_USER+501)

#define WM_UI_STATEMOVE   (WM_USER+601)
#define WM_UI_DEFCOMPMOVE (WM_USER+602)
#define WM_UI_CANDMOVE    (WM_USER+603)
#define WM_UI_GUIDEMOVE   (WM_USER+604)

/* special style */
#define WS_COMPDEFAULT (WS_DISABLED | WS_POPUP)
#define WS_COMPNODEFAULT (WS_DISABLED | WS_POPUP)

#define IsCTLPushed(x)          ((x)[VK_CONTROL] & 0x80)
#define IsSHFTPushed(x)         ((x)[VK_SHIFT] & 0x80)
#define IsALTPushed(x)          ((x)[VK_MENU] & 0x80)

#define GCS_COMPALL ( GCS_COMPSTR | GCS_COMPATTR | GCS_COMPREADSTR | GCS_COMPREADATTR | GCS_COMPCLAUSE | GCS_COMPREADCLAUSE)
#define GCS_RESULTALL ( GCS_RESULTSTR | GCS_RESULTREADSTR | GCS_RESULTCLAUSE | GCS_RESULTREADCLAUSE)

#define dwMagicNum			(0xDADADADA)

#define INPUT_OK		0
#define INPUT_ERR		1
#define INPUT_FULL		2
#define INPUT_NONE		3
#define INPUT_NOMEM		4
#define INPUT_INVALIDDIC	5


#define cbInitCompStr		(sizeof(COMPOSITIONSTRING) + cbCompInc)

/* define GET LP for COMPOSITIONSTRING members. */
#define GETLPCOMPREADATTR(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwCompReadAttrOffset)
#define GETLPCOMPREADCLAUSE(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwCompReadClauseOffset)
#define GETLPCOMPREADSTR(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwCompReadStrOffset)
#define GETLPCOMPATTR(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwCompAttrOffset)
#define GETLPCOMPCLAUSE(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwCompClauseOffset)
#define GETLPCOMPSTR(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwCompStrOffset)
#define GETLPRESULTREADSTR(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwResultReadStrOffset)
#define GETLPRESULTREADCLAUSE(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwResultReadClauseOffset)
#define GETLPRESULTCLAUSE(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwResultClauseOffset)
#define GETLPRESULTSTR(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwResultStrOffset)
#define GETLPPRIVATE(lpcs) ((LPSTR)(lpcs) + (lpcs)->dwPrivateOffset)

#define		WORDID_SERVICE_CANDIDATE	0xfffffffe
#define		WORDID_INVALID				0xffffffff

//My Guideline
typedef struct _MYGUIDELINE
{
    DWORD dwLevel;
    DWORD dwIndex;
    DWORD dwStrID;
} MYGUIDELINE, *LPMYGUIDELINE;

typedef struct _MYCAND
{
    CANDIDATEINFO  ci;
    CANDIDATELIST  cl;
    DWORD          offset[1];
} MYCAND, *LPMYCAND;

#define cbMYCAND 		(sizeof(MYCAND) - sizeof(DWORD))

typedef struct _UICHILD
{
    HWND    hWnd;
    BOOL    bShow;
    POINT   pt;
} UICHILD, *LPUICHILD;

typedef struct _UICHILD2
{
    HWND    hWnd;
    BOOL    bShow;
    RECT    rc;
} UICHILD2, *LPUICHILD2;

typedef struct _UIEXTRA
{
    HIMC     hIMC;
    UICHILD  uiStatus;
    UICHILD  uiCand;
    DWORD    dwCompStyle;
    HFONT    hFont;
    BOOL     bVertical;
    UICHILD  uiDefComp;
	int		 cuiComp;
    UICHILD2 uiComp[cCompWndMax];
    UICHILD  uiGuide;
    UICHILD  hwndScrollBar;
	HWND	 hwndCaret;
	HWND	 hwndUI;
	int			dxTarget;			//position of target clause
	int			dyTarget;			//position of target clause
} UIEXTRA, *LPUIEXTRA;

typedef struct _GENEMSG
{
    UINT 		msg;
    WPARAM 		wParam;
    LPARAM 		lParam;
} GENEMSG, *LPGENEMSG;

typedef struct _KEYTBL{		////Key table for imeui
	BYTE	bVkCode;		//VK
	BYTE	bShiftFlg;		//SHIFT
	BYTE	bCtrlFlg;		//CTRL
	WORD	wSendProc;		//SEND PROC
	WORD	wCtrlCode[7];	//JCONV
} KEYTBL, *LPKEYTBL;

//externs
extern TCHAR szUIClassName[];
extern TCHAR szCandClassName[];
extern TCHAR szCaretClassName[];
extern TCHAR szCompStrClassName[];
extern TCHAR szGuideClassName[];
extern HINSTANCE	vhInstance;
extern UINT uNumTransKey;
extern LPDWORD	lpdwCurTransKey;
extern WCHAR	vrgwch[MAXCOMPSIZE * 2];
extern TCHAR    vrgchBuf[MAXCOMPSIZE * 2];


//function prototypes

#define MyImmLockIMCC			ImmLockIMCC
#define MyImmUnlockIMCC			ImmUnlockIMCC
#define MyImmLockIMC			ImmLockIMC
#define MyImmUnlockIMC			ImmUnlockIMC

//non-ui
BOOL IsCompStr(HIMC hIMC, BOOL fConverted);
BOOL IsCandidate(LPINPUTCONTEXT lpIMC);
BOOL ConvKanji(HIMC hIMC);
BOOL GenerateMessage(HIMC hIMC, LPDWORD lpdwTransKey,LPGENEMSG lpGeneMsg);
BOOL DicKeydownHandler(HIMC hIMC, WORD wParam, LONG lParam, LPBYTE lpbKeyState);
BOOL MakeGuideLine(HIMC hIMC, DWORD dwID);
void ChangeMode(HIMC hIMC, DWORD dwToMode);


//ui 
BOOL MyIsIMEMessage(UINT message);
void DrawUIBorder(LPRECT lprc);
void DragUI(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void FillByte(void *pb, int cb, BYTE b);
LRESULT CALLBACK IME100WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK UICaretWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//candidate
void MoveCandWindow(HWND hUIWnd, LPINPUTCONTEXT lpIMC, LPUIEXTRA lpUIExtra, BOOL fForceComp);
void HideCandWindow( LPUIEXTRA lpUIExtra);
void ResizeCandWindow( LPUIEXTRA lpUIExtra,LPINPUTCONTEXT lpIMC );
void CreateCandWindow( HWND hUIWnd,LPUIEXTRA lpUIExtra, LPINPUTCONTEXT lpIMC );
LRESULT CALLBACK CandWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//composition
void HideCompWindow(LPUIEXTRA lpUIExtra);
void CreateCompWindow(HWND hUIWnd, LPUIEXTRA lpUIExtra,LPINPUTCONTEXT lpIMC);
void MoveCompWindow(LPUIEXTRA lpUIExtra,LPINPUTCONTEXT lpIMC);
void SetFontCompWindow(LPUIEXTRA lpUIExtra);
LRESULT CALLBACK CompStrWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CALLBACK CaretTimerProc(HWND hwnd, UINT uMsg, UINT id, DWORD dwTime);
BOOL FSetCompStr(HIMC hIMC, LPCTSTR lpWord, DWORD dwWord);
BOOL FSetCompClause(HIMC hIMC, DWORD *lpWord, DWORD dwWord);
BOOL FSetCompAttr(HIMC hIMC, BYTE *lpByte, DWORD dwWord);
void SetConversionMode(HIMC hIMC, DWORD dwConversionMode);

//toolbar
void UpdateStatusWindow(LPUIEXTRA lpUIExtra);
void UpdateGuideWindow(HWND hWnd, LPUIEXTRA lpUIExtra, LPINPUTCONTEXT lpIMC);
LRESULT CALLBACK GuideWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK StatusWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void OpenStatusWindow(HWND hwnd, LPINPUTCONTEXT lpIMC, LPUIEXTRA lpUIExtra);
void SetKanaKey(HIMC hIMC);
void HideStatusWindow( LPUIEXTRA lpUIExtra);
void SetKanaKeyOnOff(HIMC hIMC,int mode);
void HideGuideWindow( LPUIEXTRA lpUIExtra);

//property
void InitPropertyData(HWND hwnd);
BOOL WINAPI Ime100Configure(HKL hKL, HWND hWnd, DWORD dwMode, LPVOID lpData);
//doctool(word register)
BOOL WINAPI RegWordDialog(HKL hKL, HWND hWnd, DWORD dwMode, LPVOID lpData);

#ifdef __cplusplus
} /* end of 'extern "C" {' */
#endif	// __cplusplus

#endif //__IME100_H__
