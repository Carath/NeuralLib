#define _GNU_SOURCE // to use math constants like Pi.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "user_inputs.h"
#include "geometry.h"


#define MAX_BUFFER_SIZE 1024

static float Buffer_x[MAX_BUFFER_SIZE];
static float Buffer_y[MAX_BUFFER_SIZE];

static int pixel_rank = 0;


// Framework for user's inputs:
void input_control(void)
{
	SDL_Event event;

	SDL_WaitEvent(&event); // 10 ms wait.

	if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		quit = 1;

	// Get the mouse coordinates if the left button is down.
	// N.B: Dont get said coordinates with the 'SDL_GetMouseState' function, as it is inaccurate.
	// Moreover, using said function with NULL args may causes fast mouse clics to go unnoticed...
	else if ((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION)
		&& event.button.button == SDL_BUTTON_LEFT)
	{
		// Accurate coordinates:
		int mouse_x = event.motion.x;
		int mouse_y = event.motion.y;

		// User drawing:
		if (is_in_rect(&rect_frame, mouse_x, mouse_y))
		{
			// SDL_Log("Mouse Button 1 (left) is pressed: %3d, %3d", mouse_x, mouse_y);

			float x_ratio = (float) (mouse_x - rect_frame.x) / rect_frame.w;
			float y_ratio = (float) (mouse_y - rect_frame.y) / rect_frame.h;
			// printf("=> r_x: %.3f, r_y: %.3f\n", x_ratio, y_ratio);

			float r_dist_x = pixel_rank == 0 ? 0.f : fabsf(x_ratio - Buffer_x[pixel_rank - 1]);
			float r_dist_y = pixel_rank == 0 ? 0.f : fabsf(y_ratio - Buffer_y[pixel_rank - 1]);

			int check_dist = pixel_rank == 0 || r_dist_x > RATIO_THRESHOLD || r_dist_y > RATIO_THRESHOLD;

			if (pixel_rank < MAX_BUFFER_SIZE && check_dist)
			{
				printf("=> i: %3d, r_x: %.3f, r_y: %.3f\n", pixel_rank, x_ratio, y_ratio);
				Buffer_x[pixel_rank] = x_ratio;
				Buffer_y[pixel_rank] = y_ratio;
				++pixel_rank;
			}

			fill_texture_drawing(mouse_x, mouse_y, DOT_SIZE);
			SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);

			render_scene = 1;
			redraw_scene = 0;
		}


		// "Go" button -> printing the neural network response:
		else if (is_in_rect(&rect_button_go, mouse_x, mouse_y))
		{
			printf("\n");
			clear_pixels_texture(pixels_texture, pixels_number);

			for (int i = 0; i < pixel_rank; ++i)
			{
				if (i > 0 && i < pixel_rank - 1)
				{
					Vector2D u = {
						.x = Buffer_x[i] - Buffer_x[i - 1],
						.y = Buffer_y[i] - Buffer_y[i - 1]
					};
					Vector2D v = {
						.x = Buffer_x[i + 1] - Buffer_x[i],
						.y = Buffer_y[i + 1] - Buffer_y[i]
					};

					float theta = angle(&u, &v); // in [-pi, pi[
					printf("i: %3d, theta: %8.3f°\n", i, theta * 180.f / (float) M_PI);
				}

				// Adding 0.5f to get the closest integer:
				int mouse_x = (int) (0.5f + Buffer_x[i] * rect_frame.w + rect_frame.x);
				int mouse_y = (int) (0.5f + Buffer_y[i] * rect_frame.h + rect_frame.y);

				// printf("=> i: %3d, r_x: %.3f, r_y: %.3f\n", i, Buffer_x[i], Buffer_y[i]);
				// printf("=> i: %3d, mouse_x: %3d, mouse_y: %3d\n", i, mouse_x, mouse_y);

				fill_texture_drawing(mouse_x, mouse_y, SMALL_DOT_SIZE);
			}

			SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);

			pixel_rank = 0;
			render_scene = 1;
			redraw_scene = 1;
		}


		// "Clear" button:
		else if (is_in_rect(&rect_button_clear, mouse_x, mouse_y))
		{
			printf("\n");
			pixel_rank = 0;

			memset(Buffer_x, 0, MAX_BUFFER_SIZE * sizeof(float));
			memset(Buffer_y, 0, MAX_BUFFER_SIZE * sizeof(float));

			clear_pixels_texture(pixels_texture, pixels_number);
			SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);

			render_scene = 1;
			redraw_scene = 1;
		}
	}
}


// Fill the texture with the current dot:
void fill_texture_drawing(int mouse_x, int mouse_y, int used_dot_size)
{
	// Top left corner of the dot:
	int Xcorner = mouse_x - used_dot_size / 2 - rect_frame.x;
	int Ycorner = mouse_y - used_dot_size / 2 - rect_frame.y;

	int Xmin = MAX(0, Xcorner);
	int Xmax = MIN(FRAME_WIDTH - 1, Xcorner + used_dot_size - 1);

	int Ymin = MAX(0, Ycorner);
	int Ymax = MIN(FRAME_HEIGHT - 1, Ycorner + used_dot_size - 1);

	// Filling the pixels with a black dot:
	for (int i = Ymin; i <= Ymax; ++i)
	{
		for (int j = Xmin; j <= Xmax; ++j)
			pixels_texture[i * FRAME_WIDTH + j] = 0;
	}

	SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);
}


// Check if the given coordinates are inside the given rectangle:
inline int is_in_rect(const SDL_Rect *rect, int x, int y)
{
	return rect -> x <= x && x <= (rect -> x + rect -> w) && rect -> y <= y && y <= (rect -> y + rect -> h);
}


// Fills with white pixels:
inline void clear_pixels_texture(uint32_t *pixels, int pixels_number)
{
	memset(pixels, 255, pixels_number * sizeof(uint32_t));
}
