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
class Level;	class Texture;
class AnimationMesh;

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
	Object* setPosition(glm::vec3 newPos);
	Object* setRotation(glm::vec3 newAng);
	Object* setScale(glm::vec3 newScale);
	Object* setVelocity(glm::vec3 newVel);

	// getters
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::vec3 getVelocity();
	glm::mat4 getTransformation();

	// Loading functions
	//void loadTexture(Type::Texture type, const std::string &texFile);
	Object* loadTexture(Type::Texture type, Texture *texPtr);
	//void loadMesh(const std::string &meshFile);
	Object* loadMesh(Mesh *meshPtr);
	Object* loadAnimationFrame(AnimationMesh *meshPtr);
	// Updating functions
	Object* collide(float dt, Level* level, bool ai = false);
	virtual Object* update(float dt = 0.f, glm::vec3 offset = glm::vec3(0.f));
	//virtual void physics(float dt);
	virtual Object* draw(Shader *shader, Camera *camera, std::vector<Light*> lights = {});
	virtual Object* draw(Shader *shader, Camera *camera, Light* light);
	
	// Assets
	Mesh* mesh; //, currentMesh, nextMesh;
	AnimationMesh* aMesh;
	std::vector<AnimationMesh*> animationList;
	Material* mat;
	// Physical properties
	glm::vec4 color;
	int ammo;
	float life, cooldown, timer, t, timerReset = 35.0f;
	bool collect;
	int keyFrame = 0;
};