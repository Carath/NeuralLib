#ifndef MATRIX_H
#define MATRIX_H


#include "settings.h"


typedef enum {NoTrans, Trans} TransposeOptions;


// Matrix utilities:

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

void resetVector(Number *vector, int len);

void resetMatrix(Number **matrix, int rows, int cols);

void copyVector(Number *dest, const Number *src, int len);

void copyMatrix(Number **dest, const Number **src, int rows, int cols);


// Randomly fill vectors/matrices:

void randomFillVector_uniform(Number *vector, int len, Number bound);

void randomFillMatrix_uniform(Number **matrix, int rows, int cols, Number bound);

void randomFillVector_gaussian(Number *vector, int len, Number std_dev);

void randomFillMatrix_gaussian(Number **matrix, int rows, int cols, Number std_dev);


// Computational part:
// Matrices here are stored as 1 dimensional arrays, for compliance with high performance libraries.

// Naive implementation of: X <- alpha * X.
void naive_scal(Number *X, int len, Number alpha);


// Naive implementation of: X <- X + alpha * Y.
void naive_addScal(Number *X, Number *Y, int len, Number alpha);


// Naive implementation of: C <- op(A) * op(B). For each case, the best loop order has been chosen:
void naive_matrix_multiply(TransposeOptions optA, TransposeOptions optB, Number *A, Number *B, Number *C,
	int rows_op_A, int cols_op_B, int cols_op_A);


#endif
