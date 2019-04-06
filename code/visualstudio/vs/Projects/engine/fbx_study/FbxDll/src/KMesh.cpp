/********************************************************************
	created:	2015/09/07   11:33
	filename: 	KMesh.cpp		
	author:		LiKan
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "KMesh.h"
#include <stdio.h>
#include <algorithm>
#include <io.h>
#include <assert.h>
#include <fbxsdk.h>
//#include <decomp.h>
#include "display/DisplayCommon.h"
#include "KParser.h"
#include "KMaterail.h"
#include "KG3DAniFileWrap.h"
#include "KTangent.h"
//#include "pub.h"
//#include "document.h"
//#include "prettywriter.h"


#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


#define MAX_BONE_NAME_LENGTH 30

void QuaternionToEulerAngles(const KVec4& q, KVec3& pyr) 
{
	float t = 2 * (q.w * q.x - q.y * q.z);
	if (t < -1.0f) t = -1.0f;
	if (t > 1.0f) t = 1.0f;

	pyr.x = asinf(t);
	pyr.y = atan2f(2 * (q.w * q.y + q.z * q.x) , 1 - 2 * (q.x * q.x + q.y * q.y));
	pyr.z = atan2f(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (q.z * q.z + q.x * q.x));
}

void FixPoint(int n, KVec3 *point, KMesh *pKMesh, bool bCalAABB/* = true*/)
{	
	Point3 p(point->x, point->y, point->z);
	//FbxAMatrix mat = pKMesh->m_pFbxNode->EvaluateGlobalTransform(0);
	FbxAMatrix mat;
	pKMesh->GetVertMat(n, mat);
	Matrix3 maxMat;
	
	ToMaxMat3(maxMat, mat);	
	if(!pKMesh->m_vecBones.empty())
	{
		//如果包含骨骼，那么用第0帧的mesh顶点位置(各骨骼权重计算结果*缩放系数)导出
		//因为原引擎并不是以蒙皮姿势的状态导出，而是以tim0的骨骼和顶点状态导出的
		//理论上蒙皮姿势导出就不需要这个转换了，为了和原来的导出插件保证效果一致所以必加上这个转换过程，这个过程其实也能计算任意帧下的顶点位置
		KWeight &weight = pKMesh->m_weights[n];
		Point3 resultPoint(0, 0, 0);		
		
		for(int i = 0; i < 4; ++i)
		{
			int id = weight.boneids[i];
			float fweight = weight.weights[i];		
			if(id!= 0 || fweight!= 0.0f)
			{
				KMeshBone *pMeshBone = pKMesh->m_vecBones[id];
				Point3 _p = p * pMeshBone->initPoseMatrixInverse * pMeshBone->time0Matrix * g_Getmat3Inch2CMScale() * fweight;
				resultPoint += _p;				
			}
		}

		if (!ExportOption()->Switch())
		{
			point->x = resultPoint.x;//
			point->y = resultPoint.y;
			point->z = -resultPoint.z;//
		}
		else
		{
			point->x = resultPoint.x;
			point->y = resultPoint.z;
			point->z = resultPoint.y;
		}
	}
	else
	{	
		//不包含骨骼，用该物件相对原点的位置乘以缩放系数导出
		if(ExportOption()->bGroupExport)
		{
			p = p * g_Getmat3Inch2CMScale();
		}
		else
		{
			maxMat = maxMat * g_Getmat3Inch2CMScale();
			p = p * maxMat;
		}
		
		if (!ExportOption()->Switch())
		{
			point->x = p.x;//
			point->y = p.y;
			point->z = -p.z;//
		}
		else
		{
			point->x = p.x;
			point->y = p.z;
			point->z = p.y;
		}
	}
    if (bCalAABB)
    {
        pKMesh->m_AabbBox.AddPositon(point->x, point->y, point->z);
    }	
}

void ParseControlsPoints(KMesh* pMesh)
{
	FbxMesh *pFbxMesh = pMesh->m_pFbxMesh;
	int i, lControlPointsCount = pFbxMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pFbxMesh->GetControlPoints();


	for (i = 0; i < lControlPointsCount; i++)
	{
		// y, z要交换，回头再说
		KVec3 controlpoints((float)lControlPoints[i].mData[0],  (float)lControlPoints[i].mData[1], (float)lControlPoints[i].mData[2]);
		//vector<KVec3>::iterator it = std::find(pMesh->m_vecControlPoints.begin(), pMesh->m_vecControlPoints.end(), controlpoints);
		//if(it == pMesh->m_vecControlPoints.end())
		{
			pMesh->m_vecControlPoints.push_back(controlpoints);
		}		
	}	
}

inline DWORD32 Dx_Vec42DWORD(float r, float g, float b, float a)
{
	return  DWORD32(a * 255.0f)  << 24 | 
		DWORD32(r * 255.0f)  << 16 |
		DWORD32(g * 255.0f)  << 8  | 
		DWORD32(b* 255.0f)   << 0;	
}


void ParseFaces(KMesh* pKMesh, KParser *pParser)
{
	FbxMesh *pMesh = pKMesh->m_pFbxMesh;
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints(); 
	//char header[100];

	//DisplayString("    Polygons");
	int uvCount = 0;

	int vertexId = 0;


	vector<KPCNT3> vecVerts;
	vecVerts.reserve(lPolygonCount * 3);

	int nFaces = lPolygonCount;
	pKMesh->m_numFaces = nFaces;
	for (i = 0; i < lPolygonCount; i++)
	{
		//DisplayInt("        Polygon ", i);
		int l;

		int lPolygonSize = pMesh->GetPolygonSize(i);


		for (j = 0; j < lPolygonSize; j++)
		{
			KPCNT3 vert;
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
			
			//Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);
			FbxVector4 v = lControlPoints[lControlPointIndex];
			// y z要交换回头一起fix
			vert.pos = KVec3((float)v.mData[0], (float)v.mData[1], (float)v.mData[2]);


			//颜色信息没用上，暂时不用管它
			//for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			//}

			pKMesh->m_nUvElementCount = pMesh->GetElementUVCount();
			for (l = 0; l < pKMesh->m_nUvElementCount; ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV( l);
			
				//FBXSDK_sprintf(header, 100, "            Texture UV: "); 

				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector2 uv = leUV->GetDirectArray().GetAt(lControlPointIndex);
							//Display2DVector(header, uv);
							if(l == 0)
							{
								vert.uvw0 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
							}
							else if(l == 1)
							{
								vert.uvw1 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
							}
							else if(l == 2)
							{
								vert.uvw2 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
							}
						}

						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
							FbxVector2 uv = leUV->GetDirectArray().GetAt(id);
							//Display2DVector(header, uv);

							if(l == 0)
							{
								vert.uvw0 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
							}
							else if(l == 1)
							{
								vert.uvw1 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
							}
							else if(l == 2)
							{
								vert.uvw2 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
							}
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{						
						FbxLayerElementArrayTemplate<int> &t = leUV->GetIndexArray();
						int id = t.GetAt(i * 3 + j);
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{							
								FbxVector2 uv = leUV->GetDirectArray().GetAt(id);
							
								//Display2DVector(header, uv);
								if(l == 0)
								{
									vert.uvw0 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
								}
								else if(l == 1)
								{

									vert.uvw1 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
									//vert.uvw1 =  vert.uvw0;
								}
								else if(l == 2)
								{
									vert.uvw2 = KVec3((float)uv.mData[0], (float)(1.0 - uv.mData[1]), 0);
								}
							}
							break;						
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}

			for( l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);
				//FBXSDK_sprintf(header, 100, "            Normal: "); 

				if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 vNormal = leNormal->GetDirectArray().GetAt(vertexId);
							//Display3DVector(header, vNormal);
							//y z要交换
							vert.nomral = KVec3((float)vNormal.mData[0], (float)vNormal.mData[1], (float)vNormal.mData[2]);

						}						
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							FbxVector4 vNormal = leNormal->GetDirectArray().GetAt(id);
							//Display3DVector(header, vNormal);
							vert.nomral = KVec3((float)vNormal.mData[0], (float)vNormal.mData[1], (float)vNormal.mData[2]);
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}

			//for( l = 0; l < pMesh->GetElementVertexColorCount(); ++l)
			//只支持导出一套颜色
			pKMesh->m_nColorElementCount = pMesh->GetElementVertexColorCount();
			if(pKMesh->m_nColorElementCount)
			{
				FbxGeometryElementVertexColor *leColor = pMesh->GetElementVertexColor(0);
				if(leColor->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leColor->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{							
							FbxColor vColor = leColor->GetDirectArray().GetAt(vertexId);
							vert.color = Dx_Vec42DWORD((float)vColor.mRed, (float)vColor.mGreen, (float)vColor.mBlue, (float)vColor.mAlpha);
							//vert.color
							//y z要交换
							//vert.nomral = KVec3((float)vNormal.mData[0], (float)vNormal.mData[1], (float)vNormal.mData[2]);
						}						
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leColor->GetIndexArray().GetAt(vertexId);
							FbxColor vColor = leColor->GetDirectArray().GetAt(id);
							vert.color = Dx_Vec42DWORD((float)vColor.mRed, (float)vColor.mGreen, (float)vColor.mBlue, (float)vColor.mAlpha);
							//Display3DVector(header, vNormal);
							//vert.nomral = KVec3((float)vNormal.mData[0], (float)vNormal.mData[1], (float)vNormal.mData[2]);
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}				
			}           
			
			//用不上，不管它
			//for( l = 0; l < pMesh->GetElementTangentCount(); ++l)
			//{
			//}
			//for( l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			//{
			//}
			vertexId++;
			vecVerts.push_back(vert);

		} // for polygonSize

	} // for polygonCount

    if (pParser->IsParseBlendShape())
    {
        pKMesh->ParseBlendShapes(pMesh);                
    }


	//KPCNT3 pp0;
	//pp0.nomral =KVec3(-0.0694501549, -0.0219189152, 0.997344613);
	//pp0.color = 0;
	//pp0.uvw0 = KVec3(3.00824928, -1.01933408, 0.000000000);

	//KPCNT3 pp1;
	//pp1.nomral =KVec3(-0.0694501549, -0.0219189152, 0.997344613);
	//pp1.color = 0;
	//pp1.uvw0 = KVec3(3.00824928, -1.01933408, 0.000000000);

	//BOOL b = pp0 < pp1;


	if(ExportOption()->bTangent)
	{
		ComputTangent(vecVerts);
        if (pParser->IsParseBlendShape())
        {
            pKMesh->ComputeBlendShapeTangent(pMesh);
        }
	}

	//过滤掉重复的顶点，创建索引表
	pKMesh->m_verts.resize(vecVerts.size());
	copy(vecVerts.begin(), vecVerts.end(), pKMesh->m_verts.begin());
    //如果parseBlendShape，不应该过滤重复顶点，因为这样有可能改变mesh的拓扑结构 || 这里会引发镜像UV处的法线反向
    if (!pParser->IsParseBlendShape() /*&& !ExportOption()->bTangent*/)
    {
        sort(pKMesh->m_verts.begin(), pKMesh->m_verts.end());		
        vector<KPCNT3>::iterator it = unique(pKMesh->m_verts.begin(), pKMesh->m_verts.end());
        pKMesh->m_verts.erase(it, pKMesh->m_verts.end());	
    }

	map<KPCNT3, int> idxMap;
	i = 0;
	for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
	{
		KPCNT3 &p = *it;
		idxMap.insert(pair<KPCNT3, int>(p, i));				
		++i;
	}

	//已经过滤掉重复的了，这两个应该相等的
	//assert(idxMap.size() ==  pKMesh->m_verts.size());
		
	//重建面索引
	assert(nFaces ==  (int) vecVerts.size() / 3);
	pKMesh->m_inds.clear();
	pKMesh->m_inds.reserve(nFaces * 3);
	for(i = 0; i < nFaces; ++i)
	{
		KPCNT3& p0 = vecVerts[i * 3];
		KPCNT3& p1 = vecVerts[i * 3 + 1];
		KPCNT3& p2 = vecVerts[i * 3 + 2];
		
		int ind0 = idxMap[p0];
		int ind1 = idxMap[p1];
		int ind2 = idxMap[p2];

		pKMesh->m_inds.push_back(ind0);
		pKMesh->m_inds.push_back(ind1);
		pKMesh->m_inds.push_back(ind2);
		

		//三角形的三个顶点索引应该不一样
		//assert(ind0 != ind1);
		//assert(ind1 != ind2);
		//assert(ind2 != ind1);
	}
		

	//骨骼权重顶点map
	if(!pKMesh->m_vecBones.empty())
	{	
		//for check
		i = 0;
		for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
		{
			KPCNT3 &p = *it;
			map<KVec3, KWeight*>::iterator itt = pKMesh->m_mapWeightControlPointMapping.find(p.pos);
			if(itt != pKMesh->m_mapWeightControlPointMapping.end())
			{
				KWeight *pWeight = itt->second;
				pKMesh->m_weights.push_back(*pWeight);
			}
			else
			{
				//有没有绑定的顶点
				char msg[260];
				sprintf(msg, "error: the point [%f %f %f] has not bind to any bone! \r\n", p.pos.x, p.pos.y, p.pos.z);
				printf(msg);
				if(pParser)
				{
					pParser->errorMsg.append(msg);
				}
				assert(0);
			}

		    //展开后的顶点对应骨骼权重信息再统计一次
			for(vector<KMeshBone*>::iterator it3= pKMesh->m_vecBones.begin(); it3 != pKMesh->m_vecBones.end(); ++it3)
			{
				KMeshBone *pMeshBone = *it3;
				int nWeightCount = (int)pMeshBone->m_vecControllPointsWeight.size();
				for(int n = 0; n < nWeightCount; ++n)
				{					
					float fWeight = pMeshBone->m_vecControllPointsWeight[n];
					KVec3 &controlpoint = pMeshBone->m_vecControllPoints[n];
					if(p.pos == controlpoint)
					{
						//控制顶点可能有重复，这里没影响过的才添加，否则就多次影响权重了
						pMeshBone->m_vecIndexRef.push_back((DWORD32)i);
						pMeshBone->m_vecWeightRef.push_back(fWeight);						
					}
				}
			}
			++i;
		}
		assert(pKMesh->m_weights.size() == pKMesh->m_verts.size());		
	}
	

	//解析subset attribute mapping
	ParseSubsetMapping(pKMesh);
	ParseMaterails(pKMesh);
	ParseTextureMapping(pKMesh);	
}



