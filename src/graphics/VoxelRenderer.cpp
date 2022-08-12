#include <graphics/VoxelRenderer.hpp>

#include <future>
#include <mutex>

namespace graphics
{

const int32_t VoxelRenderer::chunkAttributes_[] = {3, 2, 1, 0};
const uint32_t VoxelRenderer::atlasSide_ = 16;
const float VoxelRenderer::uvsize_ = 1.0f / VoxelRenderer::atlasSide_;
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

inline VoxelRenderer::Vertex VoxelRenderer::makeVertex(float x, float y, float z, float u, float v,
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
    static constexpr FaceDirection topFace = 1.0f;
    static constexpr FaceDirection bottomFace = 0.75f;
    static constexpr FaceDirection rightFace = 0.95f;
    static constexpr FaceDirection leftFace = 0.85f;
    static constexpr FaceDirection frontFace = 0.9f;
    static constexpr FaceDirection backFace = 0.8f;

    FaceDirection l = 0;

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

            const float u = (id % atlasSide_) * uvsize_;
            const float v = 1 - ((1 + id / atlasSide_) * uvsize_);

            if (!chunk->hasNeighbour(x, y + 1, z))
            {
                l = topFace;
                Face face;
                face[0] = makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v, l);
                face[1] = makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                face[2] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);

                face[3] = makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v, l);
                face[4] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);
                face[5] = makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x, y - 1, z))
            {
                l = bottomFace;
                Face face;
                face[0] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                face[1] = makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                face[2] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize_, l);

                face[3] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                face[4] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                face[5] = makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x + 1, y, z))
            {
                l = rightFace;
                Face face;
                face[0] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                face[1] = makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v + uvsize_, l);
                face[2] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);

                face[3] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                face[4] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);
                face[5] = makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x - 1, y, z))
            {
                l = leftFace;
                Face face;
                face[0] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                face[1] = makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                face[2] = makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize_, l);

                face[3] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                face[4] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v, l);
                face[5] = makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x, y, z + 1))
            {
                l = frontFace;
                Face face;
                face[0] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                face[1] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);
                face[2] = makeVertex(x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize_, l);

                face[3] = makeVertex(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                face[4] = makeVertex(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize_, v, l);
                face[5] = makeVertex(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize_, v + uvsize_, l);

                pushFace(face);
            }
            if (!chunk->hasNeighbour(x, y, z - 1))
            {
                l = backFace;
                Face face;
                face[0] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                face[1] = makeVertex(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize_, v + uvsize_, l);
                face[2] = makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize_, l);

                face[3] = makeVertex(x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize_, v, l);
                face[4] = makeVertex(x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize_, l);
                face[5] = makeVertex(x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);

                pushFace(face);
            }
        }
    }
}

} // namespace graphics
