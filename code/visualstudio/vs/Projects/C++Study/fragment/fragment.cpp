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

int _tmain(int argc, _TCHAR* argv[])
{
	// 1. check system endian
	check_endian();

	FnTestAPI();
	TestAsync();
	

	getchar();
	return 0;
}

