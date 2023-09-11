#pragma once
#include "../render/texture/texture.h"
#include "../utils/types.h"
#include "pathfinding.h"

const int MONSTER_SIZE = 48;

enum class States
{
	ROAMING = 0,
	HUNTING = 1
};

struct Monster
{
	Vec2f pos;
	Vec2f* player_pos; //may want to put in global if gets annyoing
	Texture texture;
	States curr_state;

	std::vector<Vec2f> path;
	Block roam_block_goal = { Vec2f{-1,-1} };
	
	//timing
	int timer = 0;
	const int max_roam = 100000;
	const int max_hunt = 0;

	//A_Star a_star;
};
void monster_create(Monster* m, Vec2f* player_pos, float x, float y, const char* text_path);

//make this more DOD later
void monster_tick(Monster* m, const float dt);