#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "user_inputs.h"


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
			// SDL_Log("Mouse Button 1 (left) is pressed: %d, %d", mouse_x, mouse_y);

			// Top left corner of the dot:
			int Xcorner = mouse_x - DOT_SIZE / 2 - rect_frame.x;
			int Ycorner = mouse_y - DOT_SIZE / 2 - rect_frame.y;

			int Xmin = MAX(0, Xcorner);
			int Xmax = MIN(FRAME_WIDTH - 1, Xcorner + DOT_SIZE - 1);

			int Ymin = MAX(0, Ycorner);
			int Ymax = MIN(FRAME_HEIGHT - 1, Ycorner + DOT_SIZE - 1);

			// Filling the pixels with a black dot:
			for (int i = Ymin; i <= Ymax; ++i)
			{
				for (int j = Xmin; j <= Xmax; ++j)
					pixels_texture[i * FRAME_WIDTH + j] = 0;
			}

			SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);

			render_scene = 1;
			redraw_scene = 0;
			// 'clear_state' untouched.
		}


		// "Clear" button:
		else if (is_in_rect(&rect_button_clear, mouse_x, mouse_y))
		{
			clear_pixels_texture(pixels_texture, pixels_number);

			SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);

			render_scene = 1;
			redraw_scene = 1;
			clear_state = 1;
		}


		// "Go" button -> printing the neural network response:
		else if (is_in_rect(&rect_button_go, mouse_x, mouse_y))
		{
			// Preprocessing:

			// Converting to Numbers:
			for (int i = 0; i < pixels_number; ++i)
				pixels_converted[i] = (uint32_t) pixels_texture[i] + 1; // Overflowing, so white = 0 and black = 1.

			Number *pixels_used = pixels_converted; // default.

			if (centerOpt == RECENTERING)
			{
				recenter(pixels_centered, pixels_converted, FRAME_WIDTH, FRAME_HEIGHT);

				pixels_used = pixels_centered;
			}

			pooling(pixels_resized, pixels_used, MNIST_IMAGE_WIDTH, MNIST_IMAGE_HEIGHT,
				FRAME_WIDTH, FRAME_HEIGHT, AVERAGE_POOLING);

			if (PRINTING_PIXELS)
			{
				printf("\npixels_resized:\n\n");
				printGrayscaleImage(pixels_resized, MNIST_IMAGE_WIDTH, MNIST_IMAGE_HEIGHT);
			}

			// Recognition:

			int question_size = network_inputSize(network_loaded);
			int answer_size = network_outputSize(network_loaded);

			Number *the_question = image_input -> Questions[0], *the_answer = image_input -> Answers[0];

			copyVector(the_question, pixels_resized, question_size);

			prediction(network_loaded, image_input);

			// If modified MNIST inputs are used, don't search a number in the last index:
			int search_span = modified_MNIST_option ? answer_size - 1 : answer_size;

			Number confidence_level;

			int max_value_index = findMostProbable(the_answer, search_span, &confidence_level);

			sprintf(answer_str, "%d", labels[max_value_index]);
			sprintf(confidence_str, "%.0f %%", 100. * confidence_level);

			// Printing the answer in the console:
			printf("Activation values:\n\n");
			printVector(the_answer, answer_size);

			render_scene = 1;
			redraw_scene = 1;
			clear_state = 0;
		}
	}
}


// Check if the given coordinates are inside the given rectangle:
inline int is_in_rect(SDL_Rect *rect, int x, int y)
{
	return rect -> x <= x && x <= (rect -> x + rect -> w) && rect -> y <= y && y <= (rect -> y + rect -> h);
}


// Fills with white pixels:
inline void clear_pixels_texture(uint32_t *pixels, int pixels_number)
{
	memset(pixels, 255, pixels_number * sizeof(uint32_t));
}
