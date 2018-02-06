#pragma once

#include <vector>

#include "Object.h"

class Bullet;

class Player : public Object {
public:
	Player(glm::vec3 pos = { 0.0f, 0.0f, 0.0f });
	~Player();

	virtual void update(float dt, Level* level);
	virtual void draw(Shader* shader, Camera* camera, std::vector<Light> light);
	void reset();
	bool fire();
	
	float health, cooldown, ammo, reloadCd;
	bool firing = false;

	glm::vec3 acceleration = { 0.f, 0.f, 0.f };
	Bullet* bullet;
	std::vector<Bullet*> bullets;
private:
	const float RateOfFire = 2.f; // time between shots (in seconds?)
};