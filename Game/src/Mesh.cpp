
#define _CRT_SECURE_NO_WARNINGS

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
		: vertices{ v1,v2,v3 }, textureUVs{ t1, t2, t3 }, normals{ n1, n2, n3 }
	{}

	unsigned int vertices[3];
	unsigned int textureUVs[3];
	unsigned int normals[3];
};

Mesh::Mesh() {}
Mesh::~Mesh() {
	unload();
}

bool Mesh::load(const std::string &file) {
	std::ifstream input;
	input.open(file);
	if (!input) {
		std::cout << "Mesh.loadFromFile: Could not open the file " << file << std::endl;
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
			// this line is a comment so skip
			continue;
		} else if (inputString[0] == 'v') {
			if (inputString[1] == ' ') {
				// this is a vertex data
				glm::vec3 temp;
				sscanf(inputString.c_str(), "v %f %f %f", &temp.x, &temp.y, &temp.z);
				vertexData.push_back(temp);
			} else if (inputString[1] == 't') {
				// this is UV data
				glm::vec2 temp;
				sscanf(inputString.c_str(), "vt %f %f", &temp.x, &temp.y);
				textureData.push_back(temp);
			} else if (inputString[1] == 'n') {
				// this is normal data
				glm::vec3 temp;
				sscanf(inputString.c_str(), "vn %f %f %f", &temp.x, &temp.y, &temp.z);
				normalData.push_back(temp);
			}
		} else if (inputString[0] == 'f') {
			// this is face data
			Face temp;
			sscanf(inputString.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
				&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
				&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);
			faceData.push_back(temp);
		} else {
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

/*
#include <iostream>

#include <GL\freeglut.h>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"

void Mesh::draw(glm::mat4& localToWorldMatrix)
{
	if (vertices.size() == 0) {
		std::cout << "Mesh has no vertices!" << std::endl;
		return;
	}

	bool useColours = colours.size() > 0 ? true : false;
	bool useUVs = textureCoordinates.size() > 0 ? true : false;
	bool useNormals = normals.size() > 0 ? true : false;

	if (useUVs)
		if (textureCoordinates.size() != vertices.size()) {
			std::cout << "Number of texture coordinates does not match number of vertices!" << std::endl;
			return;
		}

	if (useColours)
		if (colours.size() != vertices.size()) {
			std::cout << "Number of vertex colours does not match number of vertices!" << std::endl;
			return;
		}

	if (useNormals)
		if (normals.size() != vertices.size()) {
			std::cout << "Number of vertex normals does not match number of vertices!" << std::endl;
			return;
		}

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 32);

	float dim[] = { 0.5F, 0.5F, 0.5F, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dim);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dim);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dim);

	float zero[] = { 0, 0, 0, 1 };
	float amb[] = { 0.15, 0.15, 0.15, 1 };
	float spec[] = { 0.2, 0.2, 0.2, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, zero);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(localToWorldMatrix));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	if (primitiveType == PrimitiveType::Quads)
		glBegin(GL_QUADS);
	else
		glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < vertices.size(); i++) {
		if (useUVs) glTexCoord2f(textureCoordinates[i].x, textureCoordinates[i].y);
		if (useColours) glColor4fv(&colours[i][0]);
		else glColor4f(0.8, 0.8, 0.8, 1.0);
		if (useNormals) glNormal3fv(&normals[i][0]);

		glVertex3fv(&vertices[i][0]);
	}

	glEnd();

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
}

void Mesh::setAllColours(glm::vec4 c) {
	for (auto& colour : colours)
		colour = c;
}
*/