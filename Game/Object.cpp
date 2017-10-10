#include <windows.h>
#include <GL/GL.h>

#include "Object.h"
#include "Graphics.h"

Object::Object()
	: m_pScale(1.0f), colour(1.0f)
	{}
Object::~Object() {}

void Object::setPosition(glm::vec3 newPosition) {
	m_pLocalPosition = newPosition;
}
void Object::setRotationAngle(glm::vec3 newAngle) {
	m_pRotation = newAngle;
}
void Object::setScale(glm::vec3 newScale) {
	m_pScale = newScale;
}

glm::vec3 Object::getPosition() {
	return m_pLocalPosition;
}
glm::vec3 Object::getRotationAngles() {
	return m_pRotation;
}
glm::vec3 Object::getScale() {
	return m_pScale;
}

glm::mat4 Object::getLocalToWorldMatrix() {
	return m_pLocalToWorldMatrix;
}

void Object::update(float dt) {
	// Create 4x4 transformation matrix

	// 1. Create the X,Y, and Z rotation matrices
	glm::mat4 rx = glm::rotate(glm::radians(m_pRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(glm::radians(m_pRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(glm::radians(m_pRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// 2. Combine the three rotation matrices into one
	m_pLocalRotation = rz * ry * rx;
	// 3. Create translation matrix
	glm::mat4 trans = glm::translate(m_pLocalPosition);
	// 4. Create scale matrix
	glm::mat4 scale = glm::scale(m_pScale);
	// 5. Combine all above transforms into a single matrix
	m_pLocalToWorldMatrix = trans * scale * m_pLocalRotation;

	// Note: pay attention to rotation order (2), ZYX is not the same as XYZ
	// Order is specified by an HTR file when you export it
	// Note: pay attention to transformation order (5)
}

void Object::draw()
{
	// Draw a coordinate frame for the object
	glm::vec3 wPos = m_pLocalPosition;
	glm::mat4 wRot = m_pLocalRotation;

	glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);

	//TTK::Graphics::DrawVector(wPos, wRot[0], 3.0f, red);
	Graphics::drawVector(wPos, wRot[0], 3.0f, red);
	//TTK::Graphics::DrawVector(wPos, wRot[1], 3.0f, green);
	Graphics::drawVector(wPos, wRot[1], 3.0f, green);
	//TTK::Graphics::DrawVector(wPos, wRot[2], 3.0f, blue);
	Graphics::drawVector(wPos, wRot[2], 3.0f, blue);

	// Draw node
	//TTK::Graphics::DrawSphere(m_pLocalToWorldMatrix, 0.5f, colour);
	Graphics::drawSphere(m_pLocalToWorldMatrix, 0.5f, colour);
}