#define defFILE_TAGLEN			8
#define defFILE_DESCLEN			64
#define defFILE_COMMONTAG		"KSword3D"	//文件头部标志
#define defFILE_COPYRIGHTDESC	"Copyright 1988-2004 Kingsoft Corporation. All rights Reserved"		//版权信息（默认填入文件中）
#pragma pack(push,1) 
struct TFileHeader
{
	char szCommonTag[defFILE_TAGLEN];
	char szModuleTag[defFILE_TAGLEN];
	__int32 byVersion;
	char szDescription[defFILE_DESCLEN];

	TFileHeader(char* pModuleTag = NULL, __int32 byVer = 0, char* pDescription = NULL)
	{
		Set(pModuleTag, byVer, pDescription);
	}
	void Set(char* pModuleTag = NULL, __int32 byVer = 0, char* pDescription = NULL)
	{
		memset(this, 0, sizeof(TFileHeader));
		strncpy(szCommonTag, defFILE_COMMONTAG, defFILE_TAGLEN);

		if(pModuleTag)
			strncpy(szModuleTag, pModuleTag, defFILE_TAGLEN);

		byVersion = byVer;

		if(pDescription)
			strncpy(szDescription, pDescription, defFILE_DESCLEN);
		else
			strncpy(szDescription, defFILE_COPYRIGHTDESC, defFILE_DESCLEN);
	}

};


struct _MeshHead
{
	DWORD32  NumVertices;
	DWORD32  NumFaces;
	DWORD32  NumSubset;
	DWORD32  PositionBlock;
	DWORD32  NormalBlock;
	DWORD32  DiffuseBlock;
	DWORD32  TextureUVW1Block;
	DWORD32  TextureUVW2Block;
	DWORD32  TextureUVW3Block;
	DWORD32  FacesIndexBlock;
	DWORD32  SubsetIndexBlock;
	DWORD32  SkinInfoBlock;
	DWORD32  LODInfoBlock;
	DWORD32  FlexibleBodyBlock;
	DWORD32  BBoxBlock;
	DWORD32  BlendMeshBlock;
	DWORD32  TangentBlock;
	DWORD32  ExtendBlock[16];

	_MeshHead()
	{
		NumVertices      = 0;
		NumFaces         = 0;
		NumSubset        = 0;
		PositionBlock    = 0;
		NormalBlock      = 0;
		DiffuseBlock     = 0;
		TextureUVW1Block = 0;
		TextureUVW2Block = 0;
		TextureUVW3Block = 0;
		FacesIndexBlock  = 0;
		SubsetIndexBlock = 0;
		SkinInfoBlock    = 0;
		LODInfoBlock     = 0;
		FlexibleBodyBlock = 0;
		BBoxBlock = 0;
		BlendMeshBlock = 0;
		TangentBlock = 0;
		ZeroMemory(ExtendBlock,sizeof(DWORD32)*16);
	}
};


struct _MeshFileHead
{
	TFileHeader   KSFileHeader;
	DWORD32       FileMask;
	DWORD32       BlockLength;
	DWORD32       MaterialBlock;
	DWORD32       AnimationBlock;
	DWORD32       ExtendBlock[10];
	DWORD32       MeshCount;
	_MeshHead     MeshHead;

	_MeshFileHead()
	{
		FileMask       = 0x4D455348;
		BlockLength    = 0;
		MaterialBlock  = 0;
		AnimationBlock = 0;
		ZeroMemory(ExtendBlock,sizeof(DWORD32)*10);
		MeshCount      = 1;
	};
};

struct _BlendShapeFileHead
{
    TFileHeader KSFileHeader;
    DWORD32     FileMask;
    DWORD32     dwFileVersion;
    DWORD32     dwVertexCount;
    DWORD32     dwTargetShapeCount;
    DWORD32     dwTargetShapeDataPos;
    DWORD32     dwBlendShapeCount;
    _BlendShapeFileHead()
    {
        FileMask = 0x424C5350;  //BLSP
        dwFileVersion = 1;
        dwVertexCount = dwTargetShapeCount = dwTargetShapeDataPos = dwBlendShapeCount = 0;
    }
};
#pragma pack(pop)

