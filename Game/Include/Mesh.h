#pragma once

#include <vector>
#include "GLM/glm.hpp"

enum PrimitiveType {
	Triangles = 1,
	Quads
};

class Mesh {
public:
	// Binds all three buffers. Uses OpenGL 1.0 draw calls.
	void draw(glm::mat4& localToWorldMatrix);
	
	// Sets all per-vertex colours to the specified colour
	void setAllColours(glm::vec4 colour);
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec3> normals;
	
	PrimitiveType primitiveType;
};