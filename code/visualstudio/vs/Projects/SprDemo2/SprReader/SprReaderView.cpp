// SprReaderView.cpp : implementation of the CSprReaderView class
//

#include "stdafx.h"
#include "SprReader.h"

#include "SprReaderDoc.h"
#include "SprReaderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

LPDIRECT3DDEVICE9       pd3dDevice;
// CSprReaderView

IMPLEMENT_DYNCREATE(CSprReaderView, CView)

BEGIN_MESSAGE_MAP(CSprReaderView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CSprReaderView::OnFileOpen)
	ON_COMMAND(ID_VIEW_32774, &CSprReaderView::OnViewPro)
	ON_WM_TIMER()
	ON_COMMAND(ID_TOOLS_OLD2NEW, &CSprReaderView::OnToolsOld2new)
	ON_COMMAND(ID_VIEW_INTERVAL, &CSprReaderView::OnViewSet)
	ON_MESSAGE(WM_MY_MESSAGE, OnMyMessage)
	ON_COMMAND(ID_32775, &CSprReaderView::OnSynSpr)
	ON_COMMAND(ID_SAVE_CURRENT_VIEW, &CSprReaderView::OnSaveCurrentView)
END_MESSAGE_MAP()

// CSprReaderView construction/destruction
int CSprReaderView::ms_times = 0;

CSprReaderView::CSprReaderView()
{
	// TODO: add construction code here
	m_nFrame = 0;
	m_interval = 100;
}

CSprReaderView::~CSprReaderView()
{
}

BOOL CSprReaderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSprReaderView drawing

void CSprReaderView::OnDraw(CDC* /*pDC*/)
{
	CSprReaderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSprReaderView printing
BOOL CSprReaderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSprReaderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSprReaderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
	if (m_pSprData) {delete[] (m_pSprData); (m_pSprData)=NULL;}
}


IDirect3DSurface9* getSurfaceFromFile(const char* filename, RECT *rect)
{
	HRESULT hResult;
	IDirect3DSurface9* surface = NULL;
	D3DXIMAGE_INFO imageInfo;

	// Get the width and height info from this png
	hResult = D3DXGetImageInfoFromFile(filename, &imageInfo);
	if FAILED (hResult)
		return NULL;
	
//	char str[20];
//	itoa(imageInfo.Width, str, 10);
//	MessageBox(NULL, str, "ERROR", MB_OK);
//	itoa(imageInfo.Height, str, 10);
//	MessageBox(NULL, str, "ERROR", MB_OK);
	rect->bottom = imageInfo.Height;
	rect->right = imageInfo.Width;

	hResult = pd3dDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (FAILED(hResult))
		return NULL;

	hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filename, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return NULL;

	return surface;
}

// CSprReaderView diagnostics

#ifdef _DEBUG
void CSprReaderView::AssertValid() const
{
	CView::AssertValid();
}

void CSprReaderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSprReaderDoc* CSprReaderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSprReaderDoc)));
	return (CSprReaderDoc*)m_pDocument;
}
#endif //_DEBUG


// CSprReaderView message handlers




void CSprReaderView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, NULL, this);
//	dlg.m_ofn.lpstrInitialDir = (BSTR)directoryName;  //设置对话框默认呈现的路径

	CString strFilePath;
	 if(dlg.DoModal() == IDOK){
		 strFilePath=dlg.GetPathName();
	 }

	 if(strFilePath.IsEmpty())
		 return;

	 m_FileFullName = strFilePath;

	 // 得到文件名的前缀, strlen(".spr") = 4
	 m_FilePath = strFilePath.Left(strFilePath.GetAllocLength() - 4);
//	 MessageBox(m_FilePath.GetBuffer(), "Error", MB_OK);

		 // Read spr file to get some infomation
		FILE* pF = NULL;
		pF = fopen(strFilePath.GetBuffer(), "rb");
		if(NULL == pF)
			return;
		
		// 读取spr文件内容
		m_uSprDataSize = filelength(fileno(pF));
		m_pSprData = new BYTE[m_uSprDataSize];
		if (m_pSprData == NULL)
		{
			fclose(pF);
			MessageBox(_T("new Error"), _T("ERROR"), MB_OK);
		}
		if (fread(m_pSprData, m_uSprDataSize, 1, pF) == 1)
		{
			fclose(pF);
			m_pSprHead = (FSprHead*)m_pSprData;
			if ((LPBYTE)m_pSprHead + sizeof(FSprHead) > (LPBYTE)m_pSprData + m_uSprDataSize)
			{
				MessageBox(_T("ERROR!"), _T("ERROR"), MB_OK);
			}
			if (*(int*)&m_pSprHead->Comment[0] != SPR_COMMENT_FLAG || !m_pSprHead->Frames || m_pSprHead->FrameFormat != enumFF_LinkPng)
			{
				return;
			}

			m_ppSprFrameList = new FSprFrame*[m_pSprHead->Frames];
			if (m_ppSprFrameList == NULL)
			{
//				return false;
			}
			LPBYTE pSprite = (LPBYTE)((LPBYTE)m_pSprHead + sizeof(FSprHead));
			for (int i = 0; i < m_pSprHead->Frames; i++)
			{
//				m_ppSprFrameList[i] = (FSprFrame*)(pSprite + m_pSprOffsList[i].Offset);
				//To do...
			}
		}

		// 从文件中读取相关信息
		m_nFrame = m_pSprHead->Frames;
		m_interval = m_pSprHead->Interval;
		ms_times = 0;

		/**
		 * for test
		CString strTest;
		strTest.Format("%d", m_interval);
		MessageBox(strTest.GetBuffer(), "TEST", MB_OK);
		*/
		
	// DXD 
	pd3dDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);				// 构建一个backbuffer
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(250,250,250), 1.0f, 0 );	// clear the backbuffer to a blue color

	// 图像显示的位置，默认为中间（200， 200）
	rect.left = 320 - 1.0*m_pSprHead->Width/2;
	rect.top = 240 - 1.0*m_pSprHead->Height/2;
	rect.bottom = rect.top + 1.0*m_pSprHead->Height;
	rect.right = rect.left + 1.0*m_pSprHead->Width;
	
	// 设置定时器
	SetTimer(1, m_interval, NULL);
}

