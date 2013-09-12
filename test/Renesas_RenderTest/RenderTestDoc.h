// RenderTestDoc.h : interface of the CRenderTestDoc class
//


#pragma once

class CRenderTestDoc : public CDocument
{
protected: // create from serialization only
	CRenderTestDoc();
	DECLARE_DYNCREATE(CRenderTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// Implementation
public:
	virtual ~CRenderTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


