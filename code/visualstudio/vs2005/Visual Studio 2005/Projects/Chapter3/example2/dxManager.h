#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

class dxManager
{
public:
	dxManager(void);
	~dxManager(void);
	bool init(HWND hwnd);
	void shutdown(void);
	void beginRender(void);
	void endRender(void);
	IDirect3DSurface9* getSurfaceFromBitmap(std::string filename);

	IDirect3DSurface9* getBackBuffer(void);
	void blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect);

public:
	HRESULT                 lastResult;
	LPDIRECT3D9             pD3D;
	LPDIRECT3DDEVICE9       pd3dDevice;
};
