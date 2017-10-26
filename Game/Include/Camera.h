#pragma once

#include <glm/vec3.hpp>
#include <GL/freeglut.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>

class Camera {
public:
	Camera();
	~Camera();

	// Applies view matrix to OpenGL
	void update();
	// reset the window position
	void reset();

	// rotate the window
	void processMotion(glm::vec2 change, float dt);
	// move the camera
	void pan(glm::vec3 amount);
	void move(glm::vec3 amount);
	// getters
	glm::vec3 getPosition();
	glm::vec3 getProjection();
private:
	// position vectors
	glm::vec3 position;
	glm::vec3 forwardVector;
	glm::vec3 rightVector;
	glm::vec3 upVector;
	// helpers
	float movementScalar;
	void calcRightVector();
};