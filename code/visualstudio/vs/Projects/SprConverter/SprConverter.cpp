#include "SprConverter.h"


inline void RenderToA8R8G8B8(BYTE* pDest, int nPitch, BYTE* pSrc, int width, int height, FSprPal* pPalette)
{
	int nLineAdd = nPitch - width * sizeof(DWORD);

	BYTE nColorIndex = 0;
	BYTE nAlphaLen = 0;
	BYTE nAlpha = 0;
	int nDrawWidth = 0;

	for (int i = 0; i < height; i++)
	{
		nDrawWidth = 0;
		while(nDrawWidth < width)
		{
			nAlphaLen = *pSrc++;
			nAlpha = *pSrc++;
			nDrawWidth += nAlphaLen;
			if (nAlpha)
			{
				while(nAlphaLen--)
				{
					nColorIndex = *pSrc++;
					pDest[0] = pPalette[nColorIndex].Blue;
					pDest[1] = pPalette[nColorIndex].Green;
					pDest[2] = pPalette[nColorIndex].Red;
					pDest[3] = nAlpha;
					pDest += sizeof(DWORD);
				}
			}
			else
			{
				ZeroMemory(pDest, nAlphaLen * sizeof(DWORD));
				pDest += nAlphaLen * sizeof(DWORD);
			}			
		}
		assert(nDrawWidth == width);
		ZeroMemory(pDest, nLineAdd * sizeof(DWORD));
		pDest += nLineAdd * sizeof(DWORD);
	}
}

SprConverter::SprConverter()
{
	m_uSprDataSize = 0;
	m_pSprData = NULL;
	m_pSprHead = NULL;
	m_pSprPaList = NULL;
	m_pSprOffsList = NULL;
	m_ppSprFrameList = NULL;
}

SprConverter::~SprConverter()
{
	Release();
}

bool SprConverter::Converter(const char* pszFileName)
{
	if (LoadSprFile(pszFileName))
	{
		if (ExportNewFormatFile(pszFileName))
		{
			return true;
		}
	}
	return false;
}

bool SprConverter::LoadSprFile( const char* pszFileName )
{
	Release();
	if(pszFileName == NULL)
		return false;

	FILE* pF = NULL;
	pF = fopen(pszFileName, "rb");
	if (pF == NULL)
		return false;

	m_uSprDataSize = filelength(fileno(pF));
	m_pSprData = new BYTE[m_uSprDataSize];
	if (m_pSprData == NULL)
	{
		fclose(pF);
		return false;
	}
	if (fread(m_pSprData, m_uSprDataSize, 1, pF) == 1)
	{
		fclose(pF);
		m_pSprHead = (FSprHead*)m_pSprData;
		if ((LPBYTE)m_pSprHead + sizeof(FSprHead) > (LPBYTE)m_pSprData + m_uSprDataSize)
		{
			return false;
		}
		if (*(int*)&m_pSprHead->Comment[0] != SPR_COMMENT_FLAG || m_pSprHead->Colors > 256 || !m_pSprHead->Frames || m_pSprHead->FrameFormat != enumFF_Default)
		{
			return false;
		}
		m_pSprPaList = (FSprPal*)((LPBYTE)m_pSprHead + sizeof(FSprHead));
		if ((LPBYTE)m_pSprPaList + sizeof(FSprPal) * m_pSprHead->Colors > (LPBYTE)m_pSprData + m_uSprDataSize)
		{
			return false;
		}
		m_pSprOffsList = (FSprOffs*)((LPBYTE)m_pSprPaList + sizeof(FSprPal) * m_pSprHead->Colors);
		if ((LPBYTE)m_pSprOffsList + sizeof(FSprOffs) * m_pSprHead->Frames > (LPBYTE)m_pSprData + m_uSprDataSize)
		{
			return false;
		}
		m_ppSprFrameList = new FSprFrame*[m_pSprHead->Frames];
		if (m_ppSprFrameList == NULL)
		{
			return false;
		}
		LPBYTE pSprite = (LPBYTE)m_pSprOffsList + sizeof(FSprOffs) * m_pSprHead->Frames;
		for (int i = 0; i < m_pSprHead->Frames; i++)
		{
			m_ppSprFrameList[i] = (FSprFrame*)(pSprite + m_pSprOffsList[i].Offset);
			//检查长度是否溢出
			//To do...
		}
	}
	else
	{
		fclose(pF);
		return false;
	}
	return true;
}


void SprConverter::Release()
{
	SAFE_DELETE_ARRAY( m_pSprData );
	SAFE_DELETE_ARRAY( m_ppSprFrameList );
	m_pSprHead = NULL;
	m_pSprPaList = NULL;
	m_pSprOffsList = NULL;
	m_uSprDataSize = 0;
}

