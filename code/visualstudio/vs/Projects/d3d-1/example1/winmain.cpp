// Include the windows header file that's needed for all Windows applications
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

HINSTANCE hInstance;	// global handle to hold the application instance
HWND wndHandle;			// global variable to hold the window handle

// forward declarations
bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// This is winmain, the main entry point for Windows applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Initialize the window
	if( !initWindow(hInstance))
		return false;

	// main message loop;
	MSG msg;
	ZeroMemory( &msg, sizeof( msg));
	while( msg.message != WM_QUIT)
	{
		// Check the message queue
		while (GetMessage(&msg, wndHandle, 0, 0))
		{
			TranslateMessage( &msg);
			DispatchMessage( &msg);
		}
	}
	return (int) msg.wParam;
}


/*
* bool initWindow( HINSTANCE hInstance)
* initWindow registers the window class for the application, creates the window
*/
bool initWindow( HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	// Fill in the WNDCLASSEX structure, This describes how the window
	// will look to the system
	wcex.cbSize			= sizeof(WNDCLASSEX);		// the size of the structure
	wcex.style			= CS_HREDRAW | CS_VREDRAW;	// the class style
	wcex.lpfnWndProc	= (WNDPROC)WndProc;			// the window procedure callback
	wcex.cbClsExtra		= 0;						// extra bytes to allocate for this class
	wcex.cbWndExtra		= 0;						// extra bytes to allocate for this instance
	wcex.hInstance		= hInstance;				// handle to the application instance
	wcex.hIcon			= 0;						// icon to associate with the application
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);// the default cursor
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);	// the background color
	wcex.lpszMenuName	= NULL;						// the resource name for the menu
	wcex.lpszClassName	= _T("DirectXExample");			// the class name being created
	wcex.hIconSm		= 0;						// the handle to the small icon
	RegisterClassEx(&wcex);

	// Create the window
	wndHandle = CreateWindow(
		_T("DirectXExample"),		// the window class to use
		_T("DirectXExample"),		// the title bar text
		WS_OVERLAPPEDWINDOW,		// the window style
		CW_USEDEFAULT,			// the starting x coordinate
		CW_USEDEFAULT,			// the starting y coordinate
		640,					// the pixel width of the window
		480,					// the pixel height of the window
		NULL,					// the parent window; NULL for desktop
		NULL,					// the menu for the application; NULL for none
		hInstance,				// the handle to the application instance
		NULL);					// no values passed to the window
	// Make sure that the window handle that is created is valid
	if(!wndHandle)
		return false;

	//Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	return true;
}
			

/*
*LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
* THe window procedure
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	switch(message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}