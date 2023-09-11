#pragma once
#include <vector>
#include "../utils/types.h"

const int BLOCK_SIZE = 64;

struct Block
{
	Vec2f pos;
	int flags;
};

struct Map
{
	//if I want to add removeablity add IDS
	//for interactables add a function to grab a block 
	//the add an interactable flag to see if we can interact with it
	std::vector<Vec2f> pos;
	std::vector<U32> texture_ids;
	std::vector<int> flags;
	U32 dimension[2];
	bool map_loaded = false;
	bool textures_loaded = false;
};

void map_load(Map* m, float* player_x, float* player_y, 
	float* monster_x, float* monster_y, const char* path);

void map_load_texture(Map* m, const char* load_path);
void map_render(Map m, Vec2f cam_pos);

//testing
void map_change_texture(Map* m, int texture_index,
	int x, int y, bool adjusted);

Block map_get_block(Map m, int x, int y, bool adjusted);