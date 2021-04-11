#include <stdio.h>
#include <stdlib.h>

#include "drawing.h"


#define X_MARGIN 50
#define Y_MARGIN 0
#define Y_STEP 25
#define Y_BIG_STEP 50

#define BUTTON_OFFSET ((WINDOW_WIDTH - BUTTON_WIDTH) / 2)
#define X_GUESS ((WINDOW_WIDTH + BUTTON_WIDTH) / 2 + SIZE_GAP)


SDL_Rect rect_button_clear =
{
	.x = BUTTON_OFFSET,
	.y = 0, // will be set during runtime.
	.w = BUTTON_WIDTH,
	.h = BUTTON_HEIGHT,
};

SDL_Rect rect_button_go =
{
	.x = BUTTON_OFFSET,
	.y = 0, // will be set during runtime.
	.w = BUTTON_WIDTH,
	.h = BUTTON_HEIGHT,
};

SDL_Rect rect_frame =
{
	.x = BUTTON_OFFSET - FRAME_WIDTH - SIZE_GAP,
	.y = 0, // will be set during runtime.
	.w = FRAME_WIDTH,
	.h = FRAME_HEIGHT,
};


void draw_neurons_line(SDL_Rect *rectangle, const NeuronLayer *layer,
	int neurons_number_to_draw, int line_start_Xpos, int start)
{
	rectangle -> x = line_start_Xpos;

	for (int i = 0; i < neurons_number_to_draw; ++i)
	{
		if (!clear_state)
		{
			// Neurons activation:

			int color_channel = convert(layer -> Output[start + i]);

			SDLA_SetDrawColor(0, color_channel, 0);

			SDL_RenderFillRect(renderer, rectangle);
		}

		// Neurons frame:

		SDLA_SetDrawColor(0, 128, 0);

		SDL_RenderDrawRect(renderer, rectangle);

		rectangle -> x += NEURON_SIZE;
	}
}


// Returns the Y-coordinate at which the network drawing ends:
int draw_network(const NeuralNetwork *network)
{
	NeuronLayer *layer = network -> Layers;

	SDL_Rect rectangle = {0, Y_MARGIN, NEURON_SIZE, NEURON_SIZE};

	for (int l = 0; l < network -> LayersNumber; ++l)
	{
		rectangle.y += Y_STEP;

		// Neurons per line, unless said line is the last one:
		const int neurons_per_line = MIN(layer -> NeuronsNumber, (WINDOW_WIDTH - 2 * X_MARGIN) / NEURON_SIZE);

		const int neurons_line_number = 1 + layer -> NeuronsNumber / neurons_per_line;

		const int line_start_Xpos = MAX((WINDOW_WIDTH - neurons_per_line * NEURON_SIZE) / 2, X_MARGIN);

		sprintf(layer_str, "Layer %d activations: %d neurons", l + 1, layer -> NeuronsNumber);

		SDLA_SlowDrawText(font_medium, &Yellow, SDLA_CENTERED, rectangle.y, layer_str);

		rectangle.y += 2 * Y_STEP;

		int neurons_to_draw_left = layer -> NeuronsNumber;

		for (int k = 0; k < neurons_line_number; ++k)
		{
			int neurons_to_draw_now = MIN(neurons_to_draw_left, neurons_per_line);

			draw_neurons_line(&rectangle, layer, neurons_to_draw_now, line_start_Xpos, k * neurons_per_line);

			neurons_to_draw_left -= neurons_per_line;

			rectangle.y += NEURON_SIZE;
		}

		++layer;

		if (l == network -> LayersNumber - 1)
		{
			rectangle.y -= 10;

			char *labels = MNIST_option ? "0 1 2 3 4 5 6 7 8 9 M" : "0 1 2 3 4 5 6 7 8 9";

			SDLA_SlowDrawText(font_small, &Yellow, line_start_Xpos + 2, rectangle.y, labels);
		}
	}

	return rectangle.y + TTF_FontHeight(font_small);
}


// Main drawing function:
void drawing(void)
{
	if (!render_scene)
		return;

	// For monitoring the drawing time of each frame:
	// double time_0 = get_time();

	if (redraw_scene)
	{
		// Window is cleared only when the entire scene needs to be redrawn. This causes flickering
		// when used with hardware acceleration, but assures that the user drawings have no lag:
		SDLA_ClearWindow(NULL);

		// Drawing the neural network activations. Needs to be done before drawing the buttons!

		rect_button_clear.y = draw_network(network_loaded) + Y_BIG_STEP;
		rect_button_go.y = rect_button_clear.y + BUTTON_HEIGHT;
		rect_frame.y = rect_button_go.y - FRAME_HEIGHT / 2;

		// Drawing the button 'Clear':

		SDLA_SetDrawColor(192, 0, 0);
		SDL_RenderFillRect(renderer, &rect_button_clear);

		SDLA_SetDrawColor(255, 255, 0);
		SDL_RenderDrawRect(renderer, &rect_button_clear);

		// Drawing the button 'Go':

		SDLA_SetDrawColor(0, 192, 0);
		SDL_RenderFillRect(renderer, &rect_button_go);

		SDLA_SetDrawColor(255, 255, 0);
		SDL_RenderDrawRect(renderer, &rect_button_go);

		// Text drawing:

		SDLA_SlowDrawText(font_medium, &Yellow, rect_button_clear.x + 12, rect_button_clear.y + 30, "Clear");
		SDLA_SlowDrawText(font_medium, &Yellow, rect_button_go.x + 25, rect_button_go.y + 30, "Go!");
		SDLA_SlowDrawText(font_medium, &Yellow, rect_frame.x - 5, rect_frame.y - 45, "Draw here!");
		SDLA_SlowDrawText(font_medium, &Yellow, X_GUESS, rect_button_clear.y, "Guess:");
		SDLA_SlowDrawText(font_medium, &Yellow, X_GUESS, rect_button_go.y, "Confidence:");

		if (!clear_state)
		{
			SDLA_SlowDrawText(font_big, &Yellow, X_GUESS + 30, rect_button_clear.y + 40, answer_str);
			SDLA_SlowDrawText(font_big, &Yellow, X_GUESS + 30, rect_button_go.y + 40, confidence_str);
		}

		redraw_scene = 0;
	}

	// Drawing the frame for user drawing:

	SDL_RenderCopy(renderer, texture_drawing, NULL, &rect_frame); // must be drawn in all cases.
	SDLA_SetDrawColor(255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect_frame);

	// double time_1 = get_time();
	// printf("Frame time: %.3f ms\n", 1000. * (time_1 - time_0));
}


// Maps values from the real line to [0, 255]:
inline int convert(Number value)
{
	if (value > 1.f)
		return 255;

	if (value < 0.f)
		return 0;

	return (int) (255.f * value);
}