void SaveMeshFile(char* szOutMeshName, KMesh *pKMesh, const char *szOutDir, const char *sourceName, BOOL bMdl, BOOL bCollision)
{
	int nRetCode = false;
	_MeshFileHead MeshFileHead;	

	DWORD32 BlockEnd  = 0xFFFFFFFF;  

	char pFileName[MAX_PATH];
	int len = (int) strlen(szOutDir);

	BOOL bHD = ExportOption()->bHD;
	char ext[MAX_PATH] = { 0 };
	char szLODExt[MAX_PATH] = {0};
	if (pKMesh->m_nLOD > 0)
		sprintf(szLODExt, "_lod%d", pKMesh->m_nLOD);

	if (bCollision) strcpy_s(ext, ".CollisionMesh");
	else
	{
		if (bHD) sprintf(ext, "_HD%s%s", szLODExt, ".mesh");
		else     sprintf(ext, "%s%s", szLODExt, ".mesh");
	}
	

	if (!bMdl && !ExportOption()->bGroupExport)
	{
		sprintf(szOutMeshName, "%s%s", sourceName, ext);
		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
			sprintf(pFileName, "%s%s%s", szOutDir, sourceName, ext);
		else
			sprintf(pFileName, "%s\\%s%s", szOutDir, sourceName, ext);

		if (_access(szOutDir, 0) != 0)
		{
			Make_Dirs(szOutDir);
		}
	}
	else
	{
		char szTotalPath[MAX_PATH];
		char szMeshOutDir[MAX_PATH];

		char *p = strstri(pKMesh->m_szMeshName.c_str(), "#");
		if (p)
		{
			GetMapsSourcePathFromPath(szOutDir, szMeshOutDir, MAX_PATH);
		}
		else
		{
			strcpy(szMeshOutDir, szOutDir);
		}

		string strMeshName = pKMesh->m_szMeshName;
		ReplaceAll(strMeshName, "#", "\\");

		//if (!bMdl && bCollision)
		//{
		//	unsigned uSuffixLen = (unsigned)strlen(SUFFIX_COLLISION);
		//	string::size_type nIdx = strMeshName.rfind(SUFFIX_COLLISION);
		//	assert(nIdx != string::npos);
		//	assert(nIdx == strMeshName.length() - uSuffixLen);
		//	strMeshName.replace(nIdx, 2, "");
		//}
		
		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
		{
			sprintf(pFileName, "%s%s%s", szMeshOutDir, strMeshName.c_str(), ext);
		}
		else
		{
			sprintf(pFileName, "%s\\%s%s", szMeshOutDir, strMeshName.c_str(), ext);
		}
		sprintf(szOutMeshName, "%s%s", strMeshName.c_str(), ext);

		GetParentPath(pFileName, szTotalPath, MAX_PATH);
		if (_access(szTotalPath, 0) != 0)
		{
			Make_Dirs(szTotalPath);
		}

	}
	
	FILE* pFile = fopen(pFileName,"wb");
	if(!pFile)
	{
		goto Exit0;
	}

	if(!ExportOption()->bGroupExport && !ExportOption()->bOnlyAniExport)
	{
		SetSaveFilePath(pFileName, MAX_PATH);
	}

	DWORD32   Pos_Start = ftell(pFile);
	MeshFileHead.MeshCount = 1;
	MeshFileHead.MeshHead.NumFaces =  pKMesh->m_numFaces;
	MeshFileHead.MeshHead.NumVertices = (int) pKMesh->m_verts.size();
	MeshFileHead.MeshHead.NumSubset = (int) pKMesh->m_SubsetIds.size();

	fwrite(&MeshFileHead,sizeof(_MeshFileHead),1,pFile);

	int vertCount = (int) pKMesh->m_verts.size();

	//position
	MeshFileHead.MeshHead.PositionBlock = ftell(pFile) - Pos_Start;
	int n = 0;
	//printf("====================\r\n");
    //test 测试blendshape
    //vector<KPCNT3>& verts = pKMesh->m_blendShapes[0]->m_vBlendShapeChannels[1]->m_vBlendShapeTargets[0]->m_vControlPoints;
    vector<KPCNT3>& verts = pKMesh->m_verts;

	for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
	{
		KPCNT3 p = *it;
		FixPoint(n, &p.pos, pKMesh);
		if (strcmp(pKMesh->m_szMeshName.c_str(), "1") == 0)
		{
			printf("[%d] %f %f %f \r\n", n, p.pos.x, p.pos.y, p.pos.z);
		}
		fwrite(&p.pos, sizeof(KVec3), 1, pFile);
		++n;
	}
	fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);
	
	//printf("====================\r\n");
	n = 0;
	//normal
	MeshFileHead.MeshHead.NormalBlock = ftell(pFile) - Pos_Start;
	for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
	{
		KPCNT3 p = *it;
		if (!ExportOption()->Switch())
		{
			//p.nomral.x = p.nomral.x;//
			p.nomral.z = -p.nomral.z;//
		}
		else
		{
			float tmp = 0;
			tmp = p.nomral.z;
			p.nomral.z = p.nomral.y;
			p.nomral.y = tmp;
		}
		
		if (strcmp(pKMesh->m_szMeshName.c_str(), "1") == 0)
		{
			printf("[%d]%f %f %f \r\n", n, p.nomral.x, p.nomral.y, p.nomral.z);
		}
		//printf("[%d]%f %f %f \r\n", n, p.nomral.x, p.nomral.y, p.nomral.z);
		fwrite(&p.nomral, sizeof(KVec3), 1, pFile);		
		++n;
	}
	fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);

	//diffuse
	if(pKMesh->m_nColorElementCount > 0)
	{
		MeshFileHead.MeshHead.DiffuseBlock = ftell(pFile) - Pos_Start;
		for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
		{
			KPCNT3& p = *it;
			fwrite(&p.color, sizeof(DWORD32), 1, pFile);
		}
		fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);
	}	

	//uv0
	if(pKMesh->m_nUvElementCount > 0)
	{
		MeshFileHead.MeshHead.TextureUVW1Block = ftell(pFile) - Pos_Start;
		for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
		{
			KPCNT3 &p = *it;
			fwrite(&p.uvw0, sizeof(KVec3), 1, pFile);
		}
		fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);
	}

	//uv1
	if(pKMesh->m_nUvElementCount > 1)
	{
		MeshFileHead.MeshHead.TextureUVW2Block = ftell(pFile) - Pos_Start;
		for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
		{
			KPCNT3 &p = *it;
			fwrite(&p.uvw1, sizeof(KVec3), 1, pFile);
		}
		fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);
	}
	
	//uv2,引擎目前并不支持3套uv
	//if(pKMesh->m_nUvElementCount > 2)
	//{
	//	MeshFileHead.MeshHead.TextureUVW3Block = ftell(pFile) - Pos_Start;
	//	for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
	//	{
	//		KPCNT3 &p = *it;
	//		fwrite(&p.uvw2, sizeof(KVec3), 1, pFile);
	//	}
	//	fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);
	//}

	// face index  buffer
	MeshFileHead.MeshHead.FacesIndexBlock = ftell(pFile) - Pos_Start;
	for(int i = 0; i < pKMesh->m_numFaces; ++i)
	{
		int id0 = pKMesh->m_inds[i * 3 + 0];
		int id1 = pKMesh->m_inds[i * 3 + 1];
		int id2 = pKMesh->m_inds[i * 3 + 2];

		//if(ExportOption()->bExchangeYZ)

		if (!ExportOption()->Switch())
		{			
			fwrite(&id0, sizeof(DWORD32), 1, pFile);
			fwrite(&id1, sizeof(DWORD32), 1, pFile);
			fwrite(&id2, sizeof(DWORD32), 1, pFile);
		}
		else
		{
			fwrite(&id0, sizeof(DWORD32), 1, pFile);
			fwrite(&id1, sizeof(DWORD32), 1, pFile);
			fwrite(&id2, sizeof(DWORD32), 1, pFile);
		}
	}
	//fwrite(&pKMesh->m_inds[0], sizeof(DWORD32), (int) pKMesh->m_inds.size(), pFile);
	fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);
	
	//subset index
	MeshFileHead.MeshHead.SubsetIndexBlock = ftell(pFile) - Pos_Start;
	fwrite(&pKMesh->m_vecFaceSubsetId[0], sizeof(DWORD32), (int) pKMesh->m_vecFaceSubsetId.size(), pFile);
	fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);

	
	if(!pKMesh->m_vecBones.empty())
	{
		int n1 = ftell(pFile);
		//skin info
		MeshFileHead.MeshHead.SkinInfoBlock = ftell(pFile) - Pos_Start;
		DWORD32 dwNumBones = (DWORD32) pKMesh->m_vecBones.size();
		fwrite(&dwNumBones, sizeof(DWORD32), 1, pFile);
		for(DWORD32 i_Bone = 0; i_Bone < dwNumBones; i_Bone ++)
		{			
			char fileName[MAX_BONE_NAME_LENGTH];
			KMeshBone *pBone = pKMesh->m_vecBones[i_Bone];
			fwrite(pBone->szBoneName, sizeof(char), MAX_BONE_NAME_LENGTH, pFile);
			strncpy(fileName, pBone->m_szParentName.c_str(), MAX_BONE_NAME_LENGTH);
			fwrite(fileName, sizeof(char), MAX_BONE_NAME_LENGTH, pFile);
			DWORD32 nChildBoneCount = (DWORD32) pBone->m_vecChildNames.size();
			fwrite(&nChildBoneCount, sizeof(DWORD32), 1, pFile);
			for(DWORD32 i_Child = 0; i_Child < nChildBoneCount; ++i_Child)
			{
				strncpy(fileName, pBone->m_vecChildNames[i_Child].c_str(), MAX_BONE_NAME_LENGTH);
				fwrite(fileName, sizeof(char), MAX_BONE_NAME_LENGTH, pFile);
			}
			fwrite(&pBone->MatrixOffset, sizeof(KMatrix), 1, pFile);
			//这个不用了，只是占个位置而已
			fwrite(&pBone->MatrixOffset, sizeof(KMatrix), 1, pFile);
			DWORD32 dwNumVertices = (DWORD32) pBone->m_vecIndexRef.size();
			fwrite(&dwNumVertices, sizeof(DWORD32), 1, pFile);
			if(dwNumVertices)
			{
				fwrite(&pBone->m_vecIndexRef[0], sizeof(DWORD32), dwNumVertices, pFile);
				fwrite(&pBone->m_vecWeightRef[0], sizeof(float), dwNumVertices, pFile);
			}

		}
		int n = ftell(pFile);

		//socket
		DWORD32 NunSocket = (DWORD32) pKMesh->m_vecSocket.size();
		fwrite(&NunSocket,sizeof(DWORD32),1,pFile);
		for(vector<KSocket *>::iterator it = pKMesh->m_vecSocket.begin(); it != pKMesh->m_vecSocket.end(); ++it)
		{
			KSocket *pSocket = *it;
			char szName[30];
			char szParentName[30];
			strncpy(szName, pSocket->szName.c_str(), 30);
			strncpy(szParentName, pSocket->szParentName.c_str(), 30);
			fwrite(szName, sizeof(char), 30, pFile);
			fwrite(szParentName, sizeof(char), 30, pFile);
			fwrite(&pSocket->MatrixOffset, sizeof(KMatrix), 1, pFile);
		}
		
		fwrite(&BlockEnd,sizeof(DWORD32),1,pFile);

		// write flexible body information
		DWORD32 dwFBNum = 0;
	}
	
	//printf("====================\r\n");	
	n = 0;
	//tangent
	MeshFileHead.MeshHead.TangentBlock = ftell(pFile) - Pos_Start;
	for(vector<KPCNT3>::iterator it = pKMesh->m_verts.begin(); it != pKMesh->m_verts.end(); ++it)
	{
		KPCNT3 p = *it;
		if (!ExportOption()->Switch())
		{
			//p.tangent.x = p.tangent.x;//
			p.tangent.z = -p.tangent.z;//
		}
		else
		{
			float tmp = 0;
			tmp = p.tangent.z;
			p.tangent.z = p.tangent.y;
			p.tangent.y = tmp;
		}
		//printf("[%d]%f %f %f %f\r\n", n, p.tangent.x, p.tangent.y, p.tangent.z, p.tangent.w);
		fwrite(&p.tangent, sizeof(KVec4), 1, pFile);		
		++n;
	}
	fwrite(&BlockEnd, sizeof(DWORD32), 1, pFile);
	
	MeshFileHead.BlockLength = ftell(pFile) - Pos_Start;
	fseek(pFile, Pos_Start, SEEK_SET);
	fwrite(&MeshFileHead, sizeof(_MeshFileHead), 1, pFile);	
	fclose(pFile);

	//直接调用kg3d_meshtobin.exe来压缩mesh
	if (bHD)
	{
		char szParam[MAX_PATH * 2];
		sprintf_s(szParam, "\"%s\" \"%s\"", pFileName, pFileName);
		ShellExecute(0, "open", "KG3D_MeshToBinX64.exe", szParam, ".", SW_HIDE);

		if (strstr(pKMesh->m_szMeshName.c_str(), " "))
		{
			char szErr[512];
			sprintf_s(szErr, "名字有空格：%s", pKMesh->m_szMeshName.c_str());
			MessageBoxA(NULL, szErr, "ERROR", 0);
		}
	}

Exit0:
    SaveMeshBlendShapeFile(pKMesh, szOutDir, sourceName, bMdl);
	return;
}


