#pragma once
#include <iostream>
#include <vector>
#include "../utils/types.h"
#include "../map/map.h"


struct Node
{
	//g distance from curr pos to start
	//h distance from curr pos to end
	//f is g+h
	int x, y;
	int parent_x, parent_y;
	float f, g, h;
	// goal start closed
	bool is_goal = false;
	bool is_start = false;
	bool is_collasped = false;
};

struct A_Star
{
	Node start;
	Node goal;
	Map map;

	int curr_step = 0;

	std::vector<Node> open, closed;
	bool path_found = false;
};

void pathfinding_init(A_Star* alg, int start_x, int start_y, int goal_x, int goal_y);
void pathfinding_step(A_Star* alg, Node curr);
std::vector<Vec2f> pathfinding_traceback(A_Star alg);