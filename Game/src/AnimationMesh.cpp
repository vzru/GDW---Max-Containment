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
	AttributeDescriptor verts1, verts2;
	verts1.attributeLocation = AttributeLocations::VERTEX;
	verts1.elementType = GL_FLOAT;
	verts1.elementSize = sizeof(float);
	verts1.numElementsPerAttrib = 3;
	verts1.numElements = unpackedVertexData.size();
	verts1.data = &unpackedVertexData[0];
	verts1.attributeName = "vIn_vertex";
	verts2 = verts1;
	verts2.numElements = unpackedVertexData2.size();
	verts2.data = &unpackedVertexData2[0];
	verts2.attributeName = "vIn_vertex2";
	vbo.addAttributeArray(verts1);
	vbo.addAttributeArray(verts2);

	AttributeDescriptor uvs1, uvs2;
	uvs1.attributeLocation = AttributeLocations::TEX_COORD;
	uvs1.elementType = GL_FLOAT;
	uvs1.elementSize = sizeof(float);
	uvs1.numElementsPerAttrib = 2;
	uvs1.numElements = unpackedTextureData.size();
	uvs1.data = &unpackedTextureData[0];
	uvs1.attributeName = "vIn_uv";
	uvs2 = uvs1;
	uvs2.numElements = unpackedTextureData2.size();
	uvs2.data = &unpackedTextureData2[0];
	uvs2.attributeName = "vIn_uv2";
	vbo.addAttributeArray(uvs1);
	vbo.addAttributeArray(uvs2);

	AttributeDescriptor norms1, norms2;
	norms1.attributeLocation = AttributeLocations::NORMAL;
	norms1.elementType = GL_FLOAT;
	norms1.elementSize = sizeof(float);
	norms1.numElementsPerAttrib = 3;
	norms1.numElements = unpackedNormalData.size();
	norms1.data = &unpackedNormalData[0];
	norms1.attributeName = "vIn_normal";
	norms2 = norms1;
	norms2.numElements = unpackedNormalData2.size();
	norms2.data = &unpackedNormalData2[0];
	norms2.attributeName = "vIn_normal2";
	vbo.addAttributeArray(norms1);
	vbo.addAttributeArray(norms2);

	vbo.createVBO(GL_STATIC_DRAW);

	return true;
}

void AnimationMesh::unload() {
	vbo.destroy();

	numFaces = 0;
	numVertices = 0;
	numFaces2 = 0;
	numVertices2 = 0;
}

void AnimationMesh::draw() {
	vbo.draw();
}

unsigned int AnimationMesh::getNumFaces() { return numFaces; }
unsigned int AnimationMesh::getNumVertices() { return numVertices; }