void SaveMeshBlendShapeFile(KMesh *pKMesh, const char *szOutDir, const char *sourceName, BOOL bMdl)
{
    if (pKMesh->m_blendShapes.size() <= 0)
    {
        return;
    }
    int len = (int) strlen(szOutDir);
    char pFileName[MAX_PATH];

	char szTotalPath[MAX_PATH];

	char szMeshOutDir[MAX_PATH];
    char szSourceName[MAX_PATH];

	char *p = strstri(pKMesh->m_szMeshName.c_str(), "#");
	if (p)
	{
		GetMapsSourcePathFromPath(szOutDir, szMeshOutDir, MAX_PATH);
	}
	else
	{
		strcpy(szMeshOutDir, szOutDir);
	}

	string strMeshName = pKMesh->m_szMeshName.c_str();
	ReplaceAll(strMeshName, "#", "\\");
    if (ExportOption()->bHD)
    {
        strMeshName = strMeshName + "_HD";
        sprintf_s(szSourceName, "%s_HD", sourceName);
    }
    else
        strcpy_s(szSourceName, sourceName);
        

    if(bMdl)
    {	
        if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
            sprintf(pFileName, "%s%s.mesh.blendshape", szMeshOutDir, strMeshName.c_str());
        else
            sprintf(pFileName, "%s\\%s.mesh.blendshape", szMeshOutDir, strMeshName.c_str());
    }
    else
    {
        if(ExportOption()->bGroupExport)
        {
			char szRelativePath[MAX_PATH];
			char szNoPrefixFileName[MAX_PATH];
			GetRelativePathFromFileName(pKMesh->m_szMeshName.c_str(), szRelativePath, MAX_PATH);
			RemoveRelativePrefix(pKMesh->m_szMeshName.c_str(), szNoPrefixFileName, MAX_PATH);

            if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
                sprintf(pFileName, "%s%s%s_%s.mesh.blendshape", szMeshOutDir, szRelativePath, szSourceName, szNoPrefixFileName);
            else
                sprintf(pFileName, "%s\\%s%s_%s.mesh.blendshape", szMeshOutDir, szRelativePath, szSourceName, szNoPrefixFileName);
        }
        else
        {
            if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
                sprintf(pFileName, "%s%s.mesh.blendshape", szOutDir, szSourceName);
            else
                sprintf(pFileName, "%s\\%s.mesh.blendshape", szOutDir, szSourceName);
        }
    }

	printf("blendshape:  %s\n", pFileName);
    _BlendShapeFileHead fileHead;
    FILE* pFile = fopen(pFileName,"wb");
    if(!pFile)
    {
        goto Exit0;
    }    
    int nTargetID = 0;
    int nVerticesCount = pKMesh->m_verts.size();
//    fwrite(&nVerticesCount, sizeof(int), 1, pFile);
    long lTargetCountPos = 0;//ftell(pFile);
//    fwrite(&nTargetID, sizeof(int), 1, pFile);      //target的数量，全部写完后还要写回来
    long lTargetDataPos = 0;    //target顶点数据起始位置，后面记录完再写回来
//    fwrite(&lTargetDataPos, sizeof(long), 1, pFile);
    int nBlendShapesCount = pKMesh->m_blendShapes.size();
//    fwrite(&nBlendShapesCount, sizeof(int), 1, pFile);    
    fileHead.dwFileVersion = 1;         //第一版，存相对于原顶点的偏移数据
    fileHead.dwVertexCount = nVerticesCount;
    fileHead.dwTargetShapeCount = nTargetID;
    fileHead.dwTargetShapeDataPos = lTargetDataPos;
    fileHead.dwBlendShapeCount = nBlendShapesCount;
    fwrite(&fileHead, sizeof(_BlendShapeFileHead), 1, pFile);
    for (int i = 0; i < nBlendShapesCount; ++i)
    {
        KBlendShape* pBlendShape = pKMesh->m_blendShapes[i];
        fwrite(pBlendShape->szName.c_str(), sizeof(char), MAX_PATH, pFile);
        int nChannelCount = pBlendShape->m_vBlendShapeChannels.size();
        fwrite(&nChannelCount, sizeof(int), 1, pFile);
        for (int j = 0; j < nChannelCount; ++j)
        {
            KBlendShapeChannel* pChannel = pBlendShape->m_vBlendShapeChannels[j];
            fwrite(pChannel->szName.c_str(), sizeof(char), MAX_PATH, pFile);
            int nTargetCount = pChannel->m_vBlendShapeTargets.size();
            fwrite(&nTargetCount, sizeof(int), 1, pFile);
            for (int k = 0; k < nTargetCount; ++k)
            {
                KBlendShapeTarget* pTarget = pChannel->m_vBlendShapeTargets[k];
                fwrite(pTarget->szName.c_str(), sizeof(char), MAX_PATH, pFile);
                fwrite(&nTargetID, sizeof(int), 1, pFile);
                nTargetID++;
                fwrite(&pTarget->fHightWeight, sizeof(float), 1, pFile);
            }
        }
    }
    lTargetDataPos = ftell(pFile);
    //全部顶点数据写在一起
    for (int i = 0; i < nBlendShapesCount; ++i)
    {
        for (int j = 0; j < pKMesh->m_blendShapes[i]->m_vBlendShapeChannels.size(); ++j)
        {
            for (int k = 0; k < pKMesh->m_blendShapes[i]->m_vBlendShapeChannels[j]->m_vBlendShapeTargets.size(); ++ k)
            {
                KBlendShapeTarget* pTarget = pKMesh->m_blendShapes[i]->m_vBlendShapeChannels[j]->m_vBlendShapeTargets[k];
                vector<KPCNT3>& verts = pTarget->m_vControlPoints;
                int n = 0;
                for(vector<KPCNT3>::iterator it = verts.begin(); it != verts.end(); ++it)
                {
                    KPCNT3 pOriginal = pKMesh->m_verts[n];
                    //pos
                    KPCNT3 p = *it;
                    FixPoint(n, &p.pos, pKMesh, false);
                    FixPoint(n, &pOriginal.pos, pKMesh, false);
                    p.pos.x = p.pos.x - pOriginal.pos.x;
                    p.pos.y = p.pos.y - pOriginal.pos.y;
                    p.pos.z = p.pos.z - pOriginal.pos.z;
                    fwrite(&p.pos, sizeof(KVec3), 1, pFile);
                    //normal
                    p = *it;
					if (!ExportOption()->Switch())
					{
						//p.nomral.x = p.nomral.x;//
                        //pOriginal.nomral.x = pOriginal.nomral.x;//
						p.nomral.z = -p.nomral.z;//
						pOriginal.nomral.z = -pOriginal.nomral.z;//
					}
					else
                    {
                        float tmp = 0;
                        tmp = p.nomral.z;
                        p.nomral.z = p.nomral.y;
                        p.nomral.y = tmp;

                        tmp = pOriginal.nomral.z;
                        pOriginal.nomral.z = pOriginal.nomral.y;
                        pOriginal.nomral.y = tmp;
                    }
                    p.nomral.x = p.nomral.x - pOriginal.nomral.x;
                    p.nomral.y = p.nomral.y - pOriginal.nomral.y;
                    p.nomral.z = p.nomral.z - pOriginal.nomral.z;
                    fwrite(&p.nomral, sizeof(KVec3), 1, pFile);
                    //tangent
                    p = *it;
					if (!ExportOption()->Switch())
					{
						//p.tangent.x = p.tangent.x;//
                        //pOriginal.tangent.x = pOriginal.tangent.x;//
						p.tangent.z = -p.tangent.z;//
						pOriginal.tangent.z = -pOriginal.tangent.z;//
					}
					else
                    {
                        float tmp = 0;
                        tmp = p.tangent.z;
                        p.tangent.z = p.tangent.y;
                        p.tangent.y = tmp;
                        tmp = pOriginal.tangent.z;
                        pOriginal.tangent.z = pOriginal.tangent.y;
                        pOriginal.tangent.y = tmp;
                    }
                    p.tangent.x = p.tangent.x - pOriginal.tangent.x;
                    p.tangent.y = p.tangent.y - pOriginal.tangent.y;
                    p.tangent.z = p.tangent.z - pOriginal.tangent.z;
                    //tangent写float3，以便对齐。
                    KVec3 tangent;
                    tangent.x = p.tangent.x;
                    tangent.y = p.tangent.y;
                    tangent.z = p.tangent.z;
                    fwrite(&tangent, sizeof(KVec3), 1, pFile);	

                    ++n;
                }
            }
        }
    }
//    fseek(pFile, lTargetCountPos, SEEK_SET);
//    fwrite(&nTargetID, sizeof(int), 1, pFile);      //target的数量，全部写完后还要写回来
//    fwrite(&lTargetDataPos, sizeof(long), 1, pFile); //target顶点数据起始位置，后面记录完再写回来
    fseek(pFile, 0, SEEK_SET);
    fileHead.dwTargetShapeCount = nTargetID;
    fileHead.dwTargetShapeDataPos = lTargetDataPos;
    fwrite(&fileHead, sizeof(_BlendShapeFileHead), 1, pFile);
    fclose(pFile);
Exit0:
    return;
}

void SaveMeshAnimationToFile_NoCompress(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser,  int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo)
{
	char fileName[MAX_PATH];
	int len = (int) strlen(pOutDir);

	char szTotalPath[MAX_PATH];

	char szMeshOutDir[MAX_PATH];

	char *p = strstri(szName, "#");
	if (p)
	{
		GetMapsSourcePathFromPath(pOutDir, szMeshOutDir, MAX_PATH);
	}
	else
	{
		strcpy(szMeshOutDir, pOutDir);
	}

	string strMeshName = szName;
	ReplaceAll(strMeshName, "#", "\\");

	if(pOutDir[len - 1] == '\\'  || pOutDir[len - 1] == '/')
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}
	else
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s\\%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s\\%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}
	
	FILE *fp = NULL;	

	DWORD32 Mask = 0x414E494D;//ANIM对应的ASECC代码	
	DWORD32 BlockLength = 0;
	DWORD32 NumAnimation = 1;
	DWORD32 BlockEnd  = 0xFFFFFFFF;
	DWORD32 dwAnimationType = ANIMATION_BONE_RTS;
	float fFrameLen = nFrameTime;
	pParser->m_nSampleFrameInterval = nSampleInterval;
	DWORD32 dwBoneNum = (DWORD32) pKMesh->m_vecBones.size();
	const char *szAniName = szName;
	if(!pParser->m_nAnimationFrames)
		goto Exit0;
	
	GetParentPath(fileName, szTotalPath, MAX_PATH);
	if (_access(szTotalPath, 0) != 0)
	{
		Make_Dirs(szTotalPath);
	}

	fp = fopen(fileName, "wb");
	if(!fp)
		goto Exit0;



	int nFrames = 0;
	for(int i = nFrameFrom; i < nFrameTo; i += pParser->m_nSampleFrameInterval)
	{
		nFrames ++;
	}

	fwrite(&Mask, sizeof(DWORD32), 1, fp);

	BlockLength += sizeof(DWORD32) * 3;//动画数目，结束标志，动画类型
	BlockLength += sizeof(float) + sizeof(DWORD32)*2;//时间间隔，骨骼数目，动作帧数
	BlockLength += sizeof(char) * 30 * dwBoneNum;//骨骼名称  m_dNumAnimationBones:7
	BlockLength += sizeof(RTSBoneAnimation) * dwBoneNum * pParser->m_nAnimationFrames;//骨骼变换矩阵

	fwrite(&BlockLength, sizeof(DWORD32), 1, fp);
	fwrite(&NumAnimation,sizeof(DWORD32), 1, fp);                      //动画数目，这里默认为１	
	fwrite(&dwAnimationType, sizeof(DWORD32), 1, fp);
	fwrite( szAniName, sizeof(char), 30, fp);

	fwrite(&dwBoneNum, sizeof(DWORD32), 1, fp);
	fwrite(&nFrames, sizeof(DWORD32), 1, fp);
	fwrite(&fFrameLen, sizeof(float), 1, fp);

	for(DWORD32 i_Bone = 0; i_Bone < dwBoneNum; i_Bone++)
	{
		const char * BoneName = pKMesh->m_vecBones[i_Bone]->szBoneName;
		fwrite(BoneName,sizeof(char), 30, fp); 
	}		


	for(vector<KMeshBone *>::iterator it = pKMesh->m_vecBones.begin(); it != pKMesh->m_vecBones.end(); ++it)
	{
		KMeshBone *pBone = *it;
		for(int i = nFrameFrom; i < nFrameTo; i += pParser->m_nSampleFrameInterval)
		{
			FbxTime tm;
			tm.SetMilliSeconds(pParser->m_nFrameTimeFrameMilSecond * i);

			if(pBone->bIsRoot)
			{	
				//作为根骨骼来保存				
				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
				Matrix3 maxMat;
				ToMaxMat3(maxMat, m);
				maxMat = maxMat * g_Getmat3Inch2CMScale();
				g_ChangeMatrixYZ(&maxMat);
				AffineParts ap;
				decomp_affine(maxMat, &ap);
				RTSBoneAnimation rts;
				ToRTS(rts, ap);
				fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);
			}
			else
			{
				//子骨骼的保存
				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
				Matrix3 mMat;
				ToMaxMat3(mMat, m);


				FbxAMatrix mp;				
				KMeshBone *pParentMeshBone = pKMesh->GetBone(pBone->m_szParentName.c_str());
				if(pParentMeshBone)
				{
					mp = pParentMeshBone->pFbxNode->EvaluateGlobalTransform(tm);
				}
				else
				{
					mp = pBone->pFbxNode->GetParent()->EvaluateGlobalTransform(tm);
				}

				Matrix3 mParentInverse;
				ToMaxMat3(mParentInverse, mp);
				mParentInverse.Invert();

				Matrix3 mlocal = mMat * mParentInverse;

				g_ChangeMatrixYZ(&mlocal);
				AffineParts ap;
				decomp_affine(mlocal, &ap);
				RTSBoneAnimation rts;
				ToRTS(rts, ap);
				fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);
			}			
		}
	}

	fwrite(&BlockEnd, sizeof(DWORD32), 1, fp);

