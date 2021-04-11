#ifndef DRAWING_H
#define DRAWING_H


#include "SDLA.h"
#include "animation_settings.h"


extern SDL_Renderer *renderer;

extern TTF_Font *font_big;
extern TTF_Font *font_medium;
extern TTF_Font *font_small;

extern SDL_Color Yellow;

extern SDL_Texture *texture_drawing;

extern int render_scene;
extern int redraw_scene;


// Main drawing function:
void drawing(void);


#endif
