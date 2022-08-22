#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <cmath>
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#include "sdk.h"
#include "ray.h"
#include "camera.h"
#include <vector>
#include <SDL2-2.0.22/include/SDL.h>


struct aimhitobject {
	glm::vec3 hit_coord = glm::vec3(0.0f);
	int sphere_index;
};


class aimtrain
{
public:
	std::vector<aimhitobject> hits;
	glm::mat3 rotation_matrix;
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
	int hit_filter(glm::vec3 origin, std::vector<aimhitobject>& hits)
	{
		int current = hits.at(0).sphere_index;
		aimhitobject current_hit = hits.at(0);
		for (int i = 0; i < hits.size(); i++)
		{
			if (glm::distance(origin, hits.at(i).hit_coord) < glm::distance(origin, current_hit.hit_coord))
			{
				current = hits.at(i).sphere_index;
			}
		}
		return current;
	}
	int ray_intersect(sphere sphereArray[], int& sphereArrayLen, ray ray, camera cam)
	{
		hits.resize(0);
		ray.direction = ray.direction * rotation_matrix;
		for (int i = 0; i < sphereArrayLen; i++)
		{
			raySphereIntersect(sphereArray[i], ray, hits, i);
		}

		if (hits.size() < 1)
		{
			return -1;
		}
		else
		{
			return hit_filter(cam.pos, hits);
		}
	}
	void raySphereIntersect(sphere &sphere, ray &ray, std::vector<aimhitobject> &hits, int index)
	{
		if (ray2AABBIntersect(ray, sphere))
		{
			float angle = glm::angle(ray.direction, glm::normalize(sphere.pos - ray.origin));
			if (angle < PI / 2)
			{
				float c = glm::distance(ray.origin, sphere.pos);	//distance to sphere
				float sinValue = sin(angle);					//sin value for radius check
				float radiusCheck = sinValue * c;
				if (radiusCheck <= sphere.radius)
				{
					aimhitobject sphere_hit;
					float IntersectDistance = sqrt(pow(sphere.radius, 2) - pow(radiusCheck, 2));
					float IntersectDistanceTemp = sqrt(pow(c, 2) - pow(radiusCheck, 2));
					sphere_hit.hit_coord = ray.direction * (IntersectDistanceTemp - IntersectDistance) + ray.origin;
					sphere_hit.sphere_index = index;
					hits.push_back(sphere_hit);
				}
			}
		}
	}
	bool rayAABBIntersect(ray& ray_in, sphere& sphere)
	{
		glm::vec3 high(sphere.pos.x + sphere.radius, sphere.pos.y + sphere.radius, sphere.pos.z + sphere.radius);
		glm::vec3 low(sphere.pos.x - sphere.radius, sphere.pos.y - sphere.radius, sphere.pos.z - sphere.radius);
		high -= ray_in.origin;
		low -= ray_in.origin;
		ray workray = ray_in;


		if ((high.x > 0 || low.x > 0) && ray_in.direction.x > 0 || (high.x < 0 || low.x < 0) && ray_in.direction.x < 0)
		{
			float multiplier = 1 / ray_in.direction.x;
			workray.direction *= multiplier;
			//workray.direction *= low.x;
			if ((workray.direction * high.x).y <= high.y && (workray.direction * high.x).z <= high.z || (workray.direction*low.x).y >= low.y && (workray.direction * low.x).z >= low.z)
			{
				return true;
			}
		}
		if ((high.y > 0 || low.y > 0) && ray_in.direction.y > 0 || (high.y < 0 || low.y < 0) && ray_in.direction.y < 0)
		{
			float multiplier = 1 / ray_in.direction.y;
			workray.direction *= multiplier;
			//workray.direction *= low.y;
			if ((workray.direction * high.y).x <= high.x && (workray.direction * high.y).z <= high.z || (workray.direction * low.y).x >= low.x && (workray.direction * low.y).z >= low.z)
			{
				return true;
			}
		}
		if ((high.z > 0 || low.z > 0) && ray_in.direction.z > 0 || (high.z < 0 || low.z < 0) && ray_in.direction.z < 0)
		{
			float multiplier = 1 / ray_in.direction.z;
			workray.direction *= multiplier;
			//workray.direction *= low.z;
			if ((workray.direction * high.z).x <= high.x && (workray.direction * high.z).y <= high.y || (workray.direction * low.z).x >= low.x && (workray.direction * low.z).y >= low.y)
			{
				return true;
			}
		}
		return false;
	}
	bool ray2AABBIntersect(ray& ray, sphere& sphere)
	{
		glm::vec3 high(sphere.pos + sphere.radius);
		glm::vec3 low(sphere.pos - sphere.radius);
		high -= ray.origin;
		low -= ray.origin;
		float multiplier;

		for (int i = 0; i < 3; i++)
		{
			if (0 > high[i] && ray.direction[i] > 0)
			{
				return false;
			}
			if (0 < low[i] && ray.direction[i] < 0)
			{
				return false;
			}
			else
			{
				multiplier = 1 / ray.direction[i];
					int val1 = (i + 1) % 3;
					int val2 = (i + 2) % 3;
					if ((ray.direction * multiplier * high[i])[val1] < high[val1] && (ray.direction * multiplier * high[i])[val2] < high[val2])
					{
						if ((ray.direction * multiplier * high[i])[val1] > low[val1] && (ray.direction * multiplier * high[i])[val2] > low[val2])
						{
							return true;
						}
					}
					if ((ray.direction * multiplier * low[i])[val1] < high[val1] && (ray.direction * multiplier * low[i])[val2] < high[val2])
					{
						if ((ray.direction * multiplier * low[i])[val1] > low[val1] && (ray.direction * multiplier * low[i])[val2] > low[val2])
						{
							return true;
						}
					}
			}
		}

		return false;
	}
};