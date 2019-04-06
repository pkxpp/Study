/********************************************************************
	created:	2015/09/07   11:32
	filename: 	KMaterail.cpp		
	author:		LiKan
	
	purpose:	
*********************************************************************/
//#include "KMaterail.h"
#include <stdio.h>
#include <algorithm>
#include <io.h>
#include <assert.h>
#include <direct.h>
#include <fbxsdk.h>
//#include <decomp.h>
//#include "DisplayCommon.h"
#include "KParser.h"
//#include "KMaterail.h"
//#include "pub.h"

#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

void RemapSubsetID(KMesh *pKMesh)
{
	std::set<DWORD32> setSubset;
	for(DWORD32 i = 0; i < pKMesh->m_numFaces; ++i)
	{
		setSubset.insert(pKMesh->m_vecFaceSubsetId[i]);
	}	
	vector<DWORD32> FixID;
	auto itSubset = setSubset.begin();
	while (itSubset != setSubset.end())
	{
		//if (*itSubset >= setSubset.size()) MessageBoxA(NULL, "subset id果然有不连续的情况", "err", 0);

		FixID.push_back(*itSubset);
		itSubset++;
	}
	sort(FixID.begin(), FixID.end());
	map<DWORD32, DWORD32> IDRemap;
	for (size_t i = 0; i < FixID.size(); i++)
	{
		IDRemap[FixID[i]] = static_cast<DWORD32>(i);
	}
	for(DWORD32 i = 0; i < pKMesh->m_numFaces; ++i)
	{
		DWORD32 dwOrgID = pKMesh->m_vecFaceSubsetId[i];
		pKMesh->m_vecFaceSubsetId[i] = IDRemap[dwOrgID];
	}
}

void ParseSubsetMapping(KMesh *pKMesh)
{	
	pKMesh->m_vecFaceSubsetId.clear();
	FbxMesh *pMesh = pKMesh->m_pFbxMesh;
	int lMtrlCount = 0;
	FbxNode* lNode = NULL;
	if(pMesh){
		lNode = pMesh->GetNode();
		if(lNode)
			lMtrlCount = lNode->GetMaterialCount();    
	}
	int nMatCount = pMesh->GetElementMaterialCount();
	pKMesh->m_SubsetIds.clear();
	for (int l = 0; l < nMatCount; l++)
	{
		FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial(l);
		if (leMat)
		{
			int lMaterialCount = 0;			
			if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lMaterialCount = lMtrlCount;
			}
			if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int i;				
				int lIndexArrayCount = leMat->GetIndexArray().GetCount(); 
				for (i = 0; i < lIndexArrayCount; i++)
				{
					//lString += leMat->GetIndexArray().GetAt(i);
					int subsetId = leMat->GetIndexArray().GetAt(i);
					pKMesh->m_vecFaceSubsetId.push_back(subsetId);
					pKMesh->m_SubsetIds.insert(subsetId);
				}				
			}
		}
	}


	if(pKMesh->m_SubsetIds.empty())
	{
		pKMesh->m_SubsetIds.insert(0);
	}
	
	//only one subset
	if(pKMesh->m_SubsetIds.size() == 1)
	{
		pKMesh->m_vecFaceSubsetId.resize(pKMesh->m_numFaces);
		//所有的subset id 都设置为1
		for(DWORD32 i = 0; i < pKMesh->m_numFaces; ++i)
		{
			pKMesh->m_vecFaceSubsetId[i] = 0;
		}		
	}

	// remap subset id : 把subsetid重新排列成0-based，递增的
	RemapSubsetID(pKMesh);

	if(pKMesh->m_vecFaceSubsetId.size() != pKMesh->m_numFaces)
	{
		printf("error: there's some faces have no subset material id, maybe something wrong");
		assert(0);
	}

}

