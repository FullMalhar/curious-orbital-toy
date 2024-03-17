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

        /**
         * @param pBods Pointer to array of bodies
         * @param nBods Number of bodies in array
        */
        Engine(const body_t *pBods, const std::size_t nBods);

        /**
         * @brief Allows the physics engine to update force, velocity, and position of each body in the system
         * @param dt Time since the update function was last called
        */
        void update(const math_t dt);

        /**
         * @brief Draws all bodies in the system in their last updated position
         * @param wind Reference to drawing window
        */
        void draw(sf::RenderWindow& wind);

        /**
         * @brief Calculates the gravitational attraction force between 2 bodies
         * @param bodyPair Pair of bodies
         * @return Pair of force vectors corresponding to input parameter pair
        */
        static std::pair<sf::Vector2f, sf::Vector2f> force2(const std::pair<body_t, body_t>& bodyPair);
    };
}

#endif // __CURIOUS_ORBITAL_TOY_HPP