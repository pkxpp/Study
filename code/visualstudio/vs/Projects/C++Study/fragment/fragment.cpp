// fragment.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <thread>
#include <functional>
#include "Async/TaskGraphInterfaces.h"
#include "Public/Containers/LockFreeList.h"
#include "Private/Memory/KMemory.h"
#include "Public/RenderingThread.h"

void TestAsync()
{
	KMemory::Initialize(nullptr);
	FTaskGraphInterface::Startup(1);

	//FStallingTaskQueue<FBaseGraphTask, PLATFORM_CACHE_LINE_SIZE, 2> StallQueue;
	//StallQueue.Push(Task, PriIndex);
	// 测试宏
	//ENQUEUE_RENDER_COMMAND(int)([]() {
	//	printf("[ENQUEUE_RENDER_COMMAND]: %u\n", GetTickCount());
	//});

	auto taskFunc = [](void) {
		printf("[task]: %u\n", GetTickCount());
	};

	auto taskFuncInRenderThread = [](void) {
		printf("[task] run in rendering thread: %u\n", GetTickCount());
	};

	//TGraphTask<FFunctionGraphTask>::CreateTask().ConstructAndDispatchWhenReady(task, ENamedThreads::RHIThread);
	//auto task = TGraphTask<FFunctionGraphTask>::CreateTask().ConstructAndHold(taskFunc, ENamedThreads::RHIThread);
	//std::thread t([&]() {
	//	while (true)
	//	{
	//		Sleep(1000);
	//		printf("[thread]Tick: %u\n", GetTickCount());
	//		FBaseGraphTask* Task = StallQueue.Pop(0, true);
	//		if (Task)
	//		{
	//			//Task->Execute();
	//			printf("[thread]Execute task: %u\n", GetTickCount());

	//		}
	//	}
	//});

	//FTaskGraphInterface::Get().AttachToThread(ENamedThreads::GameThread);

	StartRenderingThread();

	while (true)
	{
		Sleep(100);
		printf("[main]Tick: %u\n", GetTickCount());
		ENQUEUE_RENDER_COMMAND(int)([]() {
			printf("[main]: ENQUEUE_RENDER_COMMAND %u\n", GetTickCount());
		});
		//StallQueue.Push(task, 0);
	}

	//t.join();
}

void TestShaderHash()
{
	typedef struct _GUID {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	} GUID;

	auto KG_BKDRHashForBuffer64Bit = [](const char* pcszString, int nLength)
	{
		uint64_t ullHash = 0;
		for (int i = 0; i < nLength; i++)
		{
			//ullHash = ullHash * 131 + *pcwszString++;
			ullHash = (ullHash << 7) + (ullHash << 1) + ullHash + *pcszString++;              //ullHash移位7位再加上ullHash移位1位在加上ullHash即131倍，在加上pcszString取值后再往后移一位
		}

		return ullHash;
	};

	auto KG3D_GUIDTOString = [](const GUID& uuid, char* pszBuffer, unsigned uBufSize)
	{
		BOOL nResult = 0;
		BOOL nRetCode = 0;

		nRetCode = sprintf_s(pszBuffer, uBufSize, "{%08x-%04hx-%04hx-%02x%02x-%02x%02x%02x%02x%02x%02x}",
			uuid.Data1,
			uuid.Data2, uuid.Data3,
			uuid.Data4[0], uuid.Data4[1],
			uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]
		);
		pszBuffer[nRetCode] = '\0';

		nResult = 1;
	};

	//std::string strPath = "data\\source\\maps_source\\material\\长歌门幻影2_1.fx5;1;shader_mode=5,shader_x11_mtlsys=,simplify_lighting=,";
	std::string strPath = "data\\material\\Material\\Mtl_Water\\Mtl_WaterFall_Stream_lod2.fx5;1;shader_mode=5,shader_x11_mtlsys=,simplify_lighting=,";
	auto fnTest =[&](std::string strPath)
	{
		uint64_t uID = KG_BKDRHashForBuffer64Bit(strPath.c_str(), strPath.length());
		GUID guid;
		memcpy_s(&guid, sizeof(GUID), &uID, sizeof(UINT64));
		char szGuid[256];
		KG3D_GUIDTOString(guid, szGuid, 256);
		printf("guid = %s\n", szGuid);
	};
	/*fnTest(strPath);
	strPath = "data\\source\\maps_source\\material\\长歌门幻影2_1.fx5;1;shader_mode=5,shader_x11_mtlsys=,simplify_lighting=,";
	fnTest(strPath);*/
	strPath = "data\\material\\shader\\mtl_water\\mtl_underwater.fx5;20;shader_mode=5,shader_x11_mtlsys=,";
	fnTest(strPath);
	strPath = "data\\material\\shader\\mtl_water\\Mtl_UnderWater.fx5;20;shader_mode=5,shader_x11_mtlsys=,";
	fnTest(strPath);
	strPath = "data\\source\\maps_source\\material\\长歌门幻影2_1.fx5;1;SHADER_MODE=5,SHADER_X11_MTLSYS=,SIMPLIFY_LIGHTING=,";
	fnTest(strPath);

	auto KG3D_NormalizePath = [](const char* pcszStr, char* pcszRetStr, size_t uRetBufferSize)
	{
		BOOL bResult = FALSE;

		//KG_PROCESS_ERROR(pcszStr);
		//KG_PROCESS_ERROR(pcszRetStr);

		strncpy_s(pcszRetStr, uRetBufferSize, pcszStr, _TRUNCATE);
		pcszRetStr[MAX_PATH - 1] = '\0';

		unsigned uStrLen = (unsigned)strlen(pcszRetStr);

		// encode error when in unicode system, replace CharLowerBuffA with LCMapStringA
		//CharLowerBuffA(pcszRetStr, uStrLen);
		const LCID cnLocale = LocaleNameToLCID(L"zh-CN", 0);
		LCMapStringA(cnLocale, LCMAP_LOWERCASE, pcszRetStr, uStrLen, pcszRetStr, uStrLen);
		//_NormalizeSplash(pcszRetStr);

		bResult = TRUE;
	};
	char szStandard[MAX_PATH] = {0};
	KG3D_NormalizePath(strPath.c_str(), szStandard, MAX_PATH);
	printf("convert to: %s\n", szStandard);
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 1. check system endian
	check_endian();

	FnTestAPI();
	//TestAsync();
	
	TestShaderHash();

	getchar();
	return 0;
}

