#pragma once

#include <vector>

#include "Object.h"

struct Bullet {
	glm::vec2 start, end;
};

class Player : public Object {
public:
	Player(glm::vec3 pos = { 0.0f,0.0f,0.0f });

	virtual void update(float dt);
	bool fire();
	
	glm::vec3 velocity;
	float health;
	//int bullets = 100;

	std::vector<Bullet*> bullets;
private:
	void makeBullet();
	float cooldown;
	const float RateOfFire = 1000.0f; // time between shots (in ms)
};