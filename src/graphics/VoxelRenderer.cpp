#include <graphics/VoxelRenderer.hpp>

#define VERTEX_SIZE (3 + 2 + 1)

#define IS_IN(X, Y, Z)                                                                             \
    ((X) >= 0 && (X) < voxels::ChunkWidth && (Y) >= 0 && (Y) < voxels::ChunkHeight && (Z) >= 0 &&  \
     (Z) < voxels::ChunkDepth)
#define VOXEL(X, Y, Z)                                                                             \
    (chunk->operator[](((Y)*voxels::ChunkDepth + (Z)) * voxels::ChunkWidth + (X)))
#define IS_BLOCKED(X, Y, Z) ((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).id)

#define VERTEX(INDEX, X, Y, Z, U, V, L)                                                            \
    buffer_[INDEX + 0] = (X);                                                                      \
    buffer_[INDEX + 1] = (Y);                                                                      \
    buffer_[INDEX + 2] = (Z);                                                                      \
    buffer_[INDEX + 3] = (U);                                                                      \
    buffer_[INDEX + 4] = (V);                                                                      \
    buffer_[INDEX + 5] = (L);                                                                      \
    INDEX += VERTEX_SIZE;

static constexpr int32_t chunk_attrs[] = {3, 2, 1, 0};

static constexpr uint32_t VertexSize = 3 + 2 + 1;
static constexpr int32_t ChunkAttributes[] = {3, 2, 1, 0};

namespace graphics
{

const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;
const uint32_t VoxelRenderer::numberOfSides_ = 6;

VoxelRenderer::VoxelRenderer(uint32_t capacity) : capacity_(capacity)
{
    buffer_.resize(capacity_);
}

std::shared_ptr<Mesh> VoxelRenderer::Render(std::shared_ptr<voxels::Chunk> chunk)
{
    size_t index = 0;
    for (int y = 0; y < voxels::ChunkHeight; ++y)
    {
        for (int z = 0; z < voxels::ChunkDepth; ++z)
        {
            for (int x = 0; x < voxels::ChunkWidth; ++x)
            {
                voxels::voxel &vox =
                    chunk->operator[]((y * voxels::ChunkDepth + z) * voxels::ChunkWidth + x);
                unsigned int id = vox.id;

                if (!id)
                {
                    continue;
                }

                float l;
                float uvsize = 1.0f / 16.0f;
                float u = (id % 16) * uvsize;
                float v = 1 - ((1 + id / 16) * uvsize);

                if (!IS_BLOCKED(x, y + 1, z))
                {
                    l = 1.0f;
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
                }
                if (!IS_BLOCKED(x, y - 1, z))
                {
                    l = 0.75f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }

                if (!IS_BLOCKED(x + 1, y, z))
                {
                    l = 0.95f;
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                }
                if (!IS_BLOCKED(x - 1, y, z))
                {
                    l = 0.85f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }

                if (!IS_BLOCKED(x, y, z + 1))
                {
                    l = 0.9f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }
                if (!IS_BLOCKED(x, y, z - 1))
                {
                    l = 0.8f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                }
            }
        }
    }

    return std::make_shared<graphics::Mesh>(buffer_.data(), index / VERTEX_SIZE, chunk_attrs);
}

} // namespace graphics
