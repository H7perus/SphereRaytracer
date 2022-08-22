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
#include "hitobject.h"
#include <vector>
#include <SDL2-2.0.22/include/SDL.h>





class world
{
public:
	float focal_length;
	float floor_texture_scale = 1024;
	glm::mat3 rotation_matrix;
	glm::vec3 pixel_direction;
	bool raybounce;
	float floor_height = -6.0f;
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
	void render_to_SDL(sphere sphereArray[], int& sphereArrayLen, png::image<png::rgb_pixel>& HDRI, png::image<png::rgb_pixel>& BaseTexture, camera& cam, SDL_Renderer* renderer)
	{
		generate_rotationmatrix(cam);
		int width;
		int height;

		SDL_GetRendererOutputSize(renderer,
			&width, &height);

		//std::cout << width;

		

		//std::cout << glm::to_string(rotation_matrix);

		for (png::uint_32 y = 0; y < height; y++) {
			//std::cout << y << std::endl;
			for (png::uint_32 x = 0; x < width; x++) {
				pixel_direction = glm::vec3(float(x) - width / 2 + 0.5f, -float(y) + height / 2 + 0.5f, float(cam.focal_length));
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
					//bool did_intersect = ray2AABBIntersect(ray, sphereArray[0]);
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
						//raybounce = false;
						//pixel_colour = png::rgb_pixel(0, 0, 0);
					}
				}
				SDL_SetRenderDrawColor(renderer, pixel_colour.red, pixel_colour.green, pixel_colour.blue, 255);
				SDL_RenderDrawPoint(renderer, x, y);

				//std::cout << image[y][x].red;
				//std::cout << "test111";
			}
		}
		SDL_RenderPresent(renderer);
	}
	void render_to_SDL_thread(sphere sphereArray[], int sphereArrayLen, png::image<png::rgb_pixel>& HDRI, png::image<png::rgb_pixel>& BaseTexture, camera cam, png::image<png::rgb_pixel>& image, uint8_t thread_amount, uint8_t thread_number)
	{
		auto startdraw = std::chrono::high_resolution_clock::now();
		generate_rotationmatrix(cam);
		int width, height;
		glm::ivec2 start;
		int width_scale = image.get_width() / 2;
		int height_scale = image.get_height() / 2; //image.get_height() / 4;

		if (thread_amount == 4)
		{
			width = width_scale;
			height = height_scale;
			start = glm::ivec2(thread_number % 2 * width, (thread_number > 1) * height);
		}
		if (thread_amount == 6)
		{
			width = width_scale;
			height = height_scale / 1.5;
			start = glm::ivec2(thread_number % 2 * width, 1*(thread_number > 1 && thread_number < 4) * height + 2 * (thread_number > 3) * height);
		}
		if (thread_amount == 8)
		{
			width = width_scale / 2;
			height = height_scale;
			start = glm::ivec2(thread_number % 4 * width, (thread_number > 3) * height);
		}


		int floor_texture_scale = BaseTexture.get_width();


		//std::cout << glm::to_string(rotation_matrix);

		for (png::uint_32 y = start.y; y < start.y + height; y++) {
			//std::cout << y << std::endl;
			for (png::uint_32 x = start.x; x < start.x + width; x++) {
				pixel_direction = glm::vec3(float(x) - width_scale + 0.5f, -float(y) + height_scale + 0.5f, float(cam.focal_length));
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
					//bool did_intersect = ray2AABBIntersect(ray, sphereArray[0]);
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
						//raybounce = false;
						//pixel_colour = png::rgb_pixel(0, 0, 0);
					}
				}
				image[y][x] = pixel_colour;
				//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startdraw).count() << std::endl;

				//image[y][x].red;
				//std::cout << "test111";
			}
		}
	std::cout << "thread " << std::to_string(thread_number) << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startdraw).count() << "ms" << std::endl;
		//SDL_RenderPresent(renderer);
	}
	void render_to_SDL_thread_x(sphere sphereArray[], int sphereArrayLen, png::image<png::rgb_pixel>& HDRI, png::image<png::rgb_pixel>& BaseTexture, camera cam, SDL_Surface* wsurface, uint8_t thread_amount, uint8_t thread_number)
	{
		//auto startdraw = std::chrono::high_resolution_clock::now();
		generate_rotationmatrix(cam);
		int width, height;
		glm::ivec2 start;
		int width_scale = wsurface->w / 2;
		int height_scale = wsurface->h / 2;
		if (thread_amount == 1)
		{
			width = wsurface->w;
			height = wsurface->h;
			start = glm::ivec2(0, 0);
		}

		if (thread_amount == 4)
		{
			width = width_scale;
			height = height_scale;
			start = glm::ivec2(thread_number % 2 * width, (thread_number > 1) * height);
		}
		if (thread_amount == 6)
		{
			width = width_scale;
			height = height_scale / 1.5;
			start = glm::ivec2(thread_number % 2 * width, 1 * (thread_number > 1 && thread_number < 4) * height + 2 * (thread_number > 3) * height);
		}
		if (thread_amount == 8)
		{
			width = width_scale / 2;
			height = height_scale;
			start = glm::ivec2(thread_number % 4 * width, (thread_number > 3) * height);
		}
		if (thread_amount == 12)
		{
			width = width_scale / 2;
			height = height_scale / 1.5;
			start = glm::ivec2(thread_number % 4 * width, 1 * (thread_number > 3 && thread_number < 8) * height + 2 * (thread_number > 7) * height);
		}


		int floor_texture_scale = BaseTexture.get_width();

		Uint8 bytes_per_pixel = wsurface->format->BytesPerPixel;
		

		//std::cout << glm::to_string(rotation_matrix);

		for (png::uint_32 y = start.y; y < start.y + height; y++) {
			//std::cout << y << std::endl;
			for (png::uint_32 x = start.x; x < start.x + width; x++) {
				pixel_direction = glm::vec3(float(x) - width_scale + 0.5f, -float(y) + height_scale + 0.5f, float(cam.focal_length));
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
					//bool did_intersect = ray2AABBIntersect(ray, sphereArray[0]);
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
						//raybounce = false;
						//pixel_colour = png::rgb_pixel(0, 0, 0);
					}
				}
				//image[y][x] = pixel_colour;
				Uint8* pixel_ptr = (Uint8*)wsurface->pixels + y * wsurface->pitch + x * bytes_per_pixel;
				*pixel_ptr = pixel_colour.blue;
				pixel_ptr++;
				*pixel_ptr = pixel_colour.green;
				pixel_ptr++;
				*pixel_ptr = pixel_colour.red;

				//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startdraw).count() << std::endl;

				//image[y][x].red;
				//std::cout << "test111";
			}
		}
		//std::cout << "thread " << std::to_string(thread_number) << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startdraw).count() << "ms" << std::endl;
		//SDL_RenderPresent(renderer);
	}
	void test(png::image<png::rgb_pixel>& image)
	{
		for (int x = 0; x < image.get_width(); x++)
		{
			for (int y = 0; y < image.get_height(); y++)
			{
				image[y][x] = png::rgb_pixel(255, 0, 0);
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
	}
	void rayPlaneIntersect(float& baseplane_height, ray &ray, std::vector<hitobject> &hits)
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