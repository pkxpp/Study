#pragma once
#include "d3dx9.h"
#include "Structs.h"


// CDXwnd

class CDXwnd : public CWnd
{
	DECLARE_DYNAMIC(CDXwnd)

public:
	CDXwnd();
	virtual ~CDXwnd();

protected:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszAppName,int nWidth,int nHeight);

	bool InitDX(void);
	bool DispIt(float timeDelta);
	bool Setup(void);

private:
	IDirect3DDevice9*       m_pDevice;
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*  m_pIB;
	D3DDEVTYPE              m_DevType;

	bool m_IsWindowed;
	RECT m_Rect;
};


