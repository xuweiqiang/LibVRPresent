// RenderTestDoc.cpp : implementation of the CRenderTestDoc class
//

#include "stdafx.h"
#include "RenderTest.h"

#include "RenderTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRenderTestDoc

IMPLEMENT_DYNCREATE(CRenderTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CRenderTestDoc, CDocument)
END_MESSAGE_MAP()

// CRenderTestDoc construction/destruction

CRenderTestDoc::CRenderTestDoc()
{
	// TODO: add one-time construction code here

}

CRenderTestDoc::~CRenderTestDoc()
{
}

BOOL CRenderTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CRenderTestDoc serialization

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void CRenderTestDoc::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// CRenderTestDoc diagnostics

#ifdef _DEBUG
void CRenderTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}
#endif //_DEBUG


// CRenderTestDoc commands

