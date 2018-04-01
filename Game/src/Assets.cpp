#include "Assets.h"
#include "Mesh.h"
#include "AnimationMesh.h"
#include "Material.h"
#include "Sound.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>

Assets *Assets::assets = nullptr;

Assets *Assets::getInstance() {
	if (assets == nullptr)
		assets = new Assets();
	return assets;
}
Assets::~Assets() {
	for (auto& mesh : meshes) {
		delete mesh.second;
		meshes.erase(mesh.first);
	}
	for (auto& texture : textures) {
		delete texture.second;
		textures.erase(texture.first);
	}
}

Mesh* Assets::loadMesh(const std::string &name, const std::string &file) {
	meshes[name] = new Mesh();
	assert(meshes[name]->load(MESH_PATH(file)));
	return meshes[name];
}

void Assets::loadAnimation(const std::string &name, const std::string &file, const std::string &extension, const int num) {
	for (int i = 0; i < num; i++)
		loadAnimationMesh(name + std::to_string(i), file + std::to_string(i) + "." + extension, file + std::to_string((i + 1) % num) + "." + extension);
}

AnimationMesh* Assets::loadAnimationMesh(const std::string &name, const std::string &file1, const std::string &file2) {
	aMeshes[name] = new AnimationMesh();
	assert(aMeshes[name]->load(MESH_PATH(file1), MESH_PATH(file2)));
	return aMeshes[name];
}

Texture* Assets::loadTexture(const std::string &name, const std::string &file) {
	textures[name] = new Texture();
	assert(textures[name]->load(TEXTURE_PATH(file)));
	return textures[name];
}

Sound* Assets::loadSound(const std::string &name, const std::string &file, bool loop, int dim) {
	sounds[name] = new Sound(SOUND_PATH(file), loop, dim);
	return sounds[name];
}

Shader* Assets::loadShader(const std::string &name, const std::string &vertFile, const std::string & fragFile) {
	shaders[name] = new Shader();
	assert(shaders[name]->load(SHADER_PATH(vertFile), SHADER_PATH(fragFile)));
	return shaders[name];
}

Shader* Assets::loadShader(const std::string &name, const std::string &vertFile, const std::string &geoFile, const std::string &fragFile) {
	shaders[name] = new Shader();
	assert(shaders[name]->load(SHADER_PATH(vertFile), SHADER_PATH(geoFile), SHADER_PATH(fragFile)));
	return shaders[name];
}