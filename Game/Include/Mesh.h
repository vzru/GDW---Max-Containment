#pragma once

#include <string>
#include <GL\glew.h>
#include <VertexBufferObject.h>

class Mesh {
public:
	Mesh();
	~Mesh();

	// Load a mesh and send it to OpenGL
	bool load(const std::string &file);
	// Release data from OpenGL (VRAM)
	void unload();

	unsigned int getNumFaces();
	unsigned int getNumVertices();

	VertexBufferObject vbo;
	
	void draw();

protected:
	unsigned int numFaces = 0;
	unsigned int numVertices = 0;
private:
};