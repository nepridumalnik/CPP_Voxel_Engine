#include <voxels/Chunk.hpp>

#include <stdexcept>
#include <string>

namespace voxels
{

Chunk::Chunk()
{
    for (auto &x : voxels_)
    {
        for (auto &y : x)
        {
            for (uint32_t z = 0; z < ChunkDepth; ++z)
            {
                if (0 == z)
                {
                    y[z].id = VoxelType::Grass;
                }
                else
                {
                    y[z].id = VoxelType::None;
                }
            }
        }
    }
}

std::array<std::array<voxels::voxel, ChunkWidth>, ChunkHeight> &voxels::Chunk::operator[](
    uint32_t idx)
{
    return voxels_[idx];
}

} // namespace voxels