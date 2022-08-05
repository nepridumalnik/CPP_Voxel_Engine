#pragma once

#include <string>

namespace utils
{

std::string LoadFile(const std::string &file);

uint32_t LoadGLTexture(const std::string &file, int32_t &width, int32_t height);

} // namespace utils
