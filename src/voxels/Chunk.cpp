#include <voxels/Chunk.hpp>

#include <voxels/voxel.hpp>

#include <cmath>

namespace voxels
{

Chunk::Chunk()
{
    for (int32_t y = 0; y < ChunkHeight; ++y)
    {
        for (int32_t z = 0; z < ChunkDepth; ++z)
        {
            for (int32_t x = 0; x < ChunkWidth; ++x)
            {
                int32_t id = y <= (sin(x * 0.3f) * 0.5f + 0.5f) * 10;
                if (y <= voxels::BlockType::Last)
                    id = voxels::BlockType::Last;
                voxels_[(y * ChunkDepth + z) * ChunkWidth + x].id =
                    static_cast<voxels::BlockType>(id);
            }
        }
    }
}

voxel &voxels::Chunk::operator[](uint32_t idx)
{
    return voxels_.at(idx);
}

voxel &Chunk::GetVoxel(int32_t x, int32_t y, int32_t z)
{
    const uint32_t idx = (y * voxels::ChunkDepth + z) * voxels::ChunkWidth + x;
    return voxels_.at(idx);
}

} // namespace voxels