void CSprReaderView::OnViewPro()
{
	// TODO: Add your command handler code here
	char str[32];
	m_pMyDialogPro = new CMyDialog;
	m_pMyDialogPro->Create(CMyDialog::IDD);
//	if(m_pSprHead == NULL) ID_VIEW_32774	// 按钮为灰色状态

	sprintf(str, "%d × %d", m_pSprHead->Height, m_pSprHead->Width);
	m_pMyDialogPro->m_Size.SetString(str);
	sprintf(str, "%d	pixel", m_pSprHead->Height); 
	m_pMyDialogPro->m_Height.SetString(str);
	sprintf(str, "%d	pixel", m_pSprHead->Width); 
	m_pMyDialogPro->m_Width.SetString(str);
	m_pMyDialogPro->UpdateData(false);
	m_pMyDialogPro->ShowWindow(SW_SHOW);//显示对话框
}

void CSprReaderView::Render(){
}

void CSprReaderView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	char *filename = m_FilePath.GetBuffer();
	char cszFrame[5000];

	hResult = pd3dDevice->CreateOffscreenPlainSurface(m_pSprHead->Width, m_pSprHead->Height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	sprintf(cszFrame, "%s.%d.png", filename, ms_times);
//	MessageBox(cszFrame, "File Name", MB_OK);
//		bkgrd = getSurfaceFromFile(cszFrame, &rect);

	hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, cszFrame, NULL, D3DX_DEFAULT, 0, NULL);
//	if (FAILED(hResult))
//		return;y

	hResult = pd3dDevice->StretchRect(surface, NULL, backbuffer, &rect, D3DTEXF_NONE);
	pd3dDevice->Present( NULL, NULL, NULL, NULL );
	if(m_nFrame == ++ms_times) ms_times = 0;

	CView::OnTimer(nIDEvent);
}

void CSprReaderView::OnToolsOld2new()
{
	// TODO: Add your command handler code here
	m_pMyDialogTool = new CMyDialogTool;
	m_pMyDialogTool->Create(CMyDialogTool::IDD);
//	if(m_pSprHead == NULL) ID_VIEW_32774	// 按钮为灰色状态

	m_pMyDialogTool->ShowWindow(SW_SHOW);//显示对话框
}

void CSprReaderView::OnViewSet()
{
	// TODO: Add your command handler code here
	m_pKCDialogSet = new KCDialogSet;
	m_pKCDialogSet->Create(KCDialogSet::IDD);

	m_pKCDialogSet->ShowWindow(SW_SHOW);//显示对话框
}


// 自定义消息响应函数
LRESULT CSprReaderView::OnMyMessage(WPARAM wParam, LPARAM lParam)
{

	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(250,250,250), 1.0f, 0 );

	// Rect
	rect.left = 320 - 1.0*m_pSprHead->Width*g_StructSet.zoom/200;
	rect.top = 240 - 1.0*m_pSprHead->Height*g_StructSet.zoom/200;
	rect.right = rect.left + m_pSprHead->Width*g_StructSet.zoom/100.0;
	rect.bottom = rect.top + m_pSprHead->Height*g_StructSet.zoom/100.0;

	//Timer Reset
	m_interval = g_StructSet.interval;
	SetTimer(1, g_StructSet.interval, NULL);
//	MessageBox("asdf", "ERROR", MB_OK);
	//
	return 1;
}


void CSprReaderView::ReNew(UINT interval)
{
//	this->SetTimer(1, m_interval, NULL);
}

