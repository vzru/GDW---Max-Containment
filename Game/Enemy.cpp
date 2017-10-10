#include "Enemy.h"

Enemy::Enemy(glm::vec3 pos) : Object(pos) {}

void Enemy::update(float dt) {
	if (triggered)
		position += velocity * (dt / 50);


	Object::update(dt);
}