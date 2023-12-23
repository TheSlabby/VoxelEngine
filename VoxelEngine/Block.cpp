#include "Block.h"

const float CELL_SIZE = 1.0f/16.0f;
struct BlockTexture {
    glm::vec2 topTexture;
    glm::vec2 sideTexture;
    glm::vec2 bottomTexture;
};
std::unordered_map<uint8_t, BlockTexture> blockTextures;


glm::vec3 unitVertices[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)
};


std::unordered_map<Block::Face, glm::vec3> blockToVecMapping;

// Define the vertex indices for each face (two triangles per face)
unsigned int faceVertices[6][6] = {
    // TOP (3, 2, 6, 3, 6, 7)
    {3, 2, 6, 3, 6, 7},
    // BOTTOM (1, 0, 4, 1, 4, 5)
    {1, 0, 4, 1, 4, 5},
    // LEFT (0, 3, 7, 0, 7, 4)
    {0, 3, 7, 0, 7, 4},
    // RIGHT (2, 1, 5, 2, 5, 6)
    {2, 1, 5, 2, 5, 6},
    // FRONT (0, 1, 2, 0, 2, 3)
    {0, 1, 2, 0, 2, 3},
    // BACK (5, 4, 7, 5, 7, 6)
    {5, 4, 7, 5, 7, 6}
};

int Block::faceToIndex(Block::Face face) {
    switch (face) {
        case Block::TOP: return 0;
        case Block::BOTTOM: return 1;
        case Block::LEFT: return 2;
        case Block::RIGHT: return 3;
        case Block::FRONT: return 4;
        case Block::BACK: return 5;
        default: return -1; // Or some error code
    }

}

Block::Block(glm::vec3 pos, uint8_t ID) : position(pos), ID(ID) {
    //probably shouldnt put it here but whatever
    blockToVecMapping[Block::TOP] = glm::vec3(0, 1, 0);
    blockToVecMapping[Block::BOTTOM] = glm::vec3(0, -1, 0);
    blockToVecMapping[Block::FRONT] = glm::vec3(0, 0, -1);
    blockToVecMapping[Block::BACK] = glm::vec3(0, 0, 1);
    blockToVecMapping[Block::RIGHT] = glm::vec3(1, 0, 0);
    blockToVecMapping[Block::LEFT] = glm::vec3(-1, 0, 0);

    //populate textures
    blockTextures[0] = { glm::vec2(0.0f, 0.0f), glm::vec2(3*CELL_SIZE, 0), glm::vec2(2*CELL_SIZE, 0)}; // Grass

}

void Block::loadFace(Face face, std::vector<Mesh::Vertex>& vertices) {
    glm::vec2 offset = blockTextures[ID].topTexture;

    //texture
    glm::vec2 uvTopLeft = offset + glm::vec2(0.0f);
    glm::vec2 uvTopRight = offset + glm::vec2(CELL_SIZE, 0.0f);
    glm::vec2 uvBottomLeft = offset + glm::vec2(0.0f, CELL_SIZE);
    glm::vec2 uvBottomRight = offset + glm::vec2(CELL_SIZE, CELL_SIZE);

    // Define the UV coordinates for each vertex of a quad (2 triangles)
    glm::vec2 uvCoordinates[6] = {
        uvTopLeft, uvBottomLeft, uvBottomRight, // First triangle
        uvTopLeft, uvBottomRight, uvTopRight    // Second triangle
    };

    // Append the vertices for the specified face
    for (int i = 0; i < 6; i++) {
        unsigned int vIndex = faceVertices[faceToIndex(face)][i];
        Mesh::Vertex v;
        v.position = unitVertices[vIndex] + position;
        v.normal = blockToVecMapping[face];
        v.texCoords = uvCoordinates[i];
        std::cout << uvCoordinates[i].x << std::endl;
        vertices.push_back(v);
    }
}

void Block::loadAllFaces(std::vector<Mesh::Vertex>& vertices) {
    Block::loadFace(Block::TOP, vertices);
    Block::loadFace(Block::BOTTOM, vertices);
    Block::loadFace(Block::LEFT, vertices);
    Block::loadFace(Block::RIGHT, vertices);
    Block::loadFace(Block::FRONT, vertices);
    Block::loadFace(Block::BACK, vertices);
}