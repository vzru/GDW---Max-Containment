#pragma once

#include "Object.h"

class Enemy : public Object {
public:
	Enemy(glm::vec3 pos = { 0.0f,0.0f,0.0f });

	virtual void update(float dt);

	glm::vec3 velocity;
	bool triggered = false;
private:
};