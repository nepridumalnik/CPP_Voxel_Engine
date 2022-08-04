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
    static uint32_t createShader(const std::string &shaderSource, uint32_t type);

private:
    uint32_t id_;

private:
    friend std::shared_ptr<Shader> LoadShader(const std::string &vertexFile,
                                              const std::string &fragmentFile);
};

EXPORT std::shared_ptr<Shader> LoadShader(const std::string &vertexFile,
                                          const std::string &fragmentFile);

} // namespace graphics
