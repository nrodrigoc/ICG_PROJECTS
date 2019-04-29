#include <iostream>
#include <glm.hpp>
#include "mygl.h"
#include "definitions.h"
#include <vector>
#include "objLoader.h"



void pipeline() {

	/*Definicao das matrizes de escala, translacao e rotacao*/
	glm::mat4x4 S1 = glm::mat4x4( // Inverte eixo Y
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	glm::mat4x4 M_model = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	M_model = M_model * S1;

	/*Parametros da camera*/
	glm::vec3 camera_pos = glm::vec3(0, 0, -3);
	glm::vec3 camera_lookat = glm::vec3(0, 0, 0);
	glm::vec3 camera_up = glm::vec3(0, 1, 0);


	/*#####Calculo do sistema ortonormal gerado a partir dos parametros da camera#####*/
	glm::vec3 camera_dir = camera_lookat - camera_pos;


	glm::vec3 z_camera = -normalize(camera_dir);
	glm::vec3 x_camera = normalize(cross(camera_up, z_camera));
	glm::vec3 y_camera = cross(z_camera, x_camera);


	/*#####Construcao da matriz view: Esp. Univ. --> Esp. Cam. #######*/

	glm::mat4x4 Bt = glm::mat4x4(x_camera[0], y_camera[0], z_camera[0], 0,
								x_camera[1], y_camera[1], z_camera[1], 0,
								x_camera[2], y_camera[2], z_camera[2], 0,
								0, 0, 0, 1);

	glm::mat4x4 T = glm::mat4x4(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								-camera_dir[0], -camera_dir[1], -camera_dir[2], 1);

	glm::mat4x4 M_view = Bt * T;

	/*#####Construcao da matriz view: Esp. Cam. --> Esp. Proj. #######*/
	int d = 1;

	glm::mat4x4 M_projection_t = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, -(1 / d),
		0, 0, d, 0);

	/*Conversao de coordenadas do espaco cananico para o espaco de tela.*/
	int w = 512;
	int h = 512;

	glm::mat4x4 TranslacaoTela = glm::mat4x4( //posiciona o objeto no centro da tela
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		(w - 1) / 2, (h - 1) / 2, 0, 1);

		
	glm::mat4x4 S2 = glm::mat4x4( //adapta às coordenadas da tela
		w/2, 0, 0, 0,
		0, h/2, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	glm::mat4x4 M_viewport = TranslacaoTela * S2 * S1;

	glm::mat4x4 pipeline = M_viewport * M_projection_t * M_view * M_model;


	//Pegando vertices forcenidos pelo Assimp

	std::vector<glm::vec4> vertices = importModel("monkey.obj");

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = pipeline * vertices[i];
		vertices[i] = vertices[i] / vertices[i].w;
	}

	for (int i = 0; i < vertices.size(); i += 3)
	{
		Vertice pixel1 = { (int)vertices[i].x, (int)vertices[i].y };
		Vertice pixel2 = { (int)vertices[i + 1].x, (int)vertices[i + 1].y };
		Vertice pixel3 = { (int)vertices[i + 2].x, (int)vertices[i + 2].y };

		drawTriangle(pixel1, pixel2, pixel3, white);

	}



}
