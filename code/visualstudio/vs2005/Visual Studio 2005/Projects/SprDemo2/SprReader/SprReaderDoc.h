// SprReaderDoc.h : interface of the CSprReaderDoc class
//


#pragma once


class CSprReaderDoc : public CDocument
{
protected: // create from serialization only
	CSprReaderDoc();
	DECLARE_DYNCREATE(CSprReaderDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSprReaderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


