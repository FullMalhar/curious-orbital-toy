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
static int numProcessors = 0;

static std::size_t num_frames_avg = 0;
static std::size_t num_frames_cpu = 0;

bool cot::metrics::setup(const std::size_t nFAvg, const std::size_t nFCpu)
{
    // Assign configured parameters
    num_frames_avg = nFAvg;
    num_frames_cpu = nFCpu;

    // Load font to use
    if (!sfFntMetrics.loadFromFile("cot.ttf"))
        return false;

    // Prepare text object
    sfTxtMetrics.setFont(sfFntMetrics);
    sfTxtMetrics.setCharacterSize(24);
    sfTxtMetrics.setFillColor(sf::Color::White);
    sfTxtMetrics.setStyle(sf::Text::Bold);

    // Open CPU info file
    FILE* fCpuInfo = fopen("/proc/cpuinfo", "r");
    if (!fCpuInfo)
        return false;

    // Extract processor number for current process
    const std::size_t line_length = 128;
    char line[line_length];
    while (std::fgets(line, line_length, fCpuInfo))
    {
        if (std::strncmp(line, "processor", 9) == 0)
        {
            numProcessors++;
        }
    }
    fclose(fCpuInfo);

    // Initial estimate of processor usage
    tms tSample;
    lastCPU = times(&tSample);
    lastUserCPU = tSample.tms_utime;
    lastSysCPU = tSample.tms_stime;

    return true;
}

void cot::metrics::update(const cot::math_t dt)
{
    // Calculate averaging framerate
    static const std::size_t num_frames_avg = 100;
    static cot::math_t fr[num_frames_avg];
    for (std::size_t i = num_frames_avg - 1; i > 0; i--)
    {
        fr[i] = fr[i - 1];
    }
    fr[0] = std::abs(1.0f / dt);
    cot::math_t fr_avg = 0.0f;
    for (std::size_t i = 0; i < num_frames_avg; i++)
    {
        fr_avg += fr[i];
    }
    fr_avg /= static_cast<cot::math_t>(num_frames_avg);
    
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

    static cot::math_t cpu_usage = 0.0f;
    static std::size_t cpu_usage_delay = num_frames_cpu + 1;
    if (cpu_usage_delay > num_frames_cpu)
    {
        // Estimate CPU usage time
        tms tSample;
        clock_t currCPU = times(&tSample);

        if (currCPU <= lastCPU || tSample.tms_stime < lastSysCPU || tSample.tms_utime < lastUserCPU)
        {
            cpu_usage = -1.0f;
        }
        else
        {
            cpu_usage = (tSample.tms_stime - lastSysCPU) + (tSample.tms_utime - lastUserCPU);
            cpu_usage /= (currCPU - lastCPU);
            cpu_usage /= numProcessors;
            cpu_usage *= 100;
        }

        // Update for next iteration
        lastCPU = currCPU;
        lastSysCPU = tSample.tms_stime;
        lastUserCPU = tSample.tms_utime;

        // Reset delay
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



