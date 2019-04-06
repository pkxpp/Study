// ViewerDoc.cpp : implementation of the CViewerDoc class
//

#include "stdafx.h"
#include "Viewer.h"

#include "ViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewerDoc

IMPLEMENT_DYNCREATE(CViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CViewerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewerDoc construction/destruction

CViewerDoc::CViewerDoc()
{
	// TODO: add one-time construction code here
	m_dib = NULL;
}

CViewerDoc::~CViewerDoc()
{
}

BOOL CViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CViewerDoc serialization

void CViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		// load bmp here //

	}
}

/////////////////////////////////////////////////////////////////////////////
// CViewerDoc diagnostics

#ifdef _DEBUG
void CViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewerDoc commands

BOOL CViewerDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_dib)	return	m_dib->Save(lpszPathName);
	else        return  FALSE;
}

BOOL CViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	DeleteContents();
	SetModifiedFlag(FALSE);
	m_dib = new CDIBitmap;
	BOOL result =  m_dib->Load(lpszPathName);
	// adjust the window size //

	return result;
}

void CViewerDoc::DeleteContents()
{
	if(m_dib) delete m_dib;
	m_dib = NULL;
}
