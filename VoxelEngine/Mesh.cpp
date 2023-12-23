#include "Mesh.h"

Mesh::Mesh() {
    // Initializer can be empty if you're not setting up anything by default
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices) {
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);  // Generate EBO

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);  // Bind EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex attribute pointers remain the same
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Free memory
    vaoSize = indices.size();  // Now using the size of the indices
    std::vector<Vertex>().swap(vertices);
    std::vector<unsigned int>().swap(indices);  // Clear index data from memory
}

void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, vaoSize, GL_UNSIGNED_INT, 0);  // Use glDrawElements
    glBindVertexArray(0);
}