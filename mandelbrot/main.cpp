#include <SDL.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <complex>
#define RENDER_WIDTH 700 
#define RENDER_HEIGHT 700

int MAX_ITERATIONS = 500;

long double min = -2.0;
long double max = 2.0;

long double factor = 1.0;

long double map(long double value, long double in_min, long double in_max, long double out_min, long double out_max) {
	return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int main(int argc, char* argv[]) {
	

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(1440, 900, 0, &window, &renderer);
	SDL_RenderSetLogicalSize(renderer, RENDER_WIDTH, RENDER_HEIGHT);
	
	int count = 0;
	int scrolls = 0;
	while (1) {
		/*max -= 0.1 * factor;
		min += .15 * factor;
		factor *= .9;
		MAX_ITERATIONS += 5;
		*/
		if (SDL_PollEvent(&event) && event.type == SDL_MOUSEWHEEL) {
			if(event.wheel.y > 0){
				max *= .9;
				min *= .9;
				factor *= .9;
				MAX_ITERATIONS += 5;
				scrolls += 1;
			}
			if (event.wheel.y < 0) {
				max /= .9;
				min /= .9;
				factor /= .9349;
				MAX_ITERATIONS -= 5;
			}
		}
		if (SDL_PollEvent(&event) && event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_UP:
					max -= 0.1 * factor;
					min += .15 * factor;
					factor *= .9349;
					MAX_ITERATIONS += 5;
					break;
				case SDLK_DOWN:
					max += 0.1 * factor;
					min -= .15 * factor;
					factor /= .9349;
					MAX_ITERATIONS -= 5;
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				default:
					break;
			}
		}
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
			return 0;
		}
		if (GetKeyState('Q') && 0x8000) {
			return 0;
		}

		SDL_RenderPresent(renderer);
		for (int x = 0; x < RENDER_WIDTH; x++) {

			

			for (int y = 0; y < RENDER_HEIGHT; y++) {
				
				long double a = map(x, 0, RENDER_WIDTH, min, max);
				long double b = map(y, 0, RENDER_HEIGHT, min, max);
				int n = 0;

				/*
				long double a_init = a;
				long double b_init = b;

				
				for (int i = 0; i < MAX_ITERATIONS; i++) {

					long double a1 = a * a - b * b;
					long double b1 = 2 * a * b;

					a = a1 + a_init;
					b = b1 + b_init;

					if ((a + b) > 2) {
						break;
					}

					n++;
				}
				*/
				std::complex<double> coordinate(a, b);
				std::complex<double> original(a, b);
				for (int i = 0; i < MAX_ITERATIONS; i++) {

					coordinate = coordinate * coordinate + original;
					if (std::real(coordinate) * std::imag(coordinate) > 1)
						break;

					n++;
				}
				int bright = map(n, 0, MAX_ITERATIONS, 0, 255);

				if ((n == MAX_ITERATIONS)){
					bright = 0;
				}

				int red = map(bright * bright, 0, 6025, 0, 255);
				int green = bright;
				int blue = map(sqrt(bright), 0, sqrt(255), 0, 255);
					
				SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
				SDL_RenderDrawPoint(renderer, x, y);
				
			}
		}
		/*SDL_Surface* screenshot = SDL_GetWindowSurface(window);
		SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, screenshot->pixels, screenshot->pitch);
		std::string file = std::to_string(count) + ".bmp";
		SDL_SaveBMP(screenshot, file.c_str());
		SDL_FreeSurface(screenshot);
		count++;*/
	}

	return 0;
}