#include "stdafx.h"
#include <atlimage.h>
#include "LoadingWindow.h"
#include "resource.h"

#define LOADING_CONFIG_FILE			"ui\\Loading\\Loading.cfg"

LoadingWindow::LoadingWindow()
{
	m_hWnd = NULL;
	m_fCurrentPercentage = 0.0f;
	m_fPercentage = 0.0f;
	m_fNextPercentage = 0.0f;
	m_dwTimeLast = 0;
	m_dwStartTime = 0;
	m_dwTime = 0;
	m_bLoading = FALSE;
	m_bFinished = FALSE;
	m_hInstance = NULL;
	m_strClass = NULL;
	m_strTitle = NULL;
	m_strText = NULL;
	m_hBackGround = NULL;
	m_hProgress = NULL;
	m_hSprite = NULL;
	m_nBackGroundWidth = 600;
	m_nBackGroundHeight = 333;
	m_nProgressWidth = 338;
	m_nProgressHeight = 3;
	m_nSpriteWidth = 28;
	m_nSpriteHeight = 28;
	m_nProgressX = 35;
	m_nProgressY = 297;
	m_bldFunc.BlendOp = AC_SRC_OVER;
	m_bldFunc.BlendFlags = 0;
	m_bldFunc.SourceConstantAlpha = 255;
	m_bldFunc.AlphaFormat = AC_SRC_ALPHA;
	m_nRestorX = 0;
	m_nRestorY = 0;
	m_nTextX = 0;
	m_nTextY = 0;
	m_bCanceled = FALSE;
	m_hFont = NULL;
}

LoadingWindow::~LoadingWindow()
{
	EndLoading();
}

int LoadingWindow::Init(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_strClass = Semantic::cstr_string("LoadingWindow");

	_CreateFont();

	return true;
}

void LoadingWindow::Uninit()
{
	EndLoading();
}

void LoadingWindow::StartLoading(DWORD dwTime)
{
	EndLoading();
	m_bLoading = TRUE;
	m_dwTimeLast = GetTickCount();
	m_dwStartTime = m_dwTimeLast;
	if (dwTime == 0)
		dwTime = 500;
	m_dwTime = dwTime;
	m_fPercentage = 0.0f;
	m_fNextPercentage = 1.0f;
	m_bCanceled = FALSE;
	_BeginLoadingThread();
}

void LoadingWindow::SetPercentage(float fPercentage)
{
	if (m_fCurrentPercentage >= 1.0f)
		m_dwTime = (DWORD)((float)(GetTickCount() - m_dwStartTime) / fPercentage);

	m_fNextPercentage = fPercentage;
}

void LoadingWindow::EndLoading()
{
	if (m_bLoading)
	{
		if (m_hWnd)
		{
			::ShowWindow(m_hWnd, SW_HIDE);
		}

		_EndLoadingThread();
	}
}

void LoadingWindow::HideWindow()
{
	if (m_hWnd)
	{
		::ShowWindow(m_hWnd, SW_HIDE);
	}
}

BOOL LoadingWindow::IsFinished() const
{
	return m_bFinished;
}

BOOL LoadingWindow::IsCanceled() const
{
	return m_bCanceled;
}

LRESULT CALLBACK LoadingWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LoadingWindow* pWindow = (LoadingWindow*)(LONG_PTR)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	if (pWindow)
	{
		switch (message)
		{
		case WM_CREATE:
		case WM_PAINT:
			pWindow->_Paint(TRUE);
			break;
		case WM_CLOSE:
		case WM_QUIT:
			pWindow->_SetCanceled(TRUE);
			break;
		case WM_INPUTLANGCHANGEREQUEST:
			return FALSE;
			break;
		}
	}

	return DefWindowProcA(hWnd, message, wParam, lParam);
}

void LoadingWindow::_SetCanceled(BOOL bCanceled)
{
	m_bCanceled = bCanceled;
}

