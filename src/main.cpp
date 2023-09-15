#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/utils.h"
#include "utils/debug.h"
#include "global.h"
#include "player.h"
#include "monster/monster.h"
#include "render/square.h"

//TODO: when resizing the window reestablish the window width and height so we can re adjust it
const U32 SCR_WIDTH = 1024;
const U32 SCR_HEIGHT = 1024;

float aspect = SCR_WIDTH / SCR_HEIGHT;
float half_height = SCR_HEIGHT / 2;
float half_width = half_height * aspect;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
#pragma region GLFWINIT
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "game", NULL, NULL);
    if (window == NULL)
    {
        LOG("Failed to create GLFW window")
        glfwTerminate();
        return -1;
    }

    global_get()->window = window;
    global_get()->window_width = SCR_WIDTH;
    global_get()->window_height = SCR_HEIGHT;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG("Failed to initialize GLAD")
        return -1;
    }

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::ortho(-half_width, half_width, -half_height, half_height, 0.0001f, 3000.f);
    global_get()->projection = projection;
#pragma endregion
    square_init();

    Player player;
    float p_spawn_x, p_spawn_y;

    Monster monster;
    float m_spawn_x, m_spawn_y;

    Map map;

    //loading maps
    map_load(&map, &p_spawn_x, &p_spawn_y, &m_spawn_x, &m_spawn_y, "assets/maps/stage.map");
    map_load_texture(&map, "assets/maps/load_order.map");
    global_get()->map = &map;
    //=============

    //creating our player and monster
    player_create(&player, p_spawn_x, p_spawn_y, "assets/textures/player.jpg");
    monster_create(&monster, &player.pos,m_spawn_x, m_spawn_y, "assets/textures/monster.jpg");
    //==============

    float delta_time = 0.0f;
    float last_frame = 0.0f; 

    while (!glfwWindowShouldClose(window))
    {
        Vec2f cam_pos = Vec2f{ global_get()->camera_x_offset,global_get()->camera_y_offset };

        //delta time
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        //---------|

        glClearColor(0.7, 0.7, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);


        //ticks
        player_tick(&player.pos, map, player.move_speed,delta_time);
        monster_tick(&monster,delta_time);
        //=======


        //rendering========
        map_render(map, cam_pos); //map render
        square_render({}, {}, PLAYER_SIZE, player.texture, false); //player render
        square_render(monster.pos, cam_pos, MONSTER_SIZE, monster.texture, true); //monster render
        //=================

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	return 0;
}