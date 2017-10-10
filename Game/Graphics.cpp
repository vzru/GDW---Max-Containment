#include <GL\freeglut.h>

#include "Graphics.h"

void Graphics::drawGrid() {
	glPushMatrix();
	glScalef(0.1f, 0.1f, 0.1f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);

	float tempX = -100, tempY = 100, tempZ = 0;
	//glm::vec3 temp(-100, 100, 0);
	int xLines = 0, Ylines = 0, Zlines = 0;
	//glm::vec3 lines(0, 0, 0);
	for (int i = 0; i <= 40; i++) {
		if (i <= 20) {
			if (i == 10) {
				glLineWidth(50.0);
				glColor3f(0, 0, 1);
			}
			else {
				glLineWidth(0.01);
				glColor3f(0, 0, 0);
			}
			tempY = 100;
			glVertex3f(tempX, tempY, tempZ);
			glVertex3f(tempX, -100, tempZ);
			tempX += 10;
		}
		if (i == 20) {
			tempX = 100;
			tempY = -100;
		}
		if (i >= 20) {
			if (i == 30) {
				glLineWidth(50.0);
				glColor3f(1, 0, 0);
			}
			else {
				glLineWidth(0.01);
				glColor3f(0, 0, 0);
			}
			glVertex3f(tempX, tempY, tempZ);
			glVertex3f(-100, tempY, tempZ);
			tempY += 10;
		}
	}
	glColor3f(0, 1, 0);
	glVertex3f(0, 0.0f, 0.0f);
	glVertex3f(0, 0.0f, -100.0f);
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