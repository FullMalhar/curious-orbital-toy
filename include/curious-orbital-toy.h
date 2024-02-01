// Curious Orbital Toy
// Malhar Palkar
#pragma once

#ifndef __CURIOUS_ORBITAL_TOY_H
#define __CURIOUS_ORBITAL_TOY_H

#define UNUSED_PARAMETER(x) (void)(x)

#define COT_BODYCOUNT 2

// Type container for a 2D vector
typedef struct 
{
    float x, y;
} vector2d_t;

// Type container for a point mass body
typedef struct 
{
    float mass;
    vector2d_t position, velocity;
} body_t;

#endif // __CURIOUS_ORBITAL_TOY_H