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

namespace Crasheye
{
  typedef void (*FnOnCrashCallback)(BOOL bCaptureSucceed, const char* cpszCrashReportFile);
  
  struct ApiHolder {
    
    enum DumpCommandType {
      
        enumDump_Begin = 0,

        enumDump_BackgroundExecutionFlag = enumDump_Begin,		// 后台上传宕机信息（不显示DumpReport界面）
        enumDump_ForceUpload,									// Dump是否强制上报（忽略玩家在DumpReport界面上的选择）
        enumDump_LogDirectory,									// 设置log文件路径
        enumDump_CollectFile,									// 设置需收集的文件名
        enumDump_LogDirectory_UTF8,								// 设置log文件路径（传入参数为UTF8，Unity/UE 使用此参数）
        enumDump_CollectFile_UTF8,								// 设置需收集的文件名（传入参数为UTF8，Unity/UE 使用此参数）
        enumDump_SetURL,										// 设置上传url (internal_cn|internal_us|external|test) => (国内|海外|外部项目|内部测试)
        enumDump_SetBeta,										// 设置 beta
        enumDump_SetUserIdentifier,								// 设置 UserIdentifier
        enumDump_AddExtraData,								    // 添加额外数据
        enumDump_GM_TEST,										// GM测试指令

        enumDump_Count
    };
    
    typedef BOOL(*pfnInitDumperCrasheyeType)(const char* strAppkey, const char* strVersion, const char* strChannel);
    typedef void (*pfnUnInitDumperType)();
    typedef BOOL(*pfnSetOnMiniDumpCreateCallBackType)(FnOnCrashCallback pCallback);
    typedef BOOL(*pfnSetConfigType)(const int nCommandType, const void* pArg);
    
    ApiHolder():pInitFunction(nullptr), pfnSetOnMiniDumpCreateCallBack(nullptr), pfnSetConfig(nullptr), pUnInitFunction(nullptr){
      hDumper = LoadLibraryW(DUMPER_DLL);
      if (hDumper){
        pInitFunction = (pfnInitDumperCrasheyeType)::GetProcAddress(hDumper, "InitDumperCrasheye");
        pfnSetOnMiniDumpCreateCallBack = (pfnSetOnMiniDumpCreateCallBackType)::GetProcAddress(hDumper, "SetOnMiniDumpCreateCallBack");
        pfnSetConfig = (pfnSetConfigType)::GetProcAddress(hDumper, "SetConfig");
        pUnInitFunction = (pfnUnInitDumperType)::GetProcAddress(hDumper, "UnInitDumper");
      }
    }
    
    ~ApiHolder(){
      if (hDumper){
        FreeLibrary(hDumper);
        hDumper = NULL;
      }
    }
    HMODULE hDumper;
    pfnInitDumperCrasheyeType pInitFunction;
    pfnUnInitDumperType pUnInitFunction;
    pfnSetOnMiniDumpCreateCallBackType pfnSetOnMiniDumpCreateCallBack;
    pfnSetConfigType pfnSetConfig;
    
  };
  
  // The non-static inline function declaration refers to the same function in every translation unit (source file) that uses it.
  inline ApiHolder* get_api(){
    static ApiHolder s_api;
    return &s_api;
  }
  
  
  /**
  * 初始化 CrasheyeSdk. 
  * 
  * 崩溃收集相关的(如设置SEH,VEH等)一系列初始化操作.
  *
  * 本接口调用后会产生一条报活信息, 如需设置 url, 
  * 请先调用 SetURL, 再调用本接口.
  *
  * @param  strAppkey   平台申请的当前应用 appkey
  * @param  strVersion  应用当前版本号
  * @param  strChannel  应用的渠道号
  * @return 是否成功初始化
  */
  inline BOOL Init(const char* strAppkey, const char* strVersion, const char* strChannel) {
      if (get_api()->pInitFunction) {
          return get_api()->pInitFunction(strAppkey, strVersion, strChannel);
      }
      return FALSE;
  }

