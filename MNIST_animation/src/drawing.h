#ifndef DRAWING_H
#define DRAWING_H


#include "SDLA.h"
#include "animation_settings.h"


extern int MNIST_option;

extern SDL_Renderer *renderer;

extern TTF_Font *font_big;
extern TTF_Font *font_medium;
extern TTF_Font *font_small;

extern SDL_Color Yellow;

extern NeuralNetwork *network_loaded;

extern SDL_Texture *texture_drawing;

extern int render_scene;
extern int redraw_scene;
extern int clear_state;

extern char layer_str[50];
extern char answer_str[10];
extern char confidence_str[10];


void draw_neurons_line(SDL_Rect *rectangle, const NeuronLayer *layer,
	int neurons_number_to_draw, int line_start_pos, int start);


// Returns the Y-coordinate at which the network drawing ends:
int draw_network(const NeuralNetwork *network);


// Main drawing function:
void drawing(void);


// Maps values from the real line to [0, 255]:
int convert(Number value);


#endif
