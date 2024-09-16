#include "Entity.h"


glm::vec3 Entity::getPosition() {
	return this->position;
}
void Entity::setPosition(const glm::vec3& pos) {
	this->position = pos;
}


// render loop
void Entity::update(double dt) {
	// gravity
	// TODO verify we arent standing on block
	this->velocity += glm::vec3(0, -1 * dt, 0);

	// update position by velocity
	this->position += this->velocity * (float) dt;
}


void Entity::attemptMove(const glm::vec3& moveDir, double dt) {
	this->position += moveDir * (float) dt;
	//TODO collision detection
}
void Entity::attemptMove(const glm::vec3& moveDir, const glm::vec3& frontVector, double dt) {
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	glm::vec3 front = glm::normalize(frontVector);
	glm::vec3 right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	this->position += up * moveDir.y * (float)20 * (float)dt;
	this->position += front * moveDir.z * (float)-20 * (float)dt;
	this->position += right * moveDir.x * (float)20 * (float)dt;

}