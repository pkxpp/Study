#pragma once

namespace TOP
{
	class VirtualClient
	{

	public:
		VirtualClient();
		~VirtualClient();

	private:
		HINSTANCE m_hInst;
		HWND m_hWnd;
#ifdef OPENGL
		KWebResource* m_pWebResource;
#endif

		char m_szWndTitle[64] = { 0 };
		char m_szWndClass[64] = { 0 };
		int m_dwFrame = 60;
		int m_dwLogicFrame = 60;
		DWORD m_dwTimeTick;
		DWORD m_dwProgramStartTick;

		POINT m_vLastPt;

		MSG m_msg;
	private:
		int __declspec(dllexport) Loop();

	
	private:

		UINT m_nLastFrameMsg = 0;
		BOOL m_bLastFrameLuaAppHandled = FALSE;

	private:
		HANDLE m_hWaiter;
		int SetWaiterTimer(float fFrame);
		int CreateWaiterTimer();
		int DestroyWaiterTimer();

	private:
		struct LinkInfo
		{
			const char* cszLinkPath;
			bool bDir;
		};
		static LinkInfo ms_arrLinkInfo[];

protected:
	};

	extern VirtualClient* g_spTopClient;
}