#pragma once

#include <cstdint>

namespace graphics
{

class EXPORT Mesh
{
public:
    Mesh(const float *buffer, uint32_t vertices, const int32_t *attributes);
    ~Mesh();

    void Draw(uint32_t primitive);

private:
    uint32_t vao_;
    uint32_t vbo_;
    uint32_t vertices_;
};

} // namespace graphics
