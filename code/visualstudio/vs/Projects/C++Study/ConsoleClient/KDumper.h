#pragma once
#include <Windows.h>


#ifdef WIN32		// is windows.
#ifdef _DEBUG		// is debug.
#ifdef _WIN64		// is x64
#define DUMPER_DLL	L".\\DumperD64.dll"
#else				// is x86
#define DUMPER_DLL	L".\\DumperD.dll"
#endif				// _WIN64
#else				// is release.
#ifdef _WIN64		// is x64
#define DUMPER_DLL	L".\\Dumper64.dll"
#else				// is x86
#define DUMPER_DLL	L".\\Dumper.dll"
#endif				// _WIN64
#endif				// _DEBUG
#else				// is not windows.
// do nothing ...
#endif // WIN32

	// Dump发送命令行参数类型枚举值
#if _MSC_VER>=1700		// visual studio 2012,  vs2005不支持enum class语法
enum class DumpCommandType
#else
enum DumpCommandType
#endif
{
	enumDump_Begin = 0,

	enumDump_BackgroundExecutionFlag = enumDump_Begin,		// 后台上传宕机信息（不显示DumpReport界面）
	enumDump_ForceUpload,									// Dump是否强制上报（忽略玩家在DumpReport界面上的选择）
	enumDump_LogDirectory,									// 设置log文件路径
	enumDump_CollectFile,									// 设置需收集的文件名
	enumDump_LogDirectory_UTF8,								// 设置log文件路径（传入参数为UTF8，Unity/UE 使用此参数）
	enumDump_CollectFile_UTF8,								// 设置需收集的文件名（传入参数为UTF8，Unity/UE 使用此参数）

	enumDump_GM_TEST,										// GM测试指令

	enumDump_Count
};

namespace KDumper
{

	typedef void(*OnMiniDumpCreateCallback)(BOOL bCaptureSucceed, const char* cpszCrashReportFile);

	// 初始化
	BOOL InitKDumper(const char* pszProject, const char* pszVersionEx, const char* pszVersion);
	// 释放资源
	void UnInitKDumper();

	// 设置宕机捕捉后的回调函数
	BOOL SetMiniDumpCreateCallback(OnMiniDumpCreateCallback pCallback);

	// nCommandType 为 DumpCommandType 类型
	BOOL SetConfig(const int nCommandType, const void* pArg);
}