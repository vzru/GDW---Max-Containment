#include "Camera.h"

Camera::Camera()
	: position({ 0.0f, 10.0f, 10.0f }) {
	reset();
	}
Camera::~Camera() {}

void Camera::update() {
	auto m = glm::lookAt(position, position + forwardVector, upVector);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&m[0][0]);
}

void Camera::reset() {
	movementScalar = 0.5;
	upVector = { 0.0f, 1.0f, 0.0f };
	forwardVector = { 0.0f, -1.0f, -1.0f };
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

glm::vec3& Camera::getPosition() {
	return position;
}
glm::vec3& Camera::getProjection() {
	return forwardVector;
}

glm::mat4 Camera::getTransform()
{
	cameraPosition = glm::translate(cameraPosition, position);
	return cameraPosition;
}

