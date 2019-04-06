// SprReaderDoc.cpp : implementation of the CSprReaderDoc class
//

#include "stdafx.h"
#include "SprReader.h"

#include "SprReaderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSprReaderDoc

IMPLEMENT_DYNCREATE(CSprReaderDoc, CDocument)

BEGIN_MESSAGE_MAP(CSprReaderDoc, CDocument)
	
END_MESSAGE_MAP()


// CSprReaderDoc construction/destruction

CSprReaderDoc::CSprReaderDoc()
{
	// TODO: add one-time construction code here

}

CSprReaderDoc::~CSprReaderDoc()
{
}

BOOL CSprReaderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSprReaderDoc serialization

void CSprReaderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSprReaderDoc diagnostics

#ifdef _DEBUG
void CSprReaderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSprReaderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSprReaderDoc commands

