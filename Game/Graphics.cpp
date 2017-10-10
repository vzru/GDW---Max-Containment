#include <GL\freeglut.h>

#include "Graphics.h"

void Graphics::drawGrid(glm::vec3 size) {
	glPushMatrix();
	glScalef(0.1f, 0.1f, 0.1f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);

	glm::vec3 temp(-size.x, size.y, 0);
	int width = size.x / 10 * 2 + size.y / 10 * 2;
	//glm::vec3 lines(0, 0, 0);
	for (int i = 0; i <= width; i++) {
		if (i <= width / 2) {
			if (i == width / 4) {
				glLineWidth(50.0);
				glColor3f(0, 0, 1);
			} else {
				glLineWidth(0.01);
				glColor3f(0, 0, 0);
			}
			temp.y = size.y;
			glVertex3f(temp.x, temp.y, temp.z);
			glVertex3f(temp.x, -size.y, temp.z);
			temp.x += 10;
		}
		if (i == width / 2) {
			temp.x = size.x;
			temp.y = -size.y;
		}
		if (i >= width / 2) {
			if (i == width / 2 + width / 4) {
				glLineWidth(50.0);
				glColor3f(1, 0, 0);
			} else {
				glLineWidth(0.01);
				glColor3f(0, 0, 0);
			}
			glVertex3f(temp.x, temp.y, temp.z);
			glVertex3f(-size.x, temp.y, temp.z);
			temp.y += 10;
		}
	}
	glColor3f(0, 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -size.z);
	glColor3f(0, 0, 0);
	glEnd();
	glPopMatrix();
}

void Graphics::drawVector(glm::vec3 origin, glm::vec3 vectorToDraw, float lineWidth, glm::vec4 colour) {
	GLboolean lightingEnabled;
	glGetBooleanv(GL_LIGHTING, &lightingEnabled);

	glDisable(GL_LIGHTING);

	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glColor3fv(&colour[0]);
	glVertex3fv(&origin[0]);
	glVertex3f(origin[0] + vectorToDraw[0], origin[1] + vectorToDraw[1], origin[2] + vectorToDraw[2]);
	glEnd();
	glLineWidth(1.0f);

	if (lightingEnabled)
		glEnable(GL_LIGHTING);
}

void Graphics::drawSphere(glm::mat4 p0, float size, glm::vec4 colour) {
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4fv(&colour[0]);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&p0[0][0]);
	glutSolidSphere(size, 4, 4);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}