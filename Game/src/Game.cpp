#include <iostream>
#include <memory>
#include <tuple>
#include <ctime>
#include <cstdlib>

#include <Windows.h>

#include <glm/glm.hpp>

#include "Assets.h"
#include "Level.h"
#include "Mesh.h"
#include "Game.h"
#include "Shader.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "Timer.h"
#include "Camera.h"
#include "Controller.h"
#include "Light.h"

Game::Game(int& argc, char** argv)
	: windowSize(WINDOW_WIDTH, WINDOW_HEIGHT) {
	// Memory Leak Detection
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	/* initialize the window and OpenGL properly */
	/// Request an OpenGL 4.4 compatibility
	/// A compatibility context is needed to use the provided rendering utilities
	/// which are written in OpenGL 1.1
	///glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2); ///2/4
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - windowSize.x) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - windowSize.y) / 2);
	glutInitWindowSize(windowSize.x, windowSize.y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Max Containment");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutFullScreen();

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW could not be initialized." << std::endl;
		system("pause");
		exit(0);
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << ", GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Initialize GL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	srand(time(NULL));
}

Game::~Game() {
	// misc
	delete timer;
	delete assets;
	delete player;
	delete dropHP;
	delete dropAmmo;
	delete input.xBox;
	// level components
	delete level.collision;
	delete level.hitboxes;
	delete level.map;
	delete level.camera;
	delete level.light;
	delete level.light2;
	delete level.light3;
	// screen components
	delete screen.loading;
	delete screen.controls;
	delete screen.menu;
	delete screen.pause;
	delete screen.win;
	delete screen.lose;
	delete screen.camera;
	delete screen.light;
	delete screen.play.obj;
	delete screen.quit.obj;
	// hud components
	delete hud.display;
	delete hud.healthBar;
	delete hud.camera;
	delete hud.light;
	delete hud.ammo.number;
	delete[] hud.ammo.positions;
	// stuff
	level.drops.first.clear();
	level.drops.second.clear();
	std::get<0>(level.enemy.positions).clear();
	std::get<1>(level.enemy.positions).clear();
	std::get<2>(level.enemy.positions).clear();
	delete std::get<0>(level.enemy.type);
	delete std::get<1>(level.enemy.type);
	delete std::get<2>(level.enemy.type);
}

