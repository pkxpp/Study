// SprReader.h : main header file for the SprReader application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSprReaderApp:
// See SprReader.cpp for the implementation of this class
//

class CSprReaderApp : public CWinApp
{
public:
	CSprReaderApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSprReaderApp theApp;