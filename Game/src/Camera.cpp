#include "Camera.h"
#include <iostream>
#include <glm\gtc\type_ptr.hpp>

#include <glm\gtc\type_ptr.hpp>
#include <iostream>

Camera::Camera(glm::vec2 size) {
	reset(size);
}
Camera::~Camera() {}

void Camera::update(glm::vec3 center) {
	//translate = glm::translate(center);
	//auto m = glm::lookAt(position, position + forwardVector, upVector);
	view = glm::lookAt(center + position, center, { 0, 1, 0 });
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(&view[0][0]);
}

void Camera::reset(glm::vec2 size) {
	//movementScalar = 0.5;
	//upVector = { 0.0f, 1.0f, 0.0f };
	//forwardVector = { 0.0f, -1.0f, -1.0f };
	//forwardVector = { 0.0f, -position.z / 10.0f, -position.y / 10.0f };
	//translate = glm::translate(position + glm::vec3(0, 1, 0));
	//rotate = glm::rotate(0.f, { 0.f, 1.f, 0.f }); //glm::rotate(0.f, { 0.f, 0.f, 0.f });
	//transform = translate * rotate;
	proj = glm::perspective(45.f, size.x / size.y, 0.1f, 10000.f);
}

//void Camera::processMotion(glm::vec2 change, float dt) {
//	// yaw rotation
//	forwardVector = glm::rotate(glm::normalize(forwardVector), glm::radians(0.08f * change.x), { 0.0f, 1.0f, 0.0f });
//	// calc right vector
//	calcRightVector();
//	// pitch rotation
//	forwardVector = glm::rotate(forwardVector, glm::radians(0.08f * change.y), rightVector);
//	// calc up vector
//	upVector = glm::normalize(glm::cross(rightVector, forwardVector));
//}

//void Camera::pan(glm::vec3 amount) {
//	calcRightVector();
//	position += rightVector * amount.x;
//	position += glm::vec3(0.0f, 1.0f, 0.0f) * amount.y;
//	position += forwardVector * amount.z;
//}
//void Camera::move(glm::vec3 amount) {
//	position += amount;
//	position += amount;
//}

//void Camera::calcRightVector() {
//	rightVector = glm::cross(forwardVector, { 0.0f, 1.0f, 0.0f });
//	rightVector = glm::normalize(rightVector);
//}

void Camera::setPosition(glm::vec3 pos) {
	position = pos;
}

glm::mat4& Camera::getProj() {
	//projection = glm::perspective(45.f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 10000.f);
	//return projection;
	return proj;
}

glm::mat4& Camera::getView() {
	//transform = glm::rotate(transform, glm::radians(90.0f), rightVector);
	//transform = glm::translate(transform, position);
	return view;
}

//double dArray[16] = { 0.0 };
//const float *pSource = (const float*)glm::value_ptr(view);
//for (int i = 0; i < 16; ++i) {
//	dArray[i] = pSource[i];
//	if (i % 4 == 3)
//		std::cout << dArray[i] << std::endl;
//	else
//		std::cout << dArray[i] << ' ';
//}
//std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
