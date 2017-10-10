#include "Timer.h"

Timer::Timer() {
	previousTime = glutGet(GLUT_ELAPSED_TIME);
	tick();
}
Timer::~Timer() {}

// update the timer
float Timer::tick()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = currentTime - previousTime;
	previousTime = currentTime;

	return elapsedTime;
}

// return delta time in miliseconds
float Timer::getElapsedTimeMS() {
	return elapsedTime;
}

// return delta time in seconds
float Timer::getElapsedTimeSeconds() {
	return elapsedTime / 1000.0f;
}

// get time elapsed since program start
float Timer::getCurrentTime() {
	return currentTime;
}