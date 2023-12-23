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

    void loadFace(Face face, std::vector<Mesh::Vertex>& vertices);
    void loadAllFaces(std::vector<Mesh::Vertex>& vertices);

    glm::vec3 position;

    uint8_t ID;


private:
    int faceToIndex(Face face);

};