Exit0:
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}

	return;
}



void SaveMeshAnimationToFile_Compress(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser,  int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo)
{
	char fileName[MAX_PATH];
	int len = (int) strlen(pOutDir);

	char szTotalPath[MAX_PATH];

	char szMeshOutDir[MAX_PATH];

	char *p = strstri(szName, "#");
	if (p)
	{
		GetMapsSourcePathFromPath(pOutDir, szMeshOutDir, MAX_PATH);
	}
	else
	{
		strcpy(szMeshOutDir, pOutDir);
	}

	string strMeshName = szName;
	ReplaceAll(strMeshName, "#", "\\");

	if(pOutDir[len - 1] == '\\'  || pOutDir[len - 1] == '/')
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}
	else
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s\\%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s\\%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}

	GetParentPath(fileName, szTotalPath, MAX_PATH);
	if (_access(szTotalPath, 0) != 0)
	{
		Make_Dirs(szTotalPath);
	}
	
	int i = 0;
	HRESULT hResult = E_FAIL;
	HRESULT hRetCode = E_FAIL;
	KG3DAniFileWrap AniFileWrap;
	BONERTS_ANI_DATA boneRTSAniData;
	ANI_FILE_HEADER& Header = boneRTSAniData.fileHeader;
	BONERTS_ANI_HEAD& RTSHead = boneRTSAniData.boneRTSAniHead;
	pParser->m_nSampleFrameInterval = nSampleInterval;
	RTSINFO** ppBoneRTS = NULL;
	DWORD32 dwBoneNum = (DWORD32) pKMesh->m_vecBones.size();
	DWORD32 dwNumFrames = (DWORD32) pParser->m_nAnimationFrames;
	strcpy_s(Header.strDesc, MAX_CLIPDESC_LEN, szName);
	Header.strDesc[MAX_CLIPDESC_LEN - 1] = 0;
	Header.dwMask = ANI_FILE_MASK;
	Header.dwBlockLength = sizeof(ANI_FILE_HEADER) + sizeof(BONERTS_ANI_HEAD) + 
		sizeof(TCHAR) * MAX_CLIPDESC_LEN * dwBoneNum +
		sizeof(RTSINFO) * dwBoneNum * dwNumFrames +
		sizeof(DWORD32);//EndFlag
	Header.dwNumAnimations = 1;
	Header.dwType = ANIMATION_BONE_RTS;

	RTSHead.dwNumBones = dwBoneNum;
	
	RTSHead.fFrameLength = nFrameTime;

	TCHAR **ppName = new TCHAR*[dwBoneNum];

	for(DWORD32 i_Bone = 0; i_Bone < dwBoneNum; i_Bone++)
	{
		const char * BoneName = pKMesh->m_vecBones[i_Bone]->szBoneName;
		char *pName = new char[30];
		strncpy(pName, BoneName, 30);
		ppName[i_Bone] = pName;
	}

	boneRTSAniData.ppszBoneName = ppName;

	ppBoneRTS = new RTSINFO*[RTSHead.dwNumBones];
	KGLOG_PROCESS_ERROR(ppBoneRTS);

	i = 0;
	int nFrames = 0;
	for(int i = nFrameFrom; i < nFrameTo; i += pParser->m_nSampleFrameInterval)
	{
		nFrames ++;
	}
	RTSHead.dwNumFrames = nFrames;

	for(vector<KMeshBone *>::iterator it = pKMesh->m_vecBones.begin(); it != pKMesh->m_vecBones.end(); ++it)
	{
		KMeshBone *pBone = *it;
		ppBoneRTS[i] = new RTSINFO[nFrames];
		int jj = 0;
		for(int j = nFrameFrom; j < nFrameTo; j += pParser->m_nSampleFrameInterval)
		{
			FbxTime tm;
			tm.SetMilliSeconds(pParser->m_nFrameTimeFrameMilSecond * j);

			if(pBone->bIsRoot)
			{	
				//作为根骨骼来保存				
				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
				Matrix3 maxMat;
				ToMaxMat3(maxMat, m);
				maxMat = maxMat * g_Getmat3Inch2CMScale();
				g_ChangeMatrixYZ(&maxMat);
				AffineParts ap;
				decomp_affine(maxMat, &ap);
				RTSBoneAnimation rts;
				ToRTS(rts, ap);
				//fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);
				memcpy(&ppBoneRTS[i][jj], &rts, sizeof(RTSINFO));
			}
			else
			{
				//子骨骼的保存
				FbxAMatrix m = pBone->pFbxNode->EvaluateGlobalTransform(tm);
				Matrix3 mMat;
				ToMaxMat3(mMat, m);


				FbxAMatrix mp;				
				KMeshBone *pParentMeshBone = pKMesh->GetBone(pBone->m_szParentName.c_str());
				if(pParentMeshBone)
				{
					mp = pParentMeshBone->pFbxNode->EvaluateGlobalTransform(tm);
				}
				else
				{
					mp = pBone->pFbxNode->GetParent()->EvaluateGlobalTransform(tm);
				}

				Matrix3 mParentInverse;
				ToMaxMat3(mParentInverse, mp);
				mParentInverse.Invert();

				Matrix3 mlocal = mMat * mParentInverse;

				g_ChangeMatrixYZ(&mlocal);
				AffineParts ap;
				decomp_affine(mlocal, &ap);
				RTSBoneAnimation rts;
				ToRTS(rts, ap);
				//fwrite(&rts, sizeof(RTSBoneAnimation), 1, fp);
				memcpy(&ppBoneRTS[i][jj], &rts, sizeof(RTSINFO));
			}
			++jj;
		}
		++i;
	}

	//for (DWORD32 i = 0; i < RTSHead.dwNumBones; ++i)
	//{
	//	ppBoneRTS[i] = new RTSINFO[RTSHead.dwNumFrames];
	//	KGLOG_PROCESS_ERROR(ppBoneRTS[i]);

	//	for (DWORD32 j = 0; j < RTSHead.dwNumFrames; ++j)
	//	{
	//		memcpy(&ppBoneRTS[i][j], &m_lpRTSAniBones[i * RTSHead.dwNumFrames + j], sizeof(RTSINFO));
	//	}
	//}
	boneRTSAniData.ppBoneRTS = ppBoneRTS;

	boneRTSAniData.dwEndFlag = ANI_FILE_END_FLAG;

	hRetCode = AniFileWrap.ConvertAniData((BYTE *)(&boneRTSAniData));
	KGLOG_COM_PROCESS_ERROR(hRetCode);

	hRetCode = AniFileWrap.SaveToFile(fileName);
	KGLOG_COM_PROCESS_ERROR(hRetCode);

	
	hResult = S_OK;
Exit0:
	if (ppBoneRTS)
	{
		for (DWORD32 i = 0; i < RTSHead.dwNumBones; ++i)
		{
			SAFE_DELETE_ARRAY(ppBoneRTS[i]);
			SAFE_DELETE_ARRAY(ppName[i]);
		}
		SAFE_DELETE_ARRAY(ppBoneRTS);
		SAFE_DELETE_ARRAY(ppName);
	}	
	boneRTSAniData.Clear();	
}

