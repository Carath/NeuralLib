#ifndef ANIMATION_SETTINGS_H
#define ANIMATION_SETTINGS_H


#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) < (y) ? (y) : (x))
#endif


#define USE_HARDWARE_ACCELERATION 0 // a value of 1 causes the scene to flicker,
// since said scene is not cleared each frame. In order to use hardware acceleration
// without issues, one would need to store the static parts of the scene in a dedicated texture.

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 100

#define FRAME_WIDTH 150
#define FRAME_HEIGHT 150

#define SIZE_GAP 100
#define DOT_SIZE 10
#define SMALL_DOT_SIZE 3

// #define PRINTING_PIXELS 1 // for debugging.

#define RATIO_THRESHOLD 0.03f


#endif
