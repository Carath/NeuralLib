#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand().

#include "animation.h"


int main(void)
{
	srand(time(NULL)); // Initialization of the pseudo-random number generator.


	// Animating the recognition of the MNIST dataset:
	MNIST_animation();


	return EXIT_SUCCESS;
}
