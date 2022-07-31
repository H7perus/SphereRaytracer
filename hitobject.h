#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "shader.h"

class hitobject {
public:
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec3 hit_coord = glm::vec3(0.0f);
	shader shader;
};