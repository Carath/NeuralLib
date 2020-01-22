#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand().

#include "MNIST_learning.h"


#define MNIST_PATH "../../../MNIST_samples/"


int main(void)
{
	srand(time(NULL)); // Initialization of the pseudo-random number generator.


	// Printing the first 'number_to_print' inputs of the MNIST training dataset:
	// MNIST_print(MNIST_PATH, 100);


	// Learning the MNIST dataset:
	MNIST_learn(MNIST_PATH);


	// Learning the modified MNIST dataset - non exclusive class recognition:
	// MNIST_modified(MNIST_PATH);


	return EXIT_SUCCESS;
}
