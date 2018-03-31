#pragma once

#include <string>
#include <GL\glew.h>
#include "VertexBufferObject.h"

class AnimationMesh {
public:
	AnimationMesh();
	~AnimationMesh();

	// Load a mesh and send it to OpenGL
	bool load(const std::string &file1, const std::string &file2);
	// Release data from OpenGL (VRAM)
	void unload();

	unsigned int getNumFaces();
	unsigned int getNumVertices();

	// OpenGL buffers and objects
	VertexBufferObject vbo;
	
	void draw();

protected:
	unsigned int numFaces = 0;
	unsigned int numVertices = 0;
	unsigned int numFaces2 = 0;
	unsigned int numVertices2 = 0;
private:
};