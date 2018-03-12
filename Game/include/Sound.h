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
	Sound* loadSound(char * filename, bool loop, int dimension);
	Sound* createChannel(int mode);
	Sound* stopSound();
	Sound* playSound(int mode);
	Sound* setVolume(float l);
	Sound* changeSoundLoc(FMOD_VECTOR pos);
	Sound* update();
	Sound* unload();
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