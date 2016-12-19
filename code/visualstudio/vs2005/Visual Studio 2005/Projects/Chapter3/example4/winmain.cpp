#include <windows.h>
#include "dxManager.h"

HINSTANCE hInst;				// holds the instance for this app
HWND wndHandle;					// global window handle

// this holds the DirectX Manager
dxManager *dxMgr;
// this is the surface that will hold the sprites
IDirect3DSurface9* spriteSurface;

#define MAX_SPRITES 10			// this is the number of sprites we want
#define SCRN_WIDTH 640			// the width of the screen
#define SCRN_HEIGHT 480			// the height of the screen
#define SPRITE_WIDTH 64			// the width of each sprite
#define SPRITE_HEIGHT 23		// the height of each sprite
// this is the sprite structure
struct {
	RECT srcRect;				// holds the location of this sprite
								// in the source bitmap
	int posX;					// the sprites X position
	int posY;					// the sprites Y position

	// movement
	int moveX;
	int moveY;

	// animation
	int numFrames;				// number of frames this animation has
	int curFrame;				// the current frame of animation
} spriteStruct[MAX_SPRITES];

////////////////////////////////////////////// forward declarations
bool    initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// sprite functions
bool initSprites(void);
void Render(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// call our function to init and create our window
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, "Unable to create window", "ERROR", MB_OK);
		return false;
	}

	// create the directx manager
	dxMgr = new dxManager();

	// init the directx manager
	if (!dxMgr->init(wndHandle))
	{
		MessageBox(NULL, "Unable to Init the DirectX Manager", "ERROR", MB_OK);
		return false;
	}

	// initialise the sprites
	if (!initSprites())
	{
		MessageBox(NULL, "Unable to Init the Sprites", "ERROR", MB_OK);
		return false;
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
			dxMgr->beginRender();

			Render();
			
			dxMgr->endRender();
		}
    }
	
	// shutdown the directX Manager
	dxMgr->shutdown();

	return (int) msg.wParam;
}


/*****************************************************************************
* Render(void)
*****************************************************************************/
void Render(void)
{
	// This holds the back buffer
	IDirect3DSurface9* backbuffer = dxMgr->getBackBuffer();

	// Loop through all the sprite structures
	for ( int i = 0; i < MAX_SPRITES; i++ )
	{
		// Increment the sprite animation frame
		if (spriteStruct[ i ].curFrame < spriteStruct[ i ].numFrames - 1)
			spriteStruct[ i ].curFrame++;
		else
			// You have reached the last frame; reset to first frame
			spriteStruct[ i ].curFrame = 0;

		// Set the source rectangle to the correct frame position
		spriteStruct[ i ].srcRect.left = spriteStruct[ i ].curFrame * 64;
		spriteStruct[ i ].srcRect.right = spriteStruct[ i ].srcRect.left + 64;

		// make sure the sprite stays within the screen area
		spriteStruct[ i ].posX += spriteStruct[ i ].moveX;
		if ((spriteStruct[ i ].posX > SCRN_WIDTH - SPRITE_WIDTH) || (spriteStruct[ i ].posX < 0))
			spriteStruct[ i ].moveX *= -1;
		
		// Create a temporary destination RECT
		RECT destRect;
		// Fill the temporary RECT with data
		destRect.left = spriteStruct[i].posX;
		// from the current sprite structure
		destRect.top = spriteStruct[i].posY;
		// The fish sprite is 23 pixels tall
		destRect.bottom = destRect.top + SPRITE_HEIGHT;
		// The fish sprite is 64 pixels wide
		destRect.right = destRect.left + SPRITE_WIDTH;

		// Draw the sprite to the back buffer
		dxMgr->blitToSurface(spriteSurface, &spriteStruct[i].srcRect, &destRect);
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
							 SCRN_WIDTH, 
							 SCRN_HEIGHT, 
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


bool initSprites(void)
{
	spriteSurface = dxMgr->getSurfaceFromBitmap("fishes.bmp");
	if (spriteSurface == NULL)
		return false;

	for (int i=0; i < MAX_SPRITES; i++)
	{
		spriteStruct[i].srcRect.top = 0;
		spriteStruct[i].srcRect.left = i * SPRITE_WIDTH;
		spriteStruct[i].srcRect.right = spriteStruct[i].srcRect.left + SPRITE_WIDTH;
		spriteStruct[i].srcRect.bottom = SPRITE_HEIGHT;
		spriteStruct[i].posX = rand()% SCRN_WIDTH - SPRITE_WIDTH;
		spriteStruct[i].posY = rand()% SCRN_HEIGHT - SPRITE_HEIGHT;

		// set the animation data
		spriteStruct[i].curFrame = 0;
		spriteStruct[i].numFrames = 4;

		// set the move data
		spriteStruct[i].moveX = 5;
		spriteStruct[i].moveY = 0;
	}

	return true;
}
