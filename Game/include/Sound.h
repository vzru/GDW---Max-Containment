// Victor Zhang 100421055 Mar. 17, 2018
#pragma once

#include "SoundE.h"

// Channel object to hold attributes
struct Channel
{
	FMOD::Channel *channel = 0;
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	bool pause = false;
};

class Sound
{
public:
	Sound();
	Sound(const std::string &file, bool loop, int dimension);
	~Sound();
	
	// Init
	Sound* loadSound(const char *filename, bool loop, int dimension);
	Sound* createChannel(int mode, bool pause);
	Sound* createChannel(int mode, bool pause, FMOD_VECTOR pos, FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f });
	Sound* playSound(int mode);
	
	// Channel Control
	Sound* stopSound();
	Sound* stopSound(int index);
	Sound* pauseSound(int index);
	Sound* setPause(bool p);
	Sound* setPause(int index, bool p);
	Sound* setVolume(float l);
	Sound* setVolume(int index, float l);
	
	// Attribute Changes
	Sound* changeSoundLoc(int index, FMOD_VECTOR pos);
	Sound* changeSoundVelo(int index, FMOD_VECTOR pos);
	Sound* changeListenerLoc(FMOD_VECTOR p);
	Sound* changeRolloffMode(bool l);
	Sound* changeRolloffMode(int index, bool l);
	Sound* changeMinMaxDist(float min, float max);
	Sound* changeMinMaxDist(int index, float min, float max);
	Sound* set3DDist(float min, float max);
	Sound* setMode(FMOD_MODE mode);
	
	// Update
	Sound* update();
	
	// Clean up
	Sound* unload();

	// Array of channels
	std::vector<Channel*> chList;
private:
	// Sound stored from file
	FMOD::Sound *sound;

	// Sound System
	static FMOD_RESULT result;
	static SoundE sSys;
};