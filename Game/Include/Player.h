#pragma once

#include <vector>

#include "Object.h"

class Bullet;

class Player : public Object {
public:
	Player(glm::vec3 pos = { 0.0f, 0.0f, 0.0f });
	~Player();

	virtual void update(float dt, Level* level);
	virtual void draw(Shader* shader, Camera* camera, std::vector<Light*> light, int a, float lightcount = 0.0f);
	void reset(glm::vec3 pos);
	void level2(glm::vec3 pos);
	bool fire();
	void reloading();

	float ammo, ammoDepo, reloadCd;
	bool firing = false;
	bool reloaded = false;
	bool reload = false;
	bool play = false;
	float popCD;
	float nightCD;

	bool newShot = false;

	glm::vec3 acceleration = { 0.f, 0.f, 0.f };
	Bullet* bullet;
	std::vector<Bullet*> bullets;
private:
	const float RateOfFire = 2.f; // time between shots (in seconds?)
};
