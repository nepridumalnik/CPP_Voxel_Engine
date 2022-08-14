#include <voxels/VoxelRenderer.hpp>

#include <future>
#include <mutex>

namespace graphics
{

const int32_t VoxelRenderer::chunkAttributes_[] = {3, 2, 1, 0};
const uint32_t VoxelRenderer::atlasElement_ = 16;
const float VoxelRenderer::cubeSideSize_ = 0.5f;
const float VoxelRenderer::uvsize_ = 1.0f / VoxelRenderer::atlasElement_;
const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;

std::shared_ptr<Mesh> VoxelRenderer::Render(std::shared_ptr<voxels::Chunk> chunk,
                                            std::shared_ptr<voxels::Chunks> chunks)
{
    std::vector<std::future<void>> futures{voxels::ChunkHeight};
    chunks_ = chunks;

    for (uint32_t y = 0; y < voxels::ChunkHeight; ++y)
    {
        futures[y] =
            std::async(std::launch::deferred, &VoxelRenderer::generateLayer, this, chunk, y);
    }
    for (auto &future : futures)
    {
        future.get();
    }

    std::shared_ptr<graphics::Mesh> mesh = std::make_shared<graphics::Mesh>(
        buffer_.data(), buffer_.size() / VoxelRenderer::vertexSize_,
        VoxelRenderer::chunkAttributes_);

    buffer_.clear();
    chunks_ = nullptr;

    return mesh;
}

inline VoxelRenderer::Vertex VoxelRenderer::mkVtx(float x, float y, float z, float u, float v,
                                                  float l)
{
    return {x, y, z, u, v, l};
}

void VoxelRenderer::pushFace(const VoxelRenderer::Face &face)
{
    static std::mutex mtx;
    std::unique_lock lock(mtx);
    uint32_t size = buffer_.size();

    buffer_.resize(size + (face.size() * face[0].size()));
    for (uint32_t i = 0; i < face.size(); ++i)
    {
        for (uint32_t j = 0; j < face.size(); ++j)
        {
            buffer_[size++] = face[i][j];
        }
    }
}

void VoxelRenderer::generateLayer(std::shared_ptr<voxels::Chunk> chunk, uint32_t y)
{
    // Light levels
    static constexpr LightLevel top = 1.0f;
    static constexpr LightLevel bottom = 0.75f;
    static constexpr LightLevel right = 0.95f;
    static constexpr LightLevel left = 0.85f;
    static constexpr LightLevel front = 0.9f;
    static constexpr LightLevel back = 0.8f;

    for (uint32_t z = 0; z < voxels::ChunkDepth; ++z)
    {
        for (uint32_t x = 0; x < voxels::ChunkWidth; ++x)
        {
            const voxels::voxel &vox = chunk->GetVoxel(x, y, z);
            const uint32_t id = vox.id;

            if (voxels::BlockType::None == id)
            {
                continue;
            }

            const float u = (id % atlasElement_) * uvsize_;
            const float v = 1 - ((1 + id / atlasElement_) * uvsize_);

            if (!checkAllNeighbours(chunk, x, y + 1, z))
            {
                Face fc;

                fc[0] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v, top);
                fc[1] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, top);
                fc[2] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, top);

                fc[3] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v, top);
                fc[4] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, top);
                fc[5] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u, v, top);

                pushFace(fc);
            }
            if (!checkAllNeighbours(chunk, x, y - 1, z))
            {
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, bottom);
                fc[1] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, bottom);
                fc[2] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u, v + uvsize_, bottom);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, bottom);
                fc[4] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, bottom);
                fc[5] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, bottom);

                pushFace(fc);
            }
            if (!checkAllNeighbours(chunk, x + 1, y, z))
            {
                Face fc;

                fc[0] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, right);
                fc[1] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v + uvsize_, right);
                fc[2] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, right);

                fc[3] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, right);
                fc[4] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, right);
                fc[5] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u, v, right);

                pushFace(fc);
            }
            if (!checkAllNeighbours(chunk, x - 1, y, z))
            {
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, left);
                fc[1] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, left);
                fc[2] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, left);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u, v, left);
                fc[4] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v, left);
                fc[5] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, left);

                pushFace(fc);
            }
            if (!checkAllNeighbours(chunk, x, y, z + 1))
            {
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u, v, front);
                fc[1] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, front);
                fc[2] = mkVtx(lftVtx(x), upVtx(y), bwdVtx(z), u, v + uvsize_, front);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), bwdVtx(z), u, v, front);
                fc[4] = mkVtx(rgtVtx(x), downVtx(y), bwdVtx(z), u + uvsize_, v, front);
                fc[5] = mkVtx(rgtVtx(x), upVtx(y), bwdVtx(z), u + uvsize_, v + uvsize_, front);

                pushFace(fc);
            }
            if (!checkAllNeighbours(chunk, x, y, z - 1))
            {
                Face fc;

                fc[0] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, back);
                fc[1] = mkVtx(lftVtx(x), upVtx(y), fwdVtx(z), u + uvsize_, v + uvsize_, back);
                fc[2] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, back);

                fc[3] = mkVtx(lftVtx(x), downVtx(y), fwdVtx(z), u + uvsize_, v, back);
                fc[4] = mkVtx(rgtVtx(x), upVtx(y), fwdVtx(z), u, v + uvsize_, back);
                fc[5] = mkVtx(rgtVtx(x), downVtx(y), fwdVtx(z), u, v, back);

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

