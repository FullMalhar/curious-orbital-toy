// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.hpp>

#include <fstream>
#include <sstream>

static std::string sConfig;
static std::stringstream strConfig;

unsigned int cot::cfgGetNextBody(math_t& out_mass, sf::Vector2f& out_pos, sf::Vector2f& out_vel)
{
    // Check if configuration file has been read
    if (sConfig.length() <= 0)
    {
        std::fstream fConfig;
        std::stringstream isConfig;

        // Try to open configuration file
        fConfig.open("cot.csv", std::ios::in);
        if (!fConfig.is_open())
            return 0;

        // Read contents of configuration file into string and new stream
        isConfig << fConfig.rdbuf();
        sConfig = isConfig.str();
        strConfig.str(sConfig);

        // Close everything
        fConfig.close();
    }

    // Check if another body is available
    std::string cLine;
    if (std::getline(strConfig, cLine))
    {
        // Check if line contains another body
        size_t pos = 0;
        std::vector<std::string> vTokens;
        while ((pos = cLine.find(',')) != std::string::npos)
        {
            vTokens.push_back(cLine.substr(0, pos));
            cLine.erase(0, pos + 1);
        }
        if (vTokens.size() != 5)
            return 0;
    
        // Extract body data
        out_mass = std::stof(vTokens[0]);
        out_pos.x = std::stof(vTokens[1]);
        out_pos.y = std::stof(vTokens[2]);
        out_vel.x = std::stof(vTokens[3]);
        out_vel.y = std::stof(vTokens[4]);
        return 1;
    }
    
    return 0;
}