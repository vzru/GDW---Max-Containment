#include "Material.h"
#include <iostream>

Material::Material()
{
	diffuse = new Texture();
	specular = new Texture();
	normal = new Texture();
}

Material::Material(std::string diffusePath, std::string specularPath, std::string normalPath, float specExp,
	glm::vec3 &hue)	: specularExponent(specExp), hue(hue)
{
	load(Type::Texture::DIFFUSE, diffusePath);
	load(Type::Texture::SPECULAR, specularPath);
	load(Type::Texture::NORMAL, normalPath);
}

Material::~Material()
{
	delete diffuse;
	delete specular;
}

void Material::load(Type::Texture type, std::string texFile) {
	switch (type) {
	case Type::Texture::DIFFUSE:
		if (!diffuse->load(texFile)) {
			system("Pause");
			exit(0);
		} break;
	case Type::Texture::SPECULAR:
		if (!specular->load(texFile)) {
			system("Pause");
			exit(0);
		} break;
	case Type::Texture::NORMAL:
		if (!normal->load(texFile)) {
			system("Pause");
			exit(0);
		} break;
	default: {
			std::cout << "Error - TextureType not found: " << type << std::endl;
			system("Pause");
			exit(0);
		} break;
	}
}

void Material::load(Type::Texture type, Texture texture) {
	switch (type) {
	case Type::Texture::DIFFUSE:
		diffuse = &texture;
		break;
	case Type::Texture::SPECULAR:
		specular = &texture;
		break;
	case Type::Texture::NORMAL:
		normal = &texture;
		break;
	default:
		std::cout << "Error - TextureType not found: " << type << std::endl;
		system("Pause");
		exit(0);
		break;
	}
}