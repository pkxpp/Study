#include<stdio.h>
#include<iostream>

#include "delete_parent_pointer.h"
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
	//Base *b=NULL;
	//b->fun();
	//b->fun2();

	TestDeleteParentPointer();

	getchar();
	return 0;
}