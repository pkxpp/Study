// PlayFlash.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PlayFlash.h"
#include "PlayFlashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayFlashApp

BEGIN_MESSAGE_MAP(CPlayFlashApp, CWinApp)
	//{{AFX_MSG_MAP(CPlayFlashApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayFlashApp construction

CPlayFlashApp::CPlayFlashApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPlayFlashApp object

CPlayFlashApp theApp;
HINSTANCE hInst;
typedef int (__stdcall * REGISTERFUNC)(void);

/////////////////////////////////////////////////////////////////////////////
// CPlayFlashApp initialization

BOOL CPlayFlashApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

/*注册shockwave flash object控件，控件不能放在有中文的路径下
	TCHAR strCurDrt[500];
	int nLen = ::GetCurrentDirectory(500,strCurDrt);
	if( strCurDrt[nLen]!='\\' )
	{
		strCurDrt[nLen++] = '\\';
		strCurDrt[nLen] = '\0';
	}

	CString strFileName = strCurDrt;
	strFileName +="flash.ocx";
	AfxMessageBox(strFileName);
	hInst = NULL;
	hInst = LoadLibrary( strFileName );
	if( !hInst )
	{
		AfxMessageBox("加载flash.ocx失败，请检查flash.ocx是否和程序在同一目录下");
	}
	else
	{
		
		REGISTERFUNC pRegister;
		pRegister = GetProcAddress((HMODULE)hInst,"DllRegisterServer" );
		(*pRegister)();
	}

	*/
	
	CPlayFlashDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
