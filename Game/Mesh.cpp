
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
