#version 330 core
out vec4 FragColor;

in vec2 text_cord;

uniform sampler2D _texture;

void main()
{
    FragColor = texture(_texture,text_cord);
} 