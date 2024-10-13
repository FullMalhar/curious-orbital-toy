// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <sstream>
#include <sys/resource.h>
#include <unistd.h>

static sf::Font sfFntMetrics;
static sf::Text sfTxtMetrics;

bool cot::metrics::setup()
{
    // Load font to use
    if (!sfFntMetrics.loadFromFile("cot.ttf"))
        return false;

    // Prepare text object
    sfTxtMetrics.setFont(sfFntMetrics);
    sfTxtMetrics.setCharacterSize(24);
    sfTxtMetrics.setFillColor(sf::Color::White);
    sfTxtMetrics.setStyle(sf::Text::Bold);

    return true;
}

void cot::metrics::update(const cot::math_t dt)
{
    // Calculate averaging framerate
    static const std::size_t nFr = 100;
    static cot::math_t fr[nFr];
    for (std::size_t i = nFr - 1; i > 0; i--)
    {
        fr[i] = fr[i - 1];
    }
    fr[0] = std::abs(1.0f / dt);
    cot::math_t fr_avg = 0.0f;
    for (std::size_t i = 0; i < nFr; i++)
    {
        fr_avg += fr[i];
    }
    fr_avg /= static_cast<cot::math_t>(nFr);

    // Calculate current RAM usage
    static std::size_t cRam = 0;
    long rss = 0L;
    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp)
    {
        fscanf( fp, "%*s%ld", &rss );
        fclose(fp);
    }
    std::size_t mem = (size_t)rss * (size_t)sysconf( _SC_PAGESIZE) / (1024U * 1024U);

    // Generate metrics text
    std::ostringstream strMets;
    strMets.precision(0);
    strMets << "RAM: " << std::fixed << mem << "MB" << '\t';
    strMets << "FPS: " << std::fixed << std::abs(fr_avg);
    sfTxtMetrics.setString(strMets.str());
}

void cot::metrics::draw(sf::RenderWindow& wind)
{
    wind.draw(sfTxtMetrics);
}



