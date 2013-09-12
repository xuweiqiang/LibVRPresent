// vncviewerDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include <rfb/rfbclient.h>

// CVncViewerDlg dialog
class CVncViewerDlg : public CDialog
{
// Construction
public:
	CVncViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VNCVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();
	CEdit m_editServerIP;
	
public:
	CStatic m_staticDisp;
	afx_msg void OnPaint();
};
