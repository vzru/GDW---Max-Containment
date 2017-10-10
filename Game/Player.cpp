#include <iostream>

#include "Player.h"

Player::Player(glm::vec3 pos) : Object(pos) {}

void Player::update(float dt) {
	position += velocity * (dt / 50);
	if (cooldown >= 0.0f)
		cooldown -= dt;

	Object::update(dt);
}

bool Player::fire() {
	if (cooldown <= 0.0f) {
		makeBullet();
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

void Player::makeBullet() {

}