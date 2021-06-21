#include "KVector2.h"

KVec2 KVec2::ZeroVector(0.f, 0.f);

void UVDXtoOpengl(KVec2& v)
{
    v.y = 1.0f - v.y;
}
