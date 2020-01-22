//////////////////////////////////////////////////////////
// NeuralLib v1.2
//////////////////////////////////////////////////////////


#ifndef NEURAL_LIB_H
#define NEURAL_LIB_H


// This file contains the public definitions and functions from the 'NeuralLib' library.


//////////////////////////////////////////////////////////
// settings.h
//////////////////////////////////////////////////////////


#if defined _FLOAT
	typedef float Number;

#elif defined _DOUBLE
	typedef double Number;
#endif


#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) < (y) ? (y) : (x))


typedef enum {INFOS, ALL} PrintOption;


//////////////////////////////////////////////////////////
// activation.h
//////////////////////////////////////////////////////////


// For performance: don't change the following order without doing it for the switches too.
// Also Softmax needs to be placed at the beginning or end of the list:
#define ACTIVATION_APPLY(FUN)	\
	FUN(Id)			\
	FUN(Heaviside)	\
	FUN(Sigmoid)	\
	FUN(Tanh)		\
	FUN(ReLu)		\
	FUN(LReLu)		\
	FUN(ELu)		\
	FUN(SELu)		\
	FUN(Softmax)	\


#define ACTIVATION_ID(ENUM) ENUM,

// Enum of activation functions:
typedef enum {ACTIVATION_APPLY(ACTIVATION_ID)} Activation;


//////////////////////////////////////////////////////////
// inputs.h
//////////////////////////////////////////////////////////


// Answers = NULL if the inputs are not for learning.
typedef struct
{
	const int InputNumber;
	const int QuestionsSize;
	const int AnswersSize;
	Number **Questions;		// InputNumber x QuestionsSize
	Number **Answers;		// InputNumber x AnswersSize
} Inputs;


Inputs* createInputs(int InputNumber, int QuestionsSize, int AnswersSize, Number **Questions, Number **Answers);


void freeInputs(Inputs *inputs);


void printInputs(Inputs *inputs, PrintOption opt);


int saveInputs(Inputs *inputs, char *foldername);


Inputs* loadInputs(char *foldername);


// Finds the mean and standard deviation vectors of the given inputs, stored in a 2 x QuestionsSize matrix.
// This matrix is returned in order to be able to normalize other inputs by the previous mean and std deviation.
Number** find_mean_stddev_matrix(Inputs *inputs);


// Normalizes the given inputs, so that: mean = 0 and standard deviation = 1.
void normalize(Inputs *inputs, Number **mean_stddev_matrix);


// Applying the Fisher–Yates shuffle on the given inputs:
void shuffleInputs(Inputs *inputs);


//////////////////////////////////////////////////////////
// neural_network.h
//////////////////////////////////////////////////////////


typedef struct
{
	const int InputSize;
	const int NeuronsNumber; // i.e output size.
	Activation Fun;

	Number *Input;		// Pointer to the previous output if not the first layer.
	Number *Net;		// (InputSize + 1) * NeuronsNumber (last row: biases, other rows: weights)
	Number *Sum;		// MaxBatchSize * NeuronsNumber
	Number *GradSum;	// MaxBatchSize * NeuronsNumber
	Number *Output;		// MaxBatchSize * (NeuronsNumber + 1)
} NeuronLayer;


typedef struct
{
	int HasLearned;
	const int LayersNumber;
	const int MaxBatchSize; // The greater MaxBatchSize is, the faster the computations may be.
	NeuronLayer *Layers;	// size: LayersNumber
} NeuralNetwork;


// 'NeuronsNumberArray' and 'funArray' are of size 'LayersNumber'.
NeuralNetwork* createNetwork(int InputSize, int LayersNumber, int *NeuronsNumberArray, Activation *funArray, int MaxBatchSize);


// Freeing the network:
void freeNetwork(NeuralNetwork *network);


int network_inputSize(NeuralNetwork *network);


int network_outputSize(NeuralNetwork *network);


// Printing a network's content:
void printNetwork(NeuralNetwork *network, PrintOption opt);


// Saving a neural network. N.B: MaxBatchSize doesn't need to be saved, for its value is arbitrary:
int saveNetwork(NeuralNetwork *network, char *foldername);


// Loading a neural network:
NeuralNetwork* loadNetwork(char *foldername, int MaxBatchSize);


//////////////////////////////////////////////////////////
// recognition.h
//////////////////////////////////////////////////////////


// MAX_VALUE: multiclass problems only. Unique positive answer.
// MAX_CORRECT: binary or multiclass problems. At most one positive answer.
// ALL_CORRECT: binary, multiclass or non-mutually exclusive multiclass problems. Multiple positive answers allowed.
// Modes sorted by strictness: MAX_VALUE < MAX_CORRECT < ALL_CORRECT.
typedef enum {MAX_VALUE, MAX_CORRECT, ALL_CORRECT} RecognitionMode;


// Find the most probable class answer:
// For binary class problems, returns the binary answer.
// For mutally exclusive class problems, returns the index of the guessed class.
// For mixed problems, one can shift 'answer' and modify 'len' accordingly on each subset which are binary/exclusive.
// Optional: confidence_level is filled if a non-NULL arg is passed.
int findMostProbable(const Number *answer, int len, Number *confidence_level);


