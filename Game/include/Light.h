#pragma once

#include <glm\glm.hpp>


class Light {
public:
	Light();
	~Light();

	unsigned int type; // point, directional, spot

	glm::vec4 position;
	glm::vec4 direction;
	glm::vec4 original;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float specExponent;
	float spotExponent;
	float cutoff; // angle
	float innerCutoff; // inner angle
	float partial; // percentage of point light

	glm::vec3 attenuation; // constant, linear, quadratic
};
