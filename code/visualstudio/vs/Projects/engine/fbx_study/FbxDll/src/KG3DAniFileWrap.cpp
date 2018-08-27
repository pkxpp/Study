#include "StdAfx.h"
#include "KG3DAniFileWrap.h"


#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


unsigned int	g_FileNameHash(const char* pString)
{
	unsigned int Id = 0;
	char c = 0;
	for (int i = 0; pString[i]; i++)
	{
		c = pString[i];
		if (c >= 'A' && c <= 'Z')
			c += 0x20;	//中文字符的后字节亦可能被转换，人为地加大了重码的概率■
		else if (c == '/')
			c = '\\';
		Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
	}
	return (Id ^ 0x12345678);
}


KQuat * QuaternionConjugate  ( KQuat *pOut, CONST KQuat *pQ )  
{  
	pOut->x = -pQ->x;  
	pOut->y = -pQ->y;  
	pOut->z = -pQ->z;  
	pOut->w =  pQ->w;  
	return pOut;  
}  


KQuat* QuaternionIdentity( KQuat *pOut )  
{  
	pOut->x = pOut->y = pOut->z = 0.0f;  
	pOut->w = 1.0f;  
	return pOut;  
}  


void DecompressQuaternion(COMPRESSROTATION *pCR, KQuat *pRetQ)
{
	float PRECISION = 1.0f / SHRT_MAX;

	assert(pCR);
	assert(pRetQ);

	pRetQ->x = pCR->wzyx[0] * PRECISION;
	pRetQ->y = pCR->wzyx[1] * PRECISION;
	pRetQ->z = pCR->wzyx[2] * PRECISION;
	pRetQ->w = pCR->wzyx[3] * PRECISION;

	return;
}

void CompressQuaternion(KQuat *q, COMPRESSROTATION *pCR)
{
	assert(q);
	assert(pCR);

	//D3DXQuaternionNormalize(q, q); dx的数学库函数换成max的(dx,max的yz相反)
	Quat maxQuat(q->x, q->z, q->y, q->w);
	maxQuat.Normalize();
	*q = KQuat(maxQuat.x, maxQuat.z, maxQuat.y, maxQuat.w);

	pCR->wzyx[0] = (SHORT)(q->x * SHRT_MAX);
	pCR->wzyx[1] = (SHORT)(q->y * SHRT_MAX);
	pCR->wzyx[2] = (SHORT)(q->z * SHRT_MAX);
	pCR->wzyx[3] = (SHORT)(q->w * SHRT_MAX);

	return;
}

