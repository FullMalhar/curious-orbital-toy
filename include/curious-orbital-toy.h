// Curious Orbital Toy
// Malhar Palkar
#pragma once

#ifndef __CURIOUS_ORBITAL_TOY_H
#define __CURIOUS_ORBITAL_TOY_H

#define UNUSED_PARAMETER(x) (void)(x)

#define COT_BODYCOUNT 2

#include <SFML/Graphics.h>

// Consistent definition of which number to use
typedef float number_t;

// Type container for a point mass body
typedef struct 
{
    number_t mass;
    sfVector2f position, velocity;
} body_t;

// Draws a series of orbital bodies on the screen
void cot_dsp_draw(sfRenderWindow *pWindow, body_t *pScene);

// Initialises display objects
void cot_dsp_init();

// Cleans up display objects
void cot_dsp_destroy();

// Updates the scene
void cot_phy_update(body_t *scene, float dt);

#endif // __CURIOUS_ORBITAL_TOY_H