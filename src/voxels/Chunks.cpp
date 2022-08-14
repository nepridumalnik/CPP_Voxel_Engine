#include <voxels/Chunks.hpp>

#include <future>
#include <vector>

namespace voxels
{

Chunks::Chunks(uint32_t w, uint32_t h, uint32_t d) : w_(w), h_(h), d_(d), volume_(w_ * h_ * d_)
{
    std::vector<std::future<std::shared_ptr<Chunk>>> futures;

    futures.resize(volume_);
    chunks_.resize(volume_);

    for (uint32_t idx = 0, y = 0; y < h_; ++y)
    {
        for (uint32_t z = 0; z < d_; ++z)
        {
            for (uint32_t x = 0; x < w_; ++x)
            {
                static const auto lambda = [](uint32_t x, uint32_t y,
                                              uint32_t z) -> std::shared_ptr<Chunk> {
                    return std::make_shared<Chunk>(x, y, z);
                };
                futures[idx++] = std::async(std::launch::async, lambda, x, y, z);
            }
        }
    }

    for (uint32_t i = 0; i < futures.size(); ++i)
    {
        chunks_[i] = futures[i].get();
    }
}

uint32_t Chunks::Size()
{
    return chunks_.size();
}

std::shared_ptr<Chunk> Chunks::At(uint32_t idx)
{
    return chunks_[idx];
}

std::shared_ptr<Chunk> Chunks::GetChunkByChunksPos(uint32_t x, uint32_t y, uint32_t z)
{
    if (x > w_ || y > h_)
    {
        return nullptr;
    }

    const uint32_t idx = (y * d_ + z) * w_ + x;

    if (chunks_.size() > idx)
    {
        return chunks_[idx];
    }

    return nullptr;
}

Chunks::NullableReference<voxels::voxel> Chunks::GetVoxelByCoord(uint32_t x, uint32_t y, uint32_t z)
{
    int32_t cx = x / ChunkWidth;
    int32_t cy = y / ChunkHeight;
    int32_t cz = z / ChunkDepth;

    if (x < 0)
    {
        --cx;
    }
    if (y < 0)
    {
        --cy;
    }
    if (z < 0)
    {
        --cz;
    }

    if (cx < 0 || cy < 0 || cz < 0 || cx >= w_ || cy >= h_ || cz >= d_)
    {
        return {};
    }

    const uint32_t chunkIdx = (cy * d_ + cz) * w_ + cx;
    std::shared_ptr<Chunk> chunk = chunks_[chunkIdx];

    const uint32_t lx = x - cx * ChunkWidth;
    const uint32_t ly = y - cy * ChunkHeight;
    const uint32_t lz = z - cz * ChunkDepth;

    // const uint32_t voxelIdx = (ly * ChunkDepth + lz) * ChunkWidth + lx;

    return chunk->GetVoxel(lx, ly, lz);
}

void Chunks::SetVoxelByCoord(uint32_t x, uint32_t y, uint32_t z, BlockType type)
{
    int32_t cx = x / ChunkWidth;
    int32_t cy = y / ChunkHeight;
    int32_t cz = z / ChunkDepth;

    if (x < 0)
    {
        --cx;
    }
    if (y < 0)
    {
        --cy;
    }
    if (z < 0)
    {
        --cz;
    }

    if (cx < 0 || cy < 0 || cz < 0 || cx >= w_ || cy >= h_ || cz >= d_)
    {
        return;
    }

    const uint32_t chunkIdx = (cy * d_ + cz) * w_ + cx;
    std::shared_ptr<Chunk> chunk = chunks_[chunkIdx];

    const uint32_t lx = x - cx * ChunkWidth;
    const uint32_t ly = y - cy * ChunkHeight;
    const uint32_t lz = z - cz * ChunkDepth;

    if (0 == lx && (chunk = GetChunkByChunksPos(cx - 1, cy, cz)))
    {
        chunk->SetModified(true);
    }
    if (0 == ly && (chunk = GetChunkByChunksPos(cx, cy - 1, cz)))
    {
        chunk->SetModified(true);
    }
    if (0 == lz && (chunk = GetChunkByChunksPos(cx, cy, cz - 1)))
    {
        chunk->SetModified(true);
    }

    if ((ChunkWidth - 1) == lx && (chunk = GetChunkByChunksPos(cx + 1, cy, cz)))
    {
        chunk->SetModified(true);
    }
    if ((ChunkHeight - 1) == ly && (chunk = GetChunkByChunksPos(cx, cy + 1, cz)))
    {
        chunk->SetModified(true);
    }
    if ((ChunkDepth - 1) == lz && (chunk = GetChunkByChunksPos(cx, cy, cz + 1)))
    {
        chunk->SetModified(true);
    }
}

std::shared_ptr<Chunk> Chunks::GetChunkByCoord(uint32_t x, uint32_t y, uint32_t z)
{
    if (x < 0 || y < 0 || z < 0 || x >= w_ || y >= h_ || z >= d_)
    {
        return nullptr;
    }

    const uint32_t idx = (y * d_ + z) * w_ + x;

    return chunks_[idx];
}

Chunks::NullableReference<voxels::voxel> Chunks::RayCast(glm::vec3 a, glm::vec3 dir, float maxDist,
                                                         glm::vec3 &end, glm::vec3 &norm,
                                                         glm::vec3 &iend)
{
    const float px = a.x;
    const float py = a.y;
    const float pz = a.z;

    const float dx = dir.x;
    const float dy = dir.y;
    const float dz = dir.z;

    float t = 0.0f;
    int ix = floor(px);
    int iy = floor(py);
    int iz = floor(pz);

    const float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
    const float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
    const float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

    const float infinity = std::numeric_limits<float>::infinity();

    const float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
    const float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
    const float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

    const float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
    const float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
    const float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

    float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
    float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
    float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

    int steppedIndex = -1;

    while (t <= maxDist)
    {
        auto vox = GetVoxelByCoord(ix, iy, iz);
        if (vox || BlockType::None == vox->get().id)
        {
            end.x = px + t * dx;
            end.y = py + t * dy;
            end.z = pz + t * dz;

            iend.x = ix;
            iend.y = iy;
            iend.z = iz;

            norm.x = norm.y = norm.z = 0.0f;

            if (steppedIndex == 0)
            {
                norm.x = -stepx;
            }
            if (steppedIndex == 1)
            {
                norm.y = -stepy;
            }
            if (steppedIndex == 2)
            {
                norm.z = -stepz;
            }

            return vox;
        }
        if (txMax < tyMax)
        {
            if (txMax < tzMax)
            {
                ix += stepx;
                t = txMax;
                txMax += txDelta;
                steppedIndex = 0;
            }
            else
            {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        }
        else
        {
            if (tyMax < tzMax)
            {
                iy += stepy;
                t = tyMax;
                tyMax += tyDelta;
                steppedIndex = 1;
            }
            else
            {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        }
    }

    iend.x = ix;
    iend.y = iy;
    iend.z = iz;

    end.x = px + t * dx;
    end.y = py + t * dy;
    end.z = pz + t * dz;
    norm.x = norm.y = norm.z = 0.0f;

    return {};
}

} // namespace voxels