//这个过程并不重要，引擎不会直接去使用max材质的参数信息
void ParseMaterails(KMesh *pKMesh)
{
	/*
	int lMaterialCount = 0;
	FbxNode* lNode = NULL;
	FbxGeometry* pGeometry = pKMesh->m_pFbxMesh;
	if(pGeometry){
		lNode = pGeometry->GetNode();
		if(lNode)
			lMaterialCount = lNode->GetMaterialCount();    
	}

	if (lMaterialCount > 0)
	{
		FbxPropertyT<FbxDouble3> lKFbxDouble3;
		FbxPropertyT<FbxDouble> lKFbxDouble1;
		FbxColor theColor;

		for (int lCount = 0; lCount < lMaterialCount; lCount ++)
		{
			DisplayInt("        Material ", lCount);

			FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(lCount);

			DisplayString("            Name: \"", (char *) lMaterial->GetName(), "\""); 

			//Get the implementation to see if it's a hardware shader.
			const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
			FbxString lImplemenationType = "HLSL";
			if(!lImplementation)
			{
				lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
				lImplemenationType = "CGFX";
			}
			if(lImplementation)
			{
				//shader材质
				assert(0);
				printf("error shader material  does not support yet! \r\n");
			}
			else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				// We found a Phong material.  Display its properties.
				
				// Display the Ambient Color
				
				lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Ambient;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				DisplayColor("            Ambient: ", theColor);

				// Display the Diffuse Color
				lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Diffuse;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				DisplayColor("            Diffuse: ", theColor);

				// Display the Specular Color (unique to Phong materials)
				lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Specular;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				DisplayColor("            Specular: ", theColor);

				// Display the Emissive Color
				lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Emissive;

				

				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				DisplayColor("            Emissive: ", theColor);

				//Opacity is Transparency factor now
				lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->TransparencyFactor;
				DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());

				// Display the Shininess
				lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->Shininess;
				DisplayDouble("            Shininess: ", lKFbxDouble1.Get());

				// Display the Reflectivity
				lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->ReflectionFactor;
				DisplayDouble("            Reflectivity: ", lKFbxDouble1.Get());
			}
			else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
			{
				// We found a Lambert material. Display its properties.
				// Display the Ambient Color
				lKFbxDouble3=((FbxSurfaceLambert *)lMaterial)->Ambient;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				DisplayColor("            Ambient: ", theColor);

				// Display the Diffuse Color
				lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Diffuse;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				DisplayColor("            Diffuse: ", theColor);

				// Display the Emissive
				lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Emissive;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				DisplayColor("            Emissive: ", theColor);

				// Display the Opacity
				lKFbxDouble1 =((FbxSurfaceLambert *)lMaterial)->TransparencyFactor;
				DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());
			}
			else
			{
				printf("error: Unknown type of Material \r\n");
			}

			//FbxPropertyT<FbxString> lString;
			//lString = lMaterial->ShadingModel;
			//DisplayString("            Shading Model: ", lString.Get().Buffer());
			//DisplayString("");
		}
	}
	*/
}



