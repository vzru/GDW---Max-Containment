/*

#include <vector>
#include "QuadMesh.h"

QuadMesh::QuadMesh() {
	vertices.reserve(4);
	textureCoordinates.reserve(4);

	// Origin of the Quad is in the centre
	vertices.push_back(glm::vec3(-0.5f, -0.5f, 0)); // bottom left
	vertices.push_back(glm::vec3(-0.5f, 0.5f, 0)); // top left
	vertices.push_back(glm::vec3(0.5f, 0.5f, 0)); // top right
	vertices.push_back(glm::vec3(0.5f, -0.5f, 0)); // bottom right

	// Origin of the Quad is inthe bottom left
	//vertices.push_back(glm::vec3(0, 0, 0)); // bottom left
	//vertices.push_back(glm::vec3(0, 1, 0)); // top left
	//vertices.push_back(glm::vec3(1, 1, 0)); // top right
	//vertices.push_back(glm::vec3(1, 0, 0)); // bottom right

	textureCoordinates.push_back(glm::vec3(0, 0, 0)); // bottom left
	textureCoordinates.push_back(glm::vec3(0, 1, 0)); // top left
	textureCoordinates.push_back(glm::vec3(1, 1, 0)); // top right
	textureCoordinates.push_back(glm::vec3(1, 0, 0)); // bottom right

	colours = std::vector<glm::vec4>(4, glm::vec4(0, 0, 0, 1));

	primitiveType = PrimitiveType::Quads;
}

QuadMesh::~QuadMesh() {
}

void QuadMesh::setUVbottomLeft(glm::vec2 uv) {
	textureCoordinates[0] = uv;
}

void QuadMesh::setUVtopLeft(glm::vec2 uv) {
	textureCoordinates[1] = uv;
}

void QuadMesh::setUVtopRight(glm::vec2 uv) {
	textureCoordinates[2] = uv;
}


void QuadMesh::setUVbottomRight(glm::vec2 uv) {
	textureCoordinates[3] = uv;
}


*/