#pragma once

#include <freeglut.h>
#include <functional>
#include "Model.h"

/*
The Dog object, renders the dog and exposes the dog controls to the ui.
*/
class Car
{
public:
	Car(const std::string fileName);
	GLfloat local[16];	//local coordinate system transformation matrix
	Model *carro;
	GLfloat camHorizontalAngle;
	GLfloat camVerticalAngle;
	std::function<void()> nextMove;
	bool isMoving;

	void init();
	void draw();
	~Car() = default;
private:
	//update constant animation for tail wiggle and legs movement
	void updateConstantMovement();
	GLfloat tailWiggleAngle;
	bool tailWiggleDirectionLeft;
	GLfloat legsAngle;
	bool legsMovementDirectionForward;
};