Game* Game::init(inputControl) {
	// Initialize game elements
	timer = new Timer();
	assets = Assets::getInstance();
	input.xBox = new Input::XBox();
	input.xBox->callback = _controllerInput;
	input.xBox->special = _controllerSpecial;

	// Initialize loading screen
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screen.camera = new Camera(windowSize);
	screen.camera->setPosition({ 0.f, 5.f, 0.01f });
	screen.camera->update(glm::vec3(0.f));
	screen.light = new Light();
	screen.light->type = (unsigned int)Type::Light::DIRECTIONAL;
	screen.light->direction = { 0.f, -1.f, 0.f, 0.f };
	screen.light->ambient = { 0.15f, 0.15f, 0.15f };
	screen.light->diffuse = { 0.7f, 0.7f, 0.7f };
	screen.light->specular = { 1.f, 1.f, 1.f };
	screen.light->specExponent = 50.f;
	screen.light->attenuation = { 1.f, 0.1f, 0.01f };
	screen.light->original = screen.light->position;
	screen.loading = new Object();
	screen.loading->loadMesh(assets->loadMesh("screen", "screen.obj"));
	screen.loading->loadTexture(Type::DIFFUSE, assets->loadTexture("loading", "loading.png"));
	program["Phong"] = new Shader();
	if (!program["Phong"]->load("assets/shaders/Phong.vert", "assets/shaders/Phong.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
	screen.loading->draw(program["Phong"], screen.camera, screen.light);

	glutSwapBuffers();
	return this;
}

<<<<<<< HEAD
	Sound* sound = new Sound("assets/sounds/game soundtrack.wav", true, 2);
	//Sound* sound = new Sound("assets/sounds/SW.mp3", true, 2);
	soundList.push_back(sound);
	soundList[0]->createChannel(2);
	soundList[0]->setVolume(0.05f);

	//Initialize Assets
	// level
	assets->loadMesh("level", "Laboratory Level Triangulated.obj");
	assets->loadMesh("hitboxes", "Laboratory Level Hitboxes Triangulated.obj");
	assets->loadTexture("level color", "Lab_textures.png");
	assets->loadTexture("level normal", "Lab_normals.png");
	// screens
	assets->loadMesh("screen", "screen.obj");
	assets->loadTexture("pause", "pause.png");
	assets->loadTexture("win", "win.png");
	assets->loadTexture("lose", "lose.png");
	assets->loadTexture("controls", "controlsNew.png");
	assets->loadTexture("menu", "menu.png");
	// buttons
	assets->loadMesh("play", "Play Button.obj");
	assets->loadMesh("quit", "Quit Button.obj");
	// player
	assets->loadMesh("player", "character_model.obj");
	assets->loadTexture("player color", "character texture.png");
	assets->loadTexture("player normal", "character normals.png");
	// other
	assets->loadTexture("fullSpecular", "fullSpecular.png");
	assets->loadTexture("noSpecular", "noSpecular.png");
	assets->loadMesh("bullet", "bullet.obj");
	// hud
	assets->loadMesh("hud", "hud.obj");
	assets->loadMesh("hp bar", "hp bar.obj");
	assets->loadTexture("hud", "hud.png");
	assets->loadTexture("numbers", "Numbers.png");
	// drops
	assets->loadMesh("health", "health_pack.obj");
	assets->loadMesh("ammo", "ammo_pack.obj");
	assets->loadTexture("item", "items.png");
	// enemy
	assets->loadMesh("enemy0", "enemy3_model.obj");
	assets->loadMesh("enemy1", "enemy_model.obj");
	assets->loadMesh("enemy2", "enemy2_model.obj");
	assets->loadTexture("enemy0 color", "enemy3 texture.png");
	assets->loadTexture("enemy1 color", "enemy texture.png");
	assets->loadTexture("enemy2 color", "enemy2 texture.png");
	assets->loadTexture("enemy0 normal", "enemy3N.png");
	assets->loadTexture("enemy1 normal", "enemy1N.png");
	assets->loadTexture("enemy2 normal", "enemy2N.png");


	/*program["PhongSpot"] = new Shader();
	if (!program["PhongSpot"]->load("assets/shaders/Phong.vert", "assets/shaders/phongSpot.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}*/
=======
void Game::initShaders() {
	// Initialize shaders
	program["PhongSpot"] = new Shader();
	if (!program["PhongSpot"]->load("assets/shaders/Phong.vert", "assets/shaders/phongSpot.frag")) {
		std::cout << "PhongSpot Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["passThrough"] = new Shader();
	if (!program["passThrough"]->load("assets/shaders/passThrough.vert", "assets/shaders/passThrough.frag")) {
		std::cout << "PassThrough Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["PhongNoTexture"] = new Shader();
	if (!program["PhongNoTexture"]->load("assets/shaders/Phong.vert", "assets/shaders/PhongNoTexture.frag")) {
		std::cout << "PhongNoTexture Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["PhongColorSides"] = new Shader();
	if (!program["PhongColorSides"]->load("assets/shaders/PhongColorSides.vert", "assets/shaders/PhongColorSides.frag")) {
		std::cout << "PhongColorSides Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["numbers"] = new Shader();
	if (!program["numbers"]->load("assets/shaders/Phong.vert", "assets/shaders/numbers.frag")) {
		std::cout << "Number Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
}
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5

void Game::initHUD() {
	// Initialize HUD
	hud.display = new Object();
	hud.healthBar = new Object();
	hud.display->loadMesh(assets->meshes["hud"]);
	hud.healthBar->loadMesh(assets->meshes["hp bar"]);
	hud.display->loadTexture(Type::Texture::DIFFUSE, assets->textures["hud"]);
	hud.healthBar->color = glm::vec4(consts.color.red, 1.f);
	hud.display->setRotation(hud.angle);
	hud.healthBar->setRotation(hud.angle);
	hud.display->setScale(glm::vec3(0.395f));
	hud.light = new Light();
	hud.light->type = (unsigned int)Type::Light::DIRECTIONAL;
	hud.light->direction = glm::vec4(consts.direction.down, 0.f);
	hud.light->original = level.light->direction;
	hud.light->ambient = { 0.15f, 0.15f, 0.15f };
	hud.light->diffuse = { 0.7f, 0.7f, 0.7f };
	hud.light->specular = { 1.f, 1.f, 1.f };
	hud.light->specExponent = 50.f;
	hud.light->attenuation = { 1.f, 0.1f, 0.01f };
	hud.ammo.number = new Object();
	hud.ammo.number->loadMesh(assets->meshes["screen"]);
	hud.ammo.number->setScale(glm::vec3(0.05f, 1.f, 0.1f));
	hud.ammo.number->setRotation(hud.angle);

}

void Game::initLevel() {
	// stuff
	level.map = (new Object())
		->loadMesh(assets->meshes["level"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["level color"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["level normal"]);
	level.collision = new Level(0.5f);
	level.collision->load("assets/meshes/Laboratory Level Hitboxes.obj");
<<<<<<< HEAD
	level.hitboxes->loadMesh(assets->meshes["hitboxes"]);
	level.map->loadTexture(Type::Texture::DIFFUSE, assets->textures["level color"]);
	level.map->loadTexture(Type::Texture::NORMAL, assets->textures["level normal"]);
	level.hitboxes->update(deltaTime);
	level.camera = new Camera(windowSize);
	level.camera->setPosition({ 0.f, 12.5f, 2.5f });
	level.camera->update({ 0.f,0.f,0.f });

=======
	level.hitboxes = (new Object({ 0.f, -8.f, 0.f }))
		->loadMesh(assets->meshes["hitboxes"])
		->update(deltaTime);
	level.camera = (new Camera(windowSize))
		->setPosition({ 0.f, 12.5f, 2.5f })
		->update(glm::vec3(0.f));

	// lights
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
	level.light = new Light();
	level.light->type = (unsigned int)Type::Light::SPOT;
	level.light->type = (unsigned int)Type::Light::POINT;
	level.light->position = { 0.f, 1.f, 0.f, 1.f };
	//level.light->original = level.light->position;
	level.light->ambient = { 0.15f, 0.15f, 0.15f };
	level.light->diffuse = { 0.7f, 0.7f, 0.7f };
	level.light->specular = { 1.f, 1.f, 1.f };
	level.light->specExponent = 50.f;
<<<<<<< HEAD
	level.light->attenuation = { 1.f, 1.f, 1.f };
	level.lightPointers.push_back(level.light);
	//level.lightNoFlash.push_back(level.light);

	// @@@@@ FOR SEAN @@@@@@	Loads in the array containing all the lights in the level
	for (int i = 0; i < level.lightsPos.size(); i++)
	{
		Light *light = new Light();
		light->type = (unsigned int)Type::Light::SPOT;
		light->position = { level.lightsPos[i], 1.f };
		std::cout << "Load Light " << i << ':' << light->position.x << '/' << light->position.z << std::endl;
		light->direction = { 0.f, -1.0f, 0.f, 0.f };
=======
	level.light->spotExponent = 1.f;
	level.light->cutoff = glm::radians(55.f);
	level.light->innerCutoff = glm::radians(1.f);
	level.light->partial = 0.3;
	level.light->attenuation = { 0.5f, 0.1f, 0.01f };
	level.lights.push_back(level.light);
	
	// @@@@@ FOR SEAN @@@@@@	Loads in the array containing all the lights in the level
	for (auto& pos : level.lightPos) {
		std::cout << "Load Light " << lights.size() << std::endl;
		Light *light = new Light();
		light->type = (unsigned int)Type::Light::SPOT;
		light->position = glm::vec4(pos, 1.f);
		light->direction = glm::vec4(consts.direction.down, 0.f);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		light->ambient = { 0.15f, 0.15f, 0.15f };
		light->diffuse = { 0.7f, 0.7f, 0.7f };
		light->specular = { 1.f, 1.f, 1.f };
		light->specExponent = 50.f;
		light->spotExponent = 1.f;
		light->cutoff = glm::radians(55.f);
		light->innerCutoff = glm::radians(1.f);
		light->partial = 0.1;
		light->attenuation = { 0.5f, 0.1f, 0.01f };
<<<<<<< HEAD
		level.lightPointers.push_back(light);
		//level.lightNoFlash.push_back(light);
=======
		level.lights.push_back(light);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
	}

	// Flashlight gets put into the last slot
	level.light2 = new Light();
	level.light2->type = (unsigned int)Type::Light::SPOT;
	level.light2->position = { 0.f, 0.0f, 0.f, 1.f };
	level.light2->direction = { 0.f, 0.5f, 0.f, 0.f };
	level.light2->original = level.light2->position;
	level.light2->ambient = { 0.15f, 0.15f, 0.15f };
	level.light2->diffuse = { 0.7f, 0.7f, 0.7f };
	level.light2->specular = { 1.f, 1.f, 1.f };
	level.light2->specExponent = 50.f;
	level.light2->attenuation = { 1.f, 1.f, 1.f }; // WHY!!
	level.light2->spotExponent = 1.f;
	level.light2->cutoff = glm::radians(55.f);
	level.light2->innerCutoff = glm::radians(1.f);
	level.light2->partial = 0.1;
	level.light2->attenuation = { 0.5f, 0.1f, 0.01f };
	level.lights.push_back(level.light2);

	/*level.light3 = new Light();
	level.light3->type = (unsigned int)Type::Light::SPOT;
	level.light3->position = { 0.f, 0.0f, 0.f, 1.f };
	level.light3->direction = { 0.f, 0.5f, 0.f, 0.f };
	level.light3->original = level.light3->position;
	level.light3->ambient = { 0.15f, 0.15f, 0.15f };
	level.light3->diffuse = { 0.7f, 0.7f, 0.7f };
	level.light3->specular = { 1.f, 1.f, 1.f };
	level.light3->specExponent = 50.f;
	level.light3->spotExponent = 1.f;
	level.light3->cutoff = glm::radians(55.f);
	level.light3->innerCutoff = glm::radians(1.f);
	level.light3->partial = 0.3;
<<<<<<< HEAD
	level.light3->attenuation = { 1.f, 0.5f, 0.1f };*/

	// Initialize images
	screen.menu = new Object();
	screen.pause = new Object();
	screen.lose = new Object();
	screen.win = new Object();
	screen.controls = new Object();

	screen.menu->loadMesh(assets->meshes["screen"]);
	screen.pause->loadMesh(assets->meshes["screen"]);
	screen.win->loadMesh(assets->meshes["screen"]);
	screen.lose->loadMesh(assets->meshes["screen"]);
	screen.controls->loadMesh(assets->meshes["screen"]);

	screen.menu->loadTexture(Type::DIFFUSE, assets->textures["menu"]);
	screen.pause->loadTexture(Type::DIFFUSE, assets->textures["pause"]);
	screen.win->loadTexture(Type::DIFFUSE, assets->textures["win"]);
	screen.lose->loadTexture(Type::DIFFUSE, assets->textures["lose"]);
	screen.controls->loadTexture(Type::DIFFUSE, assets->textures["controls"]);

	//screen.pause->setRotation({ 10.f, 0.f, 0.f });
	//screen.pause->setPosition({ 0.f, 10.f, 0.f });
	//screen.pause->update(0.f);
=======
	level.light3->attenuation = { 1.f, 0.5f, 0.1f };

}

void Game::initScreens() {
	// Initialize Images
	screen.controls = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["controls"]);

	screen.menu = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["menu"]);

	screen.pause = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["pause"]);

	screen.win = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["win"]);

	screen.lose = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["lose"]);


	// reposition the pause screen between the level and camera
	//screen.pause->setRotation(hud.angle)
	//	->setPosition({ 0.f, 10.f, 0.f })
	//	->update();
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5

	// Initialize buttons
	screen.play.obj = (new Object({ 0.f, 0.2f, 0.3f }))
		->loadMesh(assets->meshes["play"])
		->setScale(glm::vec3(1.7f))
		->update();
	screen.play.obj->color = glm::vec4(consts.color.white, 1.f);
	screen.play.pos = { 0.30677083f, 0.73802083f, 0.44351851f, 0.6648f };

	screen.quit.obj = (new Object({ 0.f, 0.2f, 1.75f }))
		->loadMesh(assets->meshes["quit"])
		->setScale(glm::vec3(1.5f))
		->update();
	screen.quit.obj->color = glm::vec4(consts.color.white, 1.f);
	screen.quit.pos = { 0.36822917f, 0.64583333f, 0.72962963f, 0.99722223f };

<<<<<<< HEAD
	// Initialize shaders
	program["PhongSpot"] = new Shader();
	if (!program["PhongSpot"]->load("assets/shaders/Phong.vert", "assets/shaders/phongSpot.frag")) {
		std::cout << "PhongSpot Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["passThrough"] = new Shader();
	if (!program["passThrough"]->load("assets/shaders/passThrough.vert", "assets/shaders/passThrough.frag")) {
		std::cout << "PassThrough Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["PhongNoTexture"] = new Shader();
	if (!program["PhongNoTexture"]->load("assets/shaders/Phong.vert", "assets/shaders/PhongNoTexture.frag")) {
		std::cout << "PhongNoTexture Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["PhongColorSides"] = new Shader();
	if (!program["PhongColorSides"]->load("assets/shaders/PhongColorSides.vert", "assets/shaders/PhongColorSides.frag")) {
		std::cout << "PhongColorSides Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["numbers"] = new Shader();
	if (!program["numbers"]->load("assets/shaders/Phong.vert", "assets/shaders/numbers.frag")) {
		std::cout << "Number Shaders failed to initialize." << std::endl;
		system("pause");	exit(0);
	}
	program["DropItems"] = new Shader();
	if (!program["DropItems"]->load("assets/shaders/Phong.vert", "assets/shaders/DropItems.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
	program["NoFlash"] = new Shader();
	if (!program["NoFlash"]->load("assets/shaders/Phong.vert", "assets/shaders/NoFlash.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
	program["NoFlashNoNorm"] = new Shader();
	if (!program["NoFlashNoNorm"]->load("assets/shaders/Phong.vert", "assets/shaders/NoFlashNoNorm.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
=======
}

void Game::initEnemies() {
	std::get<0>(level.enemy.type) = new Enemy();
	std::get<1>(level.enemy.type) = new Enemy();
	std::get<2>(level.enemy.type) = new Enemy();

	std::get<0>(level.enemy.type)->life = std::get<0>(level.enemy.stats.life);
	std::get<1>(level.enemy.type)->life = std::get<1>(level.enemy.stats.life);
	std::get<2>(level.enemy.type)->life = std::get<2>(level.enemy.stats.life);
	std::get<0>(level.enemy.type)->moveSpeed = std::get<0>(level.enemy.stats.moveSpeed);
	std::get<1>(level.enemy.type)->moveSpeed = std::get<1>(level.enemy.stats.moveSpeed);
	std::get<2>(level.enemy.type)->moveSpeed = std::get<2>(level.enemy.stats.moveSpeed);
	std::get<0>(level.enemy.type)->damage = std::get<0>(level.enemy.stats.damage);
	std::get<1>(level.enemy.type)->damage = std::get<1>(level.enemy.stats.damage);
	std::get<2>(level.enemy.type)->damage = std::get<2>(level.enemy.stats.damage);

	std::get<0>(level.enemy.type)->loadMesh(assets->meshes["enemy0"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["enemy0 color"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["noSpecular"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["enemy0 normal"]);
	std::get<1>(level.enemy.type)->loadMesh(assets->meshes["enemy1"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["enemy1 color"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["noSpecular"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["enemy1 normal"]);
	std::get<2>(level.enemy.type)->loadMesh(assets->meshes["enemy2"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["enemy2 color"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["noSpecular"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["enemy2 normal"]);
}

Game* Game::load() {
	//Initialize Assets
	loadAssets();
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5

	// Initialize Sounds
	//game soundtrack.wav
	sounds["SW"] = (new Sound("assets/sounds/SW.mp3", true, 2))
		->createChannel(2)
		->setVolume(0.05);
	sounds["ambient"] = new Sound("assets/sounds/ambient machine noise.wav", true, 2);
	sounds["gunshot"] = new Sound("assets/sounds/Gunshot_sound.wav", true, 3);
	sounds["reload"] = new Sound("assets/sounds/Reload_sound.wav", false, 3);

	// Initialize Shaders
	initShaders();
	// Initialize Level
	initLevel();
	// Initialize Screens
	initScreens();
	// Initialize HUD
	initHUD();
	// Initialize Enemies
	initEnemies();

	// Initialize Player
	player = new Player();
	player->loadMesh(assets->meshes["player"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["player color"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["player normal"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	player->reset(level.start)
		->bullet->loadMesh(assets->meshes["bullet"])
			->color = { 1.0f , 1.0f, 1.0f, 0.3f };

	// Initialize Drops
	dropHP = (new Object())
		->loadMesh(assets->meshes["health"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["item"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["item"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	dropHP->life = 5.0f;

	dropAmmo = (new Object())
		->loadMesh(assets->meshes["ammo"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["item"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["item"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	dropAmmo->ammo = 30.0f;

<<<<<<< HEAD
	std::cout << glutGet(GLUT_ELAPSED_TIME) << " milliseconds to load in things" << std::endl;
	return this;
}

<<<<<<< HEAD
	// Initialize Enemies
	std::get<0>(enemys) = new Enemy();			std::get<1>(enemys) = new Enemy();			std::get<2>(enemys) = new Enemy();
	std::get<0>(enemys)->life = 15.f;			std::get<1>(enemys)->life = 20.f;			std::get<2>(enemys)->life = 10.f;
	std::get<0>(enemys)->movementSpeed = 250.f;	std::get<1>(enemys)->movementSpeed = 350.f;	std::get<2>(enemys)->movementSpeed = 150.f;
	std::get<0>(enemys)->damage = 1.f;			std::get<1>(enemys)->damage = 3.f;			std::get<2>(enemys)->damage = 5.f;
	std::get<0>(enemys)->loadMesh(assets->meshes["enemy0"]);
	std::get<1>(enemys)->loadMesh(assets->meshes["enemy1"]);
	std::get<2>(enemys)->loadMesh(assets->meshes["enemy2"]);
	std::get<0>(enemys)->loadTexture(Type::Texture::DIFFUSE, assets->textures["enemy0 color"]);
	std::get<1>(enemys)->loadTexture(Type::Texture::DIFFUSE, assets->textures["enemy2 color"]);
	std::get<2>(enemys)->loadTexture(Type::Texture::DIFFUSE, assets->textures["enemy2 color"]);
	std::get<0>(enemys)->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	std::get<1>(enemys)->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	std::get<2>(enemys)->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	std::get<0>(enemys)->loadTexture(Type::Texture::NORMAL, assets->textures["enemy0 normal"]);
	std::get<1>(enemys)->loadTexture(Type::Texture::NORMAL, assets->textures["enemy1 normal"]);
	std::get<2>(enemys)->loadTexture(Type::Texture::NORMAL, assets->textures["enemy2 normal"]);
	loadEnemies();

	loadDrops();

	std::cout << glutGet(GLUT_ELAPSED_TIME) << " milliseconds to load in things" << std::endl;
	//soundList[1]->createChannel();
}



void Game::loadDrops()
{
	//health
	createDropItem(glm::vec3(34.5, 0, 48), 1);
	createDropItem(glm::vec3(30, 0, 3), 1);
	createDropItem(glm::vec3(151.6, 0, 11.1), 1);
	createDropItem(glm::vec3(123.6, 0, 64.2), 1);
	createDropItem(glm::vec3(102.8, 0, 38.8), 1);

	createDropItem(glm::vec3(16.7, 0, 10.4), 2);
	createDropItem(glm::vec3(30.7, 0, 48), 2);
	createDropItem(glm::vec3(67.3, 0, 3.4), 2);
	createDropItem(glm::vec3(114.4, 0, 12.3), 2);
	createDropItem(glm::vec3(139.7, 0, 58), 2);
	createDropItem(glm::vec3(85.7, 0, 65), 2);
=======
void Game::loadAssets() {
	// level
	assets->loadMesh("level", "Laboratory Level Triangulated.obj");
	assets->loadMesh("hitboxes", "Laboratory Level Hitboxes Triangulated.obj");
	assets->loadTexture("level color", "Lab_textures.png");
	assets->loadTexture("level normal", "Lab_normals.png");
	// screens
	assets->loadMesh("screen", "screen.obj");
	assets->loadTexture("pause", "pause.png");
	assets->loadTexture("win", "win.png");
	assets->loadTexture("lose", "lose.png");
	assets->loadTexture("controls", "controls.png");
	assets->loadTexture("menu", "menu.png");
	// buttons
	assets->loadMesh("play", "Play Button.obj");
	assets->loadMesh("quit", "Quit Button.obj");
	// player
	assets->loadMesh("player", "character_model.obj");
	assets->loadTexture("player color", "character texture.png");
	assets->loadTexture("player normal", "character normals.png");
	// other
	assets->loadTexture("fullSpecular", "fullSpecular.png");
	assets->loadTexture("noSpecular", "noSpecular.png");
	assets->loadMesh("bullet", "bullet.obj");
	// hud
	assets->loadMesh("hud", "hud.obj");
	assets->loadMesh("hp bar", "hp bar.obj");
	assets->loadTexture("hud", "hud.png");
	assets->loadTexture("numbers", "Numbers.png");
	// drops
	assets->loadMesh("health", "health_pack.obj");
	assets->loadMesh("ammo", "ammo_pack.obj");
	assets->loadTexture("item", "items.png");
	// enemy
	assets->loadMesh("enemy0", "enemy3_model.obj");
	assets->loadMesh("enemy1", "enemy_model.obj");
	assets->loadMesh("enemy2", "enemy2_model.obj");
	assets->loadTexture("enemy0 color", "enemy3 texture.png");
	assets->loadTexture("enemy1 color", "enemy texture.png");
	assets->loadTexture("enemy2 color", "enemy2 texture.png");
	assets->loadTexture("enemy0 normal", "enemy3N.png");
	assets->loadTexture("enemy1 normal", "enemy1N.png");
	assets->loadTexture("enemy2 normal", "enemy2N.png");

>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
}

void Game::loadEnemies() {
	for (auto& pos : std::get<0>(level.enemy.positions)) {
		std::get<0>(level.enemy.type)->setPosition({ pos.x, 0.f, pos.y });
		enemies.push_back(new Enemy(*std::get<0>(level.enemy.type)));
	}
	for (auto& pos : std::get<1>(level.enemy.positions)) {
		std::get<1>(level.enemy.type)->setPosition({ pos.x, 0.f, pos.y });
		enemies.push_back(new Enemy(*std::get<1>(level.enemy.type)));
	}
	for (auto& pos : std::get<2>(level.enemy.positions)) {
		std::get<2>(level.enemy.type)->setPosition({ pos.x, 0.f, pos.y });
		enemies.push_back(new Enemy(*std::get<2>(level.enemy.type)));
	}
}

void Game::loadDrops() {
	// health
	for (auto& pos : level.drops.first)
		createDropItem({ pos.x, 0.f, pos.y }, 1);
	// ammo
	for (auto& pos : level.drops.second)
		createDropItem({ pos.x, 0.f, pos.y }, 2);
}

Game* Game::clear() {
	clearShaders();
	clearSounds();
	clearEnemies();
	clearDrops();
	return this;
}

void Game::clearShaders() {
	for (auto& prog : program) {
		prog.second->unload();
		delete prog.second;
	}
	program.clear();
}

void Game::clearSounds() {
	for (auto& sound : sounds) {
		sound.second->unload();
		delete sound.second;
	}
	sounds.clear();
}

void Game::clearEnemies() {
	for (auto& enemy : enemies)
		delete enemy;
	enemies.clear();
}

void Game::clearDrops() {
	for (auto& drop : drops)
		delete drop;
	drops.clear();
}

void Game::update() {
	deltaTime = timer->update();
	input.xBox->update();
	for (auto& sound : sounds)
		sound.second->update();

<<<<<<< HEAD
	if (state == State::Menu) {
		//soundList[1]->stopSound();
		//soundList[0]->playSound();
	}
	else if (state == State::Play) {
		//soundList[0]->stopSound();
		//soundList[1]->playSound();
=======
	if (state == State::Menu)	{
		//sounds["ambient"]->stopSound();
		//sounds["SW"]->playSound();
	} else if (state == State::Play) {
		//sounds["SW"]->stopSound();
		//sounds["ambient"]->playSound();
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		if (player->reloadCd > 0.0f && !player->reloaded) {
			sounds["gunshot"]->stopSound();
			player->reloaded = true;
			sounds["reload"]->playSound(3)
				->setVolume(0.5f);
		}

		//sounds["ambient"]->createChannel();
		// player movement
		if (Helper::bitState(input.keys, Input::Keys::KeyW) && !Helper::bitState(input.keys, Input::Keys::KeyS))
			player->acceleration.z = -1.f;
		else if (Helper::bitState(input.keys, Input::Keys::KeyS) && !Helper::bitState(input.keys, Input::Keys::KeyW))
			player->acceleration.z = 1.f;
		else if (!input.xBox->getConnected(0))
			player->acceleration.z = 0.f;
		if (Helper::bitState(input.keys, Input::Keys::KeyA) && !Helper::bitState(input.keys, Input::Keys::KeyD))
			player->acceleration.x = -1.f;
		else if (Helper::bitState(input.keys, Input::Keys::KeyD) && !Helper::bitState(input.keys, Input::Keys::KeyA))
			player->acceleration.x = 1.f;
		else if (!input.xBox->getConnected(0))
			player->acceleration.x = 0.f;
		if (glm::length(player->acceleration) > 0.f)
			player->acceleration = glm::normalize(player->acceleration);
		if (Helper::bitState(input.keys, Input::Keys::KeyR))
			player->reload = true;
		player->update(deltaTime, level.collision);
		glm::vec3 temp = player->getPosition();
		//std::cout << temp.x << '/' << temp.y << '/' << temp.z << std::endl;
		//std::cout << rand() % 100 << std::endl

		// stuff based on player
		hud.display->setPosition(temp + glm::vec3(0.004f, 10.6f, 1.967f))
			->update(deltaTime);
		//screen.pause->setPosition(temp + glm::vec3(0.f, 10.f, 0.f))
		//	->update(deltaTime);
		hud.healthBar->setPosition(temp + glm::vec3(-1.846f, 10.4f, 3.027f))
			->setScale({ player->life * 0.0365f, 0.f, 0.035f })
			->update(deltaTime);
		hud.ammo.number->update(deltaTime);
<<<<<<< HEAD
		level.camera->update(player->getPosition());
		// Point Light Position
		level.lightPointers[0]->position = glm::vec4(player->getPosition() + glm::vec3(0.f, 2.f, 0.f), 1.f);
		// Spot Light Position and Rotation
		level.lightPointers[level.lightPointers.size() - 1]->position = glm::vec4(player->getPosition() + glm::vec3(0.f, 1.f, 0.f), 1.f);
		float angle = glm::radians(player->getRotation().y);
		level.lightPointers[level.lightPointers.size() - 1]->direction = { cos(angle), -0.1f, -sin(angle), 0.f };
=======
		level.camera->update(temp);
		level.light->position = glm::vec4(temp + glm::vec3(0.f, 1.f, 0.f), 1.f);
		level.light2->position = glm::vec4(temp + glm::vec3(0.f, 2.f, 0.f), 1.f);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		//level.light3->position = glm::vec4(player->getPosition() + glm::vec3(0.f, 0.1f, 0.f), 1.f);
		float angle = glm::radians(player->getRotation().y);
		//level.light3->direction = { cos(angle), 0.45f, -sin(angle), 0.f };

		// bullet collision
		for (auto& bullet : player->bullets) {
			if (bullet->cooldown <= 0.f) {
				float bang = glm::radians(bullet->getRotation().y);
				float distFromPlayer = 20.f;
				Enemy* target = nullptr;
				for (auto& enemy : enemies) {
					glm::vec3 diff = enemy->getPosition() - bullet->getPosition();
					float dang = Helper::getAngle(diff);
					float dist = abs(sin(bang - dang) * glm::length(diff));
					float distToEnemy = cos(bang - dang) * glm::length(diff);
					//std::cout << glm::degrees(bang) << '\t' << glm::degrees(dang) << '\t' << dist << '\t' << distFromPlayer << "vs" << distToEnemy << '\t' << enemies[j]->life << std::endl;
					// check if enemy is along the bullet path && not too far away
					if (dist < 1.f && distFromPlayer > distToEnemy && distToEnemy > 0) {
						target = enemy;
						distFromPlayer = distToEnemy;
					}
				}
				// enemy hit
				if (target != nullptr) {
					target->life -= 2.5;
					target->triggered = true;
					target->knockbackCD = 0.2f;
<<<<<<< HEAD
					glm::vec3 dif = target->getPosition() - player->getPosition();
					target->setPosition(target->getPosition() + glm::normalize(dif) * 0.1f);
=======
					glm::vec3 dif = target->getPosition() - temp;
					target->setPosition(target->getPosition() + glm::normalize(dif) * 0.5f);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
					bullet->cooldown = 1.f;
				}
			}
		}
		// enemy update
		for (int i = 0; i < enemies.size(); i++) {
			glm::vec3 diff = enemies[i]->getPosition() - player->getPosition();
			if (enemies[i]->knockbackCD > 0)
				enemies[i]->knockbackCD -= deltaTime / 1000;
			// aim towards player
			enemies[i]->setRotation({ 0.f, Helper::getAngle(diff) - 90.f, 0.f });
			// hurt player
			if (enemies[i]->cooldown <= 0.f)
				if (glm::length(diff) < 1.f) {
					player->life -= enemies[i]->damage;
					enemies[i]->cooldown = enemies[i]->attackSpeed;
				}
			float bang = glm::radians(player->getRotation().y);
			float dang = atan2(-diff.z, diff.x);
			float dist = abs(sin(bang - dang) * glm::length(diff));
			//std::cout << bang << '/' << dang << '/' << dist << std::endl;
			// seek towards player
			if (((glm::length(diff) < 5.0f) || enemies[i]->triggered || glm::length(diff) < 10.0f && dist < 2.f && lightOn) && glm::length(diff) > 1.0f && enemies[i]->knockbackCD <= 0)
				enemies[i]->setVelocity(-glm::normalize(diff));
<<<<<<< HEAD
=======
			//enemies[i]->setVelocity({ 0.0f, 0.0f, 0.0f });
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
			else
				enemies[i]->setVelocity(glm::vec3(0.f));
			// update enemy
			enemies[i]->update(deltaTime, level.collision);
			// kill enemy
			if (enemies[i]->life <= 0.f) {
				createDropItem(enemies[i]->getPosition());
				delete enemies[i];
				enemies.erase(i + enemies.begin());
				i--;
			}
		}
		// player loses
		if (player->life <= 0.f)
			state = State::Lose;
		// player wins
		if (Helper::inside({ temp.x, temp.z }, level.exit))
			state = State::Win;
		// item drops
<<<<<<< HEAD
		for (int i = 0; i < dropItems.size(); i++)
			if (!dropItems[i]->collect) {
				float dist = glm::length(pPos - dropItems[i]->getPosition());
				if (dist < 0.3) {
					//std::cout << dropItems[i]->ammo << '/' << dropItems[i]->hp << std::endl;
					dropItems[i]->collect = true;
					player->ammoDepo += dropItems[i]->ammo;
					if (player->life < 20.0f) {
						if ((player->life + dropItems[i]->life) > 20.0f)
							player->life = 20.0f;
						else
							player->life += dropItems[i]->life;
					}
					else if (player->life == 20.0f & dropItems[i]->life > 0.0f)
						dropItems[i]->collect = false;
=======
		for (auto& drop : drops)
			if (!drop->collect) {
				if (glm::length(temp - drop->getPosition()) < 0.3) {
					//std::cout << drop->ammo << '/' << drop->life << std::endl;
					player->ammoDepo += drop->ammo;
					drop->collect = Helper::incLife(player->life, player->MaxLife, drop->life);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
				}
			}
	}
}

void Game::draw() {
	// Set up scene
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(glm::radians(60.0f), windowSize.x / windowSize.y, 0.001f, 10000.0f);
	//glMatrixMode(GL_MODELVIEW);

	//std::cout << glm::degrees(level.light->cutoff) << '/' << glm::degrees(level.light->innerCutoff) << std::endl;

	switch (state) {
	case State::Pause:
		//screen.pause->draw(program["Phong"], level.camera, hud.light);
		screen.pause->draw(program["Phong"], screen.camera, screen.light);
		break;
	case State::Play:
<<<<<<< HEAD
		if (lightOn)
		{
			player->draw(program["PhongSpot"], level.camera, level.lightPointers); // @@@@@ FOR SEAN @@@@@ Extra argument to take in how many lights are in the array
			for (auto& enemy : enemies)
				enemy->draw(program["DropItems"], level.camera, level.lightPointers);
			level.map->draw(program["PhongSpot"], level.camera, level.lightPointers);
			for (int i = 0; i < dropItems.size(); i++)
				if (!dropItems[i]->collect)
					dropItems[i]->draw(program["DropItems"], level.camera, level.lightPointers);
		}
		else
		{
			player->draw(program["NoFlash"], level.camera, level.lightPointers); // @@@@@ FOR SEAN @@@@@ Extra argument to take in how many lights are in the array
			for (auto& enemy : enemies)
				enemy->draw(program["NoFlashNoNorm"], level.camera, level.lightPointers);
			level.map->draw(program["NoFlash"], level.camera, level.lightPointers);
			for (int i = 0; i < dropItems.size(); i++)
				if (!dropItems[i]->collect)
					dropItems[i]->draw(program["NoFlashNoNorm"], level.camera, level.lightPointers);
		}
		//level.hitboxes->draw(program["PhongColorSides"], level.camera, { *level.light });
		hud.healthBar->draw(program["PhongNoTexture"], level.camera, { hud.light });
		hud.display->draw(program["Phong"], level.camera, { hud.light });
		drawAmmo();

=======
		player->draw(program["PhongSpot"], level.camera, { level.light, level.light2 });// , level.light3 });
		for (auto& enemy : enemies)
			enemy->draw(program["PhongSpot"], level.camera, { level.light, level.light2 });// , level.light3 });
		level.map->draw(program["PhongSpot"], level.camera, { level.light, level.light2 });// , level.light3 });
		//level.hitboxes->draw(program["PhongColorSides"], level.camera, level.light);
		hud.healthBar->draw(program["PhongNoTexture"], level.camera, hud.light);
		hud.display->draw(program["Phong"], level.camera, hud.light);
		drawAmmo();

		for (auto& drop : drops)
			if (!drop->collect)
				drop->draw(program["PhongSpot"], level.camera, { level.light, level.light2 });// , level.light3 });

>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		break;
	case State::Control:
		screen.controls->draw(program["Phong"], screen.camera, screen.light);
		break;
	case State::Menu:
		screen.menu->draw(program["Phong"], screen.camera, screen.light);
		screen.play.obj->draw(program["PhongNoTexture"], screen.camera, screen.light);
		screen.quit.obj->draw(program["PhongNoTexture"], screen.camera, screen.light);
		break;
	case State::Win:
		screen.win->draw(program["Phong"], screen.camera, screen.light);
		break;
	case State::Lose:
		screen.lose->draw(program["Phong"], screen.camera, screen.light);
		break;
	default:
		break;
	}

	glutSwapBuffers();
}

// keyboard callback functions
void Game::keyboardDown(unsigned char key, glm::vec2 mouse) {
	switch (key) {
	case 27: // esc key
		switch (state) {
		case State::Play:
			state = State::Pause;
			break;
		case State::Pause:
			state = State::Play;
			break;
		case State::Menu:
			glutExit();
			break;
		case State::Lose: case State::Win:
<<<<<<< HEAD
			reset();
			//soundList[0]->playSound();
=======
			player->reset(level.start);
			clearEnemies();
			clearDrops();
			sounds["ambient"]->stopSound();
			sounds["SW"]->playSound(2)
				->setVolume(0.05f);
			state = State::Menu;
			//sounds["SW"]->playSound();
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
			break;
		default:
			break;
		}
		break;
	case ' ':
		switch (state) {
		case State::Menu:
			state = State::Control;
			break;
		case State::Control:
			loadEnemies();
			loadDrops();
			state = State::Play;
<<<<<<< HEAD
			soundList[0]->stopSound();
			soundList[1]->playSound(2);
			soundList[1]->setVolume(0.05f);
=======
			sounds["SW"]->stopSound();
			sounds["ambient"]->playSound(2)
				->setVolume(0.01f);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
			break;
		case State::Pause:
			state = State::Play;
			break;
		case State::Lose: case State::Win:
<<<<<<< HEAD
			reset();
			//soundList[0]->playSound();
=======
			player->reset(level.start);
			clearEnemies();
			clearDrops();
			sounds["ambient"]->stopSound();
			sounds["SW"]->playSound(2)
				->setVolume(0.05f);
			state = State::Menu;
			//sounds["SW"]->playSound();
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
			break;
		}
		break;
	case 'T': case 't':
		printf("Total elapsed time: %f\n", timer->getElapsedTime() / 1000.0f);
		break;
	case 'Q': case 'q':
		if (state == State::Pause) {
<<<<<<< HEAD
			reset();
			//soundList[0]->playSound();
=======
			player->reset(level.start);
			clearEnemies();
			clearDrops();
			sounds["ambient"]->stopSound();
			sounds["SW"]->playSound(2)
				->setVolume(0.05f);
			state = State::Menu;
			//sounds["SW"]->playSound();
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		}
		//enemies.push_back(new Enemy({ rand() % 21 - 10 + player->getPosition().x, 0, rand() % 21 - 10 + player->getPosition().z }));
		break;
	default:
		break;
	}
	switch (key) {
<<<<<<< HEAD
	case 'W': case 'w': input.keys |= Input::Keys::KeyW; break;
	case 'A': case 'a': input.keys |= Input::Keys::KeyA; break;
	case 'S': case 's': input.keys |= Input::Keys::KeyS; break;
	case 'D': case 'd': input.keys |= Input::Keys::KeyD; break;
	case 'Q': case 'q': input.keys |= Input::Keys::KeyQ; break;
	case 'E': case 'e': input.keys |= Input::Keys::KeyE; break;
	case 'R': case 'r': input.keys |= Input::Keys::KeyR; break;
	case 'F': case 'f': input.keys |= Input::Keys::KeyF; break;
	case 'Z': case 'z': input.keys |= Input::Keys::KeyZ; break;
	case 'X': case 'x': input.keys |= Input::Keys::KeyX; break;
	case 'C': case 'c': input.keys |= Input::Keys::KeyC; break;
	case 'V': case 'v':
=======
	case 'W': case 'w': Helper::bitOn(input.keys, Input::Keys::KeyW); break;
	case 'A': case 'a': Helper::bitOn(input.keys, Input::Keys::KeyA); break;
	case 'S': case 's': Helper::bitOn(input.keys, Input::Keys::KeyS); break;
	case 'D': case 'd': Helper::bitOn(input.keys, Input::Keys::KeyD); break;
	case 'Q': case 'q': Helper::bitOn(input.keys, Input::Keys::KeyQ); break;
	case 'E': case 'e': Helper::bitOn(input.keys, Input::Keys::KeyE); break;
	case 'R': case 'r': Helper::bitOn(input.keys, Input::Keys::KeyR); break;
	case 'F': case 'f': Helper::bitOn(input.keys, Input::Keys::KeyF); break;
	case 'Z': case 'z': Helper::bitOn(input.keys, Input::Keys::KeyZ); break;
	case 'X': case 'x': Helper::bitOn(input.keys, Input::Keys::KeyX); break;
	case 'C': case 'c': Helper::bitOn(input.keys, Input::Keys::KeyC); break;
	case 'V': case 'v': Helper::bitOn(input.keys, Input::Keys::KeyV);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		std::cout << (rand() * rand()) % 100 << std::endl;
		break;
	case 'U': case 'u': Helper::bitOn(input.keys, Input::Keys::KeyU);
		level.light->cutoff += glm::radians(0.1f);
		level.light3->cutoff += glm::radians(0.1f);
		break;
	case 'I': case 'i': Helper::bitOn(input.keys, Input::Keys::KeyI);
		level.light->cutoff -= glm::radians(0.1f);
		level.light3->cutoff -= glm::radians(0.1f);
		break;
	case 'J': case 'j': Helper::bitOn(input.keys, Input::Keys::KeyJ);
		level.light->innerCutoff += glm::radians(0.1f);
		break;
	case 'K': case 'k': Helper::bitOn(input.keys, Input::Keys::KeyK);
		level.light->innerCutoff -= glm::radians(0.1f);
		break;
	case '1':			Helper::bitOn(input.keys, Input::Keys::Num1); break;
	case '2':			Helper::bitOn(input.keys, Input::Keys::Num2); break;
	case '3':			Helper::bitOn(input.keys, Input::Keys::Num3); break;
	case '4':			Helper::bitOn(input.keys, Input::Keys::Num4); break;
	case ' ':			Helper::bitOn(input.keys, Input::Keys::Space); break;
	case 26:			Helper::bitOn(input.keys, Input::Keys::Esc); break;
	default:	break;
	}
}
void Game::keyboardUp(unsigned char key, glm::vec2 mouse) {
	switch (key) {
	case 'W': case 'w':	Helper::bitOff(input.keys, Input::Keys::KeyW); break;
	case 'A': case 'a':	Helper::bitOff(input.keys, Input::Keys::KeyA); break;
	case 'S': case 's':	Helper::bitOff(input.keys, Input::Keys::KeyS); break;
	case 'D': case 'd':	Helper::bitOff(input.keys, Input::Keys::KeyD); break;
	case 'Q': case 'q':	Helper::bitOff(input.keys, Input::Keys::KeyQ); break;
	case 'E': case 'e':	Helper::bitOff(input.keys, Input::Keys::KeyE); break;
	case 'R': case 'r':	Helper::bitOff(input.keys, Input::Keys::KeyR); break;
	case 'F': case 'f':	Helper::bitOff(input.keys, Input::Keys::KeyF); break;
	case 'Z': case 'z':	Helper::bitOff(input.keys, Input::Keys::KeyZ); break;
	case 'X': case 'x':	Helper::bitOff(input.keys, Input::Keys::KeyX); break;
	case 'C': case 'c':	Helper::bitOff(input.keys, Input::Keys::KeyC); break;
	case 'V': case 'v':	Helper::bitOff(input.keys, Input::Keys::KeyV); break;
	case '1':			Helper::bitOff(input.keys, Input::Keys::Num1); break;
	case '2':			Helper::bitOff(input.keys, Input::Keys::Num2); break;
	case '3':			Helper::bitOff(input.keys, Input::Keys::Num3); break;
	case '4':			Helper::bitOff(input.keys, Input::Keys::Num4); break;
	case ' ':			Helper::bitOff(input.keys, Input::Keys::Space); break;
	case 26:			Helper::bitOff(input.keys, Input::Keys::Esc); break;

	default: break;
	}
}

void Game::specialInput(unsigned char key, glm::vec2 mouse) {
	switch (key) {
	case GLUT_KEY_SHIFT_L:
	case GLUT_KEY_SHIFT_R:
	case GLUT_KEY_CTRL_L:
	case GLUT_KEY_CTRL_R:
	case GLUT_KEY_ALT_L:
	case GLUT_KEY_ALT_R:
		break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_HOME:
	case GLUT_KEY_END:
	case GLUT_KEY_PAGE_UP:
	case GLUT_KEY_PAGE_DOWN:
		break;
	default:
		break;
	}
}

void Game::windowReshape(glm::vec2 size) {
	windowSize = size;
	glViewport(0, 0, windowSize.x, windowSize.y);
	level.camera->reset(size);
	screen.camera->reset(size);
}

// mouse callback functions
void Game::mouseClicked(int button, int state, glm::vec2 mouse) {
	input.mouse = mouse;	input.button = button;	input.state = state;
	//std::cout << mouse.x << ':' << mouse.y << std::endl;
	switch (this->state) {
	case State::Play:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			switch (state) {
			case GLUT_DOWN:
				if (player->reloadCd <= 0.0f) {
<<<<<<< HEAD
					player->firing = true;
					soundList[3]->stopSound();
					soundList[2]->playSound(3);
					soundList[2]->setVolume(0.05f);
=======
					sounds["reload"]->stopSound();
					sounds["gunshot"]->playSound(3)
						->setVolume(0.05f);
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
				}
				break;
			case GLUT_UP:
				player->firing = false;
				sounds["gunshot"]->stopSound();
				break;
			default:
				break;
<<<<<<< HEAD
			}
			break;
		case GLUT_RIGHT_BUTTON:
			switch (state) {
			case GLUT_DOWN:
				break;
			case GLUT_UP:
				lightOn = !lightOn;
				break;
			default:
				break;
			}
			break;
=======
			}	break;
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		default:
			break;
		}	break;
	case State::Menu:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			switch (state) {
			case GLUT_DOWN:
				if (Helper::inside(mouse, screen.play.pos, windowSize))
					this->state = State::Control;
<<<<<<< HEAD
				//soundList[1]->playSound();
				if (mouse.x > screen.quit.pos.x * windowSize.x && mouse.x < screen.quit.pos.y * windowSize.x && mouse.y > screen.quit.pos.z * windowSize.y && mouse.y < screen.quit.pos.w * windowSize.y)
=======
					//sounds["ambient"]->playSound();
				if (Helper::inside(mouse, screen.quit.pos, windowSize))
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
					glutExit();
				break;
			case GLUT_UP:
				break;
			default:
				break;
			}	break;
		default:
			break;
		}	break;
	default:
		break;
	}
}

void Game::mouseMoved(glm::vec2 mouse) {
	input.mouse = mouse;
	switch (state) {
	case State::Play:
		//if (mousePosition.length() > 0 && !(abs(change.x) >= 200.0f && abs(change.y) >= 200.0f))
			//camera->processMotion(change, timer->getElapsedTimeMS());
		break;
	default:
		break;
	}
	if (!input.xBox->getConnected(0) && state == State::Play) {
		glm::vec3 pPos = player->getPosition();
		//std::cout << pos.x << ':' << pos.y << ';' << pPos.x << ':' << pPos.z << ';' << windowSize.x << ':' << windowSize.y << std::endl;
		glm::vec2 diff = glm::vec2(pPos.x, pPos.z) + windowSize * 0.5f - mouse;
		player->setRotation({ 0.f, Helper::getAngle(diff), 0.f });
	}
}

void Game::mousePassive(glm::vec2 mouse) {
	input.mouse = mouse;
	if (!input.xBox->getConnected(0))
		switch (state) {
		case State::Play: {
			glm::vec3 pPos = player->getPosition();
			glm::vec2 diff = glm::vec2(pPos.x, pPos.z) + windowSize * 0.5f - mouse;
			player->setRotation({ 0.f, Helper::getAngle(diff), 0.f });
			break;
		} case State::Menu:
			//if (mouse.x > screen.play.pos.x * windowSize.x && mouse.x < screen.play.pos.y * windowSize.x && mouse.y > screen.play.pos.z * windowSize.y && mouse.y < screen.play.pos.w * windowSize.y)
			if (Helper::inside(mouse, screen.play.pos, windowSize))
				screen.play.obj->color.b = 0.f;
			else screen.play.obj->color.b = 1.f;
			//if (mouse.x > screen.quit.pos.x * windowSize.x && mouse.x < screen.quit.pos.y * windowSize.x && mouse.y > screen.quit.pos.z * windowSize.y && mouse.y < screen.quit.pos.w * windowSize.y)
			if (Helper::inside(mouse, screen.quit.pos, windowSize))
				screen.quit.obj->color.b = 0.f;
			else screen.quit.obj->color.b = 1.f;
			break;
		default:
			break;
		}
}

// controller callback functions
void Game::controllerInput(unsigned short index, Input::Button button) {
	if (index == 0)
		switch (state) {
		case State::Play:
			switch (button) {
			case Input::Button::Start:
				state = State::Pause;
				break;
			case Input::Button::RB:
				player->reload = true;
				break;
			default:
				break;
			}	break;
		case State::Pause:
			switch (button) {
			case Input::Button::A:
				state = State::Play;
<<<<<<< HEAD
			if (button == Input::Button::B) {
				reset();
			}
			break;
		case State::Lose: case State::Win:
			if (button == Input::Button::B) {
				reset();
			}
			break;
=======
				break;
			case Input::Button::B:
				state = State::Menu;
				player->reset(level.start);
				clearEnemies();
				clearDrops();
			default:
				break;
			}	break;
		case State::Lose: case State::Win:
			switch (button) {
			case Input::Button::B:
				state = State::Menu;
				player->reset(level.start);
				clearEnemies();
				clearDrops();
				break;
			default:
				break;
			}	break;
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		case State::Menu:
			switch (button) {
			case Input::Button::A:
				state = State::Control;
				break;
			case Input::Button::Start:
				glutExit();
				break;
			default:
				break;
			}	break;
		case State::Control:
			switch (button) {
			case Input::Button::B:
				state = State::Play;
				loadEnemies();
				break;
			default:
				break;
			}	break;
		default:
			break;
		}
}

void Game::controllerSpecial(unsigned short index, Input::Triggers triggers, Input::Sticks sticks) {
	switch (state) {
	case State::Play:
		if (index == 0) {
			player->acceleration = { sticks.first.x / 2.0f, 0.0f, -sticks.first.y / 2.0f };
			if (glm::length(player->acceleration) > 0.f)
				player->acceleration = glm::normalize(player->acceleration);
			if (glm::length(sticks.second) > 0.5f)
				player->setRotation({ 0.0f, glm::degrees(atan2(sticks.second.y, sticks.second.x)), 0.0f });
			if (triggers.second > 0.2)
			{
				if (player->reloadCd <= 0.0f) {
					player->firing = true;
					soundList[3]->stopSound();
					soundList[2]->playSound(3);
					soundList[2]->setVolume(0.05f);
				}
			}
			else
			{
				player->firing = false;
				soundList[2]->stopSound();
			}
		}
		break;
	default:
		break;
	}
}

void Game::createDropItem(glm::vec3 pos, int type) {
	float temp = 0;
	if (type == 0)
		temp = rand() % 100;
<<<<<<< HEAD
	}
	//std::cout << temp << '/' << player->health << std::endl;
	if ((temp > 50 & temp <= 80) || type == 2) {
		//drop->ammo = 30.0f;
		//dropAmmo->color = glm::vec4(1.0f, 0.647f, 0.f, 0.5f);
		dropAmmo->setPosition(pos);
		dropAmmo->update(deltaTime);
		//std::cout << "DROP!" << dropItems.size() << std::endl;
		dropItems.push_back(new Object(*dropAmmo));
	}
	else if (temp > 80 || type == 1) {
=======
	//std::cout << temp << '/' << player->life << std::endl;
	if (type == 1 || temp > 80) {
>>>>>>> ca276360e1b38360d81f291610aa3ba3e36982b5
		//drop->hp = 5.0f;
		//dropHP->color = glm::vec4(0.0f, 1.0f, 0.f, 0.5f);
		dropHP->setPosition(pos)
			->update(deltaTime);
		//std::cout << "DROP!" << drops.size() << std::endl;
		drops.push_back(new Object(*dropHP));
	} else if (type == 2 || (temp > 50 && temp <= 80)) {
		//drop->ammo = 30.0f;
		//dropAmmo->color = glm::vec4(1.0f, 0.647f, 0.f, 0.5f);
		dropAmmo->setPosition(pos)
			->update(deltaTime);
		//std::cout << "DROP!" << drops.size() << std::endl;
		drops.push_back(new Object(*dropAmmo));
	}
	//else
	//{
	//	drop->ammo = 0.0f;
	//	drop->hp = 0.0f;
	//}
}

void Game::drawAmmo() {
	if (hud.ammo.number) {
		hud.ammo.number->ammo = player->ammo / 10;
		hud.ammo.number->setPosition(hud.ammo.positions[0])
			->update(deltaTime)
			->draw(program["numbers"], level.camera, hud.light);
		hud.ammo.number->ammo = player->ammo;
		hud.ammo.number->setPosition(hud.ammo.positions[1])
			->update(deltaTime)
			->draw(program["numbers"], level.camera, hud.light);
		hud.ammo.number->ammo = player->ammoDepo / 100;
		hud.ammo.number->setPosition(hud.ammo.positions[2])
			->update(deltaTime)
			->draw(program["numbers"], level.camera, hud.light);
		hud.ammo.number->ammo = player->ammoDepo / 10;
		hud.ammo.number->setPosition(hud.ammo.positions[3])
			->update(deltaTime)
			->draw(program["numbers"], level.camera, hud.light);
		hud.ammo.number->ammo = player->ammoDepo;
		hud.ammo.number->setPosition(hud.ammo.positions[4])
			->update(deltaTime)
			->draw(program["numbers"], level.camera, hud.light);
	}
}

// Resets the level
void Game::reset()
{
	player->reset(level.start);
	clearEnemies();
	clearDrops();
	loadEnemies();
	loadDrops();
	lightOn = false;
	soundList[1]->stopSound();
	soundList[0]->playSound(2);
	soundList[0]->setVolume(0.05f);
	state = State::Menu;
}