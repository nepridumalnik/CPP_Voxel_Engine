#include <voxels/Chunk.hpp>

#include <voxels/voxel.hpp>

#include <cmath>

namespace voxels
{

Chunk::Chunk()
{
    for (int y = 0; y < ChunkHeight; ++y)
    {
        for (int z = 0; z < ChunkDepth; ++z)
        {
            for (int x = 0; x < ChunkWidth; ++x)
            {
                int id = y <= (sin(x * 0.3f) * 0.5f + 0.5f) * 10;
                if (y <= 2)
                    id = 2;
                voxels[(y * ChunkDepth + z) * ChunkWidth + x].id = id;
            }
        }
    }
}

voxel &voxels::Chunk::operator[](uint32_t idx)
{
    return voxels.at(idx);
}

} // namespace voxels
