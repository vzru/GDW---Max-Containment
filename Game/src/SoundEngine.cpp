#include "SoundEngine.h"
using namespace std;

void FmodErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << "Fmod Error: " << FMOD_ErrorString(result) << endl;
		//system("pause");
	}
}

SoundEngine::SoundEngine()
{
	init = false;
}

SoundEngine::~SoundEngine()
{
	CleanUp();
}
void SoundEngine::CleanUp()
{
	result = system->close();
	FmodErrorCheck(result);
	result = system->release();
	FmodErrorCheck(result);

	init = false;
}
bool SoundEngine::Init()
{
	if (!init)
	{
		init = true;

		result = FMOD::System_Create(&system);
		FmodErrorCheck(result);
		if (result != FMOD_OK) { init = false; }

		result = system->getVersion(&version);
		FmodErrorCheck(result);
		if (result != FMOD_OK) { init = false; }

		result = system->init(100, FMOD_INIT_NORMAL, extradriverdata);
		FmodErrorCheck(result);
		if (result != FMOD_OK) { init = false; }

		//Set the distance units. (meters/feet etc).

		result = system->set3DSettings(1.0, 1.0, 1.0f);
		FmodErrorCheck(result);
	}
	return init;
}