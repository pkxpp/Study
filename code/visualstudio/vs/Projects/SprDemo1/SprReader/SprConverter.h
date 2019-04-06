#ifndef __SprConverter_h__
#define __SprConverter_h__

#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <assert.h>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
using namespace std;

#ifndef SAFE_FREE
#define SAFE_FREE(a)	if (a) {free(a); (a)=NULL;}
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(a)	if (a) {(a)->Release(); (a)=NULL;}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(a)	if (a) {delete (a); (a)=NULL;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a)	if (a) {delete[] (a); (a)=NULL;}
#endif
#define	SPR_COMMENT_FLAG				0x525053	//'SPR'


extern IDirect3DDevice9*	g_pd3dDevice;


struct FSprHead
{
	BYTE	Comment[4];	// 注释文字(SPR\0)
	WORD	Width;		// 图片宽度
	WORD	Height;		// 图片高度
	WORD	CenterX;	// 重心的水平位移
	WORD	CenterY;	// 重心的垂直位移
	WORD	Frames;		// 总帧数
	WORD	Colors;		// 颜色数
	WORD	Directions;	// 方向数
	WORD	Interval;	// 每帧间隔（以游戏帧为单位）
	WORD	FrameFormat;// 对每帧数据的链接类型
	WORD	Reserved[5];// 保留字段（到以后使用）
};

struct FSprPal
{
	BYTE		Red;		// 红色分量
	BYTE		Green;		// 绿色分量
	BYTE		Blue;		// 兰色分量
};

struct FSprOffs
{
	DWORD	Offset;		// 每一帧的偏移
	DWORD	Length;		// 每一帧的长度
};

struct FSprFrame
{
	WORD	Width;		// 帧最小宽度, 最小什么意思？
	WORD	Height;		// 帧最小高度
	WORD	OffsetX;	// 水平位移（相对于原图左上角）
	WORD	OffsetY;	// 垂直位移（相对于原图左上角）
	BYTE	Sprite[1];	// RLE压缩图形数据，什么样的概念？
};

enum EFrameFormat
{
	enumFF_Default = 0,

	enumFF_OffsPng = 1,
	enumFF_OffsTga,
	enumFF_OffsDds,

	enumFF_LinkPng = 11,
	enumFF_LinkTga,
	enumFF_LinkDds,
};

class SprConverter
{
public:
	SprConverter();
	SprConverter::SprConverter(IDirect3DDevice9*	g_pd3dDevice);
	~SprConverter();
	bool Converter(const char* pszFileName);
	
	bool CreateNewSprFile(const char* pszSprName, const unsigned nFrames);	
private:
	bool LoadSprFile(const char* pszFileName);
	bool ExportNewFormatFile(const char* pszFileName);
	bool SaveNewSprFile(const char* pszSprName);
	bool SaveDdsFile(const char* pszSprName, int nFrame, IDirect3DTexture9* pTexture);
	void Release();
	
	
private:
	IDirect3DDevice9*	g_pd3dDevice;
// Spr文件结构  |FSprHead|FSprPal*nColors|FSprOffs*nFrame|(FSprFrame+(data))*nFrame|
	LPBYTE	m_pSprData;
	DWORD	m_uSprDataSize;

	FSprHead*	m_pSprHead;
	FSprPal*	m_pSprPaList;
	FSprOffs*	m_pSprOffsList;

	FSprFrame**	m_ppSprFrameList;
};





#endif /*__SprConverter_h__*/
