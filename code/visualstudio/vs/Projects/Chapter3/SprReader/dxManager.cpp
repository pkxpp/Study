#include ".\dxmanager.h"

dxManager::dxManager(void)
{
	pD3D = NULL;
 	pd3dDevice = NULL;
}

dxManager::~dxManager(void)
{
}

bool dxManager::init(HWND hwnd)
{
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		lastResult = E_FAIL;
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferWidth  = 640;
	d3dpp.hDeviceWindow    = hwnd;

    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &pd3dDevice ) ) )
    {
		lastResult = E_FAIL;
        return false;
    }

	return true;
}

void dxManager::shutdown(void)
{
    if( pd3dDevice != NULL) 
	{
        pd3dDevice->Release();
		pd3dDevice = NULL;
	}
    if( pD3D != NULL)
	{
        pD3D->Release();
		pD3D = NULL;
	}
}

void dxManager::beginRender()
{
	if( NULL == pd3dDevice )
        return;

    // Clear the backbuffer to a blue color
    pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );
}

void dxManager::endRender(void)
{
	// Present the backbuffer contents to the display
    pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

IDirect3DSurface9* dxManager::getSurfaceFromBitmap(std::string filename)
{
	HRESULT hResult;
	IDirect3DSurface9* surface = NULL;
	D3DXIMAGE_INFO imageInfo;

	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(filename.c_str(), &imageInfo);
	if FAILED (hResult)
		return NULL;


	hResult = pd3dDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (FAILED(hResult))
		return NULL;

	hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return NULL;

	return surface;
}

IDirect3DSurface9* dxManager::getSurfaceFromFile(const char* filename, RECT *rect)
{
	HRESULT hResult;
	IDirect3DSurface9* surface = NULL;
	D3DXIMAGE_INFO imageInfo;

//	MessageBox(NULL, _T("0"), _T("ERROR"), MB_OK);
	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(filename, &imageInfo);
	if FAILED (hResult)
		return NULL;
	
//	char str[20];
//	itoa(imageInfo.Width, str, 10);
//	MessageBox(NULL, str, "ERROR", MB_OK);
//	itoa(imageInfo.Height, str, 10);
//	MessageBox(NULL, str, "ERROR", MB_OK);
	rect->bottom = imageInfo.Height;
	rect->right = imageInfo.Width;

	hResult = pd3dDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (FAILED(hResult))
		return NULL;

	hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filename, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return NULL;

	return surface;
}


IDirect3DSurface9* dxManager::getBackBuffer(void)
{
	IDirect3DSurface9* backbuffer = NULL;

	if (!pd3dDevice)
		return NULL;

	HRESULT hResult = pd3dDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (FAILED(hResult))
		return NULL;
	else
		return backbuffer;
}

//void dxManager::blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect)
//{
//	pd3dDevice->StretchRect(srcSurface, srcRect, getBackBuffer(), destRect, D3DTEXF_NONE);
//}

void dxManager::blitToSurface()
{
		 // Read spr file to get some infomation
		FILE* pF = NULL;
//		pF = fopen(strFilePath.GetBuffer(), "rb");
		pF = fopen("·ÉÐÐôÂôÄ.spr", "rb");
		
		m_uSprDataSize = filelength(fileno(pF));
		m_pSprData = new BYTE[m_uSprDataSize];
		if (m_pSprData == NULL)
		{
			fclose(pF);
			MessageBox(NULL, _T("new Error"), _T("ERROR"), MB_OK);
		}
		if (fread(m_pSprData, m_uSprDataSize, 1, pF) == 1)
		{
			fclose(pF);
			m_pSprHead = (FSprHead*)m_pSprData;
			if ((LPBYTE)m_pSprHead + sizeof(FSprHead) > (LPBYTE)m_pSprData + m_uSprDataSize)
			{
				MessageBox(NULL, _T("ERROR!"), _T("ERROR"), MB_OK);
			}
			if (*(int*)&m_pSprHead->Comment[0] != SPR_COMMENT_FLAG || !m_pSprHead->Frames || m_pSprHead->FrameFormat != enumFF_LinkPng)
			{
				return;
			}

			m_ppSprFrameList = new FSprFrame*[m_pSprHead->Frames];
			if (m_ppSprFrameList == NULL)
			{
//				return false;
			}
			LPBYTE pSprite = (LPBYTE)((LPBYTE)m_pSprHead + sizeof(FSprHead));
			for (int i = 0; i < m_pSprHead->Frames; i++)
			{
//				m_ppSprFrameList[i] = (FSprFrame*)(pSprite + m_pSprOffsList[i].Offset);
				//To do...
			}
		}
		m_nFrame = m_pSprHead->Frames;


	rect.left = 200;
	rect.top = 200;
	rect.bottom = 200 + m_pSprHead->Height;
	rect.right = 200 + m_pSprHead->Width;	

//	if (!pd3dDevice)
//		return NULL;

	HRESULT hResult = pd3dDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
//	if (FAILED(hResult))
//		return NULL;
//	else
//		return backbuffer;

	char *filename = "·ÉÐÐôÂôÄ";
	char cszFrame[16];
	for(int i =0; i < m_nFrame; i++){
		hResult = pd3dDevice->CreateOffscreenPlainSurface(m_pSprHead->Width, m_pSprHead->Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
		sprintf(cszFrame, "%s.%d.png", filename, i);
//		bkgrd = getSurfaceFromFile(cszFrame, &rect);

		hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, cszFrame, NULL, D3DX_DEFAULT, 0, NULL);
		hResult = pd3dDevice->StretchRect(surface, NULL, backbuffer, &rect, D3DTEXF_NONE);
		pd3dDevice->Present( NULL, NULL, NULL, NULL );
		Sleep(100);
	}
}