#pragma once

#include "Mesh.h"

class QuadMesh : public Mesh {
public:
	QuadMesh();
	~QuadMesh();

	void setUVbottomLeft(glm::vec2 uv);
	void setUVtopLeft(glm::vec2 uv);
	void setUVbottomRight(glm::vec2 uv);
	void setUVtopRight(glm::vec2 uv);
};