// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <spdlog/spdlog.h>

#include <chrono>

int main(int argc, char **argv)
{
    // Set up logger to be really verbose
    spdlog::set_level(spdlog::level::debug);

    // Create window objects
    sf::RenderWindow sfWindow(sf::VideoMode(800, 600), "Curious Orbital Toy");
    sfWindow.setVerticalSyncEnabled(true);
    spdlog::debug("Created window.");

    // Timing objects
    std::chrono::time_point<std::chrono::system_clock> tBegin, tEnd;
    tBegin = std::chrono::system_clock::now();

    // Initialise engine
    cot::Engine pEng;
    spdlog::debug("Initialised engine.");

    // Add bodies from configuration
    cot::math_t cfg_mass;
    sf::Vector2f cfg_pos, cfg_vel;
    while (cot::cfgGetNextBody(cfg_mass, cfg_pos, cfg_vel))
    {
        pEng.addBody(cfg_mass, cfg_pos, cfg_vel);
        spdlog::info("Added body with mass {:.2} initial position ({:.2},{:.2}) and initial velocity ({:.2},{:.2}).", 
            cfg_mass, cfg_pos.x, cfg_pos.y, cfg_vel.x, cfg_vel.y);
    }

    // Program loop
    while (sfWindow.isOpen())
    {
        // Poll for events
        sf::Event sfEvent;
        while (sfWindow.pollEvent(sfEvent))
        {
            // Handle event
            switch (sfEvent.type)
            {
            case sf::Event::Closed:
                sfWindow.close();
            default:
                break;
            }
        }

        // Delta timing for elapsed time from last frame
        tEnd = std::chrono::system_clock::now();
        std::chrono::duration<cot::math_t, std::ratio<1, 1>> tDelta = tEnd - tBegin;
        tBegin = tEnd;

        // Update physics engine
        pEng.update(tDelta.count());

        // Clear window in preparation to display next frame
        sfWindow.clear(sf::Color::Black);

        // Draw new frame
        pEng.draw(sfWindow);

        // Display next frame
        sfWindow.display();
    }

    return 0;
}