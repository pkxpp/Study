// KCDialogRename.cpp : implementation file
//

#include "stdafx.h"
#include "SprReader.h"
#include "KCDialogRename.h"
#include "KCDlgSynSpr.h"

// KCDialogRename dialog

IMPLEMENT_DYNAMIC(KCDialogRename, CDialog)

CString g_FileName;

KCDialogRename::KCDialogRename(CWnd* pParent /*=NULL*/)
	: CDialog(KCDialogRename::IDD, pParent)
	, m_NewName(_T(""))
{
	m_NewName = g_FileName;
//	m_pParent = (KCDlgSynSpr*)pParent;
//	UpdateData(true);
}


//KCDialogRename::KCDialogRename(KCDlgSynSpr* pParent /*=NULL*/)
//	: CDialog(KCDialogRename::IDD, pParent)
//	, m_NewName(_T(""))
//{
//	m_NewName = g_FileName;
//	m_pParent = pParent;
//	UpdateData(true);
//}

KCDialogRename::~KCDialogRename()
{
}

void KCDialogRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITRENAME, m_NewName);
//	m_NewName = g_FileName;
//	UpdateData(true);
}


BEGIN_MESSAGE_MAP(KCDialogRename, CDialog)
	ON_BN_CLICKED(IDC_BUTTONOK, &KCDialogRename::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTONCANCEL, &KCDialogRename::OnBnClickedButtonCancel)
END_MESSAGE_MAP()

// KCDialogRename message handlers

void KCDialogRename::OnBnClickedButtonOk()
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
	if(m_NewName.IsEmpty())
	{
		MessageBox("你的输入为空", "提示", MB_OK);
	}
	else
		g_FileName = m_NewName;

//	MessageBox(g_FileName, "g_FileName", MB_OK);
	m_pParent->RenameUpdate();
	
	CDialog::OnOK();
}

void KCDialogRename::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
