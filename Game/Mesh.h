#pragma once
//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
//
// This is the base class for all meshes.
//
// Michael Gharbharan 2015
//
//////////////////////////////////////////////////////////////////////////

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