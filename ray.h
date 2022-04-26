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

	void rayTrace(sphere& sphere, png::image<png::rgb_pixel>& image, float FOV, png::image<png::rgb_pixel>& HDRI, png::image<png::rgb_pixel>& BaseTexture)
	{
		float BasePlaneHeight = -2.0f;

		float FocalLength = (1 / tan(FOV / 360 * PI)) * (image.get_width() / 2);

		float BaseTexScale = 500;

		for (png::uint_32 y = 0; y < image.get_height(); y++) {

			for (png::uint_32 x = 0; x < image.get_width(); x++) {

				origin = glm::vec3(0.0f, 0.0f, 0.0f);

				glm::ivec3 brightness(0, 0, 0);
				float z = FocalLength;
				auto testvector = glm::vec3(float(x) - image.get_width() / 2 + 0.5f, float(y) - image.get_height() / 2 + 0.5f, float(z));
				direction = glm::normalize(testvector);

				glm::vec3 spherePositionNormalized = glm::normalize(sphere.pos);
				float angle = glm::angle(direction, spherePositionNormalized);

				float c = glm::sqrt(pow(sphere.pos.x, 2) + pow(sphere.pos.y, 2) + pow(sphere.pos.z, 2));
				float sinValue = sin(angle);
				float radiusCheck = sinValue * c;


				bool HDRIhit = false;

				int count = 0;

				while (!HDRIhit)
				{
					float angle = glm::angle(direction, glm::normalize(sphere.pos - origin));
					float c = glm::sqrt(pow((sphere.pos - origin).x, 2) + pow((sphere.pos - origin).y, 2) + pow((sphere.pos - origin).z, 2));
					float cTEST = glm::sqrt(pow(sphere.pos.x, 2) + pow(sphere.pos.y, 2) + pow(sphere.pos.z, 2));
					float sinValue = sin(angle);
					float radiusCheck = sinValue * c;

					glm::vec3 intersectPlane;
					glm::vec3 intersectSphere;

					bool planehit = false;
					bool spherehit = false;
					bool rayBounceStop = false;
					while (!rayBounceStop)
					{
						intersectPlane = rayPlaneIntersect(BasePlaneHeight, planehit);
						if (spherehit == false) {
							intersectSphere = raySphereIntersect(sphere, spherehit);
						}
						else 
						{
							spherehit = false;
						}
						if (spherehit == true && planehit == true)
						{
							if ((intersectPlane - origin).length() >= (intersectSphere - origin).length())
							{
								spherehit = true;
							}
							else
							{
								planehit = false;
							}
						}
						if (spherehit == true && planehit == false)
						{
							reflect(glm::normalize(intersectSphere - sphere.pos), intersectSphere);
							//std::cout << origin.z << std::endl;

						}
						else if (spherehit == false && planehit == true)
						{
							//std::cout << x;
							rayBounceStop = true;
							glm::ivec2 BaseTexCoord(int(round(intersectPlane.x * BaseTexScale)) % BaseTexture.get_width(), int(round(intersectPlane.z * BaseTexScale)) % BaseTexture.get_height());
							//std::cout << intersectPlane.z * BaseTexScale << std::endl;
							brightness.x = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].red;
							brightness.y = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].green;
							brightness.z = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].blue;
						}
						else if (spherehit == false && planehit == false)
						{
							rayBounceStop = true;
							HDRIhit = true;
						}

					}
					//while (!rayBounceStop)
					//{
					//	float c = glm::sqrt(pow((sphere.pos - origin).x, 2) + pow((sphere.pos - origin).y, 2) + pow((sphere.pos - origin).z, 2));
					//	float radiusCheck = sinValue * c;
					//	if (direction.y < 0 && direction.y * BasePlaneHeight >= 0)
					//	{
					//		if (spherehit)
					//		{
					//			std::cout << "origin :" << origin.z << std::endl;
					//		}


					//		planehit = true;
					//		intersectPlane.x = ((origin.y - BasePlaneHeight) / direction.y) * direction.x + origin.x;

					//		intersectPlane.y = ((origin.y - BasePlaneHeight) / direction.y) * direction.y + origin.y;
					//		intersectPlane.z = ((origin.y - BasePlaneHeight) / direction.y) * direction.z + origin.z;
					//	}
					//	if (radiusCheck < sphere.radius && spherehit == false)
					//	{

					//		float IntersectDistance = sqrt(pow(sphere.radius, 2) - pow(radiusCheck, 2));
					//		float intersectDistanceTemp = sqrt(pow(c, 2) - pow(radiusCheck, 2));
					//		glm::vec3 intersect = direction * (intersectDistanceTemp - IntersectDistance) + origin;
					//		std::cout << intersect.z << std::endl;

					//		if ((intersect - origin).z * direction.z > 0 && glm::length(intersectPlane) > glm::length(intersect))
					//		{
					//				reflect(glm::normalize(intersect - sphere.pos), intersect);
					//				std::cout << "dir  :" << direction.z << std::endl;
					//				std::cout << "inter:" << origin.z << std::endl;
					//				spherehit = true;
					//				planehit = false;
					//				std::cout << "origin :" << origin.z << std::endl;
					//				std::cout << "origin :" << glm::normalize(intersect - sphere.pos).z << std::endl;
					//		}
					//		else
					//		{
					//			spherehit = true;
					//		}
					//	}
					//	else if(planehit)
					//	{
					//		rayBounceStop = true;
					//		glm::ivec2 BaseTexCoord(int(round(intersectPlane.x * BaseTexScale)) % BaseTexture.get_width(),int(round(intersectPlane.z * BaseTexScale )) % BaseTexture.get_height());
					//		std::cout << intersectPlane.z * BaseTexScale << std::endl;
					//		brightness.x = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].red;
					//		brightness.y = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].green;
					//		brightness.z = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].blue;
					//	}
					//	else
					//	{
					//		rayBounceStop = true;
					//	}
					//}

					if (radiusCheck < sphere.radius)
					{

					}

					glm::vec2 HDRIAngle(atan(direction.x / direction.z) + PI, atan(direction.y / glm::length(glm::vec2(direction.z, direction.x))));

					glm::vec3 testvec(float(x) - image.get_width() / 2 + 0.5f, float(y) - image.get_height() / 2 + 0.5f, float(z));

					if (direction.z >= 0 && direction.x >= 0)
					{
					}
					else if (direction.z < 0 && direction.x >= 0)
					{

						HDRIAngle.x += PI;
						//std::cout << "2 :" << HDRIAngle.x << " " << HDRIAngle.y << std::endl;
					}
					else if (direction.z >= 0 && direction.x < 0)
					{
						HDRIAngle.x += 2 * PI;

					}
					else if (direction.z < 0 && direction.x < 0)
					{
						HDRIAngle.x += PI;
						//std::cout << "4 :" <<  HDRIAngle.x << " " << HDRIAngle.y << std::endl;
					}


					if (HDRIAngle.x > 2 * PI)
					{
						HDRIAngle.x -= 2 * PI;
					}
					if (HDRIAngle.x < 0)
					{
						HDRIAngle.x += 2 * PI;
					}
					glm::ivec2 HDRIcoord((int)round((HDRI.get_width() / (2 * PI) - 0.5) * HDRIAngle.x), (int)round(((HDRI.get_height() / 2 + HDRIAngle.y * HDRI.get_height() / PI))));

					if (planehit == false)
					{
						brightness.x = HDRI[HDRIcoord.y][HDRIcoord.x].red;
						brightness.y = HDRI[HDRIcoord.y][HDRIcoord.x].green;
						brightness.z = HDRI[HDRIcoord.y][HDRIcoord.x].blue;
					}

					count++;
					HDRIhit = true;



					image[y][x] = png::rgb_pixel(brightness.x, brightness.y, brightness.z);
				}
			}
		}
	}
