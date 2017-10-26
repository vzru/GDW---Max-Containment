#include "Camera.h"

Camera::Camera()
	: cameraPosition({ 0.0f, 15.0f, 10.0f }) {
	reset();
	}
Camera::~Camera() {}

void Camera::update() {
	auto m = glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&m[0][0]);
}

void Camera::reset() {
	movementScalar = 0.5;
	upVector = { 0.0f, 1.0f, 0.0f };
	forwardVector = { 0.0f, -(cameraPosition.y/10), -(cameraPosition.z/10) };
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
	cameraPosition += rightVector * amount.x;
	cameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * amount.y;
	cameraPosition += forwardVector * amount.z;
}
void Camera::move(glm::vec3 amount) {
	cameraPosition += amount;
}

void Camera::calcRightVector() {
	rightVector = glm::cross(forwardVector, { 0.0f, 1.0f, 0.0f });
	rightVector = glm::normalize(rightVector);
}