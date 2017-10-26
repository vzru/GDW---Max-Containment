#include <iostream>

#include "Mesh.h"
#include "Player.h"
#include "Bullet.h"

Player::Player(glm::vec3 pos) : Object(pos) {
	mesh = std::make_shared<Mesh>();
	mesh->load("assets/models/player.obj");
}
Player::~Player() {
	mesh->unload();
}

void Player::update(float dt) {
	position += velocity * (dt / 50);
	if (cooldown >= 0.0f)
		cooldown -= dt;
	if (firing)
		fire();
	for (int i = 0; i < bullets.size(); i++)
		if (bullets[i]->update(dt)) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
		}

	Object::update(dt);
}

void Player::draw(Shader &shader) {
	for (auto bullet : bullets) {
		bullet->draw();
	}

	Object::draw(shader);
}

bool Player::fire(glm::vec2 mouse) {
	if (cooldown <= 0.0f) {
		makeBullet(mouse);
		cooldown = RateOfFire;
		return true;
	} return false;
	//if (bullets & (1 << 32)) {
		//bullets -= 1;
	//} else {
		//std::cout << "Tried to fire when there were no bullets, reloading." << std::endl;
		//bullets -= 1;
		//return false;
	//}
}
bool Player::fire() {
	if (cooldown <= 0.0f) {
		makeBullet();
		cooldown = RateOfFire;
		return true;
	} return false;
}

void Player::makeBullet(glm::vec2 pos) {
	//bullets.push_back(new Bullet({ position.x, position.z }, glm::degrees(rotation.y)));
	bullets.push_back(new Bullet({ position.x, position.z }, pos));
}
void Player::makeBullet() {
	bullets.push_back(new Bullet({ position.x, position.z }, glm::radians(-rotation.y)));
}