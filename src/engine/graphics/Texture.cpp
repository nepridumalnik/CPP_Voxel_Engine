#include <graphics/Texture.hpp>

#include <utils/loaders.hpp>

static constexpr char TextureDirectory[] = "textures/";

namespace graphics
{

Texture::Texture(uint32_t id, uint32_t width, uint32_t height)
    : id_(id), width_(width), height_(height)
{
}

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
    int32_t width;
    int32_t height;
    uint32_t texture = utils::LoadGLTexture(TextureDirectory + tFile, width, height);

    return std::make_shared<Texture>(texture, width, height);
}

} // namespace graphics
