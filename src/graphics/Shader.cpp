#include <graphics/Shader.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>

static constexpr uint32_t BufferSize = 512;
static constexpr char ShaderDirectory[] = "shaders/";

Shader::Shader(uint32_t id) : id_(id)
{
}

Shader::~Shader()
{
    glDeleteProgram(id_);
}

void Shader::Use()
{
    glUseProgram(id_);
}

std::shared_ptr<Shader> Shader::LoadShader(const std::string &vFile, const std::string &fFile)
{
    std::string vSrc = readShaderSource(vFile);
    std::string fSrc = readShaderSource(fFile);

    uint32_t vShader = createShader(vSrc, GL_VERTEX_SHADER);
    uint32_t fShader = createShader(fSrc, GL_FRAGMENT_SHADER);

    uint32_t programId;

    if (0 == (programId = glCreateProgram()))
    {
        throw std::runtime_error("Failed to create program");
    }

    glAttachShader(programId, vShader);
    glAttachShader(programId, fShader);

    glLinkProgram(programId);

    int32_t result;
    char log[BufferSize];

    glGetProgramiv(programId, GL_LINK_STATUS, &result);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    if (GL_FALSE == result)
    {
        glGetProgramInfoLog(programId, sizeof(log), nullptr, log);
        throw std::runtime_error(std::string("Failed to link program with error: ") + log);
    }

    return std::make_shared<Shader>(programId);
}

std::string Shader::readShaderSource(const std::string &file)
{
    std::ifstream shaderFile(ShaderDirectory + file);

    if (!shaderFile.is_open())
    {
        throw std::runtime_error("Failed to read shader file: " + file);
    }

    std::stringstream ss;
    ss << shaderFile.rdbuf();

    return ss.str();
}

uint32_t Shader::createShader(const std::string &shaderSource, uint32_t type)
{
    uint32_t shaderId;
    if (0 == (shaderId = glCreateShader(type)))
    {
        throw std::runtime_error("Failed to create shader with type: " + std::to_string(type));
    }

    int32_t result;
    char log[BufferSize];
    const char *ptr[] = {shaderSource.c_str()};

    glShaderSource(shaderId, 1, ptr, nullptr);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

    if (GL_FALSE == result)
    {
        glGetShaderInfoLog(shaderId, sizeof(log), nullptr, log);
        glDeleteShader(shaderId);
        throw std::runtime_error(std::string("Failed to compile shader with error: ") + log);
    }

    return shaderId;
}
