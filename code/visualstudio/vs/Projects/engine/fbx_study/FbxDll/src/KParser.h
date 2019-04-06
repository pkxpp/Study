/********************************************************************
	created:	2015/09/07   11:36
	filename: 	KParser.h		
	author:		LiKan
	
	purpose:	
*********************************************************************/
#ifndef KPARSER_H
#define KPARSER_H
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fbxsdk.h>
//#include <decomp.h>
//#include <max.h>
#include "DllInterface.h"
using namespace std;

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a)	if (a) {delete[] (a); (a)=NULL;}
#endif

#define SUFFIX_COLLISION "_C"

typedef unsigned int DWORD32;


struct KMatrix { 
	float _11; float _12; float _13; float _14;
	float _21; float _22; float _23; float _24;
	float _31; float _32; float _33; float _34;
	float _41; float _42; float _43; float _44;
	KMatrix()
	{
		memset(this, 0, sizeof(KMatrix));
	}
};

struct KVec2
{
	float x;
	float y;
	KVec2():x(0), y(0){}
	KVec2(float _x, float _y):x(_x), y(_y){}
};


struct KVec3
{
	float x;
	float y;
	float z;
	KVec3():x(0), y(0), z(0){}	
	KVec3(float _x, float _y, float _z):x(_x), y(_y), z(_z){}
};

struct KVec4
{
	float x;
	float y;
	float z;
	float w;
	KVec4():x(0), y(0), z(0){}
	KVec4(float _x, float _y, float _z, float _w):x(_x), y(_y), z(_z), w(_w){}
};


struct KAABBbox
{
	KVec3 vMax;
	KVec3 vMin;
	int nInit;

	KAABBbox()
	{
		Clear();
	}
	void Clear()
	{
		vMax.x = 0; vMax.y = 0; vMax.z = 0;
		vMin.x = 0; vMin.y = 0; vMin.z = 0;
		nInit = false;
	}
	void AddPositon(float x, float y, float z)
	{
		if(!nInit)
		{
			vMax.x = x;
			vMax.y = y;
			vMax.z = z;

			vMin.x = x;
			vMin.y = y;
			vMin.z = z;
			nInit = true;
		}
		else
		{
			if (vMin.x > x) vMin.x = x;
			if (vMin.y > y) vMin.y = y;
			if (vMin.z > z) vMin.z = z;

			if (vMax.x < x) vMax.x = x;
			if (vMax.y < y) vMax.y = y;
			if (vMax.z < z) vMax.z = z;
		}
	}	
};

typedef KVec4 KQuat;

struct KPCNT3
{
	KVec3 pos;
	DWORD32 color;
	KVec3 nomral;	
	KVec3 uvw0;
	KVec3 uvw1;
	KVec3 uvw2;
	KVec4 tangent;
//     int blendShapeCount;
//     KVec3* vBlendShapePos;
//     KVec3* vBlendShapeNormal;
//     KVec3* vBlendShapeTangent;
	KPCNT3()
	{
		memset(this, 0, sizeof(KPCNT3));
	}	
//     KPCNT3(const KPCNT3& pcnt3)
//     {
//         pos = pcnt3.pos;
//         color = pcnt3.color;
//         nomral = pcnt3.nomral;
//         uvw0 = pcnt3.uvw0;
//         uvw1 = pcnt3.uvw1;
//         uvw2 = pcnt3.uvw2;
//         tangent = pcnt3.tangent;
//         blendShapeCount = pcnt3.blendShapeCount;
//         if (blendShapeCount > 0)
//         {
//             SAFE_DELETE_ARRAY(vBlendShapePos);
//             vBlendShapePos = new KVec3[blendShapeCount];
//             memcpy(vBlendShapePos, pcnt3.vBlendShapePos, sizeof(KVec3) * blendShapeCount);
//             SAFE_DELETE_ARRAY(vBlendShapeNormal);
//             vBlendShapeNormal = new KVec3[blendShapeCount];
//             memcpy(vBlendShapeNormal, pcnt3.vBlendShapeNormal, sizeof(KVec3) * blendShapeCount);
//             SAFE_DELETE_ARRAY(vBlendShapeTangent);
//             vBlendShapeTangent = new KVec3[blendShapeCount];
//             memcpy(vBlendShapeTangent, pcnt3.vBlendShapeTangent, sizeof(KVec3) * blendShapeCount);
//         }
//         else
//         {
//             vBlendShapePos = vBlendShapeNormal = vBlendShapeTangent = NULL;
//         }
//     }
//     ~KPCNT3()
//     {
//         SAFE_DELETE_ARRAY(vBlendShapePos);
//         SAFE_DELETE_ARRAY(vBlendShapeNormal);
//         SAFE_DELETE_ARRAY(vBlendShapeTangent);
//     }
};

