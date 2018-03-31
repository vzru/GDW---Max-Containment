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
	mesh = new Mesh();
	aMesh = new AnimationMesh();
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

//void Object::loadTexture(Type::Texture type, const std::string &texFile) {
//	mat->load(type, texFile);
//}
void Object::loadTexture(Type::Texture type, Texture *texture) {
	mat->load(type, texture);
}
//void Object::loadMesh(const std::string &meshFile) {
//	if (!mesh->load(meshFile)) {
//		std::cout << "Model failed to load." << std::endl;
//		system("pause");
//		exit(0);
//	}
//}
void Object::loadMesh(Mesh *meshPtr) {
	mesh = meshPtr;
}

void Object::loadAnimationFrame(AnimationMesh * m)
{
	animationList.push_back(m);
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

	//if (p)
	//{
		// Animation mesh morphing
	if (timer >= timerReset)
	{
		keyFrame++;
		timer = 1.0f;
	}
	if (keyFrame >= animationList.size())
	{
		keyFrame = 0;
	}
	timer += dt * glm::length(velocity);
	t = timer / timerReset;
	//}
	//std::cout << keyFrame << std::endl;
	if (animationList.size() > 0)
	{
		aMesh = animationList[keyFrame];
	}
	//if(keyFrame)
	//std::cout << keyFrame << std::endl;
}

void Object::drawQuad(Shader* shader, Camera* camera) {
	// Start
	shader->bind();
	// Basic
	glm::mat4 u_mv = camera->getView() * transform;
	//glm::mat4 u_mvp = camera->getProj() * transform;
	glm::mat4 u_mvp = glm::mat4(1.f);
	shader->sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader->sendUniformMat4("uMView", glm::value_ptr(u_mv), false);
	shader->sendUniformMat4("uMProj", glm::value_ptr(u_mvp), false);
	shader->sendUniform("objectColor", color);
	//shader->sendUniform("animate", a);
	//shader->sendUniform("ammo", ammo);
	//if (lightCount == 0.0f)
	//{
	//	//std::cout << lights.size() << std::endl;
	//	shader->sendUniform("numLights", lights.size());
	//}
	//else
	//{
	//	//std::cout << lightCount << std::endl;
	//	shader->sendUniform("numLights", lightCount);
	//}
	// Material
	//shader->sendUniform("material.diffuse", 0);
	//shader->sendUniform("material.specular", 1);
	//shader->sendUniform("material.normal", 2);
	//shader->sendUniform("material.hue", mat->hue);
	//shader->sendUniform("material.specExponent", mat->specExponent);
	// Lights
	//for (int i = 0; i < lights.size(); i++) {
	//	std::string prefix = "lights[" + std::to_string(i) + "].";
	//
	//	//shader->sendUniform("NUM_LIGHTS", lights.size());
	//	shader->sendUniform(prefix + "type", lights[i]->type);
	//	shader->sendUniform(prefix + "position", camera->getView() * lights[i]->position);
	//	shader->sendUniform(prefix + "direction", camera->getView() * lights[i]->direction);
	//	shader->sendUniform(prefix + "original", camera->getView() * lights[i]->original);
	//	shader->sendUniform(prefix + "ambient", lights[i]->ambient);
	//	shader->sendUniform(prefix + "diffuse", lights[i]->diffuse);
	//	shader->sendUniform(prefix + "specular", lights[i]->specular);
	//	shader->sendUniform(prefix + "specExponent", lights[i]->specExponent);
	//	shader->sendUniform(prefix + "spotExponent", lights[i]->spotExponent);
	//	shader->sendUniform(prefix + "cutoff", lights[i]->cutoff);
	//	shader->sendUniform(prefix + "outerCutoff", lights[i]->outerCutoff);
	//	shader->sendUniform(prefix + "partial", lights[i]->partial);
	//	shader->sendUniform(prefix + "attenuation", lights[i]->attenuation);
	//}
	if (mat->diffuse)
	{
		// Textures
		glActiveTexture(GL_TEXTURE0);
		mat->diffuse->bind();
		glActiveTexture(GL_TEXTURE1);
		mat->specular->bind();
		glActiveTexture(GL_TEXTURE2);
		mat->normal->bind();
	}
	// Mesh
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->getNumVertices());
	glBindVertexArray(GL_NONE);

	if (mat->diffuse)
	{
		// Textures
		mat->normal->unbind();
		glActiveTexture(GL_TEXTURE1);
		mat->specular->unbind();
		glActiveTexture(GL_TEXTURE0);
		mat->diffuse->unbind();
	}
	// End
	shader->unbind();
}

