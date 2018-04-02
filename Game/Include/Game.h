#pragma once
//defines
#define WINDOW_SCREEN_WIDTH 1920	
#define WINDOW_SCREEN_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FRAMES_PER_SECOND 60
#define InputControl void(*_controllerInput)(unsigned short index, Input::Button button), void(*_controllerSpecial)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks)
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
class Mesh;
class ParticleEmitterSoA;
class Assets;
namespace Input {
	using Triggers = std::pair<float, float>;
	using Sticks = std::pair<glm::vec2, glm::vec2>;
	enum Button;
	class XBox;
	enum class Keys {
		KeyW,	KeyA,	KeyS,	KeyD,
		KeyQ,	KeyE,	KeyR,	KeyF,
		KeyZ,	KeyX,	KeyC,	KeyV,
		KeyU,	KeyI,	KeyJ,	KeyK,
		Num1,	Num2,	Num3,	Num4,
		Esc,	Enter,	Space,	Delete,
		LShift,	RShift,	LAlt,	RAlt,
		LCtrl,	RCtrl
	};
}
enum class State { Menu, Play, Pause, Win, Lose, Control };

class Game {
public:
	// construct & destruct
	Game(int& argc, char **argv);
	~Game();
	Game* init(InputControl);
	
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
	FrameBufferObject fboD;
	Assets* assets;
	Timer* timer = nullptr;
	float deltaTime = 0.f;
	int score = 0;
	State state = State::Menu;
	glm::vec2 windowSize;
	// objects
	Player* player;
	Object *corpse;
	std::vector<Enemy*> enemies;
	Object *dropHP, *dropAmmo;
	std::vector<Object*> drops, items;
	ParticleEmitterSoA* partE;
	std::vector<ParticleEmitterSoA*> parts;
	// methods
	//init
	void initShaders();
	void initScreens();
	void initLevel();
	void initHUD();
	void initEnemies();
	void initParts();
	void initItems();
	void initSounds();
	//load
	void loadAssets();
	void loadEnemies();
	void loadDrops();
	void loadSigns();
	//clear
	void clearShaders();
	void clearSounds();
	void clearEnemies();
	void clearDrops();
	void clearItems();
	void clearParts();
	// creaters
	void reloadEnemies();
	void createDropItem(glm::vec3 pos, int type = 0);
	void createCorpse(glm::vec3 pos);
	// state change
	void start(), win(), reset();
	// time
	struct Times {
		float open, load, init, start, current, clear, end;
	} times;
	// input
	struct InputData {
		glm::vec2 mouse;
		int button, state;
		unsigned int keys = 0;
		Input::XBox *xBox;
		int numControllers = 1;
	} input;
	// level
	struct LevelData {
		Level* collision;
		Object *map, *hitboxes;
		Camera *camera;
		Light *light, *flashLight;
		std::vector<Light*> lights;

		std::vector<glm::vec3> lightsPos = {
			{ 4.25f, 10.0f, 29.5f },
			{ 32.75f, 10.0f, 48.0f },
			//{ 151.0f, 10.0f, 11.0f },
			//{ 2.0f, 10.0f, 25.0f },
			{ 79.5f, 10.0f, 68.0f }
		};

		std::vector<glm::vec2> drops[3] = {
			{		// random
			}, {	// health
				{ 34.5, 48.0 },		{ 30.0, 3.0 },		{ 151.6, 11.1 },	{ 123.6, 64.2 },
				{ 102.8, 38.8 }
			}, {	// ammo														
				{ 16.7, 10.4 },		{ 30.7, 48 },		{ 67.3, 3.4 },		{ 114.4, 12.3 },
				{ 139.7, 58 },		{ 85.7, 65 },		{ 41.2, 42.5 },		{ 21.3, 12.2 },
				{ 43.3, 22.8 },		{ 56.8, 2.8 },		{ 82.2, 2.1 },		{ 149.9, 22.2 },
				{ 152, 36.5 }
			}
		};

		glm::vec3 start = { 4.f, 0.f, 6.f };
		glm::vec4 exit = { 77.f, 81.f, 67.f, 70.f };
		Object *exitR, *exitL;
		
