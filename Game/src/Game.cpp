#include <iostream>
#include <memory>
#include <tuple>
#include <ctime>
#include <cstdlib>

#include <Windows.h>

#include <glm/glm.hpp>

#include "Assets.h"
#include "Level.h"
#include "AnimationMesh.h"
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

void Game::init(void(*_controllerInput)(unsigned short index, Input::Button button), void(*_controllerSpecial)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks)) {
	// Initialize game elements
	timer = new Timer();
	Assets *assets = Assets::getInstance();
	input.xBox = new Input::XBox();
	input.xBox->callback = _controllerInput;
	input.xBox->special = _controllerSpecial;

	// Initialize loading screen
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screen.camera = new Camera(windowSize);
	screen.camera->setPosition({ 0.f, 5.f, 0.01f });
	screen.camera->update({ 0.f, 0.f, 0.f });
	screen.light = new Light();
	screen.light->type = (unsigned int)Type::Light::DIRECTIONAL;
	screen.light->direction = { 0.f, -1.f, 0.f, 0.f };
	screen.light->ambient = { 0.15f, 0.15f, 0.15f };
	screen.light->diffuse = { 0.7f, 0.7f, 0.7f };
	screen.light->specular = { 1.f, 1.f, 1.f };
	screen.light->specExponent = 50.f;
	screen.light->attenuation = { 1.f, 0.1f, 0.01f };
	screen.loading = new Object();
	screen.loading->loadMesh(assets->loadMesh("screen", "screen.obj"));
	screen.loading->loadTexture(Type::DIFFUSE, assets->loadTexture("loading", "loading.png"));
	program["Phong"] = new Shader();
	if (!program["Phong"]->load("assets/shaders/animation.vert", "assets/shaders/Phong.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
	screen.loading->draw(program["Phong"], screen.camera, { screen.light }, 0);
	glutSwapBuffers();

	std::cout << glutGet(GLUT_ELAPSED_TIME) << " milliseconds to load in things" << std::endl;


	Sound* sound = new Sound("assets/sounds/game soundtrack.wav", true, 2);
	//Sound* sound = new Sound("assets/sounds/SW.mp3", true, 2);
	soundList.push_back(sound);
	soundList[0]->createChannel(2);
	soundList[0]->setVolume(0.01f);

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

	// animation
	//assets->loadMesh("dam0", "dam0.obj");
	//assets->loadMesh("dam1", "dam1.obj");
	//assets->loadMesh("dam2", "dam2.obj");
	//assets->loadMesh("dam3", "dam3.obj");
	//assets->loadMesh("dam4", "dam4.obj");
	//assets->loadMesh("dam5", "dam5.obj");
	//assets->loadMesh("dam6", "dam6.obj");
	//assets->loadMesh("dam7", "dam7.obj");
	//assets->loadMesh("dam8", "dam8.obj");

	//assets->loadMesh("playerFrame0", "c_kframe_1.obj");
	//assets->loadMesh("playerFrame1", "c_kframe_2.obj");
	//assets->loadMesh("playerFrame2", "c_kframe_3.obj");
	//assets->loadMesh("playerFrame3", "c_kframe_4.obj");
	//assets->loadMesh("playerFrame4", "c_kframe_5.obj");

	assets->loadAnimationMesh("playerFrame0", "c_kframe_1.obj", "c_kframe_2.obj");
	assets->loadAnimationMesh("playerFrame1", "c_kframe_2.obj", "c_kframe_3.obj");
	assets->loadAnimationMesh("playerFrame2", "c_kframe_3.obj", "c_kframe_4.obj");
	assets->loadAnimationMesh("playerFrame3", "c_kframe_4.obj", "c_kframe_5.obj");
	assets->loadAnimationMesh("playerFrame4", "c_kframe_5.obj", "c_kframe_1.obj");
	

	std::cout << glutGet(GLUT_ELAPSED_TIME) << " milliseconds to load in things" << std::endl;


	/*program["PhongSpot"] = new Shader();
	if (!program["PhongSpot"]->load("assets/shaders/Phong.vert", "assets/shaders/phongSpot.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}*/

	// Initialize level
	level.map = new Object();
	level.collision = new Level(0.5f);
	level.hitboxes = new Object({ 0.f, -8.f, 0.f });
	level.map->loadMesh(assets->meshes["level"]);
	level.collision->load("assets/meshes/Laboratory Level Hitboxes.obj");
	level.hitboxes->loadMesh(assets->meshes["hitboxes"]);
	level.map->loadTexture(Type::Texture::DIFFUSE, assets->textures["level color"]);
	level.map->loadTexture(Type::Texture::NORMAL, assets->textures["level normal"]);
	level.hitboxes->update(deltaTime);
	level.camera = new Camera(windowSize);
	level.camera->setPosition({ 0.f, 12.5f, 2.5f });
	level.camera->update({ 0.f,0.f,0.f });

	level.light = new Light();
	level.light->type = (unsigned int)Type::Light::POINT;
	level.light->position = { 0.f, 1.f, 0.f, 1.f };
	//level.light->original = level.light->position;
	level.light->ambient = { 0.15f, 0.15f, 0.15f };
	level.light->diffuse = { 0.7f, 0.7f, 0.7f };
	level.light->specular = { 1.f, 1.f, 1.f };
	level.light->specExponent = 50.f;
	level.light->attenuation = { 1.f, 1.f, 1.f };
	level.lightPointers.push_back(level.light);
	//level.lightNoFlash.push_back(level.light);

	// @@@@@ FOR SEAN @@@@@@	Loads in the array containing all the lights in the level
	for (int i = 0; i < level.lightsPos.size(); i++)
	{
		//std::cout << "Load Light " << i << ':' << light->position.x << '/' << light->position.z << std::endl;
		Light *light = new Light();
		light->type = (unsigned int)Type::Light::SPOT;
		light->position = { level.lightsPos[i], 1.f };
		light->direction = { 0.f, -1.0f, 0.f, 0.f };
		light->ambient = { 0.15f, 0.15f, 0.15f };
		light->diffuse = { 0.7f, 0.7f, 0.7f };
		light->specular = { 1.f, 1.f, 1.f };
		light->specExponent = 50.f;
		light->spotExponent = 1.f;
		light->cutoff = glm::radians(55.f);
		light->innerCutoff = glm::radians(1.f);
		light->partial = 0.1;
		light->attenuation = { 0.5f, 0.1f, 0.01f };
		level.lightPointers.push_back(light);
		//level.lightNoFlash.push_back(light);
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
	level.light2->spotExponent = 1.f;
	level.light2->cutoff = glm::radians(55.f);
	level.light2->innerCutoff = glm::radians(1.f);
	level.light2->partial = 0.1;
	level.light2->attenuation = { 0.5f, 0.1f, 0.01f };
	level.lightPointers.push_back(level.light2);

	//level.lights.reserve(level.lightPointers.size());// *sizeof(Light));
	//
	//for (int i = 0; i < level.lightPointers.size(); i++)
	//{
	//	level.lights[i] = *level.lightPointers[i];
	//}

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

	// Initialize buttons
	screen.play.obj = new Object({ 0.f, 0.2f, 0.3f });
	screen.play.pos = { 0.30677083f, 0.73802083f, 0.44351851f, 0.6648f };
	screen.play.scale = 1.7f;
	screen.play.obj->color = glm::vec4(1.f);
	screen.play.obj->loadMesh(assets->meshes["play"]);
	screen.play.obj->setScale(glm::vec3(1.7f));
	screen.play.obj->update(0.f);

	screen.quit.obj = new Object({ 0.f, 0.2f, 1.75f });
	screen.quit.pos = { 0.36822917f, 0.64583333f, 0.72962963f, 0.99722223f };
	screen.quit.scale = 1.5f;
	screen.quit.obj->color = glm::vec4(1.f);
	screen.quit.obj->loadMesh(assets->meshes["quit"]);
	screen.quit.obj->setScale(glm::vec3(1.5f));
	screen.quit.obj->update(0.f);

	// Initialize shaders
	program["PhongSpot"] = new Shader();
	if (!program["PhongSpot"]->load("assets/shaders/animation.vert", "assets/shaders/phongSpot.frag")) {
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
	if (!program["DropItems"]->load("assets/shaders/animation.vert", "assets/shaders/DropItems.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
	program["NoFlash"] = new Shader();
	if (!program["NoFlash"]->load("assets/shaders/animation.vert", "assets/shaders/NoFlash.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
	program["NoFlashNoNorm"] = new Shader();
	if (!program["NoFlashNoNorm"]->load("assets/shaders/animation.vert", "assets/shaders/NoFlashNoNorm.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}
	program["MeshMorph"] = new Shader();
	if (!program["MeshMorph"]->load("assets/shaders/animation.vert", "assets/shaders/phongSpot.frag")) {
		std::cout << "Phong Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	// Initialize Sounds
	//Sound* sound = new Sound("assets/sounds/game soundtrack.wav", true, 2);
	//soundList.push_back(sound);
	//sound = new Sound("assets/sounds/ambient machine noise.wav", true, 2);
	//soundList.push_back(sound);
	//sound = new Sound("assets/sounds/Gunshot_sound.wav", true, 3);
	//soundList.push_back(sound);
	//sound = new Sound("assets/sounds/Reload_sound.wav", false, 3);
	//soundList.push_back(sound);

	Sound* sound1 = new Sound("assets/sounds/ambient machine noise.wav", true, 2);
	soundList.push_back(sound1);
	Sound* sound2 = new Sound("assets/sounds/Gunshot_sound.wav", true, 3);
	soundList.push_back(sound2);
	Sound* sound3 = new Sound("assets/sounds/Reload_sound.wav", false, 3);
	soundList.push_back(sound3);
	Sound* sound4 = new Sound("assets/sounds/Dialogue Placeholder.wav", false, 2);
	soundList.push_back(sound4);


	// Initialize Player
	player = new Player({ 4.f, 0.f, 6.f });
	//player->loadMesh(assets->meshes["player"]);
	player->loadTexture(Type::Texture::DIFFUSE, assets->textures["player color"]);
	player->loadTexture(Type::Texture::NORMAL, assets->textures["player normal"]);
	player->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	player->bullet->loadMesh(assets->meshes["bullet"]);
	player->bullet->color = { 1.0f , 1.0f, 1.0f, 0.3f };
	//player->life = 15.0f;
	player->ammo = 30.0f;
	player->ammoDepo = 90.f;
	player->reloadCd = 0.0f;

	player->loadAnimationFrame(assets->aMeshes["playerFrame0"]);
	player->loadAnimationFrame(assets->aMeshes["playerFrame1"]);
	player->loadAnimationFrame(assets->aMeshes["playerFrame2"]);
	player->loadAnimationFrame(assets->aMeshes["playerFrame3"]);
	player->loadAnimationFrame(assets->aMeshes["playerFrame4"]);


	// Initialize Drops
	dropHP = new Object();
	dropHP->loadMesh(assets->meshes["health"]);
	dropHP->loadTexture(Type::Texture::DIFFUSE, assets->textures["item"]);
	//dropHP->loadTexture(Type::Texture::NORMAL, assets->textures["item"]);
	dropHP->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	dropHP->update(0.f);
	//dropHP->ammo = 30.0f;
	dropHP->life = 5.0f;
	dropHP->collect = false;

	dropAmmo = new Object();
	dropAmmo->loadMesh(assets->meshes["ammo"]);
	dropAmmo->loadTexture(Type::Texture::DIFFUSE, assets->textures["item"]);
	//dropAmmo->loadTexture(Type::Texture::NORMAL, assets->textures["item"]);
	dropAmmo->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"]);
	dropAmmo->update(0.f);
	dropAmmo->ammo = 30.0f;
	//dropAmmo->hp = 5.0f;
	dropAmmo->collect = false;

	// Initialize HUD
	hud.display = new Object();
	hud.healthBar = new Object();
	hud.display->loadMesh(assets->meshes["hud"]);
	hud.healthBar->loadMesh(assets->meshes["hp bar"]);
	hud.display->loadTexture(Type::Texture::DIFFUSE, assets->textures["hud"]);
	hud.healthBar->color = hud.red;
	hud.display->setRotation(hud.angle);
	hud.healthBar->setRotation(hud.angle);
	hud.display->setScale(glm::vec3(0.395f));
	hud.light = new Light();
	hud.light->type = (unsigned int)Type::Light::DIRECTIONAL;
	hud.light->direction = { 0.f, -1.f, 0.f, 0.f };
	//hud.light->original = level.light->direction;
	hud.light->ambient = { 0.15f, 0.15f, 0.15f };
	hud.light->diffuse = { 0.7f, 0.7f, 0.7f };
	hud.light->specular = { 1.f, 1.f, 1.f };
	hud.light->specExponent = 50.f;
	hud.light->attenuation = { 1.f, 0.1f, 0.01f };
	hud.ammo.number = new Object();
	hud.ammo.number->loadMesh(assets->meshes["screen"]);
	hud.ammo.number->setScale(glm::vec3(0.05f, 1.f, 0.1f));
	hud.ammo.number->setRotation(hud.angle);

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

	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam0"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam1"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam2"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam3"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam4"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam5"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam6"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam7"]);
	//std::get<0>(enemys)->loadAnimationFrame(assets->meshes["dam8"]);

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
}

void Game::loadEnemies() {
	for (auto position : std::get<0>(level.enemies)) {
		std::get<0>(enemys)->setPosition({ position.x, 0.f, position.y });
		enemies.push_back(new Enemy(*std::get<0>(enemys)));
	}
	for (auto position : std::get<1>(level.enemies)) {
		std::get<1>(enemys)->setPosition({ position.x, 0.f, position.y });
		enemies.push_back(new Enemy(*std::get<1>(enemys)));
	}
	for (auto position : std::get<2>(level.enemies)) {
		std::get<2>(enemys)->setPosition({ position.x, 0.f, position.y });
		enemies.push_back(new Enemy(*std::get<2>(enemys)));
	}
}

Game::~Game() {
	delete timer;
	delete player;
	delete level.collision;
	delete level.hitboxes;
	delete level.map;
	delete level.camera;
	delete level.light;
	delete level.light2;
	delete level.light3;
	for (int i = 0; i < level.lightPointers.size(); i++) {
		delete level.lightPointers[i];
	}
	delete screen.menu;
	delete screen.pause;
	delete screen.win;
	delete screen.lose;
	delete screen.controls;
	delete screen.camera;
	delete screen.light;
	delete screen.play.obj;
	delete screen.quit.obj;
	delete screen.loading;
	delete hud.display;
	delete hud.healthBar;
	delete hud.light;
	delete input.xBox;
	delete dropHP;
	delete dropAmmo;
	for (auto& prog : program) {
		prog.second->unload();
		delete prog.second;
	}
	for (int i = 0; i < soundList.size(); i++) {
		soundList[i]->unload();
		delete soundList[i];
	}
	program.clear();
	clearEnemies();
	clearDrops();
	enemies.clear();
	std::get<0>(level.enemies).clear();
	std::get<1>(level.enemies).clear();
	std::get<2>(level.enemies).clear();
	delete std::get<0>(enemys);
	delete std::get<1>(enemys);
	delete std::get<2>(enemys);
}

void Game::clearEnemies() {
	for (auto& enemy : enemies)
		delete enemy;
	enemies.clear();
}

void Game::clearDrops() {
	for (int i = 0; i < dropItems.size(); i++)
		delete dropItems[i];
	dropItems.clear();
}

void Game::update() {
	deltaTime = timer->update();
	//std::cout << "Keys: " << keys << std::endl;

	input.xBox->update();
	for (int i = 0; i < soundList.size(); i++)
		soundList[i]->update();

	if (state == State::Menu) {
		//soundList[1]->stopSound();
		//soundList[0]->playSound();
	}
	else if (state == State::Play) {
		//soundList[0]->stopSound();
		//soundList[1]->playSound();
		if (player->reloadCd > 0.0f && !player->reloaded) {
			soundList[2]->stopSound();
			player->reloaded = true;
			soundList[3]->playSound(3);
			soundList[3]->setVolume(0.5f);
		}

		//soundList[1]->createChannel();
		// player movement
		if (input.keys & Input::Keys::KeyW && ~(input.keys & Input::Keys::KeyS))
			player->acceleration.z = -1.f;
		else if (input.keys & Input::Keys::KeyS && ~(input.keys & Input::Keys::KeyW))
			player->acceleration.z = 1.f;
		else if (!input.xBox->getConnected(0))
			player->acceleration.z = 0.f;
		if (input.keys & Input::Keys::KeyA && ~(input.keys & Input::Keys::KeyD))
			player->acceleration.x = -1.f;
		else if (input.keys & Input::Keys::KeyD && ~(input.keys & Input::Keys::KeyA))
			player->acceleration.x = 1.f;
		else if (!input.xBox->getConnected(0))
			player->acceleration.x = 0.f;
		if (glm::length(player->acceleration) > 0.f)
			player->acceleration = glm::normalize(player->acceleration);
		if (input.keys & Input::Keys::KeyR)
			player->reload = true;
		player->update(deltaTime, level.collision);
		glm::vec3 temp = player->getPosition();
		//std::cout << temp.x << '/' << temp.y << '/' << temp.z << std::endl;
		//std::cout << rand() % 100 << std::endl
		
		//if(deltaTime >= 25.0f)
		//std::cout << deltaTime << std::endl;

		// stuff based on player
		hud.display->setPosition(player->getPosition() + glm::vec3(0.004f, 10.6f, 1.967f));
		//screen.pause->setPosition(player->getPosition() + glm::vec3(0.f, 10.f, 0.f));
		hud.healthBar->setPosition(player->getPosition() + glm::vec3(-1.846f, 10.4f, 3.027f));
		hud.healthBar->setScale({ player->life * 0.0365f, 0.f, 0.035f });
		hud.display->update(deltaTime);
		//screen.pause->update(deltaTime);
		hud.healthBar->update(deltaTime);
		hud.ammo.number->update(deltaTime);
		level.camera->update(player->getPosition());
		// Point Light Position
		level.lightPointers[0]->position = glm::vec4(player->getPosition() + glm::vec3(0.f, 2.f, 0.f), 1.f);
		// Spot Light Position and Rotation
		level.lightPointers[level.lightPointers.size() - 1]->position = glm::vec4(player->getPosition() + glm::vec3(0.f, 1.f, 0.f), 1.f);
		float angle = glm::radians(player->getRotation().y);
		level.lightPointers[level.lightPointers.size() - 1]->direction = { cos(angle), -0.1f, -sin(angle), 0.f };
		//level.light3->position = glm::vec4(player->getPosition() + glm::vec3(0.f, 0.1f, 0.f), 1.f);
		//level.light3->direction = { cos(angle), 0.45f, -sin(angle), 0.f };

		// bullet collision
		for (auto bullet : player->bullets) {
			if (bullet->cooldown <= 0.f) {
				float bang = glm::radians(bullet->getRotation().y);
				float distFromPlayer = 20.f;
				Enemy* target = nullptr;
				for (auto enemy : enemies) {
					glm::vec3 diff = enemy->getPosition() - bullet->getPosition();
					float dang = atan2(-diff.z, diff.x);
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
					glm::vec3 dif = target->getPosition() - player->getPosition();
					target->setPosition(target->getPosition() + glm::normalize(dif) * 0.1f);
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
			enemies[i]->setRotation({ 0.f, glm::degrees(atan2(-diff.z, diff.x)) - 90.f, 0.f });
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
			else
				enemies[i]->setVelocity({ 0.0f, 0.0f, 0.0f });
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
		glm::vec3 pPos = player->getPosition();
		if (pPos.x > level.exit.x && pPos.x < level.exit.y && pPos.z > level.exit.z && pPos.z < level.exit.w)
			state = State::Win;
		// item drops
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

	//cout << glm::degrees(level.light->cutoff) << '/' << glm::degrees(level.light->innerCutoff) << endl;

	switch (state) {
	case State::Pause:
		//screen.pause->draw(program["Phong"], level.camera, { *hud.light });
		screen.pause->draw(program["Phong"], screen.camera, { screen.light }, 0);
		break;
	case State::Play:
		if (lightOn)
		{
			player->draw(program["MeshMorph"], level.camera, level.lightPointers, 1); // @@@@@ FOR SEAN @@@@@ Extra argument to take in how many lights are in the array
			for (auto& enemy : enemies)
				enemy->draw(program["DropItems"], level.camera, level.lightPointers, 0);
			level.map->draw(program["PhongSpot"], level.camera, level.lightPointers, 0);
			for (int i = 0; i < dropItems.size(); i++)
				if (!dropItems[i]->collect)
					dropItems[i]->draw(program["DropItems"], level.camera, level.lightPointers, 0);
		}
		else
		{
			player->draw(program["NoFlash"], level.camera, level.lightPointers, 1); // @@@@@ FOR SEAN @@@@@ Extra argument to take in how many lights are in the array
			for (auto& enemy : enemies)
				enemy->draw(program["NoFlashNoNorm"], level.camera, level.lightPointers, 0);
			level.map->draw(program["NoFlash"], level.camera, level.lightPointers, 0);
			for (int i = 0; i < dropItems.size(); i++)
				if (!dropItems[i]->collect)
					dropItems[i]->draw(program["NoFlashNoNorm"], level.camera, level.lightPointers, 0);
		}
		//level.hitboxes->draw(program["PhongColorSides"], level.camera, { *level.light });
		hud.healthBar->draw(program["PhongNoTexture"], level.camera, { hud.light }, 0);
		hud.display->draw(program["Phong"], level.camera, { hud.light }, 0);
		//drawAmmo();

		break;
	case State::Menu:
		screen.menu->draw(program["Phong"], screen.camera, { screen.light }, 0);
		screen.play.obj->draw(program["PhongNoTexture"], screen.camera, { screen.light }, 0);
		screen.quit.obj->draw(program["PhongNoTexture"], screen.camera, { screen.light }, 0);
		break;
	case State::Win:
		screen.win->draw(program["Phong"], screen.camera, { screen.light }, 0);
		break;
	case State::Lose:
		screen.lose->draw(program["Phong"], screen.camera, { screen.light }, 0);
		break;
	case State::Control:
		screen.controls->draw(program["Phong"], screen.camera, { screen.light }, 0);
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
			reset();
			//soundList[0]->playSound();
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
			state = State::Play;
			soundList[0]->stopSound();
			soundList[1]->playSound(2);
			soundList[1]->setVolume(0.05f);
			soundList[4]->playSound(2);
			soundList[4]->setVolume(0.5f);
			break;
		case State::Pause:
			state = State::Play;
			break;
		case State::Lose: case State::Win:
			reset();
			//soundList[0]->playSound();
			break;
		}
		break;
	case 'T': case 't':
		printf("Total elapsed time: %f\n", timer->getElapsedTime() / 1000.0f);
		break;
	case 'Q': case 'q':
		if (state == State::Pause) {
			reset();
			//soundList[0]->playSound();
		}
		//enemies.push_back(new Enemy({ rand() % 21 - 10 + player->getPosition().x, 0, rand() % 21 - 10 + player->getPosition().z }));
		break;
	default:
		break;
	}
	switch (key) {
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
		std::cout << (rand() * rand()) % 100 << std::endl;

		input.keys |= Input::Keys::KeyV; break;
	case '1':			input.keys |= Input::Keys::Num1; break;
	case '2':			input.keys |= Input::Keys::Num2; break;
	case '3':			input.keys |= Input::Keys::Num3; break;
	case '4':			input.keys |= Input::Keys::Num4; break;
	case ' ':			input.keys |= Input::Keys::Space; break;
	case 26:			input.keys |= Input::Keys::Esc; break;
	case 'o':
		//level.light->cutoff += glm::radians(0.1f);
		//level.light3->cutoff += glm::radians(0.1f);
		player->timerReset += 1.0f;
		break;
	case 'p':
		//level.light->cutoff -= glm::radians(0.1f);
		//level.light3->cutoff -= glm::radians(0.1f);
		player->timerReset -= 1.0f;
		break;
	case'k':
		level.light->innerCutoff += glm::radians(0.1f);
		break;
	case 'l':
		level.light->innerCutoff -= glm::radians(0.1f);
		break;
	default: break;
	}
}
void Game::keyboardUp(unsigned char key, glm::vec2 mouse) {
	switch (key) {
	case 'W': case 'w':	input.keys &= ~Input::Keys::KeyW; break;
	case 'A': case 'a':	input.keys &= ~Input::Keys::KeyA; break;
	case 'S': case 's':	input.keys &= ~Input::Keys::KeyS; break;
	case 'D': case 'd':	input.keys &= ~Input::Keys::KeyD; break;
	case 'Q': case 'q':	input.keys &= ~Input::Keys::KeyQ; break;
	case 'E': case 'e':	input.keys &= ~Input::Keys::KeyE; break;
	case 'R': case 'r':	input.keys &= ~Input::Keys::KeyR; break;
	case 'F': case 'f':	input.keys &= ~Input::Keys::KeyF; break;
	case 'Z': case 'z':	input.keys &= ~Input::Keys::KeyZ; break;
	case 'X': case 'x':	input.keys &= ~Input::Keys::KeyX; break;
	case 'C': case 'c':	input.keys &= ~Input::Keys::KeyC; break;
	case 'V': case 'v':	input.keys &= ~Input::Keys::KeyV; break;
	case '1':			input.keys &= ~Input::Keys::Num1; break;
	case '2':			input.keys &= ~Input::Keys::Num2; break;
	case '3':			input.keys &= ~Input::Keys::Num3; break;
	case '4':			input.keys &= ~Input::Keys::Num4; break;
	case ' ':			input.keys &= ~Input::Keys::Space; break;
	case 26:			input.keys &= ~Input::Keys::Esc; break;

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
					player->firing = true;
					soundList[3]->stopSound();
					soundList[2]->playSound(3);
					soundList[2]->setVolume(0.05f);
				}
				break;
			case GLUT_UP:
				player->firing = false;
				soundList[2]->stopSound();
				break;
			default:
				break;
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
		default:
			break;
		}
		break;
	case State::Menu:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			switch (state) {
			case GLUT_DOWN:
				if (mouse.x > screen.play.pos.x * windowSize.x && mouse.x < screen.play.pos.y * windowSize.x && mouse.y > screen.play.pos.z * windowSize.y && mouse.y < screen.play.pos.w * windowSize.y)
					this->state = State::Control;
				//soundList[1]->playSound();
				if (mouse.x > screen.quit.pos.x * windowSize.x && mouse.x < screen.quit.pos.y * windowSize.x && mouse.y > screen.quit.pos.z * windowSize.y && mouse.y < screen.quit.pos.w * windowSize.y)
					glutExit();
				break;
			case GLUT_UP:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			// handle left click
			break;
		case GLUT_RIGHT_BUTTON:
			// handle right click
			break;
		case GLUT_MIDDLE_BUTTON:
			// handle middle click
			break;
		default:
			break;
		}
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
		player->setRotation({ 0.f, glm::degrees(atan2(diff.y, -diff.x)), 0.f });
	}
}

void Game::mousePassive(glm::vec2 mouse) {
	input.mouse = mouse;
	if (!input.xBox->getConnected(0))
		switch (state) {
		case State::Play: {
			glm::vec3 pPos = player->getPosition();
			glm::vec2 diff = glm::vec2(pPos.x, pPos.z) + windowSize * 0.5f - mouse;
			player->setRotation({ 0.f, glm::degrees(atan2(diff.y, -diff.x)), 0.f });
			break;
		} case State::Menu:
			if (mouse.x > screen.play.pos.x * windowSize.x && mouse.x < screen.play.pos.y * windowSize.x && mouse.y > screen.play.pos.z * windowSize.y && mouse.y < screen.play.pos.w * windowSize.y)
				screen.play.obj->color.b = 0.f;
			else screen.play.obj->color.b = 1.f;
			if (mouse.x > screen.quit.pos.x * windowSize.x && mouse.x < screen.quit.pos.y * windowSize.x && mouse.y > screen.quit.pos.z * windowSize.y && mouse.y < screen.quit.pos.w * windowSize.y)
				screen.quit.obj->color.b = 0.f;
			else
				screen.quit.obj->color.b = 1.f;
			break;
		default:
			break;
		}
}

void Game::controllerInput(unsigned short index, Input::Button button) {
	if (index == 0)
		switch (state) {
		case State::Play:
			if (button == Input::Button::Start)
				state = State::Pause;
			if (button == Input::Button::RB)
				player->reload = true;
			if (button == Input::Button::X)
				lightOn = !lightOn;
			break;
		case State::Pause:
			if (button == Input::Button::A)
				state = State::Play;
			if (button == Input::Button::B) {
				reset();
			}
			break;
		case State::Lose: case State::Win:
			if (button == Input::Button::B) {
				reset();
			}
			break;
		case State::Menu:
			if (button == Input::Button::A)
				state = State::Control;
			if (button == Input::Button::Start)
				glutExit();
			break;
		case State::Control:
			if (button == Input::Button::B)
			{
				soundList[0]->stopSound();
				soundList[1]->playSound(2);
				soundList[1]->setVolume(0.05f);
				soundList[4]->playSound(2);
				state = State::Play;
			}
			break;
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
			if (triggers.second > 0.5)
			{
				if (player->reloadCd <= 0.0f) {
					player->firing = true;
					soundList[3]->stopSound();
					soundList[2]->playSound(3);
					soundList[2]->setVolume(0.05f);
				}
			}
			else if(triggers.second < 0.5)
			{
				player->firing = false;
				soundList[2]->stopSound();
			}
			
			//	lightOn = !lightOn;
			
		}
		break;
	default:
		break;
	}
}

void Game::createDropItem(glm::vec3 pos, int type) {
	float temp = 0;
	if (type == 0)
	{
		temp = rand() % 100;
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
		//drop->hp = 5.0f;
		//dropHP->color = glm::vec4(0.0f, 1.0f, 0.f, 0.5f);
		dropHP->setPosition(pos);
		dropHP->update(deltaTime);
		//std::cout << "DROP!" << dropItems.size() << std::endl;
		dropItems.push_back(new Object(*dropHP));
	}
	//else
	//{
	//	drop->ammo = 0.0f;
	//	drop->hp = 0.0f;
	//}
}
//
//void Game::drawAmmo() {
//	if (hud.ammo.number) {
//		hud.ammo.number->ammo = player->ammo / 10;
//		hud.ammo.number->setPosition(hud.ammo.positions[0]);
//		hud.ammo.number->draw(program["numbers"], level.camera, { hud.light });
//		hud.ammo.number->ammo = player->ammo;
//		hud.ammo.number->setPosition(hud.ammo.positions[1]);
//		hud.ammo.number->draw(program["numbers"], level.camera, { hud.light });
//		hud.ammo.number->ammo = player->ammoDepo / 100;
//		hud.ammo.number->setPosition(hud.ammo.positions[2]);
//		hud.ammo.number->draw(program["numbers"], level.camera, { hud.light });
//		hud.ammo.number->ammo = player->ammoDepo / 10;
//		hud.ammo.number->setPosition(hud.ammo.positions[3]);
//		hud.ammo.number->draw(program["numbers"], level.camera, { hud.light });
//		hud.ammo.number->ammo = player->ammoDepo;
//		hud.ammo.number->setPosition(hud.ammo.positions[4]);
//		hud.ammo.number->draw(program["numbers"], level.camera, { hud.light });
//	}
//}

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
	soundList[0]->setVolume(0.01f);
	soundList[4]->stopSound();
	state = State::Menu;
}