#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "Shader.h"


class Mesh
{
public:
	Mesh();
	~Mesh();
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	void draw();
	void setupMesh();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;


private:
	unsigned int vao, vbo, ebo, vaoSize;
};

