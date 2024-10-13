// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <iomanip>
#include <sstream>
#include <sys/resource.h>
#include <sys/times.h>
#include <unistd.h>

static sf::Font sfFntMetrics;
static sf::Text sfTxtMetrics;

static clock_t lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;

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

    // Prepare to collect CPU usage data
    FILE* file;
    struct tms timeSample;
    char line[128];
    lastCPU = times(&timeSample);
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;
    file = fopen("/proc/cpuinfo", "r");
    numProcessors = 0;
    while(fgets(line, 128, file) != NULL){
        if (strncmp(line, "processor", 9) == 0) numProcessors++;
    }
    fclose(file);

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

    // Calculate CPU usage
    static cot::math_t cpu_usage = 0.0f;
    static std::size_t cpu_usage_delay = 11;
    if (cpu_usage_delay > 100)
    {
        struct tms timeSample;
        clock_t now;
        now = times(&timeSample);
        if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
            timeSample.tms_utime < lastUserCPU)
        {
            cpu_usage = -1.0;
        }
        else
        {
            cpu_usage = (timeSample.tms_stime - lastSysCPU) + (timeSample.tms_utime - lastUserCPU);
            cpu_usage /= (now - lastCPU);
            cpu_usage /= numProcessors;
            cpu_usage *= 100;
        }
        lastCPU = now;
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;
        cpu_usage_delay = 0;
    }
    cpu_usage_delay++;

    // Generate metrics text
    std::ostringstream strMets;
    strMets.setf(std::ios::fixed);
    strMets << "CPU: " << std::setprecision(2) << std::setw(5) << cpu_usage << "% " << '\t';
    strMets << "RAM: " << std::setprecision(0) << mem << "MB" << '\t';
    strMets << "FPS: " << std::setprecision(0) << std::abs(fr_avg);
    sfTxtMetrics.setString(strMets.str());
}

void cot::metrics::draw(sf::RenderWindow& wind)
{
    wind.draw(sfTxtMetrics);
}



