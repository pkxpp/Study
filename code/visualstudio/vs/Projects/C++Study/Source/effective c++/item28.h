#pragma once

// 测试 只能指针的隐式转换问题
// test auto ptr implicit transfer

#include <iostream>
using namespace std;

namespace effective_study {
	template<class T>
	class SmartPtr {
	public:
		SmartPtr(T* realPtr = 0);

		SmartPtr(const SmartPtr& rhs);
		~SmartPtr();

		SmartPtr& operator=(const SmartPtr& rhs);

		T* operator->() const;
		T& operator*() const;

		//////////////////////////////////////////////////////////////////////////
		// 以下是隐式类型转换，不建议
		// implicit transfer as below, don't advice
		// ！！！注意隐式类型转换的写法
		// notice: the format
		operator void*();
	private:
		T* pointee;
	};

	template<class T>
	SmartPtr<T>::SmartPtr(T* realPtr)
		: pointee(realPtr)
	{
	}

	template<class T>
	SmartPtr<T>::SmartPtr(const SmartPtr& rhs)
	{
		pointee = rhs.pointee;

		rhs.pointee = 0;
	}

	template<class T>
	SmartPtr<T>::~SmartPtr()
	{
		delete pointee;
		pointee = 0;
	}

	template<class T>
	SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr& rhs)
	{
		if (this == &rhs)
			return *this;

		delete pointee;

		pointee = rhs.pointee;
		rhs.pointee = 0;

		return *this;
	}

	template<class T>
	T* SmartPtr<T>::operator->() const
	{
		return pointee;
	}

	template<class T>
	T& SmartPtr<T>::operator*() const
	{
		return *pointee;
	}

	template<class T>
	SmartPtr<T>::operator void *()
	{
		return (void*)pointee;
	}

	//////////////////////////////////////////////////////////////////////////
	class TestSmartPtr {
	public:
		TestSmartPtr(int a) : m_nNum(a) {}
		~TestSmartPtr() {}

		void PrintNumber() {
			cout << "num = " << m_nNum << endl;
		}

	private:
		int m_nNum;
	};
}
