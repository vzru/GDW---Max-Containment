#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "glm\glm.hpp"

#include "AnimationMesh.h"

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

AnimationMesh::AnimationMesh() {}
AnimationMesh::~AnimationMesh() {
	unload();
}

bool AnimationMesh::load(const std::string &file1, const std::string &file2) {
	std::cout << "Loading mesh: " << file1 << '/' << file2 << std::endl;
	std::ifstream input;
	input.open(file1);
	if (!input) {
		std::cout << "AnimationMesh.load: Could not open the file " << file1 << std::endl;
		return false;
	}

	std::ifstream input2;
	input2.open(file2);
	if (!input2) {
		std::cout << "AnimationMesh.load: Could not open the file " << file2 << std::endl;
		return false;
	}

	std::string inputString;
	std::string inputString2;

	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec2> textureData;
	std::vector<glm::vec3> normalData;

	std::vector<glm::vec3> vertexData2;
	std::vector<glm::vec2> textureData2;
	std::vector<glm::vec3> normalData2;

	// Index
	std::vector<Face> faceData;
	std::vector<Face> faceData2;


	// OpenGL ready data
	std::vector<float> unpackedVertexData;
	std::vector<float> unpackedTextureData;
	std::vector<float> unpackedNormalData;

	std::vector<float> unpackedVertexData2;
	std::vector<float> unpackedTextureData2;
	std::vector<float> unpackedNormalData2;

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

	while (getline(input2, inputString2)) {
		if (inputString2[0] == '#') {
			std::cout << "Line is comment, skipping: " << inputString2 << std::endl;
			continue;
		}
		else if (inputString2[0] == 'v') {
			if (inputString2[1] == ' ') {
				// this is a vertex data
				glm::vec3 temp;
				sscanf_s(inputString2.c_str(), "v %f %f %f", &temp.x, &temp.y, &temp.z);
				vertexData2.push_back(temp);
			}
			else if (inputString2[1] == 't') {
				// this is UV data
				glm::vec2 temp;
				sscanf_s(inputString2.c_str(), "vt %f %f", &temp.x, &temp.y);
				textureData2.push_back(temp);
			}
			else if (inputString2[1] == 'n') {
				// this is normal data
				glm::vec3 temp;
				sscanf_s(inputString2.c_str(), "vn %f %f %f", &temp.x, &temp.y, &temp.z);
				normalData2.push_back(temp);
			}
		}
		else if (inputString2[0] == 'f') {
			// this is face data
			Face temp;
			sscanf_s(inputString2.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
				&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
				&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);
			faceData2.push_back(temp);
		}
		else {
			std::cout << "Line not recognized, skipping: " << inputString2 << std::endl;
			continue;
		}
	}
	input2.close();

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

	for (auto &face : faceData2)
		for (unsigned int j = 0; j < 3; j++) {
			unpackedVertexData2.push_back(vertexData2[face.vertices[j] - 1].x);
			unpackedVertexData2.push_back(vertexData2[face.vertices[j] - 1].y);
			unpackedVertexData2.push_back(vertexData2[face.vertices[j] - 1].z);

			unpackedTextureData2.push_back(textureData2[face.textureUVs[j] - 1].x);
			unpackedTextureData2.push_back(textureData2[face.textureUVs[j] - 1].y);

			unpackedNormalData2.push_back(normalData2[face.normals[j] - 1].x);
			unpackedNormalData2.push_back(normalData2[face.normals[j] - 1].y);
			unpackedNormalData2.push_back(normalData2[face.normals[j] - 1].z);
		}

	numFaces = faceData.size();
	numVertices = numFaces * 3;

	numFaces2 = faceData2.size();
	numVertices2 = numFaces2 * 3;

	// Send data to OpenGL
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboVertices);	// Vertices
	glGenBuffers(1, &vboUVs);		// UVs
	glGenBuffers(1, &vboNormals);	// Normals
	glGenBuffers(1, &vboVertices2);	// Vertices
	glGenBuffers(1, &vboUVs2);		// UVs
	glGenBuffers(1, &vboNormals2);	// Normals

	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);	// Vertices
	glEnableVertexAttribArray(1);	// UVs
	glEnableVertexAttribArray(2);	// Normals

	glEnableVertexAttribArray(3);	// Vertices
	glEnableVertexAttribArray(4);	// UVs
	glEnableVertexAttribArray(5);	// Normals
	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedVertexData.size(), &unpackedVertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedVertexData2.size(), &unpackedVertexData2[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
	// UVs
	glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedTextureData.size(), &unpackedTextureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));
	glBindBuffer(GL_ARRAY_BUFFER, vboUVs2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedTextureData2.size(), &unpackedTextureData2[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)4, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));
	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedNormalData.size(), &unpackedNormalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unpackedNormalData2.size(), &unpackedNormalData2[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)5, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	// Vertices
	// UVs
	// Normals

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	return true;
}

void AnimationMesh::unload() {
	glDeleteBuffers(1, &vboVertices);
	glDeleteBuffers(1, &vboUVs);
	glDeleteBuffers(1, &vboNormals);
	glDeleteBuffers(1, &vboVertices2);
	glDeleteBuffers(1, &vboUVs2);
	glDeleteBuffers(1, &vboNormals2);
	glDeleteVertexArrays(1, &vao);

	vboVertices = NULL;
	vboUVs = NULL;
	vboNormals = NULL;
	vao = NULL;

	numFaces = 0;
	numVertices = 0;
	numFaces2 = 0;
	numVertices2 = 0;
}

unsigned int AnimationMesh::getNumFaces() { return numFaces; }
unsigned int AnimationMesh::getNumVertices() { return numVertices; }