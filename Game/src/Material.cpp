#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include <iostream>

Material::Material() {
	//diffuse = new Texture();
	//specular = new Texture();
	//normal = new Texture();
}

//Material::Material(std::string diffusePath, std::string specularPath, std::string normalPath) {
//	load(Type::Texture::DIFFUSE, diffusePath);
//	load(Type::Texture::SPECULAR, specularPath);
//	load(Type::Texture::NORMAL, normalPath);
//}
Material::Material(Texture *diffuse, Texture *specular, Texture *normal) {
	load(Type::Texture::DIFFUSE, diffuse);
	load(Type::Texture::SPECULAR, specular);
	load(Type::Texture::NORMAL, normal);
}

Material::~Material() {
	//delete diffuse;
	//delete specular;
	//delete normal;
}

//void Material::load(Type::Texture type, std::string texFile) {
//	switch (type) {
//	case Type::Texture::DIFFUSE:
//		if (!diffuse->load(texFile)) {
//			system("Pause");
//			exit(0);
//		} break;
//	case Type::Texture::SPECULAR:
//		if (!specular->load(texFile)) {
//			system("Pause");
//			exit(0);
//		} break;
//	case Type::Texture::NORMAL:
//		if (!normal->load(texFile)) {
//			system("Pause");
//			exit(0);
//		} break;
//	default: {
//			std::cout << "Error - TextureType not found: " << type << std::endl;
//			system("Pause");
//			exit(0);
//		} break;
//	}
//}

void Material::load(Type::Texture type, Texture *texture) {
	switch (type) {
	case Type::Texture::DIFFUSE:
		diffuse = texture;
		break;
	case Type::Texture::SPECULAR:
		specular = texture;
		break;
	case Type::Texture::NORMAL:
		normal = texture;
		break;
	default:
		std::cout << "Error - TextureType not found: " << type << std::endl;
		system("Pause");
		exit(0);
		break;
	}
}

void Material::setData(float specExp, glm::vec3 &color) {
	specExponent = specExp;
	hue = color;
}

void Material::bind() {
	if (diffuse)
		diffuse->bind(GL_TEXTURE0);
	if (specular)
		specular->bind(GL_TEXTURE1);
	if (normal)
		normal->bind(GL_TEXTURE2);
}

void Material::unbind() {
	if (normal)
		normal->unbind(GL_TEXTURE0);
	if (specular)
		specular->unbind(GL_TEXTURE1);
	if (diffuse)
		diffuse->unbind(GL_TEXTURE2);
}

void Material::sendUniforms(Shader* shader, std::string prefix) {
	shader->sendUniform(prefix + "diffuse", 0)
		->sendUniform(prefix + "specular", 1)
		->sendUniform(prefix + "normal", 2)
		->sendUniform(prefix + "hue", hue)
		->sendUniform(prefix + "specExponent", specExponent);
}