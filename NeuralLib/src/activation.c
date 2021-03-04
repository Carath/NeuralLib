#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "activation.h"


static const Number LRELU_COEFF = 0.01;
static const Number ELU_COEFF = 0.01;
static const Number SELU_COEFF_POS = 1.67326;
static const Number SELU_COEFF_NEG = 1.75810;


#define SUM_LIST(ENUM) + 1
#define TO_STRING_LIST(STRING) ADD_TO_LIST(TO_STRING(STRING))


// Number of Activations:
static const int ActivationsNumber = ACTIVATION_APPLY(SUM_LIST);


// Array of string of each Activation:
static const char *ActivationsStringArray[] = {ACTIVATION_APPLY(TO_STRING_LIST)};


// Returns the number of supported activation functions:
int getActivationNumber(void)
{
	return ActivationsNumber;
}


// Get the name of an Activation:
const char* getActivationString(Activation fun)
{
	return ActivationsStringArray[fun];
}


// Get an Activation from its string:
Activation getActivation(const char *string)
{
	int index = 0;

	while (index < ActivationsNumber && strcmp(string, ActivationsStringArray[index]) != 0)
		++index;

	if (index == ActivationsNumber)
	{
		printf("\nNo matching Activation found for the string: %s\n\n", string);
		exit(EXIT_FAILURE);
	}

	return index;
}


// Activation functions, without softmax:
Number activation(Activation fun, Number x)
{
	switch (fun)
	{
		case Id:

			return x;
			break;

		case Heaviside:

			return x > 0.f;
			break;

		case Sigmoid:

			return 1.f / (1.f + number_exp(-x));
			break;

		case Tanh:

			return 1.f - 2.f / (1.f + expf(2.f * x)); // way faster than a tanhf() call!
			break;

		case ReLu:

			return x >= 0.f ? x : 0.f;
			break;

		case LReLu:

			return x >= 0.f ? x : LRELU_COEFF * x;
			break;

		case ELu:

			return x >= 0.f ? x : ELU_COEFF * (number_exp(x) - 1.f);
			break;

		case SELu:

			return x >= 0.f ? SELU_COEFF_POS * x : SELU_COEFF_NEG * (number_exp(x) - 1.f);
			break;

		default:

			return 0.f;
			break;
	}
}


// Derivatives, without softmax:
Number der_activation(Activation fun, Number x)
{
	switch (fun)
	{
		case Id:

			return 1.f;
			break;

		case Heaviside:

			return 1.f; // Won't do anything with 0.
			break;

		case Sigmoid:

			;Number y = activation(Sigmoid, x);
			return y * (1.f - y);
			break;

		case Tanh:

			;Number z = activation(Sigmoid, x);
			return 1.f - z * z;
			break;

		case ReLu:

			return x >= 0.f ? 1.f : 0.f;
			break;

		case LReLu:

			return x >= 0.f ? 1.f : LRELU_COEFF;
			break;

		case ELu:

			return x >= 0.f ? 1.f : ELU_COEFF * number_exp(x);
			break;

		case SELu:

			return x >= 0.f ? SELU_COEFF_POS : SELU_COEFF_NEG * number_exp(x);
			break;

		default:

			return 1.f;
			break;
	}
}


void softmax(Number *dest, const Number *src, int len)
{
	Number sum = 0.f;

	for (int i = 0; i < len; ++i)
	{
		Number x = number_exp(src[i]);
		sum += x;
		dest[i] = x;
	}
	
	// Total sum has been computed, now dividing by it:
	for (int i = 0; i < len; ++i)
		dest[i] /= (sum + EPSILON);
}


// Updating the last layer's GradSum for the softmax activation with quadratic loss:
void updateGradSumSoftmaxQuadLoss(Number *GradSum, const Number *answer, const Number *good_answer, int len)
{
	Number sum = 0.f;

	for (int i = 0; i < len; ++i)
		sum += answer[i] * (answer[i] - good_answer[i]);

	for (int i = 0; i < len; ++i)
		GradSum[i] = answer[i] * (answer[i] - good_answer[i] - sum);
}
