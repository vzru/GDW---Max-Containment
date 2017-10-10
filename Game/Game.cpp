#include <iostream>

#include <Windows.h>

#include "Game.h"
#include "Graphics.h"
#include "Player.h"
#include "Timer.h"
#include "Camera.h"

Game::Game(int& argc, char** argv) {
	// Memory Leak Detection
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	timer = new Timer();
	camera = new Camera();
	XBoxController = new Input::XBoxInput();
	player = new Player();

	/* initialize the window and OpenGL properly */

	// Request an OpenGL 4.4 compatibility
	// A compatibility context is needed to use the provided rendering utilities 
	// which are written in OpenGL 1.1
	glutInitContextVersion(4, 4);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize.x, windowSize.y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Title");

	auto s = glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << s << std::endl;

	// Init GL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}
Game::~Game() {}

void Game::update() {
	deltaTime = timer->tick();
}
void Game::draw() {
	// Set up scene
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, windowSize.x, windowSize.y);
	gluPerspective(60.0f, (float)windowSize.x / (float)windowSize.y, 0.001f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	Graphics::drawGrid();

	camera->update();
		
	player->update(deltaTime);
	player->draw();

	for (auto& enemy : enemies) {
		enemy->update(deltaTime);
		enemy->draw();
	}

	// Swap buffers
	glutSwapBuffers();
}

// input callback functions
void Game::keyboardDown(unsigned char key, glm::vec2 mouse) {
	switch (key) {
	case 27: // esc key
		glutExit();
		//exit(1);
		break;
	case 'T': case 't':
		std::cout << "Total elapsed time: " << timer->getCurrentTime() / 1000.0f << std::endl;
		break;

	case 'W': case 'w':
		player->movement |= Up;
		break;
	case 'S': case 's':
		player->movement |= Down;
		break;
	case 'A': case 'a':
		player->movement |= Left;
		break;
	case 'D': case 'd':
		player->movement |= Right;
		break;
	case 'Q': case 'q':
		break;
	case 'E': case 'e':
		break;

	default:
		break;
	}
}
void Game::keyboardUp(unsigned char key, glm::vec2 mouse) {
	std::cout << player->movement << std::endl;

	switch (key) {

	case 'W': case 'w':
		player->movement &= ~Up;
		break;
	case 'S': case 's':
		player->movement &= ~Down;
		break;
	case 'A': case 'a':
		player->movement &= ~Left;
		break;
	case 'D': case 'd':
		player->movement &= ~Right;
		break;
	case 'Q': case 'q':
		break;
	case 'E': case 'e':
		break;

	default:
		break;
	}
}
void Game::windowReshape(glm::vec2 size) {
	windowSize = size;
}
void Game::mouseClicked(int button, int state, glm::vec2 mouse) {
	if (state == GLUT_DOWN)
	{
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

	mousePosition = mouse;
}
void Game::mouseMoved(glm::vec2 mouse) {
	glm::vec2 change = mouse - mousePosition;
	if (mousePosition.length() > 0 && !(abs(change.x) >= 200.0f) && !(abs(change.y) >= 200.0f))
		camera->processMotion(change, timer->getElapsedTimeMS());

	mousePosition = mouse;
}
void Game::mousePassive(glm::vec2 mouse) {
}
void Game::specialInput(int key, glm::vec2 mouse) {
	switch (key) {
	case GLUT_KEY_UP:
		camera->moveZ(5);
		// ...
		break;
	case GLUT_KEY_DOWN:
		camera->moveZ(-5);
		// ...
		break;
	case GLUT_KEY_LEFT:
		camera->moveX(-5);
		// ...
		break;
	case GLUT_KEY_RIGHT:
		camera->moveX(5);
		// ...
		break;
	case GLUT_KEY_PAGE_UP:
		camera->moveY(5);
		// ...
		break;
	case GLUT_KEY_PAGE_DOWN:
		camera->moveY(-5);
		// ...
		break;
	default:
		break;
	}
}

void Game::controllerInput(unsigned short index, Input::Button button) {
	if (index == 0) {
		switch (button) {
		//case Input::A:case Input::B:case Input::X:case Input::Y:case Input::DPadDown:case Input::DPadLeft:case Input::DPadRight:case Input::DPadUp:
		//case Input::LB:case Input::RB:case Input::R3:case Input::L3:
		//	std::cout << "blargh!" << std::endl;
		//	break;

		default:
			break;
		}
	}
}

void Game::controllerSpecial(unsigned short index, Input::Triggers triggers, Input::Sticks sticks) {
	if (index == 0) {
		//std::cout << triggers.first << ':' << triggers.second << ',' << sticks.first.x << ':' << sticks.first.y << ',' << sticks.second.x << ':' << sticks.second.y << std::endl;
		player->setPosition(player->getPosition() + glm::vec3(sticks.first.x / 3.0f, 0, -sticks.first.y / 3.0f));
	}
}