#pragma once
#include <iostream>
#include "world.h"
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>
#include "hitobject.h"
#include <glm/trigonometric.hpp>

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
		direction.x *= 1;
		direction.z *= 1;
	}

	void rayTrace(sphere sphereArray[], int& sphereArrayLen, png::image<png::rgb_pixel>& image, float FOV, png::image<png::rgb_pixel>& HDRI, png::image<png::rgb_pixel>& BaseTexture , camera &cam)
	{
		float BasePlaneHeight = -0.0f;

		float FocalLength = (1 / tan(FOV / 360 * PI)) * (image.get_width() / 2);

		float BaseTexScale = 102.4;

		/*for (int i = 0; i < sphereArrayLen; i++)
		{
			glm::vec3 spherePositionNormalized = glm::normalize(sphereArray[i].pos);
		}*/


		float pitch = cam.pitch;
		float yaw = cam.yaw;

		glm::vec3 i_hat = glm::vec3(glm::cos(glm::radians(yaw)), 0.0f, -glm::sin(glm::radians(yaw)));	//x unit vector
		glm::vec3 j_hat = glm::vec3(glm::sin(glm::radians(yaw)) * -glm::sin(glm::radians(pitch)), glm::cos(glm::radians(pitch)), glm::cos(glm::radians(yaw)) * -glm::sin(glm::radians(pitch)));	//y unit vector
		glm::vec3 k_hat = glm::vec3(glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)), glm::sin(glm::radians(pitch)), glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));	//z unit vector;
		
		glm::mat3 rotation_matrix = glm::mat3(i_hat.x, j_hat.x, k_hat.x,
			i_hat.y, j_hat.y, k_hat.y,
			i_hat.z, j_hat.z, k_hat.z);

		for (png::uint_32 y = 0; y < image.get_height(); y++) {

			for (png::uint_32 x = 0; x < image.get_width(); x++) {

				origin = cam.pos;

				glm::ivec3 brightness(0, 0, 0);
				float z = FocalLength;
				auto testvector = glm::vec3(float(x) - image.get_width() / 2 + 0.5f, -float(y) + image.get_height() / 2 + 0.5f, float(z));

				testvector = testvector * rotation_matrix;

				direction = glm::normalize(testvector);

				bool HDRIhit = false;

				int count = 0;
				int sphereHitNum = -1;

				while (!HDRIhit)
				{

					glm::vec3 intersectPlane;
					glm::vec3 intersectSphere;
					hitobject hit;
					bool planehit = false;
					bool spherehit = false;
					bool rayBounceStop = false;
					sphereHitNum = -1;
					while (!rayBounceStop)
					{
						intersectPlane = rayPlaneIntersect(BasePlaneHeight, planehit);
						//if (spherehit == false) {
						//	intersectSphere = raySphereIntersect(sphere, spherehit);
						//}
						if (spherehit == false) 
						{
							//std::cout << sizeof(sphereArray) << std::endl;
							//std::cout << sizeof(sphereArray[0]) << std::endl;
							//std::cout << sizeof(sphereArray) / sizeof(sphereArray[0]) << std::endl;

							for (int i = 0; i < sphereArrayLen; i++)
							{
								if (sphereHitNum != i) 
								{
									intersectSphere = raySphereIntersect(sphereArray[i], spherehit);
									if (spherehit == true)
									{
										sphereHitNum = i;
										//std::cout << "hitnum after intersect: " << sphereHitNum << std::endl;
										break;
									}
								}
							}
						}
						else 
						{
							spherehit = false;
						}
						if (spherehit == true && planehit == true)
						{
							if (glm::length(intersectPlane - origin) >= glm::length(intersectSphere - origin))
							{
								planehit = false;
								
							}
							else
							{
								spherehit = false;
							}
						}
						if (spherehit == true && planehit == false)
						{
							reflect(glm::normalize(intersectSphere - sphereArray[sphereHitNum].pos), intersectSphere);
							spherehit = false;
							/*rayBounceStop = true;
							brightness.x = 0;
							brightness.y = ((intersectSphere.y + 0.0f) / 2.0f * 255);
							brightness.z = 0;*/
							//std::cout << origin.z << std::endl;
							//std::cout <<  "hitnum at reflect: " << sphereHitNum << std::endl;
							//std::cout << origin.z << std::endl;

						}
						else if (spherehit == false && planehit == true)
						{
							//std::cout << x;
							rayBounceStop = true;
							glm::ivec2 BaseTexCoord(int(round((intersectPlane.x) * BaseTexScale)) % BaseTexture.get_width(), int(round((-intersectPlane.z) * BaseTexScale)) % BaseTexture.get_height());
							//std::cout << intersectPlane.z * BaseTexScale << std::endl;
							brightness.x = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].red;
							brightness.y = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].green;
							brightness.z = BaseTexture[BaseTexCoord.y][BaseTexCoord.x].blue;

							/*brightness.x = int(intersectPlane.x * 25);
							brightness.y = 0;
							brightness.z = 0;*/
							//reflect(glm::vec3(0.0f, 1.0f, 0.0f), intersectPlane);
							//planehit = false;
						}
						else if (spherehit == false && planehit == false)
						{
							rayBounceStop = true;
							HDRIhit = true;
						}

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
					glm::ivec2 HDRIcoord(HDRI.get_width() - (int)round((HDRI.get_width() / (2 * PI) - 0.5) * HDRIAngle.x), HDRI.get_height() - (int)round(((HDRI.get_height() / 2 + HDRIAngle.y * HDRI.get_height() / PI))));

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
		//sphere.pos -= origin;
		glm::vec3 intersect = glm::vec3(0.0f);
		float angle = glm::angle(direction, glm::normalize(sphere.pos - origin));

		if (angle < PI / 2)
		{
			float c = glm::distance(origin, sphere.pos);	//distance to sphere
			float sinValue = sin(angle);					//sin value for radius check
			float radiusCheck = sinValue * c;				
			if (radiusCheck <= sphere.radius)
			{
				float IntersectDistance = sqrt(pow(sphere.radius, 2) - pow(radiusCheck, 2));
				float IntersectDistanceTemp = sqrt(pow(c, 2) - pow(radiusCheck, 2));
				intersect = direction * (IntersectDistanceTemp - IntersectDistance) + origin;
				spherehit = true;
				//std::cout << IntersectDistanceTemp - IntersectDistance << std::endl;
			}
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
			intersectPlane.x = ((BasePlaneHeight - origin.y) / direction.y) * direction.x + origin.x;

			intersectPlane.y = ((BasePlaneHeight - origin.y) / direction.y) * direction.y + origin.y;
			intersectPlane.z = ((BasePlaneHeight - origin.y) / direction.y) * direction.z + origin.z;
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