#pragma once
#include "item27.h"
#include "item28.h"

//////////////////////////////////////////////////////////////////////////
// test function

namespace effective_study {
	void TestNewHeapOrStack()
	{
		// 作为成员变量，operate new的私有属性失效了
		Asset* pa = new Asset(100);
	}


	void TestSmartPtrFunction()
	{
		auto t = new TestSmartPtr(5);
		auto_ptr<TestSmartPtr> ptn(t);

		// error C2678: 二进制“==”: 没有找到接受“std::auto_ptr<effective_study::TestSmartPtr>”类型的左操作数的运算符(或没有可接受的转换)
		//if (ptn == 0)
		//{
		//	cout << "null pointer" << endl;
		//}

		// error C2451: “std::auto_ptr<effective_study::TestSmartPtr>”类型的条件表达式是非法的
		//if (ptn)
		//{
		//	cout << "not null pointer" << endl;
		//}
		
		// error C2675 : 一元“!” : “std::auto_ptr<effective_study::TestSmartPtr>”不定义该运算符或到预定义运算符可接收类型的转换
		//if (!ptn)
		//{
		//	cout << "null pointer" << endl;
		//}
		ptn->PrintNumber();	// ok

		t = new TestSmartPtr(6);
		SmartPtr<TestSmartPtr> ptnSelf(t);
		ptnSelf->PrintNumber();

		// OK，隐式转换之前和上面一样的错误
		if (ptnSelf == 0)
		{
			cout << "null pointer" << endl;
		}

		// OK，隐式转换之前和上面一样的错误
		if (ptnSelf)
		{
			cout << "not null pointer" << endl;
		}

		// OK，隐式转换之前和上面一样的错误
		if (!ptnSelf)
		{
			cout << "null pointer" << endl;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void TestEffectiveStudy()
	{
		//TestNewHeapOrStack();
		TestSmartPtrFunction();
	}

	
	


}

