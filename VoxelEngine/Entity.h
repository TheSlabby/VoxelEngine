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
	// getters & setters
	glm::vec3 getPosition();
	void setPosition(const glm::vec3& pos);

	void attemptMove(const glm::vec3& dir, double dt);
	void attemptMove(const glm::vec3& moveDir, const glm::vec3& frontVector, double dt); // give camera front vector


	// update entity on every render
	void update(double dt);


private:
	glm::vec3 position;
	glm::vec3 velocity;
};

