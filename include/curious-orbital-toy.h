// Curious Orbital Toy
// Malhar Palkar
#pragma once

#ifndef __CURIOUS_ORBITAL_TOY_H
#define __CURIOUS_ORBITAL_TOY_H

#define UNUSED_PARAMETER(x) (void)(x)

#define COT_BODYCOUNT 2

// Consistent definition of which number to use
typedef long double number_t;

// Type container for a 2D vector
typedef struct 
{
    number_t x, y;
} vector2d_t;

// Type container for a point mass body
typedef struct 
{
    number_t mass;
    vector2d_t position, velocity;
} body_t;

#endif // __CURIOUS_ORBITAL_TOY_H