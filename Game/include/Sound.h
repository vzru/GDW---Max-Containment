#pragma once

#include "SoundE.h"

class Sound
{
public:
	Sound();
	Sound(char * filename, bool loop);
	~Sound();
	//void fmodErrorCK(FMOD_RESULT result);
	void loadSound(char * filename, bool loop);
	void createChannel();
	void stopSound();
	void playSound();
	void setVolume(float l);
	void changeSoundLoc(FMOD_VECTOR pos);
	void update();
	void unload();
private:
	FMOD::Sound *sound;

	FMOD::Channel *channel = 0;
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	//FMOD_BOOL* isPlaying;

	static FMOD_RESULT result;
	static SoundE sSys;
};