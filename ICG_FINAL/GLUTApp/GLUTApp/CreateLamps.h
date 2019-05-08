#pragma once

#include <math.h>
#include <iostream>
#include <GL/freeglut.h>
#include <vector>

#include "Spotlight.h"

class CreateLamps {
public:
	CreateLamps(int number);
	~CreateLamps() = default;

	void Add();

};
