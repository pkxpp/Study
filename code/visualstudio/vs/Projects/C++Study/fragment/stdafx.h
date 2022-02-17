// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件
#include <iostream>


// function list
void check_endian();

void FnTestAPI();

//////////////////////////////////////////////////////////////////////////
/// 函数模板参数匹配问题
//template<typename TSTR, typename LAMBDA>
//__forceinline void EnqueueUniqueRenderCommand(LAMBDA&& Lambda)
//{
//	//if (Lambda)
//	{
//		Lambda();
//	}
//}
//
//#define ENQUEUE_RENDER_COMMAND(Type) \
//	struct Type##Name \
//	{  \
//		static const char* CStr() { return #Type; } \
//		static const TCHAR* TStr() { return TEXT(#Type); } \
//	}; \
//	EnqueueUniqueRenderCommand<Type##Name>
//
//template<typename LAMBDA>
//__forceinline void EnqueueUniqueRenderCommand(LAMBDA& Lambda)
//{
//	static_assert(sizeof(LAMBDA) == 0, "EnqueueUniqueRenderCommand enforces use of rvalue and therefore move to avoid an extra copy of the Lambda");
//}