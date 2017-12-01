#pragma once
#include "Mesh.h"
#include <vector>
#include <glm/vec2.hpp>

struct Plane {
	glm::vec2 min, max;
};

class Level : public Mesh {
public:
	Level(float r);
	Level();
	~Level();

	// Load a mesh of hitboxes
	bool load(const std::string &file);
	std::vector<Plane> hitBoxes;
private:
	float radius;
};