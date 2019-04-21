#include "libs.h"

// all variables initialized to 1.0, meaning
// the triangle will initially be white
//float red = 1.0f, blue = 1.0f, green = 1.0f;

// angle of rotation for the camera direction
float angle = 0.0f, angleM = 0.f;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, y = 1.f, z = 5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.f;
float deltaMove = 0;
int xOrigin = -1;

int mainWindow;

//Lights
PointLight pointlight;
GLfloat globalAmbient = 0.3f;

//Camera
Camera camera;

//Models
vector<Model*> models;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
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
	gluPerspective(45.f, ratio, 1.f, 100.f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void initModels() {
	Model *myModel = new Model;
	if(!myModel->importModel("Objects/carrinho.obj"))
		cout << "Import model error!" << endl;

	models.push_back(myModel);
}

void drawModels()
{
	for (int i = 0; i < models.size(); i++) {
		glPushMatrix();

		// 设置模板缓冲为可写状态，把较小的面包放入模板缓冲（设为1）
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glTranslatef(15.0, 1, 10.0);
		glRotatef(angleM, 0.f, 1.f, 0.f);
		glScalef(5.f, 5.f, 5.f);
		models[i]->renderTheModel(0.5f, false);
		glPopMatrix();
	}
}

void deleteModels() {
	for (int i = 0; i < models.size(); i++)
		delete models[i];
}

		
void computePos(float deltaMove) 
{
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void mouseMove(int x, int y) {
	// this will only be true when the left button is down
	if (xOrigin >= 0) {
		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;
		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
		// setting the main window as active
		//and marking it for redraw
		glutSetWindow(mainWindow);
		glutPostRedisplay();
	}
}

void drawSnowMan() {

	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Draw Body	
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);
	
	// Draw Eyes
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}


void display(void) {

	// Clear Color and Depth Buffers
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	// Reset transformations
	glLoadIdentity();

	GLfloat globalAmbientVec[4] = { globalAmbient, globalAmbient, globalAmbient, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientVec);

	// Set the camera
	gluLookAt(
		x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f
	);

	glPushMatrix();
	glTranslatef(pointlight.position[0], pointlight.position[1], pointlight.position[2]);
	pointlight.addLight();
	glPopMatrix();


	// Draw ground
	GLfloat color[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.f, -100.0f);
		glVertex3f(-100.0f, 0.f, 100.0f);
		glVertex3f(100.0f, 0.f, 100.0f);
		glVertex3f(100.0f, 0.f, -100.0f);
	glEnd();

	//Draw Models
	drawModels();
	angleM += 0.1f;

	// Draw 36 SnowMen
	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0, 0, j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}

	glutSwapBuffers();
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = -0.005f; break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.005f; break;
	case GLUT_KEY_UP: deltaMove = 0.5f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void processNormalKeys(unsigned char key, int xx, int yy) {

	if (key == 27)
		exit(0);
}


void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	// register callbacks
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	initModels();

}



int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	mainWindow = glutCreateWindow("JANELINHA");
	
	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutIdleFunc(display);
	init();

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pointlight.enable();

	// enter GLUT event processing cycle
	glutMainLoop();

	deleteModels();
	return 1;
}