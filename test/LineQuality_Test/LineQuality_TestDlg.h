// LibVrPresent_TestDlg.h : header file
//
#pragma once

//#include "GfxContext.h"
#include "GfxPresent.h"

// CLibVrPresent_TestDlg dialog
class CLibVrPresent_TestDlg : public CDialog
{
	typedef enum
	{
		eOverlay,
		eRGB,
		eOld,
	}eRUNTYPE;
	typedef  IMcVideoPresenter*  (*fpCreateVideoPresenter)();

// Construction
public:
	CLibVrPresent_TestDlg(CWnd* pParent = NULL);	// standard constructor
	~CLibVrPresent_TestDlg();
// Dialog Data
	enum { IDD = IDD_LIBVRPRESENT_TEST_DIALOG };


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
	// Screen Width of origianl source file
	int SetScreenRect(RECT* pSrc, IMcVideoPresenter* pPresent);
	int SetDestRect(RECT* pDst,IMcVideoPresenter* pPresent);
	int ShowSurface(int type);
	int ReleasePresent();

	IMcVideoPresenter* m_pPresent;
	DWORD m_vSourceWidth;
	DWORD m_vSourceHeight;
	DWORD m_vClipStartX;
	DWORD m_vClipStartY;
	DWORD m_vClipWidth;
	DWORD m_vClipHeight;
	DWORD m_vDestWdith;
	DWORD m_vDestHeight;
	afx_msg void OnBnClickedButtonHorizontalline();
	afx_msg void OnBnClickedButtondiagonal();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	BOOL bFullScreen;
	DWORD m_vBlackLineWidth;
	DWORD m_vWhiteLineWidth;
	VP_VIDEOBUFFER_INFO vbi;
	fpCreateVideoPresenter procCreate;
	eRUNTYPE etype;
	HMODULE m_hDll;



	afx_msg void OnBnClickedButtonVerticalline();
private:
	int LoadDLL();

};
