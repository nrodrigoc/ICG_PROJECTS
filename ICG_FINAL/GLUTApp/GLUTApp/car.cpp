#include "Car.h"
#include <GL\freeglut.h>
#include "Model.h"
#include <glm.hpp>

Car::Car(const std::string fileName) {
	carro = new Model(fileName);
	camHorizontalAngle = 0.f;
	camVerticalAngle = 15.f;
	moveSpeed = 1;
	carRotate = 90.f;
	isWPressing = isSPressing = isAPressing = isDPressing = false;
	legsAngle = 0.0f;
	legsMovementDirectionForward = true;
	nextMove = nullptr;
	isMoving = false;
}

void Car::keyPressed(const unsigned char key)
{
	switch (key) {
	case 'W':
	case 'w':
		isWPressing = true;
		break;

	case 'S':
	case 's':
		isSPressing = true;
		break;

	case 'A':
	case 'a':
		isAPressing = true;
		break;

	case 'D':
	case 'd':
		isDPressing = true;
		break;
	default:
		break;
	}
}

//Funcao que reconhece quando a tecla parou de ser pressionada
void Car::keyUp(const unsigned char key) {
	switch (key) {

	case 'W':
	case 'w':
		isWPressing = false;
		this->moveSpeed = 2;
		break;

	case 'S':
	case 's':
		isSPressing = false;
		break;

	case 'A':
	case 'a':
		isAPressing = false;
		this->carRotate = 90;
		break;

	case 'D':
	case 'd':
		isDPressing = false;
		this->carRotate = 90;
		break;
	default:
		break;
	}

}

void Car::init() {
	GLfloat viewModelMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
	glLoadIdentity();

	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0, 1.2, -5.f);

	glGetFloatv(GL_MODELVIEW_MATRIX, local);
	glLoadMatrixf(viewModelMatrix);
}

void Car::draw() {
	updateCameraHoriMovement();

	glPushMatrix();

	GLfloat carSpecular[] = { 1.f, 1.f, 1.f };
	GLfloat carShininess = 1.f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, carSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, carShininess);
	glMaterialf(GL_FRONT_AND_BACK, GL_EMISSION, 1);

	glColorMaterial(GL_FRONT, GL_SHININESS);
	glEnable(GL_COLOR_MATERIAL);


	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	glTranslatef(0, 0, 6.0);
	glScalef(5.f, 5.f, 5.f);
	glRotatef(this->carRotate, 0.f, 1.f, 0.f);
	carro->renderTheModel();
	glPopMatrix();

}

void Car::updateCameraHoriMovement() {
	double dx = 0;
	double dz = 0;

	if (isWPressing) {
		dz += 0.25 * moveSpeed;
		(moveSpeed < 2.5f) ? moveSpeed += 0.002f : moveSpeed = 2.5f;
	}
	if (isSPressing)
		dz -= 0.05;
	if (isAPressing) {
		dx += 0.75;
		this->carRotate = 100.f;
	}
	if (isDPressing) {
		dx -= 0.75;
		this->carRotate = 80.f;
	}

	if (dz != 0 && dx == 0) {
		//Move o carro
		nextMove = [dz]() {
			glTranslated(0, 0, dz);
			//glRotatef(dx, 0, 1, 0);
		};
	}
	else if (dz > 0 && dx != 0) {
		//Move o carro
		nextMove = [dz, dx]() {
			glTranslated(0, 0, dz);
			glRotatef(dx, 0, 1, 0);
		};
	}
	else if (dz < 0 && dx != 0) {
		//Move o carro
		nextMove = [dz, dx]() {
			glTranslated(0, 0, dz);
			glRotatef(-dx, 0, 1, 0);
		};
	}

	//
}