void _DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode, KMesh *pKMesh, int nMaterialIndex, FbxString textureUsage)
{
	FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pTexture);
	FbxProceduralTexture *lProceduralTexture = FbxCast<FbxProceduralTexture>(pTexture);
	
	//DisplayString("            Name: \"", (char *) pTexture->GetName(), "\"");
	if (lFileTexture)
	{
		//DisplayString("            Type: File Texture");
		//DisplayString("            File Name: \"", (char *) lFileTexture->GetFileName(), "\"");
		KMaterail mtl;
		char szTextureDirve[MAX_PATH] = {0};
		char szTextureDir[MAX_PATH] = {0};
		char szTextureName[MAX_PATH] = {0};
		char szTextureExt[MAX_PATH] = {0};
		char szMreTexturePath[MAX_PATH] = {0};
		char szNorTexturePath[MAX_PATH] = {0};

#ifdef WIN32
		char buf[MAX_PATH];		
		ConvertUTF8ToGBK(lFileTexture->GetFileName(), buf);
		string strBuf = buf;
		ReplaceAll(strBuf, "/", "\\");
		mtl.textureName = strBuf.c_str();
#else
		string strBuf = lFileTexture->GetFileName();
		ReplaceAll(strBuf, "/", "\\");
		mtl.textureName = strBuf.c_str();
#endif

		
		_splitpath(mtl.textureName.c_str(), szTextureDirve, szTextureDir, szTextureName, szTextureExt);
		sprintf(szMreTexturePath, "%s%s%s_mre%s", szTextureDirve, szTextureDir, szTextureName, szTextureExt);
		mtl.mreTextureName = szMreTexturePath;

		sprintf(szNorTexturePath, "%s%s%s_nor%s", szTextureDirve, szTextureDir, szTextureName, szTextureExt);
		mtl.norTextureName = szNorTexturePath;

		mtl.usage = textureUsage.Buffer();
		pKMesh->m_Materials.insert(pair<int, KMaterail>(nMaterialIndex, mtl));
	}
	else if (lProceduralTexture)
	{
		//DisplayString("            Type: Procedural Texture");
	}
	//DisplayDouble("            Scale U: ", pTexture->GetScaleU());
	//DisplayDouble("            Scale V: ", pTexture->GetScaleV());
	//DisplayDouble("            Translation U: ", pTexture->GetTranslationU());
	//DisplayDouble("            Translation V: ", pTexture->GetTranslationV());
	//DisplayBool("            Swap UV: ", pTexture->GetSwapUV());
	//DisplayDouble("            Rotation U: ", pTexture->GetRotationU());
	//DisplayDouble("            Rotation V: ", pTexture->GetRotationV());
	//DisplayDouble("            Rotation W: ", pTexture->GetRotationW());

	//const char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

	//DisplayString("            Alpha Source: ", lAlphaSources[pTexture->GetAlphaSource()]);
	//DisplayDouble("            Cropping Left: ", pTexture->GetCroppingLeft());
	//DisplayDouble("            Cropping Top: ", pTexture->GetCroppingTop());
	//DisplayDouble("            Cropping Right: ", pTexture->GetCroppingRight());
	//DisplayDouble("            Cropping Bottom: ", pTexture->GetCroppingBottom());

	//const char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
	//	"Box", "Face", "UV", "Environment" };

	//DisplayString("            Mapping Type: ", lMappingTypes[pTexture->GetMappingType()]);

	//if (pTexture->GetMappingType() == FbxTexture::ePlanar)
	//{
	//	const char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

	//	DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]);
	//}

	//const char* lBlendModes[]   = { "Translucent", "Add", "Modulate", "Modulate2" };   
	//if(pBlendMode >= 0)
	//	DisplayString("            Blend Mode: ", lBlendModes[pBlendMode]);
	//DisplayDouble("            Alpha: ", pTexture->GetDefaultAlpha());

	//if (lFileTexture)
	//{
	//	const char* lMaterialUses[] = { "Model Material", "Default Material" };
	//	DisplayString("            Material Use: ", lMaterialUses[lFileTexture->GetMaterialUse()]);
	//}

	//const char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map", 
	//	"Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

	//DisplayString("            Texture Use: ", pTextureUses[pTexture->GetTextureUse()]);
	//DisplayString("");                

}

void _FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, KMesh *pKMesh, int nMaterialIndex)
{

	if( pProperty.IsValid() )
	{
		int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();

		for (int j = 0; j < lTextureCount; ++j)
		{
			//Here we have to check if it's layeredtextures, or just textures:
			FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			if (lLayeredTexture)
			{
				//DisplayInt("    Layered Texture: ", j);                
				int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
				for(int k =0; k<lNbTextures; ++k)
				{
					FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
					if(lTexture)
					{

						if(pDisplayHeader){                    
							//DisplayInt("    Textures connected to Material ", nMaterialIndex);
							pDisplayHeader = false;
						}

						//NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
						//Why is that?  because one texture can be shared on different layered textures and might
						//have different blend modes.

						FbxLayeredTexture::EBlendMode lBlendMode;
						lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
						//DisplayString("    Textures for ", pProperty.GetName());
						//DisplayInt("        Texture ", k);  
						_DisplayTextureInfo(lTexture, (int) lBlendMode, pKMesh, nMaterialIndex, pProperty.GetName());   
					}

				}
			}
			else
			{
				//no layered texture simply get on the property
				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
				if(lTexture)
				{
					//display connected Material header only at the first time
					if(pDisplayHeader){                    
						//DisplayInt("    Textures connected to Material ", nMaterialIndex);
						pDisplayHeader = false;
					}             

					//DisplayString("    Textures for ", pProperty.GetName());
					//DisplayInt("        Texture ", j);  
					_DisplayTextureInfo(lTexture, -1, pKMesh, nMaterialIndex, pProperty.GetName());
				}
			}
		}
	}//end if pProperty
}

