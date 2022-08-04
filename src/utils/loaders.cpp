#include <utils/loaders.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace utils
{

std::string LoadFile(const std::string &file)
{
    std::ifstream shaderFile(file);

    if (!shaderFile.is_open())
    {
        throw std::runtime_error("Failed to read shader file: " + file);
    }

    std::stringstream ss;
    ss << shaderFile.rdbuf();

    return ss.str();
}

} // namespace utils
