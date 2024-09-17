#include "Entity.h"


glm::vec3 Entity::getPosition() {
	return this->position;
}
void Entity::setPosition(const glm::vec3& pos) {
	this->position = pos;
}

void Entity::impulse(const glm::vec3& dir) {
	this->velocity = dir;
}


uint8_t blockAt(glm::vec3 goal, std::vector<std::unique_ptr<Chunk>>& chunks) {
	int x = static_cast<int>(std::round(goal.x - .5));
	int y = goal.y;
	int z = static_cast<int>(std::round(goal.z - .5));

	int xChunkIndex = std::floor((float)x / Chunk::CHUNK_SIZE);
	int zChunkIndex = std::floor((float)z / Chunk::CHUNK_SIZE);
	// get block near me
	int block = 255;
	for (auto& chunk : chunks) {
		if (y >= chunk->CHUNK_HEIGHT)
			break;
		int chunkX = chunk->position.x;
		int chunkZ = chunk->position.y;
		if (xChunkIndex == chunkX && zChunkIndex == chunkZ) {
			int localX = x - xChunkIndex * Chunk::CHUNK_SIZE;
			int localZ = z - zChunkIndex * Chunk::CHUNK_SIZE;
			int localY = y;
			block = chunk->blocks[localX][localY][localZ];
			break;
		}
	}

	// if air, we can move here
	return block;
}
// render loop
void Entity::update(std::vector<std::unique_ptr<Chunk>>& chunks, double dt) {
	// gravity
	this->velocity += glm::vec3(0, -GRAVITY * dt, 0);

	uint8_t block = blockAt(this->position - glm::vec3(0, (float) ENTITY_HEIGHT, 0), chunks);

	onGround = block != 255;
	if (onGround) {
		if (this->velocity.y < 0)
			this->velocity.y = 0;
		this->position.y = ceil(this->position.y);
	}
	else {
		// gravity
		this->position += this->velocity * (float)dt;
	}



	// teleport entity to top if theyre too low
	if (this->position.y < 0) {
		this->position.y = 100;
		this->velocity.y = 0;
	}
	
}

void Entity::attemptMove(const glm::vec3& moveDir, float yaw, std::vector<std::unique_ptr<Chunk>>& chunks, double dt) {
	// restrict it so it doesnt go in y direction
	glm::vec3 dir = glm::normalize(glm::vec3(moveDir.x, 0, moveDir.z));
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	glm::vec3 front = glm::vec3(glm::cos(yaw), 0, glm::sin(yaw));
	glm::vec3 right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	// before we move x/z, make sure we don't go inside a block
	glm::vec3 goal = this->position - glm::vec3(0, ENTITY_HEIGHT-.5, 0);
	glm::vec3 change(0.0f, 0.0f, 0.0f);
	change += front * dir.z * (float) -MOVE_SPEED * (float)dt;
	change += right * dir.x * (float) MOVE_SPEED * (float)dt;
	if (blockAt(goal + change, chunks) == 255)
		setPosition(goal + change + glm::vec3(0, ENTITY_HEIGHT-.5, 0));
	else {
		glm::vec3 newChange = change;
		newChange.x = 0;
		if (blockAt(goal + newChange, chunks) == 255)
			setPosition(goal + newChange + glm::vec3(0, ENTITY_HEIGHT - .5, 0));
		else {
			newChange = change;
			newChange.z = 0;
			if (blockAt(goal + newChange, chunks) == 255)
				setPosition(goal + newChange + glm::vec3(0, ENTITY_HEIGHT - .5, 0));
		}
	}

}

void Entity::jump() {
	if (onGround) {
		this->velocity.y = JUMP_POWER;
	}
}