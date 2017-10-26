#pragma once

#include <vector>

#include "Object.h"

class Bullet;

class Player : public Object {
public:
	Player(glm::vec3 pos = { 0.0f,0.0f,0.0f });
	~Player();

	virtual void update(float dt);
	virtual void draw(Shader* shader, Camera* camera);
	bool fire(glm::vec2 mouse);
	bool fire();
	
	glm::vec3 velocity;
	float health;
	bool firing = false;
	//int bullets = 100;

private:
	void makeBullet(glm::vec2 pos);
	void makeBullet();
	std::vector<Bullet*> bullets;

	float cooldown;
	const float RateOfFire = 100.0f; // time between shots (in ms)
};