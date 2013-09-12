// RenderTestView.h : interface of the CRenderTestView class
//


#pragma once

class CRenderTestView : public CView
{
protected: // create from serialization only
	CRenderTestView();
	DECLARE_DYNCREATE(CRenderTestView)

// Attributes
public:
	CRenderTestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:

// Implementation
public:
	virtual ~CRenderTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RenderTestView.cpp
inline CRenderTestDoc* CRenderTestView::GetDocument() const
   { return reinterpret_cast<CRenderTestDoc*>(m_pDocument); }
#endif

