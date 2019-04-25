#include "Car.h"
#include <freeglut.h>
#include "Model.h"

Car::Car(const std::string fileName){
	carro = new Model(fileName);
	camHorizontalAngle = 0.f;
	camVerticalAngle = 15.f;
	tailWiggleDirectionLeft = true;
	legsAngle = 0.0f;
	legsMovementDirectionForward = true;
	nextMove = nullptr;
	isMoving = false;
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
	updateConstantMovement();

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

void Car::updateConstantMovement() {
	

	if (isMoving) {
		if (legsAngle > 20 || legsAngle < -20)
		{
			legsMovementDirectionForward = !legsMovementDirectionForward;
		}
		if (legsMovementDirectionForward)
		{
			legsAngle += 6.0;
		}
		else {
			legsAngle -= 6.0;
		}
		isMoving = false;
	}
}