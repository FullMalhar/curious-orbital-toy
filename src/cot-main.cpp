// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <chrono>

int main(int argc, char **argv)
{
    // Set up logger to be really verbose
    auto logger = spdlog::basic_logger_mt("logger", "cot.log");
    logger->set_level(spdlog::level::debug);
    logger->info("Start of session.");

    // Retrieve configuration to use
    auto prog_cfg = cot::cfg::configure();

    // Create window objects
    sf::RenderWindow sfWindow(sf::VideoMode(prog_cfg.init_wind_width, prog_cfg.init_wind_height), "Curious Orbital Toy");
    sfWindow.setVerticalSyncEnabled(true);
    logger->debug("Created window.");

    // Timing objects
    std::chrono::time_point<std::chrono::system_clock> tBegin, tEnd;
    tBegin = std::chrono::system_clock::now();

    // Prepare metrics
    if (!cot::metrics::setup())
    {
        logger->error("Unable to prepare metrics.");
        return 0;
    }

    // Initialize engine
    cot::Engine pEng;
    logger->debug("Initialised engine.");

    // Add bodies from configuration
    cot::math_t cfg_mass;
    sf::Vector2f cfg_pos, cfg_vel;
    std::string _s;
    while (cot::cfgGetNextBody(logger, _s, cfg_mass, cfg_pos, cfg_vel))
    {
        pEng.addBody(_s, cfg_mass, cfg_pos, cfg_vel);
        logger->info(std::string("Added body '") + _s + 
            std::string("' with mass {:.2} initial position ({:.2},{:.2}) and initial velocity ({:.2},{:.2})."), 
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
                logger->info("End of session.");
            default:
                break;
            }
        }

        // Delta timing for elapsed time from last frame
        tEnd = std::chrono::system_clock::now();
        std::chrono::duration<cot::math_t, std::ratio<1, 1>> tDelta = tEnd - tBegin;
        tBegin = tEnd;
        cot::math_t dt = tDelta.count();

        // Update physics engine and metrics
        pEng.update(dt);
        cot::metrics::update(dt);

        // Clear window in preparation to display next frame
        sfWindow.clear(sf::Color::Black);

        // Draw new frame
        pEng.draw(sfWindow);
        cot::metrics::draw(sfWindow);

        // Display next frame
        sfWindow.display();

        // Publish if needed
        cot::processPublish(pEng, dt, logger);
    }

    return 0;
}