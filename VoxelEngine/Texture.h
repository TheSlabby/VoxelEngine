#pragma once

#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Texture
{
public:
	Texture();
	~Texture();

	bool load(const char* path);
	unsigned int textureID;

private:
};

