//////////////////////////////////////////////////////////////////////////
// Macro study
// pk
// 2013/05/23
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdio>
#include <climits>

using namespace std;

//1. 使用#把宏参数变为一个字符串,用##把两个宏参数贴合在一起.
#define  STR(s) #s
#define  CONS(a, b) int(a##e##b)

//2.需要注意的是凡宏定义里有用'#'或'##'的地方宏参数是不会再展开.
#define  TWO (2)
#define MUL(a,b) (a*b)
#define  A (2)
//#define STR(s) #s
//#define CONS
/************************************************************************/
/* summary *
INT_MAX和A都不会再被展开, 然而解决这个问题的方法很简单. 加多一层中间转换宏.
加这层宏的用意是把所有宏的参数在这层里全部展开, 那么在转换宏里的那一个宏(_STR)就能得到正确的宏参数.
#define A           (2)
#define _STR(s)     #s
#define STR(s)      _STR(s)          // 转换宏
#define _CONS(a,b)  int(a##e##b)
#define CONS(a,b)   _CONS(a,b)       // 转换宏
printf("int max: %s/n", STR(INT_MAX));          // INT_MAX,int型的最大值，为一个变量 ＃i nclude<climits>
输出为: int max: 0x7fffffff
STR(INT_MAX) -->  _STR(0x7fffffff) 然后再转换成字符串；
printf("%d/n", CONS(A, A));
输出为：200
CONS(A, A)  -->  _CONS((2), (2))  --> int((2)e(2))
*/
/************************************************************************/

int main(){
	//1
	printf(STR(vck));
	printf("%d\n", CONS(2,3));	//2e3 = 2000

	//2
	printf("%d*%d = %d\n", TWO, TWO, MUL(TWO, TWO));
	printf("int max:%s\n", STR(INT_MAX));		//INT_MAX #include<climits>
	//printf("%s\n", CONS(A, A));					//Error	1	error C2065: 'AeA' : undeclared identifier


	getchar();
	return 0;
}