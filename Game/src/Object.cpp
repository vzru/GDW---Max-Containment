#include <windows.h>
#include <iostream>

#include "Mesh.h"
#include "Shader.h"
#include "Object.h"
#include "Graphics.h"
#include "Camera.h"

#include <glm\gtc\type_ptr.hpp>

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

glm::mat4 Object::getTransformation() {
	return transform;
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
	transform = tran * localRotation * scal;

	// Note: pay attention to rotation order (2), ZYX is not the same as XYZ
	// Order is specified by an HTR file when you export it
	// Note: pay attention to transformation order (5)
}

void Object::draw(Shader *shader, Camera *camera) {
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
		Graphics::drawSphere(transform, 0.5f, colour);
	else {
		shader->bind();
		shader->sendUniformMat4("uModel", glm::value_ptr(transform), false);
		shader->sendUniformMat4("uView", glm::value_ptr(camera->getPosition()), false);
		shader->sendUniformMat4("uProj", glm::value_ptr(camera->getProjection()), false);

		//double dArray[16] = { 0.0 };
		//
		//const float *pSource = (const float*)glm::value_ptr(camera->getPosition());
		//for (int i = 0; i < 16; ++i)
		//{
		//	dArray[i] = pSource[i];
		//	if (i % 4 == 3)
		//	{
		//		std::cout << dArray[i] << std::endl;
		//	}
		//	else
		//	{
		//		std::cout << dArray[i] << ' ';
		//	}
		//}
		//std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

		shader->sendUniform("lightPos", glm::vec4(4.f, 0.f, 0.f, 1.f));
		shader->sendUniform("objectColor", colour);
		shader->sendUniform("lightAmbient", glm::vec3(0.15f, 0.15f, 0.15f));
		shader->sendUniform("lightDiffuse", glm::vec3(0.7f, 0.7f, 0.7f));
		shader->sendUniform("lightSpecular", glm::vec3(1.f, 1.f, 1.f));
		shader->sendUniform("lightSpecularExponent", 50.f);
		shader->sendUniform("attenuationConstant", 1.f);
		shader->sendUniform("attenuationLinear", 0.1f);
		shader->sendUniform("attenuationQuadratic", 0.01f);

		glBindVertexArray(mesh->vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh->getNumVertices());
		glBindVertexArray(GL_NONE);

		shader->unbind();
	}
}