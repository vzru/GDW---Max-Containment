#pragma once

#include <string>
#include <memory>

#include <GLM/glm.hpp>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GL/freeglut.h>

class OBJMesh;

class Object {
protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	// these matrices are computed in the Update function
	glm::mat4 localRotation;
	glm::mat4 localToWorldMatrix;

public:
	Object(glm::vec3 pos = { 0,0,0 });
	~Object();

	// set transformation
	void setPosition(glm::vec3 newPosition);
	void setRotation(glm::vec3 newAngle);
	void setScale(glm::vec3 newScale);

	// get transformation
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	glm::mat4 getLocalToWorldMatrix();

	virtual void update(float dt);
	virtual void draw();

	// Other Properties
	std::string name;
	glm::vec4 colour;
	std::shared_ptr<OBJMesh> mesh;
	// Material* ...
};