void ParseTextureMapping(KMesh *pKMesh)
{
	int lMaterialIndex;
	FbxGeometry* pGeometry = pKMesh->m_pFbxMesh;
	FbxProperty lProperty;
	if(pGeometry->GetNode()==NULL)
		return;
	int lNbMat = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++){
		FbxSurfaceMaterial *lMaterial = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
		bool lDisplayHeader = true;

		//go through all the possible textures
		if(lMaterial){

			int lTextureIndex;
			FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
			{
				lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
				_FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, pKMesh, lMaterialIndex); 
			}

		}//end if(lMaterial)

	}// end for lMaterialIndex	
}





#define  MATERIAL_OPTION_HIDE              1

#define  MATERIAL_OPTION_ZBUFFER_FALSE     1<<1
#define  MATERIAL_OPTION_ZBUFFER_TRUE      1<<2
#define  MATERIAL_OPTION_ZBUFFER_USEW      1<<3

#define  MATERIAL_OPTION_FILL_POINT        1<<4
#define  MATERIAL_OPTION_FILL_WIREFRAME    1<<5
#define  MATERIAL_OPTION_FILL_SOLID        1<<6

#define  MATERIAL_OPTION_SHADE_FLAT        1<<7
#define  MATERIAL_OPTION_SHADE_GOURAUD     1<<8
#define  MATERIAL_OPTION_SHADE_PHONG       1<<9

#define  MATERIAL_OPTION_CULL_NONE         1<<10
#define  MATERIAL_OPTION_CULL_CW           1<<11
#define  MATERIAL_OPTION_CULL_CCW          1<<12

#define  MATERIAL_OPTION_SPECULARENABLE    1<<13
#define  MATERIAL_OPTION_LIGHTING          1<<14


const KVec4 Ambient(1, 1, 1, 1);
const KVec4 Diffuse(1, 1, 1, 1);
const KVec4 Specular(1, 0, 0, 0);
const KVec4 Emissive(1, 0, 0, 0);

void SaveMaterailFile(KMesh *pMesh, const char *szOutDir, const char* pszTgaOutPath, KParser *pParser)
{
	BOOL bHD = ExportOption()->bHD;
	if (bHD) return;

	char strFixTextureName[256];
	char strOutTextureDir[256];
	char strFileName[256];
	char strMreFileName[256];
	char strNorFileName[256];
	
	int len = (int) strlen(szOutDir);
	/*
	if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
		sprintf(strOutTextureDir, "%sTexture", szOutDir);
	else
		sprintf(strOutTextureDir, "%s\\Texture", szOutDir);
	*/
	{
		const char* pszTmp = pszTgaOutPath;
		while ((pszTmp = strchr(pszTmp, '\\')) != NULL)
		{
			if (0 == _strnicmp(pszTmp, "\\data\\", strlen("\\data\\")))
			{
				++pszTmp;
				sprintf(strOutTextureDir, "%s", pszTmp);
				break;
			}
			++pszTmp;
		}
	}

	DWORD Def_Option = MATERIAL_OPTION_ZBUFFER_TRUE|
		MATERIAL_OPTION_SHADE_GOURAUD|
		MATERIAL_OPTION_SPECULARENABLE|
		MATERIAL_OPTION_LIGHTING |
		MATERIAL_OPTION_CULL_CW |
		MATERIAL_OPTION_FILL_SOLID |
		MATERIAL_OPTION_LIGHTING |
		MATERIAL_OPTION_SPECULARENABLE |
		MATERIAL_OPTION_ZBUFFER_TRUE
		;

	//fbx材质似乎无法获取是否双面的信息
	//if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	//{
	//	StdMat* std = (StdMat*)mtl;
	//	if (std->GetTwoSided())
	//	{
	//		Def_Option |= MATERIAL_OPTION_CULL_NONE;
	//	}
	//	else
	//	{
	//		Def_Option |= MATERIAL_OPTION_CULL_CW;
	//	}

	//	if (std->GetWire()) 
	//	{
	//		Def_Option |= MATERIAL_OPTION_FILL_WIREFRAME;
	//	}
	//	else
	//	{
	//		Def_Option |= MATERIAL_OPTION_FILL_SOLID;
	//	}
	//}
	//Option |= (MATERIAL_OPTION_LIGHTING | MATERIAL_OPTION_SPECULARENABLE | MATERIAL_OPTION_ZBUFFER_TRUE);


	DWORD32 Mask = 0x4D41544C;//MATL对应的ASECC代码
	DWORD32 BlockEnd  = 0xFFFFFFFF;  
	DWORD32 BlockLength = 0;
	DWORD32 NumSubsets = (DWORD32) pMesh->m_SubsetIds.size();
	
	

	char pFileName[MAX_PATH];


	if (!pParser->IsMdlParser() && !ExportOption()->bGroupExport)
	{
		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
			sprintf(pFileName, "%s%s.mtl", szOutDir, pParser->GetSourceName());
		else
			sprintf(pFileName, "%s\\%s.mtl", szOutDir, pParser->GetSourceName());

		if (_access(szOutDir, 0) != 0)
		{
			Make_Dirs(szOutDir);
		}
	}
	else
	{
		char szTotalPath[MAX_PATH];

		char szMeshOutDir[MAX_PATH];

		char *p = strstri(pMesh->m_szMeshName.c_str(), "#");
		if (p)
		{
			GetMapsSourcePathFromPath(szOutDir, szMeshOutDir, MAX_PATH);
		}
		else
		{
			strcpy(szMeshOutDir, szOutDir);
		}

		string strMeshName = pMesh->m_szMeshName;
		ReplaceAll(strMeshName, "#", "\\");

		if(szOutDir[len - 1] == '\\'  || szOutDir[len - 1] == '/')
		{
			sprintf(pFileName, "%s%s.mtl", szMeshOutDir, strMeshName.c_str());
		}
		else
		{
			sprintf(pFileName, "%s\\%s.mtl", szMeshOutDir, strMeshName.c_str());	
		}

		GetParentPath(pFileName, szTotalPath, MAX_PATH);
		if (_access(szTotalPath, 0) != 0)
		{
			Make_Dirs(szTotalPath);
		}
	}
	
	FILE* pFile = fopen(pFileName,"wb");

	if(pFile==NULL)
		goto Exit0;

	fwrite(&Mask, sizeof(DWORD32), 1, pFile);




	//计算并写入块长度
	BlockLength += sizeof(DWORD32)*2 ;//子部分数目，结束标志
	for(DWORD32 i_Sub=0; i_Sub < NumSubsets; i_Sub++)
	{
		//LPMATERIAL pMaterial = &m_lpMaterial[i_Sub]; 
		//BlockLength += sizeof(float)*17 + sizeof(DWORD32) + //四种颜色，反光强度，帖图数目
		//	pMaterial->NumTexture*(sizeof(float)+sizeof(TCHAR)*(20+256));//帖图信息 
		BlockLength += sizeof(float)*17 + sizeof(DWORD32) + //四种颜色，反光强度，帖图数目
			1 * (sizeof(float)+sizeof(char)*(20+256));//帖图信息 
	}

	fwrite(&BlockLength,sizeof(DWORD32),1,pFile);
	fwrite(&NumSubsets,sizeof(DWORD32),1,pFile);                  //子部分数目

	float Power = 0;
	//for( i_Sub=0;i_Sub<m_dNumSubsets;i_Sub++)
	for(set<int>::iterator it = pMesh->m_SubsetIds.begin(); it != pMesh->m_SubsetIds.end(); ++it)
	{
		int subsetId = *it;
				
		fwrite(&Ambient, sizeof(KVec4), 1, pFile); 
		fwrite(&Diffuse, sizeof(KVec4), 1, pFile); 
		fwrite(&Specular, sizeof(KVec4), 1, pFile); 
		fwrite(&Emissive, sizeof(KVec4), 1, pFile); 
		fwrite(&Power, sizeof(float), 1, pFile);
		fwrite(&Def_Option, sizeof(DWORD32), 1, pFile);//材质选项
		DWORD32 NumOption = 0;
		fwrite(&NumOption,sizeof(DWORD32), 1, pFile);//选项数目

		map<int, KMaterail>::iterator itt = pMesh->m_Materials.find(subsetId);
		if(itt != pMesh->m_Materials.end())
		{
			KMaterail &mtl = itt->second;
			DWORD32 textureNum = 1;
			float Amount = 1.0;
			fwrite(&textureNum,sizeof(DWORD32),1,pFile);//帖图数目
			fwrite(&Amount,sizeof(float),1,pFile);
			char szUsage[20];
			strncpy(szUsage, mtl.usage.c_str(), 20);
			fwrite(szUsage, sizeof(char), 20, pFile);
			
			
			
			if (_access(pszTgaOutPath, 0) != 0)
			{
				Make_Dirs(pszTgaOutPath);
			}

			//char outTextureFile[MAX_PATH];	
			GetFileNameFormPath(mtl.textureName.c_str(), strFileName, 256);
			//sprintf(outTextureFile, "%s\\%s", strOutTextureDir, strFileName);

			if(_access(mtl.textureName.c_str(), 4) ==0)
			{
				char szFile[FILENAME_MAX] = { 0 };
				sprintf_s(szFile, FILENAME_MAX, "%s\\%s", pszTgaOutPath, strFileName);
				CopyFile(mtl.textureName.c_str(), szFile, FALSE);
			}
			else
			{
				char msg[260];
				//char msgOut[260];
				sprintf(msg, "warn: %s: %s is not exist \r\n", pMesh->m_szMeshName.c_str(), mtl.textureName.c_str());
				//ConvertUTF8ToGBK(msg, msgOut);
				pParser->errorMsg.append(msg);
			}

			//mre texture
			GetFileNameFormPath(mtl.mreTextureName.c_str(), strMreFileName, 256);
			if(_access(mtl.mreTextureName.c_str(), 4) ==0)
			{
				char szFile[FILENAME_MAX] = { 0 };
				sprintf_s(szFile, FILENAME_MAX, "%s\\%s", pszTgaOutPath, strMreFileName);
				CopyFile(mtl.mreTextureName.c_str(), szFile, FALSE);
			}
			else
			{
				char msg[260];
				//char msgOut[260];
				sprintf(msg, "warn: %s: %s is not exist \r\n", pMesh->m_szMeshName.c_str(), mtl.mreTextureName.c_str());
				//ConvertUTF8ToGBK(msg, msgOut);
				pParser->errorMsg.append(msg);
			}

			//nor texture
			GetFileNameFormPath(mtl.norTextureName.c_str(), strNorFileName, 256);
			if(_access(mtl.norTextureName.c_str(), 4) ==0)
			{
				char szFile[FILENAME_MAX] = { 0 };
				sprintf_s(szFile, FILENAME_MAX, "%s\\%s", pszTgaOutPath, strNorFileName);
				CopyFile(mtl.norTextureName.c_str(), szFile, FALSE);
			}
			else
			{
				char msg[260];
				//char msgOut[260];
				sprintf(msg, "warn: %s: %s is not exist \r\n", pMesh->m_szMeshName.c_str(), mtl.norTextureName.c_str());
				//ConvertUTF8ToGBK(msg, msgOut);
				pParser->errorMsg.append(msg);
			}

			
			sprintf(strFixTextureName, "%s\\%s", strOutTextureDir, strFileName);
			
			//GetRelativePath(pFileName, mtl.textureName.c_str(), strFixTextureName);
			//strncpy(strFixTextureName, mtl.textureName.c_str(), 256);
			fwrite(strFixTextureName, sizeof(char), 256, pFile);
			//此处暂时先省略了textureoption的导出...
			DWORD32 numTextureOption = 0;			
			fwrite(&numTextureOption, sizeof(DWORD), 1, pFile);//帖图选项数目			
		}
		else
		{
			//这种情况应该不会出现的
			DWORD32 textureNum = 0;
			fwrite(&textureNum,sizeof(DWORD32),1,pFile);//帖图数目			

			char msg[260] = {0};
			sprintf(msg, "warn: %s: subset %d has no texture\r\n", pMesh->m_szMeshName.c_str(),subsetId);
			printf(msg);
			pParser->errorMsg.append(msg);
		}
	}	
	fwrite(&BlockEnd,sizeof(DWORD32),1,pFile);
	//int n = (int) ftell(pFile);
	
Exit0:

	if(pFile)
	{
		fclose(pFile);
	}
	return;
}