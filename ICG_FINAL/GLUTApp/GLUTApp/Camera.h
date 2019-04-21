#pragma once

#include <freeglut.h>
#include <glm.hpp>

/*
the Camera controling the external view point.
*/
class Camera
{
public:
	Camera();
	//Functions
	void resetWinSize(int w, int h);
	void updateCameraMovement();

	glm::vec3 position = glm::vec3(0.0f, 1.f, 5.f);
	glm::vec3 target = glm::vec3(position.x + lx, position.y, position.z + lz);


	~Camera() = default;

private:
	// actual vector representing the camera's direction
	float lx = 0.0f, lz = -1.0f;


	//Functions
	void updateView();
	
};