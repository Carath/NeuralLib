#include <stdio.h>
#include <stdlib.h>

#include "MNIST_learning.h"
#include "MNIST_loading.h"


// Modified MNIST inputs: the dataset images are randomly mirrored,
// and the mirrored status is placed at the (new) last index of the answers:
static Inputs* modified_MNIST_inputs(const Inputs *MNIST_inputs)
{
	int inputs_number = MNIST_inputs -> InputNumber;
	int question_size = MNIST_inputs -> QuestionsSize;
	int answer_size = MNIST_inputs -> AnswersSize;

	// 2 times more inputs, answers have 1 more dimension:
	Number **new_Questions = createMatrix(2 * inputs_number, question_size);
	Number **new_Answers = createMatrix(2 * inputs_number, answer_size + 1);

	for (int i = 0; i < 2 * inputs_number; i += 2)
	{
		for (int j = 0; j < question_size; ++j)
		{
			int row = j / MNIST_IMAGE_WIDTH;
			int col = j % MNIST_IMAGE_HEIGHT;
			int j_symmetric = row * MNIST_IMAGE_WIDTH + MNIST_IMAGE_WIDTH - 1 - col;

			new_Questions[i][j] = MNIST_inputs -> Questions[i / 2][j];
			new_Questions[i+1][j] = MNIST_inputs -> Questions[i / 2][j_symmetric];
		}

		for (int j = 0; j < answer_size; ++j)
		{
			new_Answers[i][j] = MNIST_inputs -> Answers[i / 2][j];
			new_Answers[i+1][j] = MNIST_inputs -> Answers[i / 2][j];
		}

		// Last index: mirroring status:
		new_Answers[i][answer_size] = 0;
		new_Answers[i+1][answer_size] = 1; // N.B: 0 and 8 are symmetric: too bad.
	}

	return createInputs(2 * inputs_number, question_size, answer_size + 1, new_Questions, new_Answers);
}


// Printing the first 'number_to_print' inputs of the MNIST training dataset:
void MNIST_print(const char *MNIST_dir_path, int number_to_print)
{
	printf("\n === Printing MNIST ===\n\n");

	// Loading some inputs to print
	Inputs *training_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_TRAINING);

	for (int i = 0; i < number_to_print; ++i)
		printGrayscaleImage(training_inputs -> Questions[i], MNIST_IMAGE_WIDTH, MNIST_IMAGE_HEIGHT); // 28 x 28 images.

	freeInputs(&training_inputs);
}


// Learning the standard MNIST dataset:
void MNIST_learn(const char *MNIST_dir_path)
{
	printf("\n === Learning: MNIST ===\n\n");

	char save_filename[] = "saves/MNIST_learned";

	// Loading the MNIST training dataset:
	Inputs *training_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_TRAINING);

	// Creating a neural network:

	int max_batch_size = 100;
	int NeuronsNumberArray[] = {512, 32, 10};
	Activation funArray[] = {ReLu, ReLu, Softmax};

	int layer_number = ARRAYS_COMPARE_LENGTH(NeuronsNumberArray, funArray);

	NeuralNetwork *network = createNetwork(training_inputs -> QuestionsSize, layer_number, NeuronsNumberArray,
		funArray, max_batch_size);

	// Setting the learning parameters:

	LearningParameters *params = initLearningParameters();

	params -> Method = MINI_BATCHES;
	params -> LossFun = CROSS_ENTROPY;
	params -> Random = GAUSSIAN;
	params -> Init = AUTOMATIC_STD;
	params -> Shuffle = SHUFFLE;
	params -> Optim = NO_OPT;
	params -> Reg = NO_REG;
	params -> RecogEstimates = MAX_VALUE;

	params -> EpochNumber = 10;

	params -> BatchSize = 100;
	params -> BatchSizeMultiplier = 1.;

	params -> LearningRate = 0.005;
	params -> LearningRateMultiplier = 0.9;

	// Learning process:

	learn(network, training_inputs, params);

	// Validation:

	// Loading the MNIST validation dataset:
	Inputs *validation_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_VALIDATION);

	RecognitionMode recog = MAX_VALUE;

	printf("\nTraining inputs:\n");
	validation(network, training_inputs, recog);

	printf("Validation inputs:\n");
	validation(network, validation_inputs, recog);

	// Saving and loading:

	saveNetwork(network, save_filename);

	NeuralNetwork *network_loaded = loadNetwork(save_filename, max_batch_size);

	// Validation for the loaded network:

	printf("\nTraining inputs:\n");
	validation(network_loaded, training_inputs, recog);

	printf("Validation inputs:\n");
	validation(network_loaded, validation_inputs, recog);

	// Freeing everything:

	freeNetwork(&network_loaded);
	freeInputs(&validation_inputs);
	freeParameters(&params);
	freeNetwork(&network);
	freeInputs(&training_inputs);
}


// Learning the modified MNIST dataset - non exclusive class recognition:
void MNIST_modified(const char *MNIST_dir_path)
{
	printf("\n === Learning: MNIST modified ===\n\n");

	char save_filename[] = "saves/MNIST_modified";

	// Loading some inputs to learn:
	Inputs *training_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_TRAINING);

	Inputs *new_training_inputs = modified_MNIST_inputs(training_inputs);

	freeInputs(&training_inputs);

	// Creating a neural network:

	int max_batch_size = 100;
	int NeuronsNumberArray[] = {512, 64, 32, 11};
	Activation funArray[] = {ReLu, ReLu, ReLu, Sigmoid};

	int layer_number = ARRAYS_COMPARE_LENGTH(NeuronsNumberArray, funArray);

	NeuralNetwork *network = createNetwork(new_training_inputs -> QuestionsSize, layer_number, NeuronsNumberArray,
		funArray, max_batch_size);

	// Setting the learning parameters:

	LearningParameters *params = initLearningParameters();

	params -> Method = MINI_BATCHES;
	params -> LossFun = CROSS_ENTROPY;
	params -> Random = GAUSSIAN;
	params -> Init = AUTOMATIC_STD;
	params -> Shuffle = SHUFFLE;
	params -> Optim = NO_OPT;
	params -> Reg = NO_REG;
	params -> RecogEstimates = ALL_CORRECT; // default.

	params -> EpochNumber = 10;

	params -> BatchSize = 100;
	params -> BatchSizeMultiplier = 1.;

	params -> LearningRate = 0.003;
	params -> LearningRateMultiplier = 0.95;

	// Learning process:

	learn(network, new_training_inputs, params);

	// Validation:

	Inputs *validation_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_VALIDATION);

	Inputs *new_validation_inputs = modified_MNIST_inputs(validation_inputs);

	freeInputs(&validation_inputs);

	RecognitionMode recog = ALL_CORRECT; // non exclusive class recognition!

	printf("\nTraining inputs:\n");
	validation(network, new_training_inputs, recog);

	printf("Validation inputs:\n");
	validation(network, new_validation_inputs, recog);

	// Saving:

	saveNetwork(network, save_filename);

	// Freeing everything:

	freeInputs(&new_validation_inputs);
	freeParameters(&params);
	freeNetwork(&network);
	freeInputs(&new_training_inputs);
}
