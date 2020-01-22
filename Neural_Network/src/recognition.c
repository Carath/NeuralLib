#include <stdio.h>
#include <stdlib.h>

#include "recognition.h"
#include "matrix.h"


#define RECOG_THRESHOLD 0.5


// Returns the index of the greatest value in 'vector':
inline static int maxValueIndex(const Number *vector, int len)
{
	int index = 0;
	Number max = vector[0];

	for (int i = 1; i < len; ++i)
	{
		if (vector[i] > max)
		{
			max = vector[i];
			index = i;
		}
	}

	return index;
}


// Validation:

// MAX_VALUE -> Checks if the class with higher value is the good answer. Do not use for binary classification.

// MAX_CORRECT -> Checks if the class with higher value is the good answer, if said value is >= RECOG_THRESHOLD.
// Useful for binary classification. Stricter than 'valid_MaxValue'.

// ALL_CORRECT -> Checks if all classes have good answers. Useful for binary classification,
// and non-mutually exclusive classification problems. Stricter than 'valid_MaxCorrect'.

// Checks if the predicted answer is correct, whith respect to the chosen recognition method:
int validation_method(Number *good_answer, Number *answer, int len, RecognitionMode recog)
{
	if (recog == MAX_VALUE)
		return good_answer[maxValueIndex(answer, len)] == 1;

	else if (recog == MAX_CORRECT)
	{
		const int max_index = maxValueIndex(answer, len);

		return good_answer[max_index] == (answer[max_index] >= RECOG_THRESHOLD);
	}

	else // ALL_CORRECT
	{
		int index = 0;

		while (index < len && good_answer[index] == (answer[index] >= RECOG_THRESHOLD))
			++index;

		return index == len;
	}
}


// Framework for using the validation or prediction functions:
int recog_method(RecogType type, RecognitionMode recog, Number *goodOrToFill_answers, Number *answer, int len)
{
	if (type == VALIDATION)
		return validation_method(goodOrToFill_answers, answer, len, recog);

	else // PREDICTION
	{
		copyVector(goodOrToFill_answers, answer, len);
		return 0;
	}
}


// Find the most probable class answer:
// For binary class problems, returns the binary answer.
// For mutally exclusive class problems, returns the index of the guessed class.
// For mixed problems, one can shift 'answer' and modify 'len' accordingly on each subset which are binary/exclusive.
// Optional: confidence_level is filled if a non-NULL arg is passed.
int findMostProbable(const Number *answer, int len, Number *confidence_level)
{
	if (len == 1)
	{
		int binary_answer = answer[0] >= RECOG_THRESHOLD;

		if (confidence_level != NULL)
			*confidence_level = binary_answer ? answer[0] : 1. - answer[0];

		return binary_answer;
	}

	const int max_value_index = maxValueIndex(answer, len);

	if (confidence_level != NULL)
	{
		Number sum_values = 0;

		for (int i = 0; i < len; ++i)
			sum_values += answer[i];

		*confidence_level = answer[max_value_index] / sum_values;
	}

	return max_value_index;
}
