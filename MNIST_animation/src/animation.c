#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "animation.h"
#include "drawing.h"
#include "user_inputs.h"


// Global variables:

int MNIST_option;

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *font_big;
TTF_Font *font_medium;
TTF_Font *font_small;

SDL_Color Yellow = {255, 255, 0, 255};

NeuralNetwork *network_loaded;
Inputs *image_input;

int pixels_number;
int pitch;
SDL_Texture *texture_drawing = NULL;
uint32_t *pixels_texture = NULL;
Number *pixels_converted = NULL;
Number *pixels_centered = NULL;
Number *pixels_resized = NULL;

int quit;
int render_scene;
int redraw_scene;
int clear_state;

char layer_str[50];
char answer_str[10];
char confidence_str[10];

// CenteringOption centerOpt = RAW;
CenteringOption centerOpt = RECENTERING; // better!


// Animating the recognition of the MNIST dataset:
void MNIST_animation(int modified_MNIST_option)
{
	////////////////////////////////////////////////////////////
	// Initializing SDLA - rendering:

	SDLA_Init(&window, &renderer, "Neural network animation", WINDOW_WIDTH, WINDOW_HEIGHT,
		USE_HARDWARE_ACCELERATION, SDLA_BLENDED);

	////////////////////////////////////////////////////////////
	// Loading fonts:

	const char font_name[] = "/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf";

	font_big = TTF_OpenFont(font_name, 50);
	font_medium = TTF_OpenFont(font_name, 25);
	font_small = TTF_OpenFont(font_name, 15);

	////////////////////////////////////////////////////////////
	// Loading a neural network:

	MNIST_option = modified_MNIST_option;

	int max_batch_size = 100;

	if (modified_MNIST_option)
		network_loaded = loadNetwork("saves/MNIST_modified", max_batch_size);
	else
		network_loaded = loadNetwork("saves/MNIST_learned", max_batch_size);

	if (network_loaded == NULL)
	{
		printf("\nThe neural network could not be properly loaded.\n\n");
		exit(EXIT_FAILURE);
	}

	int question_size = network_inputSize(network_loaded);
	int answers_size = network_outputSize(network_loaded);

	////////////////////////////////////////////////////////////
	// Creating an input to fill with the resized grayscale image:

	int questions_number = 1;

	Number **questions = createMatrix(questions_number, question_size);
	Number **answers = createMatrix(questions_number, answers_size);

	image_input = createInputs(questions_number, question_size, answers_size, questions, answers);

	// printInputs(image_input, INFOS);

	////////////////////////////////////////////////////////////
	// Creating the texture and pixels arrays:

	pixels_number = FRAME_WIDTH * FRAME_HEIGHT;
	pitch = FRAME_WIDTH * sizeof(uint32_t);

	texture_drawing = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
		FRAME_WIDTH, FRAME_HEIGHT);

	pixels_texture = (uint32_t*) calloc(pixels_number, sizeof(uint32_t));

	pixels_converted = (Number*) calloc(pixels_number, sizeof(Number));

	if (centerOpt == RECENTERING)
	{
		printf("\nRecentering activated.\n\n");

		pixels_centered = (Number*) calloc(pixels_number, sizeof(Number));
	}

	pixels_resized = (Number*) calloc(question_size, sizeof(Number));

	////////////////////////////////////////////////////////////
	// First drawing:

	clear_pixels_texture(pixels_texture, pixels_number);
	SDL_UpdateTexture(texture_drawing, NULL, pixels_texture, pitch);

	quit = 0;
	render_scene = 1;
	redraw_scene = 1;
	clear_state = 1;

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

	free(pixels_resized);
	free(pixels_centered);
	free(pixels_converted);
	free(pixels_texture);

	SDL_DestroyTexture(texture_drawing);

	freeInputs(&image_input);
	freeNetwork(&network_loaded);

	TTF_CloseFont(font_small);
	TTF_CloseFont(font_medium);
	TTF_CloseFont(font_big);

	SDLA_Quit();
}
