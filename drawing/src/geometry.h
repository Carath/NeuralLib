#ifndef GEOMETRY_H
#define GEOMETRY_H


typedef struct
{
	float x;
	float y;
} Vector2D;


int is_null(const Vector2D *u);


float norm(const Vector2D *u);


// Computes the dot product of two 2D vectors:
float dot_product(const Vector2D *u, const Vector2D *v);


// Computes the determinant of two 2D vectors:
float determinant(const Vector2D *u, const Vector2D *v);


// Computes the angle between two non-zero 2D vectors, in [-pi, pi[:
float angle(const Vector2D *u, const Vector2D *v);


#endif
