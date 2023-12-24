#pragma once

#include <glad/glad.h>
#include "Mesh.h"
#include "Block.h"
#include "Texture.h"
#include "Camera.h"
#include "Chunk.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <memory>
#include <glm/ext/matrix_transform.hpp>

class Application
{
public:
	Application();
	~Application();

	Shader shader;
	Shader shadowShader;
	Texture texture;

	//chunks
	std::vector<Chunk> chunks;

	bool isMouseLocked = true; // Initial state - mouse is locked

	void print(const char* str);
	static void sprint(const char* str);

	void run();
	void checkErrors();



private:
	bool initialize();
	void mainLoop();
	void cleanup();

	void processInput(double dt);

	GLFWwindow* window;
};