BOOL IsVectorEqual(const KVec3& first, const KVec3& second)
{	
	if (
		abs(first.x - second.x) > 0.001 || abs(first.y - second.y) > 0.001 || abs(first.z - second.z) > 0.001)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL IsVectorEqualHigh(const KVec3& first, const KVec3& second)
{	
	if (abs(first.x - second.x) >0.00000005 || abs(first.y - second.y) > 0.00000005 || abs(first.z - second.z)> 0.00000005)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL IsVector3AllAxisEqual(const KVec3& vIn)
{
	return abs(vIn.x - vIn.y) < 0.001  && abs(vIn.x - vIn.z) < 0.001;
}

BOOL IsQuatZero(KQuat &quat)
{
	return quat.x * quat.x + quat.y * quat.y + quat.z * quat.z < 0.00000000001 || (abs(quat.w) > 0.99999 && abs(quat.w) < 1.000001);
}

DWORD32 SimpleRTSFlagToNewRTSFlag(DWORD32 flag)
{
	if (
		(flag & ROTATION) &&
		!(flag & ~ROTATION)
		)
	{
		return _ONLY_ROTATION;
	}

	if (
		(flag & ROTATION) &&
		(flag & TRANSLATION) &&
		!(flag & ~(ROTATION | TRANSLATION))
		)
	{
		return _ONLY_ROTATION_TRANSLATION;
	}

	if (
		(flag & ROTATION) && 
		(flag & TRANSLATION) &&
		(flag & AFFINESCALE) &&
		!(flag & ~(ROTATION | TRANSLATION | AFFINESCALE))
		)
	{
		return _ONLY_ROTATION_TRANSLATION_AFFINESCALE;
	}

	DWORD32 enuRet = _NONE;

	if (flag & ROTATION)
	{
		enuRet |= _ROTATION;
	}

	if (flag & TRANSLATION)
	{
		enuRet |= _TRANSLATION;
	}

	if (flag & SIGN)
	{
		enuRet |= _SIGN;
	}

	if (flag & AFFINESCALE)
	{
		enuRet |= _AFFINESCALE;
	}
	else
	{
		if (flag & SCALE)
		{
			enuRet |= _SCALE;
			if (flag & SROTATION)
			{
				enuRet |= _SROTATION;
			}
		}
	}

	return enuRet;
}

BONERTS_ANI_DATA::BONERTS_ANI_DATA()
{
	Clear();
}

BONERTS_ANI_DATA::~BONERTS_ANI_DATA()
{
	if (ppszBoneName)
	{
		for (DWORD32 i = 0; i < boneRTSAniHead.dwNumBones; ++i)
		{
			SAFE_DELETE_ARRAY(ppszBoneName[i]);
		}
		SAFE_DELETE_ARRAY(ppszBoneName);
	}
	
	if (ppBoneRTS)
	{
		for (DWORD32 i = 0; i < boneRTSAniHead.dwNumBones; ++i)
		{
			SAFE_DELETE_ARRAY(ppBoneRTS[i]);
		}
		SAFE_DELETE_ARRAY(ppBoneRTS);
	}
}

void BONERTS_ANI_DATA::Clear()
{
	ppszBoneName = NULL;
	ppBoneRTS = NULL;
}

VERTEX_ANI_DATA::VERTEX_ANI_DATA()
{
	Clear();
}

VERTEX_ANI_DATA::~VERTEX_ANI_DATA()
{
	SAFE_DELETE_ARRAY(pVertexIndex);
	SAFE_DELETE_ARRAY(pAnimatedVertexPosition);
	SAFE_DELETE_ARRAY(pUV);
}

void VERTEX_ANI_DATA::Clear()
{
	pVertexIndex = NULL;
	pAnimatedVertexPosition = NULL;
	pUV = NULL;
}

NEW_BONERTS_ANI_DATA::NEW_BONERTS_ANI_DATA()
{
	Clear();
}

NEW_BONERTS_ANI_DATA::~NEW_BONERTS_ANI_DATA()
{
	if (ppszBoneName)
	{
		for (DWORD32 i = 0; i < newBoneRTSAniHead.dwNumBones; ++i)
		{
			SAFE_DELETE_ARRAY(ppszBoneName[i]);
		}
		SAFE_DELETE_ARRAY(ppszBoneName);
	}
	
	SAFE_DELETE_ARRAY(pInitBoneRTS);
	SAFE_DELETE_ARRAY(pBoneHasAnimationIndex);
	SAFE_DELETE_ARRAY(pRealAniBoneFlag);
	SAFE_DELETE_ARRAY(pBoneTrack);
}

void NEW_BONERTS_ANI_DATA::Clear()
{
	ppszBoneName = NULL;
	pInitBoneRTS = NULL;
	pBoneHasAnimationIndex = NULL;
	pRealAniBoneFlag = NULL;
	pBoneTrack = NULL;
}

NEW_VERTEX_ANI_DATA::NEW_VERTEX_ANI_DATA()
{
	Clear();
}

NEW_VERTEX_ANI_DATA::~NEW_VERTEX_ANI_DATA()
{
	SAFE_DELETE_ARRAY(pVertexIndex);
	SAFE_DELETE_ARRAY(pRealVertexIndex);
	SAFE_DELETE_ARRAY(pInitVertexPosition);
	SAFE_DELETE_ARRAY(pAnimatedVertexPosition);
	SAFE_DELETE_ARRAY(pUV);
}

void NEW_VERTEX_ANI_DATA::Clear()
{
	pVertexIndex = NULL;
	pRealVertexIndex = NULL;
	pInitVertexPosition = NULL;
	pAnimatedVertexPosition = NULL;
	pUV = NULL;
}

BONETRACK::BONETRACK() :
pfAffineScale(NULL),
pRotation(NULL),
pTranslation(NULL),
pScale(NULL),
pSRotation(NULL)
{
}

BONETRACK::~BONETRACK()
{
	SAFE_DELETE_ARRAY(pfAffineScale);
	SAFE_DELETE_ARRAY(pRotation);
	SAFE_DELETE_ARRAY(pTranslation);
	SAFE_DELETE_ARRAY(pScale);
	SAFE_DELETE_ARRAY(pSRotation);
}

KG3DAniFileWrap::KG3DAniFileWrap()
{
	m_pbyConvertedAniData = NULL;
	m_pbyAniData = NULL;
}

KG3DAniFileWrap::~KG3DAniFileWrap()
{
	UnInit();
}

void KG3DAniFileWrap::UnInit()
{
	ANI_FILE_HEADER* pFileHeader = (ANI_FILE_HEADER*)m_pbyConvertedAniData;
	NEW_BONERTS_ANI_DATA* pBoneRTSData = NULL;
	NEW_VERTEX_ANI_DATA* pVertexData = NULL;

	KG_PROCESS_ERROR(pFileHeader);
	switch(pFileHeader->dwType)
	{
	case ANIMATION_BONE_RTS:
		pBoneRTSData = (NEW_BONERTS_ANI_DATA *)m_pbyConvertedAniData;
		SAFE_DELETE(pBoneRTSData);
		break;
	case ANIMATION_VERTICES:
		pVertexData = (NEW_VERTEX_ANI_DATA *)m_pbyConvertedAniData;
		SAFE_DELETE(pVertexData);
		break;
	default:
		break;
	}
Exit0:
	m_pbyAniData = NULL;
	m_pbyConvertedAniData = NULL;
}

HRESULT KG3DAniFileWrap::ConvertAniData(BYTE* pbyAniData)
{
	HRESULT hrResult = E_FAIL;
	HRESULT hrRetCode = E_FAIL;
	ANI_FILE_HEADER* pAniHeader;

	UnInit();
	KGLOG_PROCESS_ERROR(pbyAniData);
	
	pAniHeader = (ANI_FILE_HEADER*)pbyAniData;
	KGLOG_PROCESS_ERROR(pAniHeader->dwMask == ANI_FILE_MASK);

	m_pbyAniData = pbyAniData;
	switch(pAniHeader->dwType)
	{
	case ANIMATION_BONE_RTS:
		hrRetCode = ApplyBoneAniChange();
		break;
	case ANIMATION_VERTICES:
		hrRetCode = ApplyVertexAniChange();
		break;
	default:
		break;
	}
	hrResult = S_OK;

Exit0:
	return hrResult;
}

HRESULT KG3DAniFileWrap::GetConvertedAniData(BYTE** ppbyAniData)
{
	HRESULT hrResult = E_FAIL;

	KG_PROCESS_ERROR(ppbyAniData);
	*ppbyAniData = m_pbyConvertedAniData;
Exit0:
	return hrResult;
}

HRESULT KG3DAniFileWrap::SaveToFile(const TCHAR cszFileName[])
{
	HRESULT hrResult = E_FAIL;
	FILE* pFile = NULL;
	size_t uSize = 0;
	ANI_FILE_HEADER* pFileHeader = NULL;
	NEW_VERTEX_ANI_DATA* pVertexAniData = NULL;
	NEW_BONERTS_ANI_DATA* pBoneRTSAniData = NULL;

	KGLOG_PROCESS_ERROR(cszFileName);		

	pFile = fopen(cszFileName, "wb");
	KGLOG_PROCESS_ERROR(pFile);

	pFileHeader = (ANI_FILE_HEADER *)m_pbyConvertedAniData;
	KGLOG_PROCESS_ERROR(pFileHeader->dwMask == ANI_FILE_MASK_VERVION2);
	switch(pFileHeader->dwType)
	{
	case ANIMATION_BONE_RTS:
		pBoneRTSAniData = (NEW_BONERTS_ANI_DATA*)pFileHeader;

		uSize = fwrite(&(pBoneRTSAniData->fileHeader), sizeof(ANI_FILE_HEADER), 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		uSize = fwrite(&(pBoneRTSAniData->newBoneRTSAniHead), sizeof(NEW_BONERTS_ANI_HEAD), 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		for (DWORD32 i = 0; i < pBoneRTSAniData->newBoneRTSAniHead.dwNumBones; ++i)
		{
			uSize = fwrite(pBoneRTSAniData->ppszBoneName[i], sizeof(TCHAR) * MAX_CLIPDESC_LEN, 1, pFile);
			KGLOG_PROCESS_ERROR(uSize == 1);
		}

		uSize = fwrite(pBoneRTSAniData->pInitBoneRTS, sizeof(NEWRTSINFO) * pBoneRTSAniData->newBoneRTSAniHead.dwNumBones, 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		uSize = fwrite(pBoneRTSAniData->pBoneHasAnimationIndex, sizeof(int) * pBoneRTSAniData->newBoneRTSAniHead.dwNumBones, 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		if (pBoneRTSAniData->newBoneRTSAniHead.dwRealAniBones)
		{
			BYTE* pRealAniBoneFlag = pBoneRTSAniData->pRealAniBoneFlag;
			uSize = fwrite(pRealAniBoneFlag, sizeof(BYTE) * pBoneRTSAniData->newBoneRTSAniHead.dwRealAniBones, 1, pFile);
			KGLOG_PROCESS_ERROR(uSize == 1);

			for (DWORD32 i = 0; i < pBoneRTSAniData->newBoneRTSAniHead.dwRealAniBones; ++i)
			{
				DWORD32 flag = pRealAniBoneFlag[i];
				DWORD32 dwNumFrames = pBoneRTSAniData->newBoneRTSAniHead.dwNumFrames;
				if (flag == _ONLY_ROTATION 
					|| flag == _ONLY_ROTATION_TRANSLATION 
					|| flag == _ONLY_ROTATION_TRANSLATION_AFFINESCALE
					|| (flag & _ROTATION))
				{
					uSize = fwrite(pBoneRTSAniData->pBoneTrack[i].pRotation,sizeof(COMPRESSROTATION)*dwNumFrames,1,pFile);
					KGLOG_PROCESS_ERROR(uSize == 1);
				}

				if (flag == _ONLY_ROTATION_TRANSLATION 
					|| flag == _ONLY_ROTATION_TRANSLATION_AFFINESCALE
					||(flag & _TRANSLATION))
				{
					uSize = fwrite(pBoneRTSAniData->pBoneTrack[i].pTranslation,sizeof(KVec3)*dwNumFrames,1,pFile);
					KGLOG_PROCESS_ERROR(uSize == 1);
				}

				if (flag == _ONLY_ROTATION_TRANSLATION_AFFINESCALE
					|| (flag & _AFFINESCALE))
				{
					uSize = fwrite(pBoneRTSAniData->pBoneTrack[i].pfAffineScale,sizeof(float) * dwNumFrames,1,pFile);
					KGLOG_PROCESS_ERROR(uSize == 1);
				}
				else if (flag & _SCALE)
				{
					uSize = fwrite(pBoneRTSAniData->pBoneTrack[i].pScale,sizeof(KVec3)*dwNumFrames,1,pFile);
					KGLOG_PROCESS_ERROR(uSize == 1);

					if (flag & _SROTATION)
					{
						uSize = fwrite(pBoneRTSAniData->pBoneTrack[i].pSRotation,sizeof(COMPRESSROTATION)*dwNumFrames,1,pFile);
						KGLOG_PROCESS_ERROR(uSize == 1);
					}
				}
			}
		}	
		uSize = fwrite(&pBoneRTSAniData->dwEndFlag, sizeof(DWORD32), 1, pFile);//EndFlag
		KGLOG_PROCESS_ERROR(uSize == 1);
		break;
	case ANIMATION_VERTICES:
		uSize = fwrite(pFileHeader, sizeof(ANI_FILE_HEADER), 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		pVertexAniData = (NEW_VERTEX_ANI_DATA*)pFileHeader;
		uSize = fwrite(&(pVertexAniData->newVertexAniHead), sizeof(NEW_VERTEX_ANI_HEAD), 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		uSize = fwrite(pVertexAniData->pVertexIndex, sizeof(DWORD32) * pVertexAniData->newVertexAniHead.dwNumAnimatedVertex, 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		uSize = fwrite(pVertexAniData->pRealVertexIndex, sizeof(DWORD32) * pVertexAniData->newVertexAniHead.dwNumAnimatedVertex, 1, pFile);
		KGLOG_PROCESS_ERROR(uSize == 1);

		if (pVertexAniData->newVertexAniHead.dwRealAnimatedVertex < pVertexAniData->newVertexAniHead.dwNumAnimatedVertex)
		{
			DWORD32 dwNumStaticVertex = pVertexAniData->newVertexAniHead.dwNumAnimatedVertex - pVertexAniData->newVertexAniHead.dwRealAnimatedVertex;
			uSize = fwrite(pVertexAniData->pInitVertexPosition, sizeof(KVec3) * dwNumStaticVertex, 1, pFile);
			KGLOG_PROCESS_ERROR(uSize == 1);
		}

		if (pVertexAniData->newVertexAniHead.dwRealAnimatedVertex > 0)
		{
			uSize = fwrite(pVertexAniData->pAnimatedVertexPosition, sizeof(KVec3) * pVertexAniData->newVertexAniHead.dwRealAnimatedVertex * pVertexAniData->newVertexAniHead.dwNumFrames, 1, pFile);
			KGLOG_PROCESS_ERROR(uSize == 1);
		}

		if (pVertexAniData->pUV)
		{
			uSize = fwrite(pVertexAniData->pUV, sizeof(KVec3) * pVertexAniData->newVertexAniHead.dwNumAnimatedVertex * pVertexAniData->newVertexAniHead.dwNumFrames, 1, pFile);
			KGLOG_PROCESS_ERROR(uSize == 1);
		}

		uSize = fwrite(&(pVertexAniData->dwEndFlag), sizeof(DWORD32), 1, pFile);//EndFlag
		KGLOG_PROCESS_ERROR(uSize == 1);	
		break;
	default:
		break;
	}

	hrResult = S_OK;
Exit0:
	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	return hrResult;
}

HRESULT KG3DAniFileWrap::OldRTS2NewRTS(NEWRTSINFO& newRts, RTSINFO& oldRts,DWORD32 oldFlag)
{
	HRESULT hResult = E_FAIL;

	newRts.byFlag = (BYTE)SimpleRTSFlagToNewRTSFlag(oldFlag);
	if (oldFlag & TRANSLATION)
	{
		newRts.Translation = oldRts.Translation;
	}

	if (oldFlag & ROTATION)
	{
		newRts.Rotation = oldRts.Rotation;
	}

	if (oldFlag & AFFINESCALE)
	{
		newRts.Scale = KVec3(oldRts.Scale.x,oldRts.Scale.x,oldRts.Scale.x);

	}
	else if (oldFlag & SCALE)
	{
		newRts.Scale = oldRts.Scale;
		if (oldFlag & SROTATION)
		{
			newRts.SRotation = oldRts.SRotation;
		}
	}

	hResult = S_OK;
	return hResult;
}

HRESULT KG3DAniFileWrap::ComputerAniFlag(DWORD32 &aniFlag, DWORD32 &iniFlag, RTSINFO* pRts, DWORD32 dwNumFrame)
{
	HRESULT hResult = E_FAIL;
	aniFlag = NONE;
	iniFlag = NONE;

	RTSINFO iniRTS = pRts[0]; 
	QuaternionConjugate(&iniRTS.Rotation,&iniRTS.Rotation);
	QuaternionConjugate(&iniRTS.SRotation,&iniRTS.SRotation);//取逆
	bool bAniScale = false;
	bool noAffineScale = false;
	bool bAniHasSRotaion = false;

	for (DWORD32 i = 0; i < dwNumFrame; i ++)
	{
		if (!IsVectorEqual(iniRTS.Translation, pRts[i].Translation))
		{
			aniFlag |= TRANSLATION;
		}

		// D3DXQuaternionMultiply(&quat,&iniRTS.Rotation,&pRts[i].Rotation); 换成max的数学库(dx,max,yz相反)
		Quat maxQuat;
		Quat maxiniRot(iniRTS.Rotation.x, iniRTS.Rotation.z, iniRTS.Rotation.y, iniRTS.Rotation.w);
		Quat maxRtsRot(pRts[i].Rotation.x, pRts[i].Rotation.z, pRts[i].Rotation.y, pRts[i].Rotation.w);
		maxQuat = maxiniRot * maxRtsRot;
		KQuat quat(maxQuat.x, maxQuat.z, maxQuat.y, maxQuat.w);

		if (!IsQuatZero(quat))
		{
			aniFlag |= ROTATION;
		}

		if (!IsVector3AllAxisEqual(pRts[i].Scale))
		{
			if (!IsVectorEqualHigh(iniRTS.Scale, pRts[i].Scale))
			{
				bAniScale = true;
				noAffineScale = true;
				//D3DXQuaternionMultiply(&deltQuat,&iniRTS.SRotation,&pRts[i].SRotation); 用max数学库(dx,max的yz互反)
				Quat maxQuat;
				Quat maxiniRot(iniRTS.SRotation.x, iniRTS.SRotation.z, iniRTS.SRotation.y, iniRTS.SRotation.w);
				Quat maxRtsRot(pRts[i].SRotation.x, pRts[i].SRotation.z, pRts[i].SRotation.y, pRts[i].SRotation.w);
				maxQuat = maxiniRot * maxRtsRot;
				KQuat deltQuat(maxQuat.x, maxQuat.z, maxQuat.y, maxQuat.w);
				if (!IsQuatZero(deltQuat))
				{
					bAniHasSRotaion = true;
				}
			}

		}
		else if (!IsVectorEqual(iniRTS.Scale, pRts[i].Scale))
		{
			bAniScale = true;
		}
	}

	if(bAniScale && !noAffineScale)
	{
		aniFlag |= AFFINESCALE;
	}

	else if (bAniScale && noAffineScale)
	{
		aniFlag |= SCALE;
		if (bAniHasSRotaion)
		{
			aniFlag |= SROTATION;
		}
	}

	KVec3 tempvec = KVec3(iniRTS.Sign,0.0f,0.0f);
	if (IsVectorEqual(tempvec,KVec3(-1,0,0)))
	{
		iniFlag |= SIGN;
	}
	else if (!IsVectorEqual(tempvec,KVec3(1,0,0)))
	{
		KGLOG_PROCESS_ERROR(FALSE);
	}

	if (!IsVectorEqualHigh(iniRTS.Translation,KVec3(0,0,0))
		||
		(aniFlag & TRANSLATION))
	{
		iniFlag |= TRANSLATION;
	}

	if (!IsQuatZero(iniRTS.Rotation)
		||
		(aniFlag & ROTATION))
	{
		iniFlag |= ROTATION;
	}

	if (
		(!IsVectorEqual(iniRTS.Scale, KVec3(1,1,1)) || bAniScale)
		&&
		IsVector3AllAxisEqual(iniRTS.Scale)
		&&
		!noAffineScale)
	{
		iniFlag |= AFFINESCALE;
	}
	else if (!IsVector3AllAxisEqual(iniRTS.Scale)
		||
		noAffineScale)
	{
		iniFlag |= SCALE;
		if (!IsQuatZero(iniRTS.SRotation) || bAniHasSRotaion)
		{
			iniFlag |= SROTATION;
		}
	}

	if ((iniFlag & AFFINESCALE))
	{
		for (DWORD32 i = 0; i < dwNumFrame; i ++)
		{
			QuaternionIdentity(&pRts[i].SRotation);
		}
	}

	if (
		!(iniFlag & AFFINESCALE)
		&&
		(!(iniFlag & SCALE) || !(iniFlag & SROTATION)))
	{
		for (DWORD32 i = 0; i < dwNumFrame; i ++)
		{
			QuaternionIdentity(&pRts[i].SRotation);
		}
	}
	hResult = S_OK;
Exit0:
	return hResult;
}

DWORD32 KG3DAniFileWrap::_ComputeBoneMapHash(DWORD32 dwNumBone, TCHAR** ppszBoneName)
{
	DWORD32 dwHash = (DWORD32)-1;
	std::string strBoneMapHash;

	for (DWORD32 i = 0; i < dwNumBone; i++)
	{
		strBoneMapHash += ppszBoneName[i];
		char szIndex[20];
		_itoa(i,szIndex,10);
		strBoneMapHash += szIndex;
	}
	KGLOG_PROCESS_ERROR(strBoneMapHash.size());
	dwHash = g_FileNameHash(strBoneMapHash.c_str());
Exit0:
	return dwHash;
}

HRESULT KG3DAniFileWrap::ApplyBoneAniChange()
{
	HRESULT hResult = E_FAIL;
	HRESULT hRetCode = E_FAIL;

	BONERTS_ANI_DATA* pData = (BONERTS_ANI_DATA*)m_pbyAniData;

	DWORD32 dwNumAnimatedBone = pData->boneRTSAniHead.dwNumBones;
	DWORD32 dwNumFrames = pData->boneRTSAniHead.dwNumFrames;
	float fFrameLength = pData->boneRTSAniHead.fFrameLength;
	RTSINFO** ppBoneRTS = pData->ppBoneRTS;
	TCHAR** ppszBoneName = pData->ppszBoneName;

	TCHAR** ppszNewBoneName = NULL;
	NEWRTSINFO* pInitBoneRTS = NULL;
	int* pBoneHasAnimationIndex = NULL;
	BYTE* pRealAniBoneFlag = NULL;
	BONETRACK* pBoneAniTrack = NULL;
	DWORD32 dwNumRealAnimatedBone = 0;
	DWORD32 dwNumStaticBone = 0;
	DWORD32 dwBlockLength = 0;

	std::vector<DWORD32> vecRealAniFlag;
	int curRealAniBoneIndex = 0;
	int curStaticAniBoneIndex = 0;

	KGLOG_PROCESS_ERROR(!m_pbyConvertedAniData);
	m_pbyConvertedAniData = (BYTE *)new NEW_BONERTS_ANI_DATA;
	KGLOG_PROCESS_ERROR(m_pbyConvertedAniData);

	NEW_BONERTS_ANI_DATA* pConvertedData = (NEW_BONERTS_ANI_DATA *)m_pbyConvertedAniData;

	dwBlockLength += sizeof(ANI_FILE_HEADER) + sizeof(NEW_BONERTS_ANI_HEAD);

	ppszNewBoneName = new TCHAR*[dwNumAnimatedBone];
	KGLOG_PROCESS_ERROR(ppszNewBoneName);
	dwBlockLength += sizeof(TCHAR) * dwNumAnimatedBone * MAX_CLIPDESC_LEN;
	for (DWORD32 i = 0; i < dwNumAnimatedBone; i++)
	{
		ppszNewBoneName[i] = new TCHAR[MAX_CLIPDESC_LEN];
		KGLOG_PROCESS_ERROR(ppszNewBoneName[i]);

		memcpy(ppszNewBoneName[i], ppszBoneName[i], sizeof(TCHAR) * (MAX_CLIPDESC_LEN - 1));
		ppszNewBoneName[i][MAX_CLIPDESC_LEN - 1] = 0;
	}

	pInitBoneRTS = new NEWRTSINFO[dwNumAnimatedBone];
	KGLOG_PROCESS_ERROR(pInitBoneRTS);
	dwBlockLength += sizeof(NEWRTSINFO) * dwNumAnimatedBone;

	pBoneHasAnimationIndex = new int[dwNumAnimatedBone];
	KGLOG_PROCESS_ERROR(pBoneHasAnimationIndex);
	dwBlockLength += sizeof(int) * dwNumAnimatedBone;

	for (DWORD32 i = 0; i < dwNumAnimatedBone; i ++)
	{
		for (DWORD32 j= 0; j < dwNumFrames; j++)
		{
			QuaternionConjugate(&ppBoneRTS[i][j].Rotation,&ppBoneRTS[i][j].Rotation);
			QuaternionConjugate(&ppBoneRTS[i][j].SRotation,&ppBoneRTS[i][j].SRotation);
		}
		DWORD32 iniFlag,aniFlag;
		hRetCode = ComputerAniFlag(aniFlag,iniFlag,ppBoneRTS[i],dwNumFrames);
		KGLOG_COM_PROCESS_ERROR(hRetCode);
		if (aniFlag!=NONE)
		{
			pBoneHasAnimationIndex[i] = curRealAniBoneIndex;
			curRealAniBoneIndex++;
			vecRealAniFlag.push_back(aniFlag);
		}
		else
		{
			curStaticAniBoneIndex++;
			pBoneHasAnimationIndex[i] = -curStaticAniBoneIndex;
		}

		hRetCode = OldRTS2NewRTS(pInitBoneRTS[i],ppBoneRTS[i][0],iniFlag);
		KGLOG_COM_PROCESS_ERROR(hRetCode);
	}

	dwNumRealAnimatedBone = curRealAniBoneIndex;
	dwNumStaticBone = curStaticAniBoneIndex;
	KGLOG_PROCESS_ERROR(dwNumStaticBone + dwNumRealAnimatedBone == dwNumAnimatedBone);
//	KG_PROCESS_SUCCESS(dwNumRealAnimatedBone == 0);

	if (dwNumRealAnimatedBone > 0)
	{
		pRealAniBoneFlag = new BYTE[dwNumRealAnimatedBone];
		KGLOG_PROCESS_ERROR(pRealAniBoneFlag);
		dwBlockLength += sizeof(BYTE) * dwNumRealAnimatedBone;
		for (DWORD32 i = 0; i < dwNumRealAnimatedBone; i++)
		{
			pRealAniBoneFlag[i] = (BYTE)SimpleRTSFlagToNewRTSFlag(vecRealAniFlag[i]);
			KGLOG_PROCESS_ERROR(pRealAniBoneFlag[i] != _NONE);
		}

		pBoneAniTrack = new BONETRACK[dwNumRealAnimatedBone];
		KGLOG_PROCESS_ERROR(pBoneAniTrack);

		for (DWORD32 i = 0; i < dwNumAnimatedBone; i++)
		{
			int nRealAniIndex = pBoneHasAnimationIndex[i];
			if (nRealAniIndex >= 0)
			{
				DWORD32 nflag = vecRealAniFlag[nRealAniIndex];
				if (nflag & TRANSLATION)
				{
					pBoneAniTrack[nRealAniIndex].pTranslation = new KVec3[dwNumFrames];
					dwBlockLength += sizeof(KVec3) * dwNumFrames;
					for (DWORD32 j = 0; j < dwNumFrames; j++)
					{
						pBoneAniTrack[nRealAniIndex].pTranslation[j] = ppBoneRTS[i][j].Translation;
					}
				}

				if (nflag & ROTATION)
				{
					pBoneAniTrack[nRealAniIndex].pRotation = new COMPRESSROTATION[dwNumFrames];
					dwBlockLength += sizeof(COMPRESSROTATION) * dwNumFrames;
					for (DWORD32 j = 0; j < dwNumFrames; j++)
					{
						CompressQuaternion(&ppBoneRTS[i][j].Rotation, &pBoneAniTrack[nRealAniIndex].pRotation[j]);
					}
				}

				if (nflag & AFFINESCALE)
				{
					pBoneAniTrack[nRealAniIndex].pfAffineScale = new float[dwNumFrames];
					dwBlockLength += sizeof(float) * dwNumFrames;
					for (DWORD32 j = 0; j < dwNumFrames; j++)
					{
						pBoneAniTrack[nRealAniIndex].pfAffineScale[j] = ppBoneRTS[i][j].Scale.x;
					}
				}
				else if (nflag & SCALE)
				{
					pBoneAniTrack[nRealAniIndex].pScale = new KVec3[dwNumFrames];
					dwBlockLength += sizeof(KVec3) * dwNumFrames;
					for (DWORD32 j = 0; j < dwNumFrames; j++)
					{
						pBoneAniTrack[nRealAniIndex].pScale[j] = ppBoneRTS[i][j].Scale;
					}

					if (nflag & SROTATION)
					{
						pBoneAniTrack[nRealAniIndex].pSRotation = new COMPRESSROTATION[dwNumFrames];
						dwBlockLength += sizeof(COMPRESSROTATION) * dwNumFrames;
						for (DWORD32 j = 0; j < dwNumFrames; j++)
						{
							CompressQuaternion(&ppBoneRTS[i][j].SRotation, &pBoneAniTrack[nRealAniIndex].pSRotation[j]);
						}
					}
				}	
			}
		}
	}
	dwBlockLength += sizeof(DWORD32);//EndFlag
	//FileHeader
	memcpy(&pConvertedData->fileHeader, &pData->fileHeader, sizeof(ANI_FILE_HEADER));
	pConvertedData->fileHeader.dwMask = ANI_FILE_MASK_VERVION2;
	pConvertedData->fileHeader.dwBlockLength = dwBlockLength;
	//NewVertexAniHead
	NEW_BONERTS_ANI_HEAD* pNewHead = &(pConvertedData->newBoneRTSAniHead);
	pNewHead->dwNumBones = dwNumAnimatedBone;
	pNewHead->dwRealAniBones = dwNumRealAnimatedBone;
	pNewHead->dwNumFrames = dwNumFrames;
	pNewHead->fFrameLength = fFrameLength;
	pNewHead->bHasBoneName = TRUE;
	pNewHead->dwBoneMapMD5 = _ComputeBoneMapHash(dwNumAnimatedBone, ppszNewBoneName);
	//Body
	pConvertedData->ppszBoneName = ppszNewBoneName;
	pConvertedData->pInitBoneRTS = pInitBoneRTS;
	pConvertedData->pBoneHasAnimationIndex = pBoneHasAnimationIndex;
	pConvertedData->pRealAniBoneFlag = pRealAniBoneFlag;
	pConvertedData->pBoneTrack = pBoneAniTrack;
	//EndFlag
	pConvertedData->dwEndFlag = ANI_FILE_END_FLAG;
//Exit1:
	hResult = S_OK;
Exit0:
	if (FAILED(hResult))
	{
		if (ppszNewBoneName)
		{
			for (DWORD32 i = 0; i < dwNumAnimatedBone; i++)
				SAFE_DELETE_ARRAY(ppszNewBoneName[i]);
			SAFE_DELETE_ARRAY(ppszNewBoneName);
		}

		SAFE_DELETE_ARRAY(pInitBoneRTS);
		SAFE_DELETE_ARRAY(pBoneHasAnimationIndex);
		SAFE_DELETE_ARRAY(pRealAniBoneFlag);
		SAFE_DELETE_ARRAY(pBoneAniTrack);
	}
	return hResult;
}

HRESULT KG3DAniFileWrap::ApplyVertexAniChange()
{
	HRESULT hrResult = E_FAIL;
	BOOL bEqual = TRUE;

	DWORD32* pNewVertexIndex = NULL;
	DWORD32* pRealVertexIndex = NULL;
	KVec3* pInitVetexPosition = NULL;
	KVec3* pNewAnimatedVertexPos = NULL;
	KVec3* pNewUV = NULL;


	KGLOG_PROCESS_ERROR(!m_pbyConvertedAniData);
	m_pbyConvertedAniData = (BYTE *)new NEW_VERTEX_ANI_DATA;
	KGLOG_PROCESS_ERROR(m_pbyConvertedAniData);

	NEW_VERTEX_ANI_DATA* pConvertedData = (NEW_VERTEX_ANI_DATA *)m_pbyConvertedAniData;
	VERTEX_ANI_DATA* pData = (VERTEX_ANI_DATA*)m_pbyAniData;
	VERTEX_ANI_HEAD* pVertexAniHead = &(pData->vertexAniHead);
	KVec3* pAnimatedVertexPosition = pData->pAnimatedVertexPosition; 
	DWORD32* pVertexIndex = pData->pVertexIndex;
	KVec3* pUV = pData->pUV;

	DWORD32 dwNumRealAnimatedVertex = 0;
	DWORD32 dwNumAnimatedVertex = pVertexAniHead->dwNumAnimatedVertex;
	DWORD32 dwNumFrames = pVertexAniHead->dwNumFrames;


	DWORD32 dwBlockLength = 0;
	dwBlockLength = sizeof(ANI_FILE_HEADER) + sizeof(NEW_VERTEX_ANI_HEAD);

	pNewVertexIndex = new DWORD32[dwNumAnimatedVertex];
	KGLOG_PROCESS_ERROR(pNewVertexIndex);
	memcpy(pNewVertexIndex, pVertexIndex, sizeof(DWORD32) * dwNumAnimatedVertex);
	dwBlockLength += sizeof(DWORD32) * dwNumAnimatedVertex;

	pRealVertexIndex = new DWORD32[dwNumAnimatedVertex];
	KGLOG_PROCESS_ERROR(pRealVertexIndex);
	dwBlockLength += sizeof(DWORD32) * dwNumAnimatedVertex;
	for (DWORD32 i = 0; i < dwNumAnimatedVertex; i++)
	{
		bEqual = TRUE;
		pRealVertexIndex[i] = -1;
		KVec3 &vInitPos = pAnimatedVertexPosition[i * dwNumFrames];
		for (DWORD32 j = 1; j < dwNumFrames; j++)
		{
			KVec3 &vPos = pAnimatedVertexPosition[i * dwNumFrames + j];
			if (!IsVectorEqual(vInitPos, vPos))
			{
				bEqual = FALSE;
				break;
			}
		}
		if (!bEqual)
		{
			pRealVertexIndex[i] = i;
			dwNumRealAnimatedVertex++;
		}
	}

	if (dwNumRealAnimatedVertex < dwNumAnimatedVertex)
	{
		pInitVetexPosition = new KVec3[dwNumAnimatedVertex - dwNumRealAnimatedVertex];
		KGLOG_PROCESS_ERROR(pInitVetexPosition);
		dwBlockLength += sizeof(KVec3) * (dwNumAnimatedVertex - dwNumRealAnimatedVertex);

		for (DWORD32 i = 0, j = 0; i < dwNumAnimatedVertex && j < (dwNumAnimatedVertex - dwNumRealAnimatedVertex); i++)
		{
			if (pRealVertexIndex[i] == -1)
			{
				memcpy(&pInitVetexPosition[j++], &pAnimatedVertexPosition[i * dwNumFrames], sizeof(KVec3));
			}
		}
	}

//	KG_PROCESS_SUCCESS(dwNumRealAnimatedVertex == 0);

	if (dwNumRealAnimatedVertex > 0)
	{
		pNewAnimatedVertexPos = new KVec3[dwNumRealAnimatedVertex * dwNumFrames];
		KGLOG_PROCESS_ERROR(pNewAnimatedVertexPos);
		dwBlockLength += sizeof(KVec3) * dwNumRealAnimatedVertex * dwNumFrames;

		for (DWORD32 i = 0, k = 0; i < dwNumAnimatedVertex && k < dwNumRealAnimatedVertex; i++)
		{
			if (pRealVertexIndex[i] == -1)
				continue;

			for (DWORD32 j = 0; j < dwNumFrames; j++)
			{
				memcpy(
					&pNewAnimatedVertexPos[k * dwNumFrames + j], 
					&pAnimatedVertexPosition[i * dwNumFrames + j], 
					sizeof(KVec3)
					);
			}

			k++;
		}
	}
	if (pUV)
	{
		pNewUV = new KVec3[dwNumAnimatedVertex * dwNumFrames];
		KGLOG_PROCESS_ERROR(pNewUV);
		memcpy(pNewUV, pUV, sizeof(KVec3) * dwNumAnimatedVertex * dwNumFrames);
		dwBlockLength += sizeof(KVec3) * dwNumAnimatedVertex * dwNumFrames;
	}
	dwBlockLength += sizeof(DWORD32);
	//FileHeader
	memcpy(&pConvertedData->fileHeader, &pData->fileHeader, sizeof(ANI_FILE_HEADER));
	pConvertedData->fileHeader.dwMask = ANI_FILE_MASK_VERVION2;
	pConvertedData->fileHeader.dwBlockLength = dwBlockLength;
	//NewVertexAniHead
	NEW_VERTEX_ANI_HEAD* pNewHead = &(pConvertedData->newVertexAniHead);
	pNewHead->dwNumVertex = pVertexAniHead->dwNumVertex;
	pNewHead->dwNumAnimatedVertex = dwNumAnimatedVertex;
	pNewHead->dwNumFrames = dwNumFrames;
	pNewHead->fFrameLength = pVertexAniHead->fFrameLength;
	pNewHead->dwRealAnimatedVertex = dwNumRealAnimatedVertex;
	//Body
	pConvertedData->pVertexIndex = pNewVertexIndex;
	pConvertedData->pRealVertexIndex = pRealVertexIndex;
	pConvertedData->pInitVertexPosition = pInitVetexPosition;
	pConvertedData->pAnimatedVertexPosition = pNewAnimatedVertexPos;
	pConvertedData->pUV = pNewUV;
	//EndFlag
	pConvertedData->dwEndFlag = ANI_FILE_END_FLAG;

//Exit1:
	hrResult = S_OK;
Exit0:
	if (FAILED(hrResult))
	{
		SAFE_DELETE_ARRAY(pNewVertexIndex);
		SAFE_DELETE_ARRAY(pRealVertexIndex);
		SAFE_DELETE_ARRAY(pInitVetexPosition);
		SAFE_DELETE_ARRAY(pNewAnimatedVertexPos);
		SAFE_DELETE_ARRAY(pNewUV);
	}
	return hrResult;
}



































