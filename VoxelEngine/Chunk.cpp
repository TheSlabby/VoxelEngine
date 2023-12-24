#include "Chunk.h"

Chunk::Chunk(glm::vec2 position) : position(position) {
	
}

void Chunk::loadMesh() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				glm::vec3 pos(x + position.x * CHUNK_SIZE, y, z + position.y * CHUNK_SIZE);
				//test all faces (occlusion culling)
				//std::cout << "CHUNKS::Render ID: " << blocks[x][y][z].ID << std::endl;
				if (x + 1 >= CHUNK_SIZE || blocks[x + 1][y][z] == 255)
					Block::loadFace(blocks[x][y][z], pos, Block::RIGHT, vertices, indices);
				if (x - 1 < 0 || blocks[x - 1][y][z] == 255)
					Block::loadFace(blocks[x][y][z], pos, Block::LEFT, vertices, indices);
				if (y + 1 >= CHUNK_HEIGHT || blocks[x][y + 1][z] == 255)
					Block::loadFace(blocks[x][y][z], pos, Block::TOP, vertices, indices);
				if (y - 1 < 0 || blocks[x][y - 1][z] == 255)
					Block::loadFace(blocks[x][y][z], pos, Block::BOTTOM, vertices, indices);
				if (z + 1 >= CHUNK_SIZE || blocks[x][y][z + 1] == 255)
					Block::loadFace(blocks[x][y][z], pos, Block::BACK, vertices, indices);
				if (z - 1 < 0 || blocks[x][y][z - 1] == 255)
					Block::loadFace(blocks[x][y][z], pos, Block::FRONT, vertices, indices);
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
				int actualX = x + CHUNK_SIZE * position.x;
				int actualY = z + CHUNK_SIZE * position.y;

				blocks[x][y][z] = generateTerrain(actualX, y, actualY);
			}
		}
	}
}

uint8_t Chunk::generateTerrain(int x, int y, int z) {
	double noise = perlin.octave2D_01((x * 0.01), (z * 0.01), 4);
	noise = pow(noise, 5);
	int height = noise * 50;
	if (y > height) {
		return 255;
	}
	else if (y == height) {
		//snow
		int variance = ((float)rand() / RAND_MAX) * 7; //between 0-7
		if (y > 20 + variance)
			return 3;	//snow
		return 0;		//grass
	}
	else {
		return 1;		//dirt
	}
}