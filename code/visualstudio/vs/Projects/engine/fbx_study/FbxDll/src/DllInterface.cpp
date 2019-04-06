#include "DllInterface.h"
#include "KParser.h"
#include "Common/Common.h"
#include "fbxsdk/scene/fbxaxissystem.h"
#include <Windows.h>

DLL_FBX_API KG3DExportOption* ExportOption()
{
	static KG3DExportOption option;
	return &option;
}


class KFbxExporter:public IKFbxExporter
{
	FbxManager* m_pSdkManager;
	FbxScene* m_pScene;
		
	char m_szFbxPath[MAX_PATH];
	KParser  m_KParser;

public:
	KFbxExporter();
	~KFbxExporter();
	void Clear();
	BOOL LoadFbx(const char *szFbxPath);
	BOOL DoParse(BOOL bMdlParser, float fbxScale, BOOL bMayaFbx);
	BOOL Save(const char *szOutPath, const char* pszTgaOutPath, BOOL bOnlyAnimationChecked, int nInterval, int nFps, int nFrameFrom, int nFrameTo, BOOL bCompress, float fbxScale, BOOL bChangeYZ);
	int GetFrameCount();
	const char* GetErrorMsg();
	const char* GetWarnMsg();
};

KFbxExporter *pExporter = NULL;


DLL_FBX_API IKFbxExporter * CreateFbxExporter()
{
	if(!pExporter)
	{
		pExporter = new KFbxExporter;
	}
	return pExporter;
}


DLL_FBX_API void DestoryFbxExporter()
{
	delete(pExporter);
	pExporter = NULL;
}

KFbxExporter::KFbxExporter()
{
	// Prepare the FBX SDK.
	InitializeSdkObjects(m_pSdkManager, m_pScene);	
}


KFbxExporter::~KFbxExporter()
{
	bool bResult = false;
	if(m_pSdkManager)
	{
		DestroySdkObjects(m_pSdkManager, bResult);
	}	
}


BOOL g_bMayaFbxFile = FALSE;
float g_fFbxScale = 1.0f;
BOOL KFbxExporter::LoadFbx(const char *_szFbxPath)
{	
	Clear();
	g_bMayaFbxFile = FALSE;
	//g_fFbxScale = 2.54f;
	char szFbxPath[MAX_PATH];
	szFbxPath[0] = '\0';

	ConvertGBKToUtf8(_szFbxPath, szFbxPath);
	printf("loading...%s", _szFbxPath);
	BOOL bResult = LoadScene(m_pSdkManager, m_pScene, szFbxPath);
		
	if(!bResult)
	{		
		m_KParser.errorMsg.append("An error occurred while loading the scene,\r\n file path only support English names");
		return FALSE;
	}
	else
	{
		// 用fbx sdk自带的转换轻松搞定
		//FbxAxisSystem::MayaYUp.ConvertScene(m_pScene);
		FbxSystemUnit::cm.ConvertScene(m_pScene);

		// detect fbx file applicationName
		FbxDocumentInfo* docInfo = m_pScene->GetSceneInfo();
		FbxProperty fb = docInfo->GetFirstProperty();		
		while(fb.IsValid())
		{		
			FbxDataType tp = fb.GetPropertyDataType();
			if(tp.GetType() == eFbxString)
			{
				FbxString str = fb.Get<FbxString>();
				char *pbuf = str.Buffer();
				if(strcmp(pbuf, "3ds Max") == 0)
				{
					g_bMayaFbxFile = FALSE;
					break;
				}
				else if(strcmp(pbuf, "Maya") == 0)
				{
					g_bMayaFbxFile = TRUE;
					break;
				}		
			}
			fb = docInfo->GetNextProperty(fb);
		}

		 FbxGlobalSettings &setting = m_pScene->GetGlobalSettings();
		 fb = setting.GetFirstProperty();
		 while(fb.IsValid())
		 {
			 FbxDataType tp = fb.GetPropertyDataType();
			 FbxString st = fb.GetName();
			 char *pbuf = st.Buffer();
			 if(strcmp(pbuf, "UnitScaleFactor") == 0)
			 {
				 FbxString str = fb.Get<FbxString>();
				 char *value = str.Buffer();
				 g_fFbxScale = (float) atof(value);
				 //break;
			 }		
			 else if(strcmp(pbuf, "UpAxis") == 0)
			 {
				 FbxString str = fb.Get<FbxString>();
				 char *value = str.Buffer();
				 int nUpAxis = (int) atoi(value);

				 if(nUpAxis == 1)// y-up
				 {
					ExportOption()->bExchangeYZ = FALSE;
				 }
				 else if(nUpAxis == 2)// z-up
				 {
					 ExportOption()->bExchangeYZ = TRUE;
				 }
				 //break;
			 }
			 fb = setting.GetNextProperty(fb);
		 }

	}
	printf("  finished.\r\n");
	strncpy(m_szFbxPath, _szFbxPath, MAX_PATH);
	return TRUE;
}

BOOL KFbxExporter::DoParse(BOOL bMdlParser, float fbxScale, BOOL bMayaFbx)
{
	BOOL bResult = TRUE;
	//char szName[MAX_PATH];
	//GetNameFromPath(m_szFbxPath, szName, MAX_PATH);
	////ExportOption()->bMayaFbx = bMayaFbx;

	////这两个参数改成fbx自身解析了，外部不需要这两个参数了
	//ExportOption()->bMayaFbx = g_bMayaFbxFile;
	//ExportOption()->fFbxScale = g_fFbxScale;

	//char szParentPathName[MAX_PATH];
	//if(!GetParentPathName(m_szFbxPath, szParentPathName, MAX_PATH))
	//{
	//	m_KParser.errorMsg.append("error: the input fbxfile has no parent path\r\n");
	//	goto Exit0;
	//}
	//printf("parsing....");
	//m_KParser.Parse(m_pScene, szName, szParentPathName, m_pSdkManager, bMdlParser);	
	//printf("finished.\r\n");
	//bResult = TRUE;
Exit0:
	return bResult;
}

BOOL KFbxExporter::Save(const char *szOutPath, const char* pszTgaOutPath, BOOL bOnlyAnimationChecked, int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress, float fbxScale, BOOL bChangeYZ)
{
// 	if(!g_bMayaFbxFile)
// 	{
// 		//如果是max强制交换YZ好了
// 		bChangeYZ = TRUE;
// 	}
// 	else
// 	{
// 		bChangeYZ = FALSE;
// 	}
	printf("exporting...");
	m_KParser.Save(szOutPath, pszTgaOutPath, bOnlyAnimationChecked, nSampleInterval, nFrameTime, nFrameFrom, nFrameTo, bCompress, fbxScale, bChangeYZ);
	printf("finished.\r\n");
	return TRUE;
}

void KFbxExporter::Clear()
{
	m_KParser.Clear();
}


const char* KFbxExporter::GetErrorMsg()
{	
	return m_KParser.errorMsg.c_str();
}

const char* KFbxExporter::GetWarnMsg()
{
	return m_KParser.warnMsg.c_str();
}

int KFbxExporter::GetFrameCount()
{
	return m_KParser.m_nAnimationFrames;
}


char saveFilepath[MAX_PATH] = {0};
DLL_FBX_API void SetSaveFilePath(const char *pcszPath, int nLen)
{
	saveFilepath[0] = '\0';

	if(pcszPath && nLen <= MAX_PATH)
	{
		strncpy(saveFilepath, pcszPath, MAX_PATH);
	}	
}

DLL_FBX_API const char * GetSaveFilePath()
{
	return &saveFilepath[0];
}