#pragma once

typedef unsigned int Texture;

void texture_create(Texture* texture, int count, const char* image_path);
void texture_bind(Texture texture);