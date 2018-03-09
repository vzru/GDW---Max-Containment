#pragma once

#include <string>
#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GL/freeglut.h>

#include "Light.h"
#include "Type.h"

class Mesh;		class Shader;
class Camera;	class Material;
class Level;

class Object {
protected:
	static const unsigned int rotType = (unsigned int)Type::Rotate::XYZ;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 velocity;

	// these matrices are computed in the Update function
	glm::mat4 rotate;
	glm::mat4 translate;
	glm::mat4 transform;

public:
	Object(glm::vec3 pos = { 0.f, 0.f, 0.f });
	~Object();

	// set transformation
	void setPosition(glm::vec3 newPos);
	void setRotation(glm::vec3 newAng);
	void setScale(glm::vec3 newScale);
	void setVelocity(glm::vec3 newVel);

	// getters
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::vec3 getVelocity();
	glm::mat4 getTransformation();

	// loading functions
	void loadTexture(Type::Texture type, const std::string &texFile);
	void loadMesh(const std::string &meshFile);

	// Updating functions
	void collide(float dt, Level* level, bool ai = false);
	virtual void update(float dt);
	//virtual void physics(float dt);
	virtual void draw(Shader *shader, Camera *camera, std::vector<Light> lights);

	// Physical properties
	glm::vec4 color;
	Mesh* mesh;
	Material* mat;
	int hp, ammo;
	bool collect;
};


class GameObject
{
protected:
	glm::vec3 m_pLocalScale;

	glm::vec3 m_pLocalRotationAngles; // local rotation angles (degrees)

	glm::vec3 m_pLocalPosition;
	glm::mat4 m_pLocalRotation;
	glm::mat4 m_pLocalTransformMatrix;
	glm::mat4 m_pLocalToWorldMatrix; // parentLocalToWorld * m_pLocalTransformation 

									 // Forward kinematics
	GameObject* m_pParent; // Pointer to parent
	std::vector<GameObject*> m_pChildren; // Pointers to children

public:
	GameObject();
	~GameObject();

	// Forward Kinematics
	void addChild(GameObject* newChild);
	void removeChild(GameObject* rip);

	// Note: These now set LOCAL transformation properties
	// Local means: relative to the PARENT
	// Example:
	// if this gameobject's parent located at (0, 2, 0) in WORLD SPACE
	// and this gameobject's LOCAL position is (0,0,0) then that means it is (0,0,0) units
	// from its parent. The WORLD SPACE position of this gameobject would be (0,2,0).
	// If this gameobject's LOCAL position was (0,2,0) then it would be (0,2,0) units from
	// it's parent. Which would mean the gameobject's WORLD position would be (0,4,0).
	// (0,4,0) = world position of parent + gameobject's local offset from parent
	// This is a very important concept to understand!
	void setLocalPosition(glm::vec3 newPosition);
	void setLocalRotationAngleX(float newAngle);
	void setLocalRotationAngleY(float newAngle);
	void setLocalRotationAngleZ(float newAngle);
	void setLocalScale(glm::vec3 newScale);

	glm::vec3 getLocalPosition();
	glm::vec3 getLocalRotationAngles();
	glm::vec3 getLocalScale();

	// parentLocalToWorld * localTransformation
	glm::mat4 getLocalToWorldMatrix();

	glm::vec3 getWorldPosition();

	glm::mat4 getWorldRotation();

	std::vector<GameObject*> getChildren();

	GameObject* getParent();

	virtual void update(float dt);
	virtual void draw();

	//void drawKinematicLinkage();

	// Other Properties
	std::string name;
	glm::vec4 colour;
	// Material* ...
};