#pragma once

#include <vector>

#include "Object.h"

class Bullet;

class Player : public Object {
public:
	Player(glm::vec3 pos = { 0.0f, 0.0f, 0.0f });
	~Player();

	virtual Player* update(float dt, Level* level);
	virtual Player* draw(Shader* shader, Camera* camera, std::vector<Light*> light = {});
	Player* reset(glm::vec3 pos);
	bool fire();
	void reloading();

	int ammoDepo;
	float reloadCd;
	bool firing = false;
	bool reloaded = false;
	bool reload = false;
	bool play = false;
	bool newShot = false;

	glm::vec3 acceleration = { 0.f, 0.f, 0.f };
	Bullet* bullet;

	// Constants
	const float MaxLife = 20.f;
private:
	const float RateOfFire = 2.f; // time between shots (in seconds?)
};
