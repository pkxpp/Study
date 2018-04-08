// MyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SprReader.h"
#include "MyDialog.h"


// CMyDialog dialog

IMPLEMENT_DYNAMIC(CMyDialog, CDialog)

CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDialog::IDD, pParent)
	, m_Size(_T(""))
	, m_Height(_T(""))
	, m_Width(_T(""))
{

}
/*
CMyDialog::CMyDialog(CSprReaderView* pParent)
{
	m_pSprReaderView = pParent;
}*/

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Size);
	DDX_Text(pDX, IDC_EDIT2, m_Height);
	DDX_Text(pDX, IDC_EDIT3, m_Width);
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
	ON_COMMAND(ID_32775, &CMyDialog::OnCreateNewSpr)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDialog::OnCancel)
END_MESSAGE_MAP()


// CMyDialog message handlers

void CMyDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	this->DestroyWindow();
}

void CMyDialog::OnCreateNewSpr()
{
	// TODO: Add your command handler code here
}

void CMyDialog::OnCancel()
{
	// TODO: Add your control notification handler code here
	this->DestroyWindow();
}