// page@2018/08/23
//struct RTSBoneAnimation
//{
//	Point3 Translation;
//	Point3 Scale;
//	Quat Rotation;
//	float Sign;
//	Quat SRotation;
//	RTSBoneAnimation()
//	{
//		memset(this, 0, sizeof(RTSBoneAnimation));
//	}
//};
//
//void Matrix3ToMatrix4(KMatrix *pMat4, Matrix3* pMat3);
//
//void ToRTS(RTSBoneAnimation &rts, AffineParts &ap);
//
//void ToMaxMat3(Matrix3 &matOut, FbxAMatrix &mat);
// end---

struct KWeight
{
	int boneids[4];
	float weights[4];
	float weights_orign[4];

	KWeight()
	{		
		for(int i = 0; i < 4; ++i)
		{
			boneids[i] = 0;
			weights[i] = 0;
			weights_orign[i] = 0;
		}
	}
	BOOL AddWeight(int nBoneId, float fWeight)
	{
		BOOL bFind = false;
		int minId = 0;
		float fminWeight = 999.0;
		for(int i = 0; i < 4; ++i)
		{			
			if(boneids[i] == nBoneId && weights_orign[i] != 0)
			{				
				weights[i] = fWeight;
				weights_orign[i] = fWeight;
				//已经添加过这个骨骼的权重值了，出现这种情况的原因是可能有多个相同的控制点，这里得去重
				return TRUE;
			}
			
			if(boneids[i] == 0 && weights_orign[i] == 0)
			{
				bFind = true;
				boneids[i] = nBoneId;
				weights[i] = fWeight;
				weights_orign[i] = fWeight;
				break;
			}

			//找最小的权重下标
			if(fminWeight > weights_orign[i])
			{
				fminWeight = weights_orign[i];
				minId = i;
			}
		}

		//if(!bFind && fWeight > fminWeight)
		//{
		//	//这肯定是权重超过4了，舍弃最小的权重，进行替换
		//	boneids[minId] = nBoneId;
		//	weights[minId] = fWeight;
		//	weights_orign[minId] = fWeight;
		//}
		
		//控制点去重也会带来权重二义性，这里需要融合处理一下
		float all = weights_orign[0] + weights_orign[1] + weights_orign[2] + weights_orign[3];
		if(all > 1.0)
		{
			weights[0] = weights_orign[0] / all;
			weights[1] = weights_orign[1] / all;
			weights[2] = weights_orign[2] / all;
			weights[3] = weights_orign[3] / all;
		}


		//assert(bFind);
		return bFind;		
	}
};

struct KSocket
{
	string   szName;
	string   szParentName;
	KMatrix  MatrixOffset;
};

//void ToMaxMat3(Matrix3 &matOut, FbxAMatrix &mat);
bool operator == (const KVec2 &v0, const KVec2 &v1);
bool operator == (const KVec3 &v0, const KVec3 &v1);
bool operator == (const KVec4 &v0, const KVec4 &v1);
bool operator == (const KPCNT3 &v0, const KPCNT3 &v1);

bool operator > (const KVec2 &v0, const KVec2 &v1);
bool operator > (const KVec3 &v0, const KVec3 &v1);
bool operator > (const KVec4 &v0, const KVec4 &v1);
bool operator > (const KPCNT3 &v0, const KPCNT3 &v1);

bool operator < (const KVec2 &v0, const KVec2 &v1);
bool operator < (const KVec3 &v0, const KVec3 &v1);
bool operator < (const KVec4 &v0, const KVec4 &v1);
bool operator < (const KPCNT3 &v0, const KPCNT3 &v1);



void GetNameFromPath(const char* inPath, char *outPath, int outLen);
BOOL GetParentPathName(const char* inPath, char *outPath, int outLen);
void GetParentPath(const char* inPath, char *outPath, int outLen);
void GetFileNameFormPath(const char* inPath, char *outPath, int outLen);

