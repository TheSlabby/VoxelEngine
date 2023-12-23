#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "Shader.h"


class Mesh
{
public:
	Mesh();
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	Mesh(const std::vector<Vertex>& vertices);
	void draw();
	void setupMesh();

	std::vector<Vertex> vertices;


private:
	unsigned int vao, vbo, vaoSize;
};

