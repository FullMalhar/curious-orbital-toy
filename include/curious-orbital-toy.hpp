// Curious Orbital Toy
// Malhar Palkar
#pragma once

#ifndef __CURIOUS_ORBITAL_TOY_HPP
#define __CURIOUS_ORBITAL_TOY_HPP

#include <SFML/Graphics.hpp>

#include <tuple>
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

    /**
     * @brief Datatype representing the system, consisting of array of tuple
     * @details Tuple contains (1) body data, (2) circle shape, (3) force vector arrow shape
    */
    typedef std::vector<std::tuple<body_t, sf::CircleShape, sf::ConvexShape>> system_t;
    
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
    };
}

#endif // __CURIOUS_ORBITAL_TOY_HPP