bool VoxelRenderer::checkAllNeighbours(std::shared_ptr<voxels::Chunk> chunk, int32_t x, int32_t y,
                                       int32_t z)
{
    const uint32_t w = chunk->GetX();
    const uint32_t h = chunk->GetY();
    const uint32_t d = chunk->GetZ();

    // Check vertical
    if (0 > y)
    {
        auto nChunk = chunks_->GetNeighbour(voxels::Neighbour::Down, w, h, d);
        if (nChunk && voxels::BlockType::None != nChunk->GetVoxel(x, voxels::ChunkHeight - 1, z).id)
        {
            return true;
        }
    }
    else if (y > (voxels::ChunkHeight - 1))
    {
        auto nChunk = chunks_->GetNeighbour(voxels::Neighbour::Up, w, h, d);
        if (nChunk && voxels::BlockType::None != nChunk->GetVoxel(x, 0, z).id)
        {
            return true;
        }
    }

    // Check horizontal
    if (0 > x)
    {
        auto nChunk = chunks_->GetNeighbour(voxels::Neighbour::Left, w, h, d);
        if (nChunk && voxels::BlockType::None != nChunk->GetVoxel(voxels::ChunkWidth - 1, y, z).id)
        {
            return true;
        }
    }
    else if (x > (voxels::ChunkWidth - 1))
    {
        auto nChunk = chunks_->GetNeighbour(voxels::Neighbour::Right, w, h, d);
        if (nChunk && voxels::BlockType::None != nChunk->GetVoxel(0, y, z).id)
        {
            return true;
        }
    }

    // Check frontal
    if (0 > z)
    {
        auto nChunk = chunks_->GetNeighbour(voxels::Neighbour::Front, w, h, d);
        if (nChunk && voxels::BlockType::None != nChunk->GetVoxel(x, y, voxels::ChunkDepth - 1).id)
        {
            return true;
        }
    }
    else if (z > (voxels::ChunkDepth - 1))
    {
        auto nChunk = chunks_->GetNeighbour(voxels::Neighbour::Back, w, h, d);
        if (nChunk && voxels::BlockType::None != nChunk->GetVoxel(x, y, 0).id)
        {
            return true;
        }
    }

    // Else

    return chunk->hasNeighbour(x, y, z);
}

} // namespace graphics
