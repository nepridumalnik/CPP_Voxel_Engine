#include <graphics/VoxelRenderer.hpp>

#include <future>
#include <mutex>

namespace graphics
{

const int32_t VoxelRenderer::chunkAttributes_[] = {3, 2, 1, 0};
const uint32_t VoxelRenderer::atlasElement_ = 16;
const float VoxelRenderer::cubeSideSize_ = 0.5f;
const float VoxelRenderer::uvsize_ = 1.0f / VoxelRenderer::atlasElement_;
const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;

std::shared_ptr<Mesh> VoxelRenderer::Render(std::shared_ptr<voxels::Chunk> chunk)
{
    std::vector<std::future<void>> futures{voxels::ChunkHeight};

    for (int32_t y = 0; y < voxels::ChunkHeight; ++y)
    {
        futures[y] = std::async(std::launch::async, &VoxelRenderer::generateLayer, this, chunk, y);
    }
    for (auto &future : futures)
    {
        future.get();
    }

    return std::make_shared<graphics::Mesh>((float *)(buffer_.data()),
                                            buffer_.size() / VoxelRenderer::vertexSize_,
                                            VoxelRenderer::chunkAttributes_);
}

inline VoxelRenderer::Vertex VoxelRenderer::mkVtx(float x, float y, float z, float u, float v,
                                                  float l)
{
    return {x, y, z, u, v, l};
}

void VoxelRenderer::pushFace(const VoxelRenderer::Face &fc)
{
    static std::mutex mtx;
    std::unique_lock lock(mtx);

    buffer_.reserve(buffer_.size() + (fc.size() * fc[0].size()));
    for (uint32_t i = 0; i < fc.size(); ++i)
    {
        for (uint32_t j = 0; j < fc.size(); ++j)
        {
            buffer_.push_back(fc[i][j]);
        }
    }
}

void VoxelRenderer::generateLayer(std::shared_ptr<voxels::Chunk> chunk, int32_t y)
{
    // Light levels
    static constexpr LightLevel topFace = 1.0f;
    static constexpr LightLevel bottomFace = 0.75f;
    static constexpr LightLevel rightFace = 0.95f;
    static constexpr LightLevel leftFace = 0.85f;
    static constexpr LightLevel frontFace = 0.9f;
    static constexpr LightLevel backFace = 0.8f;

    LightLevel light = 0;

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

            const float u = (id % atlasElement_) * uvsize_;
            const float v = 1 - ((1 + id / atlasElement_) * uvsize_);

            if (!chunk->hasNeighbour(x, y + 1, z))
            {
                light = topFace;
                Face fc;

                fc[0] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v, light);
                fc[1] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                fc[2] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);

                fc[3] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v, light);
                fc[4] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);
                fc[5] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u, v, light);

                pushFace(fc);
            }
            if (!chunk->hasNeighbour(x, y - 1, z))
            {
                light = bottomFace;
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                fc[1] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                fc[2] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u, v + uvsize_, light);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                fc[4] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                fc[5] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);

                pushFace(fc);
            }
            if (!chunk->hasNeighbour(x + 1, y, z))
            {
                light = rightFace;
                Face fc;

                fc[0] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                fc[1] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v + uvsize_, light);
                fc[2] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);

                fc[3] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                fc[4] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);
                fc[5] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u, v, light);

                pushFace(fc);
            }
            if (!chunk->hasNeighbour(x - 1, y, z))
            {
                light = leftFace;
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                fc[1] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                fc[2] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, light);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                fc[4] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v, light);
                fc[5] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);

                pushFace(fc);
            }
            if (!chunk->hasNeighbour(x, y, z + 1))
            {
                light = frontFace;
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u, v, light);
                fc[1] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                fc[2] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u, v, light);
                fc[4] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v, light);
                fc[5] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);

                pushFace(fc);
            }
            if (!chunk->hasNeighbour(x, y, z - 1))
            {
                light = backFace;
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                fc[1] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v + uvsize_, light);
                fc[2] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, light);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                fc[4] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, light);
                fc[5] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u, v, light);

                pushFace(fc);
            }
        }
    }
}

inline float VoxelRenderer::lftVtx(uint32_t x)
{
    return x - cubeSideSize_;
}

inline float VoxelRenderer::rgtVtx(uint32_t x)
{
    return x + cubeSideSize_;
}

inline float VoxelRenderer::upVtx(uint32_t y)
{
    return y + cubeSideSize_;
}

inline float VoxelRenderer::downVtx(uint32_t y)
{
    return y - cubeSideSize_;
}

inline float VoxelRenderer::fwdVtx(uint32_t z)
{
    return z - cubeSideSize_;
}

inline float VoxelRenderer::bwdVtx(uint32_t z)
{
    return z + cubeSideSize_;
}

} // namespace graphics
