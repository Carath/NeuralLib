#include <stdio.h>
#include <stdlib.h>

#include "MNIST_learning.h"


// Dataset found at: http://yann.lecun.com/exdb/mnist/
// Files must be extracted.

// Specify here the path to uncompressed MNIST files:
#define MNIST_PATH "/home/mypc/Documents/MNIST_samples/"


int main(int argc, char **argv)
{
	// // Printing the first 'number_to_print' inputs of the MNIST training dataset:
	// MNIST_print(MNIST_PATH, 100);


	// Passing 0 uses the default, 1 the modified MNIST:
	int modified_MNIST_option = argc <= 1 || atoi(argv[1]) <= 0 ? 0 : 1;

	if (modified_MNIST_option) // Learning the modified MNIST dataset - non exclusive class recognition:
	{
		MNIST_modified(MNIST_PATH);
	}

	else // Learning the standard MNIST dataset:
	{
		MNIST_learn(MNIST_PATH);
	}

	return EXIT_SUCCESS;
}
