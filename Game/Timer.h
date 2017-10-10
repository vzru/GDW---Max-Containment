#pragma once
#include <GL\glut.h>

// keeps track of elapsed time so we can use it for things like physics calculations

class Timer {
public:
	Timer();
	~Timer();

	float tick();
	float getElapsedTimeMS();
	float getElapsedTimeSeconds();
	float getCurrentTime();
	
private:
	float previousTime, currentTime, elapsedTime;
};