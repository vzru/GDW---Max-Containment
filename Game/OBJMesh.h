#pragma once

#include <string>
#include "Mesh.h"

class OBJMesh : public Mesh {
public:
	void loadMesh(std::string filename);
};