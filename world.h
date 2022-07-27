#pragma once
#include <glm/glm.hpp>

class camera
{
public:
	camera();
	~camera();
	glm::vec3 pos;
	float pitch;
	float yaw;

private:

};

camera::camera()
{
}

camera::~camera()
{
}