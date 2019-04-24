#include <iostream>
#include <glm.hpp>
#include "main.h"
#include "mygl.h"

#define PI 3.1415

using namespace std;

objLoader *objData;

float S = 1; //escala

float phi = 0;
float theta = 0;


void aplicaDist(glm::mat4 &transforma){
    glm::vec4 vertex[3];

    for(int i = 0; i < objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];
		for(int j = 0; j < 3; j++){
			for(int k = 0; k < 3; k++){
				vertex[j][k] = objData->vertexList[o->vertex_index[j]]->e[k];
			}

			vertex[j][3] = 1;
			vertex[j] = transforma * vertex[j];
			vertex[j] /= vertex[j][3];
		}

		Triangle *faces = new Triangle(new Pixel(BRANCO, vertex[0].x, vertex[0].y),
					   	   	   	   	   new Pixel(BRANCO, vertex[1].x, vertex[1].y),
									   new Pixel(BRANCO, vertex[2].x, vertex[2].y));

		faces->DrawTriangle();
		delete faces;
	}

}

void myGlDraw(){
/*######Matriz model: Esp. Obj. --> Esp. Univ.############*/
    glm::mat4 M_model(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);


    //Parametros da camera
    glm::vec3 camera_pos(0, 0, 2);  // Posicao da camera no universo.
    glm::vec3 camera_lookat(0, 0, 0);  //     Ponto para onde a camera esta olhando.
    glm::vec3 camera_up(0, 1, 0);  // 'up' da camera no espaco do universo.


    //Calculo do sistema ortonormal gerado a partir dos parametros da camera
    glm::vec3 camera_dir = camera_lookat - camera_pos;

    glm::vec3 z_camera = -(camera_dir) / (camera_dir);
    glm::vec3 x_camera = glm::cross(camera_up, z_camera) / glm::normalize(glm::cross(camera_up, z_camera));
    glm::vec3 y_camera = glm::cross(z_camera, x_camera);


    //Construcao da matriz view: Esp. Univ. --> Esp. Cam.
    glm::mat4 Bt(x_camera.x, x_camera.y, x_camera.z, 0,
                 y_camera.x, y_camera.y, y_camera.z, 0,
                 z_camera.x, z_camera.y, z_camera.z, 0,
                 0,        0,           0,             1);

    glm::mat4 T(1, 0, 0, -camera_pos.x,
                0, 1, 0, -camera_pos.y,
                0, 0, 1, -camera_pos.z,
                0, 0, 0, 1);

    glm::mat4 M_view = Bt * T;

/*######Construcao da matriz de ModelView: Esp. Obj. --> Esp. Cam. ############*/

    glm::mat4 M_model_view = M_view * M_model;

/*######Construcao da matriz de Projecao: Esp. Cam. --> Esp. Recorte ############*/

    int d = 2; // distancia do centro de projecao para o viewplane

    glm::mat4 M_projection(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, d,
                 0, 0, -(1/d), 0);

/*######Construcao da matriz ModelViewProjection: Esp. Obj. --> Esp. Recorte############*/

    glm::mat4 M_model_view_proj = M_projection * M_model_view;

/*######Aplicacao da matriz ModelViewProjection: Esp. Obj. --> Esp. Recorte############*/
    CleanScreen();
    aplicaDist(M_model_view_proj);


/*  Para rotacionar o objeto variamos o ângulo phi  **/

	if(phi < 2*PI)
		phi += PI/64;
	else
		phi = 0;

	if(theta < PI){
		theta += PI/512;
        S = sin(theta) * 1.7;
	}else
		theta = 0;

}


int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	objData = new objLoader();			// cria o objeto que carrega o modelo
	objData->load("monkey_head2.obj");	// a carga do modelo é indicada atraves do nome do arquivo.

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = myGlDraw;

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}
