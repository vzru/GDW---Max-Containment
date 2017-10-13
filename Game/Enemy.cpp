#include "Enemy.h"
#include "OBJMesh.h"

Enemy::Enemy(glm::vec3 pos) : Object(pos) {
	//mesh = std::shared_ptr<OBJMesh>();
	//mesh->loadMesh("assets/enemyModel.obj");
}
Enemy::~Enemy() {}

void Enemy::update(float dt) {
	if (triggered)
		position += velocity * (dt / 100);

	Object::update(dt);
}