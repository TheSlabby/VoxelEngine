#pragma once

#include "Mesh.h"
#include "Block.h"

class Chunk :
    public Mesh
{
public:
    //constants
    static constexpr int CHUNK_SIZE = 16;
    static constexpr int CHUNK_HEIGHT = 32;
    static constexpr int CHUNK_RENDER_RADIUS = 0;

    Chunk(glm::vec2 position);


    Block blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    glm::vec2 position;

    void loadMesh();    //setup vertices
    void loadBlocks();  //use procedural generation for this part
};

