#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#include "sdk.h"
#include "ray.h"
#include "camera.h"
#include "hitobject.h"
#include <vector>


class world
{
public:
	float focal_length;
	float floor_texture_scale = 1024;
	glm::mat3 rotation_matrix;
	glm::vec3 pixel_direction;
	bool raybounce;
	float floor_height = -2.0f;
	float BaseTexScale = 102.4;

	void render(sphere sphereArray[], int& sphereArrayLen, png::image<png::rgb_pixel>& image, png::image<png::rgb_pixel>& HDRI, png::image<png::rgb_pixel>& BaseTexture, camera& cam)
	{
		generate_rotationmatrix(cam);
		//std::cout << glm::to_string(rotation_matrix);

		for (png::uint_32 y = 0; y < image.get_height(); y++) {
			//std::cout << y << std::endl;
			for (png::uint_32 x = 0; x < image.get_width(); x++) {
				pixel_direction = glm::vec3(float(x) - image.get_width() / 2 + 0.5f, -float(y) + image.get_height() / 2 + 0.5f, float(cam.focal_length));
				pixel_direction = glm::normalize(glm::vec3(pixel_direction) * rotation_matrix);
				

				png::rgb_pixel pixel_colour;
				raybounce = true;
				ray ray;
				ray.direction = pixel_direction;
				ray.origin = cam.pos;
				std::vector<hitobject> hits(0);
				while (raybounce == true)
				{
					glm::ivec3 color;
					hits.resize(0);
					bool did_intersect = ray_intersect(sphereArray, sphereArrayLen, ray, hits);
					if (did_intersect == false)
					{
						//glm::vec3 test(-6.68168068e-05, 0.895292044, -0.445479155);
						pixel_colour = hdri_hit(HDRI, ray.direction);
						raybounce = false;
					}
					else
					{
						hitobject final_hit = hit_filter(ray.origin, hits);
						if (final_hit.shader.use_floor_texture == true)
						{
							glm::ivec2 BaseTexCoord(int(round((final_hit.hit_coord.x) * BaseTexScale)) % BaseTexture.get_width(), int(round((-final_hit.hit_coord.z) * BaseTexScale)) % BaseTexture.get_height());
							pixel_colour = BaseTexture[BaseTexCoord.y][BaseTexCoord.x];
							glm::ivec3 color = glm::ivec3(pixel_colour.red, pixel_colour.green, pixel_colour.blue);
							final_hit.shader.shade_lambertian(ray.direction, final_hit.hit_coord, glm::vec3(0, 1, 0), ray.origin, color);
							pixel_colour = png::rgb_pixel(color.x, color.y, color.z);
							raybounce = false;
						}
						else
						{
							color = glm::ivec3(100, 60, 120);
							
							final_hit.shader.shade_lambertian(ray.direction, final_hit.hit_coord, final_hit.normal, ray.origin, color);
							pixel_colour = png::rgb_pixel(final_hit.shader.pixel_colour.x, final_hit.shader.pixel_colour.y, final_hit.shader.pixel_colour.z);
							if (final_hit.shader.roughness > 0)
							{
								pixel_colour = png::rgb_pixel(final_hit.shader.pixel_colour.x, final_hit.shader.pixel_colour.y, final_hit.shader.pixel_colour.z);
								raybounce = false;
							}
							
						}
						//pixel_colour = png::rgb_pixel(0, 0, 0);
					}
				}
				image[y][x] = pixel_colour;
				//std::cout << image[y][x].red;
				//std::cout << "test111";
			}
		}
	}
private:
	void generate_rotationmatrix(camera cam)
	{
		glm::vec3 i_hat = glm::vec3(glm::cos(glm::radians(cam.yaw)), 0.0f, -glm::sin(glm::radians(cam.yaw)));	//x unit vector
		glm::vec3 j_hat = glm::vec3(glm::sin(glm::radians(cam.yaw)) * -glm::sin(glm::radians(cam.pitch)), glm::cos(glm::radians(cam.pitch)), glm::cos(glm::radians(cam.yaw)) * -glm::sin(glm::radians(cam.pitch)));	//y unit vector
		glm::vec3 k_hat = glm::vec3(glm::sin(glm::radians(cam.yaw)) * glm::cos(glm::radians(cam.pitch)), glm::sin(glm::radians(cam.pitch)), glm::cos(glm::radians(cam.yaw)) * glm::cos(glm::radians(cam.pitch)));	//z unit vector;
		//std::cout << glm::to_string(i_hat);
		rotation_matrix = glm::mat3(	i_hat.x, j_hat.x, k_hat.x,
										i_hat.y, j_hat.y, k_hat.y,
										i_hat.z, j_hat.z, k_hat.z);
	}
	hitobject hit_filter(glm::vec3 origin, std::vector<hitobject>& hits)
	{
		hitobject current = hits.at(0);
		for (int i = 0; i < hits.size(); i++)
		{
			if (glm::distance(origin, hits.at(i).hit_coord) < glm::distance(origin, current.hit_coord))
			{
				current = hits.at(i);
			}
		}
		return current;
	}
	bool ray_intersect(sphere sphereArray[], int& sphereArrayLen, ray& ray, std::vector<hitobject> &hits)
	{
		for (int i = 0; i < sphereArrayLen; i++)
		{
			raySphereIntersect(sphereArray[i], ray, hits);
		}
		rayPlaneIntersect(floor_height, ray, hits);

		if (hits.size() < 1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void raySphereIntersect(sphere &sphere, ray &ray, std::vector<hitobject> &hits)
	{
		float angle = glm::angle(ray.direction, glm::normalize(sphere.pos - ray.origin));
		if (angle < PI / 2)
		{
			float c = glm::distance(ray.origin, sphere.pos);	//distance to sphere
			float sinValue = sin(angle);					//sin value for radius check
			float radiusCheck = sinValue * c;
			if (radiusCheck <= sphere.radius)
			{
				hitobject sphere_hit;
				float IntersectDistance = sqrt(pow(sphere.radius, 2) - pow(radiusCheck, 2));
				float IntersectDistanceTemp = sqrt(pow(c, 2) - pow(radiusCheck, 2));
				sphere_hit.hit_coord = ray.direction * (IntersectDistanceTemp - IntersectDistance) + ray.origin;
				sphere_hit.normal = glm::normalize(sphere_hit.hit_coord - sphere.pos);
				sphere_hit.shader = sphere.shader;
				hits.push_back(sphere_hit);
			}
		}
	}
	void rayPlaneIntersect(float& baseplane_height, ray &ray, std::vector<hitobject>& hits)
	{
		hitobject plane_hit;
		if (ray.direction.y < 0 && ray.origin.y > baseplane_height || ray.direction.y > 0 && ray.origin.y < baseplane_height)
		{
			plane_hit.hit_coord.x = ((baseplane_height - ray.origin.y) / ray.direction.y) * ray.direction.x + ray.origin.x;
			plane_hit.hit_coord.y = ((baseplane_height - ray.origin.y) / ray.direction.y) * ray.direction.y + ray.origin.y;
			plane_hit.hit_coord.z = ((baseplane_height - ray.origin.y) / ray.direction.y) * ray.direction.z + ray.origin.z;
			plane_hit.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			plane_hit.shader.use_floor_texture = true;
			hits.push_back(plane_hit);
		}
	}
	png::rgb_pixel hdri_hit(png::image<png::rgb_pixel>& HDRI, glm::vec3 direction)
	{
		if (direction.y > 1)
		{
			direction.y = 1;
		}
		glm::vec2 HDRIAngle(atan2(-direction.x, -direction.z), asin(direction.y));
		if (HDRIAngle.x < 0)
		{
			HDRIAngle.x += 2 * PI;
		}

		int testx = (int)round((HDRI.get_width() / (2 * PI) - 0.5) * HDRIAngle.x);
		int testy = (HDRI.get_height() - 1) - (int)round((((HDRI.get_height() - 1) / 2 + HDRIAngle.y * (HDRI.get_height() - 1) / PI)));

		glm::ivec2 HDRIcoord((HDRI.get_width() - 1) - (int)round(((HDRI.get_width()-1) / (2 * PI)) * HDRIAngle.x), (HDRI.get_height()- 1) - (int)round((((HDRI.get_height()-1) / 2 + HDRIAngle.y * (HDRI.get_height()-1) / PI))));
		//std::cout << HDRIAngle.x * 180 / PI << std::endl;
		return HDRI[HDRIcoord.y][HDRIcoord.x];
	}
};