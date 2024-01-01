#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"
#include <iostream>
#include <unordered_map>


//BLOCK ID MACROS
#define BLOCK_AIR 255
#define BLOCK_GRASS 0
#define BLOCK_DIRT 1
#define BLOCK_STONE 2
#define BLOCK_SNOW 3


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

    static void loadFace(uint8_t ID, glm::vec3 position, Face face, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices);
    static void loadAllFaces(uint8_t ID, glm::vec3 position, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices);

    glm::vec3 position;
    uint8_t ID;

    static void Initialize();
    static glm::vec2 getTexCoords(uint8_t pos);


private:
    static int faceToIndex(Face face);


    struct BlockTexture {
        glm::vec2 topTexture;
        glm::vec2 sideTexture;
        glm::vec2 bottomTexture;
    };
    static std::unordered_map<Block::Face, glm::vec3> blockToVecMapping;
    static std::unordered_map<uint8_t, BlockTexture> blockTextures;
};

