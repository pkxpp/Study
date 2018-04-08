/****************************************************************
* example2
* shows the user how to setup a windowed directx application
* which clears the window to a blue color
****************************************************************/
#include <windows.h>
#include <tchar.h>
// include directx9
#include <d3d9.h>

// global variables
HINSTANCE hInst;							// application instance
HWND wndHandle;								// application window handle

LPDIRECT3D9             pD3D;				// the Direct3D Object
LPDIRECT3DDEVICE9       pd3dDevice;			// the Direct3D Device

////////////////////////////////////////////// forward declarations
bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// DirectX functions
bool initDirect3D();	
void render(void);

/*********************************************************************
* WinMain
*********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow)
{
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, _T("Unable to create window"), _T("ERROR"), MB_OK);
		return false;
	}

	if (!initDirect3D())
	{
		MessageBox(NULL, _T("Unable to init Direct3D"), L"ERROR", MB_OK);
		return false;
	}

	// Main message loop:
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }		
		else
		{
			render();
		} 
    }

	// release the device and the direct3D object
	if( pd3dDevice != NULL) 
        pd3dDevice->Release();

    if( pD3D != NULL)
        pD3D->Release();
		
	return (int) msg.wParam;
}

/*********************************************************************
* initWindow
*********************************************************************/
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
	wcex.lpszClassName	= _T("DirectXExample");
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	// create the window
	wndHandle = CreateWindow(_T("DirectXExample"), 
							 _T("DirectXExample"), 
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

/*********************************************************************
* WndProc
*********************************************************************/
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

/*********************************************************************
* initDirect3D
* initializes direct3D
*********************************************************************/
bool initDirect3D()
{
	pD3D = NULL;
	pd3dDevice = NULL;

	// create the directX object
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return false;
	}

	// fill the presentation parameters structure
	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferWidth  = 640;
	d3dpp.hDeviceWindow    = wndHandle;

	// create a default directx device
    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wndHandle,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &pd3dDevice ) ) )
    {
        return false;
    }
	return true;
}

/*********************************************************************
* render
*********************************************************************/
void render(void)
{
	// check to make sure we have a valid Direct3D Device
	if( NULL == pd3dDevice )
        return;

    // Clear the backbuffer to a blue color
    pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	// Present the backbuffer contents to the display
    pd3dDevice->Present( NULL, NULL, NULL, NULL );
}