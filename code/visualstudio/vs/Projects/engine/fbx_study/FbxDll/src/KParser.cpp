/********************************************************************
	created:	2015/09/07   11:36
	filename: 	KParser.cpp		
	author:		LiKan
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "KParser.h"
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <cstddef>
#include <regex>
#include <iostream>
//#include "pub.h"
#include "display/DisplayCommon.h"
//#include "KMaterail.h"
//#include "KMesh.h"
//#include "KG3DAniFileWrap.h"
#include "Common/Common.h"
//#include "KTangent.h"

#define  ANIMATION_NONE              0
#define  ANIMATION_VERTICES          1
#define  ANIMATION_VERTICES_RELATIVE 2
#define  ANIMATION_BONE              10
#define  ANIMATION_BONE_RELATIVE     11
#define ANIMATION_BONE_RTS    13//12 has been used as low precision matrix
#define ANIMATION_BONE_KEYONLY 14

#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif



//
//void Matrix3ToMatrix4(KMatrix *pMat4, Matrix3* pMat3)
//{
//	Point3 Row = pMat3->GetRow(0);
//	pMat4->_11 = Row.x;	pMat4->_12 = Row.y;	pMat4->_13 = Row.z;	pMat4->_14 = 0;
//	Row = pMat3->GetRow(1);
//	pMat4->_21 = Row.x;	pMat4->_22 = Row.y;	pMat4->_23 = Row.z;	pMat4->_24 = 0;
//	Row = pMat3->GetRow(2);
//	pMat4->_31 = Row.x;	pMat4->_32 = Row.y;	pMat4->_33 = Row.z;	pMat4->_34 = 0;
//	Row = pMat3->GetRow(3);
//	pMat4->_41 = Row.x;	pMat4->_42 = Row.y;	pMat4->_43 = Row.z;	pMat4->_44 = 1;
//}
//
//void ToRTS(RTSBoneAnimation &rts, AffineParts &ap)
//{
//	rts.Scale.x = ap.k.x;
//	rts.Scale.y = ap.k.y;
//	rts.Scale.z = ap.k.z;
//
//	rts.Rotation.x = ap.q.x;
//	rts.Rotation.y = ap.q.y;
//	rts.Rotation.z = ap.q.z;
//	rts.Rotation.w = ap.q.w;
//
//	rts.Translation.x = ap.t.x;
//	rts.Translation.y = ap.t.y;
//	rts.Translation.z = ap.t.z;
//
//	rts.Sign = ap.f;
//
//	rts.SRotation.x = ap.u.x;
//	rts.SRotation.y = ap.u.y;
//	rts.SRotation.z = ap.u.z;
//	rts.SRotation.w = ap.u.w;
//}
//
//void ToMaxMat3(Matrix3 &matOut, FbxAMatrix &mat)
//{
//	if (!ExportOption()->SwitchToMaxMat())
//	{	
//		matOut.SetRow(0, Point3(mat.mData[0][0], -mat.mData[0][1], -mat.mData[0][2]));
//		matOut.SetRow(1, Point3(-mat.mData[1][0], mat.mData[1][1], mat.mData[1][2]));
//		matOut.SetRow(2, Point3(-mat.mData[2][0], mat.mData[2][1], mat.mData[2][2]));
//		matOut.SetRow(3, Point3(-mat.mData[3][0], mat.mData[3][1], mat.mData[3][2]));
//	}
//	else
//	{	
//		matOut.SetRow(0, Point3(mat.mData[0][0], -mat.mData[0][2], mat.mData[0][1]));
//		matOut.SetRow(1, Point3(mat.mData[1][0], -mat.mData[1][2], mat.mData[1][1]));
//		matOut.SetRow(2, Point3(mat.mData[2][0], -mat.mData[2][2], mat.mData[2][1]));
//		matOut.SetRow(3, Point3(mat.mData[3][0], -mat.mData[3][2], mat.mData[3][1]));
//	}
//}
//
//void GetGlobalRTS(FbxNode* pNode, KVec3* pTrans, KVec3* pScale, KVec4* pRot, KVec3 vCenter = KVec3(0, 0, 0))
//{
//    if (!pNode)
//        return;
//
//    Matrix3 maxMat;
//    FbxAMatrix mat = pNode->EvaluateGlobalTransform(0);
//    ToMaxMat3(maxMat, mat);
//    AffineParts ap;
//
//    Point3 trans;
//    Point3 scale;
//    Quat quat;
//    decomp_affine(maxMat, &ap);
//
//    if (!ExportOption()->SwitchToMaxMat())
//    {
//        // maya坐标系和DX坐标系是不同左右手坐标系
//        trans.x = -(ap.t.x - vCenter.x);
//        trans.y = ap.t.y - vCenter.y;
//        trans.z = ap.t.z - vCenter.z;
//        if (ExportOption()->fFbxScale)
//        {
//            trans = trans * g_Getmat3Inch2CMScale();
//        }
//    }
//    else
//    {
//        trans.x = ap.t.x - vCenter.x;
//        trans.y = ap.t.z - vCenter.z;
//        trans.z = ap.t.y - vCenter.y;
//        if(ExportOption()->fFbxScale)
//        {
//            trans = trans * g_Getmat3Inch2CMScale();
//        }
//    }
//
//    if (ExportOption()->bMayaFbx)
//    {
//        scale.x = ap.k.x;//
//        scale.y = ap.k.y;
//        scale.z = ap.k.z;
//
//        quat.x = ap.q.x;
//        quat.y = -ap.q.y;
//        quat.z = -ap.q.z;
//        quat.w = ap.q.w;
//    }
//    else
//    {
//        scale.x = ap.k.x;
//        scale.y = ap.k.z;
//        scale.z = ap.k.y;
//
//        quat.x = ap.q.x;
//        quat.y = ap.q.z;
//        quat.z = ap.q.y;
//        quat.w = ap.q.w;
//    }
//
//    if (pTrans)
//    {
//        pTrans->x = trans.x;
//        pTrans->y = trans.y;
//        pTrans->z = trans.z;
//    }
//
//    if (pScale)
//    {
//        pScale->x = scale.x;
//        pScale->y = scale.y;
//        pScale->z = scale.z;
//    }
//
//    if (pRot)
//    {
//        pRot->x = quat.x;
//        pRot->y = quat.y;
//        pRot->z = quat.z;
//        pRot->w = quat.w;
//    }
//
//}
//
//BOOL EndWithStr(const char *pStr, const char *pDest)
//{
//	char *p = strstr((char *)pStr, pDest);
//	int nDestLen = (int) strlen(pDest);
//	int nStrLen = (int) strlen(pStr);
//	if(p && p + nDestLen - pStr == nStrLen)
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
//
void GetNameFromPath(const char* inPath, char *outPath, int outLen)
{
	memset(outPath, 0, outLen);
	int ilen = (int) strlen(inPath);	
	int end = ilen;
	int start = 0;
	int bEndOk = false;
	int bStartOk = false;
	for(int i = ilen; i != -1; --i)
	{
		if(inPath[i] == '.' && !bEndOk)
		{
			end = i;
			bEndOk = true;
		}

		if((inPath[i] == '/' || inPath[i] == '\\') && !bStartOk)
		{
			start = i + 1;
			bStartOk = true;
		}

		if(bStartOk && bEndOk)
		{
			break;
		}
	}

	if(outLen > end - start)
	{
		strncpy(outPath, inPath + start, end - start);
	}
}
//
//void GetFileNameFormPath(const char* inPath, char *outPath, int outLen)
//{
//	memset(outPath, 0, outLen);
//	int ilen = (int) strlen(inPath);	
//	int end = ilen;
//	int start = 0;
//	for(int i = ilen; i != -1; --i)
//	{	
//		if(inPath[i] == '/' || inPath[i] == '\\')
//		{
//			start = i + 1;
//			break;
//		}
//	}
//	if(outLen > end - start)
//	{
//		strncpy(outPath, inPath + start, end - start);
//	}
//}
//
//// 小物件#栏杆#WJ_SCA_002.mesh
//void GetRelativePathFromFileName(const char *szFileName, char *szRelativePath, int outLen)
//{
//	memset(szRelativePath, 0, outLen);
//
//	char szPathItem[256];
//	memset(szPathItem, 0, 256);
//
//	int ilen = (int) strlen(szFileName);	
//	int start = 0;
//	int end = 0;
//
//	for(int i = 0; i != ilen; ++i)
//	{	
//		if(szFileName[i] == '#')
//		{
//			end = i;
//			strncpy(szPathItem, szFileName + start, end - start);
//			sprintf(szRelativePath, "%s%s\\", szRelativePath, szPathItem);
//			start = i + 1;
//		}
//	}
//}
//
//void GetMapsSourcePathFromPath(const char* inPath, char *outPath, int outLen)
//{
//	memset(outPath, 0, outLen);
//	char *p = strstri(inPath, "\\data\\source\\maps_source");
//	if (!p)
//	{
//		strncpy(outPath, inPath, strlen(inPath));
//	}
//	else
//	{
//		int len = p - inPath + strlen("\\data\\source\\maps_source");
//		strncpy(outPath, inPath, len);
//	}
//}
//
//void RemoveRelativePrefix(const char *szFileName, char *szOutName, int outLen)
//{
//	memset(szOutName, 0, outLen);
//
//	int ilen = (int) strlen(szFileName);	
//	int start = 0;
//	int end = ilen;
//
//	for (int i = ilen; i != -1; --i)
//	{
//		if (szFileName[i] == '#')
//		{
//			start = i + 1;
//			break;
//		}
//	}
//
//	if(outLen > end - start)
//	{
//		strncpy(szOutName, szFileName + start, end - start);
//	}
//}
//
//int Make_Dirs(const char *dir)
//{
//	if(NULL == dir || '\0' == *dir)
//        return -1;
//
//	string strDir = dir;
//	char szDir[MAX_PATH];
//
//	ReplaceAll(strDir, "/", "\\");
//	strcpy(szDir, strDir.c_str());
//
//
//    char *p = strrchr(szDir, '\\');
//
//    if(p != NULL)
//    {
//        char parent_dir[4096] = { 0 };
//        strncpy(parent_dir, szDir, p - szDir);
//        if(_access(parent_dir, 0) != 0)
//            Make_Dirs(parent_dir);
//    }
//
//    if(_access(szDir, 0) != 0)
//    {
//        if( _mkdir(szDir) != 0)
//            return -1;
//    }
//
//    return 0;
//}
//
////////////////////////////////////////////////////////////////////////
//// Temporary Save Jsoninspack(template: mtl_std.def)
//void ReplaceAll(string& str,char* oldValue,char* newValue)  
//{  
//	string::size_type pos(0);  
//
//	while(true){  
//		pos=str.find(oldValue,pos);  
//		if (pos!=(string::npos))  
//		{  
//			str.replace(pos,strlen(oldValue),newValue);  
//			pos+=strlen(oldValue);//注意是加2，为了跳到下一个反斜杠  
//		}  
//		else  
//			break;  
//	}  
//} 
//
//vector<WCHAR> wCharBuf;
//vector<char> vecOutStr;
//
//int ConvertGBKToUtf8(const char * pcszStrGBK, char *pOut) 
//{
//	int len=MultiByteToWideChar(CP_ACP, 0, pcszStrGBK, -1, NULL,0);
//	wCharBuf.resize(len + 1);
//	memset(&wCharBuf[0], 0, sizeof(WCHAR)*(len  + 1));
//	MultiByteToWideChar(CP_ACP, 0, pcszStrGBK, -1, &wCharBuf[0], len);
//	len = WideCharToMultiByte(CP_UTF8, 0, &wCharBuf[0], -1, NULL, 0, NULL, NULL); 
//	vecOutStr.resize(len + 1);
//	memset(&vecOutStr[0], 0, len + 1);
//
//	int nRet =  WideCharToMultiByte (CP_UTF8, 0, &wCharBuf[0], -1, &vecOutStr[0], len, NULL,NULL);
//	if(nRet && vecOutStr.size())
//	{
//		strncpy(pOut, &vecOutStr[0], vecOutStr.size());
//	}
//	return nRet;
//}
//
//
//int ConvertUTF8ToGBK(const char * pcszStrUTF8, char *pOut) 
//{
//	int len=MultiByteToWideChar(CP_UTF8, 0, pcszStrUTF8, -1, NULL,0);
//	wCharBuf.resize(len + 1);
//	memset(&wCharBuf[0], 0, sizeof(WCHAR)*(len  + 1));
//	MultiByteToWideChar(CP_UTF8, 0, pcszStrUTF8, -1, &wCharBuf[0], len);
//	len = WideCharToMultiByte(CP_ACP, 0, &wCharBuf[0], -1, NULL, 0, NULL, NULL); 
//	vecOutStr.resize(len + 1);
//	memset(&vecOutStr[0], 0, len + 1);
//
//	int nRet =  WideCharToMultiByte (CP_ACP, 0, &wCharBuf[0], -1, &vecOutStr[0], len, NULL,NULL);
//	if(nRet && vecOutStr.size())
//	{
//		strncpy(pOut, &vecOutStr[0], vecOutStr.size());
//	}
//	return nRet;
//}
//
//void GetParentPath(const char* inPath, char *outPath, int outLen)
//{
//	memset(outPath, 0, outLen);
//	int ilen = (int) strlen(inPath);
//	int end = ilen;
//	int start = 0;
//	
//
//	for(int i = ilen; i != -1; --i)
//	{
//		if(inPath[i] == '/' || inPath[i] == '\\')
//		{
//			end = i;
//			break;
//		}
//	}
//
//	if(outLen > end - start && end > start)
//	{
//		strncpy(outPath, inPath, end);
//	}
//	else
//	{
//		strncpy(outPath, inPath, ilen);
//	}
//}
//
BOOL GetParentPathName(const char* inPath, char *outPath, int outLen)
{
	memset(outPath, 0, outLen);
	int ilen = (int) strlen(inPath);
	int end = ilen;
	int start = 0;
	int nFoundPathSymbol = 0;
	for(int i = ilen; i != -1; --i)
	{
		if(inPath[i] == '/' || inPath[i] == '\\')
		{
			nFoundPathSymbol ++;

			if(nFoundPathSymbol == 1)
			{
				end = i;
			}
			if(nFoundPathSymbol == 2)
			{
				start = i + 1;
				break;
			}
		}
	}

	if(outLen > end - start && end > start)
	{
		strncpy(outPath, inPath + start, end - start);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
bool operator > (const KVec2 &v0, const KVec2 &v1)
{
	if(v0.x > v1.x) return true;
	if(v0.x < v1.y) return false;
	
	if(v0.y > v1.y) return true;
	if(v1.y < v1.y) return false;

	return false;
}

bool operator < (const KVec2 &v0, const KVec2 &v1)
{
	if(v0.x > v1.x) return false;
	if(v0.x < v1.x) return true;

	if(v0.y > v1.y) return false;
	if(v1.y < v1.y) return true;

	return false;
}

bool operator > (const KVec3 &v0, const KVec3 &v1)
{
	if(v0.x > v1.x) return true;
	if(v0.x < v1.x) return false;

	if(v0.y > v1.y) return true;
	if(v1.y < v1.y) return false;	

	if(v0.z > v1.z) return true;
	if(v0.z < v1.z) return false;

	return false;
}

bool operator < (const KVec3 &v0, const KVec3 &v1)
{
	if(v0.x > v1.x) return false;
	if(v0.x < v1.x) return true;

	if(v0.y > v1.y) return false;
	if(v1.y < v1.y) return true;	

	if(v0.z > v1.z) return false;
	if(v0.z < v1.z) return true;

	return false;
}

bool operator > (const KVec4 &v0, const KVec4 &v1)
{
	if(v0.x > v1.x) return true;
	if(v0.x < v1.x) return false;

	if(v0.y > v1.y) return true;
	if(v1.y < v1.y) return false;	

	if(v0.z > v1.z) return true;
	if(v0.z < v1.z) return false;

	if(v0.w > v1.w) return true;
	if(v0.w < v1.w) return false;

	return false;	
}

bool operator < (const KVec4 &v0, const KVec4 &v1)
{
	if(v0.x > v1.x) return false;
	if(v0.x < v1.x) return true;

	if(v0.y > v1.y) return false;
	if(v1.y < v1.y) return true;	

	if(v0.z > v1.z) return false;
	if(v0.z < v1.z) return true;

	if(v0.w > v1.w) return false;
	if(v0.w < v1.w) return true;

	return false;	
}

bool operator > (const KPCNT3 &v0, const KPCNT3 &v1)
{
	if(v0.pos > v1.pos) return true;
	if(v0.pos < v1.pos) return false;

	if(v0.color > v1.color) return true;
	if(v0.color < v1.color) return false;

	if(v0.nomral > v1.nomral) return true;
	if(v0.nomral < v1.nomral) return false;

	if(v0.uvw0 > v1.uvw0) return true;
	if(v0.uvw0 < v1.uvw0) return false;

	if(v0.uvw1 > v1.uvw1) return true;
	if(v0.uvw1 < v1.uvw1) return false;

	if(v0.uvw2 > v1.uvw2) return true;
	if(v0.uvw2 < v1.uvw2) return false;

	if(v0.tangent > v1.tangent) return true;
	if(v0.tangent < v1.tangent) return false;

	return false;
}

bool operator < (const KPCNT3 &v0, const KPCNT3 &v1)
{
	if(v0.pos > v1.pos) return false;
	if(v0.pos < v1.pos) return true;

	if(v0.color > v1.color) return false;
	if(v0.color < v1.color) return true;

	if(v0.nomral > v1.nomral) return false;
	if(v0.nomral < v1.nomral) return true;

	if(v0.uvw0 > v1.uvw0) return false;
	if(v0.uvw0 < v1.uvw0) return true;

	if(v0.uvw1 > v1.uvw1) return false;
	if(v0.uvw1 < v1.uvw1) return true;

	if(v0.uvw2 > v1.uvw2) return false;
	if(v0.uvw2 < v1.uvw2) return true;

	if(v0.tangent > v1.tangent) return false;
	if(v0.tangent < v1.tangent) return true;

	return false;
}
*/

bool operator == (const KVec2 &v0, const KVec2 &v1)
{
	if(v0.x == v1.x && v0.y == v1.y)
	  return true;
	
	return false;
}
bool operator == (const KVec3 &v0, const KVec3 &v1)
{
	if(v0.x == v1.x && v0.y == v1.y && v0.z == v1.z)
	   return true;
	
	 return false;
}
bool operator == (const KVec4 &v0, const KVec4 &v1)
{
	if(v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w == v1.w)
		return true;
	
	return false;
}

