// StaticLib2.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"
#include "StaticLib2.h"
#include "KVec2.h"

void StalicLib2::UseStructInOhterLibAndCalledByOtherLib(KVec2Test v2Test)
{

}

void StalicLib2::UseStructInOhterLibAndCalledByOtherLib1(std::vector<KVec2Test>& Vec2TestList)
{
	KVec2Test a, b;
	KVec2Test c = a.cross(b);
	Vec2TestList.push_back(a);
	Vec2TestList.push_back(b);
	Vec2TestList.push_back(c);
}