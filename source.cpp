#include <string>
#include <iostream>
#include <cmath>
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <Windows.h>
#include "sdk.h"
#include "ray.h"
#include "world.h"

float geradenKonstante = 1;

#define PI 3.14159265358979323846 
int const ResWidth = 1920;
int const ResHeight = 1080;
float const Fov = 20;







int main() 
{
	png::image<png::rgb_pixel> image(ResWidth, ResHeight);
	
	glm::vec3 direction = glm::reflect(glm::normalize(glm::vec3(0, -.1, 1)), glm::normalize(glm::vec3(0, -1, -1)));

	std::cout << direction.x << " " << direction.y << " " << direction.z << std::endl;

	camera cam;

	float camrotation = 0;



	cam.pos.x = 0;
	cam.pos.y = 4.5;
	cam.pos.z = -5;

	cam.yaw = 0;
	cam.pitch = 0;

	sphere sphere0;
	sphere0.pos.x = 0.0f;
	sphere0.pos.y = 5.0f;
	sphere0.pos.z = 0.0f;
	sphere0.radius = 1.0f;
	sphere sphere1;
	sphere1.pos.x = 1.5f;
	sphere1.pos.y = 2.0f;
	sphere1.pos.z = 3.0f;
	sphere1.radius = 1.5f;
	sphere spheres[] = {sphere0, sphere1};
	ray ray;
	int spheresLen = sizeof(spheres) / sizeof(spheres[0]);
	png::image< png::rgb_pixel > HDRI("assets/studio_country_hall_4k.png");
	png::image< png::rgb_pixel > BaseTexture("assets/uvgrid2.png");

	for (int i = 0; i < 50; i++)
	{
		cam.pos.x = -glm::sin(glm::radians(camrotation)) * 20;
		cam.pos.y = 4.5;
		cam.pos.z = -glm::cos(glm::radians(camrotation)) * 20;
		cam.yaw = camrotation;
		std::cout << std::to_string(cam.pos.x) + " " + std::to_string(cam.pos.z) << std::endl;
		ray.rayTrace(spheres, spheresLen, image, Fov, HDRI, BaseTexture, cam);
		image.write("results/render1" + std::to_string(i) + ".png");
		camrotation += 1;
	}

	//ray.rayTrace(spheres, spheresLen, image, Fov, HDRI, BaseTexture, cam);
	//image.write("renderTEST4.png");

}