private:
	glm::vec3 raySphereIntersect(sphere& sphere, bool& spherehit)
	{
		glm::vec3 intersect = glm::vec3(0, 0, 0);
		float angle = glm::angle(direction, glm::normalize(sphere.pos - origin));
		float c = glm::sqrt(pow((sphere.pos - origin).x, 2) + pow((sphere.pos - origin).y, 2) + pow((sphere.pos - origin).z, 2));
		float sinValue = sin(angle);
		float radiusCheck = sinValue * c;
		if (radiusCheck <= sphere.radius)
		{
			float IntersectDistance = sqrt(pow(sphere.radius, 2) - pow(radiusCheck, 2));
			float intersectDistanceTemp = sqrt(pow(c, 2) - pow(radiusCheck, 2));
			intersect = direction * (intersectDistanceTemp - IntersectDistance) + origin;
			spherehit = true;
			
		}
		return intersect;
	}
	glm::vec3 rayPlaneIntersect(float& BasePlaneHeight, bool& planehit)
	{
		glm::vec3 intersectPlane = glm::vec3(0, 0, 0);;
		planehit = false;
		if (direction.y < 0 && origin.y > BasePlaneHeight || direction.y > 0 && origin.y < BasePlaneHeight)
		{
			planehit = true;
			intersectPlane.x = ((origin.y - BasePlaneHeight) / direction.y) * direction.x + origin.x;

			intersectPlane.y = ((origin.y - BasePlaneHeight) / direction.y) * direction.y + origin.y;
			intersectPlane.z = ((origin.y - BasePlaneHeight) / direction.y) * direction.z + origin.z;
		}
		return intersectPlane;


	}


};

ray::ray()
{
}

ray::~ray()
{
}