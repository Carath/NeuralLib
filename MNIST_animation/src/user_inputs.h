#ifndef USER_INPUTS_H
#define USER_INPUTS_H


#include "SDLA.h"
#include "animation_settings.h"


#define MNIST_ANSWER_SIZE 10 // 10 digits: from 0 to 9.
#define MNIST_IMAGE_WIDTH 28
#define MNIST_IMAGE_HEIGHT 28


extern int MNIST_option;

extern SDL_Renderer *renderer;

extern NeuralNetwork *network_loaded;
extern Inputs *image_input;

extern int pixels_number;
extern int pitch;
extern SDL_Texture *texture_drawing;
extern uint32_t *pixels_texture;
extern Number *pixels_converted;
extern Number *pixels_preprocessed;
extern Number *pixels_downscaled;

extern int quit;
extern int render_scene;
extern int redraw_scene;
extern int clear_state;

extern char answer_str[10];
extern char confidence_str[10];

extern CenteringOption centerOpt;

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