void GetRelativePathFromFileName(const char *szFileName, char *szRelativePath, int outLen);
void GetMapsSourcePathFromPath(const char* inPath, char *outPath, int outLen);
void RemoveRelativePrefix(const char *szFileName, char *szOutName, int outLen);

int ConvertGBKToUtf8(const char * pcszStrGBK, char *pOut);
int ConvertUTF8ToGBK(const char * pcszStrUTF8, char *pOut); 

int Make_Dirs(const char *dir);

void ReplaceAll(string& str,char* oldValue,char* newValue);

class KParser;
class KMesh;

//mesh部件的骨骼，包含顶点weight信息
struct KMeshBone
{
	
	//int nVertId[4];
	//float fWeights[4];


	//对控制顶点的权重数据
	vector<KVec3> m_vecControllPoints;
	vector<float> m_vecControllPointsWeight;
	char szBoneName[30];

	//对展开以后的顶点影响的权重数据
	vector<DWORD32> m_vecIndexRef;
	vector<float> m_vecWeightRef;
	

	
	string m_szParentName;
	vector<string> m_vecChildNames;
		

	//for export
	KMatrix MatrixOffset;
	
	//Matrix3 time0Matrix;
	//Matrix3 initPoseMatrixInverse;

	FbxNode *pFbxNode;

	BOOL bIsRoot;

	FbxCluster* m_pCluster;
	

	KMeshBone()
	{	
		m_pCluster = NULL;
		bIsRoot = FALSE;		
		pFbxNode = NULL;				
	}

	BOOL ComputBoneWeight(KMesh *pKMesh, int nBoneId);

	void Reset()
	{
		m_vecControllPoints.clear();
		m_vecControllPointsWeight.clear();

		m_szParentName = "";
		m_vecIndexRef.clear();
		m_vecWeightRef.clear();
		pFbxNode = NULL;
		szBoneName[0] = '\0';		
	}
};

//整个bip骨架的骨骼，不针对特定mesh，可能是多个mesh共用的，所以不包含weight信息，最后骨骼动画导出使用这个结构
struct KBipBone
{
	char szBoneName[30];
	//如果是flexBone是不生成骨架文件的	
	FbxNode *pFbxNode;
	KBipBone *pParentBone;


	vector<string> m_vecChildNames;

	KBipBone()
	{
		pFbxNode = NULL;
		pParentBone = NULL;		
		szBoneName[0] = '\0';
	}
};


//本来材质可能不止一张贴图，但引擎有自己的材质系统，基本不需要这里的数据，所以这里归集的信息从简
struct KMaterail
{
	string textureName;
	string mreTextureName;
	string norTextureName;
	string usage;	
};


struct KBlendShape;
class KMesh
{
public:	
	string m_szMeshName;	
	vector<KMeshBone*> m_vecBones;
	set<string> m_weightBoneNames;
	set<string> m_socketNames;
	int m_nLOD;

	KAABBbox m_AabbBox;
	
	map<string, FbxNode *> m_mapRoots;	

	vector<KVec3> m_vecControlPoints;

	map<KVec3, KWeight*> m_mapWeightControlPointMapping;
	
	vector<KPCNT3> m_verts;

    vector<KBlendShape*> m_blendShapes;
	
	vector<KWeight> m_weights;

	DWORD32 m_numFaces;
	vector<DWORD32> m_inds;
	vector<DWORD32> m_vecFaceSubsetId;

	FbxMesh *m_pFbxMesh;
	FbxNode *m_pFbxNode;
	int m_nUvElementCount;
	int m_nColorElementCount;
    int m_nBlendShapeElementCount;

	set<int> m_SubsetIds;
	map<int, KMaterail> m_Materials;

	vector<KSocket *> m_vecSocket;

	struct KPointCache
	{
		int id;
		vector<KVec3> vecPointCache;
	};
	
	map<KVec3, KPointCache> m_mapPointCache;

    DWORD32 m_dwPointCacheFrames;

    struct KSpline
    {
        string strName;
        vector<KVec3> vecPoints;
        vector<int> vecEndPointsIndex;
        KVec3 f3Translation;
        KVec3 f3Scaling;
        KVec4 f4Rotation;
    };
    vector<KSpline*> m_vecSplines;

