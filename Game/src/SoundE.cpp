#include "SoundE.h"

SoundE::SoundE()
{
}

SoundE::~SoundE()
{
}

void SoundE::fmodErrorCK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << "Sound Error: " << FMOD_ErrorString(result) << endl;
		system("pause");
	}
}

void SoundE::initializeS()
{

	result = FMOD::System_Create(&sSystem);
	fmodErrorCK(result);

	result = sSystem->getVersion(&version);
	fmodErrorCK(result);

	if (version < FMOD_VERSION)
	{
		cout << "Error Wrong Version" << endl;
	}

	result = sSystem->init(100, FMOD_INIT_NORMAL, extradriverdata);
	fmodErrorCK(result);

	result = sSystem->set3DSettings(1.0f, 1.0f, 1.0f);
	fmodErrorCK(result);

	init = true;
}

void SoundE::updateS()
{
	result = sSystem->update();
	fmodErrorCK(result);
}

FMOD::System* SoundE::getSystem()
{
	return sSystem;
}


//void SoundE::loadSound(char* filename, bool loop)
//{
//	Sound s;
//	result = sSystem->createSound(filename, FMOD_3D, 0, &s.sound);
//	fmodErrorCK(result);
//	result = s.sound->set3DMinMaxDistance(0.5f, 100.0f);
//	fmodErrorCK(result);
//	if (loop)
//	{
//		result = s.sound->setMode(FMOD_LOOP_NORMAL);
//	}
//	else
//	{
//		result = s.sound->setMode(FMOD_DEFAULT);
//	}
//	fmodErrorCK(result);
//	soundL.push_back(s);
//}
//
//void SoundE::createChannel(int soundIndex)
//{
//	FMOD::Channel *channel = 0;
//	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
//	result = sSystem->playSound(soundL[soundIndex], 0, true, &channel);
//	fmodErrorCK(result);
//	result = channel->set3DAttributes(&pos, &vel);
//	fmodErrorCK(result);
//	result = channel->setPaused(false);
//	fmodErrorCK(result);
//	channelL.push_back(channel);
//}

//void SoundE::changeSoundLoc(FMOD_VECTOR pos)
//{
//
//}

void SoundE::changeListenerLoc(FMOD_VECTOR pos)
{
	listenerpos = pos;

	result = sSystem->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
	fmodErrorCK(result);
}

void SoundE::unload()
{
	if (init)
	{
		result = sSystem->close();
		fmodErrorCK(result);
		result = sSystem->release();
		fmodErrorCK(result);
	/*	for (int i = 0; i < soundL.size(); i++)
		{
			result = soundL[i]->sound.release();
			fmodErrorCK(result);
		}*/
	}
	init = false;
}

//void Sound::loadSound(char * filename, bool loop)
//{
//	SoundE::result = sSystem->createSound(filename, FMOD_3D, 0, &sound);
//	fmodErrorCK(result);
//	result = sound->set3DMinMaxDistance(0.5f, 100.0f);
//	fmodErrorCK(result);
//	if (loop)
//	{
//		result = sound->setMode(FMOD_LOOP_NORMAL);
//	}
//	else
//	{
//		result = sound->setMode(FMOD_DEFAULT);
//	}
//	fmodErrorCK(result);
//}
//
//void Sound::createChannel()
//{
//	result = sSystem->playSound(sound, 0, true, &channel);
//	fmodErrorCK(result);
//	result = channel->set3DAttributes(&pos, &vel);
//	fmodErrorCK(result);
//	result = channel->setPaused(false);
//	fmodErrorCK(result);
//}
//
//void Sound::unload()
//{
//	result = sound->release();
//	fmodErrorCK(result);
//}
//
//Channel::Channel(FMOD::Channel *ch, FMOD_VECTOR p, FMOD_VECTOR v) 
//	: channel(ch), pos(p), vel(v)
//{
//}
//
//Sound::Sound(FMOD::Sound * s)
//	: sound(s)
//{
//}
