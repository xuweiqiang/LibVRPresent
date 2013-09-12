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
#ifndef __TABVIEW_HPP__
#define __TABVIEW_HPP__
/*


Abstract:

This header file defines the interface to the TabView_t class
which implements the customizable tab user interface.

*/

#include <windows.h>

class Tab_t;
typedef class Tab_t* PTC;

typedef struct { // ti
    RECT rc;        // for hit testing and drawing
    int iImage;     // image index
    int xLabel;     // position of the text for drawing (relative to rc)
    int yLabel;     // (relative to rc)
    int cxLabel;    // width of the label.  this is needed if we're drawing in vertical mode

    int xImage;     // Position of the icon for drawing (relative to rc)
    int yImage;
    int iRow;           // what row is it in?
    LPTSTR pszText;

    DWORD dwState;

    UINT etoRtlReading;

    union {
        LPARAM lParam;
        BYTE   abExtra[1];
    };
} TABITEM, FAR *LPTABITEM;

class TabView_t
{	

private:
    TabView_t();                            //    NOT IMPLEMENTED default constructor
    TabView_t(const TabView_t&);            //    NOT IMPLEMENTED default copy constructor
    TabView_t& operator=(const TabView_t&);    //    NOT IMPLEMENTED default assignment constructor
    ~TabView_t();                            //    NOT IMPLEMENTED default destructor

public:

    static void Draw(
        HWND    hwnd,
        HWND    hwndParent,
        HDC        hdc,
        RECT*    prcClient,
        DWORD    style,
        DWORD    dwStyleEx,
        UINT     flags,         // TCF_ values (internal state bits)
        int        tmHeight,
        int        cItems,
        HIMAGELIST himl,
        HDPA    hdpa,
        int        cbExtra,
        int        iSel,
        int        iNewSel,
        int        cxTabs,
        int        iFirstVisible,
        int        iLastVisible,
        int        iLastTopRow
        );
};

// tab control flag values
#define TCF_FOCUSED     0x0001
#define TCF_MOUSEDOWN   0x0002
#define TCF_DRAWSUNKEN  0x0004
#define TCF_REDRAW      0x0010  /* Value from WM_SETREDRAW message */
#define TCF_BUTTONS     0x0020  /* draw using buttons instead of tabs */

#define TCF_FONTSET     0x0040  /* if this is set, they set the font */
#define TCF_FONTCREATED 0x0080


#endif /* __TABVIEW_HPP__ */