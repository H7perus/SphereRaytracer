#pragma once
#include <glm/glm.hpp>

class ray
{
public:
	ray();
	~ray();
	glm::vec3 origin;
	glm::vec3 direction;

	void reflect(glm::vec3 normal, glm::vec3 intersect)
	{
		origin = intersect;
		direction = glm::reflect(direction, normal);
	}

private:

};

ray::ray()
{
}

ray::~ray()
{
}