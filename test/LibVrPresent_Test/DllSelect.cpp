// DllSelect.cpp : implementation file
//

#include "stdafx.h"
#include "LibVrPresent_TestDlg.h"
#include "DllSelect.h"


// CDllSelect dialog

IMPLEMENT_DYNAMIC(CDllSelect, CDialog)

CDllSelect::CDllSelect(eRUNTYPE type, CWnd* pParent /*=NULL*/)
	: CDialog(CDllSelect::IDD, pParent)
	, m_dllnum(0)
	, m_ValideNum(0)
	, curPage(1)
	, maxPage(0)
{
    m_initPara_type = type;
}

CDllSelect::~CDllSelect()
{
}

void CDllSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DLLPREVIOUS, m_btnPre);
	DDX_Control(pDX, IDC_BUTTON_DLLNEXT, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON_DLLOK, m_btnDllOK);
}


BEGIN_MESSAGE_MAP(CDllSelect, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_DLLOK, &CDllSelect::OnBnClickedButtonDllok)
	ON_BN_CLICKED(IDC_BUTTON_DLLREFRESH, &CDllSelect::OnBnClickedButtonDllrefresh)
	ON_BN_CLICKED(IDC_BUTTON_DLLPREVIOUS, &CDllSelect::OnBnClickedButtonDllprevious)
	ON_BN_CLICKED(IDC_BUTTON_DLLNEXT, &CDllSelect::OnBnClickedButtonDllnext)
	ON_BN_CLICKED(IDC_RADIODLL1, &CDllSelect::OnBnClickedRadiodll)
	ON_BN_CLICKED(IDC_RADIODLL2, &CDllSelect::OnBnClickedRadiodll)
	ON_BN_CLICKED(IDC_RADIODLL3, &CDllSelect::OnBnClickedRadiodll)
	ON_BN_CLICKED(IDC_RADIODLL4, &CDllSelect::OnBnClickedRadiodll)
	ON_BN_CLICKED(IDC_RADIODLL5, &CDllSelect::OnBnClickedRadiodll)
	ON_BN_CLICKED(IDC_RADIO_AUTO, &CDllSelect::OnBnClickedModeSel)
	ON_BN_CLICKED(IDC_RADIO_YUV, &CDllSelect::OnBnClickedModeSel)
	ON_BN_CLICKED(IDC_RADIO_RGB, &CDllSelect::OnBnClickedModeSel)
	ON_BN_CLICKED(IDC_RADIO_GDI, &CDllSelect::OnBnClickedModeSel)
	ON_BN_CLICKED(IDC_RADIO_PRI, &CDllSelect::OnBnClickedModeSel)
END_MESSAGE_MAP()

BOOL CDllSelect::OnInitDialog()
{
	m_cDllFile[0] = '\0';

	UpdateData(TRUE);

	OnBnClickedButtonDllrefresh();

	m_btnPre.EnableWindow(FALSE);
	m_btnDllOK.EnableWindow(FALSE);
	CButton *pBtn = NULL;	
    switch(m_initPara_type)
    {
        case eDDRAW:
            pBtn = (CButton*)GetDlgItem(IDC_RADIO_YUV);
            pBtn->SetCheck(TRUE);
            break;
        case eGDI:
            pBtn = (CButton*)GetDlgItem(IDC_RADIO_GDI);
            pBtn->SetCheck(TRUE);
            break;
        case ePRI:
            pBtn = (CButton*)GetDlgItem(IDC_RADIO_PRI);
            pBtn->SetCheck(TRUE);
            break;
        case eAuto:
        default:
            pBtn = (CButton*)GetDlgItem(IDC_RADIO_AUTO);
            pBtn->SetCheck(TRUE);
            break;
    }
       if(m_dllnum)
	{
		pBtn = (CButton*)GetDlgItem(IDC_RADIODLL(0));
   		pBtn->SetCheck(TRUE);
            OnBnClickedRadiodll();
       }

	return 0;
}

// CDllSelect message handlers


void CDllSelect::OnBnClickedButtonDllok()
{
	// TODO: Add your control notification handler code here

	//MessageBox(m_cDllFile);
	this->GetParent()->EnableWindow(TRUE);
	DestroyWindow();
}

void CDllSelect::OnBnClickedButtonDllrefresh()
{
	CButton *pBtn = NULL;	
	GetAllDll();
	curPage = 0;
	SetDllList(m_cAllDllFile, curPage);
	m_btnPre.EnableWindow(FALSE);
       if(m_dllnum)
	{
		pBtn = (CButton*)GetDlgItem(IDC_RADIODLL(0));
   		pBtn->SetCheck(TRUE);
            OnBnClickedRadiodll();
       }

}

//page: start from one; num: PAGE_NUM
void CDllSelect::SetDllList(TCHAR *dllFiles[], int page)
{
	int i = 0;

	i = page*PAGE_NUM;

	while(i < (page+1)*PAGE_NUM)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIODLL(i%PAGE_NUM));
		
		if(i >= m_dllnum)
		{
			pBtn->ShowWindow(SW_HIDE);
			i++;
			continue;
		}
		else
		{
			pBtn->ShowWindow(SW_SHOW);
		}

		pBtn->SetWindowText(dllFiles[i]);
		pBtn->SetCheck(FALSE);
		m_btnDllOK.EnableWindow(FALSE);

		if(!((CLibVrPresent_TestDlg*)(GetParent()))->IsValidDll(dllFiles[i]))
		{
			pBtn->EnableWindow(FALSE);
		}
		else
		{
			pBtn->EnableWindow(TRUE);
		}
		i++;
	}
	UpdateData(TRUE);
}

//get all dll files to m_cAllDllFiles array
void CDllSelect::GetAllDll()
{
	TCHAR DirPath[MAX_PATH] = {0};
	WIN32_FIND_DATA FindData = { 0 };
	HANDLE hFileSearch = NULL;
	
	GetModuleFileName(NULL, DirPath, MAX_PATH);
	TCHAR*	p = _tcsrchr(DirPath, '\\');
	*(++p) = 0;


	_tcscat(DirPath, _T("*.dll"));

	// Search DLL
	hFileSearch = FindFirstFile(DirPath, &FindData);
	if(hFileSearch == INVALID_HANDLE_VALUE)  // no Tuners
	{
		return ;
	}

	int i = 0;
	do
	{
		m_cAllDllFile[i] = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
		_tcscpy(m_cAllDllFile[i++], (LPCTSTR) FindData.cFileName);
	} while(FindNextFile(hFileSearch, &FindData));
	
	m_dllnum = i;
	maxPage = (m_dllnum / PAGE_NUM) + 1;
	if(maxPage <= 1)
		m_btnNext.EnableWindow(FALSE);
	else
		m_btnNext.EnableWindow(TRUE);

	if(m_dllnum > MAX_DLL_NUM)
	{
		MessageBox(_T("To many DLL in this folder!\n"));
		return;
	}
}

void CDllSelect::OnBnClickedButtonDllprevious()
{
	// TODO: Add your control notification handler code here
	if(curPage < 1)
	{
		MessageBox(TEXT("PageNumber Error"),TEXT("Error"));
		return;
	}
	curPage--;
	SetDllList(m_cAllDllFile, curPage);

	if(curPage == 0)
	{
		m_btnPre.EnableWindow(FALSE);
	}
	m_btnNext.EnableWindow(TRUE);
}

void CDllSelect::OnBnClickedButtonDllnext()
{
	// TODO: Add your control notification handler code here
	if(curPage >= maxPage-1)
	{
		MessageBox(TEXT("PageNumber Error"),TEXT("Error"));
		return;
	}
	curPage++;
	SetDllList(m_cAllDllFile, curPage);

	if(curPage == maxPage-1)
	{
		m_btnNext.EnableWindow(FALSE);
	}
	m_btnPre.EnableWindow(TRUE);
}

void CDllSelect::OnBnClickedRadiodll()
{
	// TODO: Add your control notification handler code here
	for(DWORD i = 0;i< 5;i++)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIODLL(i));
		if(BST_CHECKED == (pBtn->GetCheck()))
		{
			pBtn->GetWindowText(m_cDllFile,pBtn->GetWindowTextLength()+1);
			m_btnDllOK.EnableWindow(TRUE);
		}
	}
}

void CDllSelect::OnBnClickedModeSel()
{
	CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_AUTO);
	if(BST_CHECKED == (pBtn->GetCheck()))
	{
		m_initPara_type = eAuto;
			return;
	}
	pBtn = (CButton*)GetDlgItem(IDC_RADIO_YUV);
	if(BST_CHECKED == (pBtn->GetCheck()))
	{
		m_initPara_type = eDDRAW;
			return;
	}	
	pBtn = (CButton*)GetDlgItem(IDC_RADIO_GDI);
	if(BST_CHECKED == (pBtn->GetCheck()))
	{
		m_initPara_type = eGDI;
			return;
	}
	pBtn = (CButton*)GetDlgItem(IDC_RADIO_PRI);
	if(BST_CHECKED == (pBtn->GetCheck()))
	{
		m_initPara_type = ePRI;
			return;
	}
}
void CDllSelect::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	this->GetParent()->EnableWindow(TRUE);
	this->DestroyWindow();
}

