// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <ctgmath>

cot::Engine::Engine(const body_t *pBods, const std::size_t nBods)
{
    // Add each body to the vector
    for (auto i = 0; i < nBods; i++)
    {
        body_t cBody = *(pBods + i);

        sf::CircleShape cShape;
        cShape.setRadius(cBody.mass);
        cShape.setFillColor(sf::Color::Yellow);

        this->vSystem.push_back(std::make_pair(cBody, cShape));
    }
}

void cot::Engine::update(const cot::math_t dt)
{
    // List of forces for each object in the system
    std::vector<sf::Vector2f> vForces(this->vSystem.size());

    // Loop through each body combination in the scene
    for (std::size_t i = 0; i < this->vSystem.size(); i++)
    {
        for (std::size_t j = 0; j < i; j++)
        {
            // Calculate force interaction between the 2 bodies
            std::pair<sf::Vector2f, sf::Vector2f> vForcePair = cot::Engine::force2(std::make_pair(this->vSystem[i].first, this->vSystem[j].first));

            // Add force interaction to the system force vector
            vForces[i].x += vForcePair.first.x;     vForces[i].y += vForcePair.first.y;
            vForces[j].x += vForcePair.second.y;    vForces[j].y += vForcePair.second.y;
        }
    }

    // Loop through each body in the scene
    for (std::size_t i = 0; i < this->vSystem.size(); i++)
    {
        // Calculate position movement due to velocity
        vSystem[i].first.position.x += vSystem[i].first.velocity.x * dt;
        vSystem[i].first.position.y += vSystem[i].first.velocity.y * dt;

        // Calculate next velocity based on force
        vSystem[i].first.velocity.x += vForces[i].x * dt / vSystem[i].first.mass;
        vSystem[i].first.velocity.y += vForces[i].y * dt / vSystem[i].first.mass;
    }
}

void cot::Engine::draw(sf::RenderWindow& wind)
{
    // Loop through each body in the system
    for (auto& cBod : this->vSystem)
    {
        // Update position
        cBod.second.setPosition(cBod.first.position);

        // Draw shape
        wind.draw(cBod.second);
    }
}

std::pair<sf::Vector2f, sf::Vector2f> cot::Engine::force2(const std::pair<body_t, body_t>& bodyPair)
{
    // Gravitational constant
    static const math_t gConst = 6.6743E-11f;

    // Mass scaling factor
    static const math_t mScale = 1e7f;

    // Calculate distances and angles
    sf::Vector2f vDist;
    vDist.x = bodyPair.first.position.x - bodyPair.second.position.x;
    vDist.y = bodyPair.first.position.y - bodyPair.second.position.y;
    cot::math_t mDist = std::sqrt(std::pow(vDist.x, 2.0f) + std::pow(vDist.y, 2.0f));
    cot::math_t mAngle = std::atan(std::abs(vDist.y / vDist.x));

    // Calculate absolute value of force - zero if distance is non-zero
    cot::math_t mForce = (mDist != 0.0f ? gConst * bodyPair.first.mass * bodyPair.second.mass * std::pow(mScale, 2.0f) / std::pow(mDist, 2.0f) : 0.0f);

    // Decompose force vector into constituents
    sf::Vector2f vForceA;
    vForceA.x = std::copysign(mForce * std::cos(mAngle), -vDist.x);
    vForceA.y = std::copysign(mForce * std::sin(mAngle), -vDist.y);
    
    // Force on body B
    sf::Vector2f vForceB;
    vForceB.x = -vForceA.x;
    vForceB.y = -vForceA.y;

    return std::make_pair(vForceA, vForceB);
}

