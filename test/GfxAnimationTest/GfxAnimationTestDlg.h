// GfxAnimationTestDlg.h : header file
//

#pragma once
#include "GfxPresent.h"

// CGfxAnimationTestDlg dialog
class CGfxAnimationTestDlg : public CDialog
{
// Construction
public:
	CGfxAnimationTestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CGfxAnimationTestDlg();


// Dialog Data
	enum { IDD = IDD_GFXANIMATIONTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInit();
	BOOL m_lineEffect;
	BOOL m_rotateEffect;
	BOOL m_shakeEffect;
	BOOL m_AlphaEffect;
	BOOL m_fadeOurEffect;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedCheck1();

	HRESULT InitPresenter();
	HRESULT InitVideoPresenterV2();
	static  DWORD WINAPI ThreadProc( LPVOID lpParameter);
	HRESULT DestroyPresenter();

	IMcVideoPresenterAdv* m_pPresenter;
	IMcVideoPresenterAnimation* m_pAnimation;
	IMcVideoPresenterAnimation* m_pAnimation2;
	bool m_bOpen;
	bool m_bInit;
	HWND m_hwnd;
	CDialog* m_pDiag;
	IMcVideoPresenterStream* m_pStream;
	IMcVideoPresenterStream* m_pStream2;
	int m_srcWidth;
	int m_srcHeight;
	int m_canvasWidth;
	int m_canvasHeight;
	
	HANDLE m_hThread;
	bool m_threadRuning;
	
};
