#pragma once
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

class camera
{
public:
	camera();
	~camera();
	glm::vec3 pos;
	float pitch;
	float yaw;
	float FOV;
	float focal_length;


	void point_at(glm::vec3 target)
	{
		glm::vec3 target_direction = target - pos;

		yaw = atan2(target_direction.x, target_direction.z) * 180 / PI;
		pitch = atan(target_direction.y / sqrt(pow(target_direction.x, 2) + pow(target_direction.z, 2))) * 180 / PI;
	}
	void set_focal_length(int image_width)
	{
		focal_length = (1 / tan(FOV / 360 * glm::pi<float>())) * (image_width / 2);
		std::cout << focal_length; 
	}
private:

};

camera::camera()
{
}

camera::~camera()
{
}