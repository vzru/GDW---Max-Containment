#pragma once
#include <FMOD/fmod.hpp>
#include <string>
#include <unordered_map>>

class Sound {
public:
	static Sound* getInstance();
	~Sound();

	void createSound(std::string dir, std::string name, std::string extension);
	void playSound(std::string, bool = false);
	void releaseSound(std::string name);
	
	void update();
private:
	static Sound* m_instance;
	Sound();

	std::unordered_map<std::string, FMOD::Sound*> m_sounds;
	FMOD::System* m_system;
	FMOD::Channel* m_channel;
};

