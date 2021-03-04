#include <stdio.h>
// #include <stdlib.h>
#include <math.h>

#include "random.h"


static const double two_pi = 2.0 * 3.14159265358979323846;
// static const double sqrt_2 = 1.41421356237309504880;


// Returns a random number in [min, max].
inline Number uniform_random(void *rng, Number min, Number max)
{
	return (max - min) * rng32_nextFloat(rng) + min;
}


// Box–Muller transform, generate two 'Number' following the distribution N(0,1):
void Box_Muller(void *rng, Number *x1, Number *x2)
{
	double u1;
	double u2 = rng32_nextFloat(rng); // ~ U[0, 1]

	do { u1 = rng32_nextFloat(rng); } // ~ U]0, 1], needs to be > 0 !!!
	while (u1 == 0);

	double rho = sqrt(-2. * log(u1));
	double theta = two_pi * u2;

	*x1 = (Number) (rho * cos(theta));
	*x2 = (Number) (rho * sin(theta));
}


// Generate two 'Number' following the distribution N(mean, std_dev²):
void gaussian_random(void *rng, Number *x1, Number *x2, Number mean, Number std_dev)
{
	Box_Muller(rng, x1, x2);

	*x1 = mean + std_dev * *x1;
	*x2 = mean + std_dev * *x2;
}


// Shuffling:


// Fisher–Yates shuffle, for an array of pointers:
void shuffle(void *rng, void* *array, int len)
{
	// In case RNG32_MAX is small, for some (terrible) RNG...
	if (len >= RNG32_MAX)
		printf("\nArray length (%d) is greater or equal to RNG32_MAX!\n\n", len);

	for (unsigned int i = len - 1; i >= 1; --i)
	{
		unsigned int j = rng32_nextInt(rng) % (i + 1); // 0 ≤ j ≤ i. Biased, but negligeable.

		void* temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}


// Readable if every element of array is the address of a Number...
void demo_print(Number* *array, int len)
{
	if (array == NULL)
		return;

	for (int i = 0; i < len; ++i)
		printf("%8.f", *(array[i])); // only printing the integer part.
	printf("\n");
}
