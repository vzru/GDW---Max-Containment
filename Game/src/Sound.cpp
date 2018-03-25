// Victor Zhang 100421055 Mar. 17, 2018

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
	// Create and load sound when constructer is called
	loadSound(f, l, d);
}


Sound::~Sound()
{
	unload();
}

void Sound::loadSound(char * filename, bool loop, int dimension)
{
	// 2D vs 3D checker
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

	// Default roll off
	sound->setMode(FMOD_3D_LINEARROLLOFF);

	// Playback Loop checker
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

void Sound::createChannel(int mode, bool pause)
{
	// Temp channel holder
	Channel* ch = new Channel;

	// Create the sound channel
	result = sSys.getSystem()->playSound(sound, 0, true, &ch->channel);
	sSys.fmodErrorCK(result);

	// Check for 3D attribute
	if (mode == 3)
	{
		result = ch->channel->set3DAttributes(&ch->pos, &ch->vel);
		sSys.fmodErrorCK(result);
	}

	// Setting paused mode
	result = ch->channel->setPaused(pause);
	sSys.fmodErrorCK(result);

	// Add into array of channels for the specific sound
	chList.push_back(ch);
}

void Sound::createChannel(int mode, bool pause, FMOD_VECTOR p, FMOD_VECTOR v)
{
	// Temp channel holder
	Channel* ch = new Channel;

	// Create the sound channel
	result = sSys.getSystem()->playSound(sound, 0, pause, &ch->channel);
	sSys.fmodErrorCK(result);
	
	std::cout << p.x << '/' << p.z << std::endl;

	// Setting new attributes
	ch->pos = p;
	ch->vel = v;
	if (mode == 3)
	{
		result = ch->channel->set3DAttributes(&ch->pos, &ch->vel);
		sSys.fmodErrorCK(result);
	}
	result = ch->channel->setPaused(pause);
	sSys.fmodErrorCK(result);

	// Add into array of channels for the specific sound
	chList.push_back(ch);
}

void Sound::pauseSound(int index)
{
	// Flip paused state
	chList[index]->channel->setPaused(!chList[index]->pause);
	chList[index]->pause = !chList[index]->pause;
}

void Sound::setPause(bool p)
{
	for (int i = 0; i < chList.size(); i++)
	{
		chList[i]->channel->setPaused(p);
	}
}

void Sound::setPause(int index, bool p)
{
	// Set paused or unpaused
	chList[index]->channel->setPaused(p);
}

void Sound::stopSound()
{
	for (int i = 0; i < chList.size(); i++)
	{
		chList[i]->channel->stop();
		chList.pop_back();
	}
}

void Sound::stopSound(int index)
{
	// Delete the channel and sound
	chList[index]->channel->stop();
	chList.pop_back();
}

void Sound::playSound(int mode)
{
	// Create new channel
	Channel* ch = new Channel;
	result = sSys.getSystem()->playSound(sound, 0, true, &ch->channel);
	sSys.fmodErrorCK(result);
	if (mode == 3)
	{
		result = ch->channel->set3DAttributes(&ch->pos, &ch->vel);
		sSys.fmodErrorCK(result);
	}
	result = ch->channel->setPaused(false);
	sSys.fmodErrorCK(result);
	chList.push_back(ch);
}

void Sound::setVolume(float l)
{
	// Change volume levels
	for (int i = 0; i < chList.size(); i++)
	{
		chList[i]->channel->setVolume(l);
	}
}

void Sound::setVolume(int index, float l)
{
	// Change volume levels
	chList[index]->channel->setVolume(l);
}

void Sound::changeSoundLoc(int index, FMOD_VECTOR p)
{
	// Change sound channel location
	chList[index]->pos = p;
	chList[index]->channel->set3DAttributes(&chList[index]->pos, &chList[index]->vel);
}

void Sound::changeSoundVelo(int index, FMOD_VECTOR v)
{
	// Change sound channel velocity
	chList[index]->vel = v;
	chList[index]->channel->set3DAttributes(&chList[index]->pos, &chList[index]->vel);
}

void Sound::changeListenerLoc(FMOD_VECTOR p)
{
	// Change listener location
	sSys.changeListenerLoc(p);
}

void Sound::changeRolloffMode(bool l)
{
	for (int i = 0; i < chList.size(); i++)
	{
		if (l)
		{
			chList[i]->channel->setMode(FMOD_3D_INVERSEROLLOFF);
		}
		else
		{
			chList[i]->channel->setMode(FMOD_3D_LINEARROLLOFF);
		}
	}
}

void Sound::changeRolloffMode(int index, bool l)
{
	// Change roll off mode 
	if (l)
	{
		chList[index]->channel->setMode(FMOD_3D_INVERSEROLLOFF);
	}
	else
	{
		chList[index]->channel->setMode(FMOD_3D_LINEARROLLOFF);
	}
}

void Sound::changeMinMaxDist(float min, float max)
{
	for (int i = 0; i < chList.size(); i++)
	{
		result = chList[i]->channel->set3DMinMaxDistance(min, max);
		sSys.fmodErrorCK(result);
	}
}

void Sound::changeMinMaxDist(int index, float min, float max)
{
	// Change Min and Max Distances for Roll Off
	result = chList[index]->channel->set3DMinMaxDistance(min, max);
	sSys.fmodErrorCK(result);
}

void Sound::set3DDist(float min, float max)
{
	result = sound->set3DMinMaxDistance(min, max);
	sSys.fmodErrorCK(result);
}

void Sound::setMode(FMOD_MODE mode)
{
	result = sound->setMode(mode);
	sSys.fmodErrorCK(result);
}

void Sound::update()
{
	sSys.updateS();
}

void Sound::unload()
{
	// Clean up the channel list
	for (int i = 0; i < chList.size(); i++)
	{
		chList[i]->channel->stop();
		delete chList[i];
	}
	result = sound->release();
	sSys.fmodErrorCK(result);
	sSys.unload();
}
