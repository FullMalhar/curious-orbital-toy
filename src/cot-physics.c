// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.h>

#include <math.h>

#define COT_PHY_GCONST 6.6743E-11L

// Calculates gravitational attraction force between 2 bodies
void cot_phy_force2(const body_t *bodyA, const body_t *bodyB)
{
    vector2d_t vDist, vForce;

    // Calculate distances
    vDist.x = fabsl(bodyA->position.x - bodyB->position.x);
    vDist.y = fabsl(bodyA->position.y - bodyB->position.y);

    // Absolute value of forces
    vForce.x = COT_PHY_GCONST * bodyA->mass * bodyB->mass / powl(vDist.x, 2.0L);
    vForce.y = COT_PHY_GCONST * bodyA->mass * bodyB->mass / powl(vDist.y, 2.0L);
    
}