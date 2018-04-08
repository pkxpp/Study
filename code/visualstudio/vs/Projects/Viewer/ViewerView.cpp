// ViewerView.cpp : implementation of the CViewerView class
//

#include "stdafx.h"
#include "Viewer.h"

#include "ViewerDoc.h"
#include "ViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewerView

IMPLEMENT_DYNCREATE(CViewerView, CView)

BEGIN_MESSAGE_MAP(CViewerView, CView)
	//{{AFX_MSG_MAP(CViewerView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewerView construction/destruction

CViewerView::CViewerView()
{
	// TODO: add construction code here
}

CViewerView::~CViewerView()
{
}

BOOL CViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CViewerView drawing

void CViewerView::OnDraw(CDC* pDC)
{
	CViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CRect rect;
	GetClientRect(rect);
	if(pDoc->m_dib)
	pDoc->m_dib->DrawDIB(pDC,0,0,rect.Width(),rect.Height());
}

/////////////////////////////////////////////////////////////////////////////
// CViewerView printing

BOOL CViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CViewerView diagnostics

#ifdef _DEBUG
void CViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CViewerDoc* CViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CViewerDoc)));
	return (CViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewerView message handlers

void CViewerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
}

void CViewerView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CDC *dc = GetDC();
	CString s;
	int x=dc->GetDeviceCaps(LOGPIXELSX);
	int y=dc->GetDeviceCaps(LOGPIXELSY);
	int x1=pDC->GetDeviceCaps(LOGPIXELSX);
	int y1=pDC->GetDeviceCaps(LOGPIXELSY);
	s = GetDocument()->GetTitle();
	pDC->TextOut(0,0,s);
	pDC->MoveTo(0,75);
	pDC->LineTo(pInfo->m_rectDraw.Width(),75);
	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(x,y);
	pDC->SetViewportExt(x1,y1);
	pDC->SetViewportOrg(0,100);
	CView::OnPrint(pDC,pInfo);
}

void CViewerView::OnInitialUpdate() 
{
	
	// TODO: Add your specialized code here and/or call the base class
	CViewerDoc* pDoc = GetDocument();
	if(pDoc->m_dib)	 
	{
		// adjust the view according to the size of the dib//
		DWORD style,exstyle;
		CRect framerect,viewrect;
		viewrect.SetRect(0,0,pDoc->m_dib->GetWidth(),pDoc->m_dib->GetHeight());
		CFrameWnd *frame = GetParentFrame();
		style=GetStyle();
		exstyle=GetExStyle();
		AdjustWindowRectEx(&viewrect,style,FALSE,exstyle);
		style=frame->GetStyle();
		exstyle=frame->GetExStyle();
		AdjustWindowRectEx(&viewrect,style,TRUE,exstyle);
		frame->GetWindowRect(&framerect);
		framerect.right = framerect.left + viewrect.Width();
		framerect.bottom = framerect.top + viewrect.Height()+44;
		frame->MoveWindow(&framerect);
	}
		CView::OnInitialUpdate();
}