bool SprConverter::ExportNewFormatFile( const char* pszFileName )
{
	if (!SaveNewSprFile(pszFileName))
	{
		return false;
	}
	for (int  i = 0; i < m_pSprHead->Frames; i++)
	{
		BYTE* pSprExchangeBuffer = NULL;
		pSprExchangeBuffer = new BYTE[m_ppSprFrameList[i]->Width * m_ppSprFrameList[i]->Height * 4]; /* 4? Byte */
		RenderToA8R8G8B8(pSprExchangeBuffer,
			m_ppSprFrameList[i]->Width * sizeof(DWORD),
			m_ppSprFrameList[i]->Sprite,
			m_ppSprFrameList[i]->Width,
			m_ppSprFrameList[i]->Height,
			m_pSprPaList);
		IDirect3DTexture9* pTexture = NULL;
		if (!FAILED(D3DXCreateTexture(g_pd3dDevice, 
			m_ppSprFrameList[i]->Width, 
			m_ppSprFrameList[i]->Height, 
			1, 
			0, 
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED, 
			&pTexture)))
		{
			IDirect3DSurface9* pSurface = NULL;
			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = m_ppSprFrameList[i]->Width;
			rect.bottom = m_ppSprFrameList[i]->Height;
			pTexture->GetSurfaceLevel(0, &pSurface);
 			D3DXLoadSurfaceFromMemory(pSurface, NULL, NULL, pSprExchangeBuffer,
 				D3DFMT_A8R8G8B8, m_ppSprFrameList[i]->Width * sizeof(DWORD), NULL, &rect,D3DX_DEFAULT, 0);
			SaveDdsFile(pszFileName, i, pTexture);
			pSurface->Release();
			pTexture->Release();
			SAFE_DELETE_ARRAY( pSprExchangeBuffer );
		}
		else
		{
			//Log
		}
	}
	return true;
}

bool SprConverter::SaveNewSprFile( const char* pszSprName )
{
	FILE* pFile = fopen(pszSprName,"wb");
	if(!pFile)
		return false;
	FSprHead Head = *m_pSprHead;
	Head.FrameFormat = enumFF_LinkPng;
	fwrite(&Head,sizeof(FSprHead),1,pFile);
	for(int i = 0; i<m_pSprHead->Frames; i++)
	{
		FSprFrame sFrame = *m_ppSprFrameList[i];
		sFrame.Sprite[0] = 0;
		fwrite(&sFrame,sizeof(FSprFrame),1,pFile);
	}
	fclose(pFile);
	return true;
}

bool SprConverter::SaveDdsFile( const char* pszSprName, int nFrame, IDirect3DTexture9* pTexture )
{
	if (pszSprName == NULL || pTexture == NULL)
	{
		return false;
	}
	string szDDS = pszSprName;
	szDDS.resize(szDDS.size() - 4);
	char cszFrame[16];
	sprintf(cszFrame, ".%d.png", nFrame);
	szDDS +=  cszFrame;
	D3DXSaveTextureToFileA(szDDS.c_str(), D3DXIFF_PNG, pTexture, NULL);
	return true;
}

//将名字为为pszSprName.0.png到pszSprName.nFrames.png转化成新的spr格式，文件名为pszSprName
//成功返回True，否则返回False
bool SprConverter::CreateNewSprFile(const char* pszSprName, const unsigned nFrames){
	//检查文件是否存在

	/* create new file name is "pszSprName.spr" */
	string sNew = pszSprName;
	sNew += ".spr";
//	printf("new name: %s\n", sNew.c_str());
	/* open new file */
	FILE* pFile = fopen(sNew.c_str(),"wb");
	if(!pFile)
		return false;

	/* Spr Head */
	FSprHead Head = {0};
	FSprFrame sFrame;
	Head.FrameFormat = enumFF_LinkPng;
	Head.Comment[0] = 'S';
	Head.Comment[1] = 'P';
	Head.Comment[2] = 'R';
	Head.Comment[3] = 0;

	Head.Frames = nFrames;	/* number of Frames */
	Head.Directions = 1;	/* 方向数 */
	Head.Interval = 1;		/* 每帧间隔 */

//	fwrite(&Head,sizeof(FSprHead),1,pFile);

	char cszFrame[16];
	for(int i = 0; i < nFrames; i++){
		sprintf(cszFrame, "%s.%d.png", pszSprName, i);
		printf("name: %s\n", cszFrame);

		D3DXIMAGE_INFO pSrcInfo;
		if( D3DXGetImageInfoFromFile(cszFrame, &pSrcInfo) != D3D_OK ) {
			printf("read image failed!\n");
			return false;
		}
		printf("w: %u, h: %u, format: %d\n", pSrcInfo.Width, pSrcInfo.Height, pSrcInfo.Format);

		/* 填充 */
		sFrame.Height = pSrcInfo.Height;
		sFrame.Width = pSrcInfo.Width;
		sFrame.OffsetX = sFrame.OffsetY = 0;
		sFrame.Sprite[0] = 0;

		/* 写文件头部 */
		Head.Width = pSrcInfo.Width;
		Head.Height = pSrcInfo.Height;
		if(i == 0)
			fwrite(&Head,sizeof(FSprHead),1,pFile);

		fwrite(&sFrame,sizeof(FSprFrame),1,pFile);
	}

	/* 文件指针回到文件开始，重新写spr文件头部 */
//	getchar();
	fclose(pFile);
	return true;
}