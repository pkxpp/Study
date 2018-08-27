#ifndef _KG3DANIFILEWRAP_H
#define _KG3DANIFILEWRAP_H
#include "KParser.h"

#define  ANIMATION_NONE              0
#define  ANIMATION_VERTICES          1
#define  ANIMATION_VERTICES_RELATIVE 2
#define  ANIMATION_BONE              10
#define  ANIMATION_BONE_RELATIVE     11

#define ANIMATION_BONE_RTS    13//12 has been used as low precision matrix
#define ANIMATION_BONE_KEYONLY 14

const DWORD ANI_FILE_MASK = 0x414E494D;
const DWORD ANI_FILE_MASK_VERVION2 = 0x324E494D;
const DWORD ANI_FILE_END_FLAG = 0xFFFFFFFF;

enum enuSimpleRTSFlag
{
	NONE = 0,
	ROTATION = 1 ,
	TRANSLATION = 1 << 1,
	AFFINESCALE= 1 << 2,
	SCALE = 1 << 3,
	SROTATION = 1 << 4,
	SIGN = 1 << 5

};

enum enuNewRTSFlag
{
	_NONE = 0,
	_ONLY_ROTATION = 1,
	_ONLY_ROTATION_TRANSLATION = 2,
	_ONLY_ROTATION_TRANSLATION_AFFINESCALE = 3,

	_ROTATION = 1 << 2,
	_TRANSLATION = 1 << 3,
	_AFFINESCALE = 1 << 4,
	_SCALE = 1 << 5,
	_SROTATION = 1 << 6,
	_SIGN = 1 << 7
};

enum
{
	MAX_CLIPDESC_LEN = 30,
	MAX_BONENAME_LEN = 30,
};

struct RTSINFO
{
	KVec3 Translation;
	KVec3 Scale;
	KQuat Rotation;
	float Sign;
	KQuat SRotation;
};

struct NEWRTSINFO
{
	BYTE byFlag;
	KVec3 Translation;
	KVec3 Scale;
	KQuat Rotation;
	KQuat SRotation;
};

struct COMPRESSROTATION
{
	SHORT wzyx[4];
};

struct BONETRACK
{
	COMPRESSROTATION *pRotation;
	KVec3	*pTranslation;
	float *pfAffineScale;
	KVec3 *pScale;
	COMPRESSROTATION *pSRotation;
	BONETRACK();
	~BONETRACK();
};

#pragma pack(push,1)
//文件存储结构
struct ANI_FILE_HEADER
{
	DWORD32 dwMask;
	DWORD32 dwBlockLength;
	DWORD32 dwNumAnimations;
	DWORD32 dwType;
	TCHAR strDesc[MAX_CLIPDESC_LEN];
};

struct BONERTS_ANI_HEAD
{
	DWORD32 dwNumBones;
	DWORD32 dwNumFrames;
	float fFrameLength;
};

struct VERTEX_ANI_HEAD
{
	DWORD32 dwNumVertex;
	DWORD32 dwNumAnimatedVertex;
	DWORD32 dwNumFrames;
	float fFrameLength;
};

struct NEW_BONERTS_ANI_HEAD
{
	DWORD32 dwNumBones;
	DWORD32 dwRealAniBones;
	DWORD32 dwNumFrames;
	float fFrameLength;
	BOOL  bHasBoneName;
	DWORD32 dwBoneMapMD5;
};

struct NEW_VERTEX_ANI_HEAD
{
	DWORD32 dwNumVertex;
	DWORD32 dwNumAnimatedVertex;
	DWORD32 dwNumFrames;
	float fFrameLength;
	DWORD32 dwRealAnimatedVertex;
};
#pragma pack(pop)

struct BONERTS_ANI_DATA
{
	ANI_FILE_HEADER fileHeader;
	BONERTS_ANI_HEAD boneRTSAniHead;
	TCHAR** ppszBoneName;
	RTSINFO** ppBoneRTS;
	DWORD32 dwEndFlag;
	BONERTS_ANI_DATA();
	~BONERTS_ANI_DATA();
	void Clear();
};

struct VERTEX_ANI_DATA
{
	ANI_FILE_HEADER fileHeader;
	VERTEX_ANI_HEAD vertexAniHead;
	DWORD32* pVertexIndex;
	KVec3* pAnimatedVertexPosition;
	KVec3* pUV;
	DWORD32 dwEndFlag;
	VERTEX_ANI_DATA();
	~VERTEX_ANI_DATA();
	void Clear();
};

struct NEW_BONERTS_ANI_DATA
{
	ANI_FILE_HEADER fileHeader;
	NEW_BONERTS_ANI_HEAD newBoneRTSAniHead;
	TCHAR** ppszBoneName;
	NEWRTSINFO* pInitBoneRTS;
	int* pBoneHasAnimationIndex; 
	BYTE* pRealAniBoneFlag;
	BONETRACK* pBoneTrack;
	DWORD32 dwEndFlag;
	NEW_BONERTS_ANI_DATA();
	~NEW_BONERTS_ANI_DATA();
	void Clear();
};

struct NEW_VERTEX_ANI_DATA
{
	ANI_FILE_HEADER fileHeader;
	NEW_VERTEX_ANI_HEAD newVertexAniHead;
	DWORD32* pVertexIndex;
	DWORD32* pRealVertexIndex;
	KVec3* pInitVertexPosition;
	KVec3* pAnimatedVertexPosition;
	KVec3* pUV;
	DWORD32 dwEndFlag;
	NEW_VERTEX_ANI_DATA();
	~NEW_VERTEX_ANI_DATA();
	void Clear();
};

//此类读入Ani文件或者buffer, 转换格式后得到新的buffer

typedef long HRESULT;

class KG3DAniFileWrap
{
public:
	KG3DAniFileWrap();
	~KG3DAniFileWrap();
public:
	HRESULT ConvertAniData(BYTE* pbyAniData);
	HRESULT GetConvertedAniData(BYTE** ppbyAniData);
	HRESULT SaveToFile(const TCHAR cszFileName[]);
private:
	void UnInit();
	HRESULT ApplyBoneAniChange();
	HRESULT ApplyVertexAniChange();
	DWORD32 _ComputeBoneMapHash(DWORD32 dwNumBone, TCHAR** ppszBoneName);
public:
	static HRESULT OldRTS2NewRTS(NEWRTSINFO& newRts, RTSINFO& oldRts,DWORD32 oldFlag); 
	static HRESULT ComputerAniFlag(DWORD32 &aniFlag, DWORD32 &iniFlag, RTSINFO* pRts, DWORD32 nNumFrame);
private:
	BYTE* m_pbyAniData;
	BYTE* m_pbyConvertedAniData;
};

#endif