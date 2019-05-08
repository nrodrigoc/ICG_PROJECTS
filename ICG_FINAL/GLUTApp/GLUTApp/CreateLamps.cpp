#include "CreateLamps.h"

std::vector<SpotLight> lamp;


CreateLamps::CreateLamps(int number) {

	//SpotLight spot(1.f, 5.f, 30.f, 1.f);

	for (int i = 0; i < number; i++) {
		SpotLight *spot = new SpotLight(-8.0f, 5.f, (i*10) + 30.0f, 1.f);
		lamp.push_back(*spot);
	}

	std::cout << lamp.size();
}

void CreateLamps::Add() {

	
	std::vector<SpotLight>::iterator it = lamp.begin();
	
	while (it != lamp.end())
	{

		glPushMatrix();
		glTranslatef(1, 0, 30.0);
		glTranslatef(it->position[0], it->position[3] * 3, it->position[2]);
		glRotatef(100.f, -5.0, -10, 0);
		it->addlight();
		it->draw();
		it++;
		glPopMatrix();
		
	}

}
