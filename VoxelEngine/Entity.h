#pragma once

#include <glad/glad.h>
#include "Mesh.h"
#include "Block.h"
#include "Texture.h"
#include "Camera.h"
#include "Chunk.h"
#include "Client.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <memory>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>


class Entity
{
public:
	// constants
	static const int MOVE_SPEED = 10;
	static const int JUMP_POWER = 10;
	static const int GRAVITY = 30;
	static const int ENTITY_HEIGHT = 2;


	// getters & setters
	glm::vec3 getPosition();
	void setPosition(const glm::vec3& pos);

	void attemptMove(const glm::vec3& moveDir, float yaw, std::vector<std::unique_ptr<Chunk>>& chunks, double dt); // give camera front vector

	void impulse(const glm::vec3& dir);
	void jump();


	// update entity on every render
	void update(std::vector<std::unique_ptr<Chunk>>& chunks, double dt);


private:
	glm::vec3 position;
	glm::vec3 velocity;

	bool onGround;
};

