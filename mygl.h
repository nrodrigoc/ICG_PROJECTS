#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************
//Coordenadas dos pixels
struct Vertice{
        int x;
        int y;

}typedef Vertice;

//Cores dos pixeis
struct RGBA{
    int red;
    int green;
    int blue;
    int alpha;

}typedef RGBA;

//Insere um pixel na tela
void putPixel(Vertice pixel, RGBA cor){


   int ajuste = (4*(pixel.x) + 4*pixel.y*512);

    FBptr[ajuste] = cor.red;
    FBptr[ajuste+1] = cor.green;
    FBptr[ajuste+2] = cor.blue;
    FBptr[ajuste+3] = cor.alpha;


}

//Em caso de retas verticais
void noIncLine(Vertice p1, Vertice p2, RGBA c1, RGBA c2){
    if(p2.y < p1.y){
        noIncLine(p2, p1, c2, c1);
        return;
    }
    while(p1.y != p2.y){
        putPixel(p1, c1);
        p1.y++;
    }
}


//Desenho de retas inclinadas
void drawLine(Vertice pixel1, Vertice pixel2, RGBA cor1, RGBA cor2){


    //delta x e delta y
    int dx = pixel2.x - pixel1.x;
    int dy = pixel2.y - pixel1.y;
    int inclinacao = 0;

    /*inverte os vertices caso o dx seja negativo*/
    if(dx<0){
        drawLine(pixel2,pixel1,cor2,cor1);
        return;
    }

    if(dx == 0){
        noIncLine(pixel1, pixel2, cor1, cor2);
        return;
    }

    if(dy<0){
        inclinacao = -1;
    }else{
        inclinacao = 1;
    }


    int d;

    RGBA cor = cor1;
    Vertice pixel = pixel1;

    putPixel(pixel, cor);

    if(dx >= inclinacao*dy){    // inclinacao maior ou igual a 1
        if(dy<0){ // caso y2 < y1
            d = 2*dy+dx;
            while(pixel.x < pixel2.x){
                if(d<0){// escolhido é o pixel acima do ponto medio
                    d += 2*(dy+dx);
                    pixel.x++;
                    pixel.y--;

                }else{//escolhido é o pixel abaixo do ponto medio
                    d+=2*dy;
                    pixel.x++; // varia apenas no eixo x

                }
                putPixel(pixel,cor);
            }
        }else{ // caso y1<y2
            d=2*dy-dx;
            while(pixel.x<pixel2.x){
                if(d<0){ //o pixel de cima é escolhido
                    d+=2*dy;
                    pixel.x++; // varia apenas no eixo x
                }
                else{ //o pixel de baixo é escolhido
                    d+=2*(dy-dx);
                    pixel.x++;
                    pixel.y++;
                }
                putPixel(pixel,cor);
            }
        }
    }
    putPixel(pixel2, cor2);
}

void desenhar(){
    Vertice p1 = {5, 50};
    Vertice p2 = {50, 5};
    Vertice p3 = {100, 50};
    RGBA cor = {255, 0, 0, 255};

    //triangulo1
    drawLine(p1, p2, cor, cor);
    drawLine(p1, p3, cor, cor);
    drawLine(p2, p3, cor, cor);

    //triangulo2
    Vertice p4 = {200, 300};
    Vertice p5 = {400, 200};
    Vertice p6 = {200, 250};

    drawLine(p4, p5, cor, cor);
    drawLine(p4, p6, cor, cor);
    drawLine(p5, p6, cor, cor);

}


#endif // _MYGL_H_

