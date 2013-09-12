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
class Tab_t {
public:
    CONTROLINFO m_ci;

    HWND m_hwndArrows;    // Hwnd Arrows.
    HDPA m_hdpa;          // item array structure
    UINT m_flags;         // TCF_ values (internal state bits)
    int  m_cbExtra;       // extra bytes allocated for each item
    DWORD m_dwStyleEx;    // set by TCM_SETEXTENDEDSTYLE
    HFONT m_hfontLabel;   // font to use for labels
    int m_iSel;           // index of currently-focused item
    int m_iNewSel;        // index of next potential selection

    int m_cxItem;         // width of all tabs
    int m_cxMinTab;       // width of minimum tab
    int m_cyTabs;         // height of a row of tabs
    int m_cxTabs;     // The right hand edge where tabs can be painted.

    int m_cxyArrows;      // width and height to draw arrows
    int m_iFirstVisible;  // the index of the first visible item.
                        // wont fit and we need to scroll.
    int m_iLastVisible;   // Which one was the last one we displayed?

    int m_cxPad;           // Padding space between edges and text/image
    int m_cyPad;           // should be a multiple of c?Edge

    int m_iTabWidth;      // size of each tab in fixed width mode
    int m_iTabHeight;     // settable size of each tab
    int m_iLastRow;       // number of the last row.
    int m_iLastTopRow;    // the number of the last row that's on top (SCROLLOPPOSITE mode)

    int m_cyText;         // where to put the text vertically
    int m_cyIcon;         // where to put the icon vertically

    HIMAGELIST m_himl;    // images,
#ifndef _WIN32_CE // WinCE doesn't support tooltips for this control.
    HWND m_hwndToolTips;
#endif // !_WIN32_CE
#if defined(FE_IME) || !defined(WINNT)
    HIMC m_hPrevImc;      // previous input context handle
#endif

#ifndef _WIN32_CE // WinCE doesn't support TCS_EX_REGISTERDROP
    HDRAGPROXY m_hDragProxy;
    DWORD m_dwDragDelay;  // delay for auto page-change during drag
    int m_iDragTab;       // last tab dragged over
#endif // !_WIN32_CE

    int m_tmHeight;    // text metric height
    BOOL m_fMinTabSet:1;  // have they set the minimum tab width
    BOOL m_fTrackSet:1;

#ifndef _WIN32_CE // WinCE doesn't support hot tracking and hover selection
    int m_iHot;
#endif // !_WIN32_CE

    void Paint(HDC hdcIn);
    int OnHitTest(int x, int y, UINT FAR *lpuFlags);
    void OnSetRedraw(BOOL fRedraw);
    void CalcPaintMetrics(HDC hdc);
    void OnSetFont(HFONT hfont, BOOL fRedraw);
    BOOL RedrawAll(UINT uFlags);
    void ImageList_Draw(int iImage, HDC hdc, int x, int y, UINT uFlags);
    void DrawFocusRect(HDC hdc, LPRECT lprc);
    void PatBlt(HDC hdc, int x1, int y1, int w, int h, UINT rop);
    void RightJustify();
    BOOL OnGetItemRect(int iItem, LPRECT lprc);
    void OnAdjustRect(BOOL fGrow, LPRECT prc);
    void DoCorners(HDC hdc, LPRECT prc, BOOL fBottom);

    int ChangeSel(int iNewSel, BOOL bSendNotify, BOOL bUpdateCursorPos);
    void PutzRowToBottom(int iRowMoving);
    void InvertRows();
    void CalcTabHeight(HDC hdc);
    void Scroll(int dx, int iNewFirstIndex);
    void DeselectAll(BOOL fExcludeFocus);
    void InvalidateItem(int iItem, BOOL bErase);
    void SmoothScrollWindow(int dx, int dy, LPRECT lprcSrc, LPRECT lprcClip,
                            HRGN hrgn, LPRECT lprcUpdate, UINT fuScroll, UINT uScrollMin);
    void InvalidateTabRect(LPRECT prc, BOOL b);
    void FillLastRow();
    BOOL OnSetItem(int iItem, const TC_ITEM FAR* ptci);
    BOOL OnDeleteAllItems();
    BOOL BorrowOne(int iCurLast, int iPrevLast, int iBorrow);
    void UpdateArrows(BOOL fSizeChanged);
    BOOL FreeItem(TABITEM FAR* pitem);
    int OnInsertItem(int iItem, const TC_ITEM FAR* ptci);
    void OnHScroll(HWND hwndCtl, UINT code, int pos);
    void RefreshArrows(HDC hdc);
    TABITEM* CreateItem(const TC_ITEM FAR* ptci);
    void SetCurFocus(int iStart);
    void OnKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags);
    int FindTab(int iStart, UINT vk);
    void OnLButtonDown(int x, int y, UINT keyFlags);
    BOOL OnGetItem(int iItem, TC_ITEM FAR* ptci);
    BOOL OnSetItemExtra(int cbExtra);
    void OnMouseMove(WPARAM fwKeys, int x, int y);
    void OnButtonUp(int x, int y, BOOL fNotify);
    BOOL OnDeleteItem(int i);
    void OnRemoveImage(int iItem);
    void TabSize();
    void StyleChanged(UINT gwl,  LPSTYLESTRUCT pinfo);
    DWORD ExtendedStyleChange(DWORD dwNewStyle, DWORD dwExMask);
    BOOL ThunkItemWtoA (TC_ITEMW * pItemW, TC_ITEMA * pItemA);
    TC_ITEMW * ThunkItemAtoW (TC_ITEMA * pItemA);
    BOOL OnCreate();
    void OnDestroy();

    void DVFlipTabRect(LPRECT prc);
    void DFlipRect(LPRECT prc);
    void VDFlipTabRect(LPRECT prc);
    void VFlipTabRect(LPRECT prc);
    void GetTabClientRect(LPRECT prc);
};

