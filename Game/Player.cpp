#include "Player.h"

void Player::update(float dt) {
	if (movement & Up)
		setPosition(getPosition() - glm::vec3(0, 0, 0.5));
	if (movement & Down)
		setPosition(getPosition() + glm::vec3(0, 0, 0.5));
	if (movement & Left)
		setPosition(getPosition() - glm::vec3(0.5, 0, 0));
	if (movement & Right)
		setPosition(getPosition() + glm::vec3(0.5, 0, 0));

	Object::update(dt);
}