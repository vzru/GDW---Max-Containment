#pragma once

#include "Object.h"

class Object;

class Bullet : public Object {
public:
	Bullet(Object* origin);
	~Bullet();

	virtual Bullet* update(float dt);
	//virtual void draw(Shader *shader, Camera *camera, std::vector<Light> lights);
private:
};