//////////////////////////////////////////////////////////
// learning.h
//////////////////////////////////////////////////////////


typedef enum {ON_LINE, MINI_BATCHES, FULL_BATCH} BatchMethod;
typedef enum {QUADRATIC, CROSS_ENTROPY} LossFunction;
typedef enum {UNIFORM, GAUSSIAN} InitRandDistribution;
typedef enum {AUTOMATIC_STD, AUTOMATIC_NORMALIZED, BY_RANGE} InitMethod;
typedef enum {NO_OPT, MOMENTUM, RMSprop, ADAM} Optimizer;
typedef enum {NO_REG, L2} Regularization;
typedef enum {NO_SHUFFLE, SHUFFLE} ShuffleMode;

// Tips:
// ON_LINE is slower than MINI_BATCHES, which performs best when BatchSize >= 16.
// AUTOMATIC_NORMALIZED works better when Init = UNIFORM.


typedef struct
{
	BatchMethod Method;
	LossFunction LossFun;
	InitRandDistribution Random;
	InitMethod Init;
	ShuffleMode Shuffle; // Shuffling by default.
	Optimizer Optim;
	Regularization Reg;
	RecognitionMode RecogEstimates; // ALL_CORRECT by default. This can be changed for better estimates.

	int PrintEstimates;
	int EpochNumber;
	int BatchSize;
	Number BatchSizeMultiplier; // Multiply the batch size by this value after each epoch.
	Number InitRange; // If Init = BY_RANGE, weights are randomly chosen between -InitRange and InitRange.
	Number LearningRate;
	Number LearningRateMultiplier; // Multiply the learning rates by this value after each epoch.

	// Optimizers settings:
	Number MomentumRate;
	Number RMScoeff;
	Number AdamBetaM;
	Number AdamBetaV;

	// Regularization settings:
	Number L2regCoeff;
} LearningParameters;


// Creates some learning settings, initialized with default values:
LearningParameters* initLearningParameters(void);


void freeParameters(LearningParameters *params);


// Make the neural network learn the given inputs, while using the given parameters:
void learn(NeuralNetwork *network, Inputs *inputs, LearningParameters *params);


// Compare the network answers to the correct ones, and print the validation level.
void validation(NeuralNetwork *network, Inputs *inputs, RecognitionMode recog);


// Write the network answers in the given inputs.
void prediction(NeuralNetwork *network, Inputs *inputs);


//////////////////////////////////////////////////////////
// matrix.h
//////////////////////////////////////////////////////////


// Filled with 0s:
Number* createVector(int len);

// Filled with 0s:
Number** createMatrix(int rows, int cols);

// This is equivalent to a regular free() call:
void freeVector(Number *vector);

void freeMatrix(Number **matrix, int rows);

void printVector(Number *vector, int len);

void printFlatMatrix(Number *matrix, int rows, int cols);

void printMatrix(Number **matrix, int rows, int cols);

void copyVector(Number *dest, const Number *src, int len);

void copyMatrix(Number **dest, const Number **src, int rows, int cols);


//////////////////////////////////////////////////////////
// saving.h
//////////////////////////////////////////////////////////


// Creating a folder:
void createFolder(char *foldername);


// Get the size of the given binary file, in bytes:
long int getFileSize(char *filename);


void save_flat_matrix(Number *matrix, int rows, int cols, char *filename);


void save_matrix(Number **matrix, int rows, int cols, char *filename);


// Matrix must alrealy be allocated in memory:
void load_toFlatMatrix(Number *matrix, int rows, int cols, char *filename);


// Matrix must alrealy be allocated in memory:
void load_toMatrix(Number **matrix, int rows, int cols, char *filename);


//////////////////////////////////////////////////////////
// image_processing.h
//////////////////////////////////////////////////////////


typedef enum {MAX_POOLING, AVERAGE_POOLING} PoolingMode;


// Print in the console a grayscale image contained in a 1-dimensional array:
void printGrayscaleImage(Number *image, int width, int height);


void pooling(Number *output, const Number *input, int output_width, int output_height,
	int input_width, int input_height, PoolingMode poolmeth);


void recenter(Number *dest, const Number *src, int width, int height);


//////////////////////////////////////////////////////////
// random.h
//////////////////////////////////////////////////////////


// Returns a random number in [min, max[.
Number uniform_random(Number min, Number max);


//////////////////////////////////////////////////////////
// benchmarking
//////////////////////////////////////////////////////////


#include <time.h>


// Simple function, but may give erroneous results if multithreading is used:
double time_elapsed(clock_t start);


// Write the current time in the given struct:
void get_time(struct timespec *current_time);


// More precise timer: (POSIX)
double precise_time(struct timespec *start, struct timespec *finish);


//////////////////////////////////////////////////////////
// endian.h
//////////////////////////////////////////////////////////


// The following functions are useful to translate float/double matrices
// from the little-endian format to the big-endian format, and vice versa.


// Swap the bytes order of every float in the given matrix:
void swap_matrix_float(float **matrix, int rows, int cols);


// Swap the bytes order of every double in the given matrix:
void swap_matrix_double(double **matrix, int rows, int cols);


#endif
