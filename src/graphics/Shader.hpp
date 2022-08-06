#pragma once

#include <utils/macros.hpp>

#include <memory>
#include <stdint.h>
#include <string>

namespace graphics
{

class EXPORT Shader
{
public:
    Shader(uint32_t id);
    ~Shader();

    void Use();

private:
    uint32_t id_;
};

EXPORT std::shared_ptr<Shader> LoadShader(const std::string &vertexFile,
                                          const std::string &fragmentFile);

} // namespace graphics
