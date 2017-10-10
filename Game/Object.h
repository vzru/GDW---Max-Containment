#pragma once

#include <string>

#include <GLM/glm.hpp>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GL/freeglut.h>

class Object
{
protected:
	glm::vec3 m_pLocalPosition;
	glm::vec3 m_pRotation;
	glm::vec3 m_pScale;

	// these matrices are computed in the Update function
	glm::mat4 m_pLocalRotation;
	glm::mat4 m_pLocalToWorldMatrix;

public:
	Object();
	~Object();

	void setPosition(glm::vec3 newPosition);
	void setRotationAngle(glm::vec3 newAngle);
	void setScale(glm::vec3 newScale);

	glm::vec3 getPosition();
	glm::vec3 getRotationAngles();
	glm::vec3 getScale();

	glm::mat4 getLocalToWorldMatrix();

	virtual void update(float dt);
	virtual void draw();

	// Other Properties
	std::string name;
	glm::vec4 colour;
	// Mesh* ...
	// Material* ...
};