#pragma once
#include <GL\glut.h>

/*	keeps track of elapsed time
 *	so we can use it for things like
 *	physics calculations
 */
class Timer {
public:
	Timer();
	~Timer();

	float update();

	float getElapsedTime();
	float getCurrentTime();
private:
	float previousTime, currentTime, elapsedTime;
};