#include <graphics/Mesh.hpp>

namespace graphics
{

Mesh::Mesh(const float *buffer, uint32_t vertices, const int32_t *attributes) : vertices_(vertices)
{
    uint32_t vertexSize = 0;

    for (uint32_t i = 0; i < attributes[i]; ++i)
    {
        vertexSize += attributes[i];
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(float) * vertices_, buffer, GL_STATIC_DRAW);

    uint32_t offset = 0;

    for (uint32_t i = 0; attributes[i]; ++i)
    {
        uint32_t size = attributes[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), nullptr);
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void Mesh::Draw(uint32_t primitive)
{
    glBindVertexArray(vao_);
    glDrawArrays(primitive, 0, vertices_);
}

} // namespace graphics
