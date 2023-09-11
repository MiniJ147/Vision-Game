#include "monster.h"
#include "../utils/debug.h"
#include "../global.h"

#include<cstdlib>

static float move_speed = 200.f;
static int path_index = 0;

//testing
bool run_alg = false;

void state_switch(States* curr_state, States new_state)
{
    *curr_state = new_state;
}

void monster_create(Monster* m, Vec2f* player_pos, float x, float y, const char* text_path)
{
    m->pos = Vec2f{ x,y };
    m->player_pos = player_pos;
    m->curr_state = States::ROAMING;

    //pathfinding_init(&m->a_star, x,y, 64,64);
    texture_create(&m->texture, 1, text_path);
}

int move_to_pos(Vec2f* pos, Vec2f goal, const float dt)
{
    //todo add buffer
    //move x
    int buffer_x = 6;
    //right
    if (pos->x < goal.x + buffer_x) { pos->x += move_speed * dt; return 0; };
    if (pos->x + MONSTER_SIZE > (goal.x + BLOCK_SIZE) - buffer_x) { pos->x -= move_speed * dt; return 0; }
  
    //move y
    int buffer_y = 6;
    if (pos->y < goal.y + buffer_y) { pos->y += move_speed * dt; return 0; }
    if (pos->y + MONSTER_SIZE > goal.y + BLOCK_SIZE - buffer_y) { pos->y -= move_speed * dt; return 0; }
   
    return 1;
}

//complex logic gets stored in here
void monster_roaming(int timer, Vec2f* pos, Block* goal, std::vector<Vec2f>& path, const float dt)
{
    if (timer==0 || run_alg)
    {
        path.clear();

        srand(time(0));

        // Get a random number
        int r_block_x = rand() % global_get()->map->dimension[0];
        int r_block_y = rand() % global_get()->map->dimension[1];

        Block b = map_get_block(*global_get()->map, r_block_x, r_block_y, true);
        if ((b.flags & 0b1) == 0b1)
        {
            LOG("FOUND VAILED BLOCK");
            *goal = b;

            A_Star pf;
            pathfinding_init(&pf, (int)pos->x, (int)pos->y, (int)b.pos.x, (int)b.pos.y);
            pathfinding_step(&pf, pf.start);

            if (pf.path_found) { path = pathfinding_traceback(pf); run_alg = false; }
        }
    }

    if (path_index < path.size()) path_index += move_to_pos(pos, path[path_index], dt);
    else if (path_index >= path.size()) //reseting goal pos once we have reached the end
    {
        path_index = 0;
        run_alg = true;
    }
}

//watch out may have to switch to pointer 
//m -> monster
//p -> player
void monster_hunting(std::vector<Vec2f>& path, int timer, Vec2f* m_pos, Vec2f p_pos, const float dt)
{
    //testing
    int reset_time = 300;
    if (timer == 0 || timer % reset_time == 0 || run_alg) {
        path.clear();
        path_index = 0;

        A_Star pf;
        pathfinding_init(&pf, (int)m_pos->x,(int)m_pos->y, (int)p_pos.x, p_pos.y);
        pathfinding_step(&pf, pf.start);

        if (pf.path_found) { path = pathfinding_traceback(pf); run_alg = false; }//LOG("TRACE BACK"); run_alg = false; };
    }

    if (path_index < path.size())
    {
        path_index += move_to_pos(m_pos, path[path_index], dt);
    }
    else if (path_index == path.size()) {
        run_alg = true;
        path_index = 0;
    }
}

void monster_tick(Monster* m, const float dt)
{
    //state machine logic
    switch (m->curr_state)
    {
    case States::ROAMING:
        if (m->timer > m->max_roam) {
            m->timer = 0;
            m->path.clear();

            run_alg = false;
            state_switch(&m->curr_state, States::HUNTING);
            break;
        } 
        monster_roaming(m->timer,&m->pos, &m->roam_block_goal,m->path, dt);
        m->timer++;
        break;


    case States::HUNTING:
        if (m->timer > m->max_hunt) {
            m->timer = 0;

            m->path.clear();
            path_index = 0;

            run_alg = false;
            state_switch(&m->curr_state, States::ROAMING);
            break;
        }
        monster_hunting(m->path, m->timer, &m->pos,*m->player_pos, dt);

        m->timer++;
        break;
    default: //if we get some random state set it to idle
        state_switch(&m->curr_state, States::ROAMING);
        break;
    }
}