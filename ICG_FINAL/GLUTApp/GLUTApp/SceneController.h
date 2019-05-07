#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm.hpp>
#include "Model.h"

#define SkyboxSize 600.f             //天空盒大小

void loadTex(int i,const char *filename, GLuint* texture);

void drawRect(GLuint texture);