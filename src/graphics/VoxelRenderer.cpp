#include <graphics/VoxelRenderer.hpp>

namespace graphics
{

const int32_t VoxelRenderer::chunkAttributes_[] = {3, 2, 1, 0};
const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;

VoxelRenderer::VoxelRenderer(uint32_t capacity) : capacity_(capacity)
{
    buffer_.reserve(capacity_);
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
                const voxels::voxel &vox = chunk->GetVoxel(x, y, z);
                const uint32_t id = vox.id;

                if (!id)
                {
                    continue;
                }

                float l = 0;
                const float uvsize = 1.0f / 16.0f;
                const float u = (id % 16) * uvsize;
                const float v = 1 - ((1 + id / 16) * uvsize);

                if (!hasNeighbour(chunk, x, y + 1, z))
                {
                    l = 1.0f;
                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
                }
                if (!hasNeighbour(chunk, x, y - 1, z))
                {
                    l = 0.75f;
                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize, l);

                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }

                if (!hasNeighbour(chunk, x + 1, y, z))
                {
                    l = 0.95f;
                    pushVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    pushVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                }
                if (!hasNeighbour(chunk, x - 1, y, z))
                {
                    l = 0.85f;
                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }

                if (!hasNeighbour(chunk, x, y, z + 1))
                {
                    l = 0.9f;
                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }
                if (!hasNeighbour(chunk, x, y, z - 1))
                {
                    l = 0.8f;
                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
                    pushVertex(x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                }
            }
        }
    }

    return std::make_shared<graphics::Mesh>(buffer_.data(),
                                            buffer_.size() / VoxelRenderer::vertexSize_,
                                            VoxelRenderer::chunkAttributes_);
}

inline void VoxelRenderer::pushVertex(float x, float y, float z, float u, float v, float l)
{
    buffer_.push_back(x);
    buffer_.push_back(y);
    buffer_.push_back(z);
    buffer_.push_back(u);
    buffer_.push_back(v);
    buffer_.push_back(l);
}

inline bool VoxelRenderer::hasNeighbour(std::shared_ptr<voxels::Chunk> chunk, uint32_t x,
                                        uint32_t y, uint32_t z)
{
    if (x >= voxels::ChunkWidth || y >= voxels::ChunkHeight || z >= voxels::ChunkDepth)
    {
        return false;
    }

    return 0 != chunk->GetVoxel(x, y, z).id;
}

} // namespace graphics
