// Curious Orbital Toy
// Malhar Palkar
#pragma once

#ifndef __CURIOUS_ORBITAL_TOY_HPP
#define __CURIOUS_ORBITAL_TOY_HPP

#include <SFML/Graphics.hpp>

#include <vector>

namespace cot
{
    // Precision math datatype
    typedef float math_t;

    // Representation of a physical body
    typedef struct _body
    {
        math_t mass;
        sf::Vector2f position, velocity;
    } body_t;

    // Representation of a system
    typedef std::vector<std::pair<body_t, sf::CircleShape>> system_t;
    
    // Physics engine
    class Engine
    {
    private:

        // Vector of all bodies and their corresponding shapes representing the system
        system_t vSystem;

    public:
        Engine(const body_t *pBods, const std::size_t nBods);

        // Updates the physics for each body
        void update(const math_t dt);

        // Draws each body
        void draw(sf::RenderWindow& wind);

        // Calculates the force between 2 bodies
        static std::pair<sf::Vector2f, sf::Vector2f> force2(const body_t *bodyA, const body_t *bodyB);
    };
}

#endif // __CURIOUS_ORBITAL_TOY_HPP