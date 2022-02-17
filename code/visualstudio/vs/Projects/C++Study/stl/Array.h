#ifndef ARRAY_H
#define ARRAY_H

#include "PsArray.h"
//#include "PsInlineArray.h"
//#include "PsHashMap.h"
#include "PsAllocator.h"
//#include "Test.h"
//#include "PxNameSpaceMangle.h"

using namespace physx;

template<typename T>
class MyArray : public Ps::Array<T, Ps::RawAllocator>
{
public:
	PX_INLINE explicit MyArray() : Ps::Array<T, Ps::RawAllocator>() {}
	PX_INLINE explicit MyArray(int size, const T& a = T()) : Ps::Array<T, Ps::RawAllocator>(size, a) {}
};


template<typename T, int N>
class MyInlineArray : public Ps::InlineArray<T, N, Ps::RawAllocator> {};


#endif

