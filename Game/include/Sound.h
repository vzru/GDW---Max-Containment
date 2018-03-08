// Victor Zhang 100421055 Feb, 6, 2018
#pragma once

#include "SoundE.h"

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
	Sound(char * filename, bool loop, int dimension);
	~Sound();
	void loadSound(char * filename, bool loop, int dimension);
	void createChannel(int mode);
	void pauseSound(int index);
	void stopSound(int index);
	void playSound();
	void setVolume(int index, float l);
	void changeSoundLoc(int index, FMOD_VECTOR pos);
	void changeListenerLoc(FMOD_VECTOR p);
	void changeRolloffMode(int index, bool l);
	void update();
	void unload();
private:
	FMOD::Sound *sound;
	std::vector<Channel*> chList;

	static FMOD_RESULT result;
	static SoundE sSys;
};