#pragma once
#include "Resource.h"

// CDisplayDlg dialog

class CDisplayDlg : public CDialog
{
	DECLARE_DYNAMIC(CDisplayDlg)

public:
	CDisplayDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DISPLAY };
	BOOL m_bCKey;
	DWORD m_dwCKey;
	HRESULT SetCKey(BOOL bCKey, DWORD dwCKey)
	{
		m_bCKey = bCKey;
		m_dwCKey = dwCKey;
		return S_OK;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
public:
	afx_msg void OnClose();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
