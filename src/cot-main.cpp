// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <chrono>

static const cot::body_t bodyArr[] = {
    {100.0f, {300.0f, 300.0f}, {0.0f, 0.0f}},
    {5.0f, {10.0f, 10.0f}, {50.0f, 0.0f}}
};

int main(int argc, char **argv)
{
    // Create window objects
    sf::RenderWindow sfWindow(sf::VideoMode(800, 600), "Curious Orbital Toy");
    sfWindow.setVerticalSyncEnabled(true);

    // Timing objects
    std::chrono::time_point<std::chrono::system_clock> tBegin, tEnd;
    tBegin = std::chrono::system_clock::now();

    // Initialise engine
    cot::Engine pEng(bodyArr, sizeof(bodyArr) / sizeof(cot::body_t));

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