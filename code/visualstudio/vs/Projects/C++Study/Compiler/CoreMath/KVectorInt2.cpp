#pragma once

#include "KVectorInt2.h"
#include "KVec2.h"

KVectorInt2 KVectorInt2::ZeroVector(0, 0);

int KVectorInt2::Distance(const KVectorInt2& other) const
{
	return KMathTest::Abs(other.x - x) + KMathTest::Abs(other.y - y);
}

