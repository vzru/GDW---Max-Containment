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
#include "ParticleEmitterSoA.h"
#include "FrameBufferObject.h"

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

Game* Game::init(InputControl) {
	// Initialize game elements
	timer = new Timer();
	assets = Assets::getInstance();
	input.xBox = new Input::XBox();
	input.xBox->callback = _controllerInput;
	input.xBox->special = _controllerSpecial;
	fboD.create(windowSize.x, windowSize.y, 1, false);

	//fboD.createFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 1, true);

	//fboD.bindFrameBufferForDrawing();

	// Initialize loading screen
	screen.camera = new Camera(windowSize);
	screen.camera->setPosition({ 0.f, 5.f, 0.f });
	screen.camera->update();
	screen.light = new Light();
	screen.light->type = (unsigned int)Type::Light::DIRECTIONAL;
	screen.light->direction = glm::vec4(consts.direction.down, 0.f);
	screen.light->ambient = { 0.15f, 0.15f, 0.15f };
	screen.light->diffuse = { 0.7f, 0.7f, 0.7f };
	screen.light->specular = { 1.f, 1.f, 1.f };
	screen.light->specExponent = 50.f;
	screen.light->attenuation = { 1.f, 0.1f, 0.01f };
	screen.loading = new Object();
	screen.loading->loadMesh(assets->loadMesh("screen", "screen.obj"));
	screen.loading->loadTexture(Type::DIFFUSE, assets->loadTexture("loading", "loading.png"));

	fboD.clear({ 0,1,0, 1.0f });
	screen.loading->draw(assets->loadShader("Unlit", "animation.vert", "unlitTexture_f.glsl"), screen.camera);
	glutSwapBuffers();

	times.open = glutGet(GLUT_ELAPSED_TIME);

	std::cout << times.open << " milliseconds to get to loading screen" << std::endl;

	assets->loadSound("soundtrack", "game soundtrack.wav", true, 2)
		->createChannel(2, false)//(0, false)
		->setVolume(0.03f);//(0, 0.1f);
	//Sound* sound = new Sound("assets/sounds/game soundtrack.wav", true, 2);
	//Sound* sound = new Sound("assets/sounds/SW.mp3", true, 2);
	//sounds.push_back(sound);
	//sounds[0]->createChannel(0, false);
	//sounds[0]->setVolume(0, 0.1f);
	
	loadAssets();

	// Initialize Player
	player = new Player();
	player->loadTexture(Type::Texture::DIFFUSE, assets->textures["player color"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"])
		->loadTexture(Type::Texture::NORMAL, assets->textures["player normal"]);
	//	->loadMesh(assets->meshes["player"]);
	for (int i = 0; i < 9; i++)
		player->loadAnimationFrame(assets->aMeshes["playerFrame" + std::to_string(i)]);
	player->reset(level.start)
		->bullet = new Bullet();
	player->bullet->loadMesh(assets->meshes["bullet"]);
	player->bullet->color = glm::vec4(consts.color.white, 0.3f);

	// Initialize Shaders
	initShaders();
	// Initialize Sounds
	initSounds();
	// Initialize Level
	initLevel();
	// Initialize Screens
	initScreens();
	// Initialize HUD
	initHUD();
	// Initialize Enemies
	initEnemies();
	// Initialize Particles
	initParts();
	// Initialize Items
	initItems();

	// Load Enemies
	loadEnemies();
	// Load Drops
	loadDrops();
	// Load Signs
	loadSigns();


	times.init = glutGet(GLUT_ELAPSED_TIME);
	std::cout << times.init - times.load - times.open << " milliseconds to initialize the rest" << std::endl;
	std::cout << times.init << " milliseconds to load and initialize everything" << std::endl;
	return this;
}

void Game::loadAssets() {
	// level
	//assets->loadMesh("level", "Laboratory Level Triangulated.obj");
	assets->loadMesh("level", "new level.obj");
	//assets->loadMesh("hitboxes", "Laboratory Level Hitboxes Triangulated.obj");
	//assets->loadTexture("level color", "Lab_textures.png");
	assets->loadTexture("level color", "new level texture.png");
	assets->loadTexture("level normal", "new level texture normals.png");
	// screens
	//assets->loadMesh("screen", "screen.obj");
	assets->loadMesh("quad", "quad.obj");
	assets->loadTexture("pause", "pause.png");
	assets->loadTexture("win", "win.png");
	assets->loadTexture("lose", "lose.png");
	assets->loadTexture("controls", "controlsNew.png");
	assets->loadTexture("menu", "menu.png");
	// buttons
	assets->loadMesh("play", "Play Button.obj");
	assets->loadMesh("quit", "Quit Button.obj");
	assets->loadMesh("resume", "resume.obj");
	assets->loadMesh("credits", "credits.obj");
	// player
	//assets->loadMesh("player", "character_model.obj");
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
	assets->loadMesh("health", "health.obj");
	assets->loadMesh("ammo", "ammo.obj");
	assets->loadMesh("night_v", "night_v.obj");
	assets->loadTexture("item", "drops.png");
	assets->loadMesh("splatter", "splatter.obj");
	assets->loadTexture("corpse", "splatter.png");
	assets->loadMesh("exitR", "exit.obj");
	assets->loadMesh("exitL", "exit left.obj");
	assets->loadTexture("exitR", "exit1.png");
	assets->loadTexture("exitL", "exit1.png");
	// enemy
	//assets->loadMesh("enemy0", "enemy3_model.obj");
	//assets->loadMesh("enemy1", "enemy_model.obj");
	//assets->loadMesh("enemy2", "enemy2_model.obj");
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

	assets->loadAnimation("playerFrame", "player0", "obj", 9);

	assets->loadAnimation("damaged", "dam0", "obj", level.enemy[0].frames);

	assets->loadAnimation("deformed", "def0", "obj", level.enemy[1].frames);

	assets->loadAnimation("demented", "dem0", "obj", level.enemy[2].frames);

	times.load = glutGet(GLUT_ELAPSED_TIME);
	std::cout << times.load - times.open << " milliseconds to load in assets" << std::endl;
}

void Game::initShaders() {
	assets->loadShader("PhongSpot", "animation.vert", "phongSpot.frag");
	assets->loadShader("passThrough", "passThrough.vert", "passThrough.frag");
	assets->loadShader("PhongNoTexture", "Phong.vert", "PhongNoTexture.frag");
	assets->loadShader("PhongColorSides", "PhongColorSides.vert", "PhongColorSides.frag");
	assets->loadShader("numbers", "Phong.vert", "numbers.frag");
	assets->loadShader("particles", "passThru.glsl", "particles_g.glsl", "unlitTexture_f.glsl");
}

void Game::initSounds() {
	assets->loadSound("soundtrack", "game soundtrack.wav", true, 2)
		->createChannel(2, false)
		->setVolume(0.03f);
	assets->loadSound("dialogue", "Dialogue Placeholder.wav", false, 2);
	assets->loadSound("ambient", "ambient machine noise.wav", true, 2);

	assets->loadSound("gunshot", "Gunshot_sound2.wav", true, 3);
	assets->loadSound("reload", "Reload_sound.wav", false, 3);

	assets->loadSound("damaged", "Damaged.wav", true, 3);
	assets->loadSound("deformed", "Deformed.wav", true, 3);
	assets->loadSound("demented", "Demented.wav", true, 3);

	//Sound* sound = new Sound("assets/sounds/game soundtrack.wav", true, 2);
	//soundList.push_back(sound);
	//sound = new Sound("assets/sounds/ambient machine noise.wav", true, 2);
	//soundList.push_back(sound);
	//sound = new Sound("assets/sounds/Gunshot_sound.wav", true, 3);
	//soundList.push_back(sound);
	//sound = new Sound("assets/sounds/Reload_sound.wav", false, 3);
	//soundList.push_back(sound);

	//Sound* sound1 = new Sound("assets/sounds/ambient machine noise.wav", true, 2);
	//sounds.push_back(sound1);
	//Sound* sound2 = new Sound("assets/sounds/Gunshot_sound2.wav", false, 3);
	//sounds.push_back(sound2);
	//Sound* sound3 = new Sound("assets/sounds/Reload_sound.wav", false, 3);
	//sounds.push_back(sound3);
	//Sound* sound4 = new Sound("assets/sounds/Dialogue Placeholder.wav", false, 2);
	//sounds.push_back(sound4);
	//Sound* sound5 = new Sound("assets/sounds/Damaged.wav", true, 3);
	////sound5->set3DDist(0.5f, 5.f);
	////sound5->setMode(FMOD_3D_INVERSEROLLOFF);
	//sounds.push_back(sound5);
	//Sound* sound6 = new Sound("assets/sounds/Deformed.wav", true, 3);
	////sound6->set3DDist(0.5f, 5.f);
	////sound6->setMode(FMOD_3D_INVERSEROLLOFF);
	//sounds.push_back(sound6);
	//Sound* sound7 = new Sound("assets/sounds/Demented.wav", true, 3);
	////sound7->set3DDist(0.5f, 5.f);
	////sound7->setMode(FMOD_3D_INVERSEROLLOFF);
	//sounds.push_back(sound7);

	//soundList[1]->createChannel();
}

void Game::initLevel() {
	// Initialize level
	level.map = new Object();
	level.collision = new Level(0.5f);
	level.hitboxes = new Object({ 0.f, -8.f, 0.f });
	level.map->loadMesh(assets->meshes["level"]);
	//level.collision->load("assets/meshes/Laboratory Level Hitboxes.obj");
	level.collision->load("assets/meshes/new level hitboxes.obj");
	level.hitboxes->loadMesh(assets->meshes["hitboxes"]);
	level.map->loadTexture(Type::Texture::DIFFUSE, assets->textures["level color"]);
	level.map->loadTexture(Type::Texture::NORMAL, assets->textures["level normal"]);
	level.hitboxes->update(deltaTime);
	level.camera = new Camera(windowSize);
	level.camera->setPosition({ 0.f, 12.5f, 2.5f });
	level.camera->update();

	level.light = new Light();
	level.light->type = (unsigned int)Type::Light::POINT;
	level.light->position = { 0.f, 1.f, 0.f, 1.f };
	level.light->ambient = { 0.15f, 0.15f, 0.15f };
	level.light->diffuse = { 0.7f, 0.7f, 0.7f };
	level.light->specular = { 1.f, 1.f, 1.f };
	level.light->specExponent = 50.f;
	level.light->attenuation = { 1.f, 1.f, 1.f };
	level.lights.push_back(level.light);
	
	// Loads in the array containing all the lights in the level
	for (auto& pos : level.lightsPos) {
		std::cout << "Load Light " << level.lights.size() << std::endl;
		Light *light = new Light();
		light->type = (unsigned int)Type::Light::SPOT;
		light->position = glm::vec4(pos, 1.f);
		light->direction = glm::vec4(consts.direction.down, 0.f);
		light->ambient = { 0.15f, 0.15f, 0.15f };
		light->diffuse = { 0.7f, 0.7f, 0.7f };
		light->specular = { 1.f, 1.f, 1.f };
		light->specExponent = 50.f;
		light->spotExponent = 1.f;
		light->cutoff = glm::cos(glm::radians(15.5f));
		light->outerCutoff = glm::cos(glm::radians(20.5f));
		light->partial = 0.1;
		light->attenuation = { 0.5f, 0.1f, 0.01f };
		level.lights.push_back(light);
		//level.lightNoFlash.push_back(light);
	}

	// Flashlight gets put into the last slot
	level.flashLight = new Light();
	level.flashLight->type = (unsigned int)Type::Light::SPOT;
	level.flashLight->position = { 0.f, 0.f, 0.f, 1.f };
	level.flashLight->direction = { 0.f, 0.5f, 0.f, 0.f };
	level.flashLight->ambient = { 0.15f, 0.15f, 0.15f };
	level.flashLight->diffuse = { 0.7f, 0.7f, 0.7f };
	level.flashLight->specular = { 1.f, 1.f, 1.f };
	level.flashLight->specExponent = 50.f;
	level.flashLight->spotExponent = 1.f;
	level.flashLight->cutoff = glm::cos(glm::radians(15.5f));// 55.f);
	level.flashLight->outerCutoff = glm::cos(glm::radians(20.5f));// 70.f);
	level.flashLight->partial = 0.1;
	level.flashLight->attenuation = { 0.5f, 0.1f, 0.01f };
	//level.lights.push_back(level.flashLight);	// start the game with the light off

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
	level.light3->outerCutoff = glm::radians(1.f);
	level.light3->partial = 0.3;
	level.light3->attenuation = { 1.f, 0.5f, 0.1f };*/
}

void Game::initEnemies() {
	for (int i = 0; i < 3; i++) {
		level.enemy[i].type = new Enemy();
		for (int t = 0; t < level.enemy[i].frames; t++)
			level.enemy[i].type->loadAnimationFrame(assets->aMeshes[level.enemy[i].name + std::to_string(t)]);
		level.enemy[i].type->loadTexture(Type::Texture::DIFFUSE, assets->textures["enemy" + std::to_string(i) + " color"]);
		level.enemy[i].type->loadTexture(Type::Texture::NORMAL, assets->textures["enemy" + std::to_string(i) + " normal"]);
		level.enemy[i].type->life = level.enemy[i].stats.life;
		level.enemy[i].type->moveSpeed = level.enemy[i].stats.moveSpeed;
		level.enemy[i].type->damage = level.enemy[i].stats.damage;
		level.enemy[i].type->points = level.enemy[i].points;
	}
}

void Game::initParts() {
	ParticleEmitterSoA *part = new ParticleEmitterSoA();
	part->lifeR = { 0.001f, 0.01f, 0.0f };
	part->initForceMin = { -0.3f, -0.5f, -0.3f };
	part->initForceMax = { 0.3f, 0.0f, 0.3f };
	part->size = 0.02f;
	part->color = { 0.7f, 0.7f, 0.7f };
	//part->material = materials["particles"];
	//part[index].texture = textures["smoke"];
	part->dtFactor = 50.f;
	part->init(10);
	part->play();
	part->initPos = player->getPosition() + glm::vec3(1.0f, 1.0f, 0.0f);
	parts.push_back(part);
	parts[0]->pause();
}

void Game::initItems() {
	// Initialize Drops
	dropHP = (new Object())
		->loadMesh(assets->meshes["health"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["item"])
	//	->loadTexture(Type::Texture::NORMAL, assets->textures["item"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"])
		->update();
	dropHP->life = 4.0f;

	dropAmmo = (new Object())
		->loadMesh(assets->meshes["ammo"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["item"])
	//	->loadTexture(Type::Texture::NORMAL, assets->textures["item"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"])
		->update();
	dropAmmo->ammo = 30.0f;

	corpse = (new Object())
		->loadMesh(assets->meshes["splatter"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["corpse"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"])
		->update();

	level.exitR = (new Object())
		->loadMesh(assets->meshes["exitR"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["exitR"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"])
		->update();

	level.exitL = (new Object())
		->loadMesh(assets->meshes["exitL"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["exitL"])
		->loadTexture(Type::Texture::SPECULAR, assets->textures["fullSpecular"])
		->update();
}

void Game::initScreens() {
	// Initialize Images
	screen.controls = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["controls"]);

	screen.menu = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["menu"]);

	screen.pause = (new Object({ 0.f, 8.f, 1.2f }))
		->setRotation(hud.angle)
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["pause"]);

	screen.win = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["win"]);

	screen.lose = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::DIFFUSE, assets->textures["lose"]);

	// Initialize buttons
	screen.play.obj = (new Object(screen.play.center))
		->loadMesh(assets->meshes["play"])
		->setScale(glm::vec3(screen.play.scale))
		->update();
	screen.play.obj->color = glm::vec4(consts.color.white, 1.f);

	screen.quit.obj = (new Object(screen.quit.center))
		->loadMesh(assets->meshes["quit"])
		->setScale(glm::vec3(screen.quit.scale))
		->update();
	screen.quit.obj->color = glm::vec4(consts.color.white, 1.f);

	screen.resume.obj = (new Object(screen.resume.center))
		->loadMesh(assets->meshes["resume"])
		->setScale(glm::vec3(screen.resume.scale))
		->update();
	screen.resume.obj->color = glm::vec4(consts.color.white, 1.f);

	screen.credit.obj = (new Object(screen.credit.center))
		->loadMesh(assets->meshes["credit"])
		->setScale(glm::vec3(screen.credit.scale))
		->update();
	screen.credit.obj->color = glm::vec4(consts.color.white, 1.f);
}

void Game::initHUD() {
	// Initialize HUD
	hud.display = (new Object({ 0.004f, 10.6f, 1.967f }))
		->loadMesh(assets->meshes["hud"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["hud"])
		->setRotation(hud.angle)
		->setScale(glm::vec3(0.395f))
		->update();
	hud.healthBar = (new Object({ -1.846f, 10.4f, 3.027f }))
		->loadMesh(assets->meshes["hp bar"])
		->setRotation(hud.angle)
		->update();
	hud.healthBar->color = glm::vec4(consts.color.red, 1.f);
	hud.light = new Light();
	hud.light->type = (unsigned int)Type::Light::DIRECTIONAL;
	hud.light->direction = glm::vec4(consts.direction.down, 0.f);
	hud.light->ambient = { 0.15f, 0.15f, 0.15f };
	hud.light->diffuse = { 0.7f, 0.7f, 0.7f };
	hud.light->specular = { 1.f, 1.f, 1.f };
	hud.light->specExponent = 50.f;
	hud.light->attenuation = { 1.f, 0.1f, 0.01f };
	hud.ammo.number = (new Object())
		->loadMesh(assets->meshes["screen"])
		->loadTexture(Type::Texture::DIFFUSE, assets->textures["numbers"])
		->setScale(glm::vec3(0.02f, 1.f, 0.05f))
		->setRotation(hud.angle);
}

void Game::loadDrops() {
	for (int i = 0; i < 3; i++)
		for (auto& pos : level.drops[i])
			createDropItem({ pos.x, 0.0, pos.y }, i);
}

void Game::loadSigns() {
	for (auto& pos : level.exitPosR) {
		level.exitR->setPosition(glm::vec3(pos))
			->setRotation({ 0.0f, -90.f * pos.w, 0.f })
			->update(deltaTime);
		items.push_back(new Object(*level.exitR));
	}
	for (auto& pos : level.exitPosL) {
		level.exitL->setPosition(glm::vec3(pos))
			->setRotation({ 0.0f, -90.f * pos.w, 0.f })
			->update(deltaTime);
		items.push_back(new Object(*level.exitL));
	}
}

void Game::loadEnemies() {
	for (auto& enemy : level.enemy)
		for (auto& position : enemy.positions) {
			glm::vec3 temp = { position.x, 0.0f, position.y };
			enemy.type->setPosition(temp);
			enemies.push_back(new Enemy(*enemy.type));
			assets->sounds[enemy.name]->createChannel(3, true, Helper::castVec(temp))
				->changeRolloffMode(false)
				->changeMinMaxDist(0.f, 11.0f);
		}
	assets->sounds["damaged"]->setVolume(0.4f);
}

void Game::reloadEnemies() {
	for (int i = 0; i < 3; i++)
		for (auto& position : level.enemy[i].positions) {
			level.enemy[i].type->setPosition({ position.x, 0.f, position.y });
			enemies.push_back(new Enemy(*level.enemy[i].type));
		}
}

Game::~Game() {
	// clearing
	clearEnemies();
	clearDrops();
	clearItems();
	clearParts();
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
	level.lightsPos.clear();
	for (auto& light : level.lights)
		delete light;
	level.lights.clear();
	delete level.light;
	delete level.flashLight;
	level.exitPosL.clear();
	level.exitPosR.clear();
	delete level.exitR;
	delete level.exitL;
	// screen components
	delete screen.loading;
	delete screen.controls;
	delete screen.credits;
	delete screen.menu;
	delete screen.pause;
	delete screen.win;
	delete screen.lose;
	delete screen.camera;
	delete screen.light;
	delete screen.loading;
	delete screen.play.obj;
	delete screen.quit.obj;
	delete screen.resume.obj;
	delete screen.credit.obj;
	// hud components
	delete hud.display;
	delete hud.healthBar;
	delete hud.camera;
	delete hud.light;
	delete hud.ammo.number;
	hud.ammo.positions.clear();
	// stuff
	for (int i = 0; i < 3; i++) {
		level.drops[i].clear();
		level.enemy[i].positions.clear();
		delete level.enemy[i].type;
	}

//	delete level.light3;
//	for (int i = 0; i < partEList.size(); i++) {
//		partEList[i]->freeMemory();
//		delete partEList[i];
//	}
//	enemies.clear();
//	std::get<0>(level.enemies).clear();
//	std::get<1>(level.enemies).clear();
//	std::get<2>(level.enemies).clear();
//	delete std::get<0>(enemys);
//	delete std::get<1>(enemys);
//	delete std::get<2>(enemys);
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

void Game::clearItems() {
	for (auto& item : items)
		delete item;
	items.clear();
}

void Game::clearParts() {
	for (auto& part : parts)
		delete part;
	parts.clear();
}

void Game::update() {
	deltaTime = timer->update();
	input.xBox->update();
	//std::cout << "Keys: " << keys << std::endl;

	for (auto& part : parts)
		part->update(deltaTime);
	for (auto& sound : assets->sounds)
		sound.second->update();

	if (state == State::Play) {
		times.current += deltaTime;
		if (player->reloadCd > 0.0f && !player->reloaded) {
			assets->sounds["gunshot"]->stopSound();
			player->reloaded = true;
			assets->sounds["reload"]->playSound(0)
				->setVolume(0, 0.5f);
			//soundList[3]->playSound(3);
			//soundList[3]->setVolume(0, 0.5f);
		}
		
		//soundList[1]->createChannel();
		// player movement
		if (Helper::bitState(input.keys, Input::Keys::KeyW, Input::Keys::KeyS))
			player->acceleration.z = -0.6f;
		else if (Helper::bitState(input.keys, Input::Keys::KeyS, Input::Keys::KeyW))
			player->acceleration.z = 0.6f;
		else if (!input.xBox->getConnected(0))
			player->acceleration.z = 0.f;
		if (Helper::bitState(input.keys, Input::Keys::KeyA, Input::Keys::KeyD))
			player->acceleration.x = -0.6f;
		else if (Helper::bitState(input.keys, Input::Keys::KeyD, Input::Keys::KeyA))
			player->acceleration.x = 0.6f;
		else if (!input.xBox->getConnected(0))
			player->acceleration.x = 0.f;
		if (glm::length(player->acceleration) > 0.f)
			player->acceleration = glm::normalize(player->acceleration);
		if (Helper::bitState(input.keys, Input::Keys::KeyR))
			player->reload = true;
		player->update(deltaTime, level.collision);

		// stuff based on player
		glm::vec3 pPos = player->getPosition();
		FMOD_VECTOR listener = Helper::castVec(pPos);
		assets->sounds["soundrack"]->changeListenerLoc(listener);
		//std::cout << temp.x << '/' << temp.y << '/' << temp.z << std::endl;
		//std::cout << rand() % 100 << std::endl
		float partAngle = glm::radians(player->getRotation().y);
		//partEList[0]->direction = { cos(partAngle), 0, -sin(partAngle), 0.f };
		parts[0]->initPos = pPos + glm::vec3(cos(partAngle) * 0.7f, 1.0f, -sin(partAngle)* 0.7f);

		FMOD_VECTOR tempS = { pPos.x, 0.0f, pPos.z };
		if (assets->sounds["gunshot"]->chList.size())
			assets->sounds["gunshot"]->changeSoundLoc(0, tempS);
		if (assets->sounds["reload"]->chList.size())
			assets->sounds["reload"]->changeSoundLoc(0, tempS);
		//if(deltaTime >= 25.0f)
		//std::cout << deltaTime << std::endl;

		// stuff based on player
		//hud.display->setPosition(player->getPosition() + glm::vec3(0.004f, 10.6f, 1.967f));
		//screen.pause->setPosition(player->getPosition() + glm::vec3(0.f, 10.f, 0.f));
		//hud.healthBar->setPosition(player->getPosition() + glm::vec3(-1.846f, 10.4f, 3.027f));
		//hud.healthBar->setScale({ player->life * 0.0365f, 0.f, 0.035f });
		hud.display->update(deltaTime, pPos);
		screen.pause->update(deltaTime, pPos);
		hud.healthBar->setScale({ player->life * 0.0365f, 0.f, 0.035f })
			->update(deltaTime, pPos);
		hud.ammo.number->update(deltaTime);
		level.camera->update(pPos);
		level.light->position = glm::vec4(pPos + glm::vec3(0.f, 2.f, 0.f), 1.f);
		level.flashLight->position = glm::vec4(pPos + glm::vec3(0.f, 1.f, 0.f), 1.f);
		float angle = glm::radians(player->getRotation().y);
		level.flashLight->direction = { cos(angle), -0.1f, -sin(angle), 0.f };
		
		// enemy update
		Enemy* target = nullptr;
		glm::vec3 dist = { 0.f, 0.f, 20.f }; // line-point, to-current, furthest
		for (int i = 0; i < enemies.size(); i++) {
			auto& enemy = enemies[i];
			if (!enemy->collect) {
				glm::vec3 ePos = enemy->getPosition();
				// bullet collision
				if (player->bullet != nullptr && player->bullet->cooldown <= 0.f) {
					glm::vec3 diff = ePos - player->bullet->getPosition();
					float bang = glm::radians(player->bullet->getRotation().y);
					float dang = Helper::getAngle(diff);
					dist = glm::vec3(Helper::getDist(bang - dang, diff), dist.z);
					dist.x = abs(dist.x);
					// check if enemy is along the bullet path && not too far away
					if (dist.x < 1.f && dist.z > dist.y && dist.y > 0.f) {
						target = enemy;
						dist.z = dist.y;
					}
				}
				// enemy update
				glm::vec3 diff = ePos - pPos;
				enemy->range = glm::length(diff) < 20.0f;
				if (enemy->range) {
					FMOD_VECTOR loc = Helper::castVec(ePos);
					int type = Helper::convArray(i, level.enemy);
					assets->sounds[level.enemy[type].name]->changeSoundLoc(i - Helper::offsetIndex(type, level.enemy), loc);
					if (enemy->life <= 0.0f)
						assets->sounds[level.enemy[type].name]->setPause(i - Helper::offsetIndex(type, level.enemy), true);
					if (enemies[i]->knockbackCD > 0)
						enemies[i]->knockbackCD -= deltaTime / 1000;
					// hurt player
					if (enemy->cooldown <= 0.f && glm::length(diff) < 1.f) {
						player->life -= enemy->damage;
						enemy->cooldown = enemy->attackSpeed;
					}
					float bang = glm::radians(player->getRotation().y);
					float dang = Helper::getAngle(diff);
					glm::vec2 dist = Helper::getDist(bang - dang, diff); // line-point, to-current
					// seek towards player
					if ((enemy->triggered || glm::length(diff) < 5.0f || (glm::length(diff) < 15.0f && dist.x < 2.f && player->lightOn)) && glm::length(diff) > 1.0f && enemy->knockbackCD <= 0 && dist.y > 0.0f) {
						enemy->setRotation({ 0.f, glm::degrees(dang) - 90.f, 0.f });
						enemy->setVelocity(-glm::normalize(diff));
					} else	enemy->setVelocity(glm::vec3(0.f));
					// update enemy
					enemy->update(deltaTime, level.collision);
					// kill enemy
					if (enemies[i]->life <= 0.f) {
						createDropItem(enemies[i]->getPosition());
						createCorpse(enemies[i]->getPosition());
						enemies[i]->collect = true;
						//delete enemies[i];
						//enemies.erase(i + enemies.begin());
						//i--;
					}
				}
			}
		}
		if (target != nullptr) {
			target->life -= 2.5;
			target->triggered = true;
			score += target->points;
			if (target->KB) {
				target->knockbackCD = 0.2f;
				glm::vec3 dif = target->getPosition() - player->getPosition();
				target->setPosition(target->getPosition() + glm::normalize(dif) * 0.1f);
			}
			player->bullet->cooldown = 1.f;
		}
		glm::vec3 temp = player->getPosition();
		// player gun sound
		if (player->newShot) {
			assets->sounds["gunshot"]->stopSound()
				->playSound(3)
				->setVolume(0.05f);
			player->newShot = false;
		}
		// player loses
		if (player->life <= 0.f)
			state = State::Lose;
		// player wins
		if (Helper::inside(temp, level.exit))
			win();
		// item drops
		for (auto& drop : drops)
			if (!drop->collect && glm::length(temp - drop->getPosition()) < 0.5) {
				//std::cout << drop->ammo << '/' << drop->life << std::endl;
				drop->collect = true;
				player->ammoDepo += drop->ammo;
				if (player->life < player->MaxLife) {
					if ((player->life + drop->life) > player->MaxLife)
						player->life = player->MaxLife;
					else
						player->life += drop->life;
				}
				else if (player->life == player->MaxLife & drop->life > 0.0f)
					drop->collect = false;
			}
	}
}

void Game::draw() {
	// Set up scene
	fboD.clear({ 1.f, 0.f, 0.f, 1.f });
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(glm::radians(60.0f), windowSize.x / windowSize.y, 0.001f, 10000.0f);
	//glMatrixMode(GL_MODELVIEW);

	//cout << glm::degrees(level.light->cutoff) << '/' << glm::degrees(level.light->outerCutoff) << endl;

	switch (state) {
	case State::Pause:
		screen.pause->draw(assets->shaders["Phong"], level.camera, hud.light);
		screen.resume.obj->draw(assets->shaders["PhongNoTexture"], screen.camera, screen.light);
		screen.quit.obj->draw(assets->shaders["PhongNoTexture"], screen.camera, screen.light);
	fboD.clear({ 0,1,0, 1.0f });
		break;
	case State::Play: {
		player->draw(assets->shaders["PhongSpot"], level.camera, level.lights);
		for (auto& enemy : enemies)
			if (!enemy->collect && enemy->range)
				enemy->draw(assets->shaders["PhongSpot"], level.camera, level.lights);
		level.map->draw(assets->shaders["PhongSpot"], level.camera, level.lights);
		//level.hitboxes->draw(program["PhongColorSides"], level.camera, level.lights);
		hud.healthBar->draw(assets->shaders["PhongNoTexture"], level.camera, hud.light);
		hud.display->draw(assets->shaders["Phong"], level.camera, hud.light);

		int nums[] = { player->ammo / 10, player->ammo, player->ammoDepo / 100, player->ammoDepo / 10, player->ammoDepo };
		glm::vec3 temp = player->getPosition();
		for (int i = 0; i < hud.ammo.positions.size(); i++) {
			hud.ammo.number->ammo = nums[i];
			//std::cout << i << ": " << num[i] << std::endl;
			hud.ammo.number->setPosition(hud.ammo.positions[i])
				->update(deltaTime, temp)
				->draw(assets->shaders["numbers"], level.camera, hud.light);
		}

		parts[0]->draw(assets->shaders["particles"], level.camera);
		for (int i = 0; i < items.size(); i++)
			if (!items[i]->collect)
				items[i]->draw(assets->shaders["PhongSpot"], level.camera, level.lights);
				//parts[i + 1]->draw(program["particles"], level.camera);
		for (int i = 0; i < drops.size(); i++)
			if (!drops[i]->collect) {
				drops[i]->draw(assets->shaders["PhongSpot"], level.camera, level.lights);
				parts[i + 1]->play();
				parts[i + 1]->draw(assets->shaders["particles"], level.camera);
			}
		}	break;
	case State::Control:
		screen.controls->draw(assets->shaders["Unlit"], screen.camera);
		break;
	case State::Menu:
		screen.menu->draw(assets->shaders["Unlit"], screen.camera);
		screen.play.obj->draw(assets->shaders["PhongNoTexture"], screen.camera, screen.light);
		screen.quit.obj->draw(assets->shaders["PhongNoTexture"], screen.camera, screen.light);
		screen.credit.obj->draw(assets->shaders["PhongNoTexture"], screen.camera, screen.light);
		break;
	case State::Win:
		screen.win->draw(assets->shaders["Unlit"], screen.camera);
		break;
	case State::Lose:
		screen.lose->draw(assets->shaders["Unlit"], screen.camera);
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
			start();
			break;
		case State::Pause:
			state = State::Play;
			break;
		case State::Lose: case State::Win:
			reset();
			//soundList[0]->playSound();
			//sounds["soundtrack"]->playSound();
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
			//sounds["soundtrack"]->playSound();
		}
		//enemies.push_back(new Enemy({ rand() % 21 - 10 + player->getPosition().x, 0, rand() % 21 - 10 + player->getPosition().z }));
		break;
	default:
		break;
	}
	switch (key) {
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
	case 'V': case 'v':
		std::cout << (rand() * rand()) % 100 << std::endl;

		Helper::bitOn(input.keys, Input::Keys::KeyV); break;
	case '1':			Helper::bitOn(input.keys, Input::Keys::Num1); break;
	case '2':			Helper::bitOn(input.keys, Input::Keys::Num2); break;
	case '3':			Helper::bitOn(input.keys, Input::Keys::Num3); break;
	case '4':			Helper::bitOn(input.keys, Input::Keys::Num4); break;
	case ' ':			Helper::bitOn(input.keys, Input::Keys::Space); break;
	case 26:			Helper::bitOn(input.keys, Input::Keys::Esc); break;
	case 'U': case 'u': Helper::bitOn(input.keys, Input::Keys::KeyU);
		level.flashLight->cutoff += glm::radians(0.1f);
		break;
	case 'I': case 'i': Helper::bitOn(input.keys, Input::Keys::KeyI);
		level.flashLight->cutoff -= glm::radians(0.1f);
		break;
	case 'J': case 'j': Helper::bitOn(input.keys, Input::Keys::KeyJ);
		level.flashLight->outerCutoff += glm::radians(0.1f);
		break;
	case 'K': case 'k': Helper::bitOn(input.keys, Input::Keys::KeyK);
		level.flashLight->outerCutoff -= glm::radians(0.1f);
		break;
	default: break;
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
	case 'U': case 'u':	Helper::bitOff(input.keys, Input::Keys::KeyU); break;
	case 'I': case 'i':	Helper::bitOff(input.keys, Input::Keys::KeyI); break;
	case 'J': case 'j':	Helper::bitOff(input.keys, Input::Keys::KeyJ); break;
	case 'K': case 'k':	Helper::bitOff(input.keys, Input::Keys::KeyK); break;
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
					player->firing = true;
					//parts[0]->play();
					//soundList[3]->stopSound();
					//soundList[2]->playSound(3);
					//soundList[2]->setVolume(0.05f);
				} else
					;//parts[0]->pause();
				break;
			case GLUT_UP:
				player->firing = false;
				//parts[0]->pause();
				//sounds[2]->stopSound();
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
				player->lightOn = !player->lightOn;
				if (player->lightOn)
					level.lights.push_back(level.flashLight);
				else
					level.lights.pop_back();
				break;
			default:
				break;
			}	break;
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
				//soundList[1]->playSound();
				if (Helper::inside(mouse, screen.quit.pos, windowSize))
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
	case State::Pause:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			switch (state) {
			case GLUT_DOWN:
				if (Helper::inside(mouse, screen.resume.pos, windowSize))
					this->state = State::Play;
				//soundList[1]->playSound();
				if (Helper::inside(mouse, screen.quit.pos, windowSize))
					this->state = State::Menu;
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
			if (Helper::inside(mouse, screen.play.pos, windowSize))
				screen.play.obj->color.b = 0.f;
			else screen.play.obj->color.b = 1.f;
			if (Helper::inside(mouse, screen.quit.pos, windowSize))
				screen.quit.obj->color.b = 0.f;
			else screen.quit.obj->color.b = 1.f;
			break;
		default:
			break;
		}
}

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
			case Input::Button::X:
				player->lightOn = !player->lightOn;
				break;
			default:
				break;
			}	break;
		case State::Pause:
			switch (button) {
			case Input::Button::A:
				state = State::Play;
				break;
			case Input::Button::B:
				reset();
			default:
				break;
			}	break;
		case State::Lose: case State::Win:
			switch (button) {
			case Input::Button::B:
				reset();
				break;
			default:
				break;
			}	break;
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
				start();
				break;
			default:
				break;
			}	break;
		default:
			break;
		}
}

void Game::controllerSpecial(unsigned short index, Input::Triggers triggers, Input::Sticks sticks) {
	if (index == 0)
		switch (state) {
		case State::Play:
			player->acceleration = { sticks.first.x / 2.0f, 0.0f, -sticks.first.y / 2.0f };
			if (glm::length(player->acceleration) > 0.f)
				player->acceleration = glm::normalize(player->acceleration);
			if (glm::length(sticks.second) > 0.5f)
				player->setRotation({ 0.0f, glm::degrees(atan2(sticks.second.y, sticks.second.x)), 0.0f });

			if (triggers.second > 0.5) {
				if (player->reloadCd <= 0.0f) {
					player->firing = true;
					assets->sounds["reload"]->stopSound();
					assets->sounds["gunshot"]->playSound(3)
						->setVolume(0.05f);
					assets->sounds["reload"]->stopSound();
					if (assets->sounds["reload"]->chList.size() <= 0) {
						assets->sounds["gunshot"]->playSound(2)
							->setVolume(0.05f);
					}
				}
			} else {
				player->firing = false;
				assets->sounds["gunshot"]->stopSound();
			}
			break;
		default:
			break;
		}
}

void Game::createDropItem(glm::vec3 pos, int type) {
	static float temp = 0;
	ParticleEmitterSoA *partD = new ParticleEmitterSoA();
	if (!type)
		temp = rand() % 100;
	//std::cout << temp << '/' << player->health << std::endl;
	if (type == 2 || (temp > 50.0 & temp <= 80.0)) {
		dropAmmo->setPosition(pos);
		dropAmmo->update(deltaTime);
		drops.push_back(new Object(*dropAmmo));
		partD->color = consts.color.yellow;
	} else if (type == 1 || temp > 80.0) {
		dropHP->setPosition(pos);
		dropHP->update(deltaTime);
		drops.push_back(new Object(*dropHP));
		partD->color = consts.color.green;
	}
	//std::cout << "DROP!" << dropItems.size() << std::endl;
	
	if (type || temp > 50.0) {
		partD->lifeR = { 0.01f, 0.7f, 0.f };
		partD->initForceMin = { -0.5f, 0.f, -0.5f };
		partD->initForceMax = { 0.5f, 1.f, 0.5f };
		partD->size = 0.01f;
		partD->init(50);
		partD->dtFactor = 2000.f;
		partD->play();
		partD->initPos = pos + glm::vec3(0.0f, 0.3f, 0.0f);
		partD->playing = true;
		parts.push_back(partD);
	}
}

void Game::createCorpse(glm::vec3 pos) {
	corpse->setPosition(pos);
	corpse->update(deltaTime);
	items.push_back(new Object(*corpse));
}

void Game::start() {
	//soundList[0]->stopSound();
	//soundList[1]->playSound(2);
	//soundList[1]->setVolume(0.05f);
	//soundList[4]->playSound(2);
	//soundList[5]->setPause(false);
	//soundList[6]->setPause(false);
	//soundList[7]->setPause(false);
	assets->sounds["soundtrack"]->stopSound();
	assets->sounds["ambient"]->playSound(2)
		->setVolume(0.05f);
	assets->sounds["dialogue"]->playSound(2);
	
	state = State::Play;

	loadEnemies();
	loadDrops();
	loadSigns();
	//loadParts();
}
void Game::win() {
	times.clear = times.current;
	std::cout << "You cleared the level in " << times.clear << " miliseconds" << std::endl;

	state = State::Win;
}
void Game::reset() {
	//soundList[1]->stopSound();
	//soundList[4]->stopSound();
	//soundList[5]->setPause(true);
	//soundList[6]->setPause(true);
	//soundList[7]->setPause(true);
	//soundList[0]->playSound(2);
	//soundList[0]->setVolume(0.1f);
	assets->sounds["ambient"]->stopSound();
	assets->sounds["soundtrack"]->playSound(2)
		->setVolume(0.05f);
	
	state = State::Menu;
	
	player->reset(level.start);
	clearEnemies();
	clearDrops();
	clearItems();
	clearParts();
}