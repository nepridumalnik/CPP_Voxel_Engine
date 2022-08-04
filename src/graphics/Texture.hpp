#pragma once

#include <memory>
#include <stdint.h>
#include <string>

namespace graphics
{

class Texture
{
public:
    Texture(uint32_t id);
    ~Texture();

    void Bind();

private:
    uint32_t id_;
};

std::shared_ptr<Texture> LoadTexture(const std::string &tFile);

} // namespace graphics
