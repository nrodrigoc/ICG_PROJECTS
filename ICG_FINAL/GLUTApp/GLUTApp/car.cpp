#include "Car.h"
#include <freeglut.h>
#include "Model.h"
#include <glm.hpp>

Car::Car(const std::string fileName){
	carro = new Model(fileName);
	camHorizontalAngle = 0.f;
	camVerticalAngle = 15.f;
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

void Car::keyUp(const unsigned char key) {
	switch (key) {

	case 'W':
	case 'w':
		isWPressing = false;
		break;

	case 'S':
	case 's':
		isSPressing = false;
		break;

	case 'A':
	case 'a':
		isAPressing = false;
		break;

	case 'D':
	case 'd':
		isDPressing = false;
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
	glTranslatef(0.0, 1, -5.f);

	glGetFloatv(GL_MODELVIEW_MATRIX, local);
	glLoadMatrixf(viewModelMatrix);
}

void Car::draw() {
	updateCameraHoriMovement();

	glPushMatrix();

	// 设置模板缓冲为可写状态，把较小的面包放入模板缓冲（设为1）
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	glTranslatef(0, 0, 6.0);
	glScalef(5.f, 5.f, 5.f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	carro->renderTheModel();
	glPopMatrix();

}

void Car::updateCameraHoriMovement() {
	double dx = 0;
	double dz = 0;

	if (isWPressing)
		dz += 0.2;
	if (isSPressing)
		dz -= 0.2;
	if (isAPressing)
		dx -= 2;
	if (isDPressing)
		dx += 2;

	if (dz != 0 || dx != 0) {
		//Move o carro
		nextMove = [dz]() {glTranslated(0, 0, dz); };

	}
}