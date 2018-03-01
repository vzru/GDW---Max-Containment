#include <iostream>

#include "Mesh.h"
#include "Player.h"
#include "Bullet.h"
#include "Material.h"
#include "Level.h"

Player::Player(glm::vec3 pos) : Object(pos), bullet(new Bullet(this)), health(20.f) {
}
Player::~Player() {
	delete bullet;
	while (bullets.size() > 0) {
		delete bullets[0];
		bullets.erase(bullets.begin());
	}
}

void Player::update(float dt, Level* level) {
	// movement
	velocity += acceleration * (dt / 200.f);
	//if (glm::length(velocity) > 4.f)
		//velocity = glm::normalize(velocity) * 4.f;
	collide(dt, level);
	if (glm::length(velocity) > 0.f)

	position += velocity * (dt / 200.f);
	if (glm::length(velocity) > 0.f)
		velocity = glm::normalize(velocity) * glm::length(velocity) * 0.9f;
	
	// gun cooldown
	if (cooldown >= 0.f)
		cooldown -= dt / 100.f;
	// shooting
	if (firing) fire();
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->update(dt / 1000.f);
		if (bullets[i]->life <= 0.f) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
	
	if(reloadCd > 0)
	reloadCd -= dt/1000.f;

	reloading();
	//std::cout << "Player Position: " << acceleration.x << '/' << acceleration.y << '/' << acceleration.z << std::endl;
	//std::cout << "Player Velocity: " << velocity.x << '/' << velocity.y << '/' << velocity.z << std::endl;

	// create transformation matrix
	Object::update(dt);
}

void Player::draw(Shader* shader, Camera* camera, std::vector<Light> lights) {
	for (auto bullet : bullets) {
		bullet->draw(shader, camera, lights);
	}

	Object::draw(shader, camera, lights);
}

void Player::reset() {
	position = { 4.f, 0.f, 6.f };
	velocity = { 0.f, 0.f, 0.f };
	acceleration = { 0.f, 0.f, 0.f };
	cooldown = 0.f;
	health = 20.f;
	ammo = 30.0f;
	ammoDepo = 90.0f;
	reloadCd = 0.0f;
	firing = false;
}

bool Player::fire() {
	if (cooldown <= 0.0f && reloadCd <= 0.0f && ammo > 0.0f) {
		ammo--;
		reloaded = false;
		//std::cout << ammo << std::endl;
		bullet->setPosition(position);
		bullet->setRotation(rotation);
		bullets.push_back(new Bullet(*bullet));
		cooldown = RateOfFire;
		std::cout << ammo << '/' << ammoDepo << std::endl;
		if (ammo <= 0 && ammoDepo > 0.0f)
		{
			if (ammoDepo > 30.0f)
			{
				ammo = 30.0f;
				ammoDepo -= 30.0f;
			}
			else if (ammoDepo > 0.0f)
			{
				ammo = ammoDepo;
				ammoDepo = 0.0f;
			}
			reloadCd = 2.5f;
			std::cout << "Reload!" << std::endl;
		}
		return true;
	} return false;
}


void Player::reloading()
{
	std::cout << ammo << '/' << ammoDepo << '/' << reload << std::endl;
	if ((ammo <= 0 && ammoDepo > 0.0f) || reload == true)
	{
		float diff = 30.f - ammo;
		if (ammoDepo > 30.0f)
		{
			ammo += diff;
			ammoDepo -= diff;
		}
		else if (ammoDepo > 0.0f)
		{
			ammo = ammoDepo;
			ammoDepo = 0.0f;
		}
		reloadCd = 2.5f;
		reload = false;
		std::cout << "Reload!" << std::endl;
	}
}