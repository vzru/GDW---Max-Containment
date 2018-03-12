#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "glm\glm.hpp"

#include "Mesh.h"

#define BUFFER_OFFSET(i) ((char*)0 + (i))

struct Face {
	Face(unsigned int v1 = 0, unsigned int v2 = 0, unsigned int v3 = 0,
		unsigned int t1 = 0, unsigned int t2 = 0, unsigned int t3 = 0,
		unsigned int n1 = 0, unsigned int n2 = 0, unsigned int n3 = 0)
		: vertices{ v1,v2,v3 }, textureUVs{ t1, t2, t3 }, normals{ n1, n2, n3 } {}
	unsigned int vertices[3];
	unsigned int textureUVs[3];
	unsigned int normals[3];
};


Mesh::Mesh() {}
Mesh::~Mesh() {
	unload();
}

bool Mesh::load(const std::string &file) {
	std::cout << "Loading mesh: " << file << std::endl;
	std::ifstream input;
	input.open(file);
	if (!input) {
		std::cout << "Mesh.load: Could not open the file " << file << std::endl;
		return false;
	}
	std::string inputString;

	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec2> textureData;
	std::vector<glm::vec3> normalData;

	// Index
	std::vector<Face> faceData;

	// OpenGL ready data
	std::vector<float> unpackedVertexData;
	std::vector<float> unpackedTextureData;
	std::vector<float> unpackedNormalData;

	while (getline(input, inputString)) {
		if (inputString[0] == '#') {
			std::cout << "Line is comment, skipping: " << inputString << std::endl;
			continue;
		}
		else if (inputString[0] == 'v') {
			if (inputString[1] == ' ') {
				// this is a vertex data
				glm::vec3 temp;
				sscanf_s(inputString.c_str(), "v %f %f %f", &temp.x, &temp.y, &temp.z);
				vertexData.push_back(temp);
			}
			else if (inputString[1] == 't') {
				// this is UV data
				glm::vec2 temp;
				sscanf_s(inputString.c_str(), "vt %f %f", &temp.x, &temp.y);
				textureData.push_back(temp);
			}
			else if (inputString[1] == 'n') {
				// this is normal data
				glm::vec3 temp;
				sscanf_s(inputString.c_str(), "vn %f %f %f", &temp.x, &temp.y, &temp.z);
				normalData.push_back(temp);
			}
		}
		else if (inputString[0] == 'f') {
			// this is face data
			Face temp;
			sscanf_s(inputString.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
				&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
				&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);
			faceData.push_back(temp);
		}
		else {
			std::cout << "Line not recognized, skipping: " << inputString << std::endl;
			continue;
		}
	}
	input.close();

	// unpack data
	for (auto &face : faceData)
		for (unsigned int j = 0; j < 3; j++) {
			unpackedVertexData.push_back(vertexData[face.vertices[j] - 1].x);
			unpackedVertexData.push_back(vertexData[face.vertices[j] - 1].y);
			unpackedVertexData.push_back(vertexData[face.vertices[j] - 1].z);

			unpackedTextureData.push_back(textureData[face.textureUVs[j] - 1].x);
			unpackedTextureData.push_back(textureData[face.textureUVs[j] - 1].y);

			unpackedNormalData.push_back(normalData[face.normals[j] - 1].x);
			unpackedNormalData.push_back(normalData[face.normals[j] - 1].y);
			unpackedNormalData.push_back(normalData[face.normals[j] - 1].z);
		}

	numFaces = faceData.size();
	numVertices = numFaces * 3;

	// Send data to OpenGL
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboVertices);	// Vertices
	glGenBuffers(1, &vboUVs);		// UVs
	glGenBuffers(1, &vboNormals);	// Normals

	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);	// Vertices
	glEnableVertexAttribArray(1);	// UVs
	glEnableVertexAttribArray(2);	// Normals
	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedVertexData.size(), &unpackedVertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
	// UVs
	glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedTextureData.size(), &unpackedTextureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));
	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedNormalData.size(), &unpackedNormalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	return true;
}

void Mesh::unload() {
	glDeleteBuffers(1, &vboVertices);
	glDeleteBuffers(1, &vboUVs);
	glDeleteBuffers(1, &vboNormals);
	glDeleteVertexArrays(1, &vao);

	vboVertices = NULL;
	vboUVs = NULL;
	vboNormals = NULL;
	vao = NULL;

	numFaces = 0;
	numVertices = 0;
}

unsigned int Mesh::getNumFaces() { return numFaces; }
unsigned int Mesh::getNumVertices() { return numVertices; }