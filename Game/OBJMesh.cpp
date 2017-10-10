#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <string.h>

#include "glm/glm.hpp"

#include "OBJMesh.h"

struct Face3 {
	int vertex1, texture1, normal1;
	int vertex2, texture2, normal2;
	int vertex3, texture3, normal3;
};

void OBJMesh::loadMesh(std::string filename) {
	std::ifstream file;
	//open file
	file.open(filename.c_str());
	//check if file opened
	if (file.fail()) {
		std::cout << "Error - OBJMesh::loadMesh file: " << filename << " not found.\n";
		return;
	}

	char currentChar;

	glm::vec3 temp;
	Face3 temp2;

	// Containers for OBJ data
	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec3> objUVs;
	std::vector<glm::vec3> objNormals;
	std::vector<Face3> objFaces;

	file.get(currentChar);
	while (!file.eof()) {
		if (currentChar == 'v') {
			file.get(currentChar);
			if (currentChar == ' ') {
				file >> temp.x >> temp.y >> temp.z;
				objVertices.push_back(temp);
			} else if (currentChar == 't') {
				file >> temp.x >> temp.y;
				temp.z = 0.0f;
				objUVs.push_back(temp);
			} else if (currentChar == 'n') {
				file >> temp.x >> temp.y >> temp.z;
				objNormals.push_back(temp);
			}
		} else {
			if (currentChar == 'f') {
				file.get(currentChar);
				if (currentChar == ' ') {
					file >> temp2.vertex1 >> currentChar >> temp2.texture1 >> currentChar >> temp2.normal1;
					file >> temp2.vertex2 >> currentChar >> temp2.texture2 >> currentChar >> temp2.normal2;
					file >> temp2.vertex3 >> currentChar >> temp2.texture3 >> currentChar >> temp2.normal3;
					objFaces.push_back(temp2);
				}
			}
		} file.get(currentChar);
	} file.close();

	// Unpack data
	vertices.reserve(objVertices.size());
	normals.reserve(objNormals.size());
	textureCoordinates.reserve(objUVs.size());

	for (int i = 0; i < objFaces.size(); i++) {
		Face3* face = &objFaces[i];

		vertices.push_back(objVertices[face->vertex1 - 1]);
		vertices.push_back(objVertices[face->vertex2 - 1]);
		vertices.push_back(objVertices[face->vertex3 - 1]);

		normals.push_back(objNormals[face->normal1 - 1]);
		normals.push_back(objNormals[face->normal2 - 1]);
		normals.push_back(objNormals[face->normal3 - 1]);

		textureCoordinates.push_back(objUVs[face->texture1 - 1]);
		textureCoordinates.push_back(objUVs[face->texture2 - 1]);
		textureCoordinates.push_back(objUVs[face->texture3 - 1]);
	}

	//createVBO();
}

