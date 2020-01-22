#ifndef LEARNING_MNIST_H
#define LEARNING_MNIST_H


// Printing the first 'number_to_print' inputs of the MNIST training dataset:
void MNIST_print(char *MNIST_dir_path, int number_to_print);


// Learning the MNIST dataset:
void MNIST_learn(char *MNIST_dir_path);


// Learning the modified MNIST dataset - non exclusive class recognition:
void MNIST_modified(char *MNIST_dir_path);


#endif
