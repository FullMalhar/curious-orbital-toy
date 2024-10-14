// Curious Orbital Toy
// Malhar Palkar
#pragma once

#ifndef __CURIOUS_ORBITAL_TOY_HPP
#define __CURIOUS_ORBITAL_TOY_HPP

#include <SFML/Graphics.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <memory>
#include <vector>

namespace cot
{
    // Precision math datatype
    typedef float math_t;

    // Representation of a state of a physical body
    typedef struct _state
    {
        math_t              mass;       // Mass of body (used to determine radius)
        sf::Vector2f        position;   // Position of body (pixels)
        sf::Vector2f        velocity;   // Velocity of body (pixels/sec)
        std::string         name;       // Name of body
    } state_t;

    // Representation of a physical body
    typedef struct _body : _state
    {
        sf::CircleShape                 planet;     // Circular body object
        sf::ConvexShape                 arrow;      // Force vector arrow
        std::vector<sf::CircleShape>    history;    // Persistence history
        std::size_t                     stamps;     // Number of stamps recorded in persistence history
    } body_t;

    // Representation of a list of bodies
    typedef std::vector<cot::body_t> system_t;

    /**
     * @brief Returns the next body in the configuration file
    */
    unsigned int cfgGetNextBody(std::shared_ptr<spdlog::logger> logger, std::string& out_name, math_t& out_mass, sf::Vector2f& out_pos, sf::Vector2f& out_vel);
    
    // Physics engine
    class Engine
    {
    private:

        // Vector of all bodies and their corresponding shapes representing the system
        system_t vSystem;

        // Number of persistence points
        std::size_t nPersist;

    public:

        /**
         * @param nPerst Number of persistence points
        */
        Engine(const std::size_t nPerst);

        /**
         * @brief Adds a body to the physics engine
         * @param in_mass Mass of the body
         * @param init_pos Initial position (pixels) of the body
         * @param init_vel Initial velocity (pixels/sec) of the body
        */
        void addBody(std::string in_name, math_t in_mass, sf::Vector2f init_pos, sf::Vector2f init_vel);

        /**
         * @brief Allows the physics engine to update force, velocity, and position of each body in the system
         * @param dt Time since the update function was last called
         * @param n_persist Number of points to use for persistence
        */
        void update(const math_t dt);

        /**
         * @brief Publishes the current state of every body in the system
         * @return An array of states for every body in the system, must be deallocated after processed
         * @param out_sz The number of states published
        */
        std::vector<state_t> publish();

        /**
         * @brief Draws all bodies in the system in their last updated position
         * @param wind Reference to drawing window
        */
        void draw(sf::RenderWindow& wind);
    };
    
    /**
     * @brief Handles the publishing from an engine
     * @param eng Engine to publish
     * @param dt Time elapsed since the last frame
     * @param logger Reference to logging object
     * @param interv Publishing interval (s)
    */
   void processPublish(Engine& eng, const math_t dt, std::shared_ptr<spdlog::logger> logger, const math_t interv);

    namespace metrics
    {
        /**
         * @brief Sets up the metrics calculations
         * @param nFAvg Number of frames to average when calculating framerate (must be >1)
         * @param nFCpu Calculate CPU and RAM usage every nFCpu frames
         * @return Whether metrics setup was successful or not
        */
        bool setup(const std::size_t nFAvg, const std::size_t nFCpu);

        /**
         * @brief Allows the metrics calculations to update
         * @param dt Time since the update function was last called
        */
        void update(const cot::math_t dt);

        /**
         * @brief Draws the on-screen metrics
         * @param wind Reference to drawing window
        */
        void draw(sf::RenderWindow& wind);
    }

    namespace cfg
    {
        // Program configuration
        typedef struct _config
        {
            std::size_t  n_persist_pts      = 400;      // Number of points to use for persistence
            std::size_t  n_frames_avg       = 100;      // Number of frames to use to calculate average framerate
            std::size_t  n_frames_cpu       = 100;      // CPU usage calculated every nth frame    
            cot::math_t  publish_interval   = 0.1f;     // How often to publish data (s)
            unsigned int init_wind_width    = 800;      // Initial window width (pixels)
            unsigned int init_wind_height   = 600;      // initial window height (pixels)
        } config_t;

        /**
         * @brief Retrieves the program configuration or the precompiled defaults if none exists
         * @return Configuration object with the determined system configuration
        */
        config_t configure();
    }    
}

#endif // __CURIOUS_ORBITAL_TOY_HPP