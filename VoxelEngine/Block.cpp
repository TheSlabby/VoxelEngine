#include "Block.h"

//canvas is a square
const int CANVAS_SIZE = 16;
const float CELL_SIZE = 1.0f/CANVAS_SIZE;



glm::vec3 unitVertices[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)
};


std::unordered_map<Block::Face, glm::vec3> Block::blockToVecMapping;
std::unordered_map<uint8_t, Block::BlockTexture> Block::blockTextures;
void Block::Initialize() {
    blockToVecMapping[Block::TOP] = glm::vec3(0, 1, 0);
    blockToVecMapping[Block::BOTTOM] = glm::vec3(0, -1, 0);
    blockToVecMapping[Block::FRONT] = glm::vec3(0, 0, -1);
    blockToVecMapping[Block::BACK] = glm::vec3(0, 0, 1);
    blockToVecMapping[Block::RIGHT] = glm::vec3(1, 0, 0);
    blockToVecMapping[Block::LEFT] = glm::vec3(-1, 0, 0);

    //populate textures
    blockTextures[0] = { getTexCoords(0), getTexCoords(3), getTexCoords(2) }; // Grass
}
glm::vec2 Block::getTexCoords(uint8_t pos) {
    int x = pos % CANVAS_SIZE;
    int y = pos / CANVAS_SIZE;
    return glm::vec2(x * CELL_SIZE, y * CELL_SIZE);
}

// Define the vertex indices for each face (two triangles per face)
unsigned int faceVertices[6][4] = {
    // TOP (3, 2, 6, 7)
    {3, 2, 6, 7},
    // BOTTOM (1, 0, 4, 5)
    {1, 0, 4, 5},
    // LEFT (0, 3, 7, 4)
    {0, 3, 7, 4},
    // RIGHT (2, 1, 5, 6)
    {2, 1, 5, 6},
    // FRONT (0, 1, 2, 3)
    {0, 1, 2, 3},
    // BACK (5, 4, 7, 6)
    {5, 4, 7, 6}
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
    
}
Block::Block(uint8_t ID) : ID(ID) {

}
Block::Block() {
    ID = 255; //air
}

void Block::loadFace(uint8_t ID, glm::vec3 position, Face face, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices) {
    if (ID == 255) // Air block check
        return;

    glm::vec2 offset = glm::vec2(0.0f);
    if (faceToIndex(face) == 0) {
        offset = blockTextures[ID].topTexture;
    }
    else if (faceToIndex(face) == 1) {
        offset = blockTextures[ID].bottomTexture;
    }
    else {
        offset = blockTextures[ID].sideTexture;
    }

    glm::vec2 uvCoordinates[4] = {
        offset + glm::vec2(0.0f, CELL_SIZE),         // Bottom Left
        offset + glm::vec2(CELL_SIZE, CELL_SIZE),   // Bottom Right
        offset + glm::vec2(CELL_SIZE, 0.0f),         // Top Right
        offset + glm::vec2(0.0f, 0.0f)              // Top Left
    };

    unsigned int indexOffset = vertices.size();
    // Append vertices for the specified face
    for (int i = 0; i < 4; i++) {
        unsigned int vIndex = faceVertices[faceToIndex(face)][i];
        Mesh::Vertex v;
        v.position = unitVertices[vIndex] + position;
        v.normal = blockToVecMapping[face];
        v.texCoords = uvCoordinates[i];
        vertices.push_back(v);
    }

    // Append indices for the specified face (two triangles)
    indices.push_back(indexOffset);
    indices.push_back(indexOffset + 1);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset + 3);
}

void Block::loadAllFaces(uint8_t ID, glm::vec3 position, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices) {
    Block::loadFace(ID, position, Block::TOP, vertices, indices);
    Block::loadFace(ID, position, Block::BOTTOM, vertices, indices);
    Block::loadFace(ID, position, Block::LEFT, vertices, indices);
    Block::loadFace(ID, position, Block::RIGHT, vertices, indices);
    Block::loadFace(ID, position, Block::FRONT, vertices, indices);
    Block::loadFace(ID, position, Block::BACK, vertices, indices);
}