// PlayFlashDlg.cpp : implementation file
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
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayFlashDlg dialog

CPlayFlashDlg::CPlayFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayFlashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayFlashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlayFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayFlashDlg)
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_FlashPlayer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlayFlashDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayFlashDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayFlashDlg message handlers

BOOL CPlayFlashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int with= GetSystemMetrics(SM_CXFULLSCREEN); //获取当前屏幕大小
	
	int heigh= GetSystemMetrics(SM_CYFULLSCREEN);
	
	int   cx   =   GetSystemMetrics(   SM_CXSCREEN   );   
	int   cy   =   GetSystemMetrics(   SM_CYSCREEN   );

	   CRect rd(0,0,cx,cy);
	   //RECT rc;
	   this->GetClientRect(&rd);
	   
	::MoveWindow( GetSafeHwnd(),-10,-30,cx+10+5,cy+30+5, true );//设置窗口全屏

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	RECT rc;
	this->GetClientRect(&rc);

	m_FlashPlayer.MoveWindow( &rc, true );

	TCHAR strCurDrt[500];
	int nLen = ::GetCurrentDirectory(500,strCurDrt);
	if( strCurDrt[nLen]!='\\' )
	{
		strCurDrt[nLen++] = '\\';
		strCurDrt[nLen] = '\0';
	}

	CString strFileName = strCurDrt;
	strFileName += "startup.swf";
	m_FlashPlayer.LoadMovie(0, strFileName);
	m_FlashPlayer.Play();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlayFlashDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPlayFlashDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlayFlashDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CPlayFlashDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPlayFlashDlg)
	ON_EVENT(CPlayFlashDlg, IDC_SHOCKWAVEFLASH1, 150 /* FSCommand */, OnFSCommandShockwaveflash1, VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPlayFlashDlg::OnFSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args) 
{
	// TODO: Add your control notification handler code here
	if( 0 == strcmp(command,"bt") )
	{
		if( 0 == strcmp(args,"enter") )
		{
			MessageBox("欢迎进入系统！");
		}
	}
	else if( 0 == strcmp(command,"quit") )
	{
		MessageBox("您选择了退出！");
		CDialog::OnCancel();
	}
}
