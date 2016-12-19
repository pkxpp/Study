// PlayFlashDlg.h : header file
//
//{{AFX_INCLUDES()
#include "shockwaveflash.h"
//}}AFX_INCLUDES

#if !defined(AFX_PLAYFLASHDLG_H__8B817D42_C8F1_4D95_A34C_E03279FD410A__INCLUDED_)
#define AFX_PLAYFLASHDLG_H__8B817D42_C8F1_4D95_A34C_E03279FD410A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPlayFlashDlg dialog

class CPlayFlashDlg : public CDialog
{
// Construction
public:
	CPlayFlashDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayFlashDlg)
	enum { IDD = IDD_PLAYFLASH_DIALOG };
	CShockwaveFlash	m_FlashPlayer;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayFlashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPlayFlashDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYFLASHDLG_H__8B817D42_C8F1_4D95_A34C_E03279FD410A__INCLUDED_)
