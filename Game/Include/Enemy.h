#pragma once

#include "Object.h"

class Enemy : public Object {
public:
	Enemy(glm::vec3 pos = { 0.f, 0.f, 0.f });
	~Enemy();

	virtual Enemy* update(float dt, Level* level);

	bool triggered = false;
	float attackSpeed = 0.5f; // time between attacks (in seconds)
	float moveSpeed; // amount to divide dt by
	float damage; // damage dealt to the player per attack
	float knockbackCD = 0.0f;
private:
};