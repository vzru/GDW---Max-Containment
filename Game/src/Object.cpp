#include <windows.h>
#include <iostream>

#include "Mesh.h"
#include "AnimationMesh.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Level.h"

#include <glm\gtc\type_ptr.hpp>

Object::Object(glm::vec3 pos)
	: scale(1.0f), position(pos) {
	//mesh = new Mesh();
	//aMesh = new AnimationMesh();
	mat = new Material();
	update(0.f);
}
Object::~Object() {
	//mesh->unload();
	//delete mesh;
	//delete mat;
}

Object* Object::setPosition(glm::vec3 newPosition) {
	position = newPosition;
	return this;
}
Object* Object::setRotation(glm::vec3 newAngles) {
	rotation = newAngles;
	return this;
}
Object* Object::setScale(glm::vec3 newScale) {
	scale = newScale;
	return this;
}
Object* Object::setVelocity(glm::vec3 newVelocity) {
	velocity = newVelocity;
	return this;
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

//void Object::loadTexture(Type::Texture type, const std::string &texFile) {
//	mat->load(type, texFile);
//}
Object* Object::loadTexture(Type::Texture type, Texture *texture) {
	mat->load(type, texture);
	return this;
}
//void Object::loadMesh(const std::string &meshFile) {
//	if (!mesh->load(meshFile)) {
//		std::cout << "Model failed to load." << std::endl;
//		system("pause");
//		exit(0);
//	}
//}
Object* Object::loadMesh(Mesh *meshPtr) {
	mesh = meshPtr;
	return this;
}

Object* Object::loadAnimationFrame(AnimationMesh * meshPtr) {
	animationList.push_back(meshPtr);
	return this;
}

Object* Object::collide(float dt, Level* level, bool ai) {
	glm::vec3 vOrig = velocity;
	glm::vec2 pX = { position.x + velocity.x * dt / 200.f, position.z };
	glm::vec2 pY = { position.x, position.z + velocity.z * dt / 200.f };
	for (auto& box : level->hitBoxes) {
		if (Helper::inside(pX, Helper::castBox(box)))
			velocity.x = 0.f;
		if (Helper::inside(pY, Helper::castBox(box)))
			velocity.z = 0.f;
	}
	if (ai && ((velocity.x == 0.f && velocity.z != 0.f) || (velocity.x != 0.f && velocity.z == 0.f)))
		velocity = glm::normalize(velocity) * glm::length(vOrig);

	return this;
}

Object* Object::update(float dt, glm::vec3 offset) {
	// Offset it (mostly off of the player)
	position += offset;

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

	// Animation mesh morphing
	if (timer >= timerReset) {
		keyFrame++;
		timer = 1.0f;
	}
	if (keyFrame >= animationList.size())
		keyFrame = 0;
	timer += dt * glm::length(velocity);
	t = timer / timerReset;

	if (animationList.size() > 0)
		aMesh = animationList[keyFrame];
	
	// move it back to it's own position
	position -= offset;
	return this;
}

Object* Object::draw(Shader *shader, Camera *camera, Light* light) {
	// Start
	shader->bind()
	// Basic
		->sendUniform("u_model", transform)
		->sendUniform("u_view", camera->getView())
		->sendUniform("u_proj", camera->getProj())
		->sendUniform("u_color", color)
		->sendUniform("animated", aMesh != nullptr)
		->sendUniform("ammo", ammo)
		->sendUniform("t", t);
	// Material
	mat->sendUniforms(shader, "material.");
	// Light
	light->sendUniforms(shader, camera->getView(), "light.");

	mat->bind();
	if (aMesh != nullptr)
		aMesh->draw();
	else mesh->draw();
	mat->unbind();
	shader->unbind();
	return this;
}

Object* Object::draw(Shader* shader, Camera* camera, std::vector<Light*> lights) {
	shader->bind()
	// Basic
		->sendUniform("u_model", transform)
		->sendUniform("u_view", camera->getView())
		->sendUniform("u_proj", camera->getProj())
		->sendUniform("u_color", color)
		->sendUniform("animated", aMesh != nullptr)
		->sendUniform("ammo", ammo)
		->sendUniform("t", t)
	// Lights
		->sendUniform("numLights", lights.size());
	for (int i = 0; i < lights.size(); i++)
		lights[i]->sendUniforms(shader, camera->getView(), "lights[" + std::to_string(i) + "].");
	// Material
	mat->sendUniforms(shader, "material.");

	mat->bind();
	if (aMesh != nullptr)
		aMesh->draw();
	else mesh->draw();
	mat->unbind();
	shader->unbind();
	return this;
}