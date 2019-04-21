#include "Camera.h"
#include <freeglut.h>

Camera::Camera() {
	position = glm::vec3(0.0f, 1.f, 5.f);
	target = glm::vec3(0, 1, 0);
}


void Camera::resetWinSize(int w, int h)
{
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.f, ratio, 1.f, 1000.f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	updateView();
}



void Camera::updateCameraMovement()
{
	updateView();
}

void Camera::updateView()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		position.x, position.y, position.z,
		target.x, target.y, target.z,
		0.0f, 1.0f, 0.0f
	);

}

