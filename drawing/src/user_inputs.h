#ifndef USER_INPUTS_H
#define USER_INPUTS_H


#include "SDLA.h"
#include "animation_settings.h"


extern SDL_Renderer *renderer;

extern int pixels_number;
extern int pitch;
extern SDL_Texture *texture_drawing;
extern uint32_t *pixels_texture;

extern int quit;
extern int render_scene;
extern int redraw_scene;

extern SDL_Rect rect_button_clear;
extern SDL_Rect rect_button_go;
extern SDL_Rect rect_frame;


// Framework for user's inputs:
void input_control(void);


// Fill the texture with the current dot:
void fill_texture_drawing(int mouse_x, int mouse_y, int used_dot_size);


// Check if the given coordinates are inside the given rectangle:
int is_in_rect(const SDL_Rect *rect, int x, int y);


// Fills with white pixels:
void clear_pixels_texture(uint32_t *pixels, int pixels_number);


#endif
