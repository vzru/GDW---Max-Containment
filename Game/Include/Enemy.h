#pragma once

#include "Object.h"

class Enemy : public Object {
public:
	Enemy(glm::vec3 pos = { 0.f, 0.f, 0.f });
	~Enemy();

	virtual void update(float dt, Level* level);
	float cooldown;
	float health = 10.f;

	bool triggered = false;
	float attackSpeed = 0.5f; // time between attacks (in seconds)
	float movementSpeed; // amount to divide dt by
	float damage; // damage dealt to the player per attack
private:
};