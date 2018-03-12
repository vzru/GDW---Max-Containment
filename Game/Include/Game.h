#pragma once
//defines
#define WINDOW_SCREEN_WIDTH 1920	
#define WINDOW_SCREEN_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FRAMES_PER_SECOND 60
#define inputControl void(*_controllerInput)(unsigned short index, Input::Button button), void(*_controllerSpecial)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks)
//core includes
#include <vector>
#include <unordered_map>
//windows includes
#include <Windows.h>
#include "Sound.h"
//3rd party includes
//#include <GL\GL.h>
//#include <GL\GLU.h>
#include <GL\glut.h>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
//helpers
class Timer;	class Camera;
class Enemy;	class Player;
class Shader;	class Light;
class Level;	class Object;
class Assets;
namespace Input {
	using Triggers = std::pair<float, float>;
	using Sticks = std::pair<glm::vec2, glm::vec2>;
	enum Button;
	class XBox;
	enum Keys {
		None,
		KeyW,	KeyA,	KeyS,	KeyD,
		KeyQ,	KeyE,	KeyR,	KeyF,
		KeyZ,	KeyX,	KeyC,	KeyV,
		KeyU,	KeyI,	KeyJ,	KeyK,
		Num1,	Num2,	Num3,	Num4,
		Esc,	Enter,	Space,	Delete,
		LShift,	RShift,	LAlt,	RAlt,
		LCtrl, RCtrl
	};
}
enum class State { Menu, Control, Play, Pause, Win, Lose };

class Game {
public:
	// construct & destruct
	Game(int& argc, char **argv);
	~Game();

	Game* init(inputControl);
	Game* load();
	Game* clear();

	// updates
	void update();
	void draw();

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
	// helpers
	Assets* assets;
	Timer* timer = nullptr;
	float deltaTime = 0.f;
	State state = State::Menu;
	glm::vec2 windowSize;
	// objects
	std::unordered_map<std::string, Sound*> sounds;
	std::unordered_map<std::string, Shader*> program;
	Player* player;
	std::vector<Enemy*> enemies;
	Object *dropHP, *dropAmmo;
	std::vector<Object*> drops;
	// methods
	//init
	void initShaders();
	void initScreens();
	void initLevel();
	void initHUD();
	void initEnemies();
	//load
	void loadAssets();
	void loadEnemies(), loadDrops();
	//clear
	void clearShaders(), clearSounds();
	void clearEnemies(), clearDrops();
	void createDropItem(glm::vec3 pos, int type = 0);
	void drawAmmo();
	// input
	struct InputData {
		glm::vec2 mouse;
		int button, state;
		unsigned int keys = Input::Keys::None;
		Input::XBox *xBox;
		int numControllers = 1;
	} input;
	// level
	struct LevelData {
		Level* collision;
		Object *map, *hitboxes;
		Camera *camera;
		
		// @@@@@ FOR SEAN @@@@@ Light positions, have to move them up from floor position
		std::vector<glm::vec3> lightsPos =
		{
			// Examples for formatting
			{ 16.0f, 10.0f, 10.0f },		
			{ 33.0f, 15.0f, 49.0f },
			{ 151.0f, 10.0f, 11.0f },
			{ 2.0f, 10.0f, 25.0f },
			{ 86.0f, 10.0f, 65.0f },
		};

		std::vector<Light*> lights;
		Light *light, *light2, *light3;
		glm::vec3 start = { 4.f, 0.f, 6.f };
		glm::vec4 exit = { 77.f, 81.f, 67.f, 70.f };
		std::pair<std::vector<glm::vec2>, std::vector<glm::vec2>> drops = {
			{ { 34.5f, 48.f }, { 30.f, 3.f }, { 151.6f, 11.1f }, { 123.6f, 64.2f }, { 102.8f, 38.8f } },
			{ { 16.7f, 10.4f }, { 30.7f, 48.f }, { 67.3f, 3.4f }, { 114.4f, 12.3f }, { 139.7f, 58.f }, { 85.7f, 65.f } }
		};
		struct EnemyData {
			std::tuple<Enemy*, Enemy*, Enemy*> type;
			struct EnemyStats {
				std::tuple<float, float, float> life = { 15.f, 20.f, 10.f };
				std::tuple<float, float, float> moveSpeed = { 250.f, 350.f, 150.f };
				std::tuple<float, float, float> damage = { 1.f, 3.f, 5.f };
			} stats;
			std::tuple<std::vector<glm::vec2>, std::vector<glm::vec2>, std::vector<glm::vec2>> positions = {
				{
					{ 2.86804f,	0.8909f },		{ 32.4772f, 47.1091f },		{ 63.368f,  29.1091f },		{ 27.5863f, 3.10911f },
					{ 33.5863f, 3.10911f },		{ 35.368f,  9.10911f },		{ 29.5863f, 8.89089f },		{ 57.368f,  3.10911f },
					{ 33.5863f, 8.89089f },		{ 151.368f, 20.8909f },		{ 127.368f, 4.89089f },		{ 133.368f, 4.89089f },
					{ 113.368f, 4.89089f },		{ 105.368f, 4.89089f },		{ 87.5863f, 2.89089f },		{ 135.368f, 11.1091f },
					{ 136.477f, 42.f },			{ 132.477f, 42.f },			{ 136.477f, 46.f },			{ 98.4771f, 64.f },
					{ 102.477f, 64.f },			{ 106.477f, 64.f },			{ 76.4771f, 34.f },			{ 80.4771f, 34.f },
					{ 33.248f,  34.821f },		{ 29.131f,  29.103f },		{ 50.228f,  12.264f },		{ 111.795f, 17.262f },
					{ 115.858f, 17.976f },		{ 106.910f, 37.245f },		{ 136.785f, 55.583f },		{ 111.646f, 63.259f }
				},	{
					{ 34.544f,  28.836f },		{ 32.371f,  18.886f },		{ 96.425f,  22.153f },		{ 99.169f,  19.952f },
					{ 99.553f,  22.368f },		{ 101.694f, 21.489f },		{ 130.350f, 16.604f },		{ 137.761f, 16.219f },
					{ 119.206f, 27.034f },		{ 119.045f, 53.868f },		{ 103.488f, 52.919f }
				},	{
					{ 67.368f,  15.1091f },		{ 51.5863f, 44.8909f },		{ 151.586f, 16.8909f },		{ 151.368f, 12.8909f },
					{ 141.368f, 58.f },			{ 101.586f, 64.f },			{ 102.477f, 36.f },			{ 79.368f,  64.f } ,
					{ 71.923f,  22.586f },		{ 147.219f, 36.128f },		{ 149.117f, 62.974f }
				}
			};
		} enemy;
	} level;
	// hud
	struct Hud {
		Object *display;
		Object *healthBar;
		struct Ammo {
			Object *number;
			glm::vec3 positions[5] = {
				{0,10,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}
			};
			//Object *digit0, *digit1, *digit2, *digit3, *digit4, *digit5, *digit6, *digit7, *digit8, *digit9;
			//glm::vec3 pos1, pos2, pos3, pos4, pos5;
		} ammo;
		glm::vec3 angle = { 10.f, 0.f, 0.f };
		Light *light;
		Camera *camera;
	} hud;
	// screens
	struct Screen {
		Object *loading, *menu, *controls, *pause, *win, *lose;
		struct Button {
			Object *obj;
			glm::vec4 pos;
		} play, quit;
		Camera *camera;
		Light *light;
	} screen;
};
