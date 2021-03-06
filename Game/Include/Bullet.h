#pragma once

#include "Object.h"

class Object;

class Bullet : public Object {
public:
	Bullet(Object* origin);
	~Bullet();

	virtual void update(float dt);
	//virtual void draw(Shader *shader, Camera *camera, std::vector<Light> lights);

	float cooldown = 0.f;
	float life = 0.05f;
private:

};
