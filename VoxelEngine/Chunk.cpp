#include "Chunk.h"

Chunk::Chunk(glm::vec2 position) : position(position) {
	
}

void Chunk::loadMesh() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				//test all faces (occlusion culling)
				//std::cout << "CHUNKS::Render ID: " << blocks[x][y][z].ID << std::endl;
				if (x + 1 >= CHUNK_SIZE || blocks[x + 1][y][z].ID == 255)
					blocks[x][y][z].loadFace(Block::RIGHT, vertices);
				if (x - 1 < 0 || blocks[x - 1][y][z].ID == 255)
					blocks[x][y][z].loadFace(Block::LEFT, vertices);
				if (y + 1 >= CHUNK_HEIGHT || blocks[x][y + 1][z].ID == 255)
					blocks[x][y][z].loadFace(Block::TOP, vertices);
				if (y - 1 < 0 || blocks[x][y - 1][z].ID == 255)
					blocks[x][y][z].loadFace(Block::BOTTOM, vertices);
				if (z + 1 >= CHUNK_SIZE || blocks[x][y][z + 1].ID == 255)
					blocks[x][y][z].loadFace(Block::BACK, vertices);
				if (z - 1 < 0 || blocks[x][y][z - 1].ID == 255)
					blocks[x][y][z].loadFace(Block::FRONT, vertices);
			}
		}
	}

	//and setupMesh too
	setupMesh();
}

//use noise to generate blocks in chunk
void Chunk::loadBlocks() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				blocks[x][y][z].ID = 1;
				//set position with chunk offset
				if (y > (sin(x/5.0f) + sin(z/5.0)) * 8 + 20)
					blocks[x][y][z].ID = 255;
				blocks[x][y][z].position = glm::vec3(position.x * CHUNK_SIZE + x, y, position.y * CHUNK_SIZE + z);
			}
		}
	}
}