unsigned int __stdcall LoadingWindow::_ThreadFunction(void* pvThis)
{
	LoadingWindow* pLoadingWindow = (LoadingWindow*)pvThis;
	if (pLoadingWindow)
	{
		pLoadingWindow->_ThreadFunction();
	}
	return true;
}

int LoadingWindow::_LoadResource()
{
	// 	int nResult = false;
	// 	int nRetCode = false;
	// 	Configure::IFile* pFile = NULL;
	// 	Configure::Table tabLoading = NULL;
	// 	BITMAP Bmp;
	// 	CImage Img;
	// 
	// 	do
	// 	{
	// 		pFile = CreateRLFile();
	// 		ERROR_BREAK(pFile);
	// 
	// 		tabLoading = pFile->Open(LOADING_CONFIG_FILE, eRLFileMode_Read, eRLFileType_Auto);
	// 		ERROR_BREAK(tabLoading);
	// 
	// 		uint uCount = tabLoading->Count();
	// 
	// 		srand((unsigned)time(NULL));
	// 		uint uIndex = rand() % uCount;
	// 
	// 		tabLoading = tabLoading->GetTable(uIndex);
	// 		ERROR_BREAK(tabLoading);
	// 
	// 		c_str strBackGround = tabLoading->GetString("BackGroundBMP");
	// 		ERROR_BREAK(strBackGround && strBackGround[0]);
	// 
	// 		c_str strProgress = tabLoading->GetString("ProgressBMP");
	// 		ERROR_BREAK(strProgress && strProgress[0]);
	// 
	// 		c_str strSprite = tabLoading->GetString("SpriteBMP");
	// 		ERROR_BREAK(strSprite && strSprite[0]);
	// 
	// 		m_nProgressX = tabLoading->GetInteger("ProgressX", 0);
	// 		m_nProgressY = tabLoading->GetInteger("ProgressY", 0);
	// 
	// 		Img.Load(strBackGround);
	// 		m_hBackGround = Img.Detach();
	// 		ERROR_BREAK(m_hBackGround);
	// 
	// 		GetObject(m_hBackGround, sizeof(BITMAP), &Bmp);
	// 		m_nBackGroundWidth = Bmp.bmWidth;
	// 		m_nBackGroundHeight = Bmp.bmHeight;
	// 
	// 		Img.Load(strProgress);
	// 		m_hProgress = Img.Detach();
	// 		ERROR_BREAK(m_hProgress);
	// 
	// 		GetObject(m_hProgress, sizeof(BITMAP), &Bmp);
	// 		m_nProgressWidth = Bmp.bmWidth;
	// 
	// 		Img.Load(strSprite);
	// 		m_hSprite = Img.Detach();
	// 		ERROR_BREAK(m_hSprite);
	// 
	// 		GetObject(m_hSprite, sizeof(BITMAP), &Bmp);
	// 		m_nSpriteWidth = Bmp.bmWidth;
	// 		m_nSpriteHeight = Bmp.bmHeight;
	// 
	// 		nResult = true;
	// 	} while (0);
	// 
	// 	SAFE_RELEASE(pFile);
	// 
	// 	if (!nResult)
	// 		_UnloadResource();
	// 
	// 	return nResult;
	return true;
}

void LoadingWindow::_UnloadResource()
{
	if (m_hSprite)
	{
		DeleteObject(m_hSprite);
		m_hSprite = NULL;
	}

	if (m_hProgress)
	{
		DeleteObject(m_hProgress);
		m_hProgress = NULL;
	}

	if (m_hBackGround)
	{
		DeleteObject(m_hBackGround);
		m_hBackGround = NULL;
	}
}

