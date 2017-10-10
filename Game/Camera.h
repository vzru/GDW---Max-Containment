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

	// rotate the window from the mouse
	void processMotion(glm::vec2 change, float dt);

	// move the camera
	void pan(glm::vec3 amount);
	void move(glm::vec3 amount);

private:
	glm::vec3 cameraPosition;
	glm::vec3 forwardVector;
	glm::vec3 rightVector;
	glm::vec3 upVector;
	float movementScalar;
	void calcRightVector();
};