// Victor Zhang 100421055 Feb. 6, 2018
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
		result = sound->set3DMinMaxDistance(0.5f, 5.0f);
		sSys.fmodErrorCK(result);
	}
	sound->setMode(FMOD_3D_LINEARROLLOFF);
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
	Channel* ch = new Channel;
	result = sSys.getSystem()->playSound(sound, 0, true, &ch->channel);
	sSys.fmodErrorCK(result);
	if(mode == 3)
	{
	result = ch->channel->set3DAttributes(&ch->pos, &ch->vel);
	sSys.fmodErrorCK(result);
	}
	result = ch->channel->setPaused(pause);
	sSys.fmodErrorCK(result);
	chList.push_back(ch);
}

void Sound::createChannel(int mode, bool pause, FMOD_VECTOR p, FMOD_VECTOR v)
{
	Channel* ch = new Channel;
	result = sSys.getSystem()->playSound(sound, 0, true, &ch->channel);
	sSys.fmodErrorCK(result);
	ch->pos = p;
	ch->vel = v;
	if (mode == 3)
	{
		result = ch->channel->set3DAttributes(&ch->pos, &ch->vel);
		sSys.fmodErrorCK(result);
	}
	result = ch->channel->setPaused(pause);
	sSys.fmodErrorCK(result);
	chList.push_back(ch);
}

void Sound::pauseSound(int index)
{
	chList[index]->channel->setPaused(!chList[index]->pause);
	chList[index]->pause = !chList[index]->pause;
}

void Sound::stopSound(int index)
{
	chList[index]->channel->stop();
}

void Sound::playSound(int mode)
{
	Channel* ch = new Channel;
	result = sSys.getSystem()->playSound(sound, 0, true, &ch->channel);
	sSys.fmodErrorCK(result);
	if(mode == 3)
	{
	result = ch->channel->set3DAttributes(&ch->pos, &ch->vel);
	sSys.fmodErrorCK(result);
	}
	result = ch->channel->setPaused(false);
	sSys.fmodErrorCK(result);
	chList.push_back(ch);
}

void Sound::setVolume(int index, float l)
{
	chList[index]->channel->setVolume(l);
}

void Sound::changeSoundLoc(int index, FMOD_VECTOR p)
{
	chList[index]->pos = p;
	chList[index]->channel->set3DAttributes(&chList[index]->pos, &chList[index]->vel);
}

void Sound::changeListenerLoc(FMOD_VECTOR p)
{
	sSys.changeListenerLoc(p);
}

void Sound::changeRolloffMode(int index, bool l)
{
	if (l)
	{
		chList[index]->channel->setMode(FMOD_3D_INVERSEROLLOFF);
	}
	else
	{
		chList[index]->channel->setMode(FMOD_3D_LINEARROLLOFF);
	}
}

void Sound::update()
{
	sSys.updateS();
}

void Sound::unload()
{
	for (int i = 0; i < chList.size(); i++)
	{
		chList[i]->channel->stop();
		delete chList[i];
	}
	result = sound->release();
	sSys.fmodErrorCK(result);
	sSys.unload();
}
