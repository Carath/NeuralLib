#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // for standard, fixed size int types.
#include <byteswap.h>

#include "MNIST_loading.h"


Inputs* MNIST_createInputs(const char *directoryName, MNIST_mode mode)
{
	char images_filename[MAX_PATH_LENGTH];
	char labels_filename[MAX_PATH_LENGTH];

	if (mode == MNIST_TRAINING)
	{
		printf("Loading the MNIST training dataset.\n");
		sprintf(images_filename, "%strain-images-idx3-ubyte", directoryName);
		sprintf(labels_filename, "%strain-labels-idx1-ubyte", directoryName);
	}

	else // MNIST_VALIDATION
	{
		printf("Loading the MNIST validation dataset.\n");
		sprintf(images_filename, "%st10k-images-idx3-ubyte", directoryName);
		sprintf(labels_filename, "%st10k-labels-idx1-ubyte", directoryName);
	}

	int input_number, image_pixels;

	Number **questions = MNIST_loadImages(images_filename, &input_number, &image_pixels);
	Number **answers = MNIST_loadLabels(labels_filename, input_number);

	if (questions == NULL || answers == NULL)
	{
		printf("\nAn error happened while creating the MNIST inputs.\n\n");
		exit(EXIT_FAILURE);
	}

	return createInputs(input_number, image_pixels, MNIST_ANSWER_SIZE, questions, answers);
}


Number** MNIST_loadImages(const char *filename, int *input_number, int *image_pixels)
{
	FILE *file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("\nCannot find the file: '%s'.\n\n", filename);
		return NULL;
	}

	uint32_t head[4];

	if (fread(head, sizeof(uint32_t), 4, file) != 4)
	{
		printf("\nCannot retrieve the head of the file: '%s'.\n\n", filename);
		return NULL;
	}

	if (__bswap_32(head[0]) != 2051)
	{
		printf("\nInvalid magic number.\n\n");
		return NULL;
	}

	*input_number = __bswap_32(head[1]); // 60K / 10K
	int rows = __bswap_32(head[2]); // 28
	int cols = __bswap_32(head[3]); // 28

	*image_pixels = rows * cols; // 784
	size_t total_pixel_number = *input_number * *image_pixels;

	uint8_t *temp = (uint8_t*) calloc(total_pixel_number, sizeof(uint8_t));

	if (fread(temp, sizeof(uint8_t), total_pixel_number, file) != total_pixel_number) // faster by block.
	{
		printf("\nCannot retrieve some images values.\n\n");
		return NULL;
	}

	Number **images = createMatrix(*input_number, *image_pixels); // 'input_number' images of size 28 x 28, depth in [0, 255].

	for (int i = 0; i < *input_number; ++i)
	{
		for (int j = 0; j < *image_pixels; ++j)
			images[i][j] = (Number) temp[*image_pixels * i + j] / UINT8_MAX; // in [0, 1] for better learning.
	}

	free(temp);

	fclose(file);

	return images;
}


Number** MNIST_loadLabels(const char *filename, int input_number) // 'input_number' found while loading the images.
{
	FILE *file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("\nCannot find the file: '%s'.\n\n", filename);
		return NULL;
	}

	uint32_t head[2];

	if (fread(head, sizeof(uint32_t), 2, file) != 2)
	{
		printf("\nCannot retrieve the head of the file: '%s'.\n\n", filename);
		return NULL;
	}

	if (__bswap_32(head[0]) != 2049)
	{
		printf("\nInvalid magic number.\n\n");
		return NULL;
	}

	if (input_number != __bswap_32(head[1])) // 60K / 10K
	{
		printf("\nIncompatible input numbers.\n\n");
		return NULL;
	}

	uint8_t *temp = (uint8_t*) calloc(input_number, sizeof(uint8_t)); // intergers from 0 to 9, on 1 byte.

	if (fread(temp, sizeof(uint8_t), input_number, file) != input_number) // faster by block.
	{
		printf("\nCannot retrieve some labels values.\n\n");
		return NULL;
	}

	Number **labels = createMatrix(input_number, MNIST_ANSWER_SIZE);

	for (int i = 0; i < input_number; ++i)
		labels[i][temp[i]] = 1;

	free(temp);

	fclose(file);

	return labels;
}
