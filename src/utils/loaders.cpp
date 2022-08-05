#include <utils/loaders.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

struct FreeTexture
{
    void operator()(void *texture)
    {
        stbi_image_free(texture);
    }
};

static constexpr uint32_t RequiredChannels = 4;

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

uint32_t LoadGLTexture(const std::string &file, int32_t &width, int32_t height)
{
    int32_t channels;
    std::unique_ptr<uint8_t, FreeTexture> textureData{
        stbi_load(file.c_str(), &width, &height, &channels, RequiredChannels)};

    if (!textureData)
    {
        throw std::runtime_error("Failed to load texture: " + file);
    }

    uint32_t texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 textureData.get());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

} // namespace utils