bool SaveMtlStdJsonInspack(char* szOutName, const char* szMtlName, int nSubsetCount, char** ppszBaseColor)
{
	bool bRet = false;

	FILE *fp = NULL;

	std::string strDiffuse;
	std::string strMre;
	std::string strNor;

	char pszDrive[_MAX_DRIVE] = { 0 };
	char pszDir[_MAX_DIR] = { 0 };
	char pszFname[_MAX_FNAME] = { 0 };
	char pszExt[_MAX_EXT] = { 0 };
	char pszInspackExt[] = ".JsonInspack";
	char szInspackName[_MAX_PATH] = { 0 };

	//mtl_std.jsoninspack 模板
	char szHead[] = "{\n    \"LOD\": [\n        {\n            \"Group\": [\n                {\n                    \"Subset\": [";
	char szSubsetHead[] = "\n                        {\n                            \"Info\": {\n                                \"Version\": 2,\n                                \"RefPath\": \"data\\\\material\\\\material\\\\mtl_std.jsondef\"\n                            },\n                            \"RenderState\": {\n                                \"bOverWriteRS\": 1,\n                                \"nRenderState\": 0,\n                                \"AlphaRef\": 128,\n                                \"AlphaRef2\": 128,\n                                \"BlendMode\": 0,\n                                \"DistortionMode\": 0\n                            },\n                            \"Param\": [\n                                {\n                                    \"Name\": \"BaseColorMap\",\n                                    \"Type\": \"Texture\",\n                                    \"Value\": ";
	char szSubsetBaseColorEnd[] = "\n                                },\n                                {\n                                    \"Name\": \"Map(MRE)\",\n                                    \"Type\": \"Texture\",\n                                    \"Value\": ";
	char szSubsetMreEnd[] = "\n                                },\n                                {\n                                    \"Name\": \"Normal Map\",\n                                    \"Type\": \"Texture\",\n                                    \"Value\": ";
	char szSubsetNormalEnd[] = "\n                                },\n                                {\n                                    \"Name\": \"Emissive Color\",\n                                    \"Type\": \"Color\",\n                                    \"Value\": [\n                                        0.0,\n                                        0.0,\n                                        0.0,\n                                        1.0\n                                    ]\n                                },\n                                {\n                                    \"Name\": \"HighlightColor\",\n                                    \"Type\": \"Color\",\n                                    \"Value\": [\n                                        0.0,\n                                        0.0,\n                                        0.0,\n                                        1.0\n                                    ]\n                                }\n                            ]\n                        }";
	char szSubsetSpreprate[] = ",";
	char szEnd[] = "\n                    ]\n                }\n            ]\n        }\n    ]\n}\n";

	int nCurLen = 0;

	if (!szMtlName || !szMtlName[0] || nSubsetCount == 0 || !ppszBaseColor)
		return bRet;

	_splitpath_s(szMtlName, pszDrive, pszDir, pszFname, pszExt);
	sprintf_s(szInspackName, _MAX_PATH, "%s%s%s%s", pszDrive, pszDir, pszFname, pszInspackExt);
	sprintf_s(szOutName, _MAX_PATH, "%s%s", pszFname, pszInspackExt);

	fopen_s(&fp, szInspackName, "wt+");
	if (fp==0) 
	{ 
		printf("can't open file\n");
		return bRet;
	}
	fseek(fp, 0, SEEK_END);
	long filelen =ftell(fp); 
	rewind(fp);

	char** ppszDiffuseArray = new char*[nSubsetCount];
	char** ppszMreArray = new char*[nSubsetCount];
	char** ppszNorArray = new char*[nSubsetCount];

	if (!ppszDiffuseArray || !ppszMreArray || !ppszNorArray)
		return bRet;

	//公共的
	size_t uTotalLen = strlen(szHead) + 
		(strlen(szSubsetHead) + strlen(szSubsetBaseColorEnd) + strlen(szSubsetMreEnd) + strlen(szSubsetNormalEnd)) * nSubsetCount + 
		strlen(szSubsetSpreprate) * (nSubsetCount - 1) + strlen(szEnd);
	for (int i = 0; i < nSubsetCount; ++i)
	{
		if (!ppszBaseColor[i])
			return bRet;

		ppszDiffuseArray[i] = new char[_MAX_PATH];
		memset(ppszDiffuseArray[i], 0, sizeof(char) * _MAX_PATH);
		ppszMreArray[i] = new char[_MAX_PATH];
		memset(ppszMreArray[i], 0, sizeof(char) * _MAX_PATH);
		ppszNorArray[i] = new char[_MAX_PATH];
		memset(ppszNorArray[i], 0, sizeof(char) * _MAX_PATH);

		int nSrcLen = strlen(ppszBaseColor[i]);
		_splitpath_s(ppszBaseColor[i], pszDrive, pszDir, pszFname, pszExt);

		if (ppszDiffuseArray[i])
		{
			sprintf_s(ppszDiffuseArray[i], _MAX_PATH, "\"%s%s%s%s\"", pszDrive, pszDir, pszFname, pszExt);
			strDiffuse = ppszDiffuseArray[i];
			ReplaceAll(strDiffuse, "/", "\\\\");
			strcpy(ppszDiffuseArray[i], strDiffuse.c_str());
		}

		if (ppszMreArray[i])
		{
			sprintf_s(ppszMreArray[i], _MAX_PATH, "\"%s%s%s_mre%s\"", pszDrive, pszDir, pszFname, pszExt);
			strMre = ppszMreArray[i];
			ReplaceAll(strMre, "/", "\\\\");
			strcpy(ppszMreArray[i], strMre.c_str());
		}

		if (ppszNorArray[i])
		{
			sprintf_s(ppszNorArray[i], _MAX_PATH, "\"%s%s%s_nor%s\"", pszDrive, pszDir, pszFname, pszExt);
			strNor = ppszNorArray[i];
			ReplaceAll(strNor, "/", "\\\\");
			strcpy(ppszNorArray[i], strNor.c_str());
		}

		uTotalLen += strlen(ppszDiffuseArray[i]) + strlen(ppszMreArray[i]) + strlen(ppszNorArray[i]);

		if (i < nSubsetCount - 1)
		{
			uTotalLen += strlen(szSubsetSpreprate);
		}
	}

	char* szAll = new char[uTotalLen + 1];
	memset(szAll, 0, sizeof(char) * (uTotalLen + 1));

	nCurLen = strlen(szHead) + 1;
	sprintf_s(szAll, nCurLen, "%s", szHead);

	for (int i = 0; i < nSubsetCount; ++i)
	{
		nCurLen += strlen(szSubsetHead) +
			strlen(ppszDiffuseArray[i]) + strlen(szSubsetBaseColorEnd) + 
			strlen(ppszMreArray[i]) + strlen(szSubsetMreEnd) + 
			strlen(ppszNorArray[i]) + strlen(szSubsetNormalEnd);

		sprintf_s(szAll, nCurLen, "%s%s%s%s%s%s%s%s", szAll, szSubsetHead, ppszDiffuseArray[i], szSubsetBaseColorEnd, ppszMreArray[i], szSubsetMreEnd, ppszNorArray[i], szSubsetNormalEnd);

		if (i < nSubsetCount - 1)
		{
			nCurLen += strlen(szSubsetSpreprate);
			sprintf_s(szAll, nCurLen, "%s%s", szAll, szSubsetSpreprate);
		}
	}

	nCurLen += strlen(szEnd);
	sprintf_s(szAll, nCurLen, "%s%s", szAll, szEnd);

	fwrite(szAll, strlen(szAll), 1, fp);
	fclose(fp);

	for (int i = 0; i < nSubsetCount; ++i)
	{
		delete []ppszDiffuseArray[i];
		delete []ppszMreArray[i];
		delete []ppszNorArray[i];

		ppszDiffuseArray[i] = NULL;
		ppszMreArray[i] = NULL;
		ppszNorArray[i] = NULL;
	}
	delete []ppszDiffuseArray;
	delete []ppszMreArray;
	delete []ppszNorArray;
	ppszDiffuseArray = NULL;
	ppszMreArray = NULL;
	ppszNorArray = NULL;

	delete []szAll;
	szAll = NULL;

	bRet = true;
	return bRet;
}

void SaveJsoninspackFile(char* szOutName, KMesh *pKMesh, const char *szOutDir, const char *szTgaOutDir, const char *sourceName, BOOL bMdl)
{ 
	char** ppszDiffuse = NULL;
	char strFixTextureName[MAX_PATH] = {0};
	char szTextureDirve[MAX_PATH] = {0};
	char szTextureDir[MAX_PATH] = {0};
	char szTextureName[MAX_PATH] = {0};
	char szTextureExt[MAX_PATH] = {0};
	char pFileName[MAX_PATH];
	DWORD32 NumSubsets = (DWORD32) pKMesh->m_SubsetIds.size();

	BOOL bHD = ExportOption()->bHD;
	char ext[MAX_PATH] = { 0 };
	if (pKMesh->m_nLOD > 0)
	{
		if (bHD) sprintf(ext, "_HD_lod%d%s", pKMesh->m_nLOD, ".mesh");
		else     sprintf(ext, "_lod%d%s", pKMesh->m_nLOD, ".mesh");
	}
	else
	{
		if (bHD) strcpy_s(ext, "_HD.mesh");
		else     strcpy_s(ext, ".mesh");
	}
	
	
	if (NumSubsets == 0)
	{
		return;
	}

	ppszDiffuse = new char*[NumSubsets];
	KG_PROCESS_ERROR(ppszDiffuse);
	for (DWORD i_Sub = 0; i_Sub < NumSubsets; i_Sub ++)
	{
		ppszDiffuse[i_Sub] = new char[MAX_PATH];
		KG_PROCESS_ERROR(ppszDiffuse[i_Sub]);
		ZeroMemory(ppszDiffuse[i_Sub], MAX_PATH * sizeof(char));
	}

	int len = (int) strlen(szOutDir);
	

	if (!bMdl && !ExportOption()->bGroupExport)
	{
		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
			sprintf(pFileName, "%s%s%s", szOutDir, sourceName, ext);
		else
			sprintf(pFileName, "%s\\%s%s", szOutDir, sourceName, ext);

		if (_access(szOutDir, 0) != 0)
		{
			Make_Dirs(szOutDir);
		}
	}
	else
	{
		char szTotalPath[MAX_PATH];
		char szMeshOutDir[MAX_PATH];

		char *p = strstri(pKMesh->m_szMeshName.c_str(), "#");
		if (p)
		{
			GetMapsSourcePathFromPath(szOutDir, szMeshOutDir, MAX_PATH);
		}
		else
		{
			strcpy(szMeshOutDir, szOutDir);
		}

		string strMeshName = pKMesh->m_szMeshName;
		ReplaceAll(strMeshName, "#", "\\");

		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
		{
			sprintf(pFileName, "%s%s%s", szMeshOutDir, strMeshName.c_str(), ext);
		}
		else
		{
			sprintf(pFileName, "%s\\%s%s", szMeshOutDir, strMeshName.c_str(), ext);
		}

		GetParentPath(pFileName, szTotalPath, MAX_PATH);
		if (_access(szTotalPath, 0) != 0)
		{
			Make_Dirs(szTotalPath);
		}
	}

	int relativePath_offset = -1;
	const char *p = strstri(szTgaOutDir, "\\Data\\");
	if(p)
	{
		relativePath_offset  = p - szTgaOutDir;
	}
	int tgaLen = (int) strlen(szTgaOutDir);
	DWORD i_Sub = 0;
	for (set<int>::iterator it = pKMesh->m_SubsetIds.begin(); it != pKMesh->m_SubsetIds.end(); ++it)
	{
		int subsetId = *it;
		map<int, KMaterail>::iterator itt = pKMesh->m_Materials.find(subsetId);
		if(itt != pKMesh->m_Materials.end())
		{
			KMaterail &mtl = itt->second;

			if (_access(mtl.textureName.c_str(), 4) !=0)
			{
				strcpy_s(strFixTextureName, "data\\material\\Texture\\defaultWhite.tga");
			}
			else
			{
				_splitpath(mtl.textureName.c_str(), szTextureDirve, szTextureDir, szTextureName, szTextureExt);
				if (szTgaOutDir[tgaLen - 1] == '\\' || szTgaOutDir[tgaLen - 1] == '/')
					sprintf_s(strFixTextureName, "%s%s%s", szTgaOutDir + relativePath_offset + 1, szTextureName, szTextureExt);
				else
					sprintf_s(strFixTextureName, "%s\\%s%s", szTgaOutDir + relativePath_offset + 1, szTextureName, szTextureExt);
			}
			
			std::string strDiffuse = strFixTextureName;
			ReplaceAll(strDiffuse, "\\", "/");
			strcpy(ppszDiffuse[i_Sub], strDiffuse.c_str());
			
			i_Sub ++;
		}
	}
	
	KG_PROCESS_ERROR(SaveMtlStdJsonInspack(szOutName, pFileName, NumSubsets, ppszDiffuse));

Exit0:

	for (DWORD i_Sub = 0; i_Sub < NumSubsets; i_Sub++)
	{
		delete[] ppszDiffuse[i_Sub];
	}
	delete[] ppszDiffuse;
	return;
}

void SaveMeshAnimationToFile(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser,  int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress)
{
	if(bCompress)
	{
		SaveMeshAnimationToFile_Compress(pKMesh, szName, pOutDir, pParser, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo);
	}
	else
	{
		SaveMeshAnimationToFile_NoCompress(pKMesh, szName, pOutDir, pParser, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo);
	}
}



