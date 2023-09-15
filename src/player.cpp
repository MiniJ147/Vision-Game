#include "player.h"
#include "utils/utils.h"
#include "utils/debug.h"
#include "utils/u_math.h"
#include "utils/collision.h"

#include "global.h"


void collision_check_x(int dir, Vec2f* pos, Map m, float move_speed, float delta_time)
{
    //pos->x += move_speed * delta_time;
    int t_y, b_y;
    int goal_cell_x;
    float player_goal_x;
    t_y = floor(pos->y);
    b_y = ceil(pos->y) + PLAYER_SIZE;

    t_y -= t_y % BLOCK_SIZE;
    b_y -= b_y % BLOCK_SIZE;

    //calculating the cell they are attempting to pass into===|
    switch (dir)
    {
        //left
    case 0:
        goal_cell_x = (int)pos->x + ((int)pos->x % BLOCK_SIZE)-BLOCK_SIZE;
        player_goal_x = pos->x - ( move_speed * delta_time);
        break;
        //right
    case 1:
        goal_cell_x = (int)pos->x + (BLOCK_SIZE - ((int)pos->x % BLOCK_SIZE));
        player_goal_x = pos->x + move_speed * delta_time;
        break;
    }

    //std::cout << goal_cell_x << std::endl;
    Vec2f player_goal_pos = { player_goal_x, pos->y };

    //=========================================================|

    if (t_y == b_y) //if we are prefectly lined up and we dont have to check top and bottom 
    {
        Block goal_block = map_get_block(m, goal_cell_x / BLOCK_SIZE, t_y / BLOCK_SIZE,true);
        if ((goal_block.flags & 0b1) == 0b1 || !collision_AABB(player_goal_pos, PLAYER_SIZE, goal_block.pos, BLOCK_SIZE)) {
            pos->x = player_goal_x;
            global_get()->camera_x_offset = calculate_orgin(*pos, PLAYER_SIZE).x;
        }
    }
    else //if we have to check the cell up and down due to use colliding with two different cells
    {
        //0 UPPER | 1 Lower
        Block blocks[2] = { map_get_block(m, goal_cell_x / BLOCK_SIZE, t_y / BLOCK_SIZE,true),
                            map_get_block(m, goal_cell_x / BLOCK_SIZE, b_y / BLOCK_SIZE,true) };
        //std::cout << blocks[0].pos.x << " " << blocks[0].pos.y << std::endl;
        //std::cout << blocks[1].pos.x << " " << blocks[1].pos.y << std::endl;

        //if(b1 walkable || b1 does not collide && b2 walkwable || b2 does not collide) move;
        if ((blocks[0].flags & 0b1) == 0b1 || !collision_AABB(player_goal_pos, PLAYER_SIZE, blocks[0].pos, BLOCK_SIZE)){
            if ((blocks[1].flags & 0b1) == 0b1 || !collision_AABB(player_goal_pos, PLAYER_SIZE, blocks[1].pos, BLOCK_SIZE)) {
                pos->x = player_goal_x;
                global_get()->camera_x_offset = calculate_orgin(*pos, PLAYER_SIZE).x;
            }
        }
    }
}
void collision_check_y(int dir, Vec2f* pos, Map m, float move_speed, float delta_time)
{
    float player_goal_y;
    int l_x, r_x, goal_block_y;

    //calculating the cell they are attempting to pass into===|
    switch (dir)
    {
        //up
    case 0:
        player_goal_y = pos->y - move_speed * delta_time;
        goal_block_y = (pos->y + (int)pos->y % BLOCK_SIZE) - BLOCK_SIZE;
        break;
        //down
    case 1:
        player_goal_y = pos->y + move_speed * delta_time;
        goal_block_y = (pos->y + (BLOCK_SIZE-(int)pos->y % BLOCK_SIZE));
        break;
    }

    l_x = floor(pos->x);
    r_x = ceil(pos->x) + PLAYER_SIZE;

    l_x -= l_x % BLOCK_SIZE;
    r_x -= r_x % BLOCK_SIZE;

    //================================|

    Vec2f player_goal_pos = { pos->x, player_goal_y};

    if (l_x == r_x)//if not colliding with mutiple cells then only check one
    {
        Block b = map_get_block(m, l_x / BLOCK_SIZE, goal_block_y / BLOCK_SIZE,true);
        //std::cout << pos->y << std::endl;
        if ((b.flags & 0b1) == 0b1 || !collision_AABB(player_goal_pos, PLAYER_SIZE, b.pos, BLOCK_SIZE)) {
            pos->y = player_goal_y;
            global_get()->camera_y_offset = calculate_orgin(*pos, PLAYER_SIZE).y;
        }
    }
    else //if colliding with mutiple cells check both 
    {
        //0 left 1 right
        Block blocks[2] = { map_get_block(m, l_x / BLOCK_SIZE, goal_block_y / BLOCK_SIZE,true),
                        map_get_block(m, r_x / BLOCK_SIZE, goal_block_y / BLOCK_SIZE,true) };
        //std::cout << blocks[0].pos.x << " " << blocks[0].pos.y << std::endl;
        //std::cout << blocks[1].pos.x << " " << blocks[1].pos.y << std::endl;

        //if(b1 walkable || b1 does not collide && b2 walkwable || b2 does not collide) move;
        if ((blocks[0].flags & 0b1) == 0b1 || !collision_AABB(player_goal_pos, PLAYER_SIZE, blocks[0].pos, BLOCK_SIZE)) {
            if ((blocks[1].flags & 0b1) == 0b1 || !collision_AABB(player_goal_pos, PLAYER_SIZE, blocks[1].pos, BLOCK_SIZE)) {
                pos->y = player_goal_y;
                global_get()->camera_y_offset = calculate_orgin(*pos, PLAYER_SIZE).y;
            }
        }
    }
}

void player_create(Player* player, float x, float y, const char* texture_path)
{
    //initlizing our data
    player->pos = Vec2f{ x,y };
    player->orgin = calculate_orgin(player->pos, PLAYER_SIZE);
    
    global_get()->camera_x_offset = player->orgin.x;
    global_get()->camera_y_offset = player->orgin.y;
    std::cout << x << " " << y << std::endl;

    texture_create(&player->texture, 1, texture_path);
}

void player_tick(Vec2f* pos,Map m, float move_speed, float delta_time)
{
    //movement checks
    if (check_key_press(global_get()->window, GLFW_KEY_W)) collision_check_y(0, pos, m, move_speed, delta_time);
    if (check_key_press(global_get()->window, GLFW_KEY_S)) collision_check_y(1, pos, m, move_speed, delta_time);
    if (check_key_press(global_get()->window, GLFW_KEY_A)) collision_check_x(0, pos, m, move_speed, delta_time);
    if (check_key_press(global_get()->window, GLFW_KEY_D)) collision_check_x(1, pos, m, move_speed, delta_time);
}
