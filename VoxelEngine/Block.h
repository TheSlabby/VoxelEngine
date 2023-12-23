#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"
#include <iostream>
#include <unordered_map>

class Block
{
public:
    enum Face {
        TOP = 0x01, // 0000 0001
        BOTTOM = 0x02, // 0000 0010
        LEFT = 0x04, // 0000 0100
        RIGHT = 0x08, // 0000 1000
        FRONT = 0x10, // 0001 0000
        BACK = 0x20  // 0010 0000
    };


    Block(glm::vec3 pos, uint8_t ID);
    Block(uint8_t ID);
    Block();

    void loadFace(Face face, std::vector<Mesh::Vertex>& vertices);
    void loadAllFaces(std::vector<Mesh::Vertex>& vertices);

    glm::vec3 position;

    uint8_t ID;

    static void Initialize();
    static glm::vec2 getTexCoords(uint8_t pos);


private:
    int faceToIndex(Face face);


    struct BlockTexture {
        glm::vec2 topTexture;
        glm::vec2 sideTexture;
        glm::vec2 bottomTexture;
    };
    static std::unordered_map<Block::Face, glm::vec3> blockToVecMapping;
    static std::unordered_map<uint8_t, BlockTexture> blockTextures;
};

