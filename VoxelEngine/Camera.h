#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera
{
private:
	glm::vec2 cameraAngle; //pitch and yaw
	glm::vec3 getFrontVector();
	glm::vec2 lastMousePos = glm::vec2(400.0f, 300.0f);

	Camera();

public:

	//constants
	static constexpr int SCR_WIDTH = 1200;
	static constexpr int SCR_HEIGHT = 900;
	static constexpr float FOV = 90;
	static constexpr float NEAR_PLANE = 0.1f;
	static constexpr float FAR_PLANE = 200.0f;

	static Camera& getInstance();

	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
	void updateMouse(glm::vec2 mousePos);
	void updatePos(glm::vec3 moveDirection, double dt);
	void print();
	glm::mat4 getViewMatrix();
	glm::mat4 projectionMatrix;
};