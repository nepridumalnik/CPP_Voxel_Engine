#include <graphics/Mesh.hpp>

namespace graphics
{

Mesh::Mesh(const float *buffer, uint32_t vertices, const int32_t *attributes) : vertices_(vertices)
{
    int32_t vertexSize = 0;

    for (int32_t i = 0; attributes[i]; ++i)
    {
        vertexSize += attributes[i];
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices_, buffer, GL_STATIC_DRAW);

    int32_t offset = 0;
    for (int32_t i = 0; attributes[i]; ++i)
    {
        glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, vertexSize * sizeof(float),
                              reinterpret_cast<void *>(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += attributes[i];
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
    glBindVertexArray(0);
}

} // namespace graphics
