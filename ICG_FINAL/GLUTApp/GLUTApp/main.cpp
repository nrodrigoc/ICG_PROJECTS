#include "libs.h"

using namespace std;

#define RED_CAR "Objects/carrinho2.obj"
#define PISTA "Objects/pista.obj"

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
int w, h;

//Lights
PointLight pointlight(0, 5, 0);
SpotLight spotlight;
GLfloat globalAmbient = 0.4f;

//Camera
Camera camera;

//Models
std::vector<Car*> carros;
std::vector<Model*> models;

//Menu veriables
char s[300]; //Guardar o texto;
int mainMenu;
bool start = true;
const void* font = GLUT_BITMAP_TIMES_ROMAN_24;


void changeSize(int width, int heigth) {

	w = width;
	h = heigth;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w * 1.0 / h;


	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the correct perspective.
	gluPerspective(45.f, ratio, 1.f, 100.f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void initModels() {
	Model *pista = new Model(PISTA);
	Car *myCar = new Car(RED_CAR);
	if (!myCar->carro->importModel())
		std::cout << "Import model error!" << std::endl;

	if (!pista->importModel())
		std::cout << "Import model error!" << std::endl;

	models.push_back(pista);
	carros.push_back(myCar);
}

void drawModels()
{
	for (int i = 0; i < carros.size(); i++) {
		glPushMatrix();

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glTranslatef(0.f, 0.f, 0.f);
		glRotatef(angleM, 0.f, 1.f, 0.f);
		glScalef(5.f, 5.f, 5.f);
		carros[i]->carro->renderTheModel();
		glPopMatrix();
	}

	for (int i = 0; i < models.size(); i++) {
		glPushMatrix();

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glTranslatef(0.f, 0.f, 0.f);
		glRotatef(0.f, 0.f, 1.f, 0.f);
		glScalef(5.f, 5.f, 5.f);
		models[i]->renderTheModel();
		glPopMatrix();
	}


}

void deleteModels() {
	for (int i = 0; i < carros.size(); i++)
		delete carros[i];

	delete models[0];
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

void DrawSkyBox(void)
{
	GLfloat fExtent = 15.0f;

	glBegin(GL_QUADS);
	//////////////////////////////////////////////
	// Negative X
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-fExtent, -fExtent, fExtent);

	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-fExtent, -fExtent, -fExtent);

	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-fExtent, fExtent, -fExtent);

	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-fExtent, fExtent, fExtent);


	///////////////////////////////////////////////
	//  Postive X
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(fExtent, -fExtent, -fExtent);

	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(fExtent, -fExtent, fExtent);

	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(fExtent, fExtent, fExtent);

	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(fExtent, fExtent, -fExtent);


	////////////////////////////////////////////////
	// Negative Z 
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-fExtent, -fExtent, -fExtent);

	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(fExtent, -fExtent, -fExtent);

	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(fExtent, fExtent, -fExtent);

	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-fExtent, fExtent, -fExtent);


	////////////////////////////////////////////////
	// Positive Z 
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(fExtent, -fExtent, fExtent);

	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-fExtent, -fExtent, fExtent);

	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-fExtent, fExtent, fExtent);

	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(fExtent, fExtent, fExtent);


	//////////////////////////////////////////////////
	// Positive Y
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-fExtent, fExtent, fExtent);

	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-fExtent, fExtent, -fExtent);

	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(fExtent, fExtent, -fExtent);

	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(fExtent, fExtent, fExtent);


	///////////////////////////////////////////////////
	// Negative Y
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-fExtent, -fExtent, -fExtent);

	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-fExtent, -fExtent, fExtent);

	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(fExtent, -fExtent, fExtent);

	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(fExtent, -fExtent, -fExtent);
	glEnd();
}

void drawSnowMan() {

	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	GLfloat snowmanSpecular[] = { 1.f, 1.f, 1.f };
	GLfloat snowmanShininess = 1.f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, snowmanSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, snowmanShininess);
	glMaterialf(GL_FRONT, GL_EMISSION, 1);
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

void setOrthographiProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glScalef(1, -1, 1);
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, void *font, const char *string) {
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void drawScene() {

	glPushMatrix();
	glTranslatef(pointlight.position[0], pointlight.position[1], pointlight.position[2]);
	pointlight.addLight();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(spotlight.position[0], spotlight.position[1], spotlight.position[2]);
	spotlight.addlight();
	glPopMatrix();

	/*glPushMatrix();
	glTranslatef(spotlight.position[0], spotlight.position[1], spotlight.position[2]);
	spotlight.draw();
	glPopMatrix();*/

	// Draw ground
	glPushMatrix();
	glScalef(8.5, 8.5, 8.5);
	glTranslatef(-0.018f, 0, 0.75);
	glRotatef(180.f, 0, 1, 0);
	models[0]->renderTheModel();
	glPopMatrix();
	
	/*GLfloat color[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.f, -100.0f);
	glVertex3f(-100.0f, 0.f, 100.0f);
	glVertex3f(100.0f, 0.f, 100.0f);
	glVertex3f(100.0f, 0.f, -100.0f);
	glEnd();*/

	//Draw Car Model
	glPushMatrix();
	glMultMatrixf(carros[0]->local);
	carros[0]->draw();
	glPopMatrix();


	// Desenha os "postes" do lado direito
	for (int i = 0; i < 1; i++) {
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10, 0, j*10.0);
			glTranslatef(spotlight.position[0], spotlight.position[1], spotlight.position[2]);
			glRotatef(100.f, -5.0, -10, 0);
			spotlight.draw();
			//drawSnowMan();
			glPopMatrix();
		}
	}

	// Desenha os "postes" do lado esquerdo
	for (int i = 0; i < 1; i++) {
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i * 10, 0, j*10.0);
			glTranslatef(spotlight.position[0]*(-1), spotlight.position[1], spotlight.position[2]);
			glRotatef(100.f, -5.0, 10, 0);
			spotlight.draw();
			//drawSnowMan();
			glPopMatrix();
		}
	}
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

	//Update car transformation matrix
	if (carros[0]->nextMove) {
		carros[0]->isMoving = true;
		GLfloat viewModelMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
		glLoadMatrixf(carros[0]->local);
		carros[0]->nextMove();
		carros[0]->nextMove = nullptr;
		glGetFloatv(GL_MODELVIEW_MATRIX, carros[0]->local);
		glLoadMatrixf(viewModelMatrix);
	}

	/*Camera do carro*/
	GLfloat viewModelMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
	glLoadMatrixf(carros[0]->local);

	glRotatef(carros[0]->camVerticalAngle, 1, 0, 0);
	glRotatef(carros[0]->camHorizontalAngle, 0, 1, 0);
	glTranslated(0, 1.5, 0.9);

	GLfloat cameraPoseInCarView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraPoseInCarView);
	glLoadMatrixf(viewModelMatrix);

	//Hack...
	GLfloat zAngle = atan2(-cameraPoseInCarView[2], cameraPoseInCarView[0]);
	GLfloat yAngle = atan2(-cameraPoseInCarView[9], cameraPoseInCarView[5]);

	gluLookAt(cameraPoseInCarView[12], cameraPoseInCarView[13], cameraPoseInCarView[14],
		sin(zAngle) + cameraPoseInCarView[12],
		-yAngle + cameraPoseInCarView[13],
		cos(zAngle) + cameraPoseInCarView[14],
		0, 1, 0);


	GLfloat globalAmbientVec[4] = { globalAmbient, globalAmbient, globalAmbient, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientVec);

	drawScene();

	glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
	glEnable(GL_DEPTH_TEST); //Enable depth testing

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void normalKeyPress(unsigned char key, int xx, int yy) {
	if (start) {
		carros[0]->keyPressed(key);
	}
}

void normalKeyUp(unsigned char key, int x, int y) {
	if (start) {
		carros[0]->keyUp(key);
	}
}

void processMenuEvents(int option) {

	if (option != 0) {
		start = true;
	}
	else {
		glutDestroyWindow(mainWindow);
		exit(0);
	}
	glFlush();
}

void createGLUTMenus() {

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events

	mainMenu = glutCreateMenu(processMenuEvents);
	//add entries to our menu
	glutAddMenuEntry("Start", 1);
	glutAddMenuEntry("Exit", 0);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void MenuText(void) {

	if (!start) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3d(1.0, 1.0, 1.0);
		setOrthographiProjection();
		glPushMatrix();
		glLoadIdentity();
		sprintf(s, "SPEED");
		renderBitmapString(350, 100, (void *)font, s);
		sprintf(s, "Up arrow - accelerates");
		renderBitmapString(260, 300, (void *)font, s);
		sprintf(s, "Down arrow - break");
		renderBitmapString(260, 350, (void *)font, s);
		sprintf(s, "Left arrow - turn left");
		renderBitmapString(260, 400, (void *)font, s);
		sprintf(s, "Right arrow - turns right");
		renderBitmapString(260, 450, (void *)font, s);
		sprintf(s, "Click on the right button to start");
		renderBitmapString(300, 700, GLUT_BITMAP_HELVETICA_12, s);
		glPopMatrix();
		resetPerspectiveProjection();
		glutSwapBuffers();
	}
	else
		display();
}

void processNormalKeys(unsigned char key, int xx, int yy) {

	if (key == 27)
		exit(0);
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_RIGHT_BUTTON) {
		createGLUTMenus();
	}
}

void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	// register callbacks
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(mouseButton);

	initModels();
}

int main(int argc, char **argv) {
	// init GLUT and create window
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(150, 0);
	glutInitWindowSize(800, 600);
	mainWindow = glutCreateWindow("JANELINHA");

	// register callbacks
	//Aparecer menu
	if (start) {
		glutDisplayFunc(display);
		glutIdleFunc(display);
	}
	else {
		glutDisplayFunc(MenuText);
		glutIdleFunc(MenuText);
	}

	glutReshapeFunc(changeSize);

	init();

	// OpenGL init
	glutKeyboardFunc(normalKeyPress);
	glutKeyboardUpFunc(normalKeyUp);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	pointlight.enable();
	spotlight.enable();

	carros[0]->init();

	// enter GLUT event processing cycle
	glutMainLoop();

	deleteModels();
	return 0;
}