void LoadingWindow::_ThreadFunction()
{
	int nRetCode = false;
	MSG msg;

	do
	{
		nRetCode = _LoadResource();
		ERROR_BREAK(nRetCode);

		nRetCode = _RegisterClass();
		ERROR_BREAK(nRetCode);

		nRetCode = _CreateWindows();
		ERROR_BREAK(nRetCode);

		while (m_bLoading)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			_Calculate();
			Sleep(50);
		}
	} while (0);

	_DestroyWindows();
	_UnRegisterClass();
	_UnloadResource();
	m_bFinished = TRUE;
}

void LoadingWindow::_Calculate()
{
	DWORD dwTimeNow = GetTickCount();
	DWORD dwDeltaTime = dwTimeNow - m_dwTimeLast;
	m_dwTimeLast = dwTimeNow;

	float fDelta = (float)dwDeltaTime / (float)m_dwTime;

	float fSet = m_fCurrentPercentage;

	if (m_fCurrentPercentage < m_fPercentage)
	{
		if (fDelta < 0.003f)
			fDelta = 0.003f;
		fSet += fDelta * 3.0f;
	}
	else
	{
		fSet += fDelta;
	}

	if (fSet >= 1.0f)
		m_bFinished = TRUE;

	if (fSet > m_fNextPercentage)
		fSet = m_fNextPercentage;

	while (!m_queProgress.empty())
	{
		_ProgressUnit unit = m_queProgress.front();
		m_strText = unit.strText;
		m_queProgress.pop();
		break;
	}

	//if (m_fCurrentPercentage != fSet)
	{
		m_fCurrentPercentage = fSet;
		_Paint(FALSE);
	}
}

void LoadingWindow::_Paint(BOOL bRepaint)
{
	HDC hDc = NULL;
	HDC hdcSrc = NULL;
	HGDIOBJ hOldObj = NULL;

	do
	{
		ERROR_BREAK(m_hWnd);

		int nProgressLen = (int)(m_nProgressWidth * m_fCurrentPercentage);

		hDc = GetDC(m_hWnd);
		ERROR_BREAK(hDc);

		hdcSrc = CreateCompatibleDC(NULL);
		ERROR_BREAK(hdcSrc);

		hOldObj = SelectObject(hdcSrc, m_hBackGround);

		if (bRepaint)
		{
			BitBlt(hDc, 0, 0, m_nBackGroundWidth, m_nBackGroundHeight, hdcSrc, 0, 0, SRCCOPY);

			SelectObject(hdcSrc, m_hProgress);
			BitBlt(hDc, m_nProgressX, m_nProgressY, nProgressLen, m_nProgressHeight, hdcSrc, 0, 0, SRCCOPY);
		}
		else
		{
			BitBlt(hDc, m_nRestorX, m_nRestorY, m_nBackGroundWidth, m_nBackGroundHeight, hdcSrc, m_nRestorX, m_nRestorY, SRCCOPY);
			BitBlt(hDc, m_nTextX, m_nTextY, m_nBackGroundWidth, m_nBackGroundHeight, hdcSrc, m_nTextX, m_nTextY, SRCCOPY);

			SelectObject(hdcSrc, m_hProgress);
			BitBlt(hDc, m_nRestorX, m_nProgressY, nProgressLen - m_nRestorX + m_nProgressX, m_nProgressHeight, hdcSrc, m_nRestorX - m_nProgressX, 0, SRCCOPY);

			if (m_strText && m_strText[0])
			{
				SetBkMode(hDc, TRANSPARENT);
				SetTextColor(hDc, RGB(255, 255, 255));

				m_nTextX = m_nProgressX + nProgressLen - m_nSpriteWidth / 2 - 30;
				m_nTextY = m_nProgressY + (m_nProgressHeight - m_nSpriteHeight) / 2 + 30;

				SelectObject(hDc, m_hFont);
				TextOut(hDc, m_nTextX, m_nTextY, m_strText, (int)strlen(m_strText));
			}
		}

		m_nRestorX = m_nProgressX + nProgressLen - m_nSpriteWidth / 2;
		m_nRestorY = m_nProgressY + (m_nProgressHeight - m_nSpriteHeight) / 2;

		SelectObject(hdcSrc, m_hSprite);
		AlphaBlend(hDc, m_nRestorX, m_nRestorY, m_nSpriteWidth, m_nSpriteHeight, hdcSrc, 0, 0, m_nSpriteWidth, m_nSpriteHeight, m_bldFunc);

		SelectObject(hdcSrc, hOldObj);
	} while (0);

	if (hdcSrc)
		DeleteDC(hdcSrc);
	if (hDc)
		ReleaseDC(m_hWnd, hDc);
}

