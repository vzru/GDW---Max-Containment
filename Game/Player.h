#pragma once

#include "Object.h"

enum Move {
	Up = 0x1, Down = 0x2, Left = 0x4, Right = 0x8
};

class Player : public Object {
public:
	unsigned short movement;
	virtual void update(float dt);
};