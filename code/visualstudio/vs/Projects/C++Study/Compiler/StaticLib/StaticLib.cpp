// StaticLib.cpp : 定义静态库的函数。
// 这个StalicLib和StalicLib2都会用到CoreMath中的Kvec2Test结构，因为是静态库，所以只要都包含头文件即可
// StalicLib会调用StalicLib2中的函数，StalicLib2接口的参数是Kvec2Test相关的

#include <iostream>
#include "pch.h"
#include "framework.h"
#include "StaticLib.h"
//#include "KVec2.h"
#include "StaticLib2.h"
#include "blockallocator.h"
#include "astar.h"
//#include "KVectorInt2.h"
//#include "KVector2.h"

// TODO: 这是一个库函数示例
void TestUseStrcutInOtherStaticLib()
{
	/*StalicLib a;
	KVec2Test v2Test = KVec2Test(100, 200);
	a.UseStrcutInOtherStaticLib(v2Test);*/

	/*StalicLib2 a;
	KVec2Test v2Test;
	a.UseStructInOhterLibAndCalledByOtherLib(v2Test);

	std::vector<KVec2Test> Vec2TestList;
	a.UseStructInOhterLibAndCalledByOtherLib1(Vec2TestList);

    AStar::Params param;
    param.width = 100;
    param.height = 100;
    param.corner = true;
    param.start = KVectorInt2(1, 1);
    param.end = KVectorInt2(10, 10);
    param.can_pass = [&](const KVectorInt2& pos)->bool
    {
        return true;
    };

	BlockAllocator allocator;
	AStar algorithm(&allocator);*/
}


//void StalicLib::UseStrcutInOtherStaticLib(KVec2Test v2Test)
//{
//	std::cout << "x = " << v2Test.x << std::endl;
//}
//
//void StalicLib::UseStrcutInOtherStaticLib1(std::vector<KVec2Test> vecTests)
//{
//	std::cout << "vector.size() = " << vecTests.size() << std::endl;
//	KVec2Test a, b;
//	a.cross(b);
//}

void StalicLib::UseStrcutInOtherStaticLib2(std::vector<KVec2>& vecTests)
{
	AStar::Params param;
	param.width = 100;
	param.height = 100;
	param.corner = true;
	param.start = KVec2(1, 1);
	param.end = KVec2(10, 10);
	param.can_pass = [&](const KVec2& pos)->bool
	{
		return true;
	};

	BlockAllocator allocator;
	AStar algorithm(&allocator);
	vecTests = algorithm.find(param);
}

void StalicLib::UseStrcutInOtherStaticLib3()
{
	std::vector<KVec2> vecTests;
	UseStrcutInOtherStaticLib2(vecTests);

	KVec2 v2Test(1000, 500);
	UseStrcutInOtherStaticLib4(v2Test);
}

void StalicLib::UseStrcutInOtherStaticLib4(KVec2& v2Test)
{
	std::cout << v2Test.x << std::endl;
}