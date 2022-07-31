#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>
#include <glm/trigonometric.hpp>
#include "shader.h"

#define PI 3.14159265358979323846 


class ray
{
public:
	ray();
	~ray();
	glm::vec3 origin;
	glm::vec3 direction;

private:

};

ray::ray()
{
}

ray::~ray()
{
}