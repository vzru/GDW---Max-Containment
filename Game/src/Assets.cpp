#include "Assets.h"
#include "Mesh.h"
#include "Material.h"
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

Mesh *Assets::loadMesh(const std::string &name, const std::string &file) {
	meshes[name] = new Mesh();
	assert(meshes[name]->load(MESH_PATH(file)));
	return meshes[name];
}

Texture *Assets::loadTexture(const std::string &name, const std::string &file) {
	textures[name] = new Texture();
	assert(textures[name]->load(TEXTURE_PATH(file)));
	return textures[name];
}