void CSprReaderView::OnSynSpr()
{
	// TODO: Add your command handler code here
	m_pKCDlgSynSpr = new KCDlgSynSpr;
	m_pKCDlgSynSpr->Create(KCDlgSynSpr::IDD);

	m_pKCDlgSynSpr->ShowWindow(SW_SHOW);
//	m_pKCDlgSynSpr->m_ListCtrl.InsertColumn(0, "文件名", LVCFMT_LEFT, 50);
//	m_pKCDlgSynSpr->m_ListCtrl.InsertColumn(1, "来源位置", LVCFMT_LEFT, 50);
//	m_pKCDlgSynSpr->UpdateData(ture);
//	m_pKCDlgSynSpr->m_ListCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, rectTmp, this, IDC_LIST1);

}
/*
void xxOnSaveCurrentView()
{
	// TODO: Add your command handler code here
	/// 读取SPR头文件，修改头文件信息
	FILE* pF = NULL;
	pF = fopen(m_FileFullName.GetBuffer(), "w");
		
	// 读取spr文件内容
	m_uSprDataSize = filelength(fileno(pF));
	m_pSprData = new BYTE[m_uSprDataSize];
	if (m_pSprData == NULL)
	{
		fclose(pF);
		MessageBox(_T("new Error"), _T("ERROR"), MB_OK);
	}
	if (fread(m_pSprData, m_uSprDataSize, 1, pF) == 1)
	{
		fclose(pF);
		m_pSprHead = (FSprHead*)m_pSprData;
		if ((LPBYTE)m_pSprHead + sizeof(FSprHead) > (LPBYTE)m_pSprData + m_uSprDataSize)
		{
			MessageBox(_T("ERROR!"), _T("ERROR"), MB_OK);
		}
		if (*(int*)&m_pSprHead->Comment[0] != SPR_COMMENT_FLAG || !m_pSprHead->Frames || m_pSprHead->FrameFormat != enumFF_LinkPng)
		{
			return;
		}

		m_ppSprFrameList = new FSprFrame*[m_pSprHead->Frames];
		if (m_ppSprFrameList == NULL)
		{
//			return false;
		}
		LPBYTE pSprite = (LPBYTE)((LPBYTE)m_pSprHead + sizeof(FSprHead));
		for (int i = 0; i < m_pSprHead->Frames; i++)
		{
//			m_ppSprFrameList[i] = (FSprFrame*)(pSprite + m_pSprOffsList[i].Offset);
			//To do...
		}
	}
	// 从文件中读取相关信息
	m_nFrame = m_pSprHead->Frames;
	m_interval = m_pSprHead->Interval;
	ms_times = 0;

	CString strTest;
	strTest.Format("%d", m_pSprHead->Interval);
	MessageBox(strTest.GetBuffer(), "TEST", MB_OK);
}

*/
void CSprReaderView::OnSaveCurrentView()
{
	// TODO: Add your command handler code here
	CString strTest;

	/// 读取SPR头文件，修改头文件信息
	FILE* pF = NULL;
	pF = fopen(m_FileFullName.GetBuffer(), "rb+");

	/// 文件指针定位
	unsigned int offset = sizeof(m_pSprHead->Comment)+sizeof(m_pSprHead->Height)+sizeof(m_pSprHead->Width)+sizeof(m_pSprHead->CenterX)+sizeof(m_pSprHead->CenterY)+
		sizeof(m_pSprHead->Colors)+sizeof(m_pSprHead->Frames)+sizeof(m_pSprHead->Directions);
//	strTest.Format("%d", offset);
//	MessageBox(strTest.GetBuffer(), "TEST", MB_OK);
	fseek(pF, offset, SEEK_SET);

	///写入新的数值
	fwrite(&m_interval, sizeof(m_pSprHead->Interval), 1, pF);
	rewind(pF);
		
	// 读取spr文件内容
	DWORD uSprDataSize = filelength(fileno(pF));
	BYTE *pSprData = new BYTE[uSprDataSize];
	if (pSprData == NULL)
	{
		fclose(pF);
		MessageBox(_T("new Error"), _T("ERROR"), MB_OK);
	}
	if (fread(pSprData, uSprDataSize, 1, pF) == 1)
	{
		fclose(pF);
		m_pSprHead = (FSprHead*)pSprData;
		if ((LPBYTE)m_pSprHead + sizeof(FSprHead) > (LPBYTE)pSprData + uSprDataSize)
		{
			MessageBox(_T("ERROR!"), _T("ERROR"), MB_OK);
		}
		if (*(int*)&m_pSprHead->Comment[0] != SPR_COMMENT_FLAG || !m_pSprHead->Frames || m_pSprHead->FrameFormat != enumFF_LinkPng)
		{
			return;
		}

		m_ppSprFrameList = new FSprFrame*[m_pSprHead->Frames];
		if (m_ppSprFrameList == NULL)
		{
//			return false;
		}
		LPBYTE pSprite = (LPBYTE)((LPBYTE)m_pSprHead + sizeof(FSprHead));
		for (int i = 0; i < m_pSprHead->Frames; i++)
		{
//			m_ppSprFrameList[i] = (FSprFrame*)(pSprite + m_pSprOffsList[i].Offset);
			//To do...
		}
	}
	// 从文件中读取相关信息
	m_nFrame = m_pSprHead->Frames;
	m_interval = m_pSprHead->Interval;
	ms_times = 0;

//	strTest.Format("%d", m_pSprHead->Interval);
//	MessageBox(strTest.GetBuffer(), "TEST", MB_OK);
}
