#pragma once
#include "../utils/types.h"

void square_init();
void square_render(Vec2f pos, Vec2f camera_pos,
		U32 size, U32 texture, bool calc_orgin);