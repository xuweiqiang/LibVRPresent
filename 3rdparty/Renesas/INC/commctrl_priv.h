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
/*****************************************************************************\
*                                                                             *
* commctrl_priv.h - - Interface for the Windows Common Controls               *
*                                                                             *
* Version 1.0                                                                 *
*                                                                             *
*                                                                             *
\*****************************************************************************/
#ifndef _INC_COMMCTRL_PRIV
#define _INC_COMMCTRL_PRIV

#ifdef __cplusplus
extern "C" {
#endif


#define LVM_AVOIDFILLRECTS      (LVM_FIRST + 80)

    
#define DSAS_SORTED             0x0001
#define DSAS_INSERTBEFORE       0x0002
#define DSAS_INSERTAFTER        0x0004
typedef struct _DSA FAR* HDSA;

/*
typedef int (CALLBACK *PFNDSACOMPARE)(LPVOID p1, LPVOID p2, LPARAM lParam);
int  DSA_Search(HDSA pdsa, void FAR* pFind, int iStart,
                       PFNDSACOMPARE pfnCompare, LPARAM lParam, UINT options);
HDSA DSA_Clone(HDSA pdsa, HDSA pdsaNew);
*/
BOOL IsCapEditAvailable();

 
// Bit flags used to load common control classes using the InitCommonControlsEx
// function. This list starts at the biggest possible bit flag that can fit in
// a DWORD (32 bit unsigned int), in order to avoid conflicts with commctrl.h.
//
// Note: If you add another ICC_ define, make sure you update 
//       ICC_ALL_CLASSES in public\shellw\oak\inc\pcommctr.h
//
#define ICC_BOXSELECT_CLASS 0x80000000 //Box Select
#define ICC_LABELEDIT_CLASS 0x10000000 //Label Edit

#define WC_BOXSELECT	TEXT("BOXSELECT")
#define WC_LABELEDIT    TEXT("LABELEDIT")

// New custom draw support

// Moved to commctrl.h 
// #define CDRF_NOVERTBAR        0x00000008
//
#define CDRF_NOELLIPSES       0x00000100
// Private Toolbar style
#define BTNS_NODDARROW        0x0080    /* if BTNS_DROPDOWN, don't draw arrow part */
typedef struct
{
    NMHDR hdr;
    int iItem;
    int idCommand;
    LPTSTR pszText;
    TCHAR szText[80];
} NMTBTOOLTIP, *PNMTBTOOLTIP;
#define TBN_GETTOOLTIP          (TBN_FIRST - 9)

// SHDrawText
//
//
#define SHDT_DRAWTEXT       0x00000001
#define SHDT_ELLIPSES       0x00000002
#define SHDT_CLIPPED        0x00000004
#define SHDT_SELECTED       0x00000008
#define SHDT_DESELECTED     0x00000010
#define SHDT_DEPRESSED      0x00000020
#define SHDT_EXTRAMARGIN    0x00000040
#define SHDT_TRANSPARENT    0x00000080
#define SHDT_SELECTNOFOCUS  0x00000100
#define SHDT_HOTSELECTED    0x00000200
#define SHDT_DTELLIPSIS     0x00000400
#define SHDT_RTLREADING     0x00000800
#define SHDT_NOOPAQUE       0x00001000
#define SHDT_RESTOREHDC     0x00002000
#define SHDT_LEFT           0x00004000
#define SHDT_VCENTER        0x00008000


void WINAPI SHDrawText(HDC hdc, LPCTSTR pszText, RECT FAR* prc,
        int fmt, UINT flags, int cyChar, int cxEllipses,
        COLORREF clrText, COLORREF clrTextBk);

/*
 *  Hidden styles for datepicker
 */

#define DTS_READONLY              0x0040 // datepicker does not allow editing of subedits

#define DTM_VALIDATEINPUT       (DTM_FIRST + 0x200)

/// label edit stuff
// Some defines
#define IDC_LESTATIC       100
#define IDC_LEBUDDY        101

//Label Edit specific styles used when creating the control
#define LES_EDITONLY              0x0001    //Control is always in edit state
#define LES_ALLOWNONE             0x0002    //Use with LES_DATEPICK dudes
#define LES_MULTILINE             0x0004    //Same as ES_MULTILINE
#define LES_WANTRETURN            0x0008    //Same as ES_WANTRETURN
#define LES_NOLABELONEMPTY        0x0010    //On empty edit, don't replace with
                                            //label
#define LES_NOHILITEDEFAULT       0x0020    //When control gets focus, and 
                                            //def. text is used, don't select
#define LES_AUTOHSCROLL           0x0040    //Same as ES_AUTOHSCROLL
#define LES_AUTOELLIPSES          0x0080    //Static control will get ellipses
#define LES_CAPEDITALLWORDS       0x0100    //All words in the CAPEDIT control will be upcased
#define LES_DROPDOWN              0x0200    //Dropdown is buddy control
#define LES_DROPDOWNLIST          0x0400    //Dropdown list is buddy control
#define LES_CAPEDIT               0x0800    //Use CAPEDIT control instead of EDIT control
#define LES_RIGHT                 0x1000    //Same as SS_RIGHT
#define LES_SORT                  0x2000    //Same as CBS_SORT
#define LES_DATEPICK              0x4000    //Datepick is buddy control
#define LES_AUTOVSCROLL           0x8000    //Same as ES_AUTOVSCROLL

#define LES_BORDERLESS            0x0004    // borderless combo box.  only valid if it's a combo labeledit

//Label Edit specific messages here
#define LEM_GETLABELLENGTH        (WM_USER + 500)
#define LEM_GETLABELTEXT          (WM_USER + 501)
#define LEM_SETLABELTEXT          (WM_USER + 502)
#define LEM_KEEPBORDER            (WM_USER + 503)
#define LEM_GETDEFAULTTEXTLENGTH  (WM_USER + 504)
#define LEM_GETDEFAULTTEXT        (WM_USER + 505)
#define LEM_SETDEFAULTTEXT        (WM_USER + 506)
#define LEM_SETEDITHOOK           (WM_USER + 507)
#define LEM_SETCOMBOHOOK          (WM_USER + 508)
#define LEM_LIMITTEXT             (WM_USER + 509)
#define LEM_SETDATEPICKHOOK       (WM_USER + 510)
#define LEM_SETNEWBUDDYSTYLE      (WM_USER + 511)
#define LEM_DESTROYBUDDY          (WM_USER + 512)
#define LEM_DOESBUDDYEXIST        (WM_USER + 513)
#define LEM_SETFOCUSNOSEL         (WM_USER + 514)
#define LEM_ESCSETSLABEL          (WM_USER + 515)
#define LEM_READONLY              (WM_USER + 516)  //This is a LES_ wanna be.  The edit control 
                                                   //will be created with ES_READONLY.  Since we
                                                   //are out of LES_ bits, we use a message.
#define LEM_GETBUDDY              (WM_USER + 517)

//Label Edit specific notifications here
#define LEN_ESCPRESSED            (NM_FIRST + 101)
#define LEN_BUDDYCREATE           (NM_FIRST + 102)
#define LEN_QUERYKEEPBUDDY        (NM_FIRST + 103)
#define LEN_BUDDYDESTROY          (NM_FIRST + 104)
#define LEN_QUERYBUDDYCREATE      (NM_FIRST + 105)  // Note, it is the user's responsibility to set focus
                                                    // if they process this message
//Label Edit notifcation flags  
#define LENF_KEEPBUDDY            0x00000002     //Return in response to 
                                                 //LEN_QUERYKEEPBUDDY to 
                                                 //keep buddy
#define LENF_DONTCREATEBUDDY      0x00000003     // Return in response to LEN_QUERYBUDDYCREATE
                                                 // to prevent creating of the buddy
#define LENF_PENCREATEEDIT        0x00000001     // pen/mouse used to cause creation of 
                                                 // edit buddy

typedef struct tagLEHDR
{
    NMHDR hdr;
    HWND  hBuddy;
    HWND  hFocus;
    DWORD dwFlags;
} LEHDR, *PLEHDR;

// Private message to set/remove the listbox/edit to/from the updown's buddy state
#define WM_SETBUDDY            (WM_USER+100)


//---------------------------------------------------------------------------
// COLOR functions...


#ifndef WINCOMMCTRLAPI
#if !defined(_COMCTL32_) && defined(_WIN32)
#define WINCOMMCTRLAPI DECLSPEC_IMPORT
#else
#define WINCOMMCTRLAPI
#endif
#endif // WINCOMMCTRLAPI

#ifndef NOSHGETSYSCOLOR
#ifdef _WIN32

#define SHSysColor_MemMap	TEXT("SHSysColor_MemMap")
#define SHSysColor_Mutex TEXT("SHSysColor_Mutex")
#define SHSysColor_RegKey TEXT("Software\\Microsoft\\Color")
#define SHSysColor_RegValue TEXT("SHColor")

WINCOMMCTRLAPI COLORREF SHGetSysColor(int nIndex);
WINCOMMCTRLAPI BOOL SHSetSysColors(int cElements, const INT *rgElements, const COLORREF *rgColorVals);

// If you add something here, make sure you add it to colortable.h
// Unfurtunately, we can't generate this list (or the equivalent
// of) from colortable.h because some people are mired in the 1970's
// and still writing in C.

//vishalr:
//IF YOU ADD SOMETHING HERE YOU MUST UPDATE 
// s_P2SColorMap in private\apps\tele\home\plugman.cpp
// else YOU WILL BREAK loading of ppc themes on tpc!!!
#define SHELL_COLOR_FIRST           0x10000000
#define COLOR_ALERT                 0x10000000
#define COLOR_ACCENTTEXT            0x10000001
#define COLOR_CALOFFHOURS           0x10000002
#define COLOR_CALONHOURS            0x10000003
#define COLOR_TODAYTEXT             0x10000004
#define COLOR_STARTICONBACKGROUND   0x10000005
#define COLOR_TODAYSEPARATOR        0x10000006
#define COLOR_TODAYMENU             0x10000007
#define COLOR_NAVBAR                0x10000008
#define COLOR_NAVBARTEXT            0x10000009
#define COLOR_NAVBARHILITE          0x1000000a
#define COLOR_NOTIFTRAY             0x1000000b
#define COLOR_NOTIFTRAYHILITE       0x1000000c
#define COLOR_BUBBLECAPTION         0x1000000d
#define COLOR_GRADIENTBUBBLECAPTION 0x1000000e
#define COLOR_INTGRADLEFT           0x1000000f
#define COLOR_INTGRADRIGHT          0x10000010
#define COLOR_HIGHGRADLEFT          0x10000011
#define COLOR_HIGHGRADRIGHT         0x10000012
#define COLOR_PHONEDIALERTEXT       0x10000013
#define COLOR_TRAYGRADLEFT          0x10000014
#define COLOR_TRAYGRADRIGHT         0x10000015
#define COLOR_TRAYTEXT              0x10000016
#define COLOR_TRAYBKGND             0x10000017
#define COLOR_DIALPADBKGND          0x10000018
#define COLOR_CRITGRADLEFT          0x10000019
#define COLOR_CRITGRADRIGHT         0x1000001a
#define COLOR_BUBBLETITLE           0x1000001b
#define COLOR_CRITBUBBLETITLE       0x1000001c
#define COLOR_ALERTTITLE            0x1000001d
#define COLOR_ALERTWINDOW           0x1000001e
#define COLOR_ALERTRULE             0x1000001f
#define COLOR_HOMETEXT              0x10000020
#define COLOR_HOMERULE              0x10000021
#define COLOR_HOMEHIGHLIGHT         0x10000022
#define COLOR_HOMEHIGHLIGHTTEXT     0x10000023
#define COLOR_GRADLEFT              0x10000024
#define COLOR_GRADRIGHT             0x10000025
#define COLOR_STARTMENU             0x10000026
#define COLOR_STARTMENUTEXT         0x10000027
#define COLOR_TAPANDHOLD            0x10000028
#define SHELL_COLOR_LAST            COLOR_TAPANDHOLD


#define SHELL_COLOR_COUNT   (SHELL_COLOR_LAST - SHELL_COLOR_FIRST + 1)

#define COLOR_CLEARCACHE     0x11000000

// Check to make sure colorlist.dat is in sync -- TEMPORARILY (?)
// removed.  This code creates a number of platform-level conflicts
// with the single build model

/*  #ifdef DEBUG */
/*  #ifdef __cplusplus */

/*  #ifndef CASSERT */
/*  #define CASSERT(e)  extern int dummary_array[(e)] */
/*  #endif */

/*  #define DEFINE_SHELL_COLOR(x, y, c, b) CASSERT((x) == SHELL_COLOR_FIRST + (y)); */
/*  #define SYS_COLOR_REFERENCE(x) */
/*  #include "colorlist.h" */
/*  #undef DEFINE_SHELL_COLOR */
/*  #undef SYS_COLOR_REFERENCE */

/*  #define DEFINE_SHELL_COLOR(x, y, c, b) + 1 */
/*  #define SYS_COLOR_REFERENCE(x) */
/*  const int _QWERTY_cColors = 0 */
/*  #include "colorlist.h" */
/*  ; */
/*  #undef DEFINE_SHELL_COLOR */
/*  #undef SYS_COLOR_REFERENCE */
/*  // If thie CASSERT fails, it is likely a new color entry wasn't added */
/*  // to both colorlist.h and commctrl_priv.h */
/*  CASSERT(SHELL_COLOR_COUNT == _QWERTY_cColors); */

/*  #endif // __cplusplus */
/*  #endif // DEBUG */

#endif // _WIN32
#endif // NOSHGETSYSCOLOR

//---------------------------------------------------------------------------
// METRICS functions...

#ifndef NOSHGETSYSTEMMETRICS
#ifdef _WIN32

#define SHSysMetric_MemMap	TEXT("SHSysMetric_MemMap")
#define SHSysMetric_Mutex TEXT("SHSysMetric_Mutex")
#define SHSysMetric_RegKey TEXT("Software\\Microsoft\\Metric")
#define SHSysMetric_RegValue TEXT("SHMetric")

WINCOMMCTRLAPI int SHGetSystemMetrics(int nIndex);
WINCOMMCTRLAPI BOOL SHSetSystemMetrics(int cElements, CONST INT *rgElements, CONST INT *rgVals);

#define SHELL_SM_FIRST              0x10000000

#define SM_GRADIENT_RIGHT_BIAS      0x10000000
#define SM_GRADIENT_LEFT_BIAS       0x10000001
#define SM_GRADIENT_DITHERED        0x10000002
#define SM_GRADIENT_INTERLACE_CY    0x10000003
#define SM_TRAYGRAD_LEFT_BIAS       0x10000004
#define SM_TRAYGRAD_RIGHT_BIAS      0x10000005

#define SHELL_SM_LAST               0x10000005
#define SHELL_SM_COUNT              (SHELL_SM_LAST - SHELL_SM_FIRST + 1)

#endif // _WIN32
#endif // _NOSHGETSYSCOLOR

//---------------------------------------------------------------------------
// GRADIENT functions


#ifndef NOGRADIENTS
#ifdef _WIN32

struct _SHGRADIENTINFO {
    
    DWORD    cbSize;        // Size of structure

    COLORREF clrLeft;       // Color for the left side
    COLORREF clrRight;      // Color for the right side
    int      cx;            // Total width to draw
    int      cy;            // Total height to draw
    int      ipctStart;     // percent from the left to start the gradient
    int      ipctEnd;       // percent form the left to end the gradient.
    
    int      yOffset;       // Offset from the top of the bitmap to render
    int      xOffset;       // Offset from the left of the bitmap to render

    HDC      hdcDest;       // DC to write to.

    int      cColors;       // Number of colors to use in the gradient
                            // leave cColors zero for automatic generation.

    BOOL     fNoDither;     // Set true to force no dithering.
};

typedef struct _SHGRADIENTINFO SHGRADIENTINFO;

WINCOMMCTRLAPI HRESULT SHDrawGradient(SHGRADIENTINFO* pshgi);


struct _SHDEFAULTGRADIENTINFO {

    DWORD    cbSize;        // Size of structure

    int     cx;             // Width to draw
    int     cy;             // Height to draw (MUST BE 4 * SHGetSystemMetrics(SM_GRADIENT_INTERLACE_CY) FOR NOW)

    int     xOffset;        // Offset from left to draw.
    int     yOffset;        // Offset from top to draw.

    HDC     hdcDest;        // DC to paint the gradient to.
};

typedef struct _SHDEFAULTGRADIENTINFO SHDEFAULTGRADIENTINFO;

WINCOMMCTRLAPI HRESULT SHCreateDefaultGradient(SHDEFAULTGRADIENTINFO* pshdgi);

#endif // _WIN32
#endif // _NOGRADIENTS


#ifdef __cplusplus
}
#endif
#endif  // _INC_COMMCTRL_PRIV
