#pragma once
//#include "KCDlgSynSpr.h"

class KCDlgSynSpr;

class KCDialogRename : public CDialog
{
	DECLARE_DYNAMIC(KCDialogRename)

public:
	KCDialogRename(CWnd* pParent = NULL);   // standard constructor
//	KCDialogRename(KCDlgSynSpr* pParent = NULL);   // standard constructor
	virtual ~KCDialogRename();

// Dialog Data
	enum { IDD = IDD_DIALOGBARRENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	CString m_NewName;
	KCDlgSynSpr *m_pParent;
};

extern CString g_FileName;