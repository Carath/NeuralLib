#ifndef SETTINGS_H
#define SETTINGS_H


// Name of the folder in which neural nets or inputs are saved:
#define SAVES_FOLDER_NAME "saves"

// Maximum number of characters for the directories path:
#define MAX_PATH_LENGTH 200

#define EPSILON 0.00000001


#if defined _FLOAT
	typedef float Number;
	#define number_pow powf
	#define number_sqrt sqrtf
	#define number_abs fabsf
	#define number_min fminf
	#define number_max fmaxf
	#define number_abs fabsf

#elif defined _DOUBLE
	typedef double Number;
	#define number_pow pow
	#define number_sqrt sqrt
	#define number_abs fabs
	#define number_min fmin
	#define number_max fmax
	#define number_abs fabs
#endif


#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) < (y) ? (y) : (x))


typedef enum {INFOS, ALL} PrintOption;


#endif
