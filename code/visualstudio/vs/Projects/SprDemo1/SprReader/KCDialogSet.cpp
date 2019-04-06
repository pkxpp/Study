// KCDialogSet.cpp : implementation file
//

#include "stdafx.h"
#include "SprReader.h"
#include "KCDialogSet.h"


// KCDialogSet dialog
IMPLEMENT_DYNAMIC(KCDialogSet, CDialog)

KStructSet g_StructSet;

KCDialogSet::KCDialogSet(CWnd* pParent /*=NULL*/)
	: CDialog(KCDialogSet::IDD, pParent)
	, m_Interval(_T(""))
	, m_Zoom(_T(""))
{
	g_StructSet.interval = 100;
	g_StructSet.zoom = 100; //100% = 1
}

KCDialogSet::~KCDialogSet()
{
}

void KCDialogSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Interval);
	DDX_Text(pDX, IDC_EDIT3, m_Zoom);
}


BEGIN_MESSAGE_MAP(KCDialogSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &KCDialogSet::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KCDialogSet::OnOk)
END_MESSAGE_MAP()


// KCDialogSet message handlers

void KCDialogSet::OnCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void KCDialogSet::OnOk()
{
	// TODO: Add your control notification handler code here
	UINT nTemp = 0;
	m_pParHand = (CSprReaderView*)this->GetParent();
//	m_pParHand = (CSprReaderView*)AfxGetMainWnd();

	UpdateData(true);		// 更新窗体上的值到变量
	if(!m_Interval.IsEmpty())
		g_StructSet.interval = atoi(this->m_Interval.GetBuffer());

	if(!m_Zoom.IsEmpty())
		g_StructSet.zoom = atoi(this->m_Zoom.GetBuffer());

	//

	// 发送消息给主窗体
	CFrameWnd *pFrame;
	pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CView *pView = pFrame->GetActiveView();
	if(pView != NULL)
		pView->SendMessage(WM_MY_MESSAGE, 0, 0);
//	::SendMessage(GetParent()->GetSafeHwnd(), WM_MY_MESSAGE, 0, 0);
//	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_MY_MESSAGE, 0, 0);

	CDialog::OnOK();
}
