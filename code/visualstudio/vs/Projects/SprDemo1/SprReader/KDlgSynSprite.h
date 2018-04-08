#pragma once


// KDlgSynSprite dialog

class KDlgSynSprite : public CDialog
{
	DECLARE_DYNAMIC(KDlgSynSprite)

public:
	KDlgSynSprite(CWnd* pParent = NULL);   // standard constructor
	virtual ~KDlgSynSprite();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
