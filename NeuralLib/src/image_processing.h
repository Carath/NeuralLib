#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H


#include "settings.h"


typedef enum {MAX_POOLING, AVERAGE_POOLING} PoolingMode;

typedef enum {PIXEL_MAX, PIXEL_SUM} UpscalingOption;

typedef struct
{
	int rowMin;
	int rowMax;
	int colMin;
	int colMax;
} Box;


// Print in the console a grayscale image contained in a 1-dimensional array:
void printGrayscaleImage(const Number *image, int width, int height);


void pooling(Number *output, const Number *input, int output_width, int output_height,
	int input_width, int input_height, PoolingMode poolmeth);


// Find the hull of the given image, assuming it has a white background:
void find_hull(const Number *image, int width, int height, Box *box);


// 'src' and 'dest' must not overlap!
void recenter(Number *dest, const Number *src, int width, int height);


// 'src' and 'dest' must not overlap!
void slide(Number *dest, const Number *src, int width, int height, int delta_row, int delta_col);


// Resize and recenter the image, with the given margin ratio, which must be in ]0, 1[.
// 'src' and 'dest' must not overlap!
void resize(Number *dest, const Number *src, int width, int height, float marginRatio, UpscalingOption option);


#endif
