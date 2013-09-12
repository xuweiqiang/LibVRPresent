// SH4_t1Dlg.h : header file
//

#pragma once

#include "../../inc/GfxPresent.h"


typedef enum VideoStatus
{
	ePlay,
	ePause,
	eStop,
}eStatus,*peStatus;

// CSH4_t1Dlg dialog
class CSH4_t1Dlg : public CDialog
{
// Construction
public:
	CSH4_t1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SH4_T1_DIALOG };


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

	HRESULT LoadLib(HMODULE& h_dll);
	void OnBnClickedButton1();
	HRESULT Init_CreatePresenter(HMODULE& h_dll);

    IMcVideoPresenter* m_pPresent;
	RECT m_rect_last;
	    FILE* m_file;
	    DWORD m_FileLength;
	void OpenVideo();
	void CloseVideo();
	bool OnTimer(UINT_PTR nIDEvent);
	int SetDestRect(IMcVideoPresenter* pPresent);


	LONG m_lZoomMode;
	LONG m_lInterpolationMode;
	LONG m_lRotationMode;
	RECT Srcrect;
    RECT Cliprect;
    RECT Dstrect;
    DWORD m_vSourceWidth;
    DWORD m_vSourceHeight;
    DWORD m_vClipStartX;
    DWORD m_vClipStartY;
    DWORD m_vClipWidth;
    DWORD m_vClipHeight;
    DWORD m_vDestStartX;
    DWORD m_vDestStartY;
    DWORD m_vDestWidth;
    DWORD m_vDestHeight;
    DWORD m_vBackBufferNumber;
    INT   m_vBackBufferEnable;
	    LPBYTE m_pYbuf;
    LPBYTE m_pUbuf;
    LPBYTE m_pVbuf;
	    LPBYTE m_pTMPbuf;
    eStatus status;
    VP_ZOOM_MODE m_eVideoMode;
	DWORD m_dwStrides[3];

	DECLARE_MESSAGE_MAP()
};
