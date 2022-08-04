#include <graphics/Texture.hpp>

namespace graphics
{

Texture::Texture(uint32_t id) : id_(id) {}

Texture::~Texture()
{
    glDeleteTextures(1, &id_);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

std::shared_ptr<Texture> LoadTexture(const std::string &tFile)
{
    return nullptr;
}

} // namespace graphics