HWND LoadingWindow::GetHWND() const
{
	return m_hWnd;
}

int LoadingWindow::_RegisterClass()
{
	int nResult = false;
	int nRetCode = false;

	do
	{
		WNDCLASS wc;

		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(m_hInstance, MAKEINTATOM(IDI_MAIN));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_strClass;

		nRetCode = RegisterClassA(&wc);
		ERROR_BREAK(nRetCode);

		nResult = true;
	} while (0);

	if (!nResult)
		_UnRegisterClass();

	return nResult;
}

void LoadingWindow::_UnRegisterClass()
{
	if (m_strClass && m_hInstance)
	{
		::UnregisterClassA(m_strClass, m_hInstance);
		m_strClass = NULL;
	}
}

int LoadingWindow::_CreateWindows()
{
	int nResult = false;
	int nRetCode = false;

	do
	{
		int nX = (::GetSystemMetrics(SM_CXSCREEN) - m_nBackGroundWidth) / 2;
		int nY = (::GetSystemMetrics(SM_CYSCREEN) - m_nBackGroundHeight) / 2;

		m_hWnd = CreateWindowExA(
			0,
			m_strClass,
			m_strTitle,
			WS_VISIBLE | WS_POPUP,
			nX, nY, m_nBackGroundWidth, m_nBackGroundHeight,
			NULL,
			NULL,
			NULL,
			NULL);
		ERROR_BREAK(m_hWnd);

		::SetWindowLongPtrA(m_hWnd, GWLP_USERDATA, (__int3264)(LONG_PTR)this);

		nRetCode = UpdateWindow(m_hWnd);
		ERROR_BREAK(nRetCode);

		nResult = true;
	} while (0);

	if (!nResult)
		_DestroyWindows();

	return nResult;
}

void LoadingWindow::_DestroyWindows()
{
	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}

int LoadingWindow::_BeginLoadingThread()
{
	int nResult = false;

	do
	{
		m_bLoading = TRUE;

		m_hLoadingThread = (HANDLE)_beginthreadex(NULL
			, 0
			, &LoadingWindow::_ThreadFunction
			, this
			, 0
			, NULL
		);
		ERROR_BREAK(m_hLoadingThread);

		nResult = true;
	} while (0);

	if (!nResult)
		_EndLoadingThread();

	return nResult;
}

int LoadingWindow::_EndLoadingThread()
{
	if (m_hLoadingThread)
	{
		m_bLoading = FALSE;
		::WaitForSingleObject(m_hLoadingThread, INFINITE);
		::CloseHandle(m_hLoadingThread);
		m_hLoadingThread = NULL;
	}

	return true;
}

int LoadingWindow::_CreateFont()
{
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(logfont));
	logfont.lfCharSet = GB2312_CHARSET;
	logfont.lfHeight = 12;
	logfont.lfWeight = 200;

	m_hFont = CreateFontIndirect(&logfont);

	return true;
}

void LoadingWindow::Progress(float fPercentage, Semantic::c_str strText)
{
	if (m_fCurrentPercentage >= 1.0f && fPercentage > 0)
		m_dwTime = (DWORD)((float)(GetTickCount() - m_dwStartTime) / fPercentage);

	m_fNextPercentage = fPercentage;

	_ProgressUnit unit;
	unit.fPercentage = fPercentage;
	unit.strText = strText;
	m_queProgress.push(unit);
}
