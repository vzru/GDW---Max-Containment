#pragma once

#include <glm/vec3.hpp>
#include <GL/freeglut.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>

class Camera {
public:
	Camera(glm::vec2 size);
	~Camera();

	// updates the view matrix and applies it to OpenGL
	void update(glm::vec3 center = { 0.f, 0.f, 0.f });
	// reset the window position
	void reset(glm::vec2 size);

	// rotate the window
	//void processMotion(glm::vec2 change, float dt);
	// move the camera
	//void pan(glm::vec3 amount);
	//void move(glm::vec3 amount);
	// getters
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
	glm::mat4& getView();
	glm::mat4& getProj();
private:
	// vectors
	glm::vec3 position;
	//glm::vec3 forwardVector;
	//glm::vec3 rightVector;
	//glm::vec3 upVector; //(0,1,0)
	// matrices
	glm::mat4 proj;
	glm::mat4 view;
	//glm::mat4 translate;
	//glm::mat4 rotate;
	//glm::mat4 transform;
	// helpers
	//float movementScalar;
	//void calcRightVector();
};