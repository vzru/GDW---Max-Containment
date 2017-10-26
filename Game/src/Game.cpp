#include <iostream>
#include <memory>

#include <Windows.h>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Game.h"
#include "Graphics.h"
#include "Shader.h"
#include "Enemy.h"
#include "Player.h"
#include "Timer.h"
#include "Camera.h"
#include "Controller.h"

float dLinePoint(glm::vec2 point, glm::vec2 line, float angle) {
	//float inner = glm::dot(line, point);
	float inner = angle - atan2(point.y, point.y);
	//float length = glm::length(point) * sin(inner);
	float length = glm::length(point) * cos(inner);
	std::cout << inner, angle, length;
	return length;
}

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
	glutInitWindowSize(windowSize.x, windowSize.y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Max Containment");
	//glutFullScreen();

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW could not be initialized." << std::endl;
		system("pause");
		exit(0);
	}


	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Init GL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Initialize game elements
	timer = new Timer();
	camera = new Camera();
	xBox = new Input::XBox();
	player = new Player();
	program = new Shader();
	program->load("assets/shaders/Basic.vert", "assets/shaders/Basic.frag");

	// fill in a bunch of enemies
	for (int i = 0; i < 10; i++)
		enemies.push_back(new Enemy({ rand() % 21 - 10 + player->getPosition().x, 0, rand() % 21 - 10 + player->getPosition().z }));
}

Game::~Game() {
	delete timer;
	delete camera;
	delete xBox;
	delete player;
	for (auto& enemy : enemies)
		delete enemy;
	enemies.clear();
}

void Game::update() {
	deltaTime = timer->update();

	camera->move(player->velocity * (deltaTime / 50));
	camera->update();

	xBox->update();
	if (state == State::Play) {
		player->update(deltaTime);
		for (auto& enemy : enemies) {
			enemy->setRotation(enemy->getRotation() + glm::vec3(0, 5, 0));
			glm::vec3 diff = enemy->getPosition() - player->getPosition();
			if (glm::length(diff) < 10.0f) {
				enemy->velocity = -glm::normalize(diff);
				enemy->triggered = true;
			} else {
				enemy->velocity = { 0.0f,0.0f,0.0f };
				enemy->triggered = false;
			}
			enemy->update(deltaTime);
		}
	}
}

void Game::draw() {
	// Set up scene
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, windowSize.x, windowSize.y);
	gluPerspective(60.0f, (float)windowSize.x / (float)windowSize.y, 0.001f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
			

	switch (state) {
	case State::Play:
		Graphics::drawGrid({ 100,100,100 });
		player->draw(program, camera);
		for (auto& enemy : enemies)
			enemy->draw(program, camera);
		break;
	default:
		break;
	}

	// Swap buffers
	glutSwapBuffers();
}

