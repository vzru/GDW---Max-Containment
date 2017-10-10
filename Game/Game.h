#pragma once

#include <vector>

#include <Windows.h>

#include <GL\GL.h>
#include <GL\GLU.h>
#include <GL\glut.h>
#include <glm\vec2.hpp>

#include "Controller.h"

class Timer;
class Object;
class Player;
class Camera;

class Game
{
public:
	// construct & destruct
	Game(int& argc, char **argv);
	~Game();

	// variables
	glm::vec2 windowSize = { 800, 600 };
	glm::vec2 mousePosition;
	float numControllers = 1;
	float deltaTime = 0.0f;

	// updates
	void update();
	void draw();

	// helpers
	Input::XBoxInput* XBoxController;
	Timer* timer;
	Camera* camera;

	// objects
	Player* player;
	std::vector<Object*> enemies;

	// callback functions
	void keyboardDown(unsigned char key, glm::vec2 mouse);
	void keyboardUp(unsigned char key, glm::vec2 mouse);
	void windowReshape(glm::vec2 size);
	void mouseClicked(int button, int state, glm::vec2 mouse);
	void mouseMoved(glm::vec2 mouse);
	void mousePassive(glm::vec2 mouse);
	void specialInput(int key, glm::vec2 mouse);
	void controllerInput(unsigned short index, Input::Button button);
	void controllerSpecial(unsigned short index, Input::Triggers triggers, Input::Sticks sticks);
};