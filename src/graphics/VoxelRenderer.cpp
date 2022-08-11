#include <graphics/VoxelRenderer.hpp>

namespace graphics
{

const int32_t VoxelRenderer::chunkAttributes_[] = {3, 2, 1, 0};
const float VoxelRenderer::uvsize_ = 1.0f / VoxelRenderer::atlasSide_;
const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;
const uint32_t VoxelRenderer::atlasSide_ = 16;

std::shared_ptr<Mesh> VoxelRenderer::Render(std::shared_ptr<voxels::Chunk> chunk)
{
    using shape = float;
    static constexpr shape topShape = 1.0f;
    static constexpr shape bottomShape = 0.75f;
    static constexpr shape rightShape = 0.95f;
    static constexpr shape leftShape = 0.85f;
    static constexpr shape frontShape = 0.9f;
    static constexpr shape backShape = 0.8f;

    for (int32_t y = 0; y < voxels::ChunkHeight; ++y)
    {
        for (int32_t z = 0; z < voxels::ChunkDepth; ++z)
        {
            for (int32_t x = 0; x < voxels::ChunkWidth; ++x)
            {
                const voxels::voxel &vox = chunk->GetVoxel(x, y, z);
                const uint32_t id = vox.id;

                if (voxels::BlockType::None == id)
                {
                    continue;
                }

                shape l = 0;
                const float u = (id % VoxelRenderer::atlasSide_) * VoxelRenderer::uvsize_;
                const float v = 1 - ((1 + id / VoxelRenderer::atlasSide_) * VoxelRenderer::uvsize_);

                if (!chunk->hasNeighbour(x, y + 1, z))
                {
                    l = topShape;
                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + VoxelRenderer::uvsize_, l);

                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + VoxelRenderer::uvsize_, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
                }
                if (!chunk->hasNeighbour(x, y - 1, z))
                {
                    l = bottomShape;
                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v + VoxelRenderer::uvsize_, l);

                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                }
                if (!chunk->hasNeighbour(x + 1, y, z))
                {
                    l = rightShape;
                    pushVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + VoxelRenderer::uvsize_, l);

                    pushVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + VoxelRenderer::uvsize_, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                }
                if (!chunk->hasNeighbour(x - 1, y, z))
                {
                    l = leftShape;
                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u, v + VoxelRenderer::uvsize_, l);

                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                }
                if (!chunk->hasNeighbour(x, y, z + 1))
                {
                    l = frontShape;
                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                    pushVertex(x - 0.5f, y + 0.5f, z + 0.5f, u, v + VoxelRenderer::uvsize_, l);

                    pushVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    pushVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                }
                if (!chunk->hasNeighbour(x, y, z - 1))
                {
                    l = backShape;
                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_,
                               v + VoxelRenderer::uvsize_, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + VoxelRenderer::uvsize_, l);

                    pushVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + VoxelRenderer::uvsize_, v, l);
                    pushVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + VoxelRenderer::uvsize_, l);
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

} // namespace graphics
