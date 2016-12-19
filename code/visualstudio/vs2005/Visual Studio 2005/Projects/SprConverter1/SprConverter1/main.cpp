#include "SprConverter.h"

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();

IDirect3DDevice9*	g_pd3dDevice = NULL;


//--------------------------------------------------------------------------------------
IDirect3DDevice9* CreateNULLRefDevice()
{
	HRESULT hr;
	IDirect3D9* pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( NULL == pD3D )
		return NULL;

	D3DDISPLAYMODE Mode;
	pD3D->GetAdapterDisplayMode(0, &Mode);

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory( &pp, sizeof(D3DPRESENT_PARAMETERS) ); 
	pp.BackBufferWidth  = 1;
	pp.BackBufferHeight = 1;
	pp.BackBufferFormat = Mode.Format;
	pp.BackBufferCount  = 1;
	pp.SwapEffect       = D3DSWAPEFFECT_COPY;
	pp.Windowed         = TRUE;

	IDirect3DDevice9* pd3dDevice;
	hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, GetConsoleWindow(), 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &pd3dDevice );
	SAFE_RELEASE( pD3D );
	if( FAILED(hr) || pd3dDevice == NULL )
		return NULL;

	return pd3dDevice;
}


int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		g_pd3dDevice = CreateNULLRefDevice();
		SprConverter spr;

		/* 将argv[2]转换成数字 */
		//
		printf("%d", aoti(argv[2]));
//		spr.Converter(argv[1], argv[2]);
		spr.CreateNewSprFile(argv[1], atoi(argv[2]));
		getchar();
	}
	return 0;
}


