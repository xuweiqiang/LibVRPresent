#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define MAX_DLL_NUM 50
#define PAGE_NUM    5
#define IDC_RADIODLL(n) IDC_RADIODLL1+n

typedef enum
{
	eAuto,
	eDDRAW,
	eGDI,
	ePRI,
	eOld,
	eD3D,
	eSDL,
	eSAMSUNG
}eRUNTYPE;

// CDllSelect dialog

class CDllSelect : public CDialog
{
	DECLARE_DYNAMIC(CDllSelect)

public:
	CDllSelect(eRUNTYPE type,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDllSelect();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECTDLL };
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonDllok();
	afx_msg void OnBnClickedButtonDllrefresh();
	afx_msg void OnBnClickedButtonDllprevious();
	afx_msg void OnBnClickedButtonDllnext();
	afx_msg void OnBnClickedRadiodll();
        afx_msg void OnBnClickedModeSel();

	afx_msg void OnClose();

	void SetDllList(TCHAR * dllFiles[], int page);
	void GetAllDll();

	int m_dllnum;
	int m_ValideNum;
	int curPage;
	int maxPage;
	TCHAR m_cDllFile[MAX_PATH];
	TCHAR *m_cAllDllFile[MAX_DLL_NUM];
	TCHAR *m_cValidDllFile[MAX_DLL_NUM];

	eRUNTYPE m_initPara_type;

	CButton m_btnPre;
	CButton m_btnNext;
	CButton m_btnDllOK;

	afx_msg void OnBnClickedRadioGdi2();
	afx_msg void OnBnClickedRadioPri();
};
