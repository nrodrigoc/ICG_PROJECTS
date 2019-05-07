#pragma once
#include <math.h>
#include <GL\freeglut.h>

/*
Spotlight object handling position, target and other lighting properties.
*/
class SpotLight {
public:
	GLfloat position[4];
	GLfloat direction[3];
	

	SpotLight();
	void addlight();
	void draw();
	~SpotLight() = default;
private:
	void normalize(const GLfloat* vec, GLfloat* output);
	void cross(const GLfloat* vec1, const GLfloat* vec2, GLfloat * output);
	void lookAt(GLfloat* eye, GLfloat* center, GLfloat* up);
};
