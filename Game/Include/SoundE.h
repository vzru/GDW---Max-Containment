// Victor Zhang 100421055 Feb. 6, 2018
#pragma once
//This pre-processor definition add the lib file in code instead of using the project settings.
//This makes adding your code to other projects easier, but it may not work on other operating
//systems or compilers.
#pragma comment (lib, "fmod_vc.lib")

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"
#include <vector>
using namespace std;

class SoundE
{
public:
	SoundE();
	~SoundE();
	void fmodErrorCK(FMOD_RESULT result);
	void initializeS();
	void updateS();
	FMOD::System* getSystem();
	void changeListenerLoc(FMOD_VECTOR pos);
	void unload();
	bool init = false;
private:
	// Sound Engine
	FMOD::System *sSystem;
	FMOD_RESULT result;
	void *extradriverdata = 0;
	unsigned int version;

	// Listener
	FMOD_VECTOR listenerpos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

};

