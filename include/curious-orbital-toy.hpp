// Curious Orbital Toy
// Malhar Palkar
#pragma once

#ifndef __CURIOUS_ORBITAL_TOY_HPP
#define __CURIOUS_ORBITAL_TOY_HPP

#include <SFML/Graphics.hpp>

#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

namespace cot
{
    // Precision math datatype
    typedef float math_t;

    // Representation of a physical body
    typedef struct _body
    {
        math_t              mass;       // Mass of body (used to determine radius)
        sf::Vector2f        position;   // Position of body (pixels)
        sf::Vector2f        velocity;   // Velocity of body (pixels/sec)
        sf::CircleShape     planet;     // Circular body object
        sf::ConvexShape     arrow;      // Force vector arrow
    } body_t;

    // Representation of a list of bodies
    typedef std::vector<cot::body_t> system_t;

    /**
     * @brief Returns the next body in the configuration file
    */
    unsigned int cfgGetNextBody(math_t& out_mass, sf::Vector2f& out_pos, sf::Vector2f& out_vel);
    
    // Physics engine
    class Engine
    {
    private:

        // Vector of all bodies and their corresponding shapes representing the system
        system_t vSystem;

    public:
        Engine();

        /**
         * @brief Adds a body to the physics engine
         * @param in_mass Mass of the body
         * @param init_pos Initial position (pixels) of the body
         * @param init_vel Initial velocity (pixels/sec) of the body
        */
        void addBody(math_t in_mass, sf::Vector2f init_pos, sf::Vector2f init_vel);

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