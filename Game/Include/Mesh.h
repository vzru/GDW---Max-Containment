#pragma once

#include <string>
#include <GL\glew.h>

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

protected:
	unsigned int numFaces = 0;
	unsigned int numVertices = 0;
};

typedef struct
{
	int vertex1, texture1, normal1;
	int vertex2, texture2, normal2;
	int vertex3, texture3, normal3;
}Face3;

class OBJMesh : public Mesh
{
public:
	void loadMesh(std::string filename);
	void updateMesh(bool updateVertices, bool updateNormals, bool updateUVs, bool calculateNormals);

	// Containers for OBJ data
	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec3> objUVs;
	std::vector<glm::vec3> objNormals;
	std::vector<Face3> objFaces;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec3> normals;
	//std::vector<float> objVertWeights;
	//std::vector<int> objVertJointCount;
};