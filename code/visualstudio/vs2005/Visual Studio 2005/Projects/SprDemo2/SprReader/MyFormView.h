#pragma once



// CMyFormView form view

class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

public:
	CMyFormView();           // protected constructor used by dynamic creation
	virtual ~CMyFormView();

public:
	enum { IDD = IDD_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


