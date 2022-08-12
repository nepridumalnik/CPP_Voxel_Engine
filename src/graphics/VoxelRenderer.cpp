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

inline VoxelRenderer::Vertex VoxelRenderer::makeVtx(float x, float y, float z, float u, float v,
                                                    float l)
{
    return {x, y, z, u, v, l};
}

void VoxelRenderer::pushFace(const VoxelRenderer::Face &face)
{
    static std::mutex mtx;
    std::unique_lock lock(mtx);

    buffer_.reserve(buffer_.size() + (face.size() * face[0].size()));
    for (uint32_t i = 0; i < face.size(); ++i)
    {
        for (uint32_t j = 0; j < face.size(); ++j)
        {
            buffer_.push_back(face[i][j]);
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
                Face face;

                face[0] = makeVtx(leftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v, light);
                face[1] = makeVtx(leftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                face[2] = makeVtx(rightVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);

                face[3] = makeVtx(leftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v, light);
                face[4] = makeVtx(rightVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);
                face[5] = makeVtx(rightVtx(x), upVtx(y), fwdVtx(z), u, v, light);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x, y - 1, z))
            {
                light = bottomFace;
                Face face;

                face[0] = makeVtx(leftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                face[1] =
                    makeVtx(rightVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                face[2] = makeVtx(leftVtx(x), downVtx(y), bwdVtx(z), u, v + uvsize_, light);

                face[3] = makeVtx(leftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                face[4] = makeVtx(rightVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                face[5] =
                    makeVtx(rightVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x + 1, y, z))
            {
                light = rightFace;
                Face face;

                face[0] = makeVtx(rightVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                face[1] =
                    makeVtx(rightVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v + uvsize_, light);
                face[2] = makeVtx(rightVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);

                face[3] = makeVtx(rightVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                face[4] = makeVtx(rightVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);
                face[5] = makeVtx(rightVtx(x), downVtx(y), bwdVtx(z), u, v, light);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x - 1, y, z))
            {
                light = leftFace;
                Face face;

                face[0] = makeVtx(leftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                face[1] = makeVtx(leftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                face[2] = makeVtx(leftVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, light);

                face[3] = makeVtx(leftVtx(x), downVtx(y), fwdVtx(z), u, v, light);
                face[4] = makeVtx(leftVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v, light);
                face[5] = makeVtx(leftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x, y, z + 1))
            {
                light = frontFace;
                Face face;

                face[0] = makeVtx(leftVtx(x), downVtx(y), bwdVtx(z), u, v, light);
                face[1] =
                    makeVtx(rightVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);
                face[2] = makeVtx(leftVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, light);

                face[3] = makeVtx(leftVtx(x), downVtx(y), bwdVtx(z), u, v, light);
                face[4] = makeVtx(rightVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v, light);
                face[5] =
                    makeVtx(rightVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, light);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x, y, z - 1))
            {
                light = backFace;
                Face face;

                face[0] = makeVtx(leftVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                face[1] = makeVtx(leftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v + uvsize_, light);
                face[2] = makeVtx(rightVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, light);

                face[3] = makeVtx(leftVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, light);
                face[4] = makeVtx(rightVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, light);
                face[5] = makeVtx(rightVtx(x), downVtx(y), fwdVtx(z), u, v, light);

                pushFace(face);
            }
        }
    }
}

inline float VoxelRenderer::leftVtx(uint32_t x)
{
    return x - cubeSideSize_;
}

inline float VoxelRenderer::rightVtx(uint32_t x)
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
