
#include <Windows.h>
#include "stdio.h"
#include<memory>
#include "BlankDemo.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmd, int nShow)
{
	WNDCLASSEX cls;
	//WNDCLASS cls;
	cls.cbSize = sizeof(cls);
	cls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	cls.hCursor = LoadCursor(NULL, IDC_ARROW);
	cls.style = CS_HREDRAW | CS_VREDRAW;
	cls.lpfnWndProc = WndProc;
	cls.hInstance = hInstance;
	cls.lpszMenuName = NULL;
	cls.lpszClassName = "DX11BookWindowClass";
	cls.cbClsExtra=0;
	cls.cbWndExtra=0;
	cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// 因为缺了这个，一开始RegisterClassEx是失败的，返回87（）
	cls.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&cls)) 
	{
		char szErr[32] = {0};
		sprintf_s(szErr, sizeof(szErr), "[Error No] %d\n", GetLastError());
		MessageBox(NULL, szErr, "", 0);
		return -1;
	}
		
	RECT rc = {0, 0, 640, 480};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindow(cls.lpszClassName, "Blank Win32 Window",
		WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		0, 0, hInstance, 0);
	if(!hwnd) return -1;
	ShowWindow(hwnd, nShow);

	std::auto_ptr<Dx11DemoBase> demo(new BlankDemo());
	//Demo Initialize
	bool result = demo->Initialize( hInstance, hwnd );
	if( result == false )
		return -1;

	MSG msg = {0};
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Update and Draw
		demo->Update( 0.0f );
		demo->Render( );

	}
	//Demo Shutdown
	demo->Shutdown( );

	return static_cast<int>(msg.wParam);
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	switch(message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}