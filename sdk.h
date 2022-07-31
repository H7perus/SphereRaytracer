#pragma once
#include <glm/glm.hpp>
#include "shader.h"

class sphere
{
public:
	sphere();
	~sphere();
	float radius;
	glm::vec3 pos;
	shader shader;

private:

};

sphere::sphere()
{
}

sphere::~sphere()
{
}