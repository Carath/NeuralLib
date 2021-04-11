#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "animation.h"
#include "drawing.h"
#include "user_inputs.h"


// #define DRAWING_SAVE_DIR "some_path"

// Global variables:

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *font_big;
TTF_Font *font_medium;
TTF_Font *font_small;

SDL_Color Yellow = {255, 255, 0, 255};

int pixels_number;
int pitch;
SDL_Texture *texture_drawing = NULL;
uint32_t *pixels_texture = NULL;

int quit;
int render_scene;
int redraw_scene;


// Animating the drawing and saving of symbols.
void animation(void)
{
	////////////////////////////////////////////////////////////
	// Initializing SDLA - rendering:

	SDLA_Init(&window, &renderer, "Drawing symbols", WINDOW_WIDTH, WINDOW_HEIGHT,
		USE_HARDWARE_ACCELERATION, SDLA_BLENDED);

	////////////////////////////////////////////////////////////
	// Loading fonts:

	const char font_name[] = "/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf";

	font_big = TTF_OpenFont(font_name, 50);
	font_medium = TTF_OpenFont(font_name, 25);
	font_small = TTF_OpenFont(font_name, 15);

	////////////////////////////////////////////////////////////
	// Creating the texture and pixels arrays:

	pixels_number = FRAME_WIDTH * FRAME_HEIGHT;
	pitch = FRAME_WIDTH * sizeof(uint32_t);

	texture_drawing = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
		FRAME_WIDTH, FRAME_HEIGHT);

	pixels_texture = (uint32_t*) calloc(pixels_number, sizeof(uint32_t));

	////////////////////////////////////////////////////////////
	// First drawing:

	clear_pixels_texture(pixels_texture, pixels_number);
	SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);

	quit = 0;
	render_scene = 1;
	redraw_scene = 1;

	// int frame_index = 0; // for monitoring the frames rendering.

	while (!quit)
	{
		input_control(); // causes a 10 ms wait.

		drawing();

		// Rendering:
		if (render_scene)
		{
			// printf("rendering: %d\n", frame_index);
			// ++frame_index;

			SDL_RenderPresent(renderer);

			render_scene = 0;
		}
	}

	// Freeing everything:

	free(pixels_texture);
	SDL_DestroyTexture(texture_drawing);

	TTF_CloseFont(font_small);
	TTF_CloseFont(font_medium);
	TTF_CloseFont(font_big);

	SDLA_Quit();
}
