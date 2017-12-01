#pragma once

#include <glm\glm.hpp>


class Light {
public:
	Light();
	~Light();

	unsigned int type; // point, directional

	glm::vec4 posDir;
	glm::vec4 originalPosition;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float specularExponent;

	glm::vec3 attenuation; // constant,linear,quadratic
};