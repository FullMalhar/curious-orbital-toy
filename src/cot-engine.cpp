// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <ctgmath>

/**
 * @brief Calculates the gravitational attraction force between 2 bodies
 * @param bodyPair Pair of bodies
 * @return Pair of force vectors corresponding to input parameter pair
*/
std::pair<sf::Vector2f, sf::Vector2f> force2(const std::pair<cot::body_t, cot::body_t>& bodyPair)
{
    // Gravitational constant
    static const cot::math_t gConst = 6.6743E-11f;

    // Mass scaling factor
    static const cot::math_t mScale = 1e7f;

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

/**
 * @brief Mathematical sign function
*/
inline cot::math_t sgn(const cot::math_t _i)
{
    return (_i > 0.0f) - (_i < 0.0f);
}

/**
 * @brief Calculates the angle based on a force vector
 * @param fVect Force vector
 * @return Angle of force vector in radians (0 - 2pi)
*/
inline cot::math_t forceAngle(const sf::Vector2f& fVect)
{
    // Mark angle as NaN if no force
    return ((fVect.x == 0.0f) && (fVect.y == 0.0f) ? NAN : (M_PI 
        - ((M_PI / 2.0f) * (1.0f + sgn(fVect.x)) * (1 - sgn(std::pow(fVect.y, 2.0f))))
        - ((M_PI / 4.0f) * (2.0f + sgn(fVect.x)) * sgn(fVect.y))
        - (sgn(fVect.x * fVect.y) * std::atan((std::abs(fVect.x) - std::abs(fVect.y)) / (std::abs(fVect.x) + std::abs(fVect.y))))));
}

cot::Engine::Engine(const body_t *pBods, const std::size_t nBods)
{
    // Generic circle shape to represent each body
    sf::CircleShape cShape;
    cShape.setFillColor(sf::Color::Yellow);

    // Generic arrow shape to represent each body
    sf::ConvexShape cArrow;
    cArrow.setPointCount(7);
    cArrow.setPoint(0, sf::Vector2f(0.0f, -1.0f));
    cArrow.setPoint(1, sf::Vector2f(4.0f, -1.0f));
    cArrow.setPoint(2, sf::Vector2f(4.0f, -3.0f));
    cArrow.setPoint(3, sf::Vector2f(6.0f, 0.0f));
    cArrow.setPoint(4, sf::Vector2f(4.0f, 3.0f));
    cArrow.setPoint(5, sf::Vector2f(4.0f, 1.0f));
    cArrow.setPoint(6, sf::Vector2f(0.0f, 1.0f));
    cArrow.setOrigin(0.0f, 0.0f);
    cArrow.setScale(5.0f, 5.0f);

    // Add each body to the vector
    for (auto i = 0; i < nBods; i++)
    {
        body_t cBody = *(pBods + i);

        // Set radius based on mass and adjust origin to center
        cShape.setRadius(cBody.mass);
        cShape.setOrigin(cShape.getRadius(), cShape.getRadius());

        this->vSystem.push_back(std::make_tuple(cBody, cShape, cArrow));
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
            std::pair<sf::Vector2f, sf::Vector2f> vForcePair = force2(std::make_pair(std::get<0>(this->vSystem[i]), std::get<0>(this->vSystem[j])));

            // Add force interaction to the system force vector
            vForces[i].x += vForcePair.first.x;     vForces[i].y += vForcePair.first.y;
            vForces[j].x += vForcePair.second.x;    vForces[j].y += vForcePair.second.y;
        }
    }

    // Loop through each body in the scene
    for (std::size_t i = 0; i < this->vSystem.size(); i++)
    {
        // Calculate position movement due to velocity
        std::get<0>(this->vSystem[i]).position.x += std::get<0>(this->vSystem[i]).velocity.x * dt;
        std::get<0>(this->vSystem[i]).position.y += std::get<0>(this->vSystem[i]).velocity.y * dt;

        // Calculate next velocity based on force
        std::get<0>(this->vSystem[i]).velocity.x += vForces[i].x * dt / std::get<0>(this->vSystem[i]).mass;
        std::get<0>(this->vSystem[i]).velocity.y += vForces[i].y * dt / std::get<0>(this->vSystem[i]).mass;

        // Update force angle
        math_t forceVectorAngle = forceAngle(vForces[i]);

        // Update position from body object to circle shape
        std::get<1>(this->vSystem[i]).setPosition(std::get<0>(this->vSystem[i]).position);

        // Transform arrow to face the force vector
        std::get<2>(this->vSystem[i]).setRotation(forceVectorAngle * 180.0f / M_PI);

        // Place arrow on the edge of the mass
        std::get<1>(this->vSystem[i]).getRadius();
        std::get<2>(this->vSystem[i]).setPosition(std::get<0>(this->vSystem[i]).position);
        std::get<2>(this->vSystem[i]).move(
            std::get<1>(this->vSystem[i]).getRadius() * std::cos(forceVectorAngle), 
            std::get<1>(this->vSystem[i]).getRadius() * std::sin(forceVectorAngle));
    }
}

void cot::Engine::draw(sf::RenderWindow& wind)
{
    // Loop through each body in the system
    for (auto& cBod : this->vSystem)
    {
        // Draw objects
        wind.draw(std::get<1>(cBod));
        wind.draw(std::get<2>(cBod));
    }
}



