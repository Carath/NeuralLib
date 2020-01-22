#ifndef ANIMATION_SETTINGS_H
#define ANIMATION_SETTINGS_H


#include "../../NeuralLib.h"


#define USE_HARDWARE_ACCELERATION 0 // a value of 1 causes the scene to flicker,
// since said scene is not cleared each frame. In order to use hardware acceleration
// without issues, one would need to store the static parts of the scene in a dedicated texture.

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 800

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 100

#define FRAME_WIDTH 140
#define FRAME_HEIGHT 140

#define SIZE_GAP 100
#define DOT_SIZE 13
#define NEURON_SIZE 15

#define PRINTING_PIXELS 1 // for debugging.


typedef enum {RAW, RECENTERING} CenteringOption;


#endif