// keyboard callback functions
void Game::keyboardDown(unsigned char key, glm::vec2 mouse) {
	switch (key) {
	case 27: // esc key
		switch (state) {
		case (State::Play):
			state = State::Pause;
			break;
		case (State::Pause):
			state = State::Play;
			break;
		case (State::Menu):
			glutExit();
			break;
		default:
			break;
		}
		break;
	case ' ':
		state = State::Play;
		break;
	case 'T': case 't':
		std::cout << "Total elapsed time: " << timer->getCurrentTime() / 1000.0f << std::endl;
		break;
	case 'Q': case 'q':
		
		enemies.push_back(new Enemy({ rand() % 21 - 10 + player->getPosition().x, 0, rand() % 21 - 10 + player->getPosition().z }));
		if (state == State::Pause)
		{
			glutExit();
		}
		break;
	default:
		break;
	}
	switch (state) {
	case State::Play:
		switch (key) {
		case 'W': case 'w':
			player->velocity.z -= (player->velocity.z >= 0 ? 1 : 0);
			break;
		case 'S': case 's':
			player->velocity.z += (player->velocity.z <= 0 ? 1 : 0);
			break;
		case 'A': case 'a':
			player->velocity.x -= (player->velocity.x >= 0 ? 1 : 0);
			break;
		case 'D': case 'd':
			player->velocity.x += (player->velocity.x <= 0 ? 1 : 0);
			break;
		case 'Q': case 'q':
			break;
		case 'E': case 'e':
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
}
void Game::keyboardUp(unsigned char key, glm::vec2 mouse) {
	switch (state) {
	case State::Play:
		switch (key) {
		case 'W': case 'w':
			player->velocity.z += 1;
			break;
		case 'S': case 's':
			player->velocity.z -= 1;
			break;
		case 'A': case 'a':
			player->velocity.x += 1;
			break;
		case 'D': case 'd':
			player->velocity.x -= 1;
			break;
		case 'Q': case 'q':
			break;
		case 'E': case 'e':
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Game::specialInput(unsigned char key, glm::vec2 mouse) {
	switch (key) {
	case GLUT_KEY_UP:
		//camera->pan({ 0,0,5 });
		break;
	case GLUT_KEY_DOWN:
		//camera->pan({ 0,0,-5 });
		break;
	case GLUT_KEY_LEFT:
		//camera->pan({ -5,0,0 });
		break;
	case GLUT_KEY_RIGHT:
		//camera->pan({ 5,0,0 });
		break;
	case GLUT_KEY_PAGE_UP:
		//camera->pan({ 0,5,0 });
		break;
	case GLUT_KEY_PAGE_DOWN:
		//camera->pan({ 0,-5,0 });
		break;
	default:
		break;
	}
}

void Game::windowReshape(glm::vec2 size) {
	windowSize = size;
	camera->reset();
}
// mouse callback functions
void Game::mouseClicked(int button, int state, glm::vec2 mouse) {
	if (button == GLUT_LEFT_BUTTON) {
		switch (state) {
		case GLUT_DOWN:
			player->firing = true;
			break;
		case GLUT_UP:
			player->firing = false;
			break;
		}
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
	glm::vec2 change = mouse - mousePosition;
	switch (state) {
	case State::Play:
		//if (mousePosition.length() > 0 && !(abs(change.x) >= 200.0f && abs(change.y) >= 200.0f))
			//camera->processMotion(change, timer->getElapsedTimeMS());
		mousePosition = mouse;
		break;
	default:
		break;
	}
	if (!xBox->getConnected(0)) {
		glm::vec3 pos = player->getPosition();
		glm::vec2 diff = glm::vec2(pos.x, pos.z) + windowSize * 0.5f - mouse;
		player->setRotation({ 0,glm::degrees(atan2(diff.y, -diff.x)),0 });
	}
	player->fire();
}
void Game::mousePassive(glm::vec2 mouse) {
	if (!xBox->getConnected(0)) {
		glm::vec3 pos = player->getPosition();
		glm::vec2 diff = glm::vec2(pos.x, pos.z) + windowSize * 0.5f - mouse;
		player->setRotation({ 0,glm::degrees(atan2(diff.y, -diff.x)),0 });
	}
}
// controller callback functions
void Game::controllerInput(unsigned short index, Input::Button button) {
	if (button == Input::Button::Start)
		state = state == State::Play ? State::Pause : State::Play;
}
void Game::controllerSpecial(unsigned short index, Input::Triggers triggers, Input::Sticks sticks) {
	switch (state) {
	case State::Play:
		if (index == 0) {
			player->velocity = { sticks.first.x / 2.0f, 0.0f, -sticks.first.y / 2.0f };
			if ( glm::length(sticks.second) > 0.5f )
				player->setRotation({ 0.0f, glm::degrees(atan2(sticks.second.y, sticks.second.x)), 0.0f });
			if (triggers.second > 0.5) {
				player->fire();
				for (auto& enemy : enemies) {
					float delta = dLinePoint(enemy->getPosition(), player->getPosition(), player->getRotation().y);
				}
			}
		}
		break;
	default:
		break;
	}
}