typedef class Tab_t* PTC;

#ifndef TCS_MULTISELECT
#define TCS_MULTISELECT  0x0004
#endif

#define HASIMAGE(ptc, pitem) (m_himl && pitem->iImage != -1)

#define ID_ARROWS       1

#define TAB_DRAGDELAY   500

// Some helper macros for checking some of the flags...
#define Tab_RedrawEnabled()              (m_flags & TCF_REDRAW)
#define Tab_Count()                      DPA_GetPtrCount(m_hdpa)
#define Tab_GetItemPtr(i)                  ((LPTABITEM)DPA_GetPtr(m_hdpa, (i)))
#define Tab_FastGetItemPtr(i)              ((LPTABITEM)DPA_FastGetPtr(m_hdpa, (i)))
#define Tab_IsItemOnBottom(pitem)          ((BOOL)pitem->iRow > m_iLastTopRow)
#define Tab_DrawSunken()                 ((BOOL)m_flags & TCF_DRAWSUNKEN)

#define Tab_DrawButtons()            ((BOOL)(m_ci.style & TCS_BUTTONS))
#define Tab_MultiLine()              ((BOOL)(m_ci.style & TCS_MULTILINE))
#define Tab_RaggedRight()            ((BOOL)(m_ci.style & TCS_RAGGEDRIGHT))
#define Tab_FixedWidth()             ((BOOL)(m_ci.style & TCS_FIXEDWIDTH))
#define Tab_Vertical()               ((BOOL)(m_ci.style & TCS_VERTICAL))
#define Tab_Bottom()                 ((BOOL)(m_ci.style & TCS_BOTTOM))
#define Tab_ScrollOpposite()            ((BOOL)(m_ci.style & TCS_SCROLLOPPOSITE))
#define Tab_ForceLabelLeft()         ((BOOL)(m_ci.style & TCS_FORCELABELLEFT))
#define Tab_ForceIconLeft()          ((BOOL)(m_ci.style & TCS_FORCEICONLEFT))
#define Tab_FocusOnButtonDown()      ((BOOL)(m_ci.style & TCS_FOCUSONBUTTONDOWN))
#define Tab_OwnerDraw()              ((BOOL)(m_ci.style & TCS_OWNERDRAWFIXED))
#define Tab_FocusNever()             ((BOOL)(m_ci.style & TCS_FOCUSNEVER))
#define Tab_HotTrack()                 ((BOOL)(m_ci.style & TCS_HOTTRACK))
#define Tab_MultiSelect()            ((BOOL)(m_ci.style & TCS_MULTISELECT))
#define Tab_FlatButtons()            ((BOOL)(m_ci.style & TCS_FLATBUTTONS))

#define Tab_FlatSeparators()         ((BOOL)(m_dwStyleEx & TCS_EX_FLATSEPARATORS))
#define Tab_RegisterDropTarget(ptd)     ((BOOL)(m_dwStyleEx & TCS_EX_REGISTERDROP))

#ifdef __cplusplus
extern "C"
{
#endif

LRESULT CALLBACK Tab_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL FAR PASCAL Tab_Init(HINSTANCE hinst);
#ifdef UNICODE
//
// ANSI <=> UNICODE thunks
//

BOOL FreeItemW (TC_ITEMW *pItemW);
BOOL FreeItemA (TC_ITEMA *pItemA);
#endif

#ifdef __cplusplus
}
#endif
