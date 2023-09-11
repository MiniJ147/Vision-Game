#pragma once
#include "types.h"

inline bool collision_AABB(Vec2f pos_a, float w_a, Vec2f pos_b, float w_b)
{
    if (pos_a.x < pos_b.x + w_b &&
        pos_a.x + w_a > pos_b.x &&
        pos_a.y < pos_b.y + w_b &&
        pos_a.y + w_a > pos_b.y) return true;
	return false;
}