//////////////////////////////////////////////////////////////////////////
// study class layout
// time: 2017/04/21
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
using namespace std;

namespace ClassFragments{
	class Point3d{
	public:
		virtual ~Point3d();
		//...
	//protected:
		static Point3d origin;
		float x;
		float y;
		float z;
	};

	struct Base1{int val1;};
	struct Base2{int val2;};
	struct Derived : Base1, Base2 {};

	inline void func1(int Derived::*dmp, Derived *pd)
	{
		pd->*dmp;
	}

	inline void func2(Derived *pd)
	{
		int Base2::*bmp = &Base2::val2;
		func1(bmp, pd);
	}
}