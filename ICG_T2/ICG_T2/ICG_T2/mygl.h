#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************
//Coordenadas dos pixels
struct Vertice {
	int x;
	int y;
}typedef Vertice;

//Cores dos pixels
struct RGBA {
	int red;
	int green;
	int blue;
	int alpha;

}typedef RGBA;

RGBA black = { 0, 0, 0, 0 };
RGBA white = { 255,255,255,255 };

//Insere um pixel na tela
void putPixel(Vertice pixel, RGBA cores) {

	if ((pixel.x >= 0) && (pixel.x <= 512) && (pixel.y >= 0) && (pixel.y <= 512)) {

		int ajuste = 4 * pixel.x + 4 * pixel.y * IMAGE_WIDTH;
		FBptr[ajuste + 0] = cores.red;
		FBptr[ajuste + 1] = cores.green;
		FBptr[ajuste + 2] = cores.blue;
		FBptr[ajuste + 3] = cores.alpha;
	}

	else {
		return;
	}
}


//Em caso de retas verticais ou horizontais
void noIncLine(Vertice p1, Vertice p2, RGBA c1) {
	putPixel(p1, c1);
	if (p1.x == p2.x) {
		if (p2.y < p1.y) {
			noIncLine(p2, p1, c1);
			return;
		}
		while (p1.y != p2.y) {
			putPixel(p1, c1);
			p1.y++;
		}
	}
	else {
		if (p2.x < p1.x) {
			noIncLine(p2, p1, c1);
			return;
		}
		while (p1.x != p2.x) {
			putPixel(p1, c1);
			p1.x++;
		}
	}
}


//Desenho de retas inclinadas
void drawLine(Vertice pixel1, Vertice pixel2, RGBA cor) {


	//delta x e delta y
	int dx = pixel2.x - pixel1.x;
	int dy = pixel2.y - pixel1.y;
	int inclinacao = 1;

	if (dx == 0 || dy == 0) {
		noIncLine(pixel1, pixel2, cor);
		return;
	}

	if (dx < 0) {
		drawLine(pixel2, pixel1, cor);
		return;
	}

	if (dy < 0) {
		inclinacao = -1;
	}

	int d;                       //variavel de decisao

	Vertice pixel = pixel1;

	putPixel(pixel, cor);     //Raster do primeiro pixel

	if (dx >= inclinacao * dy) {
		if (dy < 0) {
			d = 2 * dy + dx;
			while (pixel.x < pixel2.x) {
				if (d < 0) {
					d += 2 * (dy + dx);
					pixel.x++;
					pixel.y--;
				}
				else {
					d += 2 * dy;
					pixel.x++;
				}
				putPixel(pixel, cor);
			}
		}
		else {
			d = 2 * dy - dx;
			while (pixel.x < pixel2.x) {
				if (d < 0) {                //Pixel abaixo do ponto medio escolhido
					d += 2 * dy;
					pixel.x++;
				}
				else {
					d += 2 * (dy - dx);   //Pixel acima do ponto medio escolhido
					pixel.x++;
					pixel.y++;
				}
				putPixel(pixel, cor);
			}
		}
	}
	else {
		if (dy < 0) {
			d = dy + 2 * dx;
			while (pixel.y > pixel2.y) {
				if (d < 0) {
					d += 2 * dx;
					pixel.y--;
				}
				else {
					d += 2 * (dy + dx);
					pixel.x++;
					pixel.y--;
				}
				putPixel(pixel, cor);
			}
		}
		else { // y1 < y2
			d = dy - 2 * dx;
			while (pixel.y < pixel2.y) {
				if (d < 0) {
					d += 2 * (dy - dx);
					pixel.x++;
					pixel.y++;
				}
				else {
					d += -2 * dx;
					pixel.y++;
				}
				putPixel(pixel, cor);
			}
		}
	}
	putPixel(pixel2, cor);
}

void drawTriangle(Vertice p1, Vertice p2, Vertice p3, RGBA cor) {

	//triangulo1
	drawLine(p1, p2, cor);
	drawLine(p1, p3, cor);
	drawLine(p2, p3, cor);
}


#endif // _MYGL_H_