  /**
  * 反初始化 CrasheyeSdk. 
  * 
  * 释放 Init 阶段注册的异常处理器. 
  * 在程序正常退出时调用.
  *
  */
  inline void UnInit() {
      if (get_api()->pUnInitFunction) {
          get_api()->pUnInitFunction();
      }
  }

  /**
  * 设置上报路径. 
  * 
  * 目前支持 3 种路径
  *   internal_cn => 国内自研项目
  *   internal_us => 海外自研项目
  *   external    => 外部项目
  *
  * 若不设置, 默认是 external
  *
  * 若要设置, 建议在 Init 之前调用.
  *
  * @param  szUrl  internal_cn/internal_us/external 三选一.
  * @return 是否成功设置
  */
  inline BOOL SetURL(const char* szUrl) {
      if (get_api()->pfnSetConfig) {
          return get_api()->pfnSetConfig(ApiHolder::enumDump_SetURL, szUrl);
      }
      return FALSE;
  }

  /**
  * 标记当前版本为调试版本
  * 
  * 若要设置, 建议在 Init 之前调用.
  *
  * @return 是否成功设置
  */
  inline BOOL SetBeta() {
      if (get_api()->pfnSetConfig) {
          int dummy;
          return get_api()->pfnSetConfig(ApiHolder::enumDump_SetBeta, &dummy);
      }
      return FALSE;
  }

  /**
  * 设置用户id. 
  *
  * @param  szUserIdentifier  项目内部的用户id
  * @return 是否成功设置
  */
  inline BOOL SetUserIdentifier(const char* szUserIdentifier) {
      if (get_api()->pfnSetConfig) {
          return get_api()->pfnSetConfig(ApiHolder::enumDump_SetUserIdentifier, szUserIdentifier);
      }
      return FALSE;
  }

  /**
  * 添加额外上报信息.
  *
  * 以键值对的形式添加额外信息, 添加的信息会被包含着崩溃报告中随报告一同上报.
  *
  * @param  szKey   键
  * @param  szValue 值
  * @return 是否成功添加
  */
  inline BOOL AddExtraData(const char* szKey, const char* szValue) {
      if (get_api()->pfnSetConfig) {
          const char* packed[2] = { szKey, szValue };
          return get_api()->pfnSetConfig(ApiHolder::enumDump_AddExtraData, packed);
      }
      return FALSE;
  }

  /**
  * 添加额外上报日志(文件).
  *
  * 添加的文件会被包含着崩溃报告中随报告一同上报.
  *
  * @param  szCollectFile   需要额外收集的文件
  * @return 是否成功添加
  */
  inline BOOL AddCustomLog(const char* szCollectFile) {
      if (get_api()->pfnSetConfig) {
          return get_api()->pfnSetConfig(static_cast<int>(ApiHolder::enumDump_CollectFile), szCollectFile);
      }
      return FALSE;
  }

  /**
  * 设置崩溃回调
  *
  * 设置崩溃发生时的回调, 回调会在报告文件写入完成后被调用.
  * 回调函数签名为 
  *   void(*FnOnCrashCallback)(BOOL bCaptureSucceed, const char* cpszCrashReportFile);
  *   // @param bCaptureSucceed      是否成功捕获
  *   // @param cpszCrashReportFile  报告文件名
  *
  * 注意: 回调发生时报告文件已经生成, 此时再调用 AddExtraData 和 AddCustomLog 添加的内
  * 容, 不会被加到报告中. 如果想崩溃发生时添加信息, 可以在初始化时调用 
  *     AddCustomLog("after_crash.log")
  * 然后在回调中, 把信息写入 after_crash.log
  * 
  * @param  szCollectFile   需要额外收集的文件
  * @return 是否成功添加
  */
  inline BOOL SetCrashCallback(FnOnCrashCallback pCallback) {
      if (get_api()->pfnSetOnMiniDumpCreateCallBack) {
          return get_api()->pfnSetOnMiniDumpCreateCallBack(pCallback);
      }
      return FALSE;
  }
  
  inline BOOL SendScriptException(const char* szError, const char* szCallStack) {
      // not implemented
      return FALSE;
  }
}