/********************************************************************
	created:	2015/09/07   11:32
	filename: 	KMesh.h		
	author:		LiKan
	
	purpose:	
*********************************************************************/
#ifndef KMESH_H
#define KMESH_H
typedef int BOOL;
class KMesh;
struct KVec3;

class KParser;
void ParseControlsPoints(KMesh* pMesh);
void ParseFaces(KMesh* pKMesh, KParser *pParser);
void SaveMeshFile(char* szOutMeshName, KMesh *pKMesh, const char *szOutDir, const char *sourcName, BOOL bMdl, BOOL bCollision = false);
void SaveJsoninspackFile(char* szOutName, KMesh *pKMesh, const char *szOutDir, const char *szTgaOutDir, const char *sourceName, BOOL bMdl);
void SaveMeshBlendShapeFile(KMesh *pKMesh, const char *szOutDir, const char *sourceName, BOOL bMdl);
void FixPoint(int n, KVec3 *point, KMesh *pKMesh, bool bCalAABB = true);
void SaveMeshAnimationToFile(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser,  int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress);
void SaveMeshVertexAnimationToFile(KMesh *pKMesh, const char *szName, const char *pOutDir, KParser *pParser, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress);
void SaveCollisionMeshFile(char* szOutMeshName, KMesh* pKMesh, const char *szOutDir, const char *szSourceName);
void SaveJsonModelFile(KParser *pParser, KMesh *pKMesh, const char *szOutDir, const char *sourceName, BOOL bMdl);
#endif