#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>
#include <glm/trigonometric.hpp>

class shader {
public:
	shader();
	~shader();
	float roughness = 0;
	float metallicity = 0;
	bool use_floor_texture = false;
	glm::ivec3 colour = glm::ivec3(255, 0, 255);
	glm::ivec3 pixel_colour = glm::ivec3(255, 0, 255);
	glm::vec3 sun = glm::vec3(0.0f, -1.0f, 1.0f);
	glm::vec3 sun_colour = glm::vec3(1.0f, 1.0f, 1.0f);
	
	void shade(glm::vec3& ray_direction, glm::vec3& intersect_point, glm::vec3 normal, glm::vec3 origin, glm::ivec3& colour_in)
	{
		glm::ivec3 colour = colour_in;
		if (roughness == 0)
		{
			ray_direction = glm::reflect(ray_direction, normal);
			origin = intersect_point;
		}
		else
		{
			colour = colour_in;
			glm::vec3 rough_test = -glm::reflect(ray_direction, normal);
			pixel_colour = glm::ivec3(glm::vec3(colour) * ((glm::cos(glm::angle(sun, rough_test)) + 1) / 2) * sun_colour);
			if (glm::angle(sun, rough_test) > 3.141592)
			{
				pixel_colour = glm::ivec3(0);
			}
		};
	}
	void shade_lambertian(glm::vec3& ray_direction, glm::vec3& intersect_point, glm::vec3 normal, glm::vec3 &origin, glm::ivec3& colour_in)
	{
		glm::ivec3 colour = colour_in;
		sun = glm::normalize(sun);
		if (roughness == 0)
		{
			ray_direction = glm::reflect(ray_direction, normal);
			origin = intersect_point;
		}
		else
		{
			//glm::vec3 rough_test = -glm::reflect(ray_direction, normal);
			//pixel_colour = glm::ivec3(glm::vec3(colour) * ((glm::sin(glm::angle(sun, rough_test)) + 1) / -2));
			//glm::cos(glm::angle(sun, rough_test))
			pixel_colour = glm::ivec3(glm::vec3(colour) * glm::cos(glm::angle(-sun, normal)) * sun_colour); // * glm::cos(glm::angle(-ray_direction, normal))
			pixel_colour += glm::vec3(colour) * 0.1f;

			if (glm::angle(-sun, normal) > 3.141592 / 2)
			{
				pixel_colour = glm::vec3(colour) * 0.1f;
			}
		};
	}
private:

};

shader::shader()
{
}

shader::~shader()
{
}