#pragma once
#include "Resource.h"
#include "GfxPresent.h"
//#include "VideoMixer.h"

// CDisplayDlg dialog

class CDisplayDlg : public CDialog
{
	enum { IDD = IDD_DIALOG1};

	DECLARE_DYNAMIC(CDisplayDlg)

public:
	CDisplayDlg(ICivmVideoContext* pContext,ICivmVideoMixer* pVideoMixer,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayDlg();

// Dialog Data
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

public:
	afx_msg void OnActivate( UINT data, CWnd* wnd, BOOL bAct);

 	ICivmVideoContext* m_pVideoContext ;
	ICivmVideoMixer* m_pVideoMixer;
	int m_x;
	int m_y;
	int m_w;
	int m_h;
	
};
