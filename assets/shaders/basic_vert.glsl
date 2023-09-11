#version 330 core
layout(location=0) in vec2 a_pos;

uniform mat4 u_mvp;

layout (location = 1) in vec2 a_text_cord;

out vec2 text_cord;

void main()
{
    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
    text_cord = a_text_cord;
}