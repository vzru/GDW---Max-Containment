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
	Sound(char *filename, bool loop, int dimension);
	~Sound();
	
	// Init
	void loadSound(char * filename, bool loop, int dimension);
	void createChannel(int mode, bool pause);
	void createChannel(int mode, bool pause, FMOD_VECTOR pos, FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f });
	void playSound(int mode);
	
	// Channel Control
	void stopSound();
	void stopSound(int index);
	void pauseSound(int index);
	void setPause(int index, bool p);
	void setVolume(float l);
	void setVolume(int index, float l);
	
	// Attribute Changes
	void changeSoundLoc(int index, FMOD_VECTOR pos);
	void changeSoundVelo(int index, FMOD_VECTOR pos);
	void changeListenerLoc(FMOD_VECTOR p);
	void changeRolloffMode(bool l);
	void changeRolloffMode(int index, bool l);
	void changeMinMaxDist(float min, float max);
	void changeMinMaxDist(int index, float min, float max);
	void set3DDist(float min, float max);
	void setMode(FMOD_MODE mode);
	
	// Update
	void update();
	
	// Clean up
	void unload();

	// Array of channels
	std::vector<Channel*> chList;
private:
	// Sound stored from file
	FMOD::Sound *sound;

	// Sound System
	static FMOD_RESULT result;
	static SoundE sSys;
};