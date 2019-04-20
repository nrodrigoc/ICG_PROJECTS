#include "libs.h"

// all variables initialized to 1.0, meaning
// the triangle will initially be white
//float red = 1.0f, blue = 1.0f, green = 1.0f;

// angle of rotation for the camera direction
float angle = 0.0f;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, y = 1.f, z = 5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

int mainWindow;

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

// file_name contém o nome do arquivo a ser carregado
int loadMesh(const std::string &file_name)
{
	std::ifstream fin(file_name.c_str());
	if (!fin.fail())
		fin.close();
	else
	{
		std::cerr << "Couldn't open file: " << file_name << std::endl;
		return EXIT_FAILURE;
	}

	Assimp::Importer importer;

	const aiScene *assimp_scene_ = importer.ReadFile(file_name,
		aiProcess_Triangulate);
	if (!assimp_scene_)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return EXIT_FAILURE;
	}
	if (assimp_scene_->HasMeshes())
	{
		for (unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++)
		{
			const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];
			for (unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices;
				vertex_id += 3)
			{
				const aiVector3D *vertex_ptr = &mesh_ptr->mVertices[vertex_id];
				glm::dvec3 v0{ vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z };
				glm::dvec3 v1{ vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z };
				glm::dvec3 v2{ vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z };
				// ---> Aqui você salva os vértices V0, V1 e V2 do
				// –--> triângulo na sua estrutura de dados!!!
			}
		}
	}
	return EXIT_SUCCESS;
}

void drawSnowMan() {

	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glNormal3f(0.f, 1.f, 1.f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.7f, 10, 2);
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

void renderScene(void) {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	gluLookAt(	
		x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f
	);

	// Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.f, -100.0f);
		glVertex3f(-100.0f, 0.f, 100.0f);
		glVertex3f(100.0f, 0.f, 100.0f);
		glVertex3f(100.0f, 0.f, -100.0f);
	glEnd();


	//loadMesh("Objects/capacete.obj");


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

void light() {
	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat qaAmbientLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position
	GLfloat qaLightPosition[] = { 1, 1, 1, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}


void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// register callbacks
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	//light();

}



int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	mainWindow = glutCreateWindow("JANELINHA");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	init();

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}