void SaveMeshVertexAnimationToFile_NoCompress(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo)
{
	char fileName[MAX_PATH];
	int len = (int) strlen(pOutDir);
	//vector<DWORD> idx;

	char szTotalPath[MAX_PATH];

	char szMeshOutDir[MAX_PATH];

	char *p = strstri(szName, "#");
	if (p)
	{
		GetMapsSourcePathFromPath(pOutDir, szMeshOutDir, MAX_PATH);
	}
	else
	{
		strcpy(szMeshOutDir, pOutDir);
	}

	string strMeshName = szName;
	ReplaceAll(strMeshName, "#", "\\");

	if(pOutDir[len - 1] == '\\'  || pOutDir[len - 1] == '/')
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}
	else
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s\\%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s\\%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}
	
	
	FILE *fp = NULL;


	DWORD32 Mask = 0x414E494D;//ANIM对应的ASECC代码	
	DWORD32 BlockLength = 0;
	DWORD32 NumAnimation = 1;
	DWORD32 BlockEnd  = 0xFFFFFFFF;
	DWORD32 dwAnimationType = ANIMATION_VERTICES;
	float fFrameLen = (float) nFrameTime;
	pParser->m_nSampleFrameInterval = nSampleInterval;

	DWORD32 NumAnimationVertices = (DWORD32) pKMesh->m_mapPointCache.size();
	DWORD32 numVertCount = (DWORD32) pKMesh->m_verts.size();
	const char *szAniName = szName;
	if(!pKMesh->m_dwPointCacheFrames)
		goto Exit0;

	GetParentPath(fileName, szTotalPath, MAX_PATH);
	if (_access(szTotalPath, 0) != 0)
	{
		Make_Dirs(szTotalPath);
	}

	fp = fopen(fileName, "wb");
	if(!fp)
		goto Exit0;

	fwrite(&Mask,sizeof(DWORD32),1,fp);

	BlockLength += sizeof(DWORD32)*4;//动画数目，结束标志，动画类型,顶点总数
	BlockLength += sizeof(TCHAR)*30;
	BlockLength += sizeof(float) + sizeof(DWORD32)*2;//时间间隔，顶点数目，动作帧数
	BlockLength += sizeof(DWORD32)*numVertCount;//顶点索引
	BlockLength += sizeof(KVec3)*NumAnimationVertices * pKMesh->m_dwPointCacheFrames;//顶点位置信息

	fwrite(&BlockLength, sizeof(DWORD32),1,fp);
	fwrite(&NumAnimation, sizeof(DWORD32),1,fp);               //动画数目，这里默认为１	
	fwrite(&dwAnimationType, sizeof(DWORD32),1,fp);
	fwrite( szName, sizeof(TCHAR),30,fp);


	fwrite(&numVertCount, sizeof(DWORD32), 1, fp);             //顶点总数，用来检测是否和网格相匹配 
	NumAnimationVertices = numVertCount;                       //引擎这部分值得优化
	fwrite(&NumAnimationVertices,sizeof(DWORD32), 1,fp);       //该动画中包含的顶点数目
	fwrite(&pKMesh->m_dwPointCacheFrames, sizeof(DWORD32), 1, fp);     //该动画中包含的桢数目
	fwrite(&fFrameLen, sizeof(float), 1, fp);             //该动画中包含的每桢之间的时间间隔

	//本来应该是这样
	//for(vector<KPCNT3>::iterator it = m_verts.begin(); it != m_verts.end(); ++it)
	//{
	//	KPCNT3 &point = *it;
	//	map<KVec3, KPointCache>::iterator itt = m_mapPointCache.find(point.pos);
	//	if(itt != m_mapPointCache.end())
	//	{
	//		KPointCache &pointcache = itt->second;
	//		idx.push_back(pointcache.id);
	//	}
	//	else
	//	{
	//		assert(0);
	//	}
	//}
	//fwrite(&idx[0], sizeof(DWORD32), (int) idx.size(), fp);

	//for(int i_Ver = 0; i_Ver < NumAnimationVertices;i_Ver++)
	//{	
	//	KVec3 vPoint = m_vecControlPoints[i_Ver];
	//	map<KVec3, KPointCache>::iterator itt = m_mapPointCache.find(vPoint);
	//	if(itt != m_mapPointCache.end())
	//	{
	//		KPointCache &pointcache = itt->second;
	//		for(DWORD32 i_Frame=0; i_Frame < m_dwPointCacheFrames; i_Frame++)
	//		{
	//			KVec3 pos = pointcache.vecPointCache[i_Frame];
	//			FixPoint(0, &pos, this);
	//			fwrite(&pos, sizeof(KVec3), 1, fp);
	//		}
	//	}
	//	else
	//	{
	//		assert(0);
	//	}
	//}

	//but,引擎索引数和动画顶点数量一致，所以这个索引形同虚设，存在很多冗余动画数据，值得改进
	for(int i = 0; i < (int) pKMesh->m_verts.size(); ++i)
	{
		fwrite(&i, sizeof(DWORD32), 1, fp);
	}
	for(int i = 0; i < NumAnimationVertices; ++i)
	{
		KPCNT3 &point = pKMesh->m_verts[i];
		map<KVec3, KMesh::KPointCache>::iterator it = pKMesh->m_mapPointCache.find(point.pos);
		if(it != pKMesh->m_mapPointCache.end())
		{
			KMesh::KPointCache &pointcache = it->second;
			for(DWORD32 i_Frame = 0; i_Frame < pKMesh->m_dwPointCacheFrames; i_Frame ++)
			{
				KVec3 pos = pointcache.vecPointCache[i_Frame];
				FixPoint(0, &pos, pKMesh);
				fwrite(&pos, sizeof(KVec3), 1, fp);
			}
		}
		else
		{
			assert(0);
		}
	}
	fwrite(&BlockEnd, sizeof(DWORD32), 1, fp);
Exit0:	

	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}

	return;
}

void SaveMeshVertexAnimationToFile_Compress(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo)
{
	char fileName[MAX_PATH];
	int len = (int) strlen(pOutDir);
	//vector<DWORD> idx;

	char szTotalPath[MAX_PATH];

	char szMeshOutDir[MAX_PATH];

	char *p = strstri(szName, "#");
	if (p)
	{
		GetMapsSourcePathFromPath(pOutDir, szMeshOutDir, MAX_PATH);
	}
	else
	{
		strcpy(szMeshOutDir, pOutDir);
	}

	string strMeshName = szName;
	ReplaceAll(strMeshName, "#", "\\");

	if(pOutDir[len - 1] == '\\'  || pOutDir[len - 1] == '/')
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}
	else
	{
		if(nFrameFrom ==0 && nFrameTo == pParser->m_nAnimationFrames)
		{
			sprintf(fileName, "%s\\%s.ani", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(fileName, "%s\\%s_%d_%d.ani", szMeshOutDir, strMeshName.c_str(), nFrameFrom, nFrameTo);
		}
	}

	GetParentPath(fileName, szTotalPath, MAX_PATH);
	if (_access(szTotalPath, 0) != 0)
	{
		Make_Dirs(szTotalPath);
	}

	vector<DWORD32> vecIdx;
	vector<KVec3> vecVert;

	DWORD32 *lpAnimationVerticesIndex = NULL;
	KVec3   *lpAnimationVerticesPosition = NULL;
	HRESULT hResult = E_FAIL;
	HRESULT hRetCode = E_FAIL;
	KG3DAniFileWrap AniFileWrap;
	VERTEX_ANI_DATA vertexAniData;
	ANI_FILE_HEADER& Header = vertexAniData.fileHeader;
	VERTEX_ANI_HEAD& VertexHead = vertexAniData.vertexAniHead;

	DWORD32 dwNumAnimationVertices = (DWORD32) pKMesh->m_verts.size();
	DWORD32 dwNumVertices = dwNumAnimationVertices;
	DWORD32 dwNumFrames = pKMesh->m_dwPointCacheFrames;
	DWORD32 dwAnimationType = ANIMATION_VERTICES;

	strcpy_s(Header.strDesc, MAX_CLIPDESC_LEN, szName);
	Header.strDesc[MAX_CLIPDESC_LEN - 1] = 0;
	Header.dwMask = ANI_FILE_MASK;

	Header.dwBlockLength = sizeof(ANI_FILE_HEADER) + sizeof(VERTEX_ANI_HEAD) + 
		sizeof(DWORD32) * dwNumAnimationVertices +
		sizeof(KVec3) * dwNumAnimationVertices * dwNumFrames +
		sizeof(DWORD32);          //EndFlag

	//if (m_lpAnimationUV)
	//	Header.dwBlockLength += sizeof(D3DXVECTOR3) * m_dNumAnimationVertices * m_dNumFrames;

	Header.dwNumAnimations = 1;
	Header.dwType = dwAnimationType;

	VertexHead.dwNumVertex = dwNumVertices;
	VertexHead.dwNumAnimatedVertex = dwNumAnimationVertices;
	VertexHead.dwNumFrames = dwNumFrames;
	VertexHead.fFrameLength = nFrameTime;

	for(int i = 0; i < (int) pKMesh->m_verts.size(); ++i)
	{
		vecIdx.push_back(i);
	}
	lpAnimationVerticesIndex = &vecIdx[0];

	for(int i = 0; i < dwNumAnimationVertices; ++i)
	{
		KPCNT3 &point = pKMesh->m_verts[i];
		map<KVec3, KMesh::KPointCache>::iterator it = pKMesh->m_mapPointCache.find(point.pos);
		if(it != pKMesh->m_mapPointCache.end())
		{
			KMesh::KPointCache &pointcache = it->second;
			for(DWORD32 i_Frame = 0; i_Frame < pKMesh->m_dwPointCacheFrames; i_Frame ++)
			{
				KVec3 pos = pointcache.vecPointCache[i_Frame];
				FixPoint(0, &pos, pKMesh);
				vecVert.push_back(pos);
			}
		}
		else
		{
			assert(0);
		}
	}
	lpAnimationVerticesPosition = &vecVert[0];

	vertexAniData.pVertexIndex = lpAnimationVerticesIndex;
	vertexAniData.pAnimatedVertexPosition = (KVec3 *)lpAnimationVerticesPosition;
	
	//if (m_lpAnimationUV)
	//	vertexAniData.pUV = (D3DXVECTOR3 *)m_lpAnimationUV;

	vertexAniData.dwEndFlag = ANI_FILE_END_FLAG;

	hRetCode = AniFileWrap.ConvertAniData((BYTE *)(&vertexAniData));
	KGLOG_COM_PROCESS_ERROR(hRetCode);

	hRetCode = AniFileWrap.SaveToFile(fileName);
	KGLOG_COM_PROCESS_ERROR(hRetCode);

	hResult = S_OK;
Exit0:
	vertexAniData.Clear();
	return ;
}


void SaveMeshVertexAnimationToFile(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress)
{
	if(bCompress)
	{
		SaveMeshVertexAnimationToFile_Compress(pKMesh, szName, pOutDir, pParser, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo);		
	}
	else
	{
		SaveMeshVertexAnimationToFile_NoCompress(pKMesh, szName, pOutDir, pParser, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo);		
	}
}

void SaveCollisionMeshFile(char* szOutMeshName, KMesh* pKMesh, const char *szOutDir, const char *szSourceName)
{
	if (ExportOption()->bCollision)
	{
		SaveMeshFile(szOutMeshName, pKMesh, szOutDir, szSourceName, false, true);
	}
}

