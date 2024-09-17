#include "Chunk.h"
#include <thread>

Chunk::Chunk(glm::vec2 position) : position(position) {
}
Chunk::~Chunk() {
	std::cout << "Chunk destroyed: " << this << std::endl;
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

void Chunk::loadAsync() {
	//std::thread t(&Chunk::loadBlocks, &this);
	std::thread([this] {
		this->loadBlocks();
		//std::cout << "New thread" << std::endl;
		this->renderReady = true;
	}).detach();
}

uint8_t Chunk::generateTerrain(int x, int y, int z) {
	double noise = perlin.octave2D_01((x * 0.01), (z * 0.01), 4);
	noise = pow(noise, 3);
	int height = noise * 70;
	if (y > height) {
		return BLOCK_AIR;
	}
	else if (y == height) {
		//snow
		int variance = ((float)rand() / RAND_MAX) * 7; //between 0-7
		if (y > 20 + variance)
			return BLOCK_SNOW;	//snow
		return BLOCK_GRASS;		//grass
	}
	else {
		return BLOCK_DIRT;		//dirt
	}
}