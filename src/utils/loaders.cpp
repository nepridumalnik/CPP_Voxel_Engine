#include <utils/loaders.hpp>

#include <png.h>

#include <fstream>
#include <memory>
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

uint32_t LoadGLTexture(const std::string &file, int32_t &width, int32_t &height)
{
    FILE *f{};
    int32_t alpha{};
    int32_t is_png{};
    int32_t bit_depth{};
    int32_t color_type{};
    int32_t row_bytes{};
    png_infop info{};
    png_infop endInfo{};
    png_byte header[8]{};
    png_byte *imageData{};
    png_bytepp row_pointers{};
    png_structp png{};
    GLuint texture{};

    static const auto finishAndThrow = [](const std::string &file) -> void {
        throw std::runtime_error{"Failed to read image: " + file};
    };

    if (0 != fopen_s(&f, file.c_str(), "rb"))
    {
        finishAndThrow(file);
    }

    fread(header, 1, sizeof(header), f);

    is_png = !png_sig_cmp(header, 0, sizeof(header));
    if (!is_png)
    {
        fclose(f);
        finishAndThrow(file);
    }
    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
    {
        fclose(f);
        finishAndThrow(file);
    }
    info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_read_struct(&png, (png_infopp) nullptr, (png_infopp) nullptr);
        fclose(f);
        finishAndThrow(file);
    }
    endInfo = png_create_info_struct(png);
    if (!endInfo)
    {
        png_destroy_read_struct(&png, (png_infopp) nullptr, (png_infopp) nullptr);
        fclose(f);
        finishAndThrow(file);
    }
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_read_struct(&png, &info, &endInfo);
        fclose(f);
        finishAndThrow(file);
    }
    png_init_io(png, f);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);
    png_get_IHDR(png, info, reinterpret_cast<png_uint_32 *>(&width),
                 reinterpret_cast<png_uint_32 *>(&height), &bit_depth, &color_type, nullptr,
                 nullptr, nullptr);

    width = width;
    height = height;

    png_read_update_info(png, info);
    row_bytes = png_get_rowbytes(png, info);
    imageData = (png_bytep)malloc(row_bytes * height * sizeof(png_byte));

    if (!imageData)
    {
        png_destroy_read_struct(&png, &info, &endInfo);
        fclose(f);
        finishAndThrow(file);
    }
    row_pointers = (png_bytepp)malloc(height * sizeof(png_bytep));
    if (!row_pointers)
    {
        png_destroy_read_struct(&png, &info, &endInfo);
        free(imageData);
        fclose(f);
        finishAndThrow(file);
    }
    for (unsigned int i = 0; i < height; ++i)
    {
        row_pointers[height - 1 - i] = imageData + i * row_bytes;
    }
    png_read_image(png, row_pointers);
    switch (png_get_color_type(png, info))
    {
    case PNG_COLOR_TYPE_RGBA:
        alpha = GL_RGBA;
        break;
    case PNG_COLOR_TYPE_RGB:
        alpha = GL_RGB;
        break;
    default:
        png_destroy_read_struct(&png, &info, &endInfo);
        finishAndThrow(file);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, alpha, GL_UNSIGNED_BYTE,
                 static_cast<void *>(imageData));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    png_destroy_read_struct(&png, &info, &endInfo);
    free(imageData);
    free(row_pointers);
    fclose(f);

    return texture;
}

} // namespace utils
