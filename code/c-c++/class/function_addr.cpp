#include<iostream> 
using namespace std; 
class A 
{ 
public: 
    A(int); 
    void fun1(); 
    virtual void fun2(); 
    static void fun3(); 
    int num1; 
    static int num2; 
}; 
A::A(int i) 
{ 
    num1=i; 
} 
void A::fun1() 
{ 
    cout<<"I am in fun1"<<endl; 
} 
void A::fun2() 
{ 
    cout<<"I am in fun2"<<endl; 
} 
void A::fun3() 
{ 
    cout<<"I am in fun3"<<endl; 
} 
int A::num2=1; 
void main() 
{ 
    A a(2); 
    //获取静态成员数据的地址 
    int *ptr_static=&A::num2; 
    cout<<"静态成员数据的地址"<<ptr_static<<endl; 
    ptr_static=&a.num2; 
    cout<<"a.num2静态成员数据的地址"<<ptr_static<<endl; 
    //获取静态函数的地址 
    void (*ptr_staticfun)=A::fun3; 
    cout<<"静态成员函数的地址"<<ptr_staticfun<<endl; 
    ptr_staticfun=a.fun3; 
    cout<<"a.fun3静态成员函数的地址"<<ptr_staticfun<<endl; 
    //获取普通成员函数的地址 
    typedef void (A::*ptr_commomfun)(); 
    ptr_commomfun ptr=A::fun1; 
    cout<<"普通成员函数的地址"<<ptr<<endl; //如果直接输出ptr的话，输出来的是1，因此应该把ptr地址中的内容读出来 
    cout<<"普通成员函数的地址"<<*((long*)&ptr)<<endl; 
    ptr=a.fun1; 
    cout<<"a.fun1普通成员函数的地址"<<*((long*)&ptr)<<endl; 
    ptr_commomfun ptr_virtual=A::fun2; //获取虚函数的地址 
    cout<<"虚成员函数的地址"<<*((long*)&ptr_virtual)<<endl; 
    ptr_virtual=a.fun2; 
    cout<<"a.fun2虚成员函数的地址"<<*((long*)&ptr_virtual)<<endl; 
  
    int *p; 
    int A::*q; 
    q=&A::num1; 
    //指向数据成员的指针赋予的是一个目前还不存在的一个类成员的地址，而这个地址只有在使用实际类对象进行调用时才会真正的确定下来 
    //就是说在类还没有对象时候，成员变量时没有空间的。 
    cout<<"普通成员数据的地址"<<*((long *)&q)<<endl; 
    p=&a.num1; 
    cout<<"a.num2普通成员数据的地址"<<p<<endl; 
}
