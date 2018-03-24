#pragma once

#include "SoundE.h"

//struct Channel
//{
//	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
//	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
//};

class Sound
{
public:
	Sound();
	Sound(char * filename, bool loop, int dimension);
	~Sound();
	//void fmodErrorCK(FMOD_RESULT result);
	void loadSound(char * filename, bool loop, int dimension);
	void createChannel(int mode);
	void stopSound();
	void playSound(int mode);
	void setVolume(float l);
	void changeSoundLoc(FMOD_VECTOR pos);
	void update();
	void unload();
private:
	FMOD::Sound *sound;

	FMOD::Channel *channel = 0;
	std::vector<FMOD::Channel*> channelList;
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	//FMOD_BOOL* isPlaying;

	static FMOD_RESULT result;
	static SoundE sSys;
};