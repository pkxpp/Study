#include "KVec2.h"

KVec2Test KVec2Test::ZeroVector(0.f, 0.f);

void UVDXtoOpengl(KVec2Test& v)
{
    v.y = 1.0f - v.y;
}
