#include <iostream>

#include "Mesh.h"
#include "Player.h"
#include "Bullet.h"
#include "Material.h"
#include "Level.h"

Player::Player(glm::vec3 pos) : Object(pos) {
	reset(pos);
}
Player::~Player() {
	delete bullet;
}

Player* Player::update(float dt, Level* level) {
	// movement
	velocity += acceleration * (dt / 200.f);
	//if (glm::length(velocity) > 4.f)
		//velocity = glm::normalize(velocity) * 4.f;
	collide(dt, level);
	if (glm::length(velocity) > 0.f) {
		position += velocity * (dt / 200.f);
		velocity = glm::normalize(velocity) * glm::length(velocity) * 0.9f;
	}

	newShot = false;
	// gun cooldown
	if (cooldown >= 0.f)
		cooldown -= dt / 100.f;
	// shooting
	if (firing)	fire();
	if (bullet != nullptr && bullet->life > 0.f) {
		bullet->update(dt / 1000.f);
	}

	if (reloadCd > 0)
		reloadCd -= dt / 1000.f;

	reloading();
	
	play = true;
	
	// create transformation matrix
	Object::update(dt);
	return this;
}

Player* Player::draw(Shader* shader, Camera* camera, std::vector<Light*> lights) {
	if (bullet != nullptr && bullet->life > 0.f)
		bullet->draw(shader, camera, lights);

	Object::draw(shader, camera, lights);
	return this;
}

Player* Player::reset(glm::vec3 pos) {
	position = pos;
	velocity = { 0.f, 0.f, 0.f };
	acceleration = { 0.f, 0.f, 0.f };
	cooldown = 0.f;
	life = 16.f;
	ammo = 30.0f;
	ammoDepo = 90.0f;
	reloadCd = 0.0f;
	firing = false;
	return this;
}

bool Player::fire() {
	if (cooldown <= 0.0f && reloadCd <= 0.0f && ammo > 0.0f) {
		ammo--;
		reloaded = false;
		//std::cout << ammo << std::endl;
		bullet->reset()
			->setPosition(position)
			->setRotation(rotation);
		newShot = true;
		cooldown = RateOfFire;
		return true;
	} return false;
}

void Player::reloading() {
	//std::cout << ammo << '/' << ammoDepo << '/' << reload << std::endl;
	if (reload == true || (ammo <= 0 && ammoDepo > 0.0f)) {
		float diff = 30.f - ammo;
		if (ammoDepo > 30.0f) {
			ammo += diff;
			ammoDepo -= diff;
		} else if (ammoDepo > 0.0f) {
			ammo = ammoDepo;
			ammoDepo = 0.0f;
		}
		reloadCd = 2.0f;
		reload = false;
		std::cout << "Reload!" << std::endl;
	}
}
