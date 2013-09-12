// DisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DisplayDlg.h"


// CDisplayDlg dialog

IMPLEMENT_DYNAMIC(CDisplayDlg, CDialog)

CDisplayDlg::CDisplayDlg(ICivmVideoContext* pContext,ICivmVideoMixer* pVideoMixer,CWnd* pParent/*=NULL*/)
	: CDialog(CDisplayDlg::IDD, pParent)
{
	m_bCKey = FALSE;
	m_dwCKey = 0;
	m_pVideoContext = pContext;
	m_pVideoMixer = pVideoMixer;
	m_x = 0;
	m_y = 0;
	m_w = 0;
	m_h = 0;

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
	ON_WM_ACTIVATE() 
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDisplayDlg message handlers

void CDisplayDlg::OnActivate( UINT data, CWnd* wnd, BOOL bAct)
{
	CDialog::OnActivate(data, wnd, bAct);
	if(m_pVideoContext && (WA_ACTIVE == data || WA_CLICKACTIVE == data))
	{
		m_pVideoContext->BringToTop2D();
	}

}

void CDisplayDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	m_x = x;
	m_y = y;
	RECT rDst = {m_x, m_y, m_x +m_w, m_y + m_h};
	if(m_pVideoContext)
	{
		m_pVideoContext->SetPos2D(&rDst);
	}

	if(m_pVideoMixer)
		m_pVideoMixer->SetViewPort(&rDst);

	
}


void CDisplayDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	m_w = cx;
	m_h = cy;
	RECT rDst = {m_x, m_y, m_x +m_w, m_y + m_h};
	if(m_pVideoContext)
		m_pVideoContext->SetPos2D(&rDst);

	if(m_pVideoMixer)
		m_pVideoMixer->SetViewPort(&rDst);

}

void CDisplayDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CDisplayDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
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
}
	// Do not call CDialog::OnPaint() for painting messages
