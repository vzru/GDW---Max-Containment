#include <windows.h>
#include <GL/GL.h>

#include "Object.h"
#include "Graphics.h"
#include "OBJMesh.h"

Object::Object(glm::vec3 pos)
	: scale(1.0f), colour(1.0f), position(pos), mesh(nullptr)
{}
Object::~Object() {}

void Object::setPosition(glm::vec3 newPosition) {
	position = newPosition;
}
void Object::setRotation(glm::vec3 newAngle) {
	rotation = newAngle;
}
void Object::setScale(glm::vec3 newScale) {
	scale = newScale;
}

glm::vec3 Object::getPosition() {
	return position;
}
glm::vec3 Object::getRotation() {
	return rotation;
}
glm::vec3 Object::getScale() {
	return scale;
}

glm::mat4 Object::getLocalToWorldMatrix() {
	return localToWorldMatrix;
}

void Object::update(float dt) {
	// Create 4x4 transformation matrix

	// 1. Create the X,Y, and Z rotation matrices
	glm::mat4 rx = glm::rotate(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// 2. Combine the three rotation matrices into one
	localRotation = rz * ry * rx;
	// 3. Create translation matrix
	glm::mat4 tran = glm::translate(position);
	// 4. Create scale matrix
	glm::mat4 scal = glm::scale(scale);
	// 5. Combine all above transforms into a single matrix
	localToWorldMatrix = tran * localRotation * scal;

	// Note: pay attention to rotation order (2), ZYX is not the same as XYZ
	// Order is specified by an HTR file when you export it
	// Note: pay attention to transformation order (5)
}

void Object::draw() {
	// Draw a coordinate frame for the object
	glm::vec3 wPos = position;
	glm::mat4 wRot = localRotation;

	glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);

	Graphics::drawVector(wPos, wRot[0], 3.0f, red);
	Graphics::drawVector(wPos, wRot[1], 3.0f, green);
	Graphics::drawVector(wPos, wRot[2], 3.0f, blue);

	// Draw node
	if (mesh == nullptr)
		Graphics::drawSphere(localToWorldMatrix, 0.5f, colour);
	else {
		mesh->setAllColours(colour);
		mesh->draw(localToWorldMatrix);
	}
}