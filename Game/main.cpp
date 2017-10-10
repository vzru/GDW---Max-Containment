#include "Game.h"

#define FRAMES_PER_SECOND 60

const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND;

Game* game;

void DisplayCallbackFunction(void) {
	game->draw();
}
void KeyboardCallbackFunction(unsigned char key, int x, int y) {
	game->keyboardDown(key, { x, y });
}
void KeyboardUpCallbackFunction(unsigned char key, int x, int y) {
	game->keyboardUp(key, { x, y });
}
void WindowReshapeCallbackFunction(int width, int height) {
	game->windowReshape({ width, height });
}
void MouseClickCallbackFunction(int button, int state, int x, int y) {
	game->mouseClicked(button, state, { x, y });
}
void MouseMotionCallbackFunction(int x, int y) {
	game->mouseMoved({ x, y });
}
void MousePassiveMotionCallbackFunction(int x, int y) {
	game->mousePassive({ x, y });
}
void TimerCallbackFunction(int value) {
	game->update();

	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0);
}
void SpecialInputCallbackFunction(int key, int x, int y) {
	game->specialInput(key, { x, y });
}
void ControllerInputCallbackFunction(unsigned short index, Input::Button button) {
	(*game->XBoxController->callback)(index, button);
}
void ControllerSpecialCallbackFunction(unsigned short index, Input::Triggers triggers, Input::Sticks sticks) {
	(*game->XBoxController->special)(index, triggers, sticks);
}

int main(int argc, char **argv) {
	// initialize game
	game = new Game(argc, argv);

	// setup callback functions
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutReshapeFunc(WindowReshapeCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutPassiveMotionFunc(MousePassiveMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);
	glutSpecialFunc(SpecialInputCallbackFunction);
	game->XBoxController->setCallbacks(ControllerInputCallbackFunction, ControllerSpecialCallbackFunction);

	// start game
	glutMainLoop();

	// end game
	system("pause");
	return 0;
}