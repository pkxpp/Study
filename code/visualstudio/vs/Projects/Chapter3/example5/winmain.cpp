#include <windows.h>
#include "dxManager.h"

HINSTANCE hInst;				// holds the instance for this app
HWND wndHandle;					// global window handle

// timer stuff
LARGE_INTEGER timeStart;
LARGE_INTEGER timeEnd;
LARGE_INTEGER timerFreq;
float         anim_rate;

// this holds the DirectX Manager
dxManager *dxMgr;
// this is the surface that will hold the sprites
IDirect3DSurface9* spriteSurface;

#define MAX_SPRITES 4			// this is the number of sprites we want
#define MAX_LOCX (640 - 64)		// don't want the fish to go off the screen
#define MAX_LOCY (480 - 64)

// this is the sprite structure
struct {
	RECT srcRect;				// holds the location of this sprite
								// in the source bitmap
	float posX;					// the sprites X position
	float posY;					// the sprites Y position

	// movement
	float moveX;
	float moveY;

	// animation
	int numFrames;				// number of frames this animation has
	int curFrame;				// the current frame of animation
} spriteStruct[MAX_SPRITES];

////////////////////////////////////////////// forward declarations
bool    initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// sprite functions
bool initSprites(void);
void drawSprite(int i);

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

	QueryPerformanceFrequency(&timerFreq);

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
			QueryPerformanceCounter(&timeStart);

			dxMgr->beginRender();

			for (int i=0; i < MAX_SPRITES; i++)
			{	
				// make the sprite animation through the frames
				if (spriteStruct[i].curFrame < spriteStruct[i].numFrames - 1)
					spriteStruct[i].curFrame++;
				else
					spriteStruct[i].curFrame = 0;
				
				// set the source rect to the correct frame position
				spriteStruct[i].srcRect.left = spriteStruct[i].curFrame * 64;
				spriteStruct[i].srcRect.right = spriteStruct[i].srcRect.left + 64;

				// move the sprite by it's movement rate
				spriteStruct[i].posX += spriteStruct[i].moveX * anim_rate;
				
				// make sure that this sprite is not going off the screen
				if ((spriteStruct[i].posX > MAX_LOCX) || (spriteStruct[i].posX < 0))
					spriteStruct[i].moveX *= -1.0;
				
				// set the destination for this sprite
				RECT destRect;
				destRect.left = int(spriteStruct[i].posX);
				destRect.top = int(spriteStruct[i].posY);
				destRect.bottom = destRect.top + 23;
				destRect.right = destRect.left + 64;

				// draw the sprite to the back buffer
				dxMgr->blitToSurface(spriteSurface, &spriteStruct[i].srcRect, &destRect);
			}
			dxMgr->endRender();

			QueryPerformanceCounter(&timeEnd);

			anim_rate = ( (float)timeEnd.QuadPart - (float)timeStart.QuadPart ) / timerFreq.QuadPart;
		}
    }
	
	// shutdown the directX Manager
	dxMgr->shutdown();

	return (int) msg.wParam;
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


bool initSprites(void)
{
	spriteSurface = dxMgr->getSurfaceFromBitmap("fishes.bmp", 256, 23);
	if (spriteSurface == NULL)
		return false;

	for (int i=0; i < MAX_SPRITES; i++)
	{
		spriteStruct[i].srcRect.top = 0;
		spriteStruct[i].srcRect.left = i * 64;
		spriteStruct[i].srcRect.right = spriteStruct[i].srcRect.left + 64;
		spriteStruct[i].srcRect.bottom = 23;
		spriteStruct[i].posX = float(rand()%600);
		spriteStruct[i].posY = float(rand()%430);

		// set the animation data
		spriteStruct[i].curFrame = 0;
		spriteStruct[i].numFrames = 4;

		// set the move data
		spriteStruct[i].moveX = 30.0;
		spriteStruct[i].moveY = 0.0;
	}

	return true;
}

