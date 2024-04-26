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
            std::pair<sf::Vector2f, sf::Vector2f> vForcePair = force2(std::make_pair(this->vSystem[i], this->vSystem[j]));

            // Add force interaction to the system force vector
            vForces[i].x += vForcePair.first.x;     vForces[i].y += vForcePair.first.y;
            vForces[j].x += vForcePair.second.x;    vForces[j].y += vForcePair.second.y;
        }
    }

    // Loop through each body in the scene
    for (std::size_t i = 0; i < this->vSystem.size(); i++)
    {
        // Calculate position movement due to velocity
        this->vSystem[i].position.x += this->vSystem[i].velocity.x * dt;
        this->vSystem[i].position.y += this->vSystem[i].velocity.y * dt;

        // Calculate next velocity based on force
        this->vSystem[i].velocity.x += vForces[i].x * dt / this->vSystem[i].mass;
        this->vSystem[i].velocity.y += vForces[i].y * dt / this->vSystem[i].mass;

        // Update force angle
        math_t forceVectorAngle = forceAngle(vForces[i]);

        // Update position from body object to circle shape
        this->vSystem[i].planet.setPosition(this->vSystem[i].position);

        // Transform arrow to face the force vector
        this->vSystem[i].arrow.setRotation(forceVectorAngle * 180.0f / M_PI);

        // Place arrow on the edge of the mass
        this->vSystem[i].arrow.setPosition(this->vSystem[i].position);
        this->vSystem[i].arrow.move(
            this->vSystem[i].planet.getRadius() * std::cos(forceVectorAngle), 
            this->vSystem[i].planet.getRadius() * std::sin(forceVectorAngle));
    }
}

void cot::Engine::draw(sf::RenderWindow& wind)
{
    // Loop through each body in the system
    for (auto& cBod : this->vSystem)
    {
        // Draw objects
        wind.draw(cBod.planet);
        wind.draw(cBod.arrow);
    }
}

void cot::Engine::addBody(math_t in_mass, sf::Vector2f init_pos, sf::Vector2f init_vel)
{
    // Create body based on mass and initial data
    body_t newBod;
    newBod.mass = in_mass;
    newBod.position = init_pos;
    newBod.velocity = init_vel;

    // Setup body graphics object
    newBod.planet.setFillColor(sf::Color::Yellow);
    newBod.planet.setRadius(in_mass);
    newBod.planet.setOrigin(in_mass / 2.0f, in_mass / 2.0f);

    // Setup force vector arrow object
    newBod.arrow.setPointCount(7);
    newBod.arrow.setPoint(0, sf::Vector2f(0.0f, -1.0f));
    newBod.arrow.setPoint(1, sf::Vector2f(4.0f, -1.0f));
    newBod.arrow.setPoint(2, sf::Vector2f(4.0f, -3.0f));
    newBod.arrow.setPoint(3, sf::Vector2f(6.0f, 0.0f));
    newBod.arrow.setPoint(4, sf::Vector2f(4.0f, 3.0f));
    newBod.arrow.setPoint(5, sf::Vector2f(4.0f, 1.0f));
    newBod.arrow.setPoint(6, sf::Vector2f(0.0f, 1.0f));
    newBod.arrow.setOrigin(0.0f, 0.0f);
    newBod.arrow.setScale(5.0f, 5.0f);

    // Add new body to vector
    this->vSystem.push_back(newBod);
}


