#include <stdio.h>
#include <stdlib.h>

#include "testing.h"


int main(void)
{
	// Normalization of some inputs:
	// test_normalize();


	// Testing the Box-Muller transform:
	// test_Box_Muller();


	// Shuffle demo:
	// test_shuffle();


	// 1 layer neural network for the logical gate 'AND':
	test_AND();


	// 2 layers neural network for the logical gate 'XOR':
	// test_XOR();


	return EXIT_SUCCESS;
}
