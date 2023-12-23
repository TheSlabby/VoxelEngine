#pragma once

#include "Mesh.h"
#include "Block.h"
#include "PerlinNoise.hpp"

class Chunk :
    public Mesh
{
public:
    //constants
    static constexpr int CHUNK_SIZE = 16;
    static constexpr int CHUNK_HEIGHT = 64;
    static constexpr int CHUNK_RENDER_RADIUS = 4;
    static constexpr int SEED = 123456u;

    //generation
    siv::PerlinNoise perlin{ SEED };
    uint8_t generateTerrain(int x, int y, int z);

    Chunk(glm::vec2 position);


    unsigned char blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    glm::vec2 position;

    void loadMesh();    //setup vertices
    void loadBlocks();  //use procedural generation for this part
};