    struct KAnchorPoint
    {
        string strName;
        KVec3 f3Point;
        KVec4 f4Rotation;
    };
    vector<KAnchorPoint*> m_vecAnchorPoints;

	KMesh();
	~KMesh();

	void _BuildBoneLinkName(KMeshBone *pMeshBone);
	void _BuildBoneMatrixOffset(KMeshBone *pMeshBone);

	void BuildBoneLinkNames();
	void BuildBoneMatrixOffset();
	void BuildSocketList();	
	BOOL IsMeshBone(FbxNode *pNode);
	void AddMeshBone(FbxCluster* m_pCluster, FbxNode *pRootNode, KParser *pParser);

	void BuildRootBoneByWeightBone(FbxNode *pNode, FbxNode *pRootNode, KParser *pParser);	
	void VistBoneTree(FbxNode *fbxNode, KParser *pParser);
	void BuildBoneList(KParser *pParser);	
	void BuildMeshBoneWeight(KParser *pParser);
	void FixLinks(KParser *pParser);
	KMeshBone *GetBone(const char *pBoneName);
	void FixSocketBone(FbxNode *pNode, FbxNode *pRootNode);
	KSocket *GetSocket(const char *pSocketName);
    void ParseBlendShapes(FbxMesh* pMesh);
    void ParseSplines();
    void ParseAnchorPoints();
    void ComputeBlendShapeTangent(FbxMesh* pMesh);

public:
	vector<int> m_vertsMergeMatTable;
	vector<FbxAMatrix> m_vertsMergeMat;

	void GetVertMat(int n, FbxAMatrix &mat);
};

struct KBlendShapeTarget
{
    string szName;
    float fHightWeight;
    vector<KPCNT3> m_vControlPoints;
};

struct KBlendShapeChannel
{
    string szName;
    vector<KBlendShapeTarget*> m_vBlendShapeTargets;
};

struct KBlendShape
{
    string szName;
    vector<KBlendShapeChannel*> m_vBlendShapeChannels;
};

struct KPartSocketConfig
{
	string szPartName;
	vector<string> vecSocketName;
};


struct KSkeletonTextBone
{
	string szBoneName;
	set<string> m_setChildren;
};

struct KSkeletonFileConfig
{
	string szFileName;
	vector<KSkeletonTextBone> SkeletonList;
};


// Model
class KModel{
public:
	KModel();
	bool AddLodPath(std::string strPath);
	bool AddInspackPath(std::string strPath);
	bool SetCollisionPath(std::string strPath);
	bool GetLodPath(vector<string>& vecPath);
	bool GetInspackPath(vector<string>& vecPath);
	string GetCollisionPath() {return m_strCollisionPath;};
	void ClearCache();
private:
	KMesh* m_pRootMesh;			// lod0 mesh
	int m_nLod;
	vector<std::string> m_vecLodPath;
	vector<std::string> m_vecInspackPath;
	std::string m_strCollisionPath;
};

// Model end


class KParser
{
private:	

	string m_szName;
	string m_szMdlName;

	char  m_szTgaOutPath[FILENAME_MAX];

	FbxScene *m_pScene;
	BOOL m_bMdlParser;
    BOOL m_bParseBlendShape;
		
	vector<KMesh *> m_vecMesh;
	vector<KBipBone *> m_vecBipBone;
	vector<KBipBone *> m_vecIgnoreBipBone;
	map<string, int> m_mapRootBone;
	vector<KMesh *> m_vecCollisionMesh;
	typedef map<int, vector<KMesh*>> MAP_LOD_MESH;
	MAP_LOD_MESH m_mapLodMesh;

	vector<FbxNode *> m_vecSoketFbxNode;

	KSkeletonFileConfig *m_pMartchSkeleton;
	
	vector<string> m_vecErrorBoneWeightNames;

	typedef map<string, KModel*> MAP_MODEL_SET;			// <pre name, KModel*>
	MAP_MODEL_SET m_mapModelSet;					
	//config
private:
	vector<string> m_vecPartsFilter;
	vector<string> m_vecBlackListFilter;
	vector<KPartSocketConfig> m_vecSocketPart;	
	vector<KSkeletonFileConfig *> m_vecSkeletonFile;
	
