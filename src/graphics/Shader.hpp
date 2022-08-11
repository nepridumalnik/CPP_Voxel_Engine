#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace graphics
{

class EXPORT Shader
{
public:
    Shader(uint32_t id);
    ~Shader();

    void Use();
    void UniformMatrix(const std::string &name, glm::mat4 matrix);

private:
    uint32_t id_;
};

std::shared_ptr<Shader> EXPORT LoadShader(const std::string &vertexFile,
                                          const std::string &fragmentFile);

} // namespace graphics
