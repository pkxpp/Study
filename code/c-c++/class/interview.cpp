#include <stdio.h>

class A{
public:
	void fun() {printf("A");}
};

class B : public A{
public:
	virtual void fun(){printf("B");}
};

class C : public B{
public:
	void fun() {printf("C");}
};

class D : public C{
public:
	virtual void fun() {printf("D");}
	virtual void funD() {printf("D_Test");}
};

class E : public D {
public:
	virtual void fun(){printf("E");}
	virtual void funE(){printf("E_Test");}
};

class E1 : public D{
public:
	virtual void fun(){printf("E1");}
	virtual void funE1(){printf("E1_Test");}
};

int main(){
	A* p1 = (A*)new B; p1->fun();printf(", ");
	B* p2 = (B*)new C; p2->fun(); printf(", ");
	B* p3 = (B*)new D; p3->fun(); printf(", ");
	B* p4 = (E*)new D; p4->fun(); printf(", ");
	D* p5 = (D*)new E; p5->funD(); printf(", ");
	E1* p6 = (E1*)new E; p6->funE1(); printf(", ");
	p6->funD(); printf(", ");
	printf("%d, %d\n", sizeof(B) - sizeof(A), sizeof(D) - sizeof(A));
	///////////////////////////////////////
	// address
	printf("&A::fun: %x\n", &A::fun);
	void (A::*ptrA_fun)();
	//ptrA_fun = (p1->fun);
	//printf("&p1::fun: %x\n", (void*)p1->fun);
	void (B::*ptrB_fun)();
	ptrB_fun = &B::fun;
	printf("%x\n", ptrB_fun);
	(p2->*ptrB_fun)();printf("\n");
	// &E1::funE1: 9. offset(9)=funE1;offset(0)=fun;offset(5)=funD;
	printf("&E1::funE1: %x\n", &E1::funE1);
	printf("&E1::funD: %x\n", &E1::funD);
	printf("address......................\n");
	printf("p1-A*-B; p2-B*-C; p3-B*-D; p4-B*-D; p5-D*-E; p6-E1*-E\n");
	// new B test
	B b;
	printf("p1: %x\n", p1);
	printf("&b: %x\n", &b);
	printf("p2: %x\n", p2);
	printf("p3: %x\n", p3);
	printf("p4: %x\n", p4);
	printf("p5: %x\n", p5);
	printf("p6: %x\n", p6);
	printf("\n");
	printf("===========vritual address=========\n");
	printf("vtable p1: %x\n", *((int*)p1));
	printf("vtable b: %x\n", *((int*)&b));
	printf("vtable p2: %x\n", *((int*)p2));
	printf("vtable p3: %x\n", *((int*)p3));
	printf("vtable p4: %x\n", *((int*)p4));
	printf("vtable p5: %x\n", *((int*)p5));
	printf("vtable p6: %x\n", *((int*)p6));
	printf("p1 virtual fun: %x\n", *(int*)*((int*)p1));
	printf("b virtual fun: %x\n", *(int*)*((int*)&b));
	printf("p2 virtual fun: %x\n", *(int*)*((int*)p2));
	printf("p3 virtual fun: %x\n", *(int*)*((int*)p3));
	printf("p4 virtual fun: %x\n", *(int*)*((int*)p4));
	printf("p5 virtual fun 1: %x, offset: %x\n", *(int*)*((int*)p5), &E::fun);
	printf("p5 virtual fun 2: %x, offset: %x\n", *((int*)*((int*)p5)+1), &E::funD);
	printf("p5 virtual fun 3: %x, offset: %x\n", *((int*)*((int*)p5)+2), &E::funE);
	printf("p6 virtual fun 1: %x, offset: %x\n", *(int*)*((int*)p6), &E1::fun);
	printf("p6 virtual fun 2: %x, offset: %x\n", *((int*)*((int*)p6)+1), &E1::funD);
	printf("p6 virtual fun 3: %x, offset: %x\n", *((int*)*((int*)p6)+2), &E1::funE1);
	typedef void (*Fun)();
	Fun pA = (Fun)*((int*)*((int*)p1));
	printf("p1->fun(): ");pA();printf("\n");
	Fun pB = (Fun)*((int*)*((int*)p2));
	printf("p2->fun(): ");pB();printf("\n");
	Fun pBFun = (Fun)*((int*)*((int*)&b));
	printf("b.fun(): ");pBFun();printf("\n");
	Fun p3Fun = (Fun)*((int*)*((int*)p3));
	printf("p3->fun(): ");p3Fun();printf("\n");
	Fun p4Fun = (Fun)*((int*)*((int*)p4));
	printf("p4->fun(): ");p4Fun();printf("\n");
	Fun p5Fun = (Fun)*((int*)*((int*)p5));
	printf("p5->fun(): ");p5Fun();printf("\n");
	Fun p5Fun1 = (Fun)*((int*)*((int*)p5)+1);
	printf("p5->fun1(): ");p5Fun1();printf("\n");
	Fun p5Fun2 = (Fun)*((int*)*((int*)p5)+2);
	printf("p5->fun2(): ");p5Fun2();printf("\n");
	// cant call, but exist.
	//p5->funE();	//error: 'class D' has no member named 'funE'
	Fun p6Fun = (Fun)*((int*)*((int*)p6));
	printf("p6->fun(): ");p6Fun();printf("\n");
	Fun p6Fun1 = (Fun)*((int*)*((int*)p6)+1);
	printf("p6->fun1(): ");p6Fun1();printf("\n");
	Fun p6Fun2 = (Fun)*((int*)*((int*)p6)+2);
	printf("p6->fun2(): ");p6Fun2();printf("\n");
	printf("\n");
	///////////////////////////////////////
	/*
	// my test
	// 1.class
	printf("&A::fun: %X\n", &A::fun);
	printf("&B::fun: %X\n", &B::fun);
	// 2.object
	typedef void (A::*ptrA_fun)();
	typedef void (B::*ptrB_fun)();
	ptrA_fun ptrA = &A::fun;
	ptrB_fun ptrB = &B::fun;
	//printf("&A::fun: %X\n", &p1->fun);
	printf("&A::fun: %X\n", ptrA);
	printf("&A::fun: %X\n", *((int*)&ptrA));
	printf("&B::fun: %X\n", ptrB);
	//printf("&A::fun: %X\n", &p1->fun);
	printf("&B::fun: %X\n", *((int*)&ptrB));
	//printf("&A::fun: %x, &B::fun: %x, &C::fun: %x, &D::fun: %x, %E::fun:%x, %E1::fun: %x\n", 
	//	A::fun, &(B::fun), &(C::fun), &(D::fun), &(E::fun), &(E1::fun));

	*/

	///////////////////////////////////////
	// 
}
