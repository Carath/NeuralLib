#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand().

#include "animation.h"


int main(int argc, char **argv)
{
	srand(time(NULL)); // Initialization of the pseudo-random number generator.


	// Passing 0 uses the default, 1 the modified MNIST:
	int modified_MNIST_option = argc <= 1 || atoi(argv[1]) <= 0 ? 0 : 1;


	// Animating the recognition of the MNIST dataset:
	MNIST_animation(modified_MNIST_option);


	return EXIT_SUCCESS;
}
