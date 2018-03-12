#include "Sound.h"

FMOD_RESULT Sound::result;
SoundE Sound::sSys;

Sound::Sound()
{
	if (!sSys.init)
	{
		sSys.initializeS();
	}
}

Sound::Sound(char * f, bool l, int d)
{
	if (!sSys.init)
	{
		sSys.initializeS();
	}
	loadSound(f, l, d);
}

Sound::~Sound()
{
	unload();
}

void Sound::loadSound(char * filename, bool loop, int dimension)
{
	if (dimension == 2)
	{
		result = sSys.getSystem()->createSound(filename, FMOD_2D, 0, &sound);
		sSys.fmodErrorCK(result);
	}
	else if (dimension == 3)
	{
		result = sSys.getSystem()->createSound(filename, FMOD_3D, 0, &sound);
		sSys.fmodErrorCK(result);
		result = sound->set3DMinMaxDistance(0.5f, 100.0f);
		sSys.fmodErrorCK(result);
	}
	if (loop)
	{
		result = sound->setMode(FMOD_LOOP_NORMAL);
	}
	else
	{
		result = sound->setMode(FMOD_DEFAULT);
	}
	sSys.fmodErrorCK(result);
}

void Sound::createChannel(int mode)
{
	/*channel = new Channel;*/
	result = sSys.getSystem()->playSound(sound, 0, true, &channel);
	sSys.fmodErrorCK(result);
	if(mode == 3)
	{
		result = channel->set3DAttributes(&pos, &vel);
		sSys.fmodErrorCK(result);
	}
	result = channel->setPaused(false);
	sSys.fmodErrorCK(result);
}

void Sound::stopSound()
{
	channel->stop();
}

void Sound::playSound(int mode)
{
		result = sSys.getSystem()->playSound(sound, 0, true, &channel);
		sSys.fmodErrorCK(result);
		if (mode == 3)
		{
			result = channel->set3DAttributes(&pos, &vel);
			sSys.fmodErrorCK(result);
		}
		result = channel->setPaused(false);
		sSys.fmodErrorCK(result);
}

void Sound::setVolume(float l)
{
	channel->setVolume(l);
}

void Sound::changeSoundLoc(FMOD_VECTOR p)
{
	pos = p;
}

void Sound::update()
{
	sSys.updateS();
}

void Sound::unload()
{
	result = sound->release();
	sSys.fmodErrorCK(result);
	sSys.unload();
}
