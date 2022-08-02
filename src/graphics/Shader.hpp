#pragma once

#include <memory>
#include <stdint.h>
#include <string>

class Shader
{
public:
    Shader(uint32_t id);
    ~Shader();

    void Use();

    static std::shared_ptr<Shader> LoadShader(const std::string &vertexFile,
                                              const std::string &fragmentFile);

private:
    static std::string readShaderSource(const std::string &file);
    static uint32_t createShader(const std::string &shaderSource, uint32_t type);

private:
    uint32_t id_;
};
