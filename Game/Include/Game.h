#pragma once
//defines
#define WINDOW_SCREEN_WIDTH 640
#define WINDOW_SCREEN_HEIGHT 432
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FRAMES_PER_SECOND 60
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
#include <glm\vec4.hpp>
//helpers
class Timer;	class Camera;
class Enemy;	class Player;
class Shader;	class Light;
class Level;	class Object;
namespace Input {
	using Triggers = std::pair<float, float>;
	using Sticks = std::pair<glm::vec2, glm::vec2>;
	enum Button;
	class XBox;
	enum Keys {
		None = 0x0000000,
		KeyW = 0x0000001, KeyA = 0x0000002, KeyS = 0x000004, KeyD = 0x000008,
		KeyQ = 0x0000010, KeyE = 0x0000020, KeyR = 0x000040, KeyF = 0x000080,
		KeyZ = 0x0000100, KeyX = 0x0000200, KeyC = 0x000400, KeyV = 0x000800,
		Num1 = 0x0001000, Num2 = 0x0002000, Num3 = 0x004000, Num4 = 0x008000,
		Esc = 0x0010000, Enter = 0x0020000, Space = 0x040000, Delete = 0x080000,
		LShift = 0x0100000, RShift = 0x0200000, LAlt = 0x400000, RAlt = 0x800000,
		LCtrl = 0x1000000, RCtrl = 0x2000000,
		All = 0x3FFFFFF,
	};
}
enum class State { Menu, Play, Pause, Win, Lose, Control };

class Game {
public:
	// construct & destruct
	Game(int& argc, char **argv);
	~Game();
	void extraCallbacks(void(*_controllerInput)(unsigned short index, Input::Button button), void(*_controllerSpecial)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks));

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

	void createDropItem(glm::vec3 pos);
private:
	std::vector<Sound*> soundList;
	// helpers
	Timer* timer = nullptr;
	float deltaTime = 0.f;
	State state = State::Menu;
	glm::vec2 windowSize;
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
		Light *light;
		glm::vec4 exit = { 77.f, 81.f, 67.f, 70.f };
		std::tuple<std::vector<glm::vec2>, std::vector<glm::vec2>, std::vector<glm::vec2>>
			enemies = {
			{
				{ 2.86804,	30.8909 },		{ 32.4772, 47.1091 },		{ 63.368, 29.1091 },		{ 27.5863, 3.10911 },
				{ 33.5863, 3.10911 },		{ 35.368, 9.10911 },		{ 29.5863, 8.89089 },		{ 57.368, 3.10911 },
				{ 33.5863, 8.89089 },		{ 151.368, 20.8909 },		{ 127.368, 4.89089 },		{ 133.368, 4.89089 },
				{ 113.368, 4.89089 },		{ 105.368, 4.89089 },		{ 87.5863, 2.89089 },		{ 135.368, 11.1091 },
				{ 136.477, 42.0 },			{ 132.477, 42.0 },			{ 136.477, 46.0 },			{ 98.4771, 64.0 },
				{ 102.477, 64.0 },			{ 106.477, 64.0 },			{ 76.4771, 34.0 },			{ 80.4771, 34.0 },
				{ 33.248, 34.821 },			{ 29.131,29.103 },			{ 50.228, 12.264 },			{ 111.795 ,17.262 },
				{ 115.858 ,17.976 },		{ 106.910 ,37.245 },		{ 136.785 ,55.583 },		{ 111.646 ,63.259 }
			},	{
				{ 34.544	,28.836 },		{ 32.371	,18.886 },		{ 96.425	,22.153 },		{ 99.169	,19.952 },
				{ 99.553	,22.368 },		{ 101.694	,21.489 },		{ 130.350	,16.604 },		{ 137.761	,16.219 },
				{ 119.206	,27.034 },		{ 119.045	,53.868 },		{ 103.488	,52.919 }
			},	{
				{ 67.368, 15.1091 },		{ 51.5863, 44.8909 },		{ 151.586, 16.8909 },		{ 151.368, 12.8909 },
				{ 141.368, 58.0 },			{ 101.586, 64.0 },			{ 102.477, 36.0 },			{ 79.368, 64.0 } ,
				{ 71.923	,22.586 },		{ 147.219	,36.128 },		{ 149.117	,62.974 }
			}
		};
	} level;
	// characters
	Player* player;
	std::vector<Enemy*> enemies;
	std::tuple<Enemy*, Enemy*, Enemy*> enemys;
	Object *dropHP, *dropAmmo;
	std::vector<Object*> dropItems;
	void loadEnemies(), clearEnemies();
	void clearDrops();
	// hud
	struct HudData {
		Object *display;
		Object *healthBar;
		Light *light;
		Camera *camera;
	} hud;
	// screens
	struct Screen {
		Object *menu, *pause, *win, *lose, *loading, *controls;
		Camera *camera;
		Light *light;
		Object *playObj, *quitObj;
		glm::vec4 playPos = { 0.30677083f, 0.73802083f, 0.44351851f, 0.6648f };
		glm::vec4 quitPos = { 0.36822917f, 0.64583333f, 0.72962963f, 0.99722223f };
	} screen;
	// shader programs
	std::unordered_map<std::string, Shader*> program;
};