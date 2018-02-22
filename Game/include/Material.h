#pragma once

#include <glm\glm.hpp>
#include "Texture.h"
#include "Type.h"

class Material {
public:
	Material();
	Material(std::string diffusePath, std::string specularPath, std::string normalPath, float specExp = 50.f,
		glm::vec3 &hue = glm::vec3(1.f));
	~Material();

	// loaders
	void load(Type::Texture type, std::string texFile);
	void load(Type::Texture type, Texture texture);

	// Data members
	Texture *diffuse, *specular, *normal;
	glm::vec3 hue = glm::vec3(1.f);
	float specExponent = 50.f;
};
