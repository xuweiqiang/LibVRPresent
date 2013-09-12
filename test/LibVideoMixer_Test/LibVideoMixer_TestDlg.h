// LibVideoMixer_TestDlg.h : header file
//

#pragma once

#include "GfxPresent.h"
//#include "VideoMixer.h"
#include "afxwin.h"

// CLibVideoMixer_TestDlg dialog
class CLibVideoMixer_TestDlg : public CDialog
{
// Construction
public:
	CLibVideoMixer_TestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LIBVIDEOMIXER_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnBnClickedOpenFile();
	void OnBnClickedCreateMixer();

	HRESULT LoadLib();

	IMcVideoPresenter* m_pPresent;
	ICivmVideoMixer* m_pVideoMixer;
	ICivmVideoContext* m_pCurrVideoContext;
	//IMcVideoPresenterStream* m_pCurrStream;
	HMODULE m_hVideoPresenter;
	bool m_bRuning;
	VR_ANIMATION_PARAMS m_animation_parm;

	static DWORD WINAPI ThreadProc( LPVOID lpParameter);
	static DWORD WINAPI ThreadProcStream( LPVOID lpParameter);

	DECLARE_MESSAGE_MAP()
public:
	CStatic static_text;
	afx_msg void OnBnClickedInitVR();
	HRESULT InitVM();
	HRESULT InitVR();


	afx_msg void OnBnClickedCreateStream();
	afx_msg void OnBnClickedCreateMixerContext();
	afx_msg void OnBnClickedConnectMixer();
	afx_msg void OnBnClickedConnectContext();
	afx_msg void OnBnClickedUninitVR();
	afx_msg void OnBnClickedDestroyVM();

	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonDestroystream();
	afx_msg void OnBnClickedButtonDestroyContext();
	afx_msg void OnCancel( );
	afx_msg void OnBnClickedButtonCreateanimation( );
	afx_msg void OnBnClickedButtonStartAnimation( );
	afx_msg void OnBnClickedButtonStopAnimation( );
	afx_msg void OnBnClickedButtonDestroyAnimation( );

};