bool SaveMeshBaseJsonModelInfo(KParser *pParser, rapidjson::Document& doc, const char* szSourceFbxName, KMesh* pMesh)
{
	vector<string> vecPath;
	rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
	rapidjson::Value strJson;
	strJson.SetString(rapidjson::StringRef(szSourceFbxName));
	doc.AddMember("sourcefile", strJson, allocator);

	if (!pParser)
		return false;

	// lod infomation
	rapidjson::Value jsonArrayLOD(rapidjson::kArrayType);
	if (pParser->GetModelLodPath(pMesh->m_szMeshName, vecPath))
	{
		rapidjson::Value strJsonPath;
		for (vector<string>::iterator it = vecPath.begin(); it != vecPath.end(); ++it)
		{
			strJsonPath.SetString(it->c_str(), it->length(), allocator);
			jsonArrayLOD.PushBack(strJsonPath, allocator);
		}
		doc.AddMember("lod", jsonArrayLOD, allocator);
	}

	// inspack infomation
	vecPath.clear();
	rapidjson::Value jsonArrayJsonInspack(rapidjson::kArrayType);
	if (pParser->GetModelInspackPath(pMesh->m_szMeshName, vecPath))
	{
		rapidjson::Value strJsonPath;
		for (vector<string>::iterator it = vecPath.begin(); it != vecPath.end(); ++it)
		{
			strJsonPath.SetString(it->c_str(), it->length(), allocator);
			jsonArrayJsonInspack.PushBack(strJsonPath, allocator);
		}
		doc.AddMember("jsoninspack", jsonArrayJsonInspack, allocator);
	}

	// collision infomation
	string strCollision = pParser->GetModelCollisionPath(pMesh->m_szMeshName);
	if (!strCollision.empty())
	{
		rapidjson::Value strJsonPath;
		strJsonPath.SetString(strCollision.c_str(), strCollision.length(), allocator);
		doc.AddMember("collision", strJsonPath, allocator);
	}

	return true;
}

bool SaveMeshJsonModel(KParser *pParser, const char* szModelName, const char* szSourceFbxName, KMesh* pMesh)
{
	bool bRet = false;

	FILE *fp = NULL;

	std::string strDiffuse;
	std::string strMre;
	std::string strNor;

	char pszDrive[_MAX_DRIVE] = { 0 };
	char pszDir[_MAX_DIR] = { 0 };
	char pszFname[_MAX_FNAME] = { 0 };
	char pszExt[_MAX_EXT] = { 0 };
	char pszModelExt[] = ".Model";
	char szJsonModelName[_MAX_PATH] = { 0 };

	// json 格式
	/*{
		"sourcefile": "aaa.fbx",
		"collisionmesh": "\\data\\source\\player\\f1\\部件\\f1_0000_head_C.mesh",
		"lod": [
			{
				"meshfile": "\\data\\source\\player\\f1\\部件\\f1_0000_head.mesh",
				"mtlfile": "\\data\\source\\player\\f1\\部件\\f1_0000_head.jsoninspace"
			},
			{
				"meshfile": "\\data\\source\\player\\f1\\部件\\f1_0000_head_lod1.mesh",
				"mtlfile": "\\data\\source\\player\\f1\\部件\\f1_0000_head_lod1.jsoninspace"
			},
		],
		"anchorpoints": [
			{
				"x": 0,
				"y": 1,
				"z": 2,
				"dir": 0,
				"type": 0
			}
		]
	}*/

	int nCurLen = 0;

	if (!szModelName || !szModelName[0])
		return bRet;

	_splitpath_s(szModelName, pszDrive, pszDir, pszFname, pszExt);
	sprintf_s(szJsonModelName, _MAX_PATH, "%s%s%s%s", pszDrive, pszDir, pszFname, pszModelExt);

	fopen_s(&fp, szJsonModelName, "wt+");
	if (fp==0) 
	{ 
		printf("can't open file\n");
		return bRet;
	}
	fseek(fp, 0, SEEK_END);
	long filelen =ftell(fp); 
	rewind(fp);
	
	// use rapidjson to help construct model file.
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
	doc.SetObject();

	// basice information
	SaveMeshBaseJsonModelInfo(pParser, doc, szSourceFbxName, pMesh);

    // anchorpoint
    rapidjson::Value jsonArrayAnchorPoint(rapidjson::kArrayType);

    // anchorpoint detail ...
    for (auto it : pMesh->m_vecAnchorPoints)
    {
        rapidjson::Value anchorPoint(rapidjson::kObjectType);
        anchorPoint.AddMember("name", rapidjson::StringRef(it->strName.c_str()), allocator);

        rapidjson::Value anchorPointTrans(rapidjson::kObjectType);
        anchorPointTrans.AddMember(rapidjson::Value("x"), rapidjson::Value(it->f3Point.x), allocator);
        anchorPointTrans.AddMember(rapidjson::Value("y"), rapidjson::Value(it->f3Point.y), allocator);
        anchorPointTrans.AddMember(rapidjson::Value("z"), rapidjson::Value(it->f3Point.z), allocator);
        anchorPoint.AddMember("translation", anchorPointTrans, allocator);

        rapidjson::Value anchorPointRotaion(rapidjson::kObjectType);

		KVec3 pyr;
		QuaternionToEulerAngles(it->f4Rotation, pyr);
        anchorPointRotaion.AddMember(rapidjson::Value("pitch"), rapidjson::Value(pyr.x), allocator);
        anchorPointRotaion.AddMember(rapidjson::Value("yaw"), rapidjson::Value(pyr.y), allocator);
        anchorPointRotaion.AddMember(rapidjson::Value("roll"), rapidjson::Value(pyr.z), allocator);
        anchorPoint.AddMember("rotation", anchorPointRotaion, allocator);

        jsonArrayAnchorPoint.PushBack(anchorPoint, allocator);
    }
    doc.AddMember("anchorpoint", jsonArrayAnchorPoint, allocator);
    // spline
    rapidjson::Value jsonArraySpline(rapidjson::kArrayType);
    // spline detail ...
    for (auto it : pMesh->m_vecSplines)
    {
        rapidjson::Value spline(rapidjson::kObjectType);
        spline.AddMember("name", rapidjson::StringRef(it->strName.c_str()), allocator);
        rapidjson::Value splineSublines(rapidjson::kArrayType);
        int nIndex = 0;
        for (auto itEndIndex : it->vecEndPointsIndex)
        {
            rapidjson::Value splineSubline(rapidjson::kArrayType);
            for (; nIndex <= itEndIndex; ++nIndex)
            {
                rapidjson::Value splinePoint(rapidjson::kObjectType);
                KVec3 f3Point = it->vecPoints[nIndex];
                splinePoint.AddMember(rapidjson::Value("x"), rapidjson::Value(f3Point.x), allocator);
                splinePoint.AddMember(rapidjson::Value("y"), rapidjson::Value(f3Point.y), allocator);
                splinePoint.AddMember(rapidjson::Value("z"), rapidjson::Value(f3Point.z), allocator);
                splineSubline.PushBack(splinePoint, allocator);
            }
            splineSublines.PushBack(splineSubline, allocator);
        }
        spline.AddMember("lines", splineSublines, allocator);

        rapidjson::Value splineTrans(rapidjson::kObjectType);
        splineTrans.AddMember(rapidjson::Value("x"), rapidjson::Value(it->f3Translation.x), allocator);
        splineTrans.AddMember(rapidjson::Value("y"), rapidjson::Value(it->f3Translation.y), allocator);
        splineTrans.AddMember(rapidjson::Value("z"), rapidjson::Value(it->f3Translation.z), allocator);

        rapidjson::Value splineScaling(rapidjson::kObjectType);
        splineScaling.AddMember(rapidjson::Value("x"),rapidjson::Value(it->f3Scaling.x), allocator);
        splineScaling.AddMember(rapidjson::Value("y"),rapidjson::Value(it->f3Scaling.y), allocator);
        splineScaling.AddMember(rapidjson::Value("z"),rapidjson::Value(it->f3Scaling.z), allocator);

        rapidjson::Value splineRotaion(rapidjson::kObjectType);

		KVec3 pyr;
		QuaternionToEulerAngles(it->f4Rotation, pyr);
        splineRotaion.AddMember(rapidjson::Value("pitch"),rapidjson::Value(pyr.x), allocator);
        splineRotaion.AddMember(rapidjson::Value("yaw"),rapidjson::Value(pyr.y), allocator);
        splineRotaion.AddMember(rapidjson::Value("roll"),rapidjson::Value(pyr.z), allocator);

        spline.AddMember("translation", splineTrans, allocator);
        spline.AddMember("scaling", splineScaling, allocator);
        spline.AddMember("rotation", splineRotaion, allocator);
        jsonArraySpline.PushBack(spline, allocator);
    }
    doc.AddMember("spline", jsonArraySpline, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	fwrite(buffer.GetString(), buffer.GetSize(), 1, fp);
	fclose(fp);


	bRet = true;
	return bRet;
}

void SaveJsonModelFile(KParser *pParser, KMesh *pKMesh, const char *szOutDir, const char *sourceName, BOOL bMdl)
{ 
    if (!ExportOption()->bModelFileExport)
        return;

	char szFileName[MAX_PATH];
	char szFbxFileName[MAX_PATH];

	BOOL bHD = ExportOption()->bHD;
	char szFbxExt[MAX_PATH] = ".fbx";
	char ext[MAX_PATH] = { 0 };

	if (bHD) strcpy_s(ext, "_HD.mesh");
	else     strcpy_s(ext, ".mesh");

	int len = (int) strlen(szOutDir);

	sprintf(szFbxFileName, "%s%s", sourceName, szFbxExt);
	if (!bMdl && !ExportOption()->bGroupExport)
	{
		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
			sprintf(szFileName, "%s%s%s", szOutDir, sourceName, ext);
		else
			sprintf(szFileName, "%s\\%s%s", szOutDir, sourceName, ext);

		if (_access(szOutDir, 0) != 0)
		{
			Make_Dirs(szOutDir);
		}
	}
	else
	{
		char szTotalPath[MAX_PATH];
		char szMeshOutDir[MAX_PATH];

		char *p = strstri(pKMesh->m_szMeshName.c_str(), "#");
		if (p)
		{
			GetMapsSourcePathFromPath(szOutDir, szMeshOutDir, MAX_PATH);
		}
		else
		{
			strcpy(szMeshOutDir, szOutDir);
		}

		string strMeshName = pKMesh->m_szMeshName;
		ReplaceAll(strMeshName, "#", "\\");

		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
		{
			sprintf(szFileName, "%s%s%s", szMeshOutDir, strMeshName.c_str(), ext);
		}
		else
		{
			sprintf(szFileName, "%s\\%s%s", szMeshOutDir, strMeshName.c_str(), ext);
		}

		GetParentPath(szFileName, szTotalPath, MAX_PATH);
		if (_access(szTotalPath, 0) != 0)
		{
			Make_Dirs(szTotalPath);
		}
	}

	KG_PROCESS_ERROR(SaveMeshJsonModel(pParser, szFileName, szFbxFileName, pKMesh));

Exit0:
	return;
}
