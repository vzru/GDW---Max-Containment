#pragma once

#include <string>
#include <GL\glew.h>

//enum PrimitiveType { Triangles = 1, Quads };

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

	// OpenGL buffers and objects
	GLuint vboVertices = 0;
	GLuint vboUVs = 0;
	GLuint vboNormals = 0;
	GLuint vao = 0;
private:
	unsigned int numFaces = 0;
	unsigned int numVertices = 0;
};
/*
	// Binds all three buffers. Uses OpenGL 1.0 draw calls.
	void draw(glm::mat4& localToWorldMatrix);
	
	// Sets all per-vertex colours to the specified colour
	void setAllColours(glm::vec4 colour);
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec3> normals;
	
	PrimitiveType primitiveType;
*/