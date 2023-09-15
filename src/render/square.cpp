#include "square.h"
#include <glad/glad.h>
#include "Shader.h"
#include "Buffer.h"
#include "texture/texture.h"
#include "../utils/u_math.h"
#include "../utils/debug.h"

#include "../global.h"


//inilizing our inital data we need to render
static bool init = false;

static Buffer* vb = (Buffer*)malloc(sizeof(Buffer));
static Buffer* ib = (Buffer*)malloc(sizeof(Buffer));
static U32 shader;
static U32 vao;

static float vertices[] = {
     -0.5f,  0.5f, 0.0f,0.0f,
      0.5f,  0.5f, 1.0f,0.0f,
     -0.5f, -0.5f, 0.0f,1.0f,
      0.5f, -0.5f, 1.0f, 1.0f
};

static unsigned int indices[] = {
     0,1,2,
     1,2,3
};

//=============================================

void square_init()
{
    if (init) { LOG("Already Init Square Returning...\n"); return; }

    //adjust_factor_x = (SIZE / 2) - (global_get()->window_width/2);
    //adjust_factor_y = (global_get()->window_height / 2) - (SIZE / 2);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    buffer_init(vb, GL_ARRAY_BUFFER, 1, sizeof(vertices), vertices, GL_STATIC_DRAW);
    buffer_bind(vb->type, vb->buffer);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glEnableVertexAttribArray(0);

    buffer_init(ib, GL_ELEMENT_ARRAY_BUFFER, 1, sizeof(indices), indices, GL_STATIC_DRAW);
    buffer_bind(ib->type, ib->buffer);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    //TODO WHEN WE HAD SHADERS MAKE IT SO WE CAN HAVE UNQUIE ONES
    char* pv = shader_file_parse("assets/shaders/basic_vert.glsl");
    char* pf = shader_file_parse("assets/shaders/basic_frag.glsl");

    shader = shader_create(pv, pf);

    init = true;
}
                                                         //we will need this when implementing unique shaders
void square_render(Vec2f pos, Vec2f camera_pos, U32 size, /*U32 shader,*/ U32 texture, bool calc_orgin)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 mvp(1.0f);

    glBindVertexArray(vao);
    buffer_bind(vb->type, vb->buffer);
    buffer_bind(ib->type, ib->buffer);

    glUseProgram(shader);

    Vec2f render_pos = pos;
    if(calc_orgin) render_pos = calculate_orgin(pos, size);

    view = glm::translate(view, glm::vec3(render_pos.x - camera_pos.x,
        -render_pos.y + camera_pos.y, 0.f));

    model = glm::scale(model, glm::vec3(size,size, 0.0f));

    mvp = global_get()->projection * view * model;

    texture_bind(texture);

    shader_uniform_set_mat4(shader, "u_mvp", mvp);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}