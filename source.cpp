#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include "png++/png.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "sdk.h"
#include "ray.h"
#include "camera.h"
#include "world.h"
#include "SDL2-2.0.22/include/SDL.h"
#include "SDL2-2.0.22/include/SDL_syswm.h"
#include <thread>
#include <Windows.h>
#include "ray_sphere_hit.h"
#include "SDL2-2.0.22/include/SDL_image.h"

float geradenKonstante = 1;

#define PI 3.14159265358979323846
int const ResWidth = 800;//1280;
int const ResHeight = 450;//720;
float const Fov = 90;


#undef main



void test(int& a)
{
	a++;
}
bool running = true;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::cout << "testtest" << std::endl;
	switch (uMsg)
	{
	case WM_QUIT:
		running = false;
	case WM_INPUT:
	{

	}

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



int main(int argc, char* argv[])
{

	png::image<png::rgb_pixel> image(ResWidth, ResHeight);
	
	//glm::vec3 direction = glm::reflect(glm::normalize(glm::vec3(0, -.1, 1)), glm::normalize(glm::vec3(0, -1, -1)));

	//std::cout << (1 / tan(cam.FOV / 360 * glm::pi<float>())) * (image.get_width() / 2);

	SDL_Window* window;                    // Declare a pointer

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
	window = SDL_CreateWindow(
		"RTRT",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		ResWidth,                               // width, in pixels
		ResHeight ,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	SDL_UpdateWindowSurface(window);
	// The window is open: could enter program loop here (see SDL_PollEvent())


	SDL_SetRelativeMouseMode(SDL_TRUE);
	camera cam;
	cam.FOV = 90;

	float camrotation = -6;

	world world0;
	world world1;
	world world2;
	world world3;
	world world4;
	world world5;
	world world6;
	world world7;
	world world8;
	world world9;
	world world10;
	world world11;
	aimtrain aimtrain;

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
	sphere1.shader.roughness = 1;

	sphere2.pos.x = -2.0f;
	sphere2.pos.y = 2.0f;
	sphere2.pos.z = 2.0f;
	sphere2.radius = 2.0f;
	sphere2.shader.roughness = 1;

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

	//sphere spheres[] = {sphere0, sphere3, sphere4};
	sphere spheres[3];
	float sphere_rad = 1.0;
	for (int i = 0; i < 3; i++)
	{
		sphere sphere;
		sphere.pos.z = 10;
		sphere.pos.x = (((float)rand()) / (float)RAND_MAX) * 6 - 3;
		sphere.pos.y = (((float)rand()) / (float)RAND_MAX) * 6 - 3;
		sphere.shader.roughness = 1;
		sphere.radius = sphere_rad;
		spheres[i] = sphere;
	}



	int spheresLen = sizeof(spheres) / sizeof(spheres[0]);
	png::image< png::rgb_pixel > HDRI("assets/studio_country_hall_4k.png");
	png::image< png::rgb_pixel > base_texture("assets/cobblestone/pavement_cobble_albedo.png");




	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED);
	SDL_Surface* crosshairsurf = SDL_LoadBMP("assets/20pxcrosshair.bmp");
	//SDL_Texture* crosshairtexture = SDL_CreateTextureFromSurface(renderer, crosshairsurf);//IMG_LoadTexture(renderer, "render.bmp");
	SDL_Rect crosshair;
	crosshair.w = 20;
	crosshair.h = 20;
	crosshair.x = 390;
	crosshair.y = 215;
	SDL_Rect crosshairsource;
	crosshairsource.w = 20;
	crosshairsource.h = 20;
	crosshairsource.x = 0;
	crosshairsource.y = 0;

	SDL_Surface* wsurface = SDL_GetWindowSurface(window);
	//(SDL_PixelFormat*)vfmt = screen->format;
	//(Uint8)bytes_per_pixel = vfmt->BytesPerPixel;
	//(void*)pixel_ptr = screen->pixels + y * screen->pitch + x * bytes_per_pixel;
	//Uint8 * pixel_ptr1 = pixel_ptr0 + 1;
	//Uint8 * pixel_ptr2 = pixel_ptr1 + 1;
	//pixel_ptr2++;
	//pixel_ptr2++;
	//SDL_LockSurface(wsurface);

	//*pixel_ptr = 4;

	//SDL_memset(wsurface->pixels, 0, wsurface->pitch);

	//std::cout << "size: " << std::to_string(*pixel_ptr) << std::endl;


	int mouse_x;
	int mouse_y;
	bool mouseclick = false;

	while(running)
	{
		mouseclick = false;
		mouse_x = 0;
		mouse_y = 0;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {  // poll until all events are handled!
			if (event.type == SDL_QUIT) {
				SDL_Log("Program quit after %i ticks", event.quit.timestamp);
				running = false;
				break;
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				mouse_x = event.motion.xrel;
				mouse_y = -event.motion.yrel;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				mouseclick = true;
			}
		}
		//SDL_WarpMouseInWindow(window,
			//400, 225);

		//GetRawInputData((HRAWINPUT)lParam)
		SDL_GetRelativeMouseState(&mouse_x, &mouse_y);

		const Uint8* kb = SDL_GetKeyboardState(NULL);

		Uint8 move_right = 1*(kb[SDL_SCANCODE_A] > 0);
		Uint8 move_left  = 1*(kb[SDL_SCANCODE_D] > 0);
		Uint8 move_forward = 1 * (kb[SDL_SCANCODE_W] > 0);
		Uint8 move_backward = 1 * (kb[SDL_SCANCODE_S] > 0);
		int move_rightleft = move_left - move_right;
		int move_frontback = move_forward - move_backward;

		//camrotation += (move_left - move_right) * 3;
		cam.yaw += mouse_x * 0.022;
		cam.pitch -= mouse_y * 0.022;
		glm::vec3 direction(glm::sin(glm::radians(cam.yaw)), 0, glm::cos(glm::radians(cam.yaw)));
		glm::vec3 direction_r(glm::cos(glm::radians(cam.yaw)), 0, -glm::sin(glm::radians(cam.yaw)));
		cam.pos += direction * float(move_frontback);
		cam.pos += direction_r * float(move_rightleft);
		//camrotation -= 1;
		//cam.yaw = camrotation;
		//std::cout << std::to_string(cam.pos.x) + " " + std::to_string(cam.pos.z) << std::endl;
		auto start = std::chrono::high_resolution_clock::now();
		//SDL_BlitSurface(crosshairsurf, NULL, wsurface, NULL);
		png::image<png::rgb_pixel> imageout = image;
		//auto startrender = std::chrono::high_resolution_clock::now();
		std::thread thread1(&world::render_to_SDL_thread_x, &world0, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 0);
		std::thread thread2(&world::render_to_SDL_thread_x, &world1, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 1);
		std::thread thread3(&world::render_to_SDL_thread_x, &world2, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 2);
		std::thread thread4(&world::render_to_SDL_thread_x, &world3, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 3);
		std::thread thread5(&world::render_to_SDL_thread_x, &world4, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 4);
		std::thread thread6(&world::render_to_SDL_thread_x, &world5, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 5);
		std::thread thread7(&world::render_to_SDL_thread_x, &world6, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 6);
		std::thread thread8(&world::render_to_SDL_thread_x, &world7, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 7);
		std::thread thread9(&world::render_to_SDL_thread_x, &world8, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 8);
		std::thread thread10(&world::render_to_SDL_thread_x, &world9, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 9);
		std::thread thread11(&world::render_to_SDL_thread_x, &world10, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 10);
		std::thread thread12(&world::render_to_SDL_thread_x, &world11, spheres, spheresLen, std::ref(HDRI), std::ref(base_texture), cam, wsurface, 12, 11);
		ray aim_ray;

		aim_ray.direction = glm::vec3(0, 0, 1);
		aim_ray.origin = cam.pos;
		aimtrain.generate_rotationmatrix(cam);

		int index = aimtrain.ray_intersect(spheres, spheresLen, aim_ray, cam);
		//std::cout << "                                 " << index << std::endl;
		bool distcheck = false;
		if (index >= 0 && mouseclick == true)
		{
		while (!distcheck)
		{

				spheres[index].pos.x = (((float)rand()) / (float)RAND_MAX) * 6 - 3;
				spheres[index].pos.y = (((float)rand()) / (float)RAND_MAX) * 6 - 3;
				if (glm::distance(spheres[index].pos, spheres[(index + 1) % spheresLen].pos) > 2*sphere_rad && glm::distance(spheres[index].pos, spheres[(index + 2) % spheresLen].pos) > 2 * sphere_rad)
				{
					distcheck = true;
				}
		}
		}
		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();
		thread5.join();
		thread6.join();
		thread7.join();
		thread8.join();
		thread9.join();
		thread10.join();
		thread11.join();
		thread12.join();
		//SDL_UnlockSurface(wsurface);
		//SDL_BlitSurface(crosshairsurf, NULL, wsurface, &crosshair);
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		long long milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(
			elapsed).count();
		std::cout << "rendering time: " << std::fixed << std::setprecision(2) << (float)(milliseconds / 1000.0) << "ms" << std::endl;
		SDL_UpdateWindowSurface(window);
	}
	SDL_DestroyWindow(window);
	std::cout << "test" << std::endl;

}