		std::vector<glm::vec4> exitPosR = {
			{36.5f, 1.0f, 60.5f,	0.f	},	// Right
			{122.2f, 1.0f, 31.7f,	0.f	},	// Right
			{55.4f, 1.0f, 13.f,		1.f	},	// rotate to point south Right 
			{130.7f, 1.0f, 12.2f,	1.f	},	// rotate to point south Right
			{84.1f, 1.0f, 41.f,		1.f	}	// rotate to point south Right
			//{45.f, 1.0f ,27.2f	, },	// Left
			//{141.5f, 1.0f ,59.8f, },		// Left
			//{95.7f, 1.0f ,47.4f	, },	// rotate to point north Left
		};

		std::vector<glm::vec4> exitPosL = {
			{ 45.f, 1.0f, 27.2f,	0.f},	// Left
			{ 141.5f, 1.0f, 59.8f,	0.f},	// Left
			{ 95.7f, 1.0f, 47.1f,	1.f}	// rotate to point north Left
		};
		
		struct EnemyData {
			EnemyData(std::string id, int a, int p, float hp, float v, float dmg, std::vector<glm::vec2> pos)
				: name(id), frames(a), points(p), stats(hp, v, dmg), positions(pos) {}
			std::string name;
			int frames, points;
			Enemy* type;
			struct EnemyStats {
				EnemyStats(float hp, float v, float dmg)
					: life(hp), moveSpeed(v), damage(dmg) {}
				float life, moveSpeed, damage;
			} stats;
			std::vector<glm::vec2> positions;
		} enemy[3] = {
			{    "damaged", 9, 100, 15.f, 250.f, 1.f, {
					{ 4.0, 29.4 },{ 32.7, 50.2 },{ 63.5, 51.3 },{ 51.1, 44.6 },
					{ 23.6, 40.2 },{ 26.3, 22.8 },{ 19.5, 6.2 },{ 66.8, 13.8 },
					{ 65.8, 20.8 },{ 93.3, 9.2 },{ 145.7, 18.1 },{ 114.3, 27.4 },
					{ 134.6, 35.4 },{ 125.9, 55 },{ 111.2, 53.5 },{ 96.2, 53.4 },
					{ 85.5, 36.5 },{ 59.0, 30.2 },{ 21.7, 13.4 },{ 143.1, 42.2 }
				}
			}, { "deformed", 13, 300, 20.f, 300.f, 3.f, {
					{ 34.7, 18.4 },{ 79.5, 60.1 },{ 77.0, 20.7 },{ 118, 15.5 },
					{ 143.2, 57.4 },{ 101.6, 42.1 }
				}
			}, { "demented", 5, 500, 10.f, 150.f, 5.f, {
					{ 147.9, 62.7 },	{ 136.9, 41.5 },	{ 102.5, 33.8 },	{ 76.8, 48.2 }
				}
			}
		};
	} level;
	// hud
	struct Hud {
		Light *light;
		Camera *camera;

		Object *display;
		Object *healthBar;

		glm::vec3 angle = { 10.f, 0.f, 0.f };

		struct Ammo {
			Object *number;
			std::vector<glm::vec3> positions = {
				{ 3.4f, 7.f, 3.8f },
				{ 3.65f, 7.f, 3.8f },
				{ 3.5f, 7.f, 3.4f },
				{ 3.75f, 7.f, 3.4f },
				{ 4.f, 7.f, 3.4f }
			};
		} ammo;
	} hud;
	// screens
	struct Screen {
		Camera *camera;
		Light *light;

		Object *loading, *credits, *menu, *controls, *pause, *win, *lose;

		struct ButtonData {
			ButtonData(float s, glm::vec3 c, glm::vec4 p)
				: scale(s), center(c), pos(p) {}
			float scale;
			Object *obj;
			glm::vec3 center;
			glm::vec4 pos;
		} play = { 1.7f, { 0.f, 0.2f, 0.3f }, { 0.30677083f, 0.73802083f, 0.44351851f, 0.6648f } },
			quit = { 1.5f, { 0.f, 0.2f, 1.75f }, { 0.36822917f, 0.64583333f, 0.72962963f, 0.99722223f } },
			resume = { 1.f, { 0.f, 0.2f, 1.75f }, { 0.36822917f, 0.64583333f, 1.72962963f, 0.99722223f } },
			credit = { 1.f, { 0.f, 0.2f, 1.75f }, { 0.36822917f, 0.64583333f, 0.92962963f, 0.99722223f } };
	} screen;
};