void Object::draw(Shader* shader, Camera* camera, std::vector<Light*> lights, int a, float lightCount) {
	// Start
	shader->bind();
	// Basic
	shader->sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader->sendUniformMat4("uView", glm::value_ptr(camera->getView()), false);
	shader->sendUniformMat4("uProj", glm::value_ptr(camera->getProj()), false);
	shader->sendUniform("objectColor", color);
	shader->sendUniform("animate", a);
	//shader->sendUniform("ammo", ammo);
	if (lightCount == 0.0f)
	{
		//std::cout << lights.size() << std::endl;
		shader->sendUniform("numLights", lights.size());
	}
	else
	{
		//std::cout << lightCount << std::endl;
		shader->sendUniform("numLights", lightCount);
	}
	// Material
	shader->sendUniform("material.diffuse", 0);
	shader->sendUniform("material.specular", 1);
	shader->sendUniform("material.normal", 2);
	shader->sendUniform("material.hue", mat->hue);
	shader->sendUniform("material.specExponent", mat->specExponent);
	// Lights
	for (int i = 0; i < lights.size(); i++) {
		std::string prefix = "lights[" + std::to_string(i) + "].";

		//shader->sendUniform("NUM_LIGHTS", lights.size());
		shader->sendUniform(prefix + "type", lights[i]->type);
		shader->sendUniform(prefix + "position", camera->getView() * lights[i]->position);
		shader->sendUniform(prefix + "direction", camera->getView() * lights[i]->direction);
		shader->sendUniform(prefix + "original", camera->getView() * lights[i]->original);
		shader->sendUniform(prefix + "ambient", lights[i]->ambient);
		shader->sendUniform(prefix + "diffuse", lights[i]->diffuse);
		shader->sendUniform(prefix + "specular", lights[i]->specular);
		shader->sendUniform(prefix + "specExponent", lights[i]->specExponent);
		shader->sendUniform(prefix + "spotExponent", lights[i]->spotExponent);
		shader->sendUniform(prefix + "cutoff", lights[i]->cutoff);
		shader->sendUniform(prefix + "outerCutoff", lights[i]->outerCutoff);
		shader->sendUniform(prefix + "partial", lights[i]->partial);
		shader->sendUniform(prefix + "attenuation", lights[i]->attenuation);
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

void Object::aDraw(Shader * shader, Camera * camera, std::vector<Light*> lights, int a, float lightCount)
{
	// Start
	shader->bind();
	// Basic
	shader->sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader->sendUniformMat4("uView", glm::value_ptr(camera->getView()), false);
	shader->sendUniformMat4("uProj", glm::value_ptr(camera->getProj()), false);
	shader->sendUniform("objectColor", color);
	shader->sendUniform("animate", a);
	shader->sendUniform("t", t);
	//shader->sendUniform("timerReset", t);
	//shader->sendUniform("ammo", ammo);
	if (lightCount == 0.0f)
	{
		//std::cout << lights.size() << std::endl;
		shader->sendUniform("numLights", lights.size());
	}
	else
	{
		//std::cout << lightCount << std::endl;
		shader->sendUniform("numLights", lightCount);
	}
	// Material
	shader->sendUniform("material.diffuse", 0);
	shader->sendUniform("material.specular", 1);
	shader->sendUniform("material.normal", 2);
	shader->sendUniform("material.hue", mat->hue);
	shader->sendUniform("material.specExponent", mat->specExponent);
	// Lights
	for (int i = 0; i < lights.size(); i++) {
		std::string prefix = "lights[" + std::to_string(i) + "].";

		//shader->sendUniform("NUM_LIGHTS", lights.size());
		shader->sendUniform(prefix + "type", lights[i]->type);
		shader->sendUniform(prefix + "position", camera->getView() * lights[i]->position);
		shader->sendUniform(prefix + "direction", camera->getView() * lights[i]->direction);
		shader->sendUniform(prefix + "original", camera->getView() * lights[i]->original);
		shader->sendUniform(prefix + "ambient", lights[i]->ambient);
		shader->sendUniform(prefix + "diffuse", lights[i]->diffuse);
		shader->sendUniform(prefix + "specular", lights[i]->specular);
		shader->sendUniform(prefix + "specExponent", lights[i]->specExponent);
		shader->sendUniform(prefix + "spotExponent", lights[i]->spotExponent);
		shader->sendUniform(prefix + "cutoff", lights[i]->cutoff);
		shader->sendUniform(prefix + "outerCutoff", lights[i]->outerCutoff);
		shader->sendUniform(prefix + "partial", lights[i]->partial);
		shader->sendUniform(prefix + "attenuation", lights[i]->attenuation);
	}
	// Textures
	glActiveTexture(GL_TEXTURE0);
	mat->diffuse->bind();
	glActiveTexture(GL_TEXTURE1);
	mat->specular->bind();
	glActiveTexture(GL_TEXTURE2);
	mat->normal->bind();
	// Mesh
	glBindVertexArray(aMesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, aMesh->getNumVertices());
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
