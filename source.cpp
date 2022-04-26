#include <string>
#include <iostream>
#include <cmath>
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "sdk.h"
#include "ray.h"

float geradenKonstante = 1;

#define PI 3.14159265358979323846 
int const ResWidth = 1920;
int const ResHeight = 1080;
float const Fov = 106;


int main() 
{
	png::image<png::rgb_pixel> image(ResWidth, ResHeight);
	

	sphere sphere0;
	sphere0.pos.x = 0.0f;
	sphere0.pos.y = 2.0f;
	sphere0.pos.z = 5.0f;
	sphere0.radius = 2.0f;
	sphere spheres[] = {sphere0};
	ray ray;

	png::image< png::rgb_pixel > HDRI("assets/studio_country_hall_4k.png");
	png::image< png::rgb_pixel > BaseTexture("assets/uvgrid2.png");
	std::cout << "test";
	ray.rayTrace(spheres[0], image, Fov, HDRI, BaseTexture);
	image.write("test.png");
	std::cout << "test";
	//(x - h) ^ 2 + (y - k) ^ 2 + (z - l) - r ^ 2 = 0;
}