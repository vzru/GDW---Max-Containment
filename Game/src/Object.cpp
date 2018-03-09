#include <windows.h>
#include <iostream>

#include "Mesh.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Level.h"

#include <glm\gtc\type_ptr.hpp>

Object::Object(glm::vec3 pos)
	: scale(1.0f), position(pos) {
	mesh = new Mesh();
	mat = new Material();
	update(0.f);
}
Object::~Object() {
	//mesh->unload();
	//delete mesh;
	//delete mat;
}

void Object::setPosition(glm::vec3 newPosition) {
	position = newPosition;
}
void Object::setRotation(glm::vec3 newAngles) {
	rotation = newAngles;
}
void Object::setScale(glm::vec3 newScale) {
	scale = newScale;
}
void Object::setVelocity(glm::vec3 newVelocity) {
	velocity = newVelocity;
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
glm::vec3 Object::getVelocity() {
	return velocity;
}

glm::mat4 Object::getTransformation() {
	return transform;
}

void Object::loadTexture(Type::Texture type, const std::string &texFile) {
	mat->load(type, texFile);
}
void Object::loadMesh(const std::string &meshFile) {
	if (!mesh->load(meshFile)) {
		std::cout << "Model failed to load." << std::endl;
		system("pause");
		exit(0);
	}
}

/*float lineCheck(glm::vec2 p, glm::vec2 p0, glm::vec2 p1) {
	return (p1.y - p0.y) * p.x + (p0.x - p1.x) * p.y + (p1.x * p0.y - p0.x * p1.y);
}*/

void Object::collide(float dt, Level* level, bool ai) {
	glm::vec3 vOrig = velocity;
	glm::vec2 pX = { position.x + velocity.x * dt / 200.f, position.z };
	glm::vec2 pY = { position.x, position.z + velocity.z * dt / 200.f };
	for (auto box : level->hitBoxes) {
		if (pX.x > box.min.x && pX.x < box.max.x && pX.y > box.min.y && pX.y < box.max.y) {
			//std::cout << "CollideX" << std::endl;
			velocity.x = 0.f;
		}
		if (pY.x > box.min.x && pY.x < box.max.x && pY.y > box.min.y && pY.y < box.max.y) {
			//std::cout << "CollideY" << std::endl;
			velocity.z = 0.f;
		}
	}
	if (ai && ((velocity.x == 0.f && velocity.z != 0.f) || (velocity.x != 0.f && velocity.z == 0.f)))
		velocity = glm::normalize(velocity) * glm::length(vOrig);
	//bool collided = false;
	//glm::vec2 p0 = { position.x, position.z };
	//glm::vec2 p1 = { position.x + velocity.x * dt / 200.f, position.z + velocity.z * dt / 200.f };
	//for (auto box : level->hitBoxes) {
	//	glm::vec2 mix = { box.min.x, box.max.y };
	//	glm::vec2 man = { box.max.x, box.min.y };
	//	float v0 = lineCheck(box.min, p0, p1);
	//	float v1 = lineCheck(mix, p0, p1);
	//	float v2 = lineCheck(box.max, p0, p1);
	//	float v3 = lineCheck(man, p0, p1);
	//	if ((v0 > 0.f && v1 > 0.f && v2 > 0.f && v3 > 0.f) || (v0 < 0.f && v1 < 0.f && v2 < 0.f && v3 < 0.f))
	//		continue;
	//	if ((p0.x < box.min.x && p1.x < box.min.x) || (p0.x > box.max.x && p1.x < box.max.x)
	//		|| (p0.y < box.min.y && p1.y < box.min.y) || (p0.y > box.max.y && p1.y < box.max.y))
	//		continue;
	//	std::cout << p0.x << ":" << p0.y << ";" << p1.x << ":" << p1.y << "," << v0 << ":" << v1 << ":" << v2 << ":" << v3 << "," << box.min.x << ":" << box.min.y << ";" << box.max.x << ":" << box.max.y << std::endl;
	//	collided = true;
	//}
	//return collided;
}

void Object::update(float dt) {
	// Create 4x4 transformation matrix

	// 1. Create the X,Y, and Z rotation matrices
	glm::mat4 rx = glm::rotate(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// 2. Combine the three rotation matrices into one
	switch ((Type::Rotate)rotType) {
	case Type::Rotate::XYZ: rotate = rz * ry * rx; break;
	case Type::Rotate::YXZ: rotate = rz * rx * ry; break;
	case Type::Rotate::XZY: rotate = ry * rz * rx; break;
	case Type::Rotate::ZYX: rotate = rx * ry * rz; break;
	case Type::Rotate::ZXY: rotate = ry * rx * rz; break;
	case Type::Rotate::YZX: rotate = rx * rz * ry; break;
	}
	// 3. Create translation matrix
	glm::mat4 tran = glm::translate(position);
	// 4. Create scale matrix
	glm::mat4 scal = glm::scale(scale);
	// 5. Combine all above transforms into a single matrix
	transform = tran * rotate * scal;
}

void Object::draw(Shader* shader, Camera* camera, std::vector<Light> lights) {
	// Start
	shader->bind();
	// Basic
	shader->sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader->sendUniformMat4("uView", glm::value_ptr(camera->getView()), false);
	shader->sendUniformMat4("uProj", glm::value_ptr(camera->getProj()), false);
	shader->sendUniform("objectColor", color);
	// Material
	shader->sendUniform("material.diffuse", 0);
	shader->sendUniform("material.specular", 1);
	shader->sendUniform("material.normal", 2);
	shader->sendUniform("material.hue", mat->hue);
	shader->sendUniform("material.specularExponent", mat->specularExponent);
	// Lights
	for (int i = 0; i < lights.size(); i++) {
		std::string prefix = "lights[" + std::to_string(i) + "].";
			
		shader->sendUniform(prefix + "posDir", camera->getView() * lights[i].posDir);
		shader->sendUniform(prefix + "type", lights[i].type);
		shader->sendUniform(prefix + "ambient", lights[i].ambient);
		shader->sendUniform(prefix + "diffuse", lights[i].diffuse);
		shader->sendUniform(prefix + "specular", lights[i].specular);
		shader->sendUniform(prefix + "specularExponent", lights[i].specularExponent);
		shader->sendUniform(prefix + "attenuation", lights[i].attenuation);
	}
	// Textures
	glActiveTexture(GL_TEXTURE0);
	mat->diffuse->bind();
	glActiveTexture(GL_TEXTURE1);
	mat->specular->bind();
	glActiveTexture(GL_TEXTURE2);
	mat->normal->bind();
	// Mesh
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->getNumVertices());
	glBindVertexArray(GL_NONE);
	// Textures
	mat->normal->unbind();
	glActiveTexture(GL_TEXTURE1);
	mat->specular->unbind();
	glActiveTexture(GL_TEXTURE0);
	mat->diffuse->unbind();
	// End
	shader->unbind();
}


GameObject::GameObject()
	: m_pLocalScale(1.0f), colour(glm::vec4(1.0)), m_pParent(nullptr)
{
}

GameObject::~GameObject() {}

void GameObject::addChild(GameObject * newChild)
{
	if (newChild)
	{
		m_pChildren.push_back(newChild);
		newChild->m_pParent = this; // tell new child that this game object is its parent
	}
}

void GameObject::removeChild(GameObject * rip)
{
	for (int i = 0; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i] == rip) // compare memory locations (pointers)
		{
			std::cout << "Removing child: " + rip->name << " from object: " << this->name;
			m_pChildren.erase(m_pChildren.begin() + i);
		}
	}
}

