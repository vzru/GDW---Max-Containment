#pragma once

#include <vector>

#include <Windows.h>

#include <GL\GL.h>
#include <GL\GLU.h>
#include <GL\glut.h>
#include <glm\vec2.hpp>

class Timer;
class Enemy;
class Player;
class Camera;
namespace Input {
	using Triggers = std::pair<float, float>;
	using Sticks = std::pair<glm::vec2, glm::vec2>;
	enum Button;
	class XBox;
}

enum class State {
	Menu, Play, Pause
};

class Game {
public:
	// construct & destruct
	Game(int& argc, char **argv);
	~Game();

	// updates
	void update();
	void draw();

	// helpers
	State state = State::Menu;
	Input::XBox* xBox;
	
	// callbacks
	//key
	void keyboardDown(unsigned char key, glm::vec2 mouse);
	void keyboardUp(unsigned char key, glm::vec2 mouse);
	void specialInput(unsigned char key, glm::vec2 mouse);
	void windowReshape(glm::vec2 size);
	void mouseClicked(int button, int state, glm::vec2 mouse);
	void mouseMoved(glm::vec2 mouse);
	void mousePassive(glm::vec2 mouse);
	void controllerInput(unsigned short index, Input::Button button);
	void controllerSpecial(unsigned short index, Input::Triggers triggers, Input::Sticks sticks);
private:
	// variables
	glm::vec2 windowSize = { 800, 600 };
	glm::vec2 mousePosition;
	float numControllers = 1;
	float deltaTime = 0.0f;
	// objects
	Camera* camera;
	Timer* timer;
	Player* player;
	std::vector<Enemy*> enemies;
};