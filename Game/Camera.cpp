#include "Camera.h"

Camera::Camera()
	: cameraPosition({ 0.0f, 10.0f, 10.0f }), movementScalar(0.5f),
	upVector(0.0f, 1.0f, 0.0f), forwardVector({ 0.0f, -1.0f, -1.0f })
	{	//processMotion({ 0.0f, 0.0f }, 0.03f);
	}
Camera::~Camera() {}

void Camera::update() {
	auto m = glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&m[0][0]);
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

void Camera::moveY(float scalar) {
	cameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * scalar;
}
void Camera::moveZ(float scalar) {
	cameraPosition += forwardVector * scalar;
}
void Camera::moveX(float scalar) {
	calcRightVector();
	cameraPosition += rightVector * scalar;
}

void Camera::calcRightVector() {
	rightVector = glm::cross(forwardVector, { 0.0f, 1.0f, 0.0f });
	rightVector = glm::normalize(rightVector);
}