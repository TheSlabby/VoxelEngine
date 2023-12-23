#pragma once

#include <glad/glad.h>
#include "Mesh.h"
#include "Block.h"
#include "Texture.h"
#include "Camera.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <iostream>
#include <memory>

class Application
{
public:
	Application();
	~Application();

	Mesh mesh;
	Shader shader;
	Texture texture;

	void print(const char* str);
	static void sprint(const char* str);

	void run();



private:
	bool initialize();
	void mainLoop();
	void cleanup();

	void processInput();

	GLFWwindow* window;
};

