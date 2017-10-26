#include "Timer.h"

Timer::Timer() {
	previousTime = glutGet(GLUT_ELAPSED_TIME);
	update();
}
Timer::~Timer() {}

// update the timer
float Timer::update()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = currentTime - previousTime;
	previousTime = currentTime;

	return elapsedTime;
}

// return delta time in miliseconds
float Timer::getElapsedTime() {
	return elapsedTime;
}

// return time elapsed since program start in miliseconds
float Timer::getCurrentTime() {
	return currentTime;
}