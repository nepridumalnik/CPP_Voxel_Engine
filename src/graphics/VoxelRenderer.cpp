#include <graphics/VoxelRenderer.hpp>

#include <mutex>

namespace graphics
{

const int32_t VoxelRenderer::chunkAttributes_[] = {3, 2, 1, 0};
const uint32_t VoxelRenderer::atlasSide_ = 16;
const float VoxelRenderer::uvsize_ = 1.0f / VoxelRenderer::atlasSide_;
const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;

std::shared_ptr<Mesh> VoxelRenderer::Render(std::shared_ptr<voxels::Chunk> chunk)
{
    static constexpr ShapeDirection topShape = 1.0f;
    static constexpr ShapeDirection bottomShape = 0.75f;
    static constexpr ShapeDirection rightShape = 0.95f;
    static constexpr ShapeDirection leftShape = 0.85f;
    static constexpr ShapeDirection frontShape = 0.9f;
    static constexpr ShapeDirection backShape = 0.8f;

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

                ShapeDirection l = 0;
                const float u = (id % atlasSide_) * uvsize_;
                const float v = 1 - ((1 + id / atlasSide_) * uvsize_);

                if (!chunk->hasNeighbour(x, y + 1, z))
                {
                    l = topShape;
                    Shape shape;
                    shape[0] = makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v, l);
                    shape[1] =
                        makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                    shape[2] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);

                    shape[3] = makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v, l);
                    shape[4] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);
                    shape[5] = makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);

                    pushShape(shape);
                }
                if (!chunk->hasNeighbour(x, y - 1, z))
                {
                    l = bottomShape;
                    Shape shape;
                    shape[0] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    shape[1] =
                        makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                    shape[2] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize_, l);

                    shape[3] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    shape[4] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                    shape[5] =
                        makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);

                    pushShape(shape);
                }
                if (!chunk->hasNeighbour(x + 1, y, z))
                {
                    l = rightShape;
                    Shape shape;
                    shape[0] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                    shape[1] =
                        makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v + uvsize_, l);
                    shape[2] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);

                    shape[3] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                    shape[4] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);
                    shape[5] = makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);

                    pushShape(shape);
                }
                if (!chunk->hasNeighbour(x - 1, y, z))
                {
                    l = leftShape;
                    Shape shape;
                    shape[0] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    shape[1] =
                        makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                    shape[2] = makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize_, l);

                    shape[3] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    shape[4] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v, l);
                    shape[5] =
                        makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);

                    pushShape(shape);
                }
                if (!chunk->hasNeighbour(x, y, z + 1))
                {
                    l = frontShape;
                    Shape shape;
                    shape[0] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    shape[1] =
                        makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                    shape[2] = makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);

                    shape[3] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    shape[4] = makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v, l);
                    shape[5] =
                        makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);

                    pushShape(shape);
                }
                if (!chunk->hasNeighbour(x, y, z - 1))
                {
                    l = backShape;
                    Shape shape;
                    shape[0] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                    shape[1] =
                        makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v + uvsize_, l);
                    shape[2] = makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize_, l);

                    shape[3] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                    shape[4] = makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize_, l);
                    shape[5] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);

                    pushShape(shape);
                }
            }
        }
    }

    return std::make_shared<graphics::Mesh>((float *)(buffer_.data()),
                                            buffer_.size() / VoxelRenderer::vertexSize_,
                                            VoxelRenderer::chunkAttributes_);
}

inline VoxelRenderer::Vertex VoxelRenderer::makeVertex(float x, float y, float z, float u, float v,
                                                       float l)
{
    return {x, y, z, u, v, l};
}

void VoxelRenderer::pushShape(const VoxelRenderer::Shape &shape)
{
    static std::mutex mtx;
    std::unique_lock lock(mtx);

    buffer_.reserve(buffer_.size() + (shape.size() * shape[0].size()));
    for (uint32_t i = 0; i < shape.size(); ++i)
    {
        for (uint32_t j = 0; j < shape.size(); ++j)
        {
            buffer_.push_back(shape[i][j]);
        }
    }
}

} // namespace graphics
