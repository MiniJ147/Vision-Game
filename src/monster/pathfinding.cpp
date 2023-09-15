#include "pathfinding.h"	
#include "../utils/u_math.h"
#include "../utils/debug.h"
#include "../global.h"

#define MAX_STEPS 150

#define PRINT_RESULTS_MODE 0
#if PRINT_RESULTS_MODE == 1
#define PRINT_RESULTS(t,x,y,c) map_change_texture(global_get()->map,t,x,y,c)
#else
#define PRINT_RESULTS(t,x,y,c)
#endif 


void pathfinding_init(A_Star* alg, int start_x, int start_y, int goal_x, int goal_y)
{
	//adjusting float values
	start_x -= (start_x % 64);
	start_y -= (start_y % 64);

	goal_x -= (goal_x % 64);
	goal_y -= (goal_y % 64);

	alg->start = Node{start_x, start_y};
	alg->start.is_start = true;
	alg->closed.push_back(alg->start);

	alg->goal = Node{goal_x,goal_y};
	alg->goal.is_goal = true;
	alg->closed.push_back(alg->goal);

	//ERROR [Watch out for giving it a nullptr]
	alg->map = *global_get()->map;

	//map_change_texture(global_get()->map, 4, start_x, start_y, false);
	PRINT_RESULTS(4, start_x, start_y, false);
	//map_change_texture(global_get()->map, 5, goal_x, goal_y, false);
	PRINT_RESULTS(5, goal_x, goal_y, false);
}

void init_node(Node* n, int x, int y, int p_x, int p_y, float f, float g, float h, bool collapsed)
{
	n->x = x;
	n->y = y;
	n->parent_x = p_x;
	n->parent_y = p_y;
	n->f = f;
	n->g = g;
	n->h = h;
	n->is_collasped = collapsed;
}

Node get_node(int x, int y, const std::vector<Node>& open, const std::vector<Node>& closed)
{
	for (Node n : open)
	{
		if (n.x == x && n.y == y)
			return n;
	}
	for (Node n : closed)
	{
		if (n.x == x && n.y == y)
			return n;
	}

	return Node{ -1,-1 };
}

void node_print(Node n)
{
	//LOG("X:%d Y:%d P_X:%d P_Y:%d F:%f G:%f H:%f\n", n.x, n.y, n.parent_x, n.parent_y, n.f, n.g, n.h);
}

void pathfinding_check_dir(A_Star* alg, Node org, int x, int y)
{
	Block b = map_get_block(alg->map, (org.x / 64) + x, (org.y / 64) + y, true);

	if ((b.flags & 0b1) == 0b0) return; //{ //LOG("NODE BLOCKED\n"); return;}

	//checking if node is vaild
	Node n = get_node(b.pos.x,b.pos.y,alg->open,alg->closed);
	if (n.is_goal) { // found goal
		//LOG("Found goal\n");
		alg->path_found = true;
		alg->goal.parent_x = org.x;
		alg->goal.parent_y = org.y;
		return;
	}
	else if (n.is_start) {
		//LOG("Found Start\n");
		return;
	}
	else if (n.x != -1) {
		//LOG("Node already explored\n");
		return;
	}
	
	//PRINT_RESULTS(2, b.pos.x, b.pos.y, false);

	//distance from start to current
	float g = calculate_hypo(org.x - b.pos.x, org.y - b.pos.y);
	//distance form current to end
	float h = calculate_hypo(b.pos.x - alg->goal.x, b.pos.y - alg->goal.y);
	
	float f = g + h;

	init_node(&n, b.pos.x, b.pos.y, org.x, org.y, f, g, h, false);
	node_print(n);
	alg->open.push_back(n);
}

void pathfinding_step(A_Star* alg, Node curr)
{
	if (alg->curr_step >= MAX_STEPS) { LOG("Reached max amount of steps"); return; }

	node_print(curr);

	//checking grid for path
	pathfinding_check_dir(alg, curr, 1, 0);//right
	pathfinding_check_dir(alg, curr, -1, 0);//left
	pathfinding_check_dir(alg, curr, 0, -1);//up
	pathfinding_check_dir(alg, curr, 0, 1);//down

	if (alg->path_found)
	{
		return;
	}

	//finding lowest f value
	float lowest_f = 1000000.f;
	int lowest_f_i = -1;
	for (int i=0; i<alg->open.size();++i)
	{
		Node n = alg->open[i];
		if (n.f < lowest_f) { lowest_f = n.f; lowest_f_i = i; };
	}
	//=======================|

	//error checking
	if (lowest_f_i == -1)
	{
		//LOG("Could not find lowest f cost\n");
		return;
	}

	//creating the new node to search for next step==|
	Node new_n = alg->open[lowest_f_i];
	new_n.is_collasped = true;

	alg->closed.push_back(new_n);
	
	alg->open.erase(alg->open.begin() + lowest_f_i);

	//PRINT_RESULTS(3, new_n.x, new_n.y, false);
	alg->curr_step++;

	pathfinding_step(alg, new_n);
}

std::vector<Vec2f> pathfinding_traceback(A_Star alg)
{
	if (!alg.path_found) { LOG("PATH NOT FOUND RETURNING"); return {};}

	std::vector<Vec2f> path;
	
	path.push_back(Vec2f{(float)alg.goal.x,(float)alg.goal.y});
	Node n = get_node(alg.goal.parent_x, alg.goal.parent_y, alg.open, alg.closed);

	while (!n.is_start)
	{
		path.push_back(Vec2f{ (float)n.x,(float)n.y });
		//PRINT_RESULTS(4, n.x, n.y, false);
		n = get_node(n.parent_x, n.parent_y, alg.open, alg.closed);
	}
	int i = 0;
	int j = path.size() - 1;

	//reversing order
	while (j > i)
	{
		Vec2f temp = path[i];
		path[i] = path[j];
		path[j] = temp;
		i++;
		j--;
	}

	return path;
}
