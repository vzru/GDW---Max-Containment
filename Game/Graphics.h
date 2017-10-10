#pragma once

#include <glm/glm.hpp>

namespace Graphics {
	void drawVector(glm::vec3 origin, glm::vec3 vectorToDraw, float lineWidth, glm::vec4 colour);
	void drawSphere(glm::mat4 p0, float size, glm::vec4 colour);
	void drawGrid();
}