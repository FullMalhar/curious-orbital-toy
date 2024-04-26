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
        spdlog::debug("Opening config file.");
        fConfig.open("cot.csv", std::ios::in);
        if (!fConfig.is_open())
        {
            spdlog::error("Unable to open config file.");
            return 0;
        }

        // Read contents of configuration file into string and new stream
        spdlog::debug("Reading contents of configuration file.");
        isConfig << fConfig.rdbuf();
        sConfig = isConfig.str();
        strConfig.str(sConfig);

        // Close everything
        spdlog::debug("Closing config file.");
        fConfig.close();
    }

    // Check if another body is available
    std::string cLine;
    if (std::getline(strConfig, cLine))
    {
        spdlog::debug("Read config line '{0}'", cLine);

        // Check if line contains another body
        std::vector<std::string> vTokens;
        for (size_t pos = 0; (pos = cLine.find(',')) != std::string::npos; cLine.erase(0, pos + 1))
        {
            vTokens.push_back(cLine.substr(0, pos));
        }
        if (vTokens.size() != 5)
        {
            spdlog::error("Incorrectly read {0:d} tokens.", vTokens.size());
            return 0;
        }
    
        // Extract body data
        out_mass = std::stof(vTokens[0]);
        out_pos.x = std::stof(vTokens[1]);
        out_pos.y = std::stof(vTokens[2]);
        out_vel.x = std::stof(vTokens[3]);
        out_vel.y = std::stof(vTokens[4]);
        spdlog::debug("Read mass {:.2f} init position ({:.2f},{:.2f}) init velocity ({:.2f},{:.2f})", 
            out_mass, out_pos.x, out_pos.y, out_vel.x, out_vel.y);
        return 1;
    }
    
    spdlog::error("No bodies specified in config file.");
    return 0;
}