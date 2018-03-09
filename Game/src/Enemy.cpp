#include "Mesh.h"
#include "Enemy.h"
#include "Material.h"

Enemy::Enemy(glm::vec3 pos) : Object(pos) {
	//mat = new Material();
	//mesh->load("assets/models/enemy.obj");
	life = 10.f;
}
Enemy::~Enemy() {}

void Enemy::update(float dt, Level* level) {
	collide(dt, level, true);

	position += velocity * (dt / movementSpeed);

	// player is invincible
	if (cooldown > 0.f)
		cooldown -= dt / 1000.f;

	Object::update(dt);
}