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
	g_StructSet.zoom = 1;
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
//	m_pParHand->m_interval = atoi(this->m_Interval.GetBuffer());
//	m_pParHand->ReNew(atoi(this->m_Interval.GetBuffer()));
//	MessageBox(m_Interval.GetBuffer(), "ERROR", MB_OK);

	// 发送消息给主窗体
//	CView *pView = 
	::SendMessage(GetParent()->GetSafeHwnd(), WM_MY_MESSAGE, 0, 0);
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_MY_MESSAGE, 0, 0);
//	::SendMessage(m_pParHand->GetSafeHwnd(), WM_MY_MESSAGE, 0, 0);
//	GetParent()->SendMessage(WM_MY_MESSAGE, 0, 0);
//	m_pParHand->SendMessage(WM_MY_MESSAGE, 0, 0);
//	m_pParHand->SetTimer(1, g_StructSet.interval, NULL);
	CDialog::OnOK();
}
