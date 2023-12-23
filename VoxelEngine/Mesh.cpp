#include "Mesh.h"


Mesh::Mesh() {
    std::vector<Vertex> v;
    vertices = v;
}
Mesh::Mesh(const std::vector<Vertex>& vertices) : vertices(vertices) {
}

void Mesh::setupMesh() {
    // Generate and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);

    //free memory
    vaoSize = vertices.size();
    std::vector<Vertex>().swap(vertices);
}

void Mesh::draw() {
    // Bind the VAO
    glBindVertexArray(vao);

    // Draw the mesh using vertices only
    glDrawArrays(GL_TRIANGLES, 0, vaoSize);

    // Unbind the VAO
    glBindVertexArray(0);
}
