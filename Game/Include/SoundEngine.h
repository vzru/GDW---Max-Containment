#pragma once
#include <Windows.h>
#include "FMOD/fmod.hpp"
#include <math.h>
#include "FMOD/fmod_errors.h"
#include <iostream> //cin & cout
//#include <conio.h> //Keyboard input

void FmodErrorCheck(FMOD_RESULT result);

struct Listener
{
	FMOD_VECTOR pos = { 0.0f, 0.0f, -1.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
};

class SoundEngine
{
private:
	bool init = false;
	FMOD_RESULT result;

public:
	SoundEngine();
	~SoundEngine();
	bool Init();
	void CleanUp();

	FMOD::System	*system;
	unsigned int	version;
	void			*extradriverdata = 0;
	Listener listener;
};