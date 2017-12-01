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
	rotation.y = origin->getRotation().y;
}
Bullet::~Bullet() {}

void Bullet::update(float dt) {
	life -= dt;

	Object::update(dt);
}