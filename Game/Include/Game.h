#pragma once
//defines
#define WINDOW_SCREEN_WIDTH 1920	
#define WINDOW_SCREEN_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
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
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include "FrameBufferObject.h"
//helpers
class Timer;	class Camera;
class Enemy;	class Player;
class Shader;	class Light;
class Level;	class Object;
class ParticleEmitterSoA;
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
	void reloadEnemies();
	~Game();
	void init(void(*_controllerInput)(unsigned short index, Input::Button button), void(*_controllerSpecial)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks));

	void initializeParticles();

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

	void drawAmmo();
	void reset();
	void createDropItem(glm::vec3 pos, int type = 0);
	void createCorpse(glm::vec3 pos);
private:
	std::vector<Sound*> soundList;
	std::vector<Sound*> dialogue;
	std::vector<ParticleEmitterSoA*> partEList;
	ParticleEmitterSoA* partE;
	FrameBufferObject fboD;

	// helpers
	Timer* timer = nullptr;
	float deltaTime = 0.f;
	State state = State::Menu;
	glm::vec2 windowSize;
	int dialogueMode = 0;
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
			{ 4.25f, 10.0f, 29.5f },
			{ 32.75f, 10.0f, 48.0f },
			//{ 151.0f, 10.0f, 11.0f },
			//{ 2.0f, 10.0f, 25.0f },
			{ 79.5f, 10.0f, 68.0f }
		};

		std::vector<glm::vec4> exitPosR =
		{
			{36.5f, 1.0f, 60.5f,	0.f	},	// Right
			{122.2f, 1.0f, 31.7f,	0.f	},	// Right
			{55.4f, 1.0f, 13.f,		1.f	},	// rotate to point south Right 
			{130.7f, 1.0f, 12.2f,	1.f	},	// rotate to point south Right
			{84.1f, 1.0f, 41.f,		1.f	}	// rotate to point south Right
			//{45.f, 1.0f ,27.2f	, },	// Left
			//{141.5f, 1.0f ,59.8f, },		// Left
			//{95.7f, 1.0f ,47.4f	, },	// rotate to point north Left
		};

		std::vector<glm::vec4> exitPosL =
		{
			{ 45.f, 1.0f, 27.2f,	0.f},	// Left
			{ 141.5f, 1.0f, 59.8f,	0.f},	// Left
			{ 95.7f, 1.0f, 47.1f,	1.f}	// rotate to point north Left
		};

		std::vector<Light*> lightPointers;
		//std::vector<Light*> lightNoFlash;
		//std::vector<Light> lights;
		Light *light, *light2, *light3;
		glm::vec3 start = { 4.f, 0.f, 6.f };
		glm::vec4 exit = { 77.f, 81.f, 67.f, 70.f };
		//glm::vec4 exit = { 77.f, 81.f, 67.f, 70.f };
		//glm::vec4 exit = { 77.f, 81.f, 67.f, 70.f };


		std::tuple<std::vector<glm::vec2>, std::vector<glm::vec2>, std::vector<glm::vec2>>
			enemies = {
			//{
			//	{ 2.86804, 30.8909 },		{ 32.4772, 47.1091 },		{ 63.368,  29.1091 },		{ 27.5863, 3.10911 },
			//	{ 33.5863, 3.10911 },		{ 35.368,  9.10911 },		{ 29.5863, 8.89089 },		{ 57.368,  3.10911 },
			//	{ 33.5863, 8.89089 },		{ 151.368, 20.8909 },		{ 127.368, 4.89089 },		{ 133.368, 4.89089 },
			//	{ 113.368, 4.89089 },		{ 105.368, 4.89089 },		{ 87.5863, 2.89089 },		{ 135.368, 11.1091 },
			//	{ 136.477, 42.0 },			{ 132.477, 42.0 },			{ 136.477, 46.0 },			{ 98.4771, 64.0 },
			//	{ 102.477, 64.0 },			{ 106.477, 64.0 },			{ 76.4771, 34.0 },			{ 80.4771, 34.0 },
			//	{ 33.248,  34.821 },		{ 29.131,  29.103 },		{ 50.228,  12.264 },		{ 111.795, 17.262 },
			//	{ 115.858, 17.976 },		{ 106.910, 37.245 },		{ 136.785, 55.583 },		{ 111.646, 63.259 }
			//},	{
			//	{ 34.544,  28.836 },		{ 32.371,  18.886 },		{ 96.425,  22.153 },		{ 99.169,  19.952 },
			//	{ 99.553,  22.368 },		{ 101.694, 21.489 },		{ 130.350, 16.604 },		{ 137.761, 16.219 },
			//	{ 119.206, 27.034 },		{ 119.045, 53.868 },		{ 103.488, 52.919 }
			//},	{
			//	{ 67.368,  15.1091 },		{ 51.5863, 44.8909 },		{ 151.586, 16.8909 },		{ 151.368, 12.8909 },
			//	{ 141.368, 58.0 },			{ 101.586, 64.0 },			{ 102.477, 36.0 },			{ 79.368,  64.0 } ,
			//	{ 71.923,  22.586 },		{ 147.219, 36.128 },		{ 149.117, 62.974 }
			//}
			{
				{4, 29.4},		{32.7, 50.2},		{63.5, 51.3},		{51.1, 44.6},		{23.6, 40.2},		{26.3, 22.8},		{19.5, 6.2},
				{66.8, 13.8},	{65.8, 20.8},		{93.3, 9.2},		{145.7, 18.1},		{114.3, 27.4},		{134.6, 35.4},		{125.9, 55},
				{111.2, 53.5},	{96.2, 53.4},		{85.5, 36.5},		{59.0, 30.2},		{21.7, 13.4},		{143.1, 42.2}
			},
			{
				{34.7, 18.4},	{79.5, 60.1},		{77, 20.7},			{118, 15.5},		{143.2, 57.4},		{101.6, 42.1}
			},
			{
				{147.9, 62.7},	{136.9, 41.5},		{102.5, 33.8},		{76.8, 48.2}
			}
		};
	} level;
	// characters
	Player* player;
	std::vector<Enemy*> enemies;
	std::tuple<Enemy*, Enemy*, Enemy*> enemys;
	Object *dropHP, *dropAmmo;
	std::vector<Object*> dropItems;
	Object *corpse, *exitR, *exitL;
	std::vector<Object*> permItems;
	bool lightOn = true;
	void loadDrops();
	void loadSignR();
	void loadSignL();
	void loadEnemies(), clearEnemies();
	void clearDrops();
	void clearItems();
	void clearPartEmitter();
	// hud
	struct Hud {
		Object *display;
		Object *healthBar;
		struct Ammo {
			Object *number;
			glm::vec3 positions[5] = {
				{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}
			};
			//Object *digit0, *digit1, *digit2, *digit3, *digit4, *digit5, *digit6, *digit7, *digit8, *digit9;
			//glm::vec3 pos1, pos2, pos3, pos4, pos5;
		} ammo;
		glm::vec3 angle = { 10.f, 0.f, 0.f };
		glm::vec4 red = { 1.f, 0.f, 0.f, 1.f };
		Light *light;
		Camera *camera;
	} hud;
	// screens
	struct Screen {
		Object *menu, *pause, *win, *lose, *loading, *controls;
		struct Button {
			Object *obj;
			glm::vec4 pos;
			float scale;
		} play, quit, resume, credits;
		Camera *camera;
		Light *light;
	} screen;
	// shader programs
	std::unordered_map<std::string, Shader*> program;
};
