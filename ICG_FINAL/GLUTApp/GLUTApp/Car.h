#pragma once

#include  <GL\freeglut.h>
#include <functional>
#include "Model.h"

/*
The Car object, renders the car
*/
class Car
{
public:
	Car(const std::string fileName);
	GLfloat local[16];	//local coordinate system transformation matrix
	Model *carro;
	GLfloat camHorizontalAngle;
	GLfloat camVerticalAngle;
	GLfloat moveSpeed;
	GLfloat carRotate;
	std::function<void()> nextMove;
	bool isMoving;


	//Functions
	void keyPressed(const unsigned char key);
	void keyUp(const unsigned char key);
	void init();
	void draw();
	~Car() = default;
private:
	//update constant animation for tail wiggle and legs movement
	void updateCameraHoriMovement();
	bool isWPressing, isSPressing, isAPressing, isDPressing;
	GLfloat legsAngle;
	bool legsMovementDirectionForward;
};

