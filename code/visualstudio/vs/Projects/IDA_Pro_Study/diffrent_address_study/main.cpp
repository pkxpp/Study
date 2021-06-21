#include<iostream>
#include<stdio.h>
using namespace std;

class Base {

public:
	virtual void f() { cout << "Base::f" << endl; }

	virtual void g() { cout << "Base::g" << endl; }

	virtual void h() { cout << "Base::h" << endl; }

};

typedef void(Base::* Fun)(void);
Fun pFun = NULL;

int main()
{
	Base b;
	b.f();

	//printf("通过虚函数表获取的函数地址：\n");
	////printf("%p\n", (int*)*(int*)*(int*)(&b));

	//printf("\n通过函数指针赋值获取的函数地址：\n");
	//pFun = &Base::f;
	//printf("%p\n\n", pFun);

	/*cout << "sizeof(int)		= " << sizeof(int) << endl
		<< "sizeof(long)		= " << sizeof(long) << endl
		<< "sizeof(int*)		= " << sizeof(int*) << endl
		<< "sizeof(long*)		= " << sizeof(long*) << endl
		<< "sizeof(intptr_t)	= " << sizeof(intptr_t) << endl;*/
	//typedef int CUSTOM_TYPE ;
	//printf("%p\n", (CUSTOM_TYPE*)*(CUSTOM_TYPE*)*(CUSTOM_TYPE*)(&b));

	// 32位用int类型，64位需要换成long，结果sizeof(long) = 4， 地址的话换成int*即可，sizeof(int*) = 8
	typedef long CUSTOM_TYPE;
	typedef intptr_t* CUSTOM_PTR_TYPE;
	//typedef long* CUSTOM_PTR_TYPE;

	long** pVtab = (long**)&b;
	//printf("%p\n", pVtab[0][0]);
	//printf("&b = %p\n", &b);
	//printf("(CUSTOM_PTR_TYPE)(&b) = %p\n", (CUSTOM_PTR_TYPE)(&b));
	//printf("*(CUSTOM_PTR_TYPE)(&b) = %p\n", *(CUSTOM_PTR_TYPE)(&b));
	//printf("(CUSTOM_PTR_TYPE)*(CUSTOM_PTR_TYPE)(&b) = %p\n", (CUSTOM_PTR_TYPE)*(CUSTOM_PTR_TYPE)(&b));
	//printf("*(CUSTOM_PTR_TYPE)*(CUSTOM_PTR_TYPE)(&b) = %p\n", *(CUSTOM_PTR_TYPE)*(CUSTOM_PTR_TYPE)(&b));

	printf("%p\n", (CUSTOM_PTR_TYPE)*(CUSTOM_PTR_TYPE)*(CUSTOM_PTR_TYPE)(&b));

	pFun = &Base::f;
	printf("%p\n", pFun);

	getchar();
	return 0;
}