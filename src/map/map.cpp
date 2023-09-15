#include "map.h"
#include <iostream>
#include <string>
#include <fstream>
#include "../utils/debug.h"
#include "../render/texture/texture.h"
#include "../render/square.h"

static std::vector<Texture> loaded_textures;

//texture order index list
#define WALL 0
#define TILE 1
		//1
//flags 0x0
//1 = walkable

void map_load(Map* m, float* player_x, float* player_y, float* monster_x, float* monster_y, 
		const char* path)
{
	if (m->map_loaded) { LOG("Tried loading map twice returning...\n"); return; };
	std::string output;
	// Read from the text file
	std::ifstream map_file(path);
	float x = 0;
	float y = 0;

	//reading size
	for (int i = 0; i < 2; ++i)
	{
		getline(map_file,output);
		m->dimension[i] = std::stoi(output);
	}

	while (getline(map_file, output)) {
		x = 0;
		for (char c : output)
		{
			switch (c)
			{
			case '#': //wall size: 64, 64
				m->pos.push_back(Vec2f{x,y});
				m->texture_ids.push_back(WALL);
				m->flags.push_back(0b0);
				break;
			case '.':
				m->pos.push_back(Vec2f{ x,y });
				m->texture_ids.push_back(TILE);
				m->flags.push_back(0b1);
				break;
			case 'p':
				//telling player where he will be spawned so we do not have conflicting headers
				*player_x = x;
				*player_y = y;
				m->pos.push_back(Vec2f{ x,y });
				m->texture_ids.push_back(TILE);
				m->flags.push_back(0b1);
				break;
			case 'm':
				*monster_x = x;
				*monster_y = y;

				m->pos.push_back(Vec2f{ x,y });
				m->texture_ids.push_back(TILE);
				m->flags.push_back(0b1);
				break;
			}
			x += 64;
		}
		y += 64;
	}
	// Close the file
	map_file.close();
	m->map_loaded = true;
}

//loads all map textures we will need for rendering
void map_load_texture(Map* m, const char* load_path)
{
	if (m->textures_loaded) { LOG("Error tried loading textures twice returning...\n"); return; }

	std::string texture_path;

	std::ifstream load_file(load_path);

	int i = 0;

	while (getline(load_file, texture_path))
	{
		loaded_textures.push_back(0);
		texture_create(&loaded_textures[i], 1, texture_path.c_str());
		i++;
	}

	m->textures_loaded = true;
	load_file.close();
}

void map_render(Map m, Vec2f cam_pos)
{
	for (int i = 0; i < m.pos.size(); ++i)
	{
		square_render(m.pos[i], cam_pos, BLOCK_SIZE, loaded_textures[m.texture_ids[i]],true);
		//block_render(m.pos[i], loaded_textures[m.texture_ids[i]]);
	}
}

void map_change_texture(Map* m, int texture_index, int x, int y, bool adjusted)
{
	if (!adjusted)
	{
		x -= x % 64;
		y -= y % 64;

		x = x / 64;
		y = y / 64;
	}

	int i = x + (y * m->dimension[0]);
	
	if (i >= m->texture_ids.size())
	{
		LOG("ERROR: REACHING OUT OF TEXTURE INDEX VECTOR\n");
		return;
	}

	m->texture_ids[i] = texture_index;
}

//adjusted is so we dont have to have snapped to the grid cords
Block map_get_block(Map m, int x, int y, bool adjusted)
{
	if (!adjusted)
	{
		x -= x % 64;
		y -= y % 64;

		x = x / 64;
		y = y / 64;
	}
	int i = x + (y * m.dimension[0]);
	if (i >= m.pos.size())
	{
		LOG("ERROR: REACHING OUT OF MAP (MAP_GET_BLOCK)\n");
		return Block{0,0};
	}

	return Block{ m.pos[i],m.flags[i] };
}
