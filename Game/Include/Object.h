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
	//void loadTexture(Type::Texture type, const std::string &texFile);
	void loadTexture(Type::Texture type, Texture *texPtr);
	//void loadMesh(const std::string &meshFile);
	void loadMesh(Mesh *meshPtr);
	void loadAnimationFrame(AnimationMesh *m);
	// Updating functions
	void collide(float dt, Level* level, bool ai = false);
	virtual void update(float dt);
	//virtual void physics(float dt);
	virtual void draw(Shader *shader, Camera *camera, std::vector<Light*> lights, int a, float lightCount = 0.0f);
	virtual void aDraw(Shader *shader, Camera *camera, std::vector<Light*> lights, int a, float lightCount = 0.0f);


	// Physical properties
	glm::vec4 color;
	Mesh* mesh;// , currentMesh, nextMesh;
	AnimationMesh* aMesh;
	std::vector<AnimationMesh*> animationList;
	Material* mat;
	int ammo;
	float life, cooldown, timer, t, timerReset = 35.0f;
	bool collect;
	int keyFrame = 0;
};