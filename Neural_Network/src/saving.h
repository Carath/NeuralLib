#ifndef SAVING_H
#define SAVING_H


#include "settings.h"


// Creating a folder:
void createFolder(char *foldername);


// Used to exit the program when a file error occur, raising the given message and filename:
void exitFileError(FILE *file, char *error_message, char *filename);


// Get the size of the given binary file, in bytes:
long int getFileSize(char *filename);


// Skip the given string in the opened file:
void skip(FILE *file, char *string);


// Writing:

void save_flat_matrix(Number *matrix, int rows, int cols, char *filename);


void save_matrix(Number **matrix, int rows, int cols, char *filename);


// Reading:

// Matrix must alrealy be allocated in memory:
void load_toFlatMatrix(Number *matrix, int rows, int cols, char *filename);


// Matrix must alrealy be allocated in memory:
void load_toMatrix(Number **matrix, int rows, int cols, char *filename);


#endif
