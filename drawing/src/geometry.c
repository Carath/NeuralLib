// Uncommenting what follows disable all assert in this file.
// It can also be defined via a makefile when all source files are targeted:
// #define NDEBUG

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "geometry.h"


#define EPSILON 0.0000001f


inline int is_null(const Vector2D *u)
{
	return fabsf(u->x) < EPSILON && fabsf(u->y) < EPSILON;
}


inline float norm(const Vector2D *u)
{
	return sqrtf(u->x * u->x + u->y * u->y);
}


// Computes the dot product of two 2D vectors:
inline float dot_product(const Vector2D *u, const Vector2D *v)
{
	return u->x * v->x + u->y * v->y;
}


// Computes the determinant of two 2D vectors:
inline float determinant(const Vector2D *u, const Vector2D *v)
{
	return u->x * v->y - u->y * v->x;
}


// Computes the angle between two non-zero 2D vectors, in [-pi, pi[:
float angle(const Vector2D *u, const Vector2D *v)
{
	float u_norm = norm(u);
	float v_norm = norm(v);
	// assert(u_norm * v_norm > 0.f);

	float dotp = dot_product(u, v);
	float det = determinant(u, v);

	float r = dotp / (u_norm * v_norm + EPSILON);
	r = r > 1.f ? 1.f : r;
	r = r < -1.f ? -1.f : r;

	float angle = acosf(r); // in [0, pi]
	assert(! isnan(angle));

	return det > 0.f ? angle : -angle; // has correct sign.
}