bool operator == (const KPCNT3 &v0, const KPCNT3 &v1)
{
	if(v0.pos == v1.pos && v0.color == v1.color && v0.nomral == v1.nomral && v0.uvw0 == v1.uvw0 && v0.uvw1 == v1.uvw1 && v0.uvw2 == v1.uvw2 && v0.tangent == v1.tangent)
		return true;

	return false;
}


bool operator < (const KVec2 &v0, const KVec2 &v1)
{
	if(v0.x < v1.x)
		return true;
	if(v0.x == v1.x && v0.y < v1.y)
		return true;
	
	return false;
}

bool operator < (const KVec3 &v0, const KVec3 &v1)
{
	if(v0.x < v1.x)
		return true;
	if(v0.x == v1.x && v0.y < v1.y)
		return true;
	if(v0.x == v1.x && v0.y == v1.y && v0.z < v1.z)
		return true;	
	return false;
}

bool operator < (const KVec4 &v0, const KVec4 &v1)
{
	if(v0.x < v1.x)
		return true;
	if(v0.x == v1.x && v0.y < v1.y)
		return true;
	if(v0.x == v1.x && v0.y == v1.y && v0.z < v1.z)
		return true;
	if(v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w < v1.w)
		return true;	 
	return false;
	
}

bool operator < (const KPCNT3 &v0, const KPCNT3 &v1)
{
	if(v0.pos < v1.pos)
		return true;
	if(v0.pos == v1.pos && v0.color < v1.color)
		return true;
	if(v0.pos == v1.pos && v0.color == v1.color && v0.nomral < v1.nomral)
		return true;
	if(v0.pos == v1.pos && v0.color == v1.color && v0.nomral == v1.nomral && v0.uvw0 < v1.uvw0)
		return true;
	if(v0.pos == v1.pos && v0.color == v1.color && v0.nomral == v1.nomral && v0.uvw0 == v1.uvw0 && v0.uvw1 < v1.uvw1)
		return true;
	if(v0.pos == v1.pos && v0.color == v1.color && v0.nomral == v1.nomral && v0.uvw0 == v1.uvw0 && v0.uvw1 == v1.uvw1 && v0.uvw2 < v1.uvw2)
		return true;
	if(v0.pos == v1.pos && v0.color == v1.color && v0.nomral == v1.nomral && v0.uvw0 == v1.uvw0 && v0.uvw1 == v1.uvw1 && v0.uvw2 == v1.uvw2 && v0.tangent < v1.tangent)
		return true;

	return false;
}



//const Matrix3 g_mat3Inch2CMScale0 (
//	Point3(1.0f,   0.0f,  0.0f),
//	Point3(0.0f,   1.0f,  0.0f),
//	Point3(0.0f,   0.0f,  1.0f),
//	Point3(0.0f,   0.0f,  0.0f)
//	);
//
//const Matrix3 g_mat3Inch2CMScale1 (
//	Point3(2.54f,   0.0f,   0.0f),
//	Point3(0.0f,    2.54f,  0.0f),
//	Point3(0.0f,    0.0f,   2.54f),
//	Point3(0.0f,    0.0f,   0.0f)
//	);
//
//
//const Matrix3 &g_Getmat3Inch2CMScale()
//{
//	//if(ExportOption()->fFbxScale)
//	//{
//	//	return g_mat3Inch2CMScale1;
//	//}
//	//else
//	//{
//	//	return g_mat3Inch2CMScale0;
//	//}
//	float f = ExportOption()->fFbxScale;
//
//	return Matrix3(		
//		Point3(f,   0.0f,   0.0f),
//		Point3(0.0f,    f,  0.0f),
//		Point3(0.0f,    0.0f,   f),
//		Point3(0.0f,    0.0f,   0.0f)
//
//		);
//}
//
//void g_ChangeMatrixYZ(Matrix3* mat)
//{
//	if (ExportOption()->SwitchToMaxMat()) // ?? bExchangeYZ
//	{	
//		Point3 RowY = mat->GetRow(1);
//		Point3 RowZ = mat->GetRow(2);
//		mat->SetRow(1, RowZ);
//		mat->SetRow(2, RowY);
//		Point4 ColY = mat->GetColumn(1);
//		Point4 ColZ = mat->GetColumn(2);
//		mat->SetColumn(1, ColZ);
//		mat->SetColumn(2, ColY);
//	}
//}


