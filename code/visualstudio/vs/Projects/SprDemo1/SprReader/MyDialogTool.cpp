// MyDialogTool.cpp : implementation file
//

#include "stdafx.h"
#include "SprReader.h"
#include "MyDialogTool.h"


// CMyDialogTool dialog

IMPLEMENT_DYNAMIC(CMyDialogTool, CDialog)

CMyDialogTool::CMyDialogTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDialogTool::IDD, pParent)
	, m_OldFileName(_T(""))
	, m_NewFileName(_T(""))
{

}

CMyDialogTool::~CMyDialogTool()
{
}

void CMyDialogTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_OldFileName);
	DDX_Text(pDX, IDC_EDIT3, m_NewFileName);
}


BEGIN_MESSAGE_MAP(CMyDialogTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDialogTool::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyDialogTool message handlers

void CMyDialogTool::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	UpdateData(true);
	strTemp = m_OldFileName.GetString();
	if(pd3dDevice == NULL)
		MessageBox("D3D Device is NULL", "ERROR", MB_OK);
	SprConverter spr(pd3dDevice);
	if(strTemp.IsEmpty())
		MessageBox("Please input a name", "ERROR", MB_OK);
	spr.Converter(strTemp.GetBuffer());

	strTemp.ReleaseBuffer();
	this->DestroyWindow();
}