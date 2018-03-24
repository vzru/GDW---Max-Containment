#pragma once

#include "Object.h"

class Object;

class Bullet : public Object {
public:
	Bullet();
	Bullet(Object* origin);
	~Bullet();

	Bullet* reset();

	virtual Bullet* update(float dt = 0.f, glm::vec3 offset = glm::vec3(0.f));
	//virtual void draw(Shader *shader, Camera *camera, std::vector<Light> lights);

	const float MaxLife = 0.05f;
private:
};
