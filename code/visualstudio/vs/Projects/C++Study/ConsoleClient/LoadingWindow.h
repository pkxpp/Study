#pragma once

#include "queue"

class LoadingWindow
{
public:
	LoadingWindow();
	~LoadingWindow();

public:
	int Init(HINSTANCE hInstance);
	void Uninit();

	void StartLoading(DWORD dwTime);
	void EndLoading();

	void Progress(float fPercentage, Semantic::c_str strText);
	void SetPercentage(float fPercentage);
	void HideWindow();

	BOOL IsFinished() const;
	BOOL IsCanceled() const;
	HWND GetHWND() const;

private:
	static	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static unsigned int __stdcall _ThreadFunction(void* pvThis);

	int _BeginLoadingThread();
	int _EndLoadingThread();

	void _ThreadFunction();
	void _Calculate();

	void _Paint(BOOL bRepaint);
	void _SetCanceled(BOOL bCanceled);

	int  _LoadResource();
	void _UnloadResource();

	int	 _RegisterClass();
	void _UnRegisterClass();

	int  _CreateWindows();
	void _DestroyWindows();

	int _CreateFont();
	int _DestroyFont();

private:
	struct _ProgressUnit
	{
		float fPercentage;
		Semantic::c_str strText;
	};

	typedef std::queue<_ProgressUnit> ProgressQueue;

private:
	HANDLE m_hLoadingThread;

	HWND m_hWnd;
	float m_fCurrentPercentage;
	float m_fPercentage;
	float m_fNextPercentage;
	DWORD m_dwStartTime;
	DWORD m_dwTimeLast;
	DWORD m_dwTime;
	BOOL m_bLoading;
	BOOL m_bFinished;
	BOOL m_bCanceled;

	HINSTANCE m_hInstance;
	Semantic::c_str m_strClass;
	Semantic::c_str m_strTitle;
	Semantic::c_str m_strText;

	HBITMAP m_hBackGround;
	HBITMAP m_hProgress;
	HBITMAP m_hSprite;

	int m_nBackGroundWidth;
	int m_nBackGroundHeight;

	int m_nProgressWidth;
	int m_nProgressHeight;

	int m_nSpriteWidth;
	int m_nSpriteHeight;

	int m_nProgressX;
	int m_nProgressY;

	BLENDFUNCTION m_bldFunc;

	int m_nRestorX;
	int m_nRestorY;

	int m_nTextX;
	int m_nTextY;

	HFONT m_hFont;
	ProgressQueue m_queProgress;
};
