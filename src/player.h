#pragma once
#include "utils/types.h"
#include "render/texture/texture.h"
#include "map/map.h"

static const int PLAYER_SIZE = 48;

struct Player
{
	Vec2f pos;
	Vec2f orgin;
	float move_speed = 275.f;
	Texture texture;
};

void player_create(Player* player, float x, float y,
	const char* texture_path);

void player_tick(Vec2f* pos, Map m, 
	float move_speed, float delta_time);