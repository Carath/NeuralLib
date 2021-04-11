#include <stdio.h>
#include <stdlib.h>

#include "drawing.h"


#define Y_BIG_STEP 50
#define INIT_OFFSET 100

#define BUTTON_OFFSET ((WINDOW_WIDTH - BUTTON_WIDTH) / 2)


SDL_Rect rect_button_clear =
{
	.x = BUTTON_OFFSET,
	.y = INIT_OFFSET,
	.w = BUTTON_WIDTH,
	.h = BUTTON_HEIGHT,
};

SDL_Rect rect_button_go =
{
	.x = BUTTON_OFFSET,
	.y = INIT_OFFSET,
	.w = BUTTON_WIDTH,
	.h = BUTTON_HEIGHT,
};

SDL_Rect rect_frame =
{
	.x = BUTTON_OFFSET - FRAME_WIDTH - SIZE_GAP,
	.y = INIT_OFFSET,
	.w = FRAME_WIDTH,
	.h = FRAME_HEIGHT,
};


// Main drawing function:
void drawing(void)
{
	if (!render_scene)
		return;

	if (redraw_scene)
	{
		// Window is cleared only when the entire scene needs to be redrawn. This causes flickering
		// when used with hardware acceleration, but assures that the user drawings have no lag:
		SDLA_ClearWindow(NULL);

		rect_button_go.y = rect_button_clear.y + BUTTON_HEIGHT + Y_BIG_STEP;
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

		redraw_scene = 0;
	}

	// Drawing the frame for user drawing:

	SDL_RenderCopy(renderer, texture_drawing, NULL, &rect_frame); // must be drawn in all cases.
	SDLA_SetDrawColor(255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect_frame);
}
