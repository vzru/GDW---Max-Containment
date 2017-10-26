#include "Bullet.h"
#include "Graphics.h"

Bullet::Bullet(glm::vec2 _start, float a)
	: start(_start), length({ cos(a), sin(a) }) {
	length *= 20;
}
Bullet::Bullet(glm::vec2 _start, glm::vec2 end)
	: start(_start), length(end) {
}
Bullet::~Bullet() {}

void Bullet::draw() {
	Graphics::drawVector({ start.x,0.0f,start.y }, { length.x,0.0f,length.y }, 3.0f, { 0.5f, 0.5f, 0.5f, 0.5f });
}
bool Bullet::update(float dt) {
	life -= dt;
	return life <= 0.0f;
}