	void ParseVertexAnimtion(FbxNode* pNode, KMesh *pKMesh, FbxNode *pRootNode);
	void ParseLink(FbxNode* pNode, KMesh *pKMesh, FbxNode *pRootNode);
	void ParseMesh(FbxNode* pNode, FbxNode* pRootNode);
	void ParseHierarchy(FbxNode* pParentNode, FbxNode* pNode, int pDepth);
	void ParseAnimationLayer(FbxAnimLayer* pAnimLayer, FbxNode* pNode);
	void CounterFrames(FbxAnimCurve* pAnimCurve);		
	
	void ParseMeshAndBoneWeights(FbxNode* pRootNode);
	void ParseSkeleton(FbxNode* pRootNode);
	void ParseAnimation(FbxScene* pScene);
	void SaveBipAnimationToFile_Compress(const char *szName, const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo);
	void SaveBipAnimationToFile_NoCompress(const char *szName, const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo);
	void SaveBipAnimationToFile(const char *szName, const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress);
	void SaveMesh(const char *szOutDir, vector<KMesh*>& vecMesh);
	void MergeMesh(KMesh& meshMerge, const std::vector<KMesh*>& vecMesh);
	void MergeSaveAll(const char *szOutDir);
	void MergeSave(KMesh& mesh_merge, const char *szOutDir, const vector<KMesh*>& vecMesh, int nLOD = 0);
	void SaveMeshVertexAnimation(const char *pOutDir, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress);
	void SaveMaterail(const char *szOutDir, std::vector<KMesh*>& vecMesh);
	void SaveJsoninspack(const char *szOutDir, vector<KMesh*>& vecMesh);
	void SaveMdlFile(const char *szOutDir);
	void SaveBipFile(const char *szOutDir);
	void SaveCollision(const char *szOutDir);
	void SaveJsonModel(const char *szOutDir);
	
	FbxManager* m_pSdkManager;
	KBipBone *GetBone(const char *szBoneName);
	void FixBipLinks();
	
	void FixSocketBones(FbxNode *pRootNode);
	KMesh *GetPartMesh(const char *pPartName);
	KSkeletonFileConfig* DoSkeletonMartch();
    void ValidateParseBlendShape(FbxNode* pRootNode);
	int IsCollisionNode(FbxNode* pNode);

	string GetMeshPureName(int* pLod, const char* pszName, bool bCollision = false);
	void ConstructModel(string strPureName, int nLOD);
	void AddModelInfoLODPath(string strPureName, std::string strMeshName, bool bCollision = false);
	void AddModleInfoJsonInspackPath(std::string strName, string strInspackPath);
public:
	int m_nAnimationFrames;
	int m_nFrameTimeFrameMilSecond;	
	int m_nSampleFrameInterval;	
	map<string, string> m_mapRelink;
	string errorMsg;
	string warnMsg;
	void Parse(FbxScene* pScene, const char *szName, const char *szParentPathName, FbxManager* pSdkManager, BOOL bMdlParser);	
	void Save(const char *szOutPath, const char* pszTgaOutPath, BOOL bOnlyAnimationChecked, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress, float fbxScale, BOOL bChangeYZ);
	void AddBipRootBone(const char *pcszBoneName);
	BOOL IsMarchBlackName(const char *pName);
	BOOL IsMdlParser();
	void Clear();
	BOOL IsBipNameIgnore(FbxNode *pNode);
	const char *GetSourceName();
    BOOL IsParseBlendShape() {return m_bParseBlendShape;}
	KParser();
	~KParser();

	void AddErrorWeightBoneName(const char *pName);

	bool GetModelLodPath(string strName, vector<string>& vecPath);
	bool GetModelInspackPath(string strName, vector<string>& vecPath);
	string GetModelCollisionPath(string strName);

	//////////////////////////////////////////////////////////////////////////
	// test
	void TestProcessMesh(FbxNode* pNode);
	void TestReadVertex(FbxMesh* pMesh , int ctrlPointIndex , KVec3* pVertex);
	void TestReadColor(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , KVec4* pColor);
	void TestReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , KVec2* pUV);
	void TestReadNormal(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , KVec3* pNormal);
	void TestReadTangent(FbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , KVec3* pTangent);
};



//extern const Matrix3& g_Getmat3Inch2CMScale();
//void g_ChangeMatrixYZ(Matrix3* mat);









#endif