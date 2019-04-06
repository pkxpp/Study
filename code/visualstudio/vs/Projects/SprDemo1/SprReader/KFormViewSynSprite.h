#pragma once
#include "afxcmn.h"


// KFormViewSynSprite dialog

class KFormViewSynSprite : public CDialog
{
	DECLARE_DYNAMIC(KFormViewSynSprite)

public:
	KFormViewSynSprite(CWnd* pParent = NULL);   // standard constructor
	virtual ~KFormViewSynSprite();

// Dialog Data
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
};
