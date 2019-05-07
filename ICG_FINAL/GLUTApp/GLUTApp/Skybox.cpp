#include "Skybox.h"
#include <SOIL2.h>

void loadTex(int i, const char *filename, GLuint* texture) {
	//BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap信息头    
	unsigned char* image;                                       // 纹理数据    

	int width, height;
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,         //mipmap层次(通常为，表示最上层)     
		GL_RGB,    //我们希望该纹理有红、绿、蓝数据    
		width, //纹理宽带，必须是n，若有边框+2     
		height, //纹理高度，必须是n，若有边框+2     
		0, //边框(0=无边框, 1=有边框)     
		GL_RGB,    //bitmap数据的格式    
		GL_UNSIGNED_BYTE, //每个颜色数据的类型    
		image);    //bitmap数据指针    

	SOIL_free_image_data(image);

	//glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawRect(GLuint texture) {
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);    //天空盒加环境光
	glEnable(GL_COLOR_MATERIAL);

	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]   
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x1,y2 },{ x2,y2 },{ x2,y1 } };
	int dir[4][2] = { { 1,0 },{ 1,1 },{ 0,1 },{ 0,0 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
}

void drawSkybox(GLuint* texture) {
	//Cima
	glPushMatrix();
	glTranslatef(0.0f, (SkyboxSize / 2.0f) - 400, 0.0f);
	glRotatef(270, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[0]);
	glPopMatrix();

	//Baixo  
	glPushMatrix();
	glTranslatef(0.0f, (-SkyboxSize / 2.0f) + 400, 0.0f);
	glRotatef(90, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[1]);
	glPopMatrix();

	//Esquerda
	glPushMatrix();
	glTranslatef(SkyboxSize / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize - 800, 1);
	drawRect(texture[2]);
	glPopMatrix();


	//Direita
	glPushMatrix();
	glTranslatef(-SkyboxSize / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize - 800, 1);
	drawRect(texture[3]);
	glPopMatrix();


	//Frente
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, SkyboxSize / 2.0f);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize - 800, 1);
	drawRect(texture[4]);
	glPopMatrix();

	//Costas
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -SkyboxSize / 2.0);
	glRotatef(180, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize - 800, 1);
	drawRect(texture[5]);
	glPopMatrix();

}