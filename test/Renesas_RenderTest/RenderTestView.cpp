// RenderTestView.cpp : implementation of the CRenderTestView class
//

#include "stdafx.h"
#include "RenderTest.h"

#include "RenderTestDoc.h"
#include "RenderTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRenderTestView

IMPLEMENT_DYNCREATE(CRenderTestView, CView)

BEGIN_MESSAGE_MAP(CRenderTestView, CView)
END_MESSAGE_MAP()

// CRenderTestView construction/destruction

CRenderTestView::CRenderTestView()
{
	// TODO: add construction code here

}

CRenderTestView::~CRenderTestView()
{
}

BOOL CRenderTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


// CRenderTestView drawing
void CRenderTestView::OnDraw(CDC* /*pDC*/)
{
	CRenderTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}



// CRenderTestView diagnostics

#ifdef _DEBUG
void CRenderTestView::AssertValid() const
{
	CView::AssertValid();
}

CRenderTestDoc* CRenderTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRenderTestDoc)));
	return (CRenderTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CRenderTestView message handlers
