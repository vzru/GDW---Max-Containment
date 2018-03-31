#include "Bullet.h"

Bullet::Bullet() {
	reset();
}
Bullet::Bullet(Object* origin) :
	Object(origin->getPosition())
{	
	rotation.y = origin->getRotation().y;
	reset();
}
Bullet::~Bullet() {}

Bullet* Bullet::reset() {
	cooldown = 0.f;
	life = MaxLife;
	color = { 1.0f, 1.0f, 1.0f, 0.3f };
	return this;
}

Bullet* Bullet::update(float dt, glm::vec3 offset) {
	life -= dt;

	Object::update(dt, offset);
	return this;
}