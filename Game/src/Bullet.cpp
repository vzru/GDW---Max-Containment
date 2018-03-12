#include "Bullet.h"

/*
Bullet::Bullet(glm::vec2 _start, float a)
	: start(_start), length({ cos(a), sin(a) }) {
	length *= 20;
}
Bullet::Bullet(glm::vec2 _start, glm::vec2 end)
	: start(_start), length(end) {
}*/

Bullet::Bullet(Object* origin) :
	Object(origin->getPosition())
{
	cooldown = 0.f;
	life = 0.05f;
	rotation.y = origin->getRotation().y;
}
Bullet::~Bullet() {}

Bullet* Bullet::update(float dt) {
	life -= dt;

	Object::update(dt);
	return this;
}