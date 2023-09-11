#pragma once
#include "types.h"

inline Vec2f calculate_orgin(Vec2f pos, float size)
{
    return Vec2f{ pos.x + (size / 2),pos.y + (size / 2) };
}

inline float calculate_hypo(int a, int b)
{
    return sqrt(pow(a, 2) + pow(b, 2));
}