#include<stdio.h>
#include<iostream>
using namespace std;

class Base{
	int a;
public:
	virtual void fun(){
		printf("base fun\n");
	}
	void fun2(){
		printf("base fun\n");
	}
};


int main(){
	Base *b=NULL;
	b->fun();
	b->fun2();

	getchar();
	return 0;
}