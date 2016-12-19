#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>
#include "SprConverter.h"
#include <tchar.h>

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
	IDirect3DSurface9* getSurfaceFromFile(const char* filename, RECT *rect);

	IDirect3DSurface9* getBackBuffer(void);
//	void blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect);
	void blitToSurface();

public:
	HRESULT                 lastResult;
	LPDIRECT3D9             pD3D;
	LPDIRECT3DDEVICE9       pd3dDevice;

	LPBYTE	m_pSprData;
	DWORD	m_uSprDataSize;
	FSprHead*	m_pSprHead;
//	FSprPal*	m_pSprPaList;			// for old
//	FSprOffs*	m_pSprOffsList;			// for old
	FSprFrame**	m_ppSprFrameList;
	unsigned	m_nFrame;				// 图像帧

	//帧数据相关成员变量

	//
	IDirect3DSurface9* surface;
	IDirect3DSurface9** m_ppSurface;
	IDirect3DSurface9* backbuffer;
	IDirect3DSurface9** m_ppBackbuffer;
	HRESULT hResult;
	RECT rect;
};
