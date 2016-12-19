#pragma once

#include "SprConverter.h"

// CMyDialogTool dialog
extern LPDIRECT3DDEVICE9       pd3dDevice;

class CMyDialogTool : public CDialog
{
	DECLARE_DYNAMIC(CMyDialogTool)

public:
	CMyDialogTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyDialogTool();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_OldFileName;
	CString m_NewFileName;
	afx_msg void OnBnClickedButton1();
};
