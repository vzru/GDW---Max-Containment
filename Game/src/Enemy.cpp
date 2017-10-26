#include "Mesh.h"
#include "Enemy.h"

Enemy::Enemy(glm::vec3 pos) : Object(pos) {
	//mesh = std::shared_ptr<Mesh>();
	//mesh->loadMesh("assets/models/enemy.obj");
}
Enemy::~Enemy() {}

void Enemy::update(float dt) {
	if (triggered)
		position += velocity * (dt / 100);

	Object::update(dt);
}