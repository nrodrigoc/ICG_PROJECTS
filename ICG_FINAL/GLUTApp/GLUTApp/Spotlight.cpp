#include "SpotLight.h"

using namespace std;

GLfloat target[3] = { 0.0f, 0.0f, 0.0f };
GLfloat color[3] = { 1.0f, 1.0f, 1.0f };
GLfloat cutoff(5.0f);
GLfloat exponent(15.0f);

SpotLight::SpotLight(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {

	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = w;

	direction[0] = target[0] - position[0];
	direction[1] = target[1] - position[1];
	direction[2] = (target[2] - position[2]);


}


void SpotLight::addlight() {

	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, color);
	glLightfv(GL_LIGHT1, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);

}

void SpotLight::draw() {
	if (!glIsEnabled(GL_LIGHT1)) 
		return;

	glPushMatrix();
	GLfloat up[3] = { 0, 1, 0 };
	lookAt(position, target, up);

	GLfloat ambient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat diffuse[4] = { 0.01f, 0.01f, 0.01f, 1.0f };
	GLfloat specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat shininess = 32.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glutSolidCone(0.3, 0.6, 10, 10);
	glPushMatrix();
	glTranslatef(0, 0, 0.1f);
	glutSolidCylinder(0.2, 0.39, 10, 10);
	glPopMatrix();


	glDisable(GL_LIGHTING);
	glColor3fv(color);
	glutSolidSphere(0.2, 100, 100);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void SpotLight::normalize(const GLfloat* vec, GLfloat* output)
{
	GLfloat length = sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	output[0] /= length;
	output[1] /= length;
	output[2] /= length;
}

void SpotLight::cross(const GLfloat* vec1, const GLfloat* vec2, GLfloat * output) {
	output[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	output[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	output[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

void SpotLight::lookAt(GLfloat* eye, GLfloat* center, GLfloat* up)
{
	GLfloat f[3] = { center[0] - eye[0],
				     center[1] - eye[1],
					 center[2] - eye[2] };

	normalize(f, f);
	GLfloat u[3];
	normalize(up, u);
	GLfloat s[3];
	cross(f, u, s);
	normalize(s, s);
	cross(s, f, u);
	normalize(u, u);

}
