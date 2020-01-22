#ifndef LOADING_MNIST_H
#define LOADING_MNIST_H


#include "../../NeuralLib.h"


// Maximum number of characters for the directories path:
#define MAX_PATH_LENGTH 200

#define MNIST_ANSWER_SIZE 10 // 10 digits: from 0 to 9.
#define MNIST_IMAGE_WIDTH 28
#define MNIST_IMAGE_HEIGHT 28


typedef enum {MNIST_TRAINING, MNIST_VALIDATION} MNIST_mode;


Inputs* MNIST_createInputs(char *foldername, MNIST_mode mode);

Number** MNIST_loadImages(char *filename, int *input_number, int *image_pixels);

Number** MNIST_loadLabels(char *filename, int input_number);


#endif
