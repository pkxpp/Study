#pragma once
#include <iostream>
using namespace std;
/*
 * 编译DX11的Effects11的时候，我升级到vs2019，结果编译报错
 * D:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Samples\C++\Effects11\Inc\d3dxGlobal.h
 * 这是代码
static void* __cdecl operator new(size_t s, CDataBlockStore &pAllocator)
{
    D3DXASSERT( s <= 0xffffffff );
    return pAllocator.Allocate( (UINT)s );
}

static void __cdecl operator delete(void* p, CDataBlockStore &pAllocator)
{
}
 * 报错：effects11 d3dxGlobal.h(667,22): error C2323: 'operator new': non-member operator new or delete functions may not be declared static or in a namespace other than the global namespace
 * 参考：
 * (1)https://stackoverflow.com/questions/3623631/where-should-non-member-operator-overloads-be-placed
 * (2)[Namespaces and the Interface Principle](http://www.gotw.ca/publications/mill08.htm)
 */

// 参考[2] The Interface Principle Revisited
 // in some library header
namespace N { 
    class C {
        
    }; 
    int operator+(int i, N::C) { return i + 1; }
}

// error C2323: 'operator new': non-member operator new or delete functions may not be declared static or in a namespace other than the global namespace
//static void* operator new(size_t size, std::string s)
//{
//    return 0;
//}




// error C2672: 'operator __surrogate_func': no matching overloaded function found
//int operator+(int i, N::C) { return i + 1; }

// a mainline to exercise it
#include <numeric>

// 参考[2]:Digression: Recap of a Familiar Inheritance Issue
// Example 2a: Hiding a name
//             from a base class
//
struct B {
    int f(int) {
        cout << "B::f(int)" << endl;
        return 0;
    }
    int f(double) {
        cout << "B::f(double)" << endl;
        return 0;
    }
    int g(int)
    {
        cout << "B::g(int)" << endl;
        return 0;
    }
};

struct D : public B {
    using B::g;
private:
    int g(std::string, bool)
    {
        cout << "D::g(string, bool)" << endl;
        return 0;
    }
};

void TestNamespaceAndInterfacePrinciple()
{
    //The Interface Principle Revisited
    N::C a[10];
    std::accumulate(a, a + 10, 0);


    // Digression: Recap of a Familiar Inheritance Issue
    D   d;
    int i;
    d.f(i);    // ok, means B::f(int)
    //d.g(i);    // error C2660: 'D::g': function does not take 1 arguments
    // 参考[2]:  In short, when we declare a function named g in the derived class D, it automatically hides all functions with the same name in all direct and indirect base classes.
    // 才发现D::g是private = =
    // 参考[2]，编译规则
    // (1)First, it looks in the immediate scope, in this case the scope of class D, and makes a list of all functions it can find that are named g (regardless of whether they're accessible or even take the right number of parameters). 
    // (2)Only if it doesn does it then continue "outward" into the next enclosing scope and repeat--in this case, the scope of the base class B
    // (3)--until eventually it either runs out of scopes without having found a function with the right name or else finds a scope that contains at least one candidate function. 
    // (4)If a scope is found that has one or more candidate functions, the compiler then stops searching and works with the candidates that it's found, 
    // (5)performing overload resolution and applying access rules.

    // 正确用法(1)
    //d.B::g(i); // ok, asks for B::g(int)
    // 正确用法(2) using B::g;
    d.g(i);
}
