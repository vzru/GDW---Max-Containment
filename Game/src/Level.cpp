#include "Level.h"

#include <fstream>
#include <iostream>
#include <glm/vec3.hpp>

Level::Level(float r) : radius(r) {}
Level::Level() {}
Level::~Level() {}

bool Level::load(const std::string & file) {
	std::cout << "Loading level: " << file << std::endl;
	std::ifstream input;
	input.open(file);
	if (!input) {
		std::cout << "Level.loadFromFile: Could not open the file " << file << std::endl;
		return false;
	}

	std::string inputString;

	std::vector<glm::vec3> vertexData;
	
	// Index
	struct quad { unsigned int p[4]; };
	std::vector<quad> faceData;

	while (getline(input, inputString)) {
		if (inputString[0] == '#') {
			std::cout << "Line is comment, skipping: " << inputString << std::endl;
			// this line is a comment so skip
			continue;
		} else if (inputString[0] == 'v') {
			if (inputString[1] == ' ') {
				// this is vertex data
				glm::vec3 temp;
				sscanf_s(inputString.c_str(), "v %f %f %f", &temp.x, &temp.y, &temp.z);
				vertexData.push_back(temp);
			}
		} else if (inputString[0] == 'f') {
			if (inputString[1] == ' ') {
				// this is face data
				quad temp;
				sscanf_s(inputString.c_str(), "f %u %u %u %u",
					&temp.p[0], &temp.p[1], &temp.p[2], &temp.p[3]);
				faceData.push_back(temp);
			}
		} else {
			std::cout << "Line not recognized, skipping: " << inputString << std::endl;
			continue;
		}
	}

	input.close();

	// unpack data
	for (auto& face : faceData) {
		Plane temp;
		glm::vec3 vertex = vertexData[face.p[0] - 1];
		temp.min = { vertex.x - radius, vertex.z - radius };
		temp.max = { vertex.x + radius, vertex.z + radius };
		for (unsigned int j = 0; j < 4; j++) {
			vertex = vertexData[face.p[j] - 1];
			if (temp.min.x > vertex.x - radius)
				temp.min.x = vertex.x - radius;
			if (temp.min.y > vertex.z - radius)
				temp.min.y = vertex.z - radius;
			if (temp.max.x < vertex.x + radius)
				temp.max.x = vertex.x + radius;
			if (temp.max.y < vertex.z + radius)
				temp.max.y = vertex.z + radius;
		}
		//temp.min.x += 2;
		//temp.max.x += 2;
		//std::cout << temp.min.x << "," << temp.min.y << " : " << temp.max.x << "," << temp.max.y << std::endl;
		hitBoxes.push_back(temp);
	}

	numFaces = faceData.size();
	numVertices = numFaces * 4;
	return true;
}