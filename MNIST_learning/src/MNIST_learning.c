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
	int new_answer_size = MNIST_inputs -> AnswersSize + 1; // 1 dimension more.

	Number **new_Questions = createMatrix(inputs_number, question_size);
	Number **new_Answers = createMatrix(inputs_number, new_answer_size);

	for (int i = 0; i < inputs_number; ++i)
	{
		int mirroring;

		if (MNIST_inputs -> Answers[i][0] == 1 || MNIST_inputs -> Answers[i][8] == 1) // 0 and 8 are symmetric.
			mirroring = 0;
		else
			mirroring = rand() % 2;

		for (int j = 0; j < question_size; ++j)
		{
			if (mirroring) // Mirroring the image:
			{
				int row = j / MNIST_IMAGE_WIDTH;
				int col = j % MNIST_IMAGE_WIDTH;

				if (col < MNIST_IMAGE_WIDTH / 2)
				{
					int j_symmetric = row * MNIST_IMAGE_WIDTH + MNIST_IMAGE_WIDTH - 1 - col;

					new_Questions[i][j] = MNIST_inputs -> Questions[i][j_symmetric];
					new_Questions[i][j_symmetric] = MNIST_inputs -> Questions[i][j];
				}
			}

			else
				new_Questions[i][j] = MNIST_inputs -> Questions[i][j];
		}

		for (int j = 0; j < new_answer_size - 1; ++j)
			new_Answers[i][j] = MNIST_inputs -> Answers[i][j];

		// Last index: mirroring status:
		new_Answers[i][new_answer_size - 1] = mirroring;
	}

	Inputs *new_inputs = createInputs(inputs_number, question_size, new_answer_size, new_Questions, new_Answers);

	return new_inputs;
}


// Printing the first 'number_to_print' inputs of the MNIST training dataset:
void MNIST_print(char *MNIST_dir_path, int number_to_print)
{
	printf("\n === Printing MNIST ===\n\n");

	// Loading some inputs to print
	Inputs *training_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_TRAINING);

	for (int i = 0; i < number_to_print; ++i)
		printGrayscaleImage(training_inputs -> Questions[i], MNIST_IMAGE_WIDTH, MNIST_IMAGE_HEIGHT); // 28 x 28 images.

	freeInputs(training_inputs);
}


// Learning the MNIST dataset:
void MNIST_learn(char *MNIST_dir_path)
{
	printf("\n === Learning: MNIST ===\n\n");

	char save_filename[] = "MNIST_learned";

	// Loading the MNIST training dataset:
	Inputs *training_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_TRAINING);

	// Creating a neural network:

	int layer_number = 3;
	int max_batch_size = 100;
	int NeuronsNumberArray[] = {512, 32, 10};
	Activation funArray[] = {ReLu, ReLu, Softmax};

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

	freeNetwork(network_loaded);
	freeInputs(validation_inputs);
	freeParameters(params);
	freeNetwork(network);
	freeInputs(training_inputs);
}


// Learning the modified MNIST dataset - non exclusive class recognition:
void MNIST_modified(char *MNIST_dir_path)
{
	printf("\n === Learning: MNIST modified ===\n\n");

	char save_filename[] = "MNIST_modified";

	// Loading some inputs to learn:
	Inputs *training_inputs = MNIST_createInputs(MNIST_dir_path, MNIST_TRAINING);

	Inputs *new_training_inputs = modified_MNIST_inputs(training_inputs);

	freeInputs(training_inputs);

	// Creating a neural network:

	int layer_number = 4;
	int max_batch_size = 100;
	int NeuronsNumberArray[] = {512, 64, 32, 11};
	Activation funArray[] = {ReLu, ReLu, ReLu, Sigmoid};

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

	freeInputs(validation_inputs);

	RecognitionMode recog = ALL_CORRECT; // non exclusive class recognition!

	printf("\nTraining inputs:\n");
	validation(network, new_training_inputs, recog);

	printf("Validation inputs:\n");
	validation(network, new_validation_inputs, recog);

	// Saving:

	saveNetwork(network, save_filename);

	// Freeing everything:

	freeInputs(new_validation_inputs);
	freeParameters(params);
	freeNetwork(network);
	freeInputs(new_training_inputs);
}
