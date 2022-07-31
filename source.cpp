#include <string>
#include <iostream>
#include <cmath>
#include <chrono>
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "sdk.h"
#include "ray.h"
#include "camera.h"
#include "world.h"

float geradenKonstante = 1;

#define PI 3.14159265358979323846
int const ResWidth = 1920 * 2;
int const ResHeight = 1080 * 2;
float const Fov = 90;







int main() 
{
	png::image<png::rgb_pixel> image(ResWidth, ResHeight);
	
	//glm::vec3 direction = glm::reflect(glm::normalize(glm::vec3(0, -.1, 1)), glm::normalize(glm::vec3(0, -1, -1)));

	//std::cout << (1 / tan(cam.FOV / 360 * glm::pi<float>())) * (image.get_width() / 2);

	camera cam;
	cam.FOV = 90;

	float camrotation = 0;

	world world;

	cam.pos.x = 0;
	cam.pos.y = 4.5;
	cam.pos.z = -5;

	cam.yaw = 0;
	cam.pitch = 0;
	cam.set_focal_length(image.get_width());

	sphere sphere0;
	sphere sphere1;
	sphere sphere2;
	sphere sphere3;
	sphere sphere4;
	sphere0.pos.x = 0.0f;
	sphere0.pos.y = 6.0f;
	sphere0.pos.z = 0.0f;
	sphere0.radius = 2.0f;
	sphere0.shader.roughness = 1;

	sphere1.pos.x = 2.0f;
	sphere1.pos.y = 2.0f;
	sphere1.pos.z = 2.0f;
	sphere1.radius = 2.0f;
	sphere1.shader.roughness = 0;

	sphere2.pos.x = -2.0f;
	sphere2.pos.y = 2.0f;
	sphere2.pos.z = 2.0f;
	sphere2.radius = 2.0f;
	sphere2.shader.roughness = 0;

	sphere3.pos.x = 2.0f;
	sphere3.pos.y = 2.0f;
	sphere3.pos.z = -2.0f;
	sphere3.radius = 2.0f;
	sphere3.shader.roughness = 0;

	sphere4.pos.x = -2.0f;
	sphere4.pos.y = 2.0f;
	sphere4.pos.z = -2.0f;
	sphere4.radius = 2.0f;
	sphere4.shader.roughness = 0;

	sphere spheres[] = { sphere0, sphere1, sphere2, sphere3, sphere4 };
	int spheresLen = sizeof(spheres) / sizeof(spheres[0]);
	png::image< png::rgb_pixel > HDRI("assets/studio_country_hall_4k.png");
	png::image< png::rgb_pixel > base_texture("assets/uvgrid2.png");

	cam.point_at(glm::vec3(0, 0, 0));

	//for (int i = 0; i < 360; i++)
	//{
	//	cam.pos.x = -glm::sin(glm::radians(camrotation)) * 20;
	//	cam.pos.y = 4.5;
	//	cam.pos.z = -glm::cos(glm::radians(camrotation)) * 20;
	//	camrotation += 1;
	//	cam.point_at(glm::vec3(0, 0, 0));
	//	std::cout << std::to_string(cam.pos.x) + " " + std::to_string(cam.pos.z) << std::endl;
	//	world.render(spheres, spheresLen, image, HDRI, base_texture, cam);
	//	image.write("results2/render2" + std::to_string(i) + ".png");
	//}
	camrotation = 0;
	cam.pos.x = -glm::sin(glm::radians(camrotation)) * 20;
	cam.pos.y = 8.5;
	cam.pos.z = -glm::cos(glm::radians(camrotation)) * 20;
	cam.point_at(glm::vec3(0, 0, 0));
	std::cout << std::to_string(cam.pos.x) + " " + std::to_string(cam.pos.z) << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	world.render(spheres, spheresLen, image, HDRI, base_texture, cam);
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
		elapsed).count();
	std::cout << milliseconds << std::endl;
	image.write("render.png");
	std::cout << "test" << std::endl;

}