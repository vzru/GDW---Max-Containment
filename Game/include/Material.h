#pragma once

#include <glm\glm.hpp>
#include <string>

#include "Type.h"

class Texture;
class Shader;

class Material {
public:
	Material();
	//Material(std::string diffusePath, std::string specularPath, std::string normalPath);
	Material(Texture *diffuse, Texture *specular = nullptr, Texture *normal = nullptr);
	~Material();

	// Methods
	//void load(Type::Texture type, std::string texFile);
	void load(Type::Texture type, Texture *texPtr);
	void setData(float specExp = 50.f, glm::vec3 &hue = glm::vec3(1.0f));
	void bind();
	void unbind();
	void sendUniforms(Shader* shader, std::string prefix);

	// Data members
	Texture *diffuse, *specular, *normal;
	glm::vec3 hue = glm::vec3(1.f);
	float specExponent = 50.f;
};