void GameObject::setLocalPosition(glm::vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void GameObject::setLocalRotationAngleX(float newAngle)
{
	m_pLocalRotationAngles.x = newAngle;
}

void GameObject::setLocalRotationAngleY(float newAngle)
{
	m_pLocalRotationAngles.y = newAngle;
}

void GameObject::setLocalRotationAngleZ(float newAngle)
{
	m_pLocalRotationAngles.z = newAngle;
}

void GameObject::setLocalScale(glm::vec3 newScale)
{
	m_pLocalScale = newScale;
}

glm::vec3 GameObject::getLocalPosition()
{
	return m_pLocalPosition;
}

glm::vec3 GameObject::getLocalRotationAngles()
{
	return m_pLocalRotationAngles;
}

glm::vec3 GameObject::getLocalScale()
{
	return m_pLocalScale;
}

glm::mat4 GameObject::getLocalToWorldMatrix()
{
	return m_pLocalToWorldMatrix;
}

glm::vec3 GameObject::getWorldPosition()
{
	if (m_pParent)
		return m_pParent->getLocalToWorldMatrix() * glm::vec4(m_pLocalPosition, 1.0f);
	else
		return m_pLocalPosition;
}

glm::mat4 GameObject::getWorldRotation()
{
	if (m_pParent)
		return m_pParent->getWorldRotation() * m_pLocalRotation;
	else
		return m_pLocalRotation;
}

std::vector<GameObject*> GameObject::getChildren()
{
	return m_pChildren;
}

GameObject * GameObject::getParent()
{
	return m_pParent;
}

void GameObject::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix
	glm::mat4 rx = glm::rotate(glm::radians(m_pLocalRotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(glm::radians(m_pLocalRotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(glm::radians(m_pLocalRotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	glm::mat4 tran = glm::translate(m_pLocalPosition);

	// Create scale matrix
	glm::mat4 scal = glm::scale(m_pLocalScale);

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a parent, then we must apply the parent's transform
	m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;

	if (m_pParent)
		m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
	else
		m_pLocalToWorldMatrix = m_pLocalTransformMatrix;

	// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->update(dt);
}

void GameObject::draw()
{
	//drawKinematicLinkage();
}
//
//void GameObject::drawKinematicLinkage()
//{
//	// Draw a coordinate frame for the object
//	glm::vec3 wPos = getWorldPosition();
//	glm::mat4 wRot = getWorldRotation();
//
//	glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
//	glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
//	glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
//
//	TTK::Graphics::DrawVector(wPos, wRot[0] * 0.25f, 3.0f, red);
//	TTK::Graphics::DrawVector(wPos, wRot[1] * 0.25f, 3.0f, green);
//	TTK::Graphics::DrawVector(wPos, wRot[2] * 0.25f, 3.0f, blue);
//
//	// Draw node
//	//TTK::Graphics::DrawSphere(m_pLocalToWorldMatrix, 0.5f, colour);
//
//	// Draw line to parent 
//	if (m_pParent)
//	{
//		if (name != "Hips")
//		{
//			glm::vec3 parentWorldPosition = m_pParent->getWorldPosition();
//			glm::vec3 myWorldPositon = getWorldPosition();
//			TTK::Graphics::DrawLine(myWorldPositon, parentWorldPosition, 5.0f, colour);
//		}
//	}
//
//	// Draw children
//	for (int i = 0; i < m_pChildren.size(); ++i)
//		m_pChildren[i]->drawKinematicLinkage();
//}
