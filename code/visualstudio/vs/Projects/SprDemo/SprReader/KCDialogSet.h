#pragma once

#include "stdafx.h"

class CSprReaderView;
// Set面板对应的结构体---定义全局变量方便处理
struct KStructSet
{
	UINT interval;
	UINT zoom;
};
typedef struct KStructSet KStructSet;

// KCDialogSet dialog
//extern KStructSet g_KStructSet;

class KCDialogSet : public CDialog
{
	DECLARE_DYNAMIC(KCDialogSet)

public:
	KCDialogSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~KCDialogSet();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Interval;
	CString m_Zoom;
	CSprReaderView *m_pParHand;

	afx_msg void OnCancel();
	afx_msg void OnOk();
};

extern KStructSet g_StructSet;