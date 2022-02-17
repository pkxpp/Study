#include "CastStudy.h"
#include <stdint.h>
#include <iostream>

void CastStudy::TestVoidPointerStaticCastFailed()
{
	int nID = 10000;
	void* pID = &nID;

	uint64_t uCastIDDirectly = (uint64_t)(pID); // ok

	//uint64_t uCastID = static_cast<uint64_t>(pID); // error C2440: “static_cast”: 无法从“void *”转换为“uint64_t”
	uint64_t uCastID = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(pID)); // ok



	// https://en.cppreference.com/w/cpp/language/static_cast
	// 10.A prvalue of type pointer to void (possibly cv-qualified) can be converted to pointer to any object type.
	//uintptr_t pCastID = static_cast<uintptr_t>(pID); // error C2440: “static_cast”: 无法从“void *”转换为“uintptr_t”
	// 看定义：typedef unsigned __int64  uintptr_t;
	uint64_t* pCastID0 = static_cast<uint64_t*>(pID);
}

// https://www.codeproject.com/Articles/12935/What-static-cast-is-actually-doing
void CastStudy::TestStaticCastAndReinterpreCastOfGenericTypes()
{
	float f = 12.3;
	float* pf = &f;

	// static cast<>
	// OK, n = 12
	int n = static_cast<int>(f);

	// Error, types pointed to are unrelated
	//int* pn = static_cast<int*>(pf);
	int* pn0 = (int*)pf;
	std::cout << *pn0 << std::endl;

	// OK
	void* pv = static_cast<void*>(pf);
	// OK, but *pn2 is rubbish
	int* pn2 = static_cast<int*>(pv);

	// reinterpret_cast<>
	// Error, the compiler know you should
	// call static_cast<>
	//int i = reinterpret_cast<int>(f);
	int j = (int)f;

	// OK, but *pn is actually rubbish, same as *pn2
	int* pi = reinterpret_cast<int*>(pf);


}