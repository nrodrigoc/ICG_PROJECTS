#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm.hpp>

#define SkyboxSize 1000.f

void loadTex(int i,const char *filename, GLuint* texture);

void drawRect(GLuint texture);

void drawSkybox(GLuint *texture);


