#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	//*************************************************************************
	// Chame aqui as fun��es do mygl.h
	//*************************************************************************
    desenhar();
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializa��es.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a fun��o que chama as fun��es do mygl.h
	DrawFunc = MyGlDraw;

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}

