#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace graphics
{

class EXPORT Texture
{
public:
    Texture(uint32_t id, uint32_t width, uint32_t height);
    ~Texture();

    void Bind();

private:
    uint32_t id_;
    uint32_t width_;
    uint32_t height_;
};

std::shared_ptr<Texture> EXPORT LoadTexture(const std::string &tFile);

} // namespace graphics