BOOL _IsFirstFlexiBoneName(const char* szName)
{
	if (strlen(szName) < 5)
		return FALSE;

	if (
		(szName[0] != 'F' && szName[0] != 'f') ||
		(szName[1] != 'B' && szName[1] != 'b') ||
		(szName[2] != 'R' && szName[2] != 'r') 
		)
		return FALSE;

	if ((szName[3] == '_') ||  (szName[4] == '_'))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _IsSubFlexiBoneName(const char* szName)
{
	if (strlen(szName) < 5)
		return FALSE;

	if (
		(szName[0] != 'F' && szName[0] != 'f') ||
		(szName[1] != 'B' && szName[1] != 'b') 
		)
		return FALSE;

	if ((szName[2] == '_') || (szName[3] == '_'))
	{
		return TRUE;
	}

	return FALSE;
}



//BOOL KParser::IsBipNameIgnore(FbxNode *pNode)
//{
//	const char* szName = pNode->GetName();
//	//如果是mdl骨架解析，那么物理骨骼是忽略掉的
//	if(IsMdlParser()&& (_IsFirstFlexiBoneName(szName) || _IsSubFlexiBoneName(szName)))
//	{
//		return TRUE;
//	}
//
//	if((szName[0]=='S' || szName[0]=='s') && szName[1]=='_')
//	{
//		m_vecSoketFbxNode.push_back(pNode);
//	}
//
//	//if(IsMarchBlackName(szName))
//	//{
//	//	return TRUE;
//	//}	
//	return FALSE;
//}
//
//const char *KParser::GetSourceName()
//{
//	return m_szName.c_str();
//}
//
//KMesh *KParser::GetPartMesh(const char *pPartName)
//{
//	for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;		
//		if(strstr(pMesh->m_szMeshName.c_str(), pPartName))
//		{
//			return pMesh;
//		}
//	}
//	return NULL;
//}
//
//void KParser::FixSocketBones(FbxNode *pRootNode)
//{
//	for(vector<FbxNode *>::iterator it = m_vecSoketFbxNode.begin(); it != m_vecSoketFbxNode.end(); ++it)
//	{
//		FbxNode *pNode = *it;
//		const char *pSocketName = pNode->GetName();
//		//按配置文件中定义的socket所在part的先后顺序进行查找的,也就是有优先级分配的原则，这种插槽分配方式有点混乱，最好是固定原则
//		//这里是只是程序上支持这种分配方式而已，最好还是一个插槽固定只能分配在某一个部件上，而不是可以分配在多个部件上
//		for(vector<KPartSocketConfig>::iterator it2 = m_vecSocketPart.begin(); it2 != m_vecSocketPart.end(); ++it2)
//		{
//			KPartSocketConfig &config = *it2;
//			for(vector<string>::iterator it3 = config.vecSocketName.begin(); it3 != config.vecSocketName.end(); ++it3)
//			{
//				string &socket_name = *it3;
//				string &part_name = config.szPartName;				
//				if(_stricmp(socket_name.c_str(), pSocketName) == 0)
//				{
//					KMesh *pMesh = GetPartMesh(config.szPartName.c_str());
//					if(pMesh)
//					{
//						pMesh->FixSocketBone(pNode, pRootNode);
//						goto LoopExit1;
//					}
//				}
//			}
//		}
//LoopExit1:
//		NULL;
//	}
//
//	for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;
//		pMesh->BuildSocketList();
//	}
//}
//
void KParser::Clear()
{
	m_nAnimationFrames = 0;	

	/*for(vector<KBipBone*>::iterator it = m_vecBipBone.begin(); it != m_vecBipBone.end(); ++it)
	{
		KBipBone *pBone = *it;
		delete(pBone);
	}
	m_vecBipBone.clear();

	for(vector<KBipBone*>::iterator it = m_vecIgnoreBipBone.begin(); it != m_vecIgnoreBipBone.end(); ++it)
	{
		KBipBone *pBone = *it;
		delete(pBone);
	}	
	m_vecIgnoreBipBone.clear();


	for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
	{
		KMesh *pMesh = *it;
		delete(pMesh);
	}

	for (auto it = m_vecCollisionMesh.begin(); it != m_vecCollisionMesh.end(); ++it)
	{
		KMesh *pMesh = *it;
		SAFE_DELETE(pMesh);
	}

	for (auto it = m_mapModelSet.begin(); it != m_mapModelSet.end(); ++it)
	{
		KModel* pModel = it->second;
		SAFE_DELETE(pModel);
	}
	m_vecCollisionMesh.clear();

	for (auto it = m_mapLodMesh.begin(); it != m_mapLodMesh.end(); ++it)
	{
		for(vector<KMesh *>::iterator itMesh = it->second.begin(); itMesh != it->second.end(); ++itMesh)
		{
			KMesh *pMesh = *itMesh;
			delete(pMesh);
		}
		it->second.clear();
	}
	m_mapLodMesh.clear();*/

	m_vecMesh.clear();
	m_vecSoketFbxNode.clear();
	m_pMartchSkeleton = NULL;
	m_mapRootBone.clear();
	errorMsg.clear();
	warnMsg.clear();
	m_vecErrorBoneWeightNames.clear();
	m_mapModelSet.clear();
}
//
//#define LINESIZE 2048
//
KParser::KParser()
{
	m_nAnimationFrames = 0;	
	m_nSampleFrameInterval = 5;
	m_pSdkManager = NULL;
	m_pScene = NULL;
	m_bParseBlendShape = FALSE;

	/*KIniFile IniFile;
	if(IniFile.OpenIniFile("cfg/ks_fbx_export.ini"))
	{
		char key[64];
		int num = IniFile.ReadInt("Parts", "NumKey", 0);
		for(int i = 0; i < num; ++i)
		{
			sprintf(key, "Key%d", i);
			string value = IniFile.ReadString("Parts", key, "");
			m_vecPartsFilter.push_back(value);
		}


		num = IniFile.ReadInt("BlackList", "NumKey", 0);		
		for(int i = 0; i < num; ++i)
		{
			sprintf(key, "Black%d", i);
			string value = IniFile.ReadString("BlackList", key, "");
			m_vecBlackListFilter.push_back(value);
		}		

		num = IniFile.ReadInt("Relink", "NumKey", 0);
		char keyc[64];
		char keyp[64];
		for(int i = 0; i < num; ++i)
		{
			sprintf(keyc, "RelinkC%d", i);
			sprintf(keyp, "RelinkP%d", i);
			string c_value = IniFile.ReadString("Relink", keyc, "");
			string p_value = IniFile.ReadString("Relink", keyp, "");
			m_mapRelink.insert(pair<string, string>(c_value, p_value));
		}

		num = IniFile.ReadInt("Sockets", "NumKey", 0);
		for(int i = 0; i < num; ++i)
		{
			sprintf(key, "Part%d", i);
			string szHasSocketPartName = IniFile.ReadString("Sockets", key, "");
			KPartSocketConfig partsocket;
			partsocket.szPartName = szHasSocketPartName;
			m_vecSocketPart.push_back(partsocket);			
		}

		for(vector<KPartSocketConfig>::iterator it = m_vecSocketPart.begin(); it != m_vecSocketPart.end(); ++it)
		{
			KPartSocketConfig &part = *it;
			num = IniFile.ReadInt(part.szPartName.c_str(), "NumKey", 0);
			for(int i = 0; i < num; ++i)
			{
				sprintf(key, "Socket%d", i);
				string socketName = IniFile.ReadString(part.szPartName.c_str(), key, "");
				part.vecSocketName.push_back(socketName);
			}
		}

		char path[MAX_PATH];
		num = IniFile.ReadInt("Skeleton", "NumKey", 0);

		char szLine[LINESIZE];
		for(int i = 0; i < num; ++i)
		{
			sprintf(key, "Skeleton%d", i);
			string fileName = IniFile.ReadString("Skeleton", key, "");
			KSkeletonFileConfig *pSkeletonFile = new KSkeletonFileConfig;
			pSkeletonFile->szFileName = fileName;
			sprintf(path, "cfg\\%s", fileName.c_str());
			FILE *fp = fopen(path, "rb");
			if(fp)
			{
				char *szNum = fgets(szLine, LINESIZE, fp);
				int n = atoi(szNum);
				for(int j = 0; j < n; ++j)
				{
					memset(szLine, 0, LINESIZE);
					KSkeletonTextBone skBone;
					char *pStr = fgets(szLine, LINESIZE, fp);
					int nlinelen = (int) strlen(pStr);					
					char *p = pStr;					
					int nToken = 0;
					while(p)
					{
						char *newpos = strstr(p, "  ");

						if(!newpos)
						{
							newpos = strstr(p, "\r");
						}
						else if(!newpos)
						{
							newpos = strstr(p, "\n");
						}

						if(!newpos)
						{
							break;
						}

						std::ptrdiff_t nLen = newpos - p;
						memcpy(key, p, nLen);
						key[nLen] = '\0';													

						if(nToken == 0)
						{
							skBone.szBoneName = key;
						}
						else
						{
							skBone.m_setChildren.insert(key);
						}

						nToken ++;
						p = newpos + 2;
					}
					pSkeletonFile->SkeletonList.push_back(skBone);
				}
				m_vecSkeletonFile.push_back(pSkeletonFile);
				fclose(fp);
			}
		}

		IniFile.CloseIniFile();
	}*/	
}



KParser::~KParser()
{
	Clear();
	for(vector<KSkeletonFileConfig *>::iterator it = m_vecSkeletonFile.begin(); it != m_vecSkeletonFile.end(); ++it)
	{
		//KSkeletonFileConfig *pFileConfig = *it;
		//delete(pFileConfig);
	}
}
//
//void KParser::AddErrorWeightBoneName(const char *pName)
//{
//	m_vecErrorBoneWeightNames.push_back(pName);
//	char msg[256];
//	sprintf(msg, "%s exceed 4 link \n", pName);
//	warnMsg.append(msg);
//}
//
//
//KMesh::KMesh()
//{
//	m_nUvElementCount = 0;
//	m_nColorElementCount = 0;
//	m_pFbxMesh = NULL;	
//	m_numFaces = 0;
//	m_pFbxNode = NULL;
//	m_dwPointCacheFrames = 0;	
//    m_nBlendShapeElementCount = 0;
//	m_nLOD = 0;
//}
//
//
//void KMesh::_BuildBoneLinkName(KMeshBone *pMeshBone)
//{
//	FbxNode *pParentFbxNode = pMeshBone->pFbxNode->GetParent();
//
//	//父节点
//	if(pParentFbxNode)
//		pMeshBone->m_szParentName = pParentFbxNode->GetName();
//
//	//找子节点，必须是有效的蒙皮骨骼（toDO:这里有问题)
//	int nChild = pMeshBone->pFbxNode->GetChildCount();
//	for(int i = 0; i < nChild; ++i)
//	{
//		FbxNode *pChildNode = pMeshBone->pFbxNode->GetChild(i);
//		string childName = pChildNode->GetName();
//		if(GetBone(childName.c_str()))			
//		{
//			pMeshBone->m_vecChildNames.push_back(childName);
//		}
//	}
//}
//
//void KMesh::_BuildBoneMatrixOffset(KMeshBone *pMeshBone)
//{
//	//const char *p0 = pMeshBone->pFbxNode->GetName();			
//	FbxAMatrix mat = pMeshBone->pFbxNode->EvaluateGlobalTransform(0);
//	Matrix3 maxMat;
//	ToMaxMat3(maxMat, mat);			
//	maxMat = maxMat * g_Getmat3Inch2CMScale();				
//	g_ChangeMatrixYZ(&maxMat);
//	maxMat.Invert();
//	Matrix3ToMatrix4(&pMeshBone->MatrixOffset, &maxMat);	
//}
//
//void KMesh::BuildBoneLinkNames()
//{
//	for(vector<KMeshBone*>::iterator it = m_vecBones.begin(); it != m_vecBones.end(); ++it)
//	{
//		KMeshBone *pMeshBone = *it;
//		_BuildBoneLinkName(pMeshBone);
//	}
//}
//
//void KMesh::BuildBoneMatrixOffset()
//{
//	for(vector<KMeshBone*>::iterator it = m_vecBones.begin(); it != m_vecBones.end(); ++it)
//	{
//		KMeshBone *pMeshBone = *it;
//		if(pMeshBone->pFbxNode)
//		{		
//			_BuildBoneMatrixOffset(pMeshBone);
//		}
//		else
//		{
//			assert(0);
//		}
//	}
//}
//
//
////当前骨骼或者某根父骨骼是weightbone,那就是有效的meshbone
//BOOL KMesh::IsMeshBone(FbxNode *pNode)
//{
//	BOOL bIsMeshBone = FALSE;
//	while(pNode)
//	{
//		const char *pNodeName = pNode->GetName();
//		set<string>::iterator it = m_weightBoneNames.find(pNodeName);
//		if(it != m_weightBoneNames.end())
//		{
//			bIsMeshBone = TRUE;
//			break;
//		}
//		pNode = pNode->GetParent();
//	}
//	return bIsMeshBone;
//}
//
//
//
////void KMesh::VistBoneTree(FbxNode *fbxNode, KParser *pParser)
////{
////	const char *pBoneName = fbxNode->GetName();	
////	if(IsBipNameIgnore(pBoneName,  pParser))
////	{
////		return;
////	}
////	
////	KMeshBone *pMeshBone = NULL;
////	for(vector<KMeshBone*>::iterator it = m_vecBones.begin(); it != m_vecBones.end(); ++it)
////	{
////	    KMeshBone *pBone = *it;
////		if(strcmp(pBone->szBoneName, pBoneName) == 0)
////		{
////			pMeshBone = pBone;			
////			break;
////		}
////	}
////	
////	if(!pMeshBone)
////	{
////		pMeshBone = new KMeshBone;
////		strncpy(pMeshBone->szBoneName, pBoneName, 30);
////		pMeshBone->pFbxNode = fbxNode;
////		AddMeshBone(pMeshBone);
////	}
////	pMeshBone->m_szParentName = pMeshBone->pFbxNode->GetName();	
////
////	int nChildCount = fbxNode->GetChildCount();
////	for(int i = 0; i < nChildCount; i++)
////	{
////		FbxNode *pChildNode = fbxNode->GetChild(i);
////		const char *pChildName = pChildNode->GetName();
////		if(!IsBipNameIgnore(pChildName,  pParser))
////		{
////			pMeshBone->m_vecChildNames.push_back(pChildName);
////			VistBoneTree(pChildNode, pParser);
////		}		
////	}
////}
//
////void KMesh::BuildBoneList(KParser *pParser)
////{
////	for(map<string, FbxNode *>::iterator it = m_mapRoots.begin(); it != m_mapRoots.end(); ++it)
////	{
////		FbxNode *pFbxNode = it->second;
////		VistBoneTree(pFbxNode, pParser);
////	}
////}
//
//void KMesh::BuildRootBoneByWeightBone(FbxNode *pNode, FbxNode *pRootNode, KParser *pParser)
//{	
//	const char *pName = NULL;
//	FbxNode *pParent = NULL;
//	FbxNode *pWholeRootBone = NULL;
//	FbxNode *pRootBone = NULL;
//	while(pNode)
//	{	
//		pParent = pNode->GetParent();
//		pName = pNode->GetName();
//
//		if(pParent && pParent->GetParent() == pRootNode)
//		{
//			pRootBone = pNode;						
//		}
//
//		if(pParent == pRootNode)
//		{
//			pWholeRootBone = pNode;			
//		}
//		pNode = pParent;		
//	}
//	
//	//如果rootbone是权重骨骼，那么作为根骨骼，否则不作为根骨骼
//	if(m_weightBoneNames.find(pRootBone->GetName()) != m_weightBoneNames.end())
//	{
//		pRootBone = pWholeRootBone;		
//	}
//
//	const char *pRootBoneName = pRootBone->GetName();
//	m_mapRoots[pRootBoneName] = pRootBone;
//
//	pParser->AddBipRootBone(pWholeRootBone->GetName());
//}
//
///*
//void KMesh::AddMeshBone(FbxCluster* pCluster,  FbxNode *pRootNode, KParser *pParser)
//{
//	FbxNode *pFbxNode = pCluster->GetLink();
//	FbxNode *pWeightNode = pFbxNode;
//	while(pFbxNode && pFbxNode != pRootNode)
//	{
//		const char *pBoneName = pFbxNode->GetName();
//		if(strstr(pBoneName, "Mouth_Low2_L"))
//		{
//			int x = 0;
//		}
//		KMeshBone *pMeshBone = GetBone(pBoneName);
//		if(!pMeshBone)
//		{
//			pMeshBone = new KMeshBone;
//			pMeshBone->pFbxNode = pFbxNode;
//			if(pFbxNode == pWeightNode)
//			    pMeshBone->m_pCluster = pCluster;
//			strncpy(pMeshBone->szBoneName, pBoneName, 30);
//			m_vecBones.push_back(pMeshBone);
//
//			FbxNode *pParent =  pCluster->GetLink()->GetParent();
//			if(pParent == pRootNode)
//			{
//				pMeshBone->bIsRoot = TRUE;
//			}
//		}
//
//		FbxNode *pParent = pFbxNode->GetParent();
//		const char *parentNodeName = pParent->GetName();
//		//const char *pRootName = pRootNode->GetName();
//		FbxNode *pNextNode = pParent;
//		if(pParent)
//		{		
//			BOOL bParentIgnore = FALSE;
//			//父骨骼是被忽略的骨骼，那么把当前骨骼上移到父骨骼的兄弟骨骼
//			if(pParser->IsMarchBlackName(parentNodeName))
//			{
//				FbxNode *pParentParent = pParent->GetParent();
//				if(pParentParent)
//				{
//					pParent->RemoveChild(pFbxNode);
//					pParentParent->AddChild(pFbxNode);
//					//pNextNode = pParentParent;
//				}
//			}
//		}
//
//		//添加bip根骨骼信息
//		if(pParent == pRootNode)
//		{
//			pParser->AddBipRootBone(parentNodeName);
//		}
//		pFbxNode = pNextNode;
//	}
//}
//*/
//
//void KMesh::AddMeshBone(FbxCluster* pCluster,  FbxNode *pRootNode, KParser *pParser)
//{
//
//	const char *pBoneName = pCluster->GetLink()->GetName();
//	KMeshBone *pMeshBone = GetBone(pBoneName);
//
//	if(!pMeshBone)
//	{
//		pMeshBone = new KMeshBone;
//		strncpy(pMeshBone->szBoneName, pBoneName, 30);
//		m_vecBones.push_back(pMeshBone);	
//		FbxNode *pParent =  pCluster->GetLink()->GetParent();
//		if(pParent == pRootNode)
//		{
//			pMeshBone->bIsRoot = TRUE;
//		}
//	}
//
//	{	
//		FbxNode *pCurNode = pCluster->GetLink();
//		FbxNode *pParentNode = pCurNode->GetParent();
//		if(pParentNode)
//		{
//			const char *szParentName = pParentNode->GetName();
//			//父骨骼是被忽略的骨骼，那么把当前骨骼上移到父骨骼的兄弟骨骼
//			if(pParser->IsMarchBlackName(szParentName))
//			{
//				FbxNode * pParentParent = pParentNode->GetParent();
//				if(pParentParent)
//				{
//					//pParentNode->RemoveChild(pCurNode);
//					//pParentParent->AddChild(pCurNode);
//				}
//			}
//		}
//	}
//
//
//	pMeshBone->m_pCluster = pCluster;
//	pMeshBone->pFbxNode = pCluster->GetLink();
//
//
//	FbxNode *pParent =  pMeshBone->pFbxNode->GetParent();
//	while(pParent && pParent != pRootNode)
//	{
//		const char *parentNodeName = pParent->GetName();		
//		if(!GetBone(parentNodeName))
//		{
//			//如果是mdlparser,mesh本身不需要完整骨架(但要补齐两个weightbone之间的骨骼）
//			//如果不是mdlparser,只要是weightbone，不论是不是两个weightbone之间的都要添加
//			//if(IsMeshBone(pParent))
//
//			{
//				//父骨骼不一定是影响顶点的权重骨骼，但作为骨架而言是必须的，所以如果已经解析的列表没有，那必须补齐
//				KMeshBone *pMeshBone = new KMeshBone;
//				strncpy(pMeshBone->szBoneName, pParent->GetName(), 30);				
//				m_vecBones.push_back(pMeshBone);				
//				pMeshBone->pFbxNode = pParent;
//				if(pParent->GetParent() == pRootNode)
//				{
//					pMeshBone->bIsRoot = TRUE;
//				}
//			}
//		}
//
//		FbxNode *pCurNode = pParent;
//		const char *p1 = pCurNode->GetName();
//
//		pParent = pParent->GetParent();
//		//const char *pRootName = pRootNode->GetName();
//
//		FbxNode *pParentParent = NULL;
//		if(pParent)
//		{		
//			const char *szParentName = pParent->GetName();
//			//父骨骼是被忽略的骨骼，那么把当前骨骼上移到父骨骼的兄弟骨骼
//			if(pParser->IsMarchBlackName(szParentName))
//			{
//				pParentParent = pParent->GetParent();
//				if(pParentParent)
//				{
//					//pParent->RemoveChild(pCurNode);
//					//pParentParent->AddChild(pCurNode);
//				}
//			}
//		}
//
//		//添加bip根骨骼信息
//		if(pParent == pRootNode)
//		{
//			pParser->AddBipRootBone(parentNodeName);
//		}
//		if(pParentParent)
//		{
//			pParent = pParentParent;
//		}
//	}
//}
//
///*
//void KMesh::AddMeshBone(FbxCluster* pCluster,  FbxNode *pRootNode, KParser *pParser)
//{
//	
//	const char *pBoneName = pCluster->GetLink()->GetName();
//	KMeshBone *pMeshBone = GetBone(pBoneName);
//	
//	if(!pMeshBone)
//	{
//		pMeshBone = new KMeshBone;
//		strncpy(pMeshBone->szBoneName, pBoneName, 30);
//		m_vecBones.push_back(pMeshBone);	
//		FbxNode *pParent =  pCluster->GetLink()->GetParent();
//		if(pParent == pRootNode)
//		{
//			pMeshBone->bIsRoot = TRUE;
//		}
//	}
//	
//	pMeshBone->m_pCluster = pCluster;
//	pMeshBone->pFbxNode = pCluster->GetLink();
//		
//
//	FbxNode *pParent =  pMeshBone->pFbxNode->GetParent();
//	while(pParent && pParent != pRootNode)
//	{
//		const char *parentNodeName = pParent->GetName();		
//		if(!GetBone(parentNodeName))
//		{
//			//如果是mdlparser,mesh本身不需要完整骨架(但要补齐两个weightbone之间的骨骼）
//			//如果不是mdlparser,只要是weightbone，不论是不是两个weightbone之间的都要添加
//			//if(IsMeshBone(pParent))
//
//			{
//				//父骨骼不一定是影响顶点的权重骨骼，但作为骨架而言是必须的，所以如果已经解析的列表没有，那必须补齐
//				KMeshBone *pMeshBone = new KMeshBone;
//				strncpy(pMeshBone->szBoneName, pParent->GetName(), 30);				
//				m_vecBones.push_back(pMeshBone);				
//				pMeshBone->pFbxNode = pParent;
//				if(pParent->GetParent() == pRootNode)
//				{
//					pMeshBone->bIsRoot = TRUE;
//				}
//			}
//		}
//
//		FbxNode *pCurNode = pParent;
//		const char *p1 = pCurNode->GetName();
//
//		pParent = pParent->GetParent();
//		//const char *pRootName = pRootNode->GetName();
//		if(pParent)
//		{		
//			BOOL bParentIgnore = FALSE;
//			const char *szParentName = pParent->GetName();
//			//父骨骼是被忽略的骨骼，那么把当前骨骼上移到父骨骼的兄弟骨骼
//			if(pParser->IsMarchBlackName(szParentName))
//			{
//				FbxNode *pParentParent = pParent->GetParent();
//				if(pParentParent)
//				{
//					pParent->RemoveChild(pCurNode);
//					pParentParent->AddChild(pCurNode);
//				}
//			}
//		}
//
//		//添加bip根骨骼信息
//		if(pParent == pRootNode)
//		{
//			pParser->AddBipRootBone(parentNodeName);
//		}
//	}
//}
//*/
//
//
//void KMesh::BuildSocketList()
//{
//	char szSocketName[256];
//	for(vector<KMeshBone*>::iterator it = m_vecBones.begin(); it != m_vecBones.end(); ++it)
//	{
//		KMeshBone *pMeshBone = *it;
//		int nCount = pMeshBone->pFbxNode->GetChildCount();
//		const char *pNa = pMeshBone->pFbxNode->GetName();
//		for(int i = 0; i < nCount; ++i)
//		{
//			FbxNode *pChild = pMeshBone->pFbxNode->GetChild(i);
//			const char *pName = pChild->GetName();
//			strncpy(szSocketName, pName, 256);			
//			if((strncmp(szSocketName, "S_", sizeof("S_") - 1) == 0 || strncmp(szSocketName, "s_", sizeof("s_") - 1) == 0) && 
//				m_socketNames.find(szSocketName) != m_socketNames.end()
//				)
//			{
//				KSocket *pSokect = new KSocket;
//				pSokect->szName = szSocketName;
//				pSokect->szParentName = pMeshBone->szBoneName;
//				FbxAMatrix matNodeParent = pMeshBone->pFbxNode->EvaluateGlobalTransform(0);
//				FbxAMatrix matNodeChild = pChild->EvaluateGlobalTransform(0);
//				Matrix3 mat3NodeParent;
//				Matrix3 mat3NodeChild;
//				ToMaxMat3(mat3NodeParent, matNodeParent);
//				ToMaxMat3(mat3NodeChild, matNodeChild);
//				Matrix3 mat3NodeLocal = mat3NodeChild * Inverse(mat3NodeParent);
//				g_ChangeMatrixYZ(&mat3NodeLocal);
//				AffineParts apNodeLocal;
//				decomp_affine(mat3NodeLocal, &apNodeLocal);
//
//				Matrix3 mat3_apNodeLocal_t;
//				mat3_apNodeLocal_t.IdentityMatrix();
//				mat3_apNodeLocal_t.SetTrans(apNodeLocal.t);
//
//				Matrix3 mat3_apNodeLocal_q;
//				apNodeLocal.q.MakeMatrix(mat3_apNodeLocal_q);
//
//				Matrix3 mat3_apNodeLocal_u;
//				apNodeLocal.u.MakeMatrix(mat3_apNodeLocal_u);
//
//				Matrix3 mat3_apNodeLocal_k;
//				if (ExportOption()->bNormalizeSocketSize)
//					apNodeLocal.k = Point3( 1.0f, 1.0f, 1.0f );
//
//				if (ExportOption()->fFbxScale)
//				{
//					apNodeLocal.k.x = apNodeLocal.k.x / ExportOption()->fFbxScale;
//					apNodeLocal.k.y = apNodeLocal.k.y / ExportOption()->fFbxScale;
//					apNodeLocal.k.z = apNodeLocal.k.z / ExportOption()->fFbxScale;
//					mat3_apNodeLocal_k = ScaleMatrix(apNodeLocal.k);
//				}
//				else
//				{
//					mat3_apNodeLocal_k = ScaleMatrix(apNodeLocal.k);
//				}
//
//				Matrix3 mat3_apNodeLocal_f;
//				mat3_apNodeLocal_f = ScaleMatrix(Point3(apNodeLocal.f, apNodeLocal.f, apNodeLocal.f));
//
//				mat3NodeLocal = Inverse(mat3_apNodeLocal_u) * 
//					mat3_apNodeLocal_k * 
//					mat3_apNodeLocal_u * 
//					mat3_apNodeLocal_q * 
//					mat3_apNodeLocal_f * 
//					mat3_apNodeLocal_t;
//
//				Matrix3ToMatrix4(&pSokect->MatrixOffset, &mat3NodeLocal);
//
//				m_vecSocket.push_back(pSokect);
//			}
//		}
//	}
//}
//
//KMesh::~KMesh()
//{
//	for(vector<KMeshBone*>::iterator it = m_vecBones.begin(); it != m_vecBones.end(); ++it)
//	{
//		KMeshBone *pBone = *it;
//		delete(pBone);
//	}
//	for(map<KVec3, KWeight*>::iterator it = m_mapWeightControlPointMapping.begin(); it != m_mapWeightControlPointMapping.end(); ++it)
//	{
//		KWeight *pWeight = it->second;
//		delete(pWeight);
//	}
//	for(vector<KSocket *>::iterator it = m_vecSocket.begin(); it != m_vecSocket.end(); ++it)
//	{
//		KSocket *pSocket = *it;
//		delete(pSocket);
//	}	
//    for (vector<KBlendShape*>::iterator it = m_blendShapes.begin(); it != m_blendShapes.end(); ++it)
//    {
//        KBlendShape *pBlendShape = *it;
//        for (auto it1 = pBlendShape->m_vBlendShapeChannels.begin(); it1 != pBlendShape->m_vBlendShapeChannels.end(); ++it1)
//        {
//            KBlendShapeChannel *pChannel = *it1;
//            for (auto it2 = pChannel->m_vBlendShapeTargets.begin(); it2 != pChannel->m_vBlendShapeTargets.end(); ++it2)
//            {
//                KBlendShapeTarget *pTarget = *it2;
//                delete(pTarget);
//            }
//            delete(pChannel);
//        }
//        delete(pBlendShape);
//    }
//    for(vector<KSpline *>::iterator it = m_vecSplines.begin(); it != m_vecSplines.end(); ++it)
//    {
//        KSpline *pSpline = *it;
//        delete(pSpline);
//    }
//    m_vecSplines.clear();
//    for(vector<KAnchorPoint *>::iterator it = m_vecAnchorPoints.begin(); it != m_vecAnchorPoints.end(); ++it)
//    {
//        KAnchorPoint *pAnchorPoint = *it;
//        delete(pAnchorPoint);
//    }
//    m_vecAnchorPoints.clear();
//}
//
//void KParser::MergeMesh(OUT KMesh& mesh_merge, const std::vector<KMesh*>& vecMesh)
//{
//	int nMeshCount = 0;
//	for(auto it = vecMesh.begin(); it != vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;	
//		size_t oldsize = mesh_merge.m_verts.size();
//		mesh_merge.m_verts.reserve(oldsize + pMesh->m_verts.size());
//
//
//		oldsize = mesh_merge.m_verts.size();
//		mesh_merge.m_inds.reserve(oldsize + pMesh->m_inds.size());
//		for(auto itt = pMesh->m_inds.begin(); itt != pMesh->m_inds.end(); ++itt)
//		{
//			DWORD32 id = oldsize + *itt;
//			mesh_merge.m_inds.push_back(id);
//		}
//
//
//		for(auto itt = pMesh->m_verts.begin(); itt != pMesh->m_verts.end(); ++itt)
//		{
//			KPCNT3 &vert = *itt;
//			mesh_merge.m_verts.push_back(vert);
//			mesh_merge.m_vertsMergeMatTable.push_back(nMeshCount);
//		}
//
//		oldsize = mesh_merge.m_SubsetIds.size();
//		mesh_merge.m_vecFaceSubsetId.reserve(oldsize + pMesh->m_vecFaceSubsetId.size());
//		for(auto itt = pMesh->m_vecFaceSubsetId.begin(); itt != pMesh->m_vecFaceSubsetId.end(); ++itt)
//		{
//			DWORD32 face_oldId = *itt;
//			DWORD32 face_newId = oldsize + face_oldId;
//			mesh_merge.m_vecFaceSubsetId.push_back(face_newId);
//		}
//
//		mesh_merge.m_numFaces +=  pMesh->m_numFaces;		
//		oldsize = mesh_merge.m_SubsetIds.size();
//		for(auto itt = pMesh->m_SubsetIds.begin(); itt != pMesh->m_SubsetIds.end(); ++itt)
//		{
//			DWORD32 att_oldId = *itt;
//			DWORD32 att_newId = oldsize + att_oldId;
//			auto ittt = pMesh->m_Materials.find(att_oldId);
//			if(ittt != pMesh->m_Materials.end())
//			{			
//				KMaterail &mtl = ittt->second;
//				mesh_merge.m_Materials.insert(pair<int, KMaterail>(att_newId, mtl));
//			}
//			mesh_merge.m_SubsetIds.insert(att_newId);
//		}
//
//
//
//		if(pMesh->m_nUvElementCount > mesh_merge.m_nUvElementCount)
//		{
//			 mesh_merge.m_nUvElementCount = pMesh->m_nUvElementCount;
//		}
//		FbxAMatrix mat = pMesh->m_pFbxNode->EvaluateGlobalTransform(0);
//		mesh_merge.m_vertsMergeMat.push_back(mat);
//		++nMeshCount;
//	}
//}
//
//void KParser::MergeSaveAll(const char *szOutDir)
//{
//	KMesh mesh_collision;
//	char szOutMeshName[MAX_PATH] = {0};
//	KMesh mesh_merge;
//
//	// lod
//	MAP_LOD_MESH::iterator it = m_mapLodMesh.begin();
//	for (; it != m_mapLodMesh.end(); ++it)
//	{
//		KMesh mesh_merge;
//		MergeSave(mesh_merge, szOutDir, it->second, it->first);
//	}
//
//	//合并输出,只导出成静态模型
//	MergeSave(mesh_merge, szOutDir, m_vecMesh);
//	MergeMesh(mesh_collision, m_vecCollisionMesh);
//	SaveCollisionMeshFile(szOutMeshName, &mesh_collision, szOutDir, m_szName.c_str());
//	AddModelInfoLODPath(mesh_merge.m_szMeshName, szOutMeshName, true);
//	SaveJsonModelFile(this, &mesh_merge, szOutDir, m_szName.c_str(), FALSE);
//}
//
//void KParser::MergeSave(KMesh& mesh_merge, const char *szOutDir, const vector<KMesh*>& vecMesh, int nLOD/* = 0*/)
//{
//	ExportOption()->bFbxNameExport = TRUE;
//	//KMesh mesh_merge;
//	//KMesh mesh_collision;
//
//	MergeMesh(mesh_merge, vecMesh);
//	mesh_merge.m_nLOD = nLOD;
//	mesh_merge.m_szMeshName = m_szName;
//	/*MergeMesh(mesh_collision, m_vecCollisionMesh);*/
//
//	/*
//	//重新把相同材质id合并一把
//	map<int, KMaterail> newMtlMaps;
//	set<int> newSubids;
//
//	for(auto it = mesh_merge.m_Materials.begin(); it != mesh_merge.m_Materials.end(); ++it)
//	{
//		int key = it->first;
//		KMaterail &mtl = it->second;
//		BOOL bFind = FALSE;
//		for(auto itt = newMtlMaps.begin(); itt != newMtlMaps.end(); ++itt)
//		{
//			int _key = itt->first;
//			KMaterail &_mtl = itt->second;
//			if(_mtl.textureName == mtl.textureName)
//			{
//				for(int i = 0; i < mesh_merge.m_vecFaceSubsetId.size(); ++i)
//				{
//					if(mesh_merge.m_vecFaceSubsetId[i] == key)
//					{
//						mesh_merge.m_vecFaceSubsetId[i] = _key; 
//					}					
//				}
//				bFind = TRUE;
//				break;
//			}
//		}
//
//		if(!bFind)
//		{
//			newSubids.insert(key);
//			newMtlMaps.insert(pair<int, KMaterail>(key, mtl));
//		}		
//	}
//
//	mesh_merge.m_SubsetIds = newSubids;
//	mesh_merge.m_Materials = newMtlMaps;
//	*/
//	char szOutMeshName[MAX_PATH] = {0};
//	SaveMeshFile(szOutMeshName, &mesh_merge, szOutDir, m_szName.c_str(), FALSE);
//	AddModelInfoLODPath(mesh_merge.m_szMeshName, szOutMeshName);
//	memset(szOutMeshName, 0, sizeof(szOutMeshName));
//	SaveJsoninspackFile(szOutMeshName, &mesh_merge, szOutDir, m_szTgaOutPath, m_szName.c_str(), FALSE);
//	AddModleInfoJsonInspackPath(mesh_merge.m_szMeshName, szOutMeshName);
//	SaveMaterailFile(&mesh_merge, szOutDir, m_szTgaOutPath, this);
//	//memset(szOutMeshName, 0, sizeof(szOutMeshName));
//	/*SaveCollisionMeshFile(szOutMeshName, &mesh_collision, szOutDir, m_szName.c_str());*/
//	/*AddModelInfoLODPath(&mesh_merge, szOutMeshName, true);*/
//}
//
//void KParser::SaveMesh(const char *szOutDir, vector<KMesh*>& vecMesh)
//{
//
//	//现在改成，只有多个mesh，并且不是mdl，那就按group输出
//	//if(!m_bMdlParser && m_vecMesh.size() > 1)
//	//{
//	//	ExportOption()->bGroupExport = TRUE;
//	//}
//	//else
//	//{
//	//	ExportOption()->bGroupExport = FALSE;
//	//}
//	int nLOD = -1;
//	if(m_bMdlParser)
//	{
//		ExportOption()->bGroupExport = FALSE;
//	}
//
//
//	//如果只有单mesh导出，强行按fbxName来命名
//	if(vecMesh.size() == 1)
//	{
//		ExportOption()->bFbxNameExport = TRUE;
//	}
//
//
//	KVec3 vCenter(0,0,0);
//	//int nCount = 0;
//
//	char szMeshOutName[MAX_PATH] = {0};
//	for(vector<KMesh *>::iterator it = vecMesh.begin(); it != vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;	
//		nLOD = pMesh->m_nLOD;
//		memset(&szMeshOutName, 0, sizeof(szMeshOutName));
//		SaveMeshFile(szMeshOutName, pMesh, szOutDir, m_szName.c_str(), m_bMdlParser);
//		AddModelInfoLODPath(pMesh->m_szMeshName, std::string(szMeshOutName));
//
//		//FbxAMatrix mat = pMesh->m_pFbxNode->EvaluateGlobalTransform(0);
//		//Matrix3 maxMat;
//		//ToMaxMat3(maxMat, mat);
//		//Point3 transP= maxMat.GetTrans();
//
//		//vCenter.x += transP.x;
//		//vCenter.y += transP.y;
//		//vCenter.z += transP.z;
//
//		//++nCount;
//	}
//
//	//HD
//	BOOL bHD = ExportOption()->bHD;
//	char ext[MAX_PATH] = { 0 };
//	char extMaterial[MAX_PATH] = { 0 };
//	char szLODExt[MAX_PATH] = {0};
//	if (nLOD > 0)
//		sprintf(szLODExt, "_lod%d", nLOD);
//
//	//if (nLOD <= 0)
//	//{
//	//	if (bHD) strcpy_s(ext, "_HD.mesh");
//	//	else     strcpy_s(ext, ".mesh");
//	//	if (bHD) strcpy_s(extMaterial, "_HD.JsonInspack");
//	//	else     strcpy_s(extMaterial, ".mtl");
//	//}
//
//	if (bHD) sprintf(ext, "_HD%s.mesh", szLODExt);
//	else     sprintf(ext, "%s.mesh", szLODExt);
//	if (bHD) sprintf(extMaterial, "_HD%s.JsonInspack", szLODExt);
//	else     sprintf(extMaterial, "%s.mtl", szLODExt);
//
//
//	if(ExportOption()->bGroupExport)
//	{
//		//vCenter.x /= (float) nCount;
//		//vCenter.y /= (float) nCount;
//		//vCenter.z /= (float) nCount;
//
//
//		char pFileName[MAX_PATH];
//		int len = (int) strlen(szOutDir);
//		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
//			sprintf(pFileName, "%s%s%s.Group", szOutDir, m_szName.c_str(), szLODExt);
//		else
//			sprintf(pFileName, "%s\\%s%s.Group", szOutDir, m_szName.c_str(), szLODExt);
//
//		const char *p = strstri(szOutDir, "\\Data\\");
//		if(!p)
//		{		
//			char msg[260];					
//			sprintf(msg, "warn: the export directory has no part of '\\data\\' as root, so group file will use absolute path, that could make something wrong to the engine, we stronger suggest you choose the Export Path contains '\\data\\'  \r\n");
//			errorMsg.append(msg);
//		}
//
//		if (_access(szOutDir, 0) != 0)
//		{
//			Make_Dirs(szOutDir);
//		}
//
//		FILE *fp = fopen(pFileName, "wb");
//		if(fp)
//		{
//			char meshName[MAX_PATH];
//			char mtlName[MAX_PATH];
//			int n = 0;
//
//			for(vector<KMesh *>::iterator it = vecMesh.begin(); it != vecMesh.end(); ++it)
//			{
//				KMesh *pMesh = *it;
//
//				char szMeshOutDir[MAX_PATH];
//
//				const char *q = strstri(pMesh->m_szMeshName.c_str(), "#");
//				if (q)
//				{
//					GetMapsSourcePathFromPath(szOutDir, szMeshOutDir, MAX_PATH);
//				}
//				else
//				{
//					strcpy(szMeshOutDir, szOutDir);
//				}
//
//				string strMeshName = pMesh->m_szMeshName;
//				ReplaceAll(strMeshName, "#", "\\");
//				
//				std::ptrdiff_t relativePath_offset = -1;
//				const char *p = strstri(szMeshOutDir, "\\Data\\");
//				if(p)
//				{
//					relativePath_offset  = p - szMeshOutDir;
//				}			
//				
//				int len = (int) strlen(szOutDir);
//
//				if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
//				{
//					sprintf(meshName, "%s%s%s", szMeshOutDir + relativePath_offset + 1, strMeshName.c_str(), ext);
//					sprintf(mtlName, "%s%s%s", szMeshOutDir + relativePath_offset + 1, strMeshName.c_str(), extMaterial);
//				}
//				else
//				{
//					sprintf(meshName, "%s\\%s%s", szMeshOutDir + relativePath_offset + 1, strMeshName.c_str(), ext);
//					sprintf(mtlName, "%s\\%s%s", szMeshOutDir + relativePath_offset + 1, strMeshName.c_str(), extMaterial);
//				}
//				
//                KVec3 trans;
//                KVec3 scale;
//                KVec4 quat;
//
//                GetGlobalRTS(pMesh->m_pFbxNode, &trans, &scale, &quat, vCenter);
//				
//				fprintf(fp, "[Model%d]\r\n", n);
//				fprintf(fp, "MeshName=%s\r\n", meshName);
//				fprintf(fp, "MtlName=%s\r\n", mtlName);
//				fprintf(fp, "AniName=\r\n");
//				fprintf(fp, "Translation=%f,%f,%f\r\n", trans.x, trans.y, trans.z);
//				fprintf(fp, "Scaling=%f,%f,%f\r\n", scale.x, scale.y, scale.z);
//				fprintf(fp, "Rotation=%f,%f,%f,%f\r\n", quat.x, quat.y, quat.z, quat.w);
//				fprintf(fp, "LocalBBoxvMax=%f, %f, %f\r\n", pMesh->m_AabbBox.vMax.x, pMesh->m_AabbBox.vMax.y, pMesh->m_AabbBox.vMax.z);
//				fprintf(fp, "LocalBBoxvMin=%f, %f, %f\r\n", pMesh->m_AabbBox.vMin.x, pMesh->m_AabbBox.vMin.y, pMesh->m_AabbBox.vMin.z);
//				fprintf(fp, "\r\n");
//				++n;
//			}
//			fclose(fp);
//			
//			if(ExportOption()->bGroupExport)
//				SetSaveFilePath(pFileName, MAX_PATH);
//		}
//	}
//}
//
//void KParser::SaveMeshVertexAnimation(const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress)
//{
//	for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;
//		if(!pMesh->m_mapPointCache.empty())
//		{
//			SaveMeshVertexAnimationToFile(pMesh, pMesh->m_szMeshName.c_str(), pOutDir, this, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo, bCompress);
//			//SaveMeshVertexAnimationToFile_Compress(pMesh, pMesh->m_szMeshName.c_str(), pOutDir, this, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo, bCompress);
//		}
//	}
//}
//
//void KParser::SaveMaterail(const char *szOutDir, std::vector<KMesh*>& vecMesh)
//{
//	for(vector<KMesh *>::iterator it = vecMesh.begin(); it != vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;
//		SaveMaterailFile(pMesh, szOutDir, m_szTgaOutPath, this);
//	}
//}
//
//void KParser::SaveJsoninspack(const char *szOutDir, vector<KMesh*>& vecMesh)
//{
//	char szOutJsoninspackName[MAX_PATH] = {0};
//	for(vector<KMesh *>::iterator it = vecMesh.begin(); it != vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;
//		memset(szOutJsoninspackName, 0, sizeof(szOutJsoninspackName));
//		SaveJsoninspackFile(szOutJsoninspackName, pMesh, szOutDir, m_szTgaOutPath, m_szName.c_str(), m_bMdlParser);
//		AddModleInfoJsonInspackPath(pMesh->m_szMeshName, string(szOutJsoninspackName));
//	}
//}
//
//void KParser::SaveJsonModel(const char *szOutDir)
//{
//	for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
//	{
//		KMesh *pMesh = *it;
//		SaveJsonModelFile(this, pMesh, szOutDir, m_szName.c_str(), m_bMdlParser);
//	}
//}

void KParser::Parse(FbxScene* pScene, const char *szName, const char *szParentPathName, FbxManager* pSdkManager, BOOL bMdlParser)
{
	m_pSdkManager = pSdkManager;	
	Clear();
	m_szName = szName;
	m_szMdlName = szName;
	m_pScene = pScene;
	m_bMdlParser = bMdlParser; 

	FbxNode* pRootNode = pScene->GetRootNode();

 //   ValidateParseBlendShape(pRootNode);

	//if (ExportOption()->bModelFileExport && !ExportOption()->bGroupExport)
	//	ConstructModel(m_szName, 0);
	////先解析有权重值的骨骼
	//ParseMeshAndBoneWeights(pRootNode);
	//if(!m_mapRootBone.empty())
	//{	
	//	//解析骨架
	//	ParseSkeleton(pRootNode);

	//	//解析动画帧
	//	ParseAnimation(pScene);		

	//	//保存动画文件
	//	if(bMdlParser)
	//	{
	//		FixBipLinks();
	//		FixSocketBones(pRootNode);
	//		//SaveBipAnimationToFile(szName, szOutDir);
	//	}
	//	//else
	//	//{
	//	//	for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
	//	//	{
	//	//		KMesh *pMesh = *it;
	//	//		pMesh->SaveMeshAnimationToFile(pMesh->m_szMeshName.c_str(), szOutDir, this);
	//	//	}
	//	//}
	//}

	//SaveMesh(szOutDir);
	//SaveMaterail(szOutDir);

	//if(!m_mapRootBone.empty() && bMdlParser)
	//{
	//	SaveMdlFile(szOutDir);
	//	SaveBipFile(szOutDir);
	//}
}


//void KParser::Save(const char *szOutPath, const char* pszTgaOutPath, BOOL bOnlyAnimationChecked, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress, float fbxScale, BOOL bChangeYZ)
//{
//	sprintf_s(m_szTgaOutPath, FILENAME_MAX, "%s", pszTgaOutPath);
//	ExportOption()->bOnlyAniExport = bOnlyAnimationChecked;
//	//if(ExportOption()->fFbxScale != fbxScale || ExportOption()->bExchangeYZ != bChangeYZ)
//	//{
//	//	ExportOption()->fFbxScale = fbxScale;
//	//	ExportOption()->bExchangeYZ = bChangeYZ;
//	//	//如果选项改变了，那么mesh里面的蒙皮姿势矩阵要重新计算一次，否则会有问题	
//	//	for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
//	//	{
//	//		KMesh *pMesh = *it;
//	//		pMesh->BuildBoneMatrixOffset();
//	//	}
//	//}
//	for (MAP_MODEL_SET::iterator it = m_mapModelSet.begin(); it != m_mapModelSet.end(); ++it)
//	{
//		it->second->ClearCache();
//	}
//
//	if(!m_mapRootBone.empty())
//	{
//		//保存动画文件
//		if(m_bMdlParser)
//		{
//			SaveBipAnimationToFile(m_szName.c_str(), szOutPath, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo, bCompress);
//		}
//		else
//		{
//			for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
//			{
//				KMesh *pMesh = *it;
//				SaveMeshAnimationToFile(pMesh, pMesh->m_szMeshName.c_str(), szOutPath, this, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo, bCompress);
//			}
//		}
//	}
//
//	if(!bOnlyAnimationChecked)
//	{	
//		
//		if(!ExportOption()->bGroupExport && m_vecMesh.size() > 1 && !m_bMdlParser)
//		{
//			MergeSaveAll(szOutPath);
//		}
//		else
//		{		
//			
//			SaveMesh(szOutPath, m_vecMesh);
//			SaveMaterail(szOutPath, m_vecMesh);
//			SaveJsoninspack(szOutPath, m_vecMesh);
//			SaveMeshVertexAnimation(szOutPath, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo, bCompress);
//			SaveCollision(szOutPath);
//			SaveMeshVertexAnimation(szOutPath, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo, bCompress);
//
//			// lod
//			MAP_LOD_MESH::iterator it = m_mapLodMesh.begin();
//			for (; it != m_mapLodMesh.end(); ++it)
//			{
//				SaveMesh(szOutPath, it->second);
//				SaveMaterail(szOutPath, it->second);
//				SaveJsoninspack(szOutPath, it->second);
//			}
//			
//			SaveJsonModel(szOutPath);
//
//			if(!m_mapRootBone.empty() && m_bMdlParser)
//			{
//				SaveMdlFile(szOutPath);
//				SaveBipFile(szOutPath);
//			}
//		}		
//	}
//}
//
//void KParser::AddBipRootBone(const char *pcszBoneName)
//{
//	m_mapRootBone[pcszBoneName] = 0;
//}
//
//
//BOOL KParser::IsMdlParser()
//{
//	return m_bMdlParser;
//}
//
//BOOL KParser::IsMarchBlackName(const char *pName)
//{
//	BOOL bMarch = FALSE;	
//	for(vector<string>::iterator it =  m_vecBlackListFilter.begin(); it != m_vecBlackListFilter.end(); ++it)
//	{
//		string szBlack = *it;
//		const char *pBlack = szBlack.c_str();
//		size_t nLen =  strlen(pBlack);
//		if(pBlack[0] == '*')
//		{
//			//begin *
//			char key[64] = {0};
//			strncpy(key, szBlack.c_str() + 1, nLen - 1);
//			if(EndWithStr(pName, key))
//			{
//				bMarch = TRUE;
//				break;
//			}
//		}
//		else if(pBlack[nLen -1] == '*')
//		{
//			// end *
//			if(strncmp(pName, pBlack, sizeof(char)* (nLen - 1)) == 0)
//			{
//				bMarch = TRUE;
//				break;
//			}			
//		}
//		else
//		{
//			//all
//			if(strcmp(pName, pBlack) == 0)
//			{
//				bMarch = TRUE;
//				break;
//			}
//		}
//	}
//	return bMarch;
//}
//
//void KParser::SaveMdlFile(const char *szOutDir)
//{
//	// todo.ksh 应该mdl也需要hd的后缀，以后再加
//	char fileName[MAX_PATH];
//	int len = (int) strlen(szOutDir);
//	if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
//		sprintf(fileName, "%s%s.mdl", szOutDir, m_szMdlName.c_str());
//	else
//		sprintf(fileName, "%s\\%s.mdl", szOutDir, m_szMdlName.c_str());	
//	
//	FILE *fp = fopen(fileName, "wb");
//	if(fp)
//	{
//		fprintf(fp, "%s.txt\r\n", m_szMdlName.c_str());
//		for(vector<KMesh *>::iterator it = m_vecMesh.begin(); it != m_vecMesh.end(); ++it)
//		{
//			KMesh *pMesh = *it;
//			fprintf(fp, "%s.mesh\r\n", pMesh->m_szMeshName.c_str());
//		}
//		fclose(fp);
//
//		SetSaveFilePath(fileName, MAX_PATH);
//	}
//}
//
//void KParser::SaveBipFile(const char *szOutDir)
//{
//	char fileName[MAX_PATH];
//	int len = (int) strlen(szOutDir);
//	if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
//		sprintf(fileName, "%s%s.txt", szOutDir, m_szMdlName.c_str());
//	else
//		sprintf(fileName, "%s\\%s.txt", szOutDir, m_szMdlName.c_str());	
//		
//
//	if(m_pMartchSkeleton)
//	{
//		//有匹配的预设骨架配置文件，直接复制过去
//		char szSrcFile[MAX_PATH];
//		sprintf(szSrcFile, "cfg\\%s", m_pMartchSkeleton->szFileName.c_str());
//		CopyFile(szSrcFile, fileName, FALSE);
//	}
//	else
//	{	
//		FILE *fp = fopen(fileName, "wb");
//		if(fp)
//		{
//			int nBoneCount = (int) m_vecBipBone.size();
//			fprintf(fp, "%d\r\n", nBoneCount);
//
//			for(int i = 0; i < nBoneCount; ++i)
//			{
//				KBipBone *pBone = m_vecBipBone[i];
//				fprintf(fp, "%s", pBone->szBoneName);
//				for(vector<string>::iterator it = pBone->m_vecChildNames.begin(); it != pBone->m_vecChildNames.end(); ++it)
//				{
//					string &childboneName = *it;
//					fprintf(fp, "  %s", childboneName.c_str());
//				}
//				fprintf(fp, "\r\n");
//			}
//
//			int nRootBoneCount = (int) m_mapRootBone.size();
//			fprintf(fp, "%d\r\n", nRootBoneCount);
//			for(map<string, int>::iterator it = m_mapRootBone.begin(); it != m_mapRootBone.end(); ++it)
//			{
//				int id = it->second;
//				fprintf(fp, "%d\r\n", id);
//			}
//			fclose(fp);
//		}
//	}
//}
//
//void KParser::SaveCollision(const char *szOutDir)
//{
//	if (ExportOption()->bCollision)
//	{
//		char szOutMeshName[MAX_PATH] = {0};
//		for (auto it = m_vecCollisionMesh.begin(); it != m_vecCollisionMesh.end(); ++it)
//		{
//			KMesh* pMesh = *it;
//			memset(szOutMeshName, 0, sizeof(szOutMeshName));
//			SaveCollisionMeshFile(szOutMeshName, pMesh, szOutDir, m_szName.c_str());
//			AddModelInfoLODPath(pMesh->m_szMeshName, szOutMeshName, true);
//		}
//	}
//}
//
//BOOL KMeshBone::ComputBoneWeight(KMesh *pKMesh, int nBoneId)
//{
//	BOOL bRet = TRUE;
//	if(m_pCluster)
//	{
//		int k, lIndexCount = m_pCluster->GetControlPointIndicesCount();
//
//		int* lIndices = m_pCluster->GetControlPointIndices();
//		double* lWeights = m_pCluster->GetControlPointWeights();
//
//		for(k = 0; k < lIndexCount; ++k)
//		{		
//			int nControlPointId = lIndices[k];
//			float fWeight = (float)lWeights[k];
//			KVec3 controlpoint = pKMesh->m_vecControlPoints[nControlPointId];
//
//
//			if(find(m_vecControllPoints.begin(), m_vecControllPoints.end(), controlpoint) == m_vecControllPoints.end())
//			{
//				//控制点可能重复，这里去重
//				m_vecControllPoints.push_back(controlpoint);
//				m_vecControllPointsWeight.push_back(fWeight);
//			}
//
//
//			//////////////////////////////////////
//			//for check，最终蒙皮顶点导出的蒙皮姿势计算的fix转换需要用这个列表				
//			KWeight *pWeight = NULL;
//			map<KVec3, KWeight*>::iterator it = pKMesh->m_mapWeightControlPointMapping.find(controlpoint);				
//			if(it != pKMesh->m_mapWeightControlPointMapping.end())
//			{
//				pWeight = it->second;
//			}
//			else
//			{
//				pWeight = new KWeight;
//				pKMesh->m_mapWeightControlPointMapping.insert(pair<KVec3, KWeight *>(controlpoint, pWeight));
//			}
// 			BOOL bRetCode = pWeight->AddWeight(nBoneId, fWeight);
//			if(!bRetCode)
//			{
//				bRet = FALSE;
//			}
//			///////////////////////////////////////
//		}
//
//
//		//蒙皮姿势矩阵，回头要蒙皮顶点转换到0time状态下的位置
//		FbxAMatrix initPosMatrix;
//		m_pCluster->GetTransformLinkMatrix(initPosMatrix);			
//		FbxAMatrix time0Mat = pFbxNode->EvaluateGlobalTransform(0);
//		ToMaxMat3(time0Matrix, time0Mat);
//		ToMaxMat3(initPoseMatrixInverse, initPosMatrix);
//		initPoseMatrixInverse.Invert();
//	}
//	return bRet;
//}
//
//
//void KParser::ParseVertexAnimtion(FbxNode* pNode, KMesh *pKMesh, FbxNode *pRootNode)
//{
//	FbxGeometry* pGeometry = pKMesh->m_pFbxMesh;
//	int lVertexCacheDeformerCount = pGeometry->GetDeformerCount( FbxDeformer::eVertexCache );
//	for(int i = 0; i < lVertexCacheDeformerCount; ++i)
//	{
//		FbxVertexCacheDeformer* lDeformer = static_cast<FbxVertexCacheDeformer*>(pGeometry->GetDeformer(i, FbxDeformer::eVertexCache));
//		if( !lDeformer ) continue;
//
//		fbxsdk::FbxCache* lCache = lDeformer->GetCache();
//		if( !lCache ) continue;
//
//		FbxStatus status;
//		lCache->SetCacheFileFormat(fbxsdk::FbxCache::EFileFormat::eMayaCache);
//		if(lCache->OpenFileForRead(&status))
//		{			
//			int lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
//			// skip normal channel
//			if (lChannelIndex < 0)
//				continue;
//
//			FbxString lChnlName, lChnlInterp;
//
//			FbxCache::EMCDataType lChnlType;
//			FbxTime start, stop, rate;
//			FbxCache::EMCSamplingType lChnlSampling;
//
//			unsigned int lChnlSampleCount, lDataCount;
//
//			lCache->GetChannelName(lChannelIndex, lChnlName);			
//			lCache->GetChannelDataType(lChannelIndex, lChnlType);
//			lCache->GetChannelInterpretation(lChannelIndex, lChnlInterp);			
//			lCache->GetChannelSamplingType(lChannelIndex, lChnlSampling);			
//			lCache->GetAnimationRange(lChannelIndex, start, stop);
//			lCache->GetChannelSamplingRate(lChannelIndex, rate);
//			lCache->GetChannelSampleCount(lChannelIndex, lChnlSampleCount);			
//
//			// Only display cache data if the data type is float vector array
//			if (lChnlType != FbxCache::eFloatVectorArray)
//				continue;
//			
//
//			float* lBuffer = NULL;
//			unsigned int lBufferSize = 0;
//			int lFrame = 0;
//			for (FbxTime t = start; t <= stop; t+=rate)
//			{				
//				lCache->GetChannelPointCount(lChannelIndex, t, lDataCount);
//
//				if (lBuffer == NULL)
//				{
//					lBuffer = new float[lDataCount*3];
//					lBufferSize = lDataCount*3;
//				}
//				else if (lBufferSize < lDataCount*3)
//				{
//					delete [] lBuffer;
//					lBuffer = new float[lDataCount*3];
//					lBufferSize = lDataCount*3;
//				}
//				else
//					memset(lBuffer, 0, lBufferSize*sizeof(float));
//
//				lCache->Read(lChannelIndex, t, lBuffer, lDataCount);
//				if (lChnlInterp == "normals")
//				{
//					// display normals cache data
//					// the normal data is per-polygon per-vertex. we can get the polygon vertex index
//					// from the index array of polygon vertex
//					FbxMesh* lMesh = (FbxMesh*)pGeometry;
//
//					if (lMesh == NULL)
//					{
//						// Only Mesh can have normal cache data
//						continue;
//					}
//					
//					int pi, j, lPolygonCount = lMesh->GetPolygonCount();
//					unsigned lNormalIndex = 0;
//					for (pi = 0; pi < lPolygonCount && lNormalIndex+2 < lDataCount*3; pi++)
//					{						
//						int lPolygonSize = lMesh->GetPolygonSize(pi);
//						for (j = 0; j < lPolygonSize && lNormalIndex+2 < lDataCount*3; j++)
//						{
//							FbxVector4 normal(lBuffer[lNormalIndex], lBuffer[lNormalIndex+1], lBuffer[lNormalIndex+2]);
//							//Display3DVector("                       Normal Cache Data  ", normal);
//							lNormalIndex += 3;
//						}
//					}
//				}
//				else
//				{
//					//DisplayInt("               Points Count: ", lDataCount);
//					for (unsigned int j = 0; j < lDataCount*3; j=j+3)
//					{
//						//FbxVector4 points(lBuffer[j], lBuffer[j+1], lBuffer[j+2]);
//						//Display3DVector("                   Points Cache Data: ", points);
//						KVec3 &controlpoint = pKMesh->m_vecControlPoints[j / 3];
//						KMesh::KPointCache &pointCache =  pKMesh->m_mapPointCache[controlpoint];
//						pointCache.id = j / 3;
//						pointCache.vecPointCache.push_back(KVec3(lBuffer[j], lBuffer[j+1], lBuffer[j+2]));
//					}
//				}
//				lFrame++;
//			}
//			pKMesh->m_dwPointCacheFrames = lFrame;
//			if (lBuffer != NULL) 
//			{
//				delete [] lBuffer;
//				lBuffer = NULL;
//			}
//
//			lCache->CloseFile();
//		}
//	}
//
//}
//
//void KParser::ParseLink(FbxNode* pNode, KMesh *pKMesh, FbxNode *pRootNode)
//{
//	int i, j;
//
//	int lSkinCount=0;
//	int lClusterCount=0;
//	FbxCluster* lCluster = NULL;
//	//const char *pp = pNode->GetName();
//
//	FbxGeometry* pGeometry = pKMesh->m_pFbxMesh;
//	lSkinCount=pGeometry->GetDeformerCount(FbxDeformer::eSkin);
//
//	//先把所有weightbone的名字找出来
//	for(i=0; i!=lSkinCount; ++i)
//	{
//		FbxDeformer* pFbxDeformer = pGeometry->GetDeformer(i, FbxDeformer::eSkin);
//		lClusterCount = ((FbxSkin *) pFbxDeformer)->GetClusterCount();
//		for (j = 0; j != lClusterCount; ++j)
//		{
//			lCluster=((FbxSkin *) pFbxDeformer)->GetCluster(j);						
//			int lIndexCount = lCluster->GetControlPointIndicesCount();
//			if(lIndexCount == 0)
//			{
//				continue;
//			}			
//			const char *weightBoneName =  lCluster->GetLink()->GetName();
//			pKMesh->m_weightBoneNames.insert(weightBoneName);
//		}		
//	}
//	
//
//
//	//lLinkCount = pGeometry->GetLinkCount();
//	for(i=0; i!=lSkinCount; ++i)
//	{
//		FbxDeformer* pFbxDeformer = pGeometry->GetDeformer(i, FbxDeformer::eSkin);
//		
//		FbxArray<FbxString*> array = ((FbxSkin *) pFbxDeformer)->GetNameSpaceArray(0);
//		
//		lClusterCount = ((FbxSkin *) pFbxDeformer)->GetClusterCount();
//		for (j = 0; j != lClusterCount; ++j)
//		{
//			lCluster=((FbxSkin *) pFbxDeformer)->GetCluster(j);			
//
//			int  lIndexCount = lCluster->GetControlPointIndicesCount();
//
//			if(lIndexCount == 0)
//			{
//				//没有影响权重的骨骼，那么可能是末端的骨骼，这可以忽略掉的
//				continue;
//			}
//
//			pKMesh->AddMeshBone(lCluster, pRootNode, this);
//
//
//			//int* lIndices = lCluster->GetControlPointIndices();
//			//double* lWeights = lCluster->GetControlPointWeights();
//
//			//FbxNode *pWeightNode = lCluster->GetLink();
//		    //pKMesh->BuildRootBoneByWeightBone(pWeightNode, pRootNode, this);
//			//KMeshBone *pBone = pKMesh->CreateMeshBone(lCluster->GetLink());
//			
//			//KMeshBone *pBone = new KMeshBone;
//			//pKMesh->AddMeshBone(pBone);
//			//pBone->pFbxNode = pWeightNode;
//			//strcpy(pBone->szBoneName, lCluster->GetLink()->GetName());
//			
//			//pBone->m_pCluster = lCluster;
//
//			//pBone->BuildBoneWeight(pKMesh, pBone->nBoneId);
//			/*
//			for(k = 0; k < lIndexCount; ++k)
//			{
//				int nControlPointId = lIndices[k];
//				float fWeight = (float)lWeights[k];
//				KVec3 controlpoint = pKMesh->m_vecControlPoints[nControlPointId];
//
//				if(find(pBone->m_vecControllPoints.begin(), pBone->m_vecControllPoints.end(), controlpoint) == pBone->m_vecControllPoints.end())
//				{
//					//控制点可能重复，这里去重
//					pBone->m_vecControllPoints.push_back(controlpoint);
//					pBone->m_vecControllPointsWeight.push_back(fWeight);
//				}
//
//
//				//////////////////////////////////////
//				//for check，最终蒙皮顶点导出的蒙皮姿势计算的fix转换需要用这个列表				
//				KWeight *pWeight = NULL;
//				map<KVec3, KWeight*>::iterator it = pKMesh->m_mapWeightControlPointMapping.find(controlpoint);				
//				if(it != pKMesh->m_mapWeightControlPointMapping.end())
//				{
//					pWeight = it->second;
//				}
//				else
//				{
//					pWeight = new KWeight;
//					pKMesh->m_mapWeightControlPointMapping.insert(pair<KVec3, KWeight *>(controlpoint, pWeight));
//				}
//				pWeight->AddWeight(pBone->nBoneId, fWeight);
//				///////////////////////////////////////
//			}
//			
//
//			//蒙皮姿势矩阵，回头要蒙皮顶点转换到0time状态下的位置
//			FbxAMatrix initPosMatrix;
//			lCluster->GetTransformLinkMatrix(initPosMatrix);			
//			FbxAMatrix time0Mat = pBone->pFbxNode->EvaluateGlobalTransform(0);
//			ToMaxMat3(pBone->time0Matrix, time0Mat);
//			ToMaxMat3(pBone->initPoseMatrixInverse, initPosMatrix);
//			pBone->initPoseMatrixInverse.Invert();
//			*/
//
//			//////////////////////////test
//			//FbxAMatrix mat0 = pKMesh->m_pFbxNode->EvaluateGlobalTransform(0);
//			//FbxAMatrix mat1 = pBone->pFbxNode->EvaluateGlobalTransform(0);
//			//Matrix3 maxMat0;
//			//ToMaxMat3(maxMat0, mat0);
//
//			//Matrix3 maxMat1;
//			//ToMaxMat3(maxMat1, mat1);
//
//			//Matrix3 maxMat2;
//			//ToMaxMat3(maxMat2, lMatrix);
//
//		
//			//Point3 p(32.162f, 0.0f, 11.442f);
//			//maxMat2.Invert();
//			//Point3 pp = p * maxMat2 * maxMat1;
//			//int x = 0;
//			////////////////////////////
//			
//			
//
//			//BOOL bFlexBone = false;
//			//if(_IsFirstFlexiBoneName(pBone->szBoneName) || _IsSubFlexiBoneName(pBone->szBoneName))
//			//{
//			//	bFlexBone = true;
//			//}
//
//			//if(!bFlexBone)
//			//{
//				//todo:这里要过滤去掉物理骨骼，插槽之类的东西
//				//map<string, KBipBone *>::iterator it = m_mapBipBone.find(pBone->szBoneName);
//				//if(it == m_mapBipBone.end())
//				//{
//				//	KBipBone *pBipBone = new KBipBone;
//				//	strcpy(pBipBone->szBoneName, pBone->szBoneName);
//				//	m_mapBipBone.insert(pair<string, KBipBone *>(pBone->szBoneName, pBipBone));
//				//}
//			//}
//		}
//	}
//}
//
//
//
//
//void KParser::ParseMesh(FbxNode* pNode, FbxNode* pRootNode)
//{
//	FbxNodeAttribute::EType lAttributeType;
//	int i;
//    const char *pNodeName1 = pNode->GetName();
//	if(pNode->GetNodeAttribute() == NULL)
//	{
//		_FBXSDK_printf("NULL Node Attribute\n\n");
//	}
//	else
//	{
//		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());       
//
//		if (lAttributeType == FbxNodeAttribute::eMesh)
//		{
//			FbxMesh* pMesh = (FbxMesh*) pNode->GetNodeAttribute ();
//
//			const char *pNodeName = pNode->GetName();
//			BOOL bValidMeshName = TRUE;
//			if(m_bMdlParser)
//			{
//				//如果是导出mdl，需要按partlist过滤mesh
//				bValidMeshName = FALSE;
//				for(vector<string>::iterator it = m_vecPartsFilter.begin(); it != m_vecPartsFilter.end(); ++it)
//				{
//					string szPart = *it;
//					
//					if(EndWithStr(pNodeName, szPart.c_str()))
//					{
//						bValidMeshName = TRUE;
//						break;
//					}
//				}
//			}
//			FbxNodeAttribute* att = pNode->GetNodeAttribute();
//			int nMatCount = pMesh->GetElementMaterialCount();
//			//需要导出的Mesh一定包含材质信息的，否则可能是一些虚拟体的Mesh，这种不用作为单独的Mesh导出
//			//if(nMatCount && bValidMeshName)
//			if(att->GetAttributeType() == FbxNodeAttribute::eMesh && bValidMeshName)
//			//if(bValidMeshName)
//			{			
//				KMesh *pKMesh = new KMesh;
//				string strPureName = string(pNode->GetName());
//				bool bIsCollision = IsCollisionNode(pNode);
//				// use name without _lodxx
//				if (ExportOption()->bModelFileExport)
//				{
//					int nLOD = -1;
//					strPureName = GetMeshPureName(&nLOD, pNode->GetName(), bIsCollision);
//					pKMesh->m_nLOD = nLOD;
//				}
//
//				if (bIsCollision)
//				{
//					m_vecCollisionMesh.push_back(pKMesh);
//				}
//				else
//				{
//					ConstructModel(strPureName, pKMesh->m_nLOD);
//
//					if (pKMesh->m_nLOD <= 0)
//					{
//						m_vecMesh.push_back(pKMesh);
//					}
//					else
//					{
//						MAP_LOD_MESH::iterator it = m_mapLodMesh.find(pKMesh->m_nLOD);
//						if (it == m_mapLodMesh.end())
//						{
//							vector<KMesh*> vecMesh;
//							vecMesh.push_back(pKMesh);
//							m_mapLodMesh[pKMesh->m_nLOD] = vecMesh;
//						}
//						else
//						{
//							it->second.push_back(pKMesh);
//						}
//					}
//					
//				}
//				char szName[MAX_PATH];
//				ConvertUTF8ToGBK(strPureName.c_str(), szName);
//				pKMesh->m_szMeshName = szName;				
//                bool bIsTriangleMesh = pMesh->IsTriangleMesh();
//                if (m_bParseBlendShape)
//                {
//                    if (!bIsTriangleMesh)
//                    {
//                        _FBXSDK_printf("Mesh is not Trianglized! The Blend Shapes will NOT Export correctly!!");
//                        FbxGeometryConverter converter(m_pSdkManager);
//                        pMesh = (FbxMesh*) converter.Triangulate(pMesh, true);
//                        converter.ComputeEdgeSmoothingFromNormals(pMesh);
//                        converter.ComputePolygonSmoothingFromEdgeSmoothing(pMesh);
//                    }                    
//                }
//                else
//                {
//                    FbxGeometryConverter converter(m_pSdkManager);
//                    pMesh = (FbxMesh*) converter.Triangulate(pMesh, true);
//                    converter.ComputeEdgeSmoothingFromNormals(pMesh);
//                    converter.ComputePolygonSmoothingFromEdgeSmoothing(pMesh);
//                }
//				pKMesh->m_pFbxMesh = pMesh;
//				pKMesh->m_pFbxNode = pNode;
//
//				ParseControlsPoints(pKMesh);	
//				ParseVertexAnimtion(pNode, pKMesh, pRootNode);
//				ParseLink(pNode, pKMesh, pRootNode);
//
//
//				pKMesh->BuildBoneLinkNames();
//				pKMesh->BuildBoneMatrixOffset();
//				//pKMesh->BuildSocketList();
//
//				pKMesh->FixLinks(this);
//				pKMesh->BuildMeshBoneWeight(this);
//				ParseFaces(pKMesh, this);
//                pKMesh->ParseAnchorPoints();
//                pKMesh->ParseSplines();
//			}
//		}
//	}
//	
//    int nChildCount = pNode->GetChildCount();
//	for(i = 0; i < nChildCount; i++)
//	{
//		ParseMesh(pNode->GetChild(i), pRootNode);
//	}
//}
//
//void KParser::ParseMeshAndBoneWeights(FbxNode* pRootNode)
//{
//	if(pRootNode)
//	{
//        int nChildCount = pRootNode->GetChildCount();
//		for(int i = 0; i < nChildCount; i++)
//		{
//			ParseMesh(pRootNode->GetChild(i), pRootNode);
//		}
//	}
//}
//
//
//
//void KParser::ParseSkeleton(FbxNode* pRootNode)
//{		
//	//找到有效的节点，和每个节点的正真父节点
//	for(int i = 0; i < pRootNode->GetChildCount(); i++)
//	{
//		FbxNode *pChild = pRootNode->GetChild(i);
//		const char *pChildName = pChild->GetName();
//		map<string, int>::iterator it = m_mapRootBone.find(pChildName);
//		if(it != m_mapRootBone.end())
//		{
//			ParseHierarchy(NULL, pChild, 0);	
//		}		
//	}
//
//	////给每个节点添加子节点的名字
//	for(vector<KBipBone *>::iterator it = m_vecBipBone.begin(); it != m_vecBipBone.end(); ++it)
//	{
//		KBipBone *pBone = *it;
//		for(vector<KBipBone *>::iterator itt = m_vecBipBone.begin(); itt != m_vecBipBone.end(); ++itt)
//		{
//			KBipBone *pBoneSel = *itt;
//			if(pBone == pBoneSel->pParentBone)
//			{
//				pBone->m_vecChildNames.push_back(pBoneSel->szBoneName);
//			}
//		}
//	}
//}
//
//void KParser::ParseHierarchy(FbxNode* pParentFbxNode, FbxNode* pNode, int pDepth)
//{
//	KBipBone *pParentBone = NULL;
//	if(pParentFbxNode)
//	{	
//		for(vector<KBipBone *>::iterator it = m_vecBipBone.begin(); it != m_vecBipBone.end(); ++it)
//		{
//			KBipBone *pBipBone = *it;
//			const char *parentName = pParentFbxNode->GetName();
//			if(strcmp(pBipBone->szBoneName, parentName) == 0)
//			{
//				pParentBone = pBipBone;
//				break;
//			}
//		}
//
//		if(!pParentBone)
//		{
//			for(vector<KBipBone *>::iterator it = m_vecIgnoreBipBone.begin(); it != m_vecIgnoreBipBone.end(); ++it)
//			{
//				KBipBone *pBipBone = *it;
//				const char *parentName = pParentFbxNode->GetName();
//				if(strcmp(pBipBone->szBoneName, parentName) == 0)
//				{
//					pParentBone = pBipBone;
//					break;
//				}
//			}
//		}
//	}
//
//	const char *pBoneName = pNode->GetName();	
//	if(IsBipNameIgnore(pNode))
//	{
//		return;
//	}
//
//	//if(!IsMarchBlackName(pBoneName))
//	{
//		KBipBone *pBipBone = new KBipBone;
//		strncpy(pBipBone->szBoneName, pBoneName, 30);
//		pBipBone->pFbxNode = pNode;
//		
//		//默认情况
//		pBipBone->pParentBone = pParentBone;
//
//		//过滤掉black列表的节点，重新链接该节点的上层父节点
//		while(pParentBone)
//		{
//			const char *pParentBoneName = pParentBone->pFbxNode->GetName();
//			if(!IsMarchBlackName(pParentBoneName))
//			{
//				pBipBone->pParentBone = pParentBone;
//				break;
//			}
//			pParentBone = pParentBone->pParentBone;
//		}
//
//		if(!IsMarchBlackName(pBoneName))
//		{
//			m_vecBipBone.push_back(pBipBone);
//		}
//		else
//		{
//			m_vecIgnoreBipBone.push_back(pBipBone);
//		}
//
//		if(pParentFbxNode == NULL)
//		{
//			//根骨骼
//			int nCount = (int) m_vecBipBone.size();
//			m_mapRootBone[pBipBone->szBoneName] = nCount - 1;
//		}
//	}
//
//	for(int i = 0; i < pNode->GetChildCount(); i++)
//	{
//		FbxNode *pChildNode = pNode->GetChild(i);
//		const char *pChildName = pChildNode->GetName();
//		ParseHierarchy(pNode, pChildNode, pDepth + 1);
//	}
//
//
//}
//
////void KParser::ParseHierarchy(FbxNode* pParentFbxNode, FbxNode* pNode, int pDepth)
////{
////	KBipBone *pParentNode = NULL;
////	if(pParentFbxNode)
////	{	
////		for(vector<KBipBone *>::iterator it = m_vecBipBone.begin(); it != m_vecBipBone.end(); ++it)
////		{
////			KBipBone *pBipBone = *it;
////			const char *parentName = pParentFbxNode->GetName();
////			if(strcmp(pBipBone->szBoneName, parentName) == 0)
////			{
////				pParentNode = pBipBone;
////			}
////		}
////	}
////
////	const char *pBoneName = pNode->GetName();	
////	if(IsBipNameIgnore(pNode))
////	{
////		return;
////	}
////	
////
////	KBipBone *pBipBone = new KBipBone;
////	strncpy(pBipBone->szBoneName, pBoneName, 30);
////	pBipBone->pFbxNode = pNode;
////	pBipBone->pParentBone = pParentNode;
////	m_vecBipBone.push_back(pBipBone);
////
////
////	if(pParentFbxNode == NULL)
////	{
////		//根骨骼
////		int nCount = (int) m_vecBipBone.size();
////		m_mapRootBone[pBipBone->szBoneName] = nCount - 1;
////	}
////
////	for(int i = 0; i < pNode->GetChildCount(); i++)
////	{
////		FbxNode *pChildNode = pNode->GetChild(i);
////		const char *pChildName = pChildNode->GetName();
////		if(!IsBipNameIgnore(pChildNode))
////		{
////			if(IsMarchBlackName(pChildName))
////			{
////				//子节点有可能是过Black控制节点，过滤掉，遍历孙子节点
////				int nGrandSonCount = pChildNode->GetChildCount();
////				for(int j = 0; j < nGrandSonCount; ++j)
////				{
////					FbxNode *pGrandSonNode = pChildNode->GetChild(j);
////					const char *pGrandSonName = pGrandSonNode->GetName();
////					if(!IsBipNameIgnore(pGrandSonNode) && !IsMarchBlackName(pGrandSonName))
////					{
////					   pBipBone->m_vecChildNames.push_back(pGrandSonName);
////					   ParseHierarchy(pNode, pGrandSonNode, pDepth + 1);
////					}
////				}
////			}
////			else
////			{
////				if(!IsBipNameIgnore(pChildNode) && !IsMarchBlackName(pChildName))
////				{
////					pBipBone->m_vecChildNames.push_back(pChildName);
////					ParseHierarchy(pNode, pChildNode, pDepth + 1);
////				}
////			}
////		}		
////	}
////}
//
//KBipBone *KParser::GetBone(const char *szBoneName)
//{
//	KBipBone *pFindBone = NULL;
//	for(vector<KBipBone *>::iterator it = m_vecBipBone.begin(); it != m_vecBipBone.end(); ++it)
//	{
//		KBipBone *pBipBone = *it;
//		if(strcmp(pBipBone->szBoneName,szBoneName) == 0)
//		{
//			pFindBone = pBipBone;
//			break;
//		}
//	}
//	return pFindBone;
//}
//
//void KParser::FixBipLinks()
//{
//	for(map<string, string>::iterator it = m_mapRelink.begin(); it != m_mapRelink.end(); ++it)
//	{
//		string childName = it->first;
//		string newParentName = it->second;
//		KBipBone *pChildBone = NULL;
//		KBipBone *pNewParentBone = NULL;
//		KBipBone *pOldParentBone = NULL;
//
//		pChildBone = GetBone(childName.c_str());
//		if(!pChildBone)
//			continue;
//
//
//		pNewParentBone = GetBone(newParentName.c_str());
//		pOldParentBone = pChildBone->pParentBone;
//		
//		pChildBone->pParentBone = pNewParentBone;
//
//		if(pOldParentBone)
//		{
//			BOOL bFind = FALSE;
//			for(vector<string>::iterator it3 = pOldParentBone->m_vecChildNames.begin(); it3 != pOldParentBone->m_vecChildNames.end(); ++it3)
//			{
//				string &_childName = *it3;
//				if(_childName == childName)
//				{
//					bFind = TRUE;
//					pOldParentBone->m_vecChildNames.erase(it3);
//					break;
//				}
//			}
//			assert(bFind);
//		}
//
//		if(pNewParentBone)
//		{
//			if(find(pNewParentBone->m_vecChildNames.begin(), pNewParentBone->m_vecChildNames.end(), childName) ==  pNewParentBone->m_vecChildNames.end())
//			{
//				pNewParentBone->m_vecChildNames.push_back(childName);
//			}
//			else
//			{
//				printf("error, new parent bipBone has already added this bone!");
//				errorMsg.append("error, new parent bipBone has already added this bone!\r\n");
//				assert(0);
//			}
//		}
//	}
//}
//
//void KParser::CounterFrames(FbxAnimCurve* pAnimCurve)
//{
//	int lKeyCount = pAnimCurve->KeyGetCount();
//	FbxTimeSpan span;
//	bool b = pAnimCurve->GetTimeInterval(span);
//	FbxTime tm0 = span.GetStart();
//	FbxTime tm1 = span.GetStop();
//	FbxTime tmDuration = span.GetDuration();
//	float aniTimeLen = (float)tm1.GetMilliSeconds();
//	m_nFrameTimeFrameMilSecond = (int)(aniTimeLen / (float)lKeyCount);
//	if(m_nAnimationFrames < lKeyCount)
//	{
//		m_nAnimationFrames = lKeyCount;
//	}
//	
//}
//
//void KParser::ParseAnimationLayer(FbxAnimLayer* pAnimLayer, FbxNode* pNode)
//{
//	int lModelCount;	
//	const char *pName = pNode->GetName();
//
//	
//
//	KBipBone *pBone = NULL;	
//
//	for(vector<KBipBone *>::iterator it = m_vecBipBone.begin(); it != m_vecBipBone.end(); ++it)
//	{
//		KBipBone *pBipBone = *it;
//		if(strcmp(pBipBone->szBoneName, pName) == 0)
//		{
//			pBone = pBipBone;
//			break;
//		}
//	}	
//	
//	if(pBone)
//	{		
//		FbxAnimCurve* pAnimCurve = NULL;
//
//			
//			pAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
//			if (pAnimCurve)
//			{			
//				CounterFrames(pAnimCurve);		
//			}
//			pAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//			if (pAnimCurve)
//			{
//				CounterFrames(pAnimCurve);
//			}
//			pAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
//			if (pAnimCurve)
//			{
//				CounterFrames(pAnimCurve);
//			}
//
//			pAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
//			if (pAnimCurve)
//			{
//				CounterFrames(pAnimCurve);
//			}
//			pAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//			if (pAnimCurve)
//			{
//				CounterFrames(pAnimCurve);
//			}
//			pAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
//			if (pAnimCurve)
//			{
//				CounterFrames(pAnimCurve);
//			}
//
//			pAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
//			if (pAnimCurve)
//			{
//				CounterFrames(pAnimCurve);
//			}    
//			pAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//			if (pAnimCurve)
//			{		
//				CounterFrames(pAnimCurve);
//			}
//			pAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
//			if (pAnimCurve)
//			{
//				CounterFrames(pAnimCurve);
//			}
//	}
//	
//	int lCount = pNode->GetChildCount();
//	for(lModelCount = 0; lModelCount < lCount; lModelCount++)
//	{
//		ParseAnimationLayer(pAnimLayer, pNode->GetChild(lModelCount));
//	}
//}
//
//void KParser::ParseAnimation(FbxScene* pScene)
//{
//	FbxAnimStack* pAnimStack = pScene->GetSrcObject<FbxAnimStack>(0);
//	if(pAnimStack)
//	{
//		int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
//		if(nbAnimLayers == 0)
//		{
//			return;
//		}
//
//		FbxAnimLayer* pAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(0);
//		ParseAnimationLayer(pAnimLayer, pScene->GetRootNode());
//	}
//}
//
//KSkeletonFileConfig* KParser::DoSkeletonMartch()
//{
//	m_pMartchSkeleton = NULL;
//	for(vector<KSkeletonFileConfig *>::iterator it = m_vecSkeletonFile.begin(); it != m_vecSkeletonFile.end(); ++it)
//	{
//		KSkeletonFileConfig *pConfig = *it;		
//		if(pConfig->SkeletonList.size() != m_vecBipBone.size())
//		{
//			continue;
//		}
//		for(vector<KSkeletonTextBone>::iterator it2 = pConfig->SkeletonList.begin(); it2 != pConfig->SkeletonList.end(); ++it2)
//		{
//			KSkeletonTextBone &boneItem = *it2;
//			KBipBone *pBipBone = GetBone(boneItem.szBoneName.c_str());
//			if(!pBipBone)
//			{				
//				goto ExitLoop;
//			}
//			if(pBipBone->m_vecChildNames.size() != boneItem.m_setChildren.size())
//			{				
//				goto ExitLoop;
//			}
//			for(vector<string>::iterator it = pBipBone->m_vecChildNames.begin(); it != pBipBone->m_vecChildNames.end(); ++it)
//			{
//				string &childName = *it;
//				if(boneItem.m_setChildren.find(childName) == boneItem.m_setChildren.end())
//				{
//					goto ExitLoop;
//				}
//			}
//		}
//
//		m_pMartchSkeleton = pConfig;
//ExitLoop:
//		break;
//	}
//
//	return m_pMartchSkeleton;
//}
//
//void KParser::ValidateParseBlendShape(FbxNode* pRootNode)
//{
//    if (!pRootNode)
//    {
//        m_bParseBlendShape = FALSE;
//        return;
//    }
//
//    auto _DoValidate = [] (FbxNode* pNode) -> bool
//    {
//        bool bShouldParseBlendShape = false;
//        FbxNodeAttribute::EType lAttributeType;
//
//        if(pNode->GetNodeAttribute() == NULL)
//        {
//            _FBXSDK_printf("NULL Node Attribute\n\n");
//        }
//        else
//        {
//            lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());       
//
//            if (lAttributeType == FbxNodeAttribute::eMesh)
//            {
//                FbxMesh* pMesh = (FbxMesh*) pNode->GetNodeAttribute();
//                int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
//                if (lBlendShapeDeformerCount > 0)
//                {
//                    bShouldParseBlendShape = true;
//                }
//            }
//        }
//        return bShouldParseBlendShape;
//    };
//
//    bool bShouldParseBlendShape = false;
//    int nChildCount = pRootNode->GetChildCount();
//    for(int i = 0; i < nChildCount; i++)
//    {
//        bShouldParseBlendShape = _DoValidate(pRootNode->GetChild(i));
//        if (bShouldParseBlendShape)
//        {
//            break;
//        }
//    }
//    m_bParseBlendShape = bShouldParseBlendShape;
//}
//
//int KParser::IsCollisionNode(FbxNode* pNode)
//{
//	KMesh *pKMesh = nullptr;
//	const char* pNodeName = pNode->GetName();
//	if (EndWithStr(pNodeName, SUFFIX_COLLISION))
//	{
//		FbxNode* pNodeParent = pNode->GetParent();
//		assert(pNodeParent);
//		if (pNodeParent)
//		{
//			const char* pcszParentName = pNodeParent->GetName();
//			string strPureParentName = GetMeshPureName(nullptr, pcszParentName);
//			string strPureName = GetMeshPureName(nullptr, pNodeName, true);
//			if (strPureParentName == strPureName)
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//void KParser::SaveBipAnimationToFile_Compress(const char *szName, const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo)
//{
//	char fileName[MAX_PATH];
//	int len = (int) strlen(pOutDir);
//
//	if(pOutDir[len - 1] == '\\'  || pOutDir[len - 1] == '/')
//	{
//		if(nFrameFrom ==0 && nFrameTo == m_nAnimationFrames)
//		{
//			sprintf(fileName, "%s%s.ani", pOutDir, szName);
//		}
//		else
//		{
//			sprintf(fileName, "%s%s_%d_%d.ani", pOutDir, szName, nFrameFrom, nFrameTo);
//		}
//	}
//	else
//	{
//		if(nFrameFrom ==0 && nFrameTo == m_nAnimationFrames)
//		{
//			sprintf(fileName, "%s\\%s.ani", pOutDir, szName);
//		}
//		else
//		{
//			sprintf(fileName, "%s\\%s_%d_%d.ani", pOutDir, szName, nFrameFrom, nFrameTo);
//		}
//	}
//
//	vector<KBipBone *> vecBipBone;
//	//检测是否和预设骨架文件定义一致，如果一致则采用预设骨架的骨骼顺序进行动画导出，这样可以和老的骨骼动画格式兼容，动画文件也不需要进行转换了
//	KSkeletonFileConfig *pSkeletonFileConfig = DoSkeletonMartch();
//	if(pSkeletonFileConfig)
//	{
//		for(vector<KSkeletonTextBone>::iterator it = pSkeletonFileConfig->SkeletonList.begin(); it != pSkeletonFileConfig->SkeletonList.end(); ++it)
//		{
//			KSkeletonTextBone &bone = *it;
//			KBipBone *pBone = GetBone(bone.szBoneName.c_str());
//			vecBipBone.push_back(pBone);
//		}
//	}
//	else
//	{
//		vecBipBone = m_vecBipBone;
//	}
//
//
//	DWORD32 dwBoneNum = (DWORD32) vecBipBone.size();
//	const char *szAniName = szName;
//
//
//	int i = 0;
//	HRESULT hResult = E_FAIL;
//	HRESULT hRetCode = E_FAIL;
//	KG3DAniFileWrap AniFileWrap;
//	BONERTS_ANI_DATA boneRTSAniData;
//	ANI_FILE_HEADER& Header = boneRTSAniData.fileHeader;
//	BONERTS_ANI_HEAD& RTSHead = boneRTSAniData.boneRTSAniHead;
//	m_nSampleFrameInterval = nSampleInterval;
//	RTSINFO** ppBoneRTS = NULL;	
//	DWORD32 dwNumFrames = (DWORD32) m_nAnimationFrames;
//	strcpy_s(Header.strDesc, MAX_CLIPDESC_LEN, szName);
//	Header.strDesc[MAX_CLIPDESC_LEN - 1] = 0;
//	Header.dwMask = ANI_FILE_MASK;
//	Header.dwBlockLength = sizeof(ANI_FILE_HEADER) + sizeof(BONERTS_ANI_HEAD) + 
//		sizeof(TCHAR) * MAX_CLIPDESC_LEN * dwBoneNum +
//		sizeof(RTSINFO) * dwBoneNum * dwNumFrames +
//		sizeof(DWORD32);//EndFlag
//	Header.dwNumAnimations = 1;
//	Header.dwType = ANIMATION_BONE_RTS;
//
//	RTSHead.dwNumBones = dwBoneNum;
//
//	RTSHead.fFrameLength = nFrameTime;
//
//
//	TCHAR **ppName = NULL;
//
//	if(!m_nAnimationFrames)
//		goto Exit0;
//
//	ppName = new TCHAR*[dwBoneNum];
//
//	for(DWORD32 i_Bone = 0; i_Bone < dwBoneNum; i_Bone++)
//	{
//		const char * BoneName = vecBipBone[i_Bone]->szBoneName;
//		char *pName = new char[30];
//		strncpy(pName, BoneName, 30);
//		ppName[i_Bone] = pName;
//	}
//
//	boneRTSAniData.ppszBoneName = ppName;
//
//	ppBoneRTS = new RTSINFO*[RTSHead.dwNumBones];
//	KGLOG_PROCESS_ERROR(ppBoneRTS);
//
//	i = 0;
//	int nFrames = 0;
//	for(int i = nFrameFrom; i < nFrameTo; i += m_nSampleFrameInterval)
//	{
//		nFrames ++;
//	}
//	RTSHead.dwNumFrames = nFrames;
//
//	for(vector<KBipBone *>::iterator it = vecBipBone.begin(); it != vecBipBone.end(); ++it)
//	{
//		KBipBone *pBone = *it;
//		ppBoneRTS[i] = new RTSINFO[nFrames];
//		int jj = 0;
//		for(int j = nFrameFrom; j < nFrameTo; j += m_nSampleFrameInterval)
//		{
//			FbxTime tm;
//			tm.SetMilliSeconds(m_nFrameTimeFrameMilSecond * j);
//
//			if(pBone->pParentBone == NULL)
//			{							
//				//作为根骨骼来保存				
//				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
//				Matrix3 maxMat;
//				ToMaxMat3(maxMat, m);
//				maxMat = maxMat * g_Getmat3Inch2CMScale();
//				g_ChangeMatrixYZ(&maxMat);
//				AffineParts ap;
//				decomp_affine(maxMat, &ap);
//				RTSBoneAnimation rts;
//				ToRTS(rts, ap);
//				//fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);
//				memcpy(&ppBoneRTS[i][jj], &rts, sizeof(RTSINFO));
//			}
//			else
//			{				
//				//子骨骼的保存
//				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
//				Matrix3 mMat;
//				ToMaxMat3(mMat, m);
//
//				FbxAMatrix mp = pBone->pParentBone->pFbxNode->EvaluateGlobalTransform(tm);
//				Matrix3 mParentInverse;
//				ToMaxMat3(mParentInverse, mp);
//				mParentInverse.Invert();
//
//				Matrix3 mlocal = mMat * mParentInverse;
//
//				g_ChangeMatrixYZ(&mlocal);
//				AffineParts ap;
//				decomp_affine(mlocal, &ap);
//				RTSBoneAnimation rts;
//				ToRTS(rts, ap);
//				//fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);
//				memcpy(&ppBoneRTS[i][jj], &rts, sizeof(RTSINFO));
//			}
//			++jj;
//		}
//		++i;
//	}
//	
//	//for (DWORD32 i = 0; i < RTSHead.dwNumBones; ++i)
//	//{
//	//	ppBoneRTS[i] = new RTSINFO[RTSHead.dwNumFrames];
//	//	KGLOG_PROCESS_ERROR(ppBoneRTS[i]);
//
//	//	for (DWORD32 j = 0; j < RTSHead.dwNumFrames; ++j)
//	//	{
//	//		memcpy(&ppBoneRTS[i][j], &m_lpRTSAniBones[i * RTSHead.dwNumFrames + j], sizeof(RTSINFO));
//	//	}
//	//}
//	boneRTSAniData.ppBoneRTS = ppBoneRTS;
//
//	boneRTSAniData.dwEndFlag = ANI_FILE_END_FLAG;
//
//	hRetCode = AniFileWrap.ConvertAniData((BYTE *)(&boneRTSAniData));
//	KGLOG_COM_PROCESS_ERROR(hRetCode);
//
//	hRetCode = AniFileWrap.SaveToFile(fileName);
//	KGLOG_COM_PROCESS_ERROR(hRetCode);
//
//	if(ExportOption()->bOnlyAniExport)
//	{
//		SetSaveFilePath(fileName, MAX_PATH);
//	}
//
//	hResult = S_OK;
//Exit0:
//	if (ppBoneRTS)
//	{
//		for (DWORD32 i = 0; i < RTSHead.dwNumBones; ++i)
//		{
//			SAFE_DELETE_ARRAY(ppBoneRTS[i]);
//			SAFE_DELETE_ARRAY(ppName[i]);
//		}
//		SAFE_DELETE_ARRAY(ppBoneRTS);
//		SAFE_DELETE_ARRAY(ppName);
//	}	
//	boneRTSAniData.Clear();	
//}
//
//
//void printrts(RTSBoneAnimation &rts)
//{
//	Point3 Translation;
//	Point3 Scale;
//	Quat Rotation;
//	float Sign;
//	Quat SRotation;
//
//	char msg[1024];
//	sprintf(msg, "%f %f %f \r\n %f %f %f \r\n %f %f %f %f \r\n %f \r\n %f %f %f %f \r\n",
//											rts.Translation.x, rts.Translation.y, rts.Translation.z,
//		                                    rts.Scale.x, rts.Scale.y, rts.Scale.z,
//											rts.Rotation.x, rts.Rotation.y, rts.Rotation.z, rts.Rotation.w,
//											rts.Sign,
//											rts.SRotation.x, rts.SRotation.y, rts.SRotation.z, rts.SRotation.w
//
//		);
//	OutputDebugString(msg);
//};
//
//void KParser::SaveBipAnimationToFile_NoCompress(const char *szName, const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo)
//{
//	char fileName[MAX_PATH];
//	int len = (int) strlen(pOutDir);
//
//	if(pOutDir[len - 1] == '\\'  || pOutDir[len - 1] == '/')
//	{
//		if(nFrameFrom ==0 && nFrameTo == m_nAnimationFrames)
//		{
//			sprintf(fileName, "%s%s.ani", pOutDir, szName);
//		}
//		else
//		{
//			sprintf(fileName, "%s%s_%d_%d.ani", pOutDir, szName, nFrameFrom, nFrameTo);
//		}
//	}
//	else
//	{
//		if(nFrameFrom ==0 && nFrameTo == m_nAnimationFrames)
//		{
//			sprintf(fileName, "%s\\%s.ani", pOutDir, szName);
//		}
//		else
//		{
//			sprintf(fileName, "%s\\%s_%d_%d.ani", pOutDir, szName, nFrameFrom, nFrameTo);
//		}
//	}
//	
//	int nBytes = 0;
//	FILE *fp = NULL;	
//
//	DWORD32 Mask = 0x414E494D;//ANIM对应的ASECC代码	
//	DWORD32 BlockLength = 0;
//	DWORD32 NumAnimation = 1;
//	DWORD32 BlockEnd  = 0xFFFFFFFF;
//	DWORD32 dwAnimationType = ANIMATION_BONE_RTS;
//	float fFrameLen = nFrameTime;
//	if(nSampleInterval == 0)
//		m_nSampleFrameInterval = 1;
//	else
//		m_nSampleFrameInterval = nSampleInterval;
//
//	vector<KBipBone *> vecBipBone;
//	//检测是否和预设骨架文件定义一致，如果一致则采用预设骨架的骨骼顺序进行动画导出，这样可以和老的骨骼动画格式兼容，动画文件也不需要进行转换了
//	KSkeletonFileConfig *pSkeletonFileConfig = DoSkeletonMartch();
//	if(pSkeletonFileConfig)
//	{
//		for(vector<KSkeletonTextBone>::iterator it = pSkeletonFileConfig->SkeletonList.begin(); it != pSkeletonFileConfig->SkeletonList.end(); ++it)
//		{
//			KSkeletonTextBone &bone = *it;
//			KBipBone *pBone = GetBone(bone.szBoneName.c_str());
//			vecBipBone.push_back(pBone);
//		}
//	}
//	else
//	{
//		vecBipBone = m_vecBipBone;
//	}
//
//
//	DWORD32 dwBoneNum = (DWORD32) vecBipBone.size();
//	const char *szAniName = szName;
//	if(!m_nAnimationFrames)
//		goto Exit0;
//
//	fp = fopen(fileName, "wb");
//	if(!fp)
//		goto Exit0;
//
//	int nFrames = 0;
//	for(int i = nFrameFrom; i < nFrameTo; i += m_nSampleFrameInterval)
//	{
//		nFrames ++;
//	}
//	
//	fwrite(&Mask, sizeof(DWORD32), 1, fp);
//	nBytes += sizeof(DWORD32);
//
//	BlockLength += sizeof(DWORD32) * 3;//动画数目，结束标志，动画类型	
//	BlockLength += sizeof(float) + sizeof(DWORD32)*2;//时间间隔，骨骼数目，动作帧数
//	BlockLength += sizeof(char) * 30 * dwBoneNum;//骨骼名称  m_dNumAnimationBones:7
//	BlockLength += sizeof(RTSBoneAnimation) * dwBoneNum * m_nAnimationFrames;//骨骼变换矩阵
//
//	fwrite(&BlockLength, sizeof(DWORD32), 1, fp);
//	nBytes += sizeof(DWORD32);
//	fwrite(&NumAnimation,sizeof(DWORD32), 1, fp);                      //动画数目，这里默认为１
//	nBytes += sizeof(DWORD32);
//	fwrite(&dwAnimationType, sizeof(DWORD32), 1, fp);
//	nBytes += sizeof(DWORD32);
//	fwrite( szAniName, sizeof(char), 30, fp);
//	nBytes += sizeof(char) * 30;
//
//	fwrite(&dwBoneNum, sizeof(DWORD32), 1, fp);
//	nBytes += sizeof(DWORD32);
//	fwrite(&nFrames, sizeof(DWORD32), 1, fp);
//	nBytes += sizeof(DWORD32);
//	fwrite(&fFrameLen, sizeof(float), 1, fp);
//	nBytes += sizeof(float);
//
//	for(DWORD32 i_Bone = 0; i_Bone < dwBoneNum; i_Bone++)
//	{
//		const char * BoneName = vecBipBone[i_Bone]->szBoneName;
//		fwrite(BoneName,sizeof(char), 30, fp); 
//		nBytes += sizeof(char) * 30;
//	}		
//
//	for(vector<KBipBone *>::iterator it = vecBipBone.begin(); it != vecBipBone.end(); ++it)
//	{
//		KBipBone *pBone = *it;
//		for(int i = nFrameFrom; i < nFrameTo; i += m_nSampleFrameInterval)
//		{
//
//			FbxTime tm;
//			tm.SetMilliSeconds(m_nFrameTimeFrameMilSecond * i);
//
//			if(pBone->pParentBone == NULL)
//			{	
//				//作为根骨骼来保存				
//				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
//				Matrix3 maxMat;
//				ToMaxMat3(maxMat, m);
//				
//				//char msg[128];
//				//
//				//maxMat = maxMat * g_Getmat3Inch2CMScale();
//				//float f = maxMat.GetRow(1).y;
//				//sprintf(msg,"%f", f);
//				//OutputDebugString(msg);
//
//				g_ChangeMatrixYZ(&maxMat);
//				AffineParts ap;
//				decomp_affine(maxMat, &ap);
//				RTSBoneAnimation rts;
//				ToRTS(rts, ap);
//				//printrts(rts);
//				fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);				
//			}
//			else
//			{
//				//子骨骼的保存
//				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
//				Matrix3 mMat;
//				ToMaxMat3(mMat, m);
//
//				const char *pName = pBone->pFbxNode->GetName();
//				const char *pParentName = pBone->pParentBone->pFbxNode->GetName();
//
//				FbxAMatrix mp = pBone->pParentBone->pFbxNode->EvaluateGlobalTransform(tm);
//				Matrix3 mParentInverse;
//				ToMaxMat3(mParentInverse, mp);
//				mParentInverse.Invert();
//
//				Matrix3 mlocal = mMat * mParentInverse;
//
//				g_ChangeMatrixYZ(&mlocal);
//				AffineParts ap;
//				decomp_affine(mlocal, &ap);
//				RTSBoneAnimation rts;
//				ToRTS(rts, ap);
//				fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);
//			}			
//		}
//	}
//
//	fwrite(&BlockEnd, sizeof(DWORD32), 1, fp);
//
//Exit0:
//
//	if(fp)
//	{
//		fclose(fp);
//		fp = NULL;
//	}
//
//	if(ExportOption()->bOnlyAniExport)
//	{
//		SetSaveFilePath(fileName, MAX_PATH);
//	}
//
//	return;
//}
//
//void KParser::SaveBipAnimationToFile(const char *szName, const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress)
//{
//	if(bCompress)
//	{
//		SaveBipAnimationToFile_Compress(szName, pOutDir, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo);
//	}
//	else
//	{
//		SaveBipAnimationToFile_NoCompress(szName, pOutDir, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo);
//	}
//}
//
//string KParser::GetMeshPureName(OUT int* pLod, const char* pszName, bool bCollision/* = false*/)
//{
//	std::string strRet("");
//	std::regex reg(string("(.*)_lod(\\d*)"));
//	std::smatch matchResult;
//	string strSource;
//
//	if (!pszName)
//		return strRet;
//
//	strSource = string(pszName);
//	if (bCollision)
//		reg = regex("(.*)_C");
//
//	if (std::regex_match(strSource, matchResult, reg))
//	{
//		// name + lod number
//		strRet = string(matchResult[1]);
//		if (matchResult.size() >= 3 && pLod)
//		{
//			string strLod;
//			strLod = matchResult[2].str();
//			*pLod = std::stoi(strLod);
//		}
//	}
//	return strRet;
//}
//
//void KParser::ConstructModel(string strPureName, int nLOD)
//{
//	KModel* pModel = nullptr;
//
//	if (strPureName.empty() || nLOD < 0)
//		return;
//
//	MAP_MODEL_SET::iterator it = m_mapModelSet.find(strPureName);
//	if (it != m_mapModelSet.end())
//	{
//		pModel = it->second;
//	}
//	else if (nLOD == 0)
//	{
//		pModel = new KModel();
//		if (pModel)
//			m_mapModelSet.insert(make_pair(strPureName, pModel));
//	}
//
//}
//
//void KParser::AddModelInfoLODPath(string strPureName, string strMeshName, bool bCollision/* = false*/)
//{
//	if (!ExportOption()->bModelFileExport)
//		return;
//
//	if (strPureName.empty() || strMeshName.empty())
//		return;
//
//	MAP_MODEL_SET::iterator it = m_mapModelSet.find(strPureName);
//	if (it == m_mapModelSet.end())
//		return;
//
//	KModel* pModel = it->second;
//	if (!pModel)
//		return;
//
//	if (bCollision)
//		pModel->SetCollisionPath(strMeshName);
//	else
//		pModel->AddLodPath(strMeshName);
//}
//
//void KParser::AddModleInfoJsonInspackPath(std::string strName, string strInspackPath)
//{
//	if (!ExportOption()->bModelFileExport)
//		return;
//
//	if (strName.empty() || strInspackPath.empty())
//		return;
//
//	MAP_MODEL_SET::iterator it = m_mapModelSet.find(strName);
//	if (it == m_mapModelSet.end())
//		return;
//
//	KModel* pModel = it->second;
//	if (!pModel)
//		return;
//
//	pModel->AddInspackPath(strInspackPath);
//}
//
//bool KParser::GetModelLodPath(string strName, vector<string>& vecPath)
//{
//	MAP_MODEL_SET::iterator it = m_mapModelSet.find(strName);
//	if (it == m_mapModelSet.end())
//		return false;
//
//	return it->second->GetLodPath(vecPath);
//}
//
//bool KParser::GetModelInspackPath(string strName, vector<string>& vecPath)
//{
//	MAP_MODEL_SET::iterator it = m_mapModelSet.find(strName);
//	if (it == m_mapModelSet.end())
//		return false;
//
//	return it->second->GetInspackPath(vecPath);
//}
//
//
//string KParser::GetModelCollisionPath(string strName)
//{
//	MAP_MODEL_SET::iterator it = m_mapModelSet.find(strName);
//	if (it == m_mapModelSet.end())
//		return "";
//
//	return it->second->GetCollisionPath();
//}
//
//void KMesh::BuildMeshBoneWeight(KParser *pParser)
//{
//	int nNum = (int)m_vecBones.size();
//	for(int i = 0; i < nNum; ++i)
//	{
//		KMeshBone *pMeshBone = m_vecBones[i];
//		if(!pMeshBone->ComputBoneWeight(this, i))
//		{
//			pParser->AddErrorWeightBoneName(pMeshBone->szBoneName);
//		}
//	}	
//}
//
//KMeshBone *KMesh::GetBone(const char *pBoneName)
//{
//	KMeshBone *pMeshBone = NULL;
//	for(vector<KMeshBone*>::iterator it = m_vecBones.begin(); it != m_vecBones.end(); ++it)
//	{
//		KMeshBone *pBone = *it;
//		if(strcmp(pBone->szBoneName, pBoneName) == 0)
//		{
//			pMeshBone = pBone;
//			break;
//		}
//	}
//	return pMeshBone;
//}
//
//KSocket *KMesh::GetSocket(const char *pSocketName)
//{
//	for(vector<KSocket*>::iterator it = m_vecSocket.begin(); it != m_vecSocket.end(); ++it)
//	{
//		KSocket *pSocket = *it;
//		if(_stricmp(pSocket->szName.c_str(), pSocketName) == 0)
//		{
//			return pSocket;			
//		}
//	}
//	return NULL;
//}
//
//void KMesh::ParseBlendShapes(FbxMesh* pMesh)
//{
//    m_nBlendShapeElementCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);                
//    for(int lBlendShapeIndex = 0; lBlendShapeIndex < m_nBlendShapeElementCount; ++lBlendShapeIndex)
//    {
//        FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);
//        assert(lBlendShape);
//        KBlendShape* pKBlendShape = new KBlendShape;
//        pKBlendShape->szName = lBlendShape->GetName();
//        m_blendShapes.push_back(pKBlendShape);
//        
//
//        int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
//        for(int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; ++lChannelIndex)
//        {
//            FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
//            if(!lChannel)
//                continue;
//            KBlendShapeChannel* pChannel = new KBlendShapeChannel;
//            pChannel->szName = lChannel->GetName();
//            pKBlendShape->m_vBlendShapeChannels.push_back(pChannel);
//
//            int nTargetShapeCount = lChannel->GetTargetShapeCount();
//            double* lFullWeights = lChannel->GetTargetShapeFullWeights();
//            for (int nTargetShapeIndex = 0; nTargetShapeIndex < nTargetShapeCount; ++nTargetShapeIndex)
//            {
//                const FbxShape* pShape = lChannel->GetTargetShape(nTargetShapeIndex);
//                assert(pShape);
//                KBlendShapeTarget* pKBlendShapeTarget = new KBlendShapeTarget;
//                pChannel->m_vBlendShapeTargets.push_back(pKBlendShapeTarget);
//
//                pKBlendShapeTarget->fHightWeight = lFullWeights[nTargetShapeIndex];
//                pKBlendShapeTarget->szName = pShape->GetName();
//
//                int nMeshControlPointsCount = pMesh->GetControlPointsCount();
//
//                int nControlPointsCount = pShape->GetControlPointsCount();
//                FbxVector4* vControlPoints = pShape->GetControlPoints();
//
//                FbxLayerElementArrayTemplate<FbxVector4>* lNormals = NULL;
//                pShape->GetNormals(&lNormals);
//                int nNormalsCount = lNormals->GetCount();
//
//                assert(nMeshControlPointsCount == nControlPointsCount);
//
//                int lPolygonCount = pMesh->GetPolygonCount();
//                pKBlendShapeTarget->m_vControlPoints.reserve(lPolygonCount * 3);
//                int vertexId = 0;
//                for (int i = 0; i < lPolygonCount; ++i)
//                {
//                    int lPolygonSize = pMesh->GetPolygonSize(i);
//                    for (int j = 0; j < lPolygonSize; j++)
//                    {
//                        KPCNT3 vert;
//                        int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
//                        FbxVector4 v = vControlPoints[lControlPointIndex];
//                        // y z要交换回头一起fix
//                        vert.pos = KVec3((float)v.mData[0], (float)v.mData[1], (float)v.mData[2]);
//
//                        for(int l = 0; l < pShape->GetElementNormalCount(); ++l)
//                        {
//                            const FbxGeometryElementNormal* leNormal = pShape->GetElementNormal(l);
//
//                            if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//                            {
//                                switch (leNormal->GetReferenceMode())
//                                {
//                                case FbxGeometryElement::eDirect:
//                                    {
//                                        FbxVector4 vNormal = leNormal->GetDirectArray().GetAt(vertexId);
//                                        //Display3DVector(header, vNormal);
//                                        //y z要交换
//                                        vert.nomral = KVec3((float)vNormal.mData[0], (float)vNormal.mData[1], (float)vNormal.mData[2]);
//
//                                    }						
//                                    break;
//                                case FbxGeometryElement::eIndexToDirect:
//                                    {
//                                        int id = leNormal->GetIndexArray().GetAt(vertexId);
//                                        FbxVector4 vNormal = leNormal->GetDirectArray().GetAt(id);
//                                        //Display3DVector(header, vNormal);
//                                        vert.nomral = KVec3((float)vNormal.mData[0], (float)vNormal.mData[1], (float)vNormal.mData[2]);
//                                    }
//                                    break;
//                                default:
//                                    break; // other reference modes not shown here!
//                                } // switch
//                            } // if leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex
//                        }// for pShape->GetElementNormalCount()
//                        vertexId++;
//                        pKBlendShapeTarget->m_vControlPoints.push_back(vert);
//                    }// for lPolygonSize
//                }// for lPolygonCount
//            }// for nTargetShapeCount
//        }// for lBlendShapeChannelCount
//    } // for m_nBlendShapeElementCount               
//}
//
//void KMesh::ParseSplines()
//{
//    if (!m_pFbxNode)
//        return;
//
//    for(vector<KSpline *>::iterator it = m_vecSplines.begin(); it != m_vecSplines.end(); ++it)
//    {
//        KSpline *pSpline = *it;
//        delete(pSpline);
//    }
//    m_vecSplines.clear();
//
//    int nChildCount = m_pFbxNode->GetChildCount();
//    for (int i = 0; i < nChildCount; ++i)
//    {
//        FbxNode* pChildNode = m_pFbxNode->GetChild(i);
//        if (pChildNode->GetNodeAttribute() == NULL)
//        {
//            _FBXSDK_printf("NULL Node Attribute\n\n");
//        }
//        else
//        {
//            if (pChildNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eLine)
//            {
//                FbxLine* pLine = (FbxLine*)pChildNode->GetNodeAttribute();
//                KSpline* pSpline = new KSpline;
//                int nPointCount = pLine->GetIndexArraySize();
//                int nEndPointCount = pLine->GetEndPointCount();
//                for (int j = 0; j < nPointCount; j++)
//                {
//                    int nPointIndex = pLine->GetPointIndexAt(j);
//                    FbxVector4 f4Point = pLine->GetControlPointAt(nPointIndex);
//                    Point3 f3Point;
//
//                    if (!ExportOption()->SwitchToMaxMat())
//                    {
//                        // maya坐标系和DX坐标系是不同左右手坐标系
//                        f3Point.x = -f4Point.mData[0];
//                        f3Point.y = f4Point.mData[1];
//                        f3Point.z = f4Point.mData[2];
//                    }
//                    else
//                    {
//                        f3Point.x = f4Point.mData[0];
//                        f3Point.y = f4Point.mData[2];
//                        f3Point.z = f4Point.mData[1];
//                    }
//
//                    if(ExportOption()->fFbxScale)
//                    {
//                        f3Point = f3Point * g_Getmat3Inch2CMScale();
//                    }
//
//                    pSpline->vecPoints.push_back(KVec3(f3Point.x, f3Point.y, f3Point.z));
//                }
//                for (int j = 0; j < nEndPointCount; j++)
//                {
//                    pSpline->vecEndPointsIndex.push_back(pLine->GetEndPointAt(j));
//                }
//                GetGlobalRTS(pChildNode, &pSpline->f3Translation, &pSpline->f3Scaling, &pSpline->f4Rotation);
//                pSpline->strName = pChildNode->GetName();
//                m_vecSplines.push_back(pSpline);
//            }
//        }
//    }
//}
//
//void KMesh::ParseAnchorPoints()
//{
//    if (!m_pFbxNode)
//        return;
//
//    for(vector<KAnchorPoint *>::iterator it = m_vecAnchorPoints.begin(); it != m_vecAnchorPoints.end(); ++it)
//    {
//        KAnchorPoint *pAnchorPoint = *it;
//        delete(pAnchorPoint);
//    }
//    m_vecAnchorPoints.clear();
//
//    int nChildCount = m_pFbxNode->GetChildCount();
//    for (int i = 0; i < nChildCount; ++i)
//    {
//        FbxNode* pChildNode = m_pFbxNode->GetChild(i);
//        if (pChildNode->GetNodeAttribute() == NULL)
//        {
//            _FBXSDK_printf("NULL Node Attribute\n\n");
//        }
//        else
//        {
//            if (pChildNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eNull)
//            {
//                KAnchorPoint* pAnchorPoint = new KAnchorPoint;
//                GetGlobalRTS(pChildNode, &pAnchorPoint->f3Point, 0, &pAnchorPoint->f4Rotation);
//                pAnchorPoint->strName = pChildNode->GetName();
//                m_vecAnchorPoints.push_back(pAnchorPoint);
//            }
//        }
//    }
//}
//
//void KMesh::ComputeBlendShapeTangent(FbxMesh* pMesh)
//{
//    for (auto pBlendShape : m_blendShapes)
//    {
//        for (auto pChannel : pBlendShape->m_vBlendShapeChannels)
//        {
//            for (auto pTarget : pChannel->m_vBlendShapeTargets)
//            {
//                ComputTangent(pTarget->m_vControlPoints);
//            }
//        }
//    }
//}
//
//void KMesh::GetVertMat(int n, FbxAMatrix &mat)
//{
//	if(m_pFbxNode)
//	{
//		mat = m_pFbxNode->EvaluateGlobalTransform(0);
//	}
//	else
//	{
//		int id = m_vertsMergeMatTable[n];
//		mat = m_vertsMergeMat[id];
//	}
//}
//
//void KMesh::FixSocketBone(FbxNode *pNode, FbxNode *pRootNode)
//{
//	const char *pSocketName = pNode->GetName();
//	m_socketNames.insert(pSocketName);
//	FbxNode *pParent = pNode->GetParent();
//	while(pParent)
//	{
//		const char *pParentName = pParent->GetName();
//		KMeshBone *pMeshBone = GetBone(pParentName);
//		if(!pMeshBone)
//		{
//			pMeshBone = new KMeshBone;
//			strncpy(pMeshBone->szBoneName, pParentName, 30);				
//			m_vecBones.push_back(pMeshBone);				
//			pMeshBone->pFbxNode = pParent;
//			_BuildBoneLinkName(pMeshBone);
//			_BuildBoneMatrixOffset(pMeshBone);
//			if(pParent->GetParent() == pRootNode)
//			{
//				pMeshBone->bIsRoot = TRUE;
//			}
//		}
//		else
//		{
//			break;
//		}
//		pParent = pParent->GetParent();
//	}
//}
//
//void KMesh::FixLinks(KParser *pParser)
//{
//	for(map<string, string>::iterator it = pParser->m_mapRelink.begin(); it != pParser->m_mapRelink.end(); ++it)
//	{
//		string childName = it->first;
//		string newParentName = it->second;
//
//		KMeshBone *pChildBone = NULL;
//		KMeshBone *pNewParentBone = NULL;
//		KMeshBone *pOldParentBone = NULL;
//
//		
//
//		pChildBone = GetBone(childName.c_str());
//
//		if(!pChildBone)
//			continue;
//
//		pNewParentBone = GetBone(newParentName.c_str());
//		pOldParentBone = GetBone(pChildBone->m_szParentName.c_str());
//
//	
//		pChildBone->m_szParentName = newParentName;
//		if(pOldParentBone)
//		{
//			BOOL bFind = FALSE;
//			for(vector<string>::iterator it3 = pOldParentBone->m_vecChildNames.begin(); it3 != pOldParentBone->m_vecChildNames.end(); ++it3)
//			{
//				string &_childName = *it3;
//				if(_childName == childName)
//				{
//					bFind = TRUE;
//					pOldParentBone->m_vecChildNames.erase(it3);
//					break;
//				}
//			}
//
//			if(pOldParentBone->m_vecChildNames.empty())
//			{
//				//如果父骨骼没有子骨骼了，并且也不是权重骨骼，那么删除这根骨骼
//				if(m_weightBoneNames.find(pOldParentBone->szBoneName) == m_weightBoneNames.end())
//				{
//					KMeshBone *pParentParent = GetBone(pOldParentBone->m_szParentName.c_str());
//					for(vector<string>::iterator it = pParentParent->m_vecChildNames.begin(); it != pParentParent->m_vecChildNames.end(); ++it)
//					{
//						string &szName = *it;
//						if(szName ==  pOldParentBone->szBoneName)
//						{
//							pParentParent->m_vecChildNames.erase(it);
//							break;
//						}
//					}
//
//					for(vector<KMeshBone*>::iterator it = m_vecBones.begin(); it != m_vecBones.end(); ++it)
//					{
//						KMeshBone *pMeshBone = *it;
//						if(strcmp(pMeshBone->szBoneName, pOldParentBone->szBoneName) == 0)
//						{
//							m_vecBones.erase(it);
//							break;
//						}
//					}
//				}
//			}
//
//			assert(bFind);
//		}
//
//		if(pNewParentBone)
//		{
//			if(find(pNewParentBone->m_vecChildNames.begin(), pNewParentBone->m_vecChildNames.end(), childName) ==  pNewParentBone->m_vecChildNames.end())
//			{
//				pNewParentBone->m_vecChildNames.push_back(childName);
//			}
//			else
//			{
//				printf("error, new parent has already added this bone!");
//				pParser->errorMsg.append("error, new parent has already added this bone!\r\n");
//				assert(0);
//			}
//		}
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
//// KModel
//KModel::KModel() 
//	: m_pRootMesh(nullptr)
//	, m_nLod(0)
//{
//
//}
//
//bool KModel::AddLodPath(string strPath)
//{
//	if (strPath.empty())
//		return false;
//	m_vecLodPath.push_back(strPath);
//
//	return true;
//}
//
//bool KModel::AddInspackPath(string strPath)
//{
//	if (strPath.empty())
//		return false;
//	m_vecInspackPath.push_back(strPath);
//
//	return true;
//}
//
//bool KModel::SetCollisionPath(string strPath)
//{
//	if (strPath.empty())
//		return false;
//	m_strCollisionPath = strPath;
//
//	return true;
//}
//
//bool KModel::GetLodPath(vector<string>& vecPath)
//{
//	vecPath.assign(m_vecLodPath.begin(), m_vecLodPath.end());
//	return true;
//}
//
//bool KModel::GetInspackPath(vector<string>& vecPath)
//{
//	vecPath.assign(m_vecInspackPath.begin(), m_vecInspackPath.end());
//	return true;
//}
//
//void KModel::ClearCache()
//{
//	m_vecLodPath.clear();
//	m_vecInspackPath.clear();
//	m_strCollisionPath = "";
//}