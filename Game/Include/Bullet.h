#pragma once

#include <glm/vec2.hpp>

class Bullet {
public:
	Bullet(glm::vec2 _start, float a);
	Bullet(glm::vec2 _start, glm::vec2 end);
	~Bullet();

	void draw();
	bool update(float dt);

private:
	glm::vec2 start, length;
	float life = 100.0f;
};
