#include "Camera.h"
#include "Game.h"

#include <glm\gtc\type_ptr.hpp>
#include <iostream>

Camera::Camera()
	: position({ 0.0f, 10.0f, 10.0f }) {
	reset();
	}
Camera::~Camera() {}

void Camera::update() {
	auto m = glm::lookAt(position, position + forwardVector, upVector);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&m[0][0]);
	//std::cout << position.x << '/' << position.y << '/' << position.z << std::endl;
}

void Camera::reset() {
	movementScalar = 0.5;
	upVector = { 0.0f, 1.0f, 0.0f };
	forwardVector = { 0.0f, -1.0f, -1.0f };
	//forwardVector = { 0.0f, -position.z / 10.0f, -position.y / 10.0f };
}

void Camera::processMotion(glm::vec2 change, float dt) {
	// yaw rotation
	forwardVector = glm::rotate(glm::normalize(forwardVector), glm::radians(0.08f * change.x), { 0.0f, 1.0f, 0.0f });
	// calc right vector
	calcRightVector();
	// pitch rotation
	forwardVector = glm::rotate(forwardVector, glm::radians(0.08f * change.y), rightVector);
	// calc up vector
	upVector = glm::normalize(glm::cross(rightVector, forwardVector));
}

void Camera::pan(glm::vec3 amount) {
	calcRightVector();
	position += rightVector * amount.x;
	position += glm::vec3(0.0f, 1.0f, 0.0f) * amount.y;
	position += forwardVector * amount.z;
}
void Camera::move(glm::vec3 amount) {
	position += amount;
}

void Camera::calcRightVector() {
	rightVector = glm::cross(forwardVector, { 0.0f, 1.0f, 0.0f });
	rightVector = glm::normalize(rightVector);
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::mat4& Camera::getProjection() {
	projection = glm::perspective(45.f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 10000.f);
	return projection;
}

glm::mat4& Camera::getTransform() {
	transform = glm::mat4();
	transform = glm::translate(transform, position);
	//transform = glm::rotate(transform, glm::radians(90.0f), rightVector);
	double dArray[16] = { 0.0 };

	const float *pSource = (const float*)glm::value_ptr(transform);
	for (int i = 0; i < 16; ++i) {
		dArray[i] = pSource[i];
		if (i % 4 == 3)
			std::cout << dArray[i] << std::endl;
		else
			std::cout << dArray[i] << ' ';
	}
	std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	return transform;
}

