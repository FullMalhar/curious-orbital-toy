// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <sstream>

void cot::processPublish(Engine& eng, const math_t dt, std::shared_ptr<spdlog::logger> logger, const math_t interv)
{
    static auto publish_timer = 0.0f;

    // Proceed only if the publish interval has passed
    publish_timer += dt;
    if (publish_timer < interv)
        return;

    // Reset publish interval timer
    publish_timer = 0.0f;

    // Publish all bodies in the system
    auto pStates = eng.publish();

    // Write published bodies to string
    for (const auto& cState : pStates)
    {
        // Print format string to stream
        std::ostringstream ss_publish;
        ss_publish << "Body '" << cState.name << "' with mass " << cState.mass;
        ss_publish << " has (x,y)=(" << cState.position.x << "," << cState.position.y;
        ss_publish << ") and (dx,dy)/dt=(" << cState.velocity.x << "," << cState.velocity.y << ")";

        // Dump to log stream
        logger->info(ss_publish.str());
    }
}