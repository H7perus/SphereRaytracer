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
	sphere0.pos.y = 0.0f;
	sphere0.pos.z = 5.0f;
	sphere0.radius = 2.0f;
	sphere spheres[] = {sphere0};
	ray ray;

	png::image< png::rgb_pixel > HDRI("assets/studio_country_hall_4k.png");

	ray.rayTrace(spheres[0], image, Fov, HDRI);
	//ray ray;
	//ray.origin = glm::vec3(0.0f, 0.0f, 0.0f);

	//png::image< png::rgb_pixel > hdri("assets/studio_country_hall_4k.png");

	//for (png::uint_32 y = 0; y < resheight; y++) {

	//	for (png::uint_32 x = 0; x < reswidth; x++) {
	//		
	//		ray.origin = glm::vec3(0, 0, 0);

	//		glm::ivec3 brightness(0, 0, 0);
	//		float z = focallength;
	//		auto testvector = glm::vec3(float(x) - reswidth/2 + 0.5f , float(y) - resheight / 2 + 0.5f, float(z));
	//		ray.direction = glm::normalize(testvector);
	//		
	//		glm::vec3 spherepositionnormalized = glm::normalize(sphere0.pos);
	//		float angle = glm::angle(ray.direction, spherepositionnormalized);
	//		//std::cout << angle << std::endl;
	//		float c = glm::sqrt(pow(sphere0.pos.x, 2) + pow(sphere0.pos.y, 2) + pow(sphere0.pos.z, 2));
	//		float sinvalue = sin(angle);
	//		float radiuscheck = sinvalue * c;
	//		//std::cout << angle << std::endl;


	//		bool hdrihit = false;

	//		int count = 0;

	//		while (!hdrihit)
	//		{
	//			float angle = glm::angle(ray.direction, glm::normalize(sphere0.pos - ray.origin));
	//			//std::cout << glm::reflect(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)).x << glm::reflect(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)).y << glm::reflect(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)).z << std::endl;
	//			float c = glm::sqrt(pow((sphere0.pos - ray.origin).x, 2) + pow((sphere0.pos- ray.origin).y, 2) + pow((sphere0.pos - ray.origin).z, 2));
	//			float ctest = glm::sqrt(pow(sphere0.pos.x, 2) + pow(sphere0.pos.y, 2) + pow(sphere0.pos.z, 2));
	//			//std::cout << "base :" << c << std::endl;
	//			//std::cout << "test :" << ctest << std::endl;
	//			float sinvalue = sin(angle);
	//			float radiuscheck = sinvalue * c;

	//			//std::cout << angle << std::endl;

	//			if (radiuscheck < sphere0.radius)
	//			{
	//				//std::cout << "dir  :" << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
	//				//std::cout << "origin:" << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << std::endl;
	//				//std::cout << "test :" << c << std::endl;
	//				//brightness.x = 255;
	//				//brightness.y = 255;
	//				//brightness.z = 255;
	//				
	//				float intersectdistance = sqrt(pow(sphere0.radius, 2) - pow(radiuscheck, 2));
	//				//std::cout << "before: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
	//				float intersectdistancetemp = sqrt(pow(c, 2) - pow(radiuscheck, 2));
	//				glm::vec3 intersect = ray.direction * (intersectdistancetemp - intersectdistance) + ray.origin; 
	//				//std::cout << "origin:" << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << std::endl;
	//				ray.origin = intersect;
	//				ray.reflect(glm::normalize(intersect - sphere0.pos), intersect);
	//				//std::cout << glm::length((intersect - sphere0.pos)) << std::endl;
	//				////std::cout << "after : " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
	//				/*std::cout << "intersectd : " << intersectdistance << std::endl;
	//				std::cout << "check      : " << radiuscheck << std::endl;
	//				std::cout << "normal     : " << -glm::normalize(intersect - sphere0.pos).x << " " << -glm::normalize(intersect - sphere0.pos).y << " " << -glm::normalize(intersect - sphere0.pos).z << std::endl;
	//				std::cout << "direction  : " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
	//				std::cout << "intersect  : " << intersect.x << " " << intersect.y << " " << intersect.z << std::endl;
	//				std::cout << "testreflect: " << glm::reflect(ray.direction, glm::normalize(intersect - sphere0.pos)).x << " " << glm::reflect(ray.direction, glm::normalize(intersect - sphere0.pos)).y << " " << glm::reflect(ray.direction, glm::normalize(intersect - sphere0.pos)).z << std::endl;
	//				std::cout << "testangle  : " << glm::angle(glm::vec3(-0.0145698, -0.39972, 0.916521), glm::vec3(-0.0356566, -0.97823, -0.204436)) << std::endl;
	//				std::cout << x << std::endl;*/
	//			}

	//			glm::vec2 hdriangle(atan(ray.direction.x / ray.direction.z) + pi, atan(ray.direction.y / glm::length(glm::vec2(ray.direction.z, ray.direction.x))));

	//			//if (ray.direction.y < 0 && y > 540)
	//				//std::cout << y << " " << hdriangle.y << " " << hdriangle.x << std::endl;

	//			glm::vec3 testvec(float(x) - reswidth / 2 + 0.5f, float(y) - resheight / 2 + 0.5f, float(z));
	//			
	//			//std::cout << x << " " << testvec.x << " " << testvec.z << std::endl;

	//			if (ray.direction.z >= 0 && ray.direction.x >= 0)
	//			{
	//				
	//				//std::cout << "1  :" << ray.direction.x << " " << ray.direction.z << " " << hdriangle.x << std::endl;
	//				//std::cout << "xy1 :" << x << " " << y << std::endl;
	//			}
	//			else if (ray.direction.z < 0 && ray.direction.x >= 0)
	//			{

	//				hdriangle.x += pi;
	//				//std::cout << "2 :" << hdriangle.x << " " << hdriangle.y << std::endl;
	//			}
	//			else if (ray.direction.z >= 0 && ray.direction.x < 0)
	//			{
	//				//std::cout << "3 :" << hdriangle.x << " " << hdriangle.y << std::endl;
	//				//std::cout << "xy2 :" << x << " " << y << std::endl;
	//				hdriangle.x += 2 * pi;

	//			}
	//			else if (ray.direction.z < 0 && ray.direction.x < 0)
	//			{
	//				hdriangle.x += pi;
	//				//std::cout << "4 :" <<  hdriangle.x << " " << hdriangle.y << std::endl;
	//			}


	//			if (hdriangle.x > 2 * pi)
	//			{
	//				//std::cout << "test" << std::endl;
	//				hdriangle.x -= 2 * pi;
	//			}
	//			if (hdriangle.x < 0)
	//			{
	//				//std::cout << "test" << std::endl;
	//				hdriangle.x += 2 * pi;
	//			}


	//			//glm::vec2 hdriangle(atan(ray.direction.x / ray.direction.z) + pi, atan(ray.direction.y / abs(ray.direction.z)) + pi);
	//			//std::cout << hdriangle.y << std::endl;
	//			//glm::ivec2 hdricoord((int)round((hdri.get_width() / (2 * pi) - 0.5) * hdriangle.x), (int)round(((hdri.get_height() / (2 * pi) - 0.5) * (hdriangle.x + 0.5 * pi))));
	//			glm::ivec2 hdricoord((int)round((hdri.get_width() / (2 * pi) - 0.5) * hdriangle.x), (int)round(((hdri.get_height() / 2 + hdriangle.y * hdri.get_height() / pi))));

	//			//glm::ivec2 hdricoord((int)round((hdri.get_width() / (2 * pi) - 0.5) * hdriangle.x), (int)round(hdri.get_height() / 2 + hdriangle.y * hdri.get_height() / (2 * pi)));
	//			//std::cout << x << " " << hdriangle.x << hdricoord.x << " " << hdricoord.y << std::endl;
	//			brightness.x = hdri[hdricoord.y][hdricoord.x].red;
	//			//std::cout << (int)hdri[hdricoord.y][hdricoord.x].red << std::endl;
	//			brightness.y = hdri[hdricoord.y][hdricoord.x].green;
	//			brightness.z = hdri[hdricoord.y][hdricoord.x].blue;
	//			count++;
	//			hdrihit = true;
	//		}
	//		

	//		//if(brightness > 10)
	//			//std::cout << brightness << std::endl;
	//		image[y][x] = png::rgb_pixel(brightness.x, brightness.y, brightness.z);
	//	}
	//}
	image.write("test.png");
	//(x - h) ^ 2 + (y - k) ^ 2 + (z - l) - r ^ 2 = 0;
}