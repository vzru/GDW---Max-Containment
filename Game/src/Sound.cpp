#include "Sound.h"

Sound* Sound::m_instance = nullptr;

Sound::Sound() {
	if (FMOD::System_Create(&m_system) != FMOD_OK)
		throw;

	int driverCount = 0;
	m_system->getNumDrivers(&driverCount);
	if (driverCount == 0)
		throw;

	m_system->init(32, FMOD_INIT_NORMAL, nullptr);
}

Sound::~Sound() {
	delete m_system;
	delete m_channel;
}

Sound* Sound::getInstance() {
	if (!m_instance)
		m_instance = new Sound();
	return m_instance;
}

void Sound::createSound(std::string dir, std::string name, std::string extension) {
	FMOD::Sound* sound = nullptr;
	std::string path = dir + "/" + name + "." + extension;
	m_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound);
	m_sounds[name] = sound;
}

void Sound::playSound(std::string name, bool loop) {
	if (loop) {
		m_sounds[name]->setMode(FMOD_LOOP_NORMAL);
		m_sounds[name]->setLoopCount(-1);
	}
	else m_sounds[name]->setMode(FMOD_LOOP_OFF);
	m_system->playSound(m_sounds[name], nullptr, false, &m_channel);
}

void Sound::releaseSound(std::string name) {

}

void Sound::update() {
	m_system->update();
}