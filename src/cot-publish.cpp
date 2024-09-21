// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <sstream>

// How often to publish the system states
static const cot::math_t param_publishInterval = 0.1f;

void cot::processPublish(Engine& eng, const math_t dt, std::shared_ptr<spdlog::logger> logger)
{
    static auto publish_timer = 0.0f;

    // Proceed only if the publish interval has passed
    publish_timer += dt;
    if (publish_timer < param_publishInterval)
        return;

    // Reset publish interval timer
    publish_timer = 0.0f;

    // Publish all bodies in the system
    auto pStates = eng.publish();

    // Write published bodies to string
    std::ostringstream ss_publish;
    for (const auto& cState : pStates)
    {
        ss_publish << "Body '" << cState.name << "' with mass " << cState.mass;
        ss_publish << " has (x,y)=(" << cState.position.x << "," << cState.position.y;
        ss_publish << ") and (dx,dy)/dt=(" << cState.velocity.x << "," << cState.velocity.y << ")" << std::endl;
    }

    // Send the current telemetry
    logger->info(ss_publish.str());
}