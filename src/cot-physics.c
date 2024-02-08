// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.h>

#include <tgmath.h>

#define COT_PHY_GCONST 6.6743E-11f

// Calculates the gravitational attraction force between 2 bodies
// F = G m1 m2 / r^2
void cot_phy_force2(const body_t *bodyA, const body_t *bodyB, sfVector2f *vForceA, sfVector2f *vForceB)
{
    sfVector2f vDist;

    // Calculate distances
    vDist.x = fabsf(bodyA->position.x - bodyB->position.x);
    vDist.y = fabsf(bodyA->position.y - bodyB->position.y);

    // Calculate absolute value of force
    vForceA->x = COT_PHY_GCONST * bodyA->mass * bodyB->mass / powf(vDist.x, 2.0f);
    vForceA->y = COT_PHY_GCONST * bodyA->mass * bodyB->mass / powf(vDist.y, 2.0f);

    // Set correct signs
    if (vForceA->x > vForceB->x) vForceA->x *= -1.0f;
    if (vForceA->y > vForceB->y) vForceA->y *= -1.0f;
    
    // Force on body B
    vForceB->x = -vForceA->x;
    vForceB->y = -vForceA->y;
}

void cot_phy_update(body_t *scene, float dt)
{
    sfVector2f forceA, forceB, forces[COT_BODYCOUNT];

    // Loop through each body combination
    for (size_t i = 0; i < COT_BODYCOUNT; i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            // Calculate the forces between the bodies
            cot_phy_force2(&scene[i], &scene[j], &forceA, &forceB);

            // Add calculated forces for the respective bodies
            forces[i].x += forceA.x;    forces[i].y = forceA.y;
            forces[j].x += forceB.x;    forces[j].y = forceB.y;
        }
    }

    // Loop through each body in the scene
    for (size_t i = 0; i < COT_BODYCOUNT; i++)
    {
        // Calculate the position movement due to the previous velocity
        scene[i].position.x += scene[i].velocity.x * dt;
        scene[i].position.y += scene[i].velocity.y * dt;

        // Calculate the velocity due to the force on the body
        scene[i].velocity.x += forces[i].x * dt / scene[i].mass;
        scene[i].velocity.y += forces[i].y * dt / scene[i].mass;
    }
}