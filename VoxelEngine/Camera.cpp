#include "Camera.h"

const float CAM_SENSITIVITY = .002f;
const float CAM_SPEED = 20;

//projection params

Camera::Camera() {
	projectionMatrix = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);
	camPos = glm::vec3(0, 50, 0);
}

glm::vec3 Camera::getFrontVector() {
	glm::vec3 front;
	front.x = cos(cameraAngle.x) * cos(cameraAngle.y);
	front.y = sin(cameraAngle.y);
	front.z = sin(cameraAngle.x) * cos(cameraAngle.y);

	return front;
}

void Camera::updateMouse(glm::vec2 mousePos) {
	glm::vec2 offset = mousePos - lastMousePos;
	lastMousePos = mousePos;
	cameraAngle += offset * CAM_SENSITIVITY;
	cameraAngle.y = glm::clamp(cameraAngle.y, glm::radians(-89.0f), glm::radians(89.0f));
}


void Camera::updatePos(glm::vec3 moveDirection, double dt) {
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	glm::vec3 front = glm::normalize(getFrontVector());
	glm::vec3 right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	camPos += up * moveDirection.y * CAM_SPEED * (float)dt;
	camPos += front * moveDirection.z * -CAM_SPEED * (float)dt;
	camPos += right * moveDirection.x * CAM_SPEED * (float)dt;

	// Camera::print();
}

glm::mat4 Camera::getViewMatrix() {
	glm::vec3 front = getFrontVector();

	glm::vec3 cameraTarget = camPos + front;


	return glm::lookAt(camPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

float Camera::getCameraYaw() {
	return this->cameraAngle.x;
}

void Camera::print() {
	std::cout << "Camera Position: " << camPos.x << ", " << camPos.y << ", " <<camPos.z << std::endl;
}

Camera& Camera::getInstance() {
	static Camera instance;
	return instance;
}
