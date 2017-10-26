#pragma once
//defines
#define WINDOW_SCREEN_WIDTH 640
#define WINDOW_SCREEN_HEIGHT 432
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FRAMES_PER_SECOND 60
//core includes
#include <vector>
//windows includes
#include <Windows.h>
//3rd party includes
//#include <GL\GL.h>
//#include <GL\GLU.h>
#include <GL\glut.h>
#include <glm\vec2.hpp>
//helpers
class Timer;	class Camera;
class Enemy;	class Player;
class Shader;	namespace Input {
	using Triggers = std::pair<float, float>;
	using Sticks = std::pair<glm::vec2, glm::vec2>;
	enum Button;
	class XBox;
}
enum class State { Menu, Play, Pause };
enum class Keys {
	None = 0x00000,
	W = 0x00001,		A = 0x00002,
	S = 0x00004,		D = 0x00008,
	Q = 0x00010,		E = 0x00020,
	R = 0x00040,		F = 0x00080,
	Z = 0x00100,		X = 0x00200,
	C = 0x00400,		V = 0x008000,
	Num1 = 0x01000,		Num2 = 0x04000,
	Num3 = 0x04000,		Num4 = 0x08000,
	Esc = 0x10000,		Space = 0x20000,
	Enter = 0x40000,
	All = 0x7FFFF,
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
	Keys key = Keys::None;
	Input::XBox* xBox;
	
	// callbacks
	//key
	void keyboardDown(unsigned char key, glm::vec2 mouse);
	void keyboardUp(unsigned char key, glm::vec2 mouse);
	void specialInput(unsigned char key, glm::vec2 mouse);
	//resize
	void windowReshape(glm::vec2 size);
	//mouse
	void mouseClicked(int button, int state, glm::vec2 mouse);
	void mouseMoved(glm::vec2 mouse);
	void mousePassive(glm::vec2 mouse);
	//controller
	void controllerInput(unsigned short index, Input::Button button);
	void controllerSpecial(unsigned short index, Input::Triggers triggers, Input::Sticks sticks);
private:
	// variables
	glm::vec2 windowSize = { 800, 600 };
	glm::vec2 mousePosition;
	float numControllers = 1;
	float deltaTime = 0.0f;
	// objects
	Timer* timer = nullptr;
	Camera* camera;
	Player* player;
	std::vector<Enemy*> enemies;
	Shader* program;
};