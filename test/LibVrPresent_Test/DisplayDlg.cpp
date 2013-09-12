// DisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LibVrPresent_Test.h"
#include "DisplayDlg.h"
#include "LibVrPresent_TestDlg.h"


// CDisplayDlg dialog

IMPLEMENT_DYNAMIC(CDisplayDlg, CDialog)

CDisplayDlg::CDisplayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayDlg::IDD, pParent)
{
	m_bCKey = FALSE;
	m_dwCKey = 0;
}

CDisplayDlg::~CDisplayDlg()
{
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDisplayDlg, CDialog)
       ON_WM_PAINT()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDisplayDlg message handlers

void CDisplayDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

 	((CLibVrPresent_TestDlg*)(this->GetParent()))->MoveDest(CPoint(x,y));
}

void CDisplayDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	((CLibVrPresent_TestDlg*)(this->GetParent()))->ReSize(CSize(cx, cy));
}

void CDisplayDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

 	((CLibVrPresent_TestDlg*)(this->GetParent()))->CloseVideo();
	CDialog::OnClose();
}

void CDisplayDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	((CLibVrPresent_TestDlg*)(this->GetParent()))->FullScrVideo();

	CDialog::OnLButtonDblClk(nFlags, point);
}

//void CDisplayDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//	((CLibVrPresent_TestDlg*)(this->GetParent()))->RecordPos(point);
//	
//	CDialog::OnLButtonDown(nFlags, point);
//}

//void CDisplayDlg::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//	((CLibVrPresent_TestDlg*)(this->GetParent()))->ReleaseMouse();
//
//	CDialog::OnLButtonUp(nFlags, point);
//}

//void CDisplayDlg::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	((CLibVrPresent_TestDlg*)(this->GetParent()))->ClickMove(point);
//	CDialog::OnMouseMove(nFlags, point);
//}

void CDisplayDlg::OnPaint()
{
 	CPaintDC dc(this); // device context for painting

	if(m_bCKey)
	{
		HDC hdc;
		RECT rc;
		HBRUSH hBrush;

		hdc = ::GetDC(m_hWnd);
		GetClientRect(&rc);	
		//hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		hBrush = CreateSolidBrush(m_dwCKey);
		FillRect(hdc, &rc, hBrush);
		DeleteObject(hBrush);
	}
}
	// Do not call CDialog::OnPaint() for painting messages
