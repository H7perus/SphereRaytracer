#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

class hitobject {
public:
	int objectType = 0;
	int object = 0;
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 hitCoord = glm::vec3(0.0f, 0.0f, 0.0f);
};