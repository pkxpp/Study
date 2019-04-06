#pragma once


// CMyDialog dialog

class CMyDialog : public CDialog
{
	DECLARE_DYNAMIC(CMyDialog)

public:
	CMyDialog(CWnd* pParent = NULL);   // standard constructor
//	CMyDialog(CSprReaderView* pParent = NULL); //重载构造函数
	virtual ~CMyDialog();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_Size;
	CString m_Height;
	CString m_Width;

	//
//	CSprReaderView *m_pSprReaderView;
	afx_msg void OnCreateNewSpr();
	afx_msg void OnCancel();
};
