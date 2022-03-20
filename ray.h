#pragma once
#include <iostream>
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>

#define PI 3.14159265358979323846 


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

	void rayTrace(sphere& sphere, png::image<png::rgb_pixel>& image, float FOV)
	{

		float FocalLength = (1 / tan(FOV / 360 * PI)) * (image.get_width() / 2);

		ray ray;
		ray.origin = glm::vec3(0.0f, 0.0f, 0.0f);

		png::image< png::rgb_pixel > HDRI("assets/studio_country_hall_4k.png");

		for (png::uint_32 y = 0; y < image.get_height(); y++) {

			for (png::uint_32 x = 0; x < image.get_width(); x++) {

				ray.origin = glm::vec3(0, 0, 0);

				glm::ivec3 brightness(0, 0, 0);
				float z = FocalLength;
				auto testvector = glm::vec3(float(x) - image.get_width() / 2 + 0.5f, float(y) - image.get_height() / 2 + 0.5f, float(z));
				ray.direction = glm::normalize(testvector);

				glm::vec3 spherePositionNormalized = glm::normalize(sphere.pos);
				float angle = glm::angle(ray.direction, spherePositionNormalized);
				//std::cout << angle << std::endl;
				float c = glm::sqrt(pow(sphere.pos.x, 2) + pow(sphere.pos.y, 2) + pow(sphere.pos.z, 2));
				float sinValue = sin(angle);
				float radiusCheck = sinValue * c;
				//std::cout << angle << std::endl;


				bool HDRIhit = false;

				int count = 0;

				while (!HDRIhit)
				{
					float angle = glm::angle(ray.direction, glm::normalize(sphere.pos - ray.origin));
					//std::cout << glm::reflect(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)).x << glm::reflect(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)).y << glm::reflect(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)).z << std::endl;
					float c = glm::sqrt(pow((sphere.pos - ray.origin).x, 2) + pow((sphere.pos - ray.origin).y, 2) + pow((sphere.pos - ray.origin).z, 2));
					float cTEST = glm::sqrt(pow(sphere.pos.x, 2) + pow(sphere.pos.y, 2) + pow(sphere.pos.z, 2));
					//std::cout << "base :" << c << std::endl;
					//std::cout << "test :" << cTEST << std::endl;
					float sinValue = sin(angle);
					float radiusCheck = sinValue * c;

					//std::cout << angle << std::endl;

					if (radiusCheck < sphere.radius)
					{
						//std::cout << "dir  :" << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
						//std::cout << "origin:" << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << std::endl;
						//std::cout << "test :" << c << std::endl;
						//brightness.x = 255;
						//brightness.y = 255;
						//brightness.z = 255;

						float IntersectDistance = sqrt(pow(sphere.radius, 2) - pow(radiusCheck, 2));
						//std::cout << "before: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
						float intersectDistanceTemp = sqrt(pow(c, 2) - pow(radiusCheck, 2));
						glm::vec3 intersect = ray.direction * (intersectDistanceTemp - IntersectDistance) + ray.origin;
						//std::cout << "origin:" << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << std::endl;
						ray.origin = intersect;
						ray.reflect(glm::normalize(intersect - sphere.pos), intersect);
						//std::cout << glm::length((intersect - sphere.pos)) << std::endl;
						////std::cout << "after : " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
						/*std::cout << "intersectd : " << intersectdistance << std::endl;
						std::cout << "check      : " << radiuscheck << std::endl;
						std::cout << "normal     : " << -glm::normalize(intersect - sphere.pos).x << " " << -glm::normalize(intersect - sphere.pos).y << " " << -glm::normalize(intersect - sphere.pos).z << std::endl;
						std::cout << "direction  : " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
						std::cout << "intersect  : " << intersect.x << " " << intersect.y << " " << intersect.z << std::endl;
						std::cout << "testreflect: " << glm::reflect(ray.direction, glm::normalize(intersect - sphere.pos)).x << " " << glm::reflect(ray.direction, glm::normalize(intersect - sphere.pos)).y << " " << glm::reflect(ray.direction, glm::normalize(intersect - sphere.pos)).z << std::endl;
						std::cout << "testangle  : " << glm::angle(glm::vec3(-0.0145698, -0.39972, 0.916521), glm::vec3(-0.0356566, -0.97823, -0.204436)) << std::endl;
						std::cout << x << std::endl;*/
					}

					glm::vec2 HDRIAngle(atan(ray.direction.x / ray.direction.z) + PI, atan(ray.direction.y / glm::length(glm::vec2(ray.direction.z, ray.direction.x))));

					//if (ray.direction.y < 0 && y > 540)
						//std::cout << y << " " << HDRIAngle.y << " " << HDRIAngle.x << std::endl;

					glm::vec3 testvec(float(x) - image.get_width() / 2 + 0.5f, float(y) - image.get_height() / 2 + 0.5f, float(z));

					//std::cout << x << " " << testvec.x << " " << testvec.z << std::endl;

					if (ray.direction.z >= 0 && ray.direction.x >= 0)
					{

						//std::cout << "1  :" << ray.direction.x << " " << ray.direction.z << " " << HDRIAngle.x << std::endl;
						//std::cout << "xy1 :" << x << " " << y << std::endl;
					}
					else if (ray.direction.z < 0 && ray.direction.x >= 0)
					{

						HDRIAngle.x += PI;
						//std::cout << "2 :" << HDRIAngle.x << " " << HDRIAngle.y << std::endl;
					}
					else if (ray.direction.z >= 0 && ray.direction.x < 0)
					{
						//std::cout << "3 :" << HDRIAngle.x << " " << HDRIAngle.y << std::endl;
						//std::cout << "xy2 :" << x << " " << y << std::endl;
						HDRIAngle.x += 2 * PI;

					}
					else if (ray.direction.z < 0 && ray.direction.x < 0)
					{
						HDRIAngle.x += PI;
						//std::cout << "4 :" <<  HDRIAngle.x << " " << HDRIAngle.y << std::endl;
					}


					if (HDRIAngle.x > 2 * PI)
					{
						//std::cout << "test" << std::endl;
						HDRIAngle.x -= 2 * PI;
					}
					if (HDRIAngle.x < 0)
					{
						//std::cout << "test" << std::endl;
						HDRIAngle.x += 2 * PI;
					}


					//glm::vec2 HDRIAngle(atan(ray.direction.x / ray.direction.z) + PI, atan(ray.direction.y / abs(ray.direction.z)) + PI);
					//std::cout << HDRIAngle.y << std::endl;
					//glm::ivec2 HDRIcoord((int)round((HDRI.get_width() / (2 * PI) - 0.5) * HDRIAngle.x), (int)round(((HDRI.get_height() / (2 * PI) - 0.5) * (HDRIAngle.x + 0.5 * PI))));
					glm::ivec2 HDRIcoord((int)round((HDRI.get_width() / (2 * PI) - 0.5) * HDRIAngle.x), (int)round(((HDRI.get_height() / 2 + HDRIAngle.y * HDRI.get_height() / PI))));

					//glm::ivec2 HDRIcoord((int)round((HDRI.get_width() / (2 * PI) - 0.5) * HDRIAngle.x), (int)round(HDRI.get_height() / 2 + HDRIAngle.y * HDRI.get_height() / (2 * PI)));
					//std::cout << x << " " << HDRIAngle.x << HDRIcoord.x << " " << HDRIcoord.y << std::endl;
					brightness.x = HDRI[HDRIcoord.y][HDRIcoord.x].red;
					//std::cout << (int)HDRI[HDRIcoord.y][HDRIcoord.x].red << std::endl;
					brightness.y = HDRI[HDRIcoord.y][HDRIcoord.x].green;
					brightness.z = HDRI[HDRIcoord.y][HDRIcoord.x].blue;
					count++;
					HDRIhit = true;
				}


				//if(brightness > 10)
					//std::cout << brightness << std::endl;
				image[y][x] = png::rgb_pixel(brightness.x, brightness.y, brightness.z);
			}
		}
	}
private:

};

ray::ray()
{
}

ray::~ray()
{
}