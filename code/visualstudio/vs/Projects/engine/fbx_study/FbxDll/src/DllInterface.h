#ifndef DLLINTERFACE_H
#define DLLINTERFACE_H

#ifndef DLL_FBX_API
#define DLL_FBX_API extern "C" __declspec(dllexport)
#else
#define DLL_FBX_API extern "C" __declspec(dllimport)
#endif


typedef int BOOL;
#define FALSE   0
#define TRUE    1

enum enuExportType
{
	ENUM_EXPORTTYPE_START,
	ENUM_EXPORTTYPE_MAINCHARACTOR = ENUM_EXPORTTYPE_START,
	ENUM_EXPORTTYPE_NORMAL,
	ENUM_EXPORTTYPE_SCENE,
	ENUM_EXPORTTYPE_END,
	ENUM_EXPORTTYPE_FORCE_DWORD = 0xffffffff,
};


struct KG3DExportOption
{
	enuExportType ExportType; // 导出的类型, Common通用或者Main主角模式		
	int    bUnitModel;
	int    bExchangeYZ; // 是否需要交换Y轴和Z轴的数据
	float  fFbxScale; // 是或进行inch到厘米的转换
	int    bNormalizeSocketSize;
	int    bMayaFbx;
	int    bFbxNameExport;
	int    bGroupExport;
	int    bHD;
	int    bTangent;
	int    bOnlyAniExport;	
	int    bCollision;
    int    bModelFileExport;

	KG3DExportOption()
	{
		ExportType = ENUM_EXPORTTYPE_NORMAL;
		bExchangeYZ = true;
		fFbxScale = 1.0f;//2.54f; //默认进行inch到cm的转换
		bNormalizeSocketSize = true;
		bMayaFbx = FALSE;
		bFbxNameExport = TRUE;
		bGroupExport = FALSE;
		bHD = FALSE;
		bTangent = TRUE;
		bOnlyAniExport = FALSE;		
		bCollision = FALSE;
        bModelFileExport = FALSE;
	}
	bool Switch()
	{	
		return !bMayaFbx || bExchangeYZ;
	}
	bool SwitchToMaxMat()
	{
		return !bMayaFbx && !bExchangeYZ;
	}
};

struct IKFbxExporter
{
public:	
	virtual void Clear() = 0;
	virtual BOOL LoadFbx(const char *szFbxPath) = 0;
	virtual BOOL DoParse(BOOL bMdlParser, float fbxScale, BOOL bMayaFbx) = 0;
	virtual BOOL Save(const char *szOutPath, const char* pszTgaOutPath, BOOL bOnlyAnimationChecked,  int nSampleInterval, int nFrameTime, int nFrameFrom, int nFrameTo, BOOL bCompress, float fbxScale, BOOL bChangeYZ) = 0;
	virtual int GetFrameCount() = 0;
	virtual const char* GetErrorMsg() = 0;
	virtual const char* GetWarnMsg() = 0;
};


DLL_FBX_API IKFbxExporter * CreateFbxExporter();
DLL_FBX_API void DestoryFbxExporter();
DLL_FBX_API KG3DExportOption* ExportOption();

DLL_FBX_API void SetSaveFilePath(const char *pcszPath, int nLen);
DLL_FBX_API const char * GetSaveFilePath();
#endif