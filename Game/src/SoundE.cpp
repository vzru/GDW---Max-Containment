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


void SoundE::changeListenerLoc(FMOD_VECTOR pos)
{
	listenerpos = pos;
	//std::cout << "p: " << pos.x << '/' << pos.z << std::endl;

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
	}
	init = false;
}