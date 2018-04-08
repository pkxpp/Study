#include <windows.h>
#include "dxManager.h"

HINSTANCE hInst;
HWND wndHandle;

bool initWindow(HINSTANCE hInstance);
void Render(void);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int letterWidth=48;
int letterHeight=48;
int destx = 48;
int desty = 96;

char *message = "HELLO WORLD";
IDirect3DSurface9* srcSurface;
dxManager *dxMgr = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	if (!initWindow(hInstance))
		return false;

	dxMgr = new dxManager();
	dxMgr->init(wndHandle);

	srcSurface = dxMgr->getSurfaceFromBitmap("./letters.bmp");
	if (srcSurface == NULL)
	{
		MessageBox(NULL, "Make sure the bitmap letters.bmp is in the current directory", "ERROR", MB_OK);
		return 0;
	}

	// Main message loop:
	// Enter the message loop
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
		// check for messages
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		// this is called when no messages are pending
		else
		{
			// call our render function
			dxMgr->beginRender();
			
			Render();

			dxMgr->endRender();
		}
    }
	
	// shutdown the directx manager
	dxMgr->shutdown();

	return (int) msg.wParam;
}

// Render
// This function does the actual drawing of the letters to the screen
// look into the dxManager beginRender and endRender functions for more information
// as to what needs to be done each frame
void Render(void)
{
	int letterWidth=48; // the uniform width of each letter block
	int letterHeight=48; // the uniform height of each letter block
	int destx = 48; // the top-left X coordinate for the first letter
	int desty = 96; // the top-left Y coordinate for the first letter

	// This variable will hold the pointer to the back buffer
	IDirect3DSurface9* backbuffer = NULL;
	
	// Check to make sure you have a valid Direct3D device
	if( NULL == dxMgr->pd3dDevice )
		return;

	// Set up a counter variable to hold the letter’s position on the screen
	int count=0;

	// Loop through the message one character at a time
	for ( char *c = message; c != " "; c++ )
	{
		// source and destination rectangles
		RECT src;
		RECT dest;

		// Set the source rectangle
		int srcY = ( ( ( *c - 'A' ) / 6 ) ) * letterHeight;
		int srcX = ( ( ( *c - 'A' ) %7 ) * letterWidth );
		src.top = srcY ;
		src.left = srcX;
		src.right = src.left + letterWidth;
		src.bottom = src.top + letterHeight;

		// Set the dest rectangle
		dest.top = desty;
		dest.left = destx + ( letterWidth * count );
		dest.right = dest.left + letterWidth;
		dest.bottom = dest.top + letterHeight;

		// Increase the letter count by one
		count++;

		// Copy this letter to the back buffer
		dxMgr->blitToSurface( srcSurface, // the source surface
								 &src,        // the source rectangle
								 &dest);       // destination rectangle
	}
}

bool initWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "DirectXExample";
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	wndHandle = CreateWindow("DirectXExample", 
							 "DirectXExample", 
							 WS_OVERLAPPEDWINDOW,
							 CW_USEDEFAULT, 
							 CW_USEDEFAULT, 
							 640, 
							 480, 
							 NULL, 
							 NULL, 
							 hInstance, 
							 NULL);
   if (!wndHandle)
      return false;
   
   ShowWindow(wndHandle, SW_SHOW);
   UpdateWindow(